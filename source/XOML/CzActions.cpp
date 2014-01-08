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

#include "CzActions.h"
#include "CzScene.h"
#include "CzResources.h"
#include "CzActor.h"
#include "CzApp.h"
#include "CzHashes.h"

//#define _DEBUG_ACTIONS

//
//
//
//
// CzActions implementation
//
//
//
//
CzString& CzAction::getParameter1(IzXomlResource* parent)
{
//	if (Variables[0] == NULL)
//		return Params[0];

	CzXomlVariable* var = CzXomlVariable::GetVariable(Params[0], parent);
	if (var != NULL)
	{
		if (!var->isArray())
			return var->getValue();
	}

	return Params[0];
}
CzString& CzAction::getParameter2(IzXomlResource* parent)
{
	CzXomlVariable* var = CzXomlVariable::GetVariable(Params[1], parent);
	if (var != NULL)
	{
		if (!var->isArray())
			return var->getValue();
	}

	return Params[1];
}
CzString& CzAction::getParameter3(IzXomlResource* parent)
{
	CzXomlVariable* var = CzXomlVariable::GetVariable(Params[2], parent);
	if (var != NULL)
	{
		if (!var->isArray())
			return var->getValue();
	}

	return Params[2];
}
CzString& CzAction::getParameter4(IzXomlResource* parent)
{
	CzXomlVariable* var = CzXomlVariable::GetVariable(Params[3], parent);
	if (var != NULL)
	{
		if (!var->isArray())
			return var->getValue();
	}

	return Params[3];
}
CzString& CzAction::getParameter5(IzXomlResource* parent)
{
	CzXomlVariable* var = CzXomlVariable::GetVariable(Params[4], parent);
	if (var != NULL)
	{
		if (!var->isArray())
			return var->getValue();
	}

	return Params[4];
}

//
// 
//
// CzActions implementation
//
// 
// 
void CzActions::addAction(CzAction* action)
{
	Actions.push_back(action);
	action->Parent = this;
}

void CzActions::removeAction(CzAction* action, bool auto_delete)
{
	Actions.remove(action);
	if (auto_delete)
		delete action;
}

void CzActions::Execute(IzXomlResource* source)
{
#if defined (_DEBUG_ACTIONS)
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Actions - Running action group - ", Name.c_str(), DebugInfo.c_str());
#endif // _DEBUG
	if (source == NULL)
		source = getContainer();
	if (source == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Actions - Action group has no source - ", Name.c_str(), DebugInfo.c_str());
		return;
	}

	if (Conditions != 0)
	{
		// Find the condition variable
		CzXomlVariable* var = CzXomlVariable::GetVariable(_Conditions, source);
		if (var != NULL)
		{
			bool res = var->isTrue();
			if (Not)
				res = !res;
			if (!res)
				return;
		}
#if defined (_DEBUG)
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actions - Condition variable not found - ", _Conditions.c_str(), DebugInfo.c_str());
#endif // _DEBUG
	}

	for (_Iterator it = begin(); it != end(); ++it)
	{
		CzAction*		action = *it;
		unsigned int	method_name = action->getMethodName();
		bool			condition = true;

#if defined (_DEBUG_ACTIONS)
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Actions - Running action - ", action->_MethodName.c_str(), DebugInfo.c_str());
#endif // _DEBUG

		// If action has a condition then check that the condition is true before executing the action
		if ((*it)->getConditions() != 0)
		{
			// Find the condition variable
			CzXomlVariable* var = CzXomlVariable::GetVariable((*it)->_Conditions, source);

			if (var != NULL)
			{
				condition = var->isTrue();
				if (action->Not)
					condition = !condition;
			}
#if defined (_DEBUG)
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Actions - Condition variable not found - ", (*it)->_Conditions.c_str(), DebugInfo.c_str());
#endif // _DEBUG
		}
		if (condition)
		{
			CzXoml::_ActionsIterator it2 = CZ_XOML->actions_begin();
			// Search for action
			for (; it2 != CZ_XOML->actions_end(); ++it2)
			{
				if ((*it)->getMethodName() == (*it2)->getActionNameHash())
				{
					// Execute action
					(*it2)->Execute(source, *it);
					break;
				}
			}
		}
	}
}

