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

#include "CzActionsShared.h"
#include "CzHashes.h"
#include "CzApp.h"
#include "CzScene.h"
#include "CzActor.h"

/**
 @fn	void CzXomlActions_Shared::Execute(IzXomlResource* source, CzAction* action)

 @brief	The main actions executor for scene / actor shared actions. All shared actions such as SetProperty are implemented by this method

 @param [in]	source - The source of the action
 @param [in]	action - The action to execute
 */
void CzXomlActions_Shared::Execute(IzXomlResource* source, CzAction* action)
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

	if (Type == Action_SetProperty || Type == Action_AddProperty)
	{
		if (!action->Params[3].isEmpty())	// Check for a specific scene
		{
			CzScene* new_scene = game->findScene(action->getParameter4(cont).getHash());
			if (new_scene != NULL)
				scene = new_scene;
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - Set(Add)Property - Scene was not found - ", action->getParameter4(cont).c_str(), action->Parent->getDebugInfo().c_str());
			if (action->Params[2].isEmpty())
			{
				if (scene == NULL)
					CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Action - Set(Add)Property - Scene is invalid", action->Parent->getDebugInfo().c_str());
				else
					scene->setProperty(action->getParameter1().getHash(), action->getParameter2(cont), Type == Action_AddProperty);
				return;
			}
		}
		if (!action->Params[2].isEmpty())	// Check for a specific actor
		{
			if (scene == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Action - Set(Add)Property - Scene is invalid", action->Parent->getDebugInfo().c_str());
			else
			{
				CzActor* new_actor = scene->findActor(action->getParameter3(cont).getHash());
				if (new_actor != NULL)
					actor = new_actor;
				else
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - Set(Add)Property - Actor was not found - ", action->getParameter3(cont).c_str(), action->Parent->getDebugInfo().c_str());
			}
		}
		if (actor != NULL)
			actor->setProperty(action->getParameter1().getHash(), action->getParameter2(cont), Type == Action_AddProperty);
		else
		{
			if (scene != NULL)
				scene->setProperty(action->getParameter1().getHash(), action->getParameter2(cont), Type == Action_AddProperty);
		}
		return;
	}
	else
	if (Type == Action_SetUserProp || Type == Action_AddUserProp)
	{
		if (!action->Params[3].isEmpty())	// Check for a specific scene
		{
			CzScene* new_scene = game->findScene(action->getParameter4(cont).getHash());
			if (new_scene != NULL)
				scene = new_scene;
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - Set(Add)UserProp - Scene was not found - ", action->getParameter4(cont).c_str(), action->Parent->getDebugInfo().c_str());
			if (action->Params[2].isEmpty())
			{
				if (scene == NULL)
					CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Action - Set(Add)UserProp - Scene is invalid", action->Parent->getDebugInfo().c_str());
				else
				{
					CzUserPropertyList* props = scene->getUserPropertyList();
					if (props != NULL)
						props->setProperty(action->getParameter1().getHash(), action->getParameter2(cont), Type == Action_AddUserProp);
				}
				return;
			}
		}
		if (!action->Params[2].isEmpty())	// Check for a specific actor
		{
			if (scene == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Action - Set(Add)UserProp - Scene is invalid", action->Parent->getDebugInfo().c_str());
			else
			{
				CzActor* new_actor = scene->findActor(action->getParameter3(cont).getHash());
				if (new_actor != NULL)
					actor = new_actor;
				else
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - Set(Add)UserProp - Actor was not found - ", action->getParameter3(cont).c_str(), action->Parent->getDebugInfo().c_str());
			}
		}
		if (actor != NULL)
		{
			CzUserPropertyList* props = actor->getUserPropertyList();
			if (props != NULL)
				props->setProperty(action->getParameter1().getHash(), action->getParameter2(cont), Type == Action_AddUserProp);
		}
		else
		{
			if (scene != NULL)
			{
				CzUserPropertyList* props = scene->getUserPropertyList();
				if (props != NULL)
					props->setProperty(action->getParameter1().getHash(), action->getParameter2(cont), Type == Action_AddUserProp);
			}
		}
		return;
	}
	else
	if (Type == Action_UserPropToVar)
	{
		CzXomlVariable* var = NULL;
		if (!action->Params[1].isEmpty())
		{
			var = CzXomlVariable::GetVariable(action->Params[1], scene);
			if (var == NULL)
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Action - SetUserPropToVar - Variable was not found - ", action->Params[1].c_str(), action->Parent->getDebugInfo().c_str());
				return;
			}
		}
		else
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Action - SetUserPropToVar - No variable was specified", action->Parent->getDebugInfo().c_str());
			return;
		}

		if (!action->Params[3].isEmpty())	// Check for a specific scene
		{
			CzScene* new_scene = game->findScene(action->getParameter4(cont).getHash());
			if (new_scene != NULL)
				scene = new_scene;
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - SetUserPropToVar - Scene was not found - ", action->getParameter4(cont).c_str(), action->Parent->getDebugInfo().c_str());
			if (action->Params[2].isEmpty())
			{
				if (scene == NULL)
					CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Action - SetUserPropToVar - Scene is invalid", action->Parent->getDebugInfo().c_str());
				else
				{
					CzUserPropertyList* props = scene->getUserPropertyList();
					if (props != NULL)
						props->setPropertyToVariable(action->getParameter1().getHash(), *var);
				}
				return;
			}
		}
		if (!action->Params[2].isEmpty())	// Check for a specific actor
		{
			if (scene == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Action - SetUserPropToVar - Scene is invalid", action->Parent->getDebugInfo().c_str());
			else
			{
				CzActor* new_actor = scene->findActor(action->getParameter3(cont).getHash());
				if (new_actor != NULL)
					actor = new_actor;
				else
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - SetUserPropToVar - Actor was not found - ", action->getParameter3(cont).c_str(), action->Parent->getDebugInfo().c_str());
			}
		}
		if (actor != NULL)
		{
			CzUserPropertyList* props = actor->getUserPropertyList();
			if (props != NULL)
				props->setPropertyToVariable(action->getParameter1().getHash(), *var);
		}
		else
		{
			if (scene != NULL)
			{
				CzUserPropertyList* props = scene->getUserPropertyList();
				if (props != NULL)
					props->setPropertyToVariable(action->getParameter1().getHash(), *var);
			}
		}
		return;
	}
	else
	if (Type == Action_AddModifier)
	{
		if (!action->Params[3].isEmpty())	// Check for a specific scene
		{
			CzScene* new_scene = game->findScene(action->getParameter4(cont).getHash());
			if (new_scene != NULL)
				scene = new_scene;
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - AddModifier - Scene was not found, current scene being used - ", action->getParameter4(cont).c_str(), action->Parent->getDebugInfo().c_str());
		}
		if (!action->Params[2].isEmpty())	// Check for a specific actor
		{
			if (scene == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Action - AddModifier - Scene is invalid", action->Parent->getDebugInfo().c_str());
			else
			{
				CzActor* new_actor = scene->findActor(action->getParameter3(cont).getHash());
				if (new_actor != NULL)
					actor = new_actor;
				else
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - AddModifier - Actor was not found - ", action->getParameter3(cont).c_str(), action->Parent->getDebugInfo().c_str());
			}
		}

		if (actor != NULL)
		{
			// If actor has no modifier list then create one
			if (actor->getModifiers() == NULL)
				actor->setModifiers(new CzModifierManager());
		}
		else
		if (scene != NULL)
		{
			// If scene has no modifier list then create one
			if (scene->getModifiers() == NULL)
				scene->setModifiers(new CzModifierManager());
		}
		else
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - AddModifier - can only be added to actors or scenes - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
			return;
		}

		// Find the modifiers creator
		IzModifierCreator* creator = CZ_MODS->findCreator(action->getParameter1(cont).getHash());
		if (creator != NULL)
		{
			IzModifier* mod = creator->CreateInstance();
			if (mod != NULL)
			{
				mod->setActive(true);
				mod->setParameter(0, action->getParameter2(cont));
				if (actor != NULL)
				{
					if (!actor->getModifiers()->addModifier(mod))
						delete mod;
				}
				else
				if (scene != NULL)
				{
					if (!scene->getModifiers()->addModifier(mod))
						delete mod;
				}
			}
		}
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - AddModifier - Modifier not found and could not be added - ", action->getParameter1(cont).c_str());

		return;
	}
	else
	if (Type == Action_ChangeModifier)
	{
		if (!action->Params[3].isEmpty())	// Check for a specific scene
		{
			CzScene* new_scene = game->findScene(action->getParameter4(cont).getHash());
			if (new_scene != NULL)
				scene = new_scene;
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - ChangeModifier - Scene was not found, current scene being used - ", action->getParameter4(cont).c_str());
		}
		if (!action->Params[2].isEmpty())	// Check for a specific actor
		{
			if (scene == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Action - ChangeModifier - Scene is invalid");
			else
			{
				CzActor* new_actor = scene->findActor(action->getParameter3(cont).getHash());
				if (new_actor != NULL)
					actor = new_actor;
				else
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - ChangeModifier - Actor was not found - ", action->getParameter3(cont).c_str());
			}
		}

		unsigned int command = action->getParameter2(cont).getHash();
		if (actor != NULL)
		{
			if (actor->getModifiers() != NULL)
			{
				IzModifier* mod = actor->getModifiers()->findModifier(action->getParameter1(cont).getHash());
				if (mod != NULL)
				{
					if (command == CzHashes::Activate_Hash)
						mod->setActive(true);
					else
					if (command == CzHashes::Deactivate_Hash)
						mod->setActive(false);
					else
					if (command == CzHashes::Toggle_Hash)
						mod->setActive(!mod->isActive());
					else
					if (command == CzHashes::Remove_Hash)
						actor->getModifiers()->removeModifier(mod);
					else
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - ChangeModifier - Unknown modifier command - ", action->getParameter2(cont).c_str());
				}
				else
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - ChangeModifier - Actor did not contain the modifier - ", action->getParameter1(cont).c_str());
			}
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - ChangeModifier - Actor has no modifiers");
		}
		else
		if (scene != NULL)
		{
			if (scene->getModifiers() != NULL)
			{
				IzModifier* mod = scene->getModifiers()->findModifier(action->getParameter1(cont).getHash());
				if (mod != NULL)
				{
					if (command == CzHashes::Activate_Hash)
						mod->setActive(true);
					else
					if (command == CzHashes::Deactivate_Hash)
						mod->setActive(false);
					else
					if (command == CzHashes::Toggle_Hash)
						mod->setActive(!mod->isActive());
					else
					if (command == CzHashes::Remove_Hash)
						scene->getModifiers()->removeModifier(mod);
					else
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - ChangeModifier - Unknown modifier command - ", action->getParameter2(cont).c_str());
				}
				else
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - ChangeModifier - Scene did not contain the modifier - ", action->getParameter1(cont).c_str());
			}
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - ChangeModifier - Scene has no modifiers");
		}
		else
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - ChangeModifier - Modifiers can only be removed from actors or scenes - ", action->getParameter1(cont).c_str());
			return;
		}

		return;
	}

}


