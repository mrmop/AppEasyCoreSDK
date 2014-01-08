// 
//
// AppEasy SDK - Cross Platform Multi-purpose Game and App Engine
//
// Developed by Matthew Hopwood of Pocketeers Limited - www.pocketeers.co.uk
//
// For updates, tutorials and more details check out www.appeasymobile.com
//
// This code is provided free of charge and without any warranty whatsoever. You must ensure that this whole notice is present in all files and derivatives, so the reader knows its origin.
// If you use this SDK in your product then please ensure that you credit AppEasy's usage appropriately. Please see www.appeasymobile.com for licensing details and support
//
//

#include "CzTimer.h"
#include "CzHashes.h"
#include "CzActions.h"
#include "CzScene.h"
#include "CzActor.h"

CzXomlTimer::~CzXomlTimer()
{
	// Clean-up managers
	SAFE_DELETE(EventsManager)
}

/**
 @fn	bool CzXomlTimer::Update(float dt)

 @brief	Updates the timer.

 @param	dt	The number of seconds since physics was last updated

 @return	false if the time should be deleted.
 */

bool CzXomlTimer::Update(float dt)
{
	if (!Running)
		return true;

	CurrentTime += dt;
	if (CurrentTime >= Duration)
	{
		if (RepeatCount > 0)
		{
			RepeatCount--;
			if (RepeatCount == 0)
			{
				NotifyTimeout();
				Running = false;
				if (AutoDelete)
					return false;	// Remove timer from list
			}
			else
			{
				NotifyRepeat();
				while (CurrentTime >= Duration)
					CurrentTime -= Duration;
			}
		}
		else
		{
			NotifyRepeat();
			while (CurrentTime >= Duration)
				CurrentTime -= Duration;
		}
	}

	return true;
}

void CzXomlTimer::addEventsManager()
{
	if (EventsManager == NULL)
		EventsManager = new CzEventManager();
}


/**
 @fn	int CzXomlTimer::LoadFromXoml(IzXomlResource* parent, bool load_children,
    CzXmlNode* node)

 @brief	Creates an instance of this class from XOML.

 LoadFromXoml is the main method used by all classes that can be instantiated from XOML mark-up. This method creates an instance of this class from the supplied XML node structure specified by node. 
 
 @param [in]	parent		If non-null, the parent.
 @param	load_children	  	true to load child nodes.
 @param [in]	node  		If non-null, the XOML markup node that defines this object

 @return	nergative value for error. if 0 then processing of further XOML will stop.
 */

int CzXomlTimer::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Process animation attributes
	bool local = true;

	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
			setName((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::AutoStart_Hash)
		{
			if (!(*it)->getValueAsBool())
				stop();
		}
		else
		if (name_hash == CzHashes::Duration_Hash)
			Duration = (*it)->getValueAsFloat();
		else
		if (name_hash == CzHashes::Repeat_Hash)
		{
			RepeatCount = (*it)->getValueAsInt();
			OriginalRepeatCount = RepeatCount;
		}
		else
		if (name_hash == CzHashes::AutoDelete_Hash)
			AutoDelete = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::OnTimeout_Hash)
		{
			addEventsManager();
			EventsManager->addEvent("OnTimeout", (*it)->getValue().c_str(), true);
		}
		else
		if (name_hash == CzHashes::OnRepeat_Hash)
		{
			addEventsManager();
			EventsManager->addEvent("OnRepeat", (*it)->getValue().c_str(), true);
		}
		else
		if (name_hash == CzHashes::Local_Hash)
			local = (*it)->getValueAsBool();
	}

	CzScene* scene = NULL;
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Actor_Hash)
		scene = ((CzActor*)parent)->getScene();
	else
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

	// If we are declared inside a scene or actor then actions are local to the scene
	if (parent != NULL && (parent->getClassTypeHash() == CzHashes::Scene_Hash || parent->getClassTypeHash() == CzHashes::Actor_Hash))
	{
		if (parent->getClassTypeHash() == CzHashes::Scene_Hash)
			((CzScene*)parent)->getTimersManager()->addTimer(this);
		else
		{
			if (local)
				((CzActor*)parent)->getTimersManager()->addTimer(this);
			else
				((CzActor*)parent)->getScene()->getTimersManager()->addTimer(this);
		}
	}
	else
	{
		CZ_GLOBAL_RESOURCES->getTimersManager()->addTimer(this);
	}

	return 1;
}


