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

#include "CzActionsSys.h"
#include "CzApp.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"
#include "CzXomlLoad.h"

/**
 @fn	void CzXomlActions_Sys::Execute(IzXomlResource* source, CzAction* action)

 @brief	The main actions executor for system related actions. All system specific actions such as LoadXOML are implemented by this method

 @param [in]	source - The source of the action
 @param [in]	action - The action to execute
 */
void CzXomlActions_Sys::Execute(IzXomlResource* source, CzAction* action)
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
	case Action_LoadXOML:
		{
			if (!action->Params[0].isEmpty())
			{
				IzXomlResource* parent = NULL;
				if (!action->Params[1].isEmpty())
					parent = scene->getResourceManager()->findResource(action->getParameter2(cont).getHash(), CzHashes::Scene_Hash);
				if (parent == NULL)
					parent = game;
				CZ_XOML->Process(parent, action->getParameter1(cont).c_str());
			}
		}
		break;
	case Action_FromTemplate:
		{
			if (!action->Params[0].isEmpty())
			{
				if (!action->Params[2].isEmpty())
				{
					CzScene* target_scene = game->findScene(action->getParameter3(cont).getHash());
					CzXomlLoad::CreateInstance(scene, target_scene, &action->getParameter1(cont), &action->getParameter2(cont));
				}
				else
					CzXomlLoad::CreateInstance(scene, scene, &action->getParameter1(cont), &action->getParameter2(cont));
			}
		}
		break;
	case Action_Launch:
		{
			if (!action->Params[0].isEmpty())
				PLATFORM_SYS->ExecuteViaOS(action->getParameter1(cont).c_str(), false);
		}
		break;
	case Action_SetBGColour:
		{
			int components[8];
			if (action->getParameter1(cont).getAsListOfInt(components) == 4)
				game->SetBackgroundColour((uint8)components[0], (uint8)components[1], (uint8)components[2], (uint8)components[3]);
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - SetBGColour - Incorrect component count - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
		}
		break;
	case Action_EnterValue:	// EnterValue Message To User, Variable to place value in
		{
			CzXomlVariable* var = CzXomlVariable::GetVariable(action->Params[1], scene);
			if (var != NULL)
			{
				const char* str = CZ_INPUT->showOnScreenKeyboard(action->getParameter1(cont).c_str(), 0, (action->getParameter3(cont) == "0") ? NULL : var->getValue().c_str());
				if (str != NULL)
					var->setValue(str);
			}
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - Enter Value - Target variable not found - ", action->getParameter2(cont).c_str(), action->Parent->getDebugInfo().c_str());
		}
		break;
	case Action_Exit:				// Exits the app
		PLATFORM_SYS->RequestQuit();
		break;
	case Action_DebugText:
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_NONE, action->getParameter1(cont).c_str(), action->getParameter2(cont).c_str(), action->Parent->getDebugInfo().c_str());
		}
		break;
	case Action_LoadFile:	// File, blocking, filename (optional), scene (optional)
		{
			IzXomlResource* parent = NULL;
			if (!action->Params[3].isEmpty())
				scene = game->findScene(action->getParameter4(cont).getHash());
			if (!action->Params[0].isEmpty())
			{
				if (scene == NULL)
					CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Action - LoadFile - Scene is invalid - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
				else
				{
					CzDataFile* file = (CzDataFile*)scene->getResourceManager()->findResource(action->getParameter1(cont).getHash(), CzHashes::File_Hash);
					if (file != NULL)
					{
						if (!action->Params[2].isEmpty())
						{
							// New filename was supplied so load new file contents into file
							file->Init(action->getParameter3(cont).c_str(), true, action->getParameter2(cont).getAsBool());
						}
						else
							file->Load(action->getParameter2(cont).getAsBool());
					}
					else
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - LoadFile - File resource could not be found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
				}
			}
		}
		break;
	case Action_PauseTime:				// Pauses / un-pauses app time
		game->setTimePaused(action->getParameter1(cont).getAsBool());
		break;
	}
}
