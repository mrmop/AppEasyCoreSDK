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

#include "CzActionsComms.h"
#include "CzHashes.h"
#include "CzApp.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzRemoteReq.h"
#include "CzXomlResourceManager.h"

/**
 @fn	void CzXomlActions_Comms::Execute(IzXomlResource* source, CzAction* action)

 @brief	The main actions executor for communication related actions. All communication specific actions are implemented by this method

 @param [in]	source - The source of the action
 @param [in]	action - The action to execute
 */
void CzXomlActions_Comms::Execute(IzXomlResource* source, CzAction* action)
{
	if (source == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Source was not found for action - ", action->_MethodName.c_str(), action->Parent->getDebugInfo().c_str());
#endif
		return;
	}

	// Determine the container
	CzApp* game;
	CzScene* scene;
	CzActor* actor;
	IzXomlResource* cont = IzXomlAction::getContainer(source, game, scene, actor);

	if (Type == Action_RemoteReq)
	{
		CzString* p1 = NULL;
		CzString* p2 = NULL;
		if (action->Params[0].isEmpty())
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Action - RemoteReq - requires Remote Request resource name as Param1", action->Parent->getDebugInfo().c_str());
			return;
		}
		p1 = &action->getParameter1(cont);
		CzRemoteReq* request = (CzRemoteReq*)scene->getResourceManager()->findResource(p1->c_str(), CzHashes::RemoteReq_Hash);
		if (request == NULL)
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - RemoteReq - Could not find named remote request resource in the scene or in the global resources - ", p1->c_str(), action->Parent->getDebugInfo().c_str());
			return;
		}
		if (!action->Params[1].isEmpty())
		{
			p2 = &action->getParameter2(cont);
			request->setData(p2->c_str());
		}

		request->MakeRequest();
		return;
	}
}