/**
 @fn	void CzXomlTimer::ProcessEventActions(unsigned int event_name)

 @brief	Process the event actions described by event_name.

 Processes the actions list that is associated with the event named event_name (event_name is a hash of the actual event name to speed up searches)

 @param	event_name	Name of the event as a string hash
 */

void CzXomlTimer::ProcessEventActions(unsigned int event_name)
{
	// Find the event 
	CzEvent* evt = EventsManager->findEvent(event_name);
	if (evt != NULL)
	{
		// Find actions list
		CzActions* actions = CzActionsManager::FindActions(evt->ActionsName, Container);
		if (actions != NULL)
			actions->Execute(Container);
	}
}

void CzXomlTimer::NotifyTimeout()
{
	ProcessEventActions(CzHashes::OnTimeout_Hash);
}

void CzXomlTimer::NotifyRepeat()
{
	ProcessEventActions(CzHashes::OnRepeat_Hash);
}

// 
// 
//
//
// CzTimersManager implementation
//
//
//
//

/**
 @fn	void CzTimersManager::addTimer(CzXomlTimer* Timer)

 @brief	Adds a Timer to the Timers manager.

 @param [in,out]	Timers	If non-null, the Timer.
 */

void CzTimersManager::addTimer(CzXomlTimer* Timer)
{
#if defined(CZ_REMOVE_DUPLICATES)
	CzXomlTimer* tl = findTimer(Timer->getNameHash());
	if (tl != NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Timer - Already exists, Timer was replaced - ", Timer->getName().c_str());
		removeTimer(tl);
	}
#endif
	Timers.push_back(Timer);
	Timer->setManaged(true);
}

/**
 @fn	void CzTimersManager::removeTimer(CzXomlTimer* Timer)

 @brief	Removes the Timer from the Timers manager.

 @param [in,out]	Timer	If non-null, the Timer.
 */

void CzTimersManager::removeTimer(CzXomlTimer* Timer)
{
	removeTimer(Timer->getNameHash());
}

/**
 @fn	void CzTimersManager::removeTimer(unsigned int name_hash)

 @brief	Removes the named Timer from the Timers manager.

 @param	name_hash	The Timer name as a string hash.
 */

void CzTimersManager::removeTimer(unsigned int name_hash)
{
	// Renove named Timer from the manager
	for (_Iterator it = Timers.begin(); it != Timers.end(); ++it)
	{
		if (name_hash == (*it)->getNameHash())
		{
			delete *it;
			Timers.erase(it);
			break;
		}
	}
}

/**
 @fn	CzXomlTimer* CzTimersManager::findTimer(unsigned int name_hash)

 @brief	Searches for the manager for the named Timer.

 @param	name_hash	The Timer name as a string hash.

 @return	null if it not found, else the found Timer.
 */

CzXomlTimer* CzTimersManager::findTimer(unsigned int name_hash)
{
	for (_Iterator it = Timers.begin(); it != Timers.end(); ++it)
	{
		if ((*it)->getNameHash() == name_hash)
			return *it;
	}

	return NULL;
}

/**
 @fn	CzXomlTimer* CzTimersManager::findTimer(const char* name)

 @brief	Searches for the manager for the named Timer.

 @param	name	The Timer name.

 @return	null if it not found, else the found Timer.
 */

CzXomlTimer* CzTimersManager::findTimer(const char* name)
{
	return findTimer(CzString::CalculateHash(name));
}

/**
 @fn	void CzTimersManager::clearTimers()

 @brief	Clears and destroys all the Timers in the manager.

 */

void CzTimersManager::clearTimers()
{
	// Remove and delete all Timers from the manager
	for (_Iterator it = Timers.begin(); it != Timers.end(); ++it)
		delete *it;
	Timers.clear();
}

/**
 @fn	CzXomlTimer* CzTimersManager::FindTimer(const char* name,
    IzXomlResource* container)

 @brief	Static method that searches for the named Timer.

 If the Timer is not found in the supplied conntainer then the global Timers manager will be searched.

 @param	name				 	The Timer name.
 @param [in]	container	If non-null, the container.

 @return	null if not found, else the found Timer.
 */

