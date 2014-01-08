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

#include "CzActionsScript.h"
#include "CzHashes.h"
#include "CzApp.h"
#include "CzScene.h"
#include "CzActor.h"

/**
 @fn	void CzXomlActions_Script::Execute(IzXomlResource* source, CzAction* action)

 @brief	The main actions executor for script related actions. All script specific actions such as CallScript are implemented by this method

 @param [in]	source - The source of the action
 @param [in]	action - The action to execute
 */
void CzXomlActions_Script::Execute(IzXomlResource* source, CzAction* action)
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
	case Action_CallScript:	// Calls a script function
		{
			CzString* p1 = &action->getParameter1(cont);
			CzString* p2 = NULL;
			CzActor* new_actor = NULL;
			CzScene* new_scene = scene;
			if (!action->Params[1].isEmpty())	// Check for a specific scene or actor
			{
				p2 = &action->getParameter2(cont);
				new_actor = scene->findActor(p2->getHash());
				if (new_actor != NULL)
					actor = new_actor;
				else
				{
					new_scene = game->findScene(p2->getHash());
					if (new_scene == NULL)
					{
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - CallScript - Script target actor or scene not found - ", p2->c_str(), action->Parent->getDebugInfo().c_str());
						return;
					}
				}
			}
			CzString* p3 = NULL;
			CzString* p4 = NULL;
			CzString* p5 = NULL;
			if (!action->Params[2].isEmpty())
				p3 = &action->getParameter3(cont);
			if (!action->Params[3].isEmpty())
				p4 = &action->getParameter4(cont);
			if (!action->Params[4].isEmpty())
				p5 = &action->getParameter5(cont);

			if (scene->getScriptEngine() == NULL)
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - CallScript - Scene does not contain a script engine", action->Parent->getDebugInfo().c_str());
				return;
			}

			if (p2 != NULL)
			{
				if (new_actor != NULL)
					scene->getScriptEngine()->CallFunction(new_actor, p1, p3, p4, p5);
				else
					scene->getScriptEngine()->CallFunction(new_scene, p1, p3, p4, p5);
			}
			else
			{
				if (actor != NULL)
					scene->getScriptEngine()->CallFunction(actor, p1, p3, p4, p5);
				else
					scene->getScriptEngine()->CallFunction(new_scene, p1, p3, p4, p5);
			}
		}
		break;
	case Action_Inline:	// Calls script inline
		{
			CzString* p1 = &action->getParameter1(cont);
			if (!action->Params[1].isEmpty())	// Check for a specific scene
			{
				CzString* p2 = &action->getParameter2(cont);
				scene = game->findScene(p2->getHash());
				if (scene == NULL)
				{
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - Inline - Scene not found - ", p2->c_str(), action->Parent->getDebugInfo().c_str());
					return;
				}
				if (scene->getScriptEngine() == NULL)
				{
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - Inline - Scene does not contain a script engine", action->Parent->getDebugInfo().c_str());
					return;
				}

				// Run script in scene script space
				scene->getScriptEngine()->LoadScript(p1->c_str(), p1->getLength(), "inline");
			}
			else
			{
				// Run script in global script space
				CZ_GLOBAL_RESOURCES->getScriptEngine()->LoadScript(p1->c_str(), p1->getLength(), "inline");
			}
		}
		break;
	case Action_CallGlobalScript:
		{
			CzString* p1 = &action->getParameter1(cont);
			CzString* p2 = NULL;
			CzActor* new_actor = NULL;
			CzScene* new_scene = scene;
			if (!action->Params[1].isEmpty())	// Check for a specific scene or actor
			{
				p2 = &action->getParameter2(cont);
				new_actor = scene->findActor(p2->getHash());
				if (new_actor != NULL)
					actor = new_actor;
				else
				{
					new_scene = game->findScene(p2->getHash());
					if (new_scene == NULL)
					{
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - CallScript - Script target actor or scene not found - ", p2->c_str(), action->Parent->getDebugInfo().c_str());
						return;
					}
				}
			}
			CzString* p3 = NULL;
			CzString* p4 = NULL;
			CzString* p5 = NULL;
			if (!action->Params[2].isEmpty())
				p3 = &action->getParameter3(cont);
			if (!action->Params[3].isEmpty())
				p4 = &action->getParameter4(cont);
			if (!action->Params[4].isEmpty())
				p5 = &action->getParameter5(cont);

			if (p2 != NULL)
			{
				if (new_actor != NULL)
					CZ_GLOBAL_RESOURCES->getScriptEngine()->CallFunction(new_actor, p1, p3, p4, p5);
				else
					CZ_GLOBAL_RESOURCES->getScriptEngine()->CallFunction(new_scene, p1, p3, p4, p5);
			}
			else
			{
				if (actor != NULL)
					CZ_GLOBAL_RESOURCES->getScriptEngine()->CallFunction(actor, p1, p3, p4, p5);
				else
					CZ_GLOBAL_RESOURCES->getScriptEngine()->CallFunction(new_scene, p1, p3, p4, p5);
			}
		}
		break;
	}
}
