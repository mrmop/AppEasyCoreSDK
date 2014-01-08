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

#include "CzActionsScene.h"
#include "CzHashes.h"
#include "CzApp.h"
#include "CzScene.h"
#include "CzActor.h"

/**
 @fn	void CzXomlActions_Scene::Execute(IzXomlResource* source, CzAction* action)

 @brief	The main actions executor for scene related actions. All scene specific actions such as SetCurrentScene are implemented by this method

 @param [in]	source - The source of the action
 @param [in]	action - The action to execute
 */
void CzXomlActions_Scene::Execute(IzXomlResource* source, CzAction* action)
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
	case Action_CallActions:
		{
			if (!action->Params[2].isEmpty())
			{
				CzScene* other_scene = game->findScene(action->getParameter3(cont).getHash());
				if (other_scene == NULL)
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - Container scene was not found - ", action->getParameter3(cont).c_str(), action->Parent->getDebugInfo().c_str());
				else
					scene = other_scene;
				if (action->Params[1].isEmpty())	// If actor not supplied and scene was then NULL the actor so that the scene can be chosen instead
					actor = NULL;
			}
			if (scene == NULL)
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - CallActions - Needs to be called from inside a scene", action->Parent->getDebugInfo().c_str());
				return;
			}
			CzActor* other_actor = NULL;
			if (!action->Params[1].isEmpty())
			{
				CzActor* other_actor = scene->findActor(action->getParameter2(cont).getHash());
				if (other_actor == NULL)
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - Container actor was not found - ", action->getParameter2(cont).c_str(), action->Parent->getDebugInfo().c_str());
				else
					other_actor = actor;
			}
			else
				other_actor = actor;

			CzActions* actions = CzActionsManager::FindActions(action->getParameter1(cont).getHash(), other_actor != NULL ? (IzXomlResource*)other_actor : (IzXomlResource*)scene);
			if (actions != NULL)
				actions->Execute(scene);
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - CallActions - Actions not found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
		}
		break;
	case Action_SetCurrentScene:
		{
			CzScene* scene = game->findScene(action->getParameter1().getHash());
			if (scene != NULL)
			{
				game->changeScene(scene);
				game->BringSceneToFront(scene);
			}
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - SetCurrentScene - scene not found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
		}
		break;
	case Action_BringtSceneToFront:
		{
			CzScene* scene = game->findScene(action->getParameter1().getHash());
			if (scene != NULL)
				game->BringSceneToFront(scene);
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - BringtSceneToFront - scene not found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
		}
		break;
	case Action_ChangeScene:
		game->changeScene(action->getParameter1(cont).getHash());
		break;
	case Action_SuspendScene:
		{
			CzScene* new_scene = action->Params[0].isEmpty() ? scene : game->findScene(action->getParameter1(cont).getHash());
			if (new_scene == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - SuspendScene - Scene was not found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
			else
				new_scene->Suspend(NULL);
		}
		break;
	case Action_ResumeScene:
		{
			CzScene* new_scene = action->Params[0].isEmpty() ? scene : game->findScene(action->getParameter1(cont).getHash());
			if (new_scene == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - ResumeScene - Scene was not found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
			else
				new_scene->Resume(NULL);
		}
		break;
	case Action_HideScene:
		{
			CzScene* new_scene = action->Params[0].isEmpty() ? scene : game->findScene(action->getParameter1(cont).getHash());
			if (new_scene == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - HideScene - Scene was not found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
			else
				new_scene->setVisible(false);
		}
		break;
	case Action_ShowScene:
		{
			CzScene* new_scene = action->Params[0].isEmpty() ? scene : game->findScene(action->getParameter1(cont).getHash());
			if (new_scene == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - ShowScene - Scene was not found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
			else
				new_scene->setVisible(true);
		}
		break;
	case Action_KillScene:
		{
			CzScene* new_scene = action->Params[0].isEmpty() ? scene : game->findScene(action->getParameter1(cont).getHash());
			if (new_scene == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - KillScene - Scene was not found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
			else
				game->removeScene(new_scene);
		}
		break;
	case Action_SetAllSceneTimelines:
		{
			CzAnimTimeline* timeline = CZ_GLOBAL_RESOURCES->getTimelinesManager()->findTimeline(action->getParameter1(cont).getHash());
			if (timeline != NULL)
			{
				game->SetAllTimelines(timeline);
				timeline->restart();
			}
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - SetAllSceneTimelines - Timeline was not found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
		}
		break;
	case Action_KillAllScenes:
		{
			CzVector<CzScene*> ignore_scenes;
			for (int t = 0; t < 5; t++)
			if (!action->Params[t].isEmpty())
			{
				CzScene* scene = NULL;
				switch (t)
				{
				case 0:
					scene = game->findScene(action->getParameter1(cont).getHash()); break;
				case 1:
					scene = game->findScene(action->getParameter2(cont).getHash()); break;
				case 2:
					scene = game->findScene(action->getParameter3(cont).getHash()); break;
				case 3:
					scene = game->findScene(action->getParameter4(cont).getHash()); break;
				case 4:
					scene = game->findScene(action->getParameter5(cont).getHash()); break;
				}
				if (scene != NULL)
					ignore_scenes.push_back(scene);
			}
			game->removeAllScenes(ignore_scenes);
		}
		break;
	}
}


