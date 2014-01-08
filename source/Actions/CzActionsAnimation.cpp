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

#include "CzActionsAnimation.h"
#include "CzHashes.h"
#include "CzApp.h"
#include "CzScene.h"
#include "CzActor.h"

/**
 @fn	void CzXomlActions_Animation::Execute(IzXomlResource* source, CzAction* action)

 @brief	The main actions executor for animations. All animation specific actions are implemented by this method

 @param [in]	source - The source of the action
 @param [in]	action - The action to execute
 */
void CzXomlActions_Animation::Execute(IzXomlResource* source, CzAction* action)
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
	case Action_PlayTimeline:
	case Action_StopTimeline:
	case Action_PauseTimeline:
	case Action_SetTimeline:
		{
			CzAnimTimeline* timeline = NULL;
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
					timeline = CzAnimTimelinesManager::FindTimeline(action->getParameter1(cont).getHash(), actor);
				else
					timeline = CzAnimTimelinesManager::FindTimeline(action->getParameter1(cont).getHash(), scene);
			}

			if (actor != NULL)
			{
				if (timeline == NULL)
				{
					if (Type == Action_SetTimeline)
					{
						actor->setTimeline(NULL);
						return;
					}
					else
						timeline = actor->getTimeline();
				}
				if (timeline == NULL)
				{
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - Actor timeline was not found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
					return;
				}
				if (Type == Action_SetTimeline)
				{
					actor->setTimeline(timeline);
					timeline->restart();
				}
			}
			else
			if (scene != NULL)
			{
				if (timeline == NULL)
				{
					if (Type == Action_SetTimeline)
					{
						scene->setTimeline(NULL);
						return;
					}
					else
						timeline = scene->getTimeline();
				}
				if (timeline == NULL)
				{
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - Scene timeline was not found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
					return;
				}
				if (Type == Action_SetTimeline)
				{
					scene->setTimeline(timeline);
					timeline->restart();
				}
			}
			else
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - No valid target scene or actor - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
				return;
			}

			if (Type == Action_PlayTimeline)
			{
				if (timeline->isStopped())
					timeline->restart();
				else
					timeline->play();
			}
			else
			if (Type == Action_StopTimeline)
				timeline->stop();
			else
			if (Type == Action_PauseTimeline)
				timeline->pause();
		}
		break;
	}
}
