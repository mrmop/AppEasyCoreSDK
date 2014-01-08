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

#include "CzActionsActor.h"
#include "CzHashes.h"
#include "CzApp.h"
#include "CzScene.h"
#include "CzActorText.h"
#include "CzUIBase.h"

/**
 @fn	void CzXomlActions_Actor::Execute(IzXomlResource* source, CzAction* action)

 @brief	The main actions executor for actors. All actor specific actions are implemented by this method

 @param [in]	source - The source of the action
 @param [in]	action - The action to execute
 */
void CzXomlActions_Actor::Execute(IzXomlResource* source, CzAction* action)
{
	if (source == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Source was not found for action - ", action->_MethodName.c_str(), action->Parent->getDebugInfo().c_str());
#endif
		return;
	}

	// Determine the container
	CzApp* app;
	CzScene* scene;
	CzActor* actor;
	IzXomlResource* cont = IzXomlAction::getContainer(source, app, scene, actor);

	switch (Type)
	{
	case Action_HideActor:
		{
			CzActor* new_actor = action->Params[0].isEmpty() ? actor : scene->findActor(action->getParameter1(cont).getHash());
			if (new_actor == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - HideActor - Actor was not found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
			else
				new_actor->setVisible(false);
		}
		break;
	case Action_ShowActor:
		{
			CzActor* new_actor = action->Params[0].isEmpty() ? actor : scene->findActor(action->getParameter1(cont).getHash());
			if (new_actor == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - HideActor - Actor was not found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
			else
				new_actor->setVisible(true);
		}
		break;
	case Action_ActivateActor:
		{
			CzActor* new_actor = action->Params[0].isEmpty() ? actor : scene->findActor(action->getParameter1(cont).getHash());
			if (new_actor == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - HideActor - Actor was not found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
			else
				new_actor->setActive(true);
		}
		break;
	case Action_DeactivateActor:
		{
			CzActor* new_actor = action->Params[0].isEmpty() ? actor : scene->findActor(action->getParameter1(cont).getHash());
			if (new_actor == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - HideActor - Actor was not found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
			else
				new_actor->setActive(false);
		}
		break;
	case Action_KillActor:
		{
			CzActor* new_actor = action->Params[0].isEmpty() ? actor : scene->findActor(action->getParameter1(cont).getHash());
			if (new_actor == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - HideActor - Actor was not found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
			else
				scene->removeActor(new_actor);
		}
		break;
	case Action_SetKeyFocus:
		{
			CzActor* new_actor = action->Params[0].isEmpty() ? actor : scene->findActor(action->getParameter1(cont).getHash());
			if (new_actor == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - HideActor - Actor was not found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
			else
			{
				if (new_actor->getCategory() != CzActor::AC_UI)
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - SetKeyFocus - Actor needs to be a UI actor to receive key focus", action->Parent->getDebugInfo().c_str());
				else
					app->setKeyFocus((CzUIBase*)new_actor);
			}
		}
		break;
	case Action_UpdateText:		// UpdateText Name Of Text Actor, Variable to get value from, Scene where text actor lives (optional)
		{
			if (!action->Params[2].isEmpty())
				scene = app->findScene(action->getParameter3(cont).c_str());

			CzXomlVariable* var = CzXomlVariable::GetVariable(action->Params[1], scene);
			if (var != NULL)
			{
				CzActorText* text = (CzActorText*)scene->findActor(action->getParameter1(cont).c_str());
				if (text->getActualClassTypeHash() ==  CzHashes::ActorText_Hash)
				{
					text->setText(var->getValue().c_str());
				}
				else
					CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Action - Update Text - Target actor is not a text based actor - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
			}
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - Update Text - Source variable not found - ", action->getParameter2(cont).c_str(), action->Parent->getDebugInfo().c_str());
		}
		break;
	}
}