CzXomlTimer* CzTimersManager::FindTimer(const char* name, IzXomlResource* container)
{
	return FindTimer(CZ_HASH(name), container);
}

/**
 @fn	CzXomlTimer* CzTimersManager::FindTimer(unsigned int name_hash,
    IzXomlResource* container)

 @brief	Static method that searches for the named Timer.

 If the Timer is not found in the supplied conntainer then the global Timers manager will be searched.

 @param	name				 	The Timer name as a string hash.
 @param [in]	container	If non-null, the container.

 @return	null if not found, else the found Timer.
 */

CzXomlTimer* CzTimersManager::FindTimer(unsigned int name_hash, IzXomlResource* container)
{
	CzXomlTimer* Timer = NULL;
	
	if (container != NULL)
	{
		// If container class is an actor then check the actors scenes Timers manager
		if (container->getClassTypeHash() == CzHashes::Actor_Hash)
		{
			Timer = ((CzActor*)container)->getTimersManager()->findTimer(name_hash);

			// If not found in the actor then check the actors scene
			if (Timer == NULL)
				container = ((CzActor*)container)->getScene();
		}

		// If container class is a scene then check the scenes Timers manager
		if (Timer == NULL && container->getClassTypeHash() == CzHashes::Scene_Hash)
			Timer = ((CzScene*)container)->getTimersManager()->findTimer((name_hash));
	}

	// If the Timers has no container or the Timers was not found in the supplied container then check the global Timers manager
	if (Timer == NULL)
		Timer = (CzXomlTimer*)CZ_GLOBAL_RESOURCES->getTimersManager()->findTimer((name_hash));

	return Timer;
}

/**
 @fn	CzXomlTimer* CzTimersManager::FindTimer(const char* name, CzScene* container,
    const char* actor_name)

 @brief	Static method that searches for the named Timer.

 Searches the supplied actors Timers manager for the named Timer, if supplied actor not valid then the supplied scene will be searched instead. If the Timer is 
 not found in the scenes Timers manager then the global Timers manager will be searched. 

 @param	name				 	The Timer name.
 @param [in]	container	If non-null, the container.
 @param	actor_name			 	Name of the actor.

 @return	null if not found, else the found Timer.
 */

CzXomlTimer*	CzTimersManager::FindTimer(const char* name, CzScene* container, const char* actor_name)
{
	return FindTimer(CZ_HASH(name), container, CZ_HASH(actor_name));
}

/**
 @fn	CzTimer* CzTimersManager::FindTimer(unsigned int name_hash,
    CzScene* scene, unsigned int actor_hash)

 @brief	Static method that searches for the named Timer.

 Searches the supplied actors Timers manager for the named Timer, if supplied actor not valid then the supplied scene will be searched instead. If the Timer is 
 not found in the scenes Timers manager then the global Timers manager will be searched. 

 @param	name_hash			 	The Timer name as a string hash.
 @param [in]	container	If non-null, the container.
 @param	actor_name			 	Name of the actor.

 @return	null if not found, else the found Timer.
 */

CzXomlTimer* CzTimersManager::FindTimer(unsigned int name_hash, CzScene* scene, unsigned int actor_hash)
{
	CzXomlTimer* Timer = NULL;
	
	if (scene != NULL)
	{
		CzActor* actor = scene->findActor(actor_hash);
		if (actor != NULL)
			Timer = actor->getTimersManager()->findTimer((name_hash));
		else
			Timer = scene->getTimersManager()->findTimer((name_hash));
	}

	if (Timer == NULL)
		Timer = (CzXomlTimer*)CZ_GLOBAL_RESOURCES->getTimersManager()->findTimer((name_hash));

	return Timer;
}


void CzTimersManager::Update(float dt)
{
	for (_Iterator it = Timers.begin(); it != Timers.end(); ++it)
	{
		if (!(*it)->Update(dt))
			Removals.push_back(*it);
	}

	for (_Iterator it = Removals.begin(); it != Removals.end(); ++it)
	{
		delete *it;
		Timers.remove(*it);
	}
	Removals.clear();
}

