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

#include "CzActionsTimer.h"
#include "CzApp.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"
#include "CzTimer.h"

/**
 @fn	void CzXomlActions_Timer::Execute(IzXomlResource* source, CzAction* action)

 @brief	The main actions executor for timer related actions. All timer specific actions such as ChangeTimer are implemented by this method

 @param [in]	source - The source of the action
 @param [in]	action - The action to execute
 */
void CzXomlActions_Timer::Execute(IzXomlResource* source, CzAction* action)
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
	case Action_ChangeTimer:
		{
			CzXomlTimer* timer = NULL;
			if (!action->Params[1].isEmpty())
			{
				// If user provided second parameter then take that as a target actor or scene
				IzXomlResource* other = scene->findActor(action->getParameter2(cont).getHash());
				if (other == NULL)
				{
					actor = NULL;
					scene = game->findScene(action->getParameter2(cont).getHash());
				}
				else
				{
					actor = (CzActor*)other;
					scene = actor->getScene();
				}
			}
			if (!action->Params[0].isEmpty())
			{
				if (actor != NULL)
					timer = CzTimersManager::FindTimer(action->getParameter1(cont).getHash(), actor);
				else
					timer = CzTimersManager::FindTimer(action->getParameter1(cont).getHash(), scene);
			}

			if (timer == NULL)
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action ChangeTimer - Timer not founf - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
				return;
			}

			unsigned int command_hash = action->getParameter3(cont).getHash();
			if (command_hash == CzHashes::Start_Hash)
				timer->start();
			else
			if (command_hash == CzHashes::Restart_Hash)
				timer->restart();
			else
			if (command_hash == CzHashes::Stop_Hash)
				timer->stop();

		}
		break;
	}
}
