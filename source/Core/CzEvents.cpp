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

#include "CzEvents.h"

//
//
//
//
// CzEvents implementation
//
//
//
//

CzEventManager::~CzEventManager()
{
	for (_Iterator it = begin(); it != end(); ++it)
	{
		delete *it;
	}
	Events.clear();
}

void CzEventManager::addEvent(const char* event_name, const char* actions_name, bool replace)
{
	CzEvent* evt = NULL;
	if (replace)
	{
		evt = findEvent(CZ_HASH(event_name));
		if (evt != NULL)
		{
			if (actions_name == NULL || *actions_name == 0)
			{
				delete evt;
				Events.remove(evt);
			}
			else
			{
				evt->EventName = CZ_HASH(event_name);
				evt->ActionsName = CZ_HASH(actions_name);
#if defined(_DEBUG)
				evt->_EventName = event_name;
				evt->_ActionsName = actions_name;
#endif	//_DEBUG
			}
			return;
		}
	}
	evt = new CzEvent();
	evt->EventName = CZ_HASH(event_name);
	evt->ActionsName = CZ_HASH(actions_name);
#if defined(_DEBUG)
	evt->_EventName = event_name;
	evt->_ActionsName = actions_name;
#endif	//_DEBUG
	Events.push_back(evt);
}

void CzEventManager::addEvent(CzEvent* evt)
{
	Events.push_back(evt);
}

CzEvent* CzEventManager::findEvent(unsigned int event_name_hash)
{
	for (_Iterator it = begin(); it != end(); ++it)
	{
		CzEvent* evt = (*it);
		if (evt->EventName == event_name_hash)
			return evt;
	}

	return NULL;
}