int CzActions::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
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
		if (name_hash == CzHashes::Condition_Hash)
		{
			if (*((*it)->getValue().c_str()) == '!')
			{
				Not = true;
				_Conditions = (*it)->getValue().c_str() + 1;
				Conditions = _Conditions.getHash();
			}
			else
				_Conditions = (*it)->getValue().c_str();
			Conditions = _Conditions.getHash();
		}
		else
		if (name_hash == CzHashes::Local_Hash)
			local = (*it)->getValueAsBool();
	}

	// Load in nodes
	for (CzXmlNode::_Iterator it2 = node->begin(); it2 != node->end(); ++it2)
	{
		unsigned int name_hash = (*it2)->GetName().getHash();
		if (name_hash == CzHashes::Action_Hash)
		{
			CzAction* action = new CzAction();
			// Get action data
			for (CzXmlNode::_AttribIterator it = (*it2)->attribs_begin(); it != (*it2)->attribs_end(); ++it)
			{
				unsigned int attrib_hash = (*it)->getName().getHash();

				if (attrib_hash == CzHashes::Method_Hash)
				{
					(*it)->getValue().ToLower();
					action->setMethodName((*it)->getValue().c_str());
				}
				else
				if (attrib_hash == CzHashes::Param1_Hash || attrib_hash == CzHashes::P1_Hash)
					action->setParameter1((*it)->getValue());
				else
				if (attrib_hash == CzHashes::Param2_Hash || attrib_hash == CzHashes::P2_Hash)
					action->setParameter2((*it)->getValue());
				else
				if (attrib_hash == CzHashes::Param3_Hash || attrib_hash == CzHashes::P3_Hash)
					action->setParameter3((*it)->getValue());
				else
				if (attrib_hash == CzHashes::Param4_Hash || attrib_hash == CzHashes::P4_Hash)
					action->setParameter4((*it)->getValue());
				else
				if (attrib_hash == CzHashes::Param5_Hash || attrib_hash == CzHashes::P5_Hash)
					action->setParameter5((*it)->getValue());
				else
				if (attrib_hash == CzHashes::Condition_Hash)
				{
					if (*((*it)->getValue().c_str()) == '!')
					{
						action->Not = true;
						action->setConditions((*it)->getValue().c_str() + 1);
					}
					else
						action->setConditions((*it)->getValue().c_str());
				}
			}
			// Save action
			addAction(action);
		}
	}

	// If we are declared inside a scene or actor then actions are local to the scene
	if (parent != NULL && (parent->getClassTypeHash() == CzHashes::Scene_Hash || parent->getClassTypeHash() == CzHashes::Actor_Hash))
	{
		if (parent->getClassTypeHash() == CzHashes::Scene_Hash)
			((CzScene*)parent)->getActionsManager()->addActions(this);
		else
		{
			if (local)
				((CzActor*)parent)->getActionsManager()->addActions(this);
			else
				((CzActor*)parent)->getScene()->getActionsManager()->addActions(this);
		}
	}
	else
	{
		CzActionsManager* manager = CZ_GLOBAL_RESOURCES->getActionsManager();
		manager->addActions(this);
	}

	return 1;
}


// 
// 
//
//
// CzActionsManager implementation
//
//
//
//

void CzActionsManager::addActions(CzActions* actions)
{
#if defined(CZ_REMOVE_DUPLICATES)
	CzActions* acts = findActions(actions->getNameHash());
	if (acts != NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actions - Already exists, actions list was replaced - ", actions->getName().c_str());
		removeActions(acts);
	}
#endif

	Actions.push_back(actions);
}

void CzActionsManager::removeActions(CzActions* actions)
{
	removeActions(actions->getNameHash());
}

void CzActionsManager::removeActions(unsigned int name_hash)
{
	// Renove named Actions from the manager
	for (_Iterator it = Actions.begin(); it != Actions.end(); ++it)
	{
		if (name_hash == (*it)->getNameHash())
		{
			delete *it;
			Actions.erase(it);
			break;
		}
	}
}

CzActions* CzActionsManager::findActions(unsigned int name_hash)
{
	for (_Iterator it = Actions.begin(); it != Actions.end(); ++it)
	{
		if ((*it)->getNameHash() == name_hash)
			return *it;
	}

	return NULL;
}

CzActions* CzActionsManager::findActions(const char* name)
{
	return findActions(CzString::CalculateHash(name));
}

void CzActionsManager::clearActionss()
{
	// Remove and delete all actions from the manager
	for (_Iterator it = Actions.begin(); it != Actions.end(); ++it)
		delete *it;
	Actions.clear();
}

CzActions* CzActionsManager::FindActions(const char* name, IzXomlResource* container)
{
	return FindActions(CZ_HASH(name), container);
}

CzActions* CzActionsManager::FindActions(unsigned int name_hash, IzXomlResource* container)
{
	CzActions* action = NULL;
	
	if (container != NULL)
	{
		// If container class is an actor then check the actors scenes actions manager
		if (container->getClassTypeHash() == CzHashes::Actor_Hash)
		{
			action = ((CzActor*)container)->getActionsManager()->findActions((name_hash));

			// If not found in the actor then check the actors scene
			if (action == NULL)
				container = ((CzActor*)container)->getScene();
		}

		// If container class is a scene then check the scenes actions manager
		if (action == NULL && container->getClassTypeHash() == CzHashes::Scene_Hash)
			action = ((CzScene*)container)->getActionsManager()->findActions((name_hash));
	}

	// If the actions has no container or the actions was not found in the supplied container then check the global actions manager
	if (action == NULL)
		action = (CzActions*)CZ_GLOBAL_RESOURCES->getActionsManager()->findActions((name_hash));

	return action;
}

CzActions*	CzActionsManager::FindActions(const char* name, CzScene* container, const char* actor_name)
{
	return FindActions(CZ_HASH(name), container, CZ_HASH(actor_name));
}

CzActions* CzActionsManager::FindActions(unsigned int name_hash, CzScene* scene, unsigned int actor_hash)
{
	CzActions* action = NULL;
	
	if (scene != NULL)
	{
		CzActor* actor = scene->findActor(actor_hash);
		if (actor != NULL)
			action = actor->getActionsManager()->findActions((name_hash));
		else
			action = scene->getActionsManager()->findActions((name_hash));
	}

	if (action == NULL)
		action = (CzActions*)CZ_GLOBAL_RESOURCES->getActionsManager()->findActions((name_hash));

	return action;
}

