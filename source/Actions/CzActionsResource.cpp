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

#include "CzActionsResource.h"
#include "CzApp.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzMath.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"

/**
 @fn	void CzXomlActions_Resource::Execute(IzXomlResource* source, CzAction* action)

 @brief	The main actions executor for resource related actions. All resource specific actions such as RemoveResource are implemented by this method

 @param [in]	source - The source of the action
 @param [in]	action - The action to execute
 */
void CzXomlActions_Resource::Execute(IzXomlResource* source, CzAction* action)
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

	switch (Type)
	{
	case Action_RemoveResource:		// Removes a specific resource (name, type)
		{
			if (!action->Params[0].isEmpty())
			{
				// Remove from global resource manager
				IzXomlResource* res = CZ_GLOBAL_RESOURCES->getResourceManager()->findResource(action->getParameter1(cont).c_str(), action->getParameter2(cont).c_str(), false);
				if (res != NULL)
					CZ_GLOBAL_RESOURCES->getResourceManager()->removeResource(res);
			}
		}
		break;
	case Action_RemoveResources:	// Removes a collection of tagged resources (tag)
		{
			if (!action->Params[0].isEmpty())
			{
				// Remove from global resource manager
				CZ_GLOBAL_RESOURCES->getResourceManager()->removeTaggedResources(action->getParameter1(cont).getHash());
			}
		}
		break;
	}
}
