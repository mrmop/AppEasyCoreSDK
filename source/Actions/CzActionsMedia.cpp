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

#include "CzActionsMedia.h"
#include "CzHashes.h"
#include "CzApp.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzAudio.h"
#include "CzCam.h"
#include "CzXomlResourceManager.h"

/**
 @fn	void CzXomlActions_Media::Execute(IzXomlResource* source, CzAction* action)

 @brief	The main actions executor for media related actions. All media specific actions such as PlaySound, PlayMusic etc are implemented by this method

 @param [in]	source - The source of the action
 @param [in]	action - The action to execute
 */
 
void CzXomlActions_Media::Execute(IzXomlResource* source, CzAction* action)
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
	case Action_PlaySound:
		if (!action->Params[0].isEmpty())
		{
			bool looped = false;
			if (!action->Params[1].isEmpty())
				looped = action->getParameter2(cont).getAsBool();
			PLATFORM_AUDIO->PlaySound(action->getParameter1(cont).getHash(), scene, 1.0f, 1.0f, 0, looped);
		}
		break;
	case Action_PlayMusic:
		if (!action->Params[0].isEmpty())
		{
			if (action->Params[1].isEmpty())
				PLATFORM_AUDIO->PlayMusic(action->getParameter1(cont).c_str(), 1);
			else
				PLATFORM_AUDIO->PlayMusic(action->getParameter1(cont).c_str(), action->getParameter2(cont).getAsInt());
		}
		break;
	case Action_StopMusic:
		PLATFORM_AUDIO->StopMusic();
		break;
	case Action_ChangeVideoCam:
		{
			CzString* p1 = &action->getParameter1(cont);
			CzString* p2 = NULL;
			if (!action->Params[1].isEmpty())
				p2 = &action->getParameter2(cont);
			if (p2 == NULL)
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - VideoCam - Function not supplied - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
				return;
			}
			CzVideoCam* cam = (CzVideoCam*)CzXomlResourceManager::FindResource(p1->getHash(), CzHashes::VideoCam_Hash, scene);
			if (cam != NULL)
			{
				unsigned int hash = p2->getHash();
				if (hash == CzHashes::Start_Hash)
					cam->Start();
				else
				if (hash == CzHashes::Stop_Hash)
					cam->Stop();
				else
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - VideoCam - Function not supported - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
			}
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - VideoCam - Resource was not found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
		}
		break;
	case Action_AudioEnable:
		{
			CzString* p1 = &action->getParameter1(cont);
			CzString* p2 = &action->getParameter1(cont);

			if (p1->getHash() == CzHashes::True_Hash)
				PLATFORM_AUDIO->setMusicOn(true);
			else
				PLATFORM_AUDIO->setMusicOn(false);
			if (p2->getHash() == CzHashes::True_Hash)
				PLATFORM_AUDIO->setSoundOn(true);
			else
				PLATFORM_AUDIO->setSoundOn(false);
		}
		break;
	}
}
