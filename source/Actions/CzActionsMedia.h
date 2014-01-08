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

#if !defined(_CZ_ACTIONS_MEDIA_H_)
#define _CZ_ACTIONS_MEDIA_H_

#include "CzActions.h"
#include "CzXoml.h"

/**
 @addtogroup Actions
 @{
 */
 
/**
 @class	CzXomlActions_Media

 @brief	Implements actions that relate to media control

 The CzXomlActions_Media class holds Actions (IzXomlAction) that deal with media play back and control such as playing a sound effect, music or modifiying the video cam stream. 
 
 The following actions are supported:
 - PlaySound	
 - PlayMusic	
 - StopMusic	
 - ChangeVideoCam
 - AudioEnable

 The above actions have the following XOML prototypes:
 - PlaySound(sound-name, looped)
   - sound-name - sound resource name
   - looped - true if the sound should loop
 - PlayMusic(filename, repeat_count)
   - filename - Name of the file that contains the music
   - repeat-count - Total number of tims to repeat play back (0 - forever)
 - StopMusic()
 - ChangeVideoCam(videocam-name, command)
   - videocam-name - Name of VideoCam resource to change
   - command - Command to use to change video cam (start or stop)
 - AudioEnable(music_enable, sounds_enable)

 For more information on audio playback see CzAudio and for more information on VideoCam see CzVideoCam
 
 */
 
class CzXomlActions_Media : public IzXomlAction
{
public:
	enum eActionType
	{
		Action_PlaySound		= 0, 
		Action_PlayMusic		= 1, 
		Action_StopMusic		= 2, 
		Action_ChangeVideoCam	= 3, 
		Action_AudioEnable		= 4, 
		Action_Max				= 5, 
	};
protected:
	eActionType	Type;
	CzXomlActions_Media() {}
public:
	CzXomlActions_Media(eActionType type)
	{
		Type = type;
		switch (Type)
		{
		case Action_PlaySound:
			setActionName("playsound");
			break;
		case Action_PlayMusic:
			setActionName("playmusic");
			break;
		case Action_StopMusic:
			setActionName("stopmusic");
			break;
		case Action_ChangeVideoCam:
			setActionName("changevideocam");
			break;
		case Action_AudioEnable:
			setActionName("audioenable");
			break;
		}
	}
	void Execute(IzXomlResource* source, CzAction* action);
};

/// @}

#endif // _CZ_ACTIONS_MEDIA_H_
