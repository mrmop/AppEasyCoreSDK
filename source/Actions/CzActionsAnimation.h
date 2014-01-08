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

#if !defined(_CZ_ACTIONS_ANIMATION_H_)
#define _CZ_ACTIONS_ANIMATION_H_

#include "CzActions.h"
#include "CzXoml.h"

/**
 @addtogroup Actions
 @{
 */
 
/**
 @class	CzXomlActions_Animation

 @brief	Implements a collection of animation actions.

 The CzXomlActions_Animation class holds Actions (IzXomlAction) that can be carried out on Animation Timelines (CzAnimTimeline). An action is basically a command that tells 
 the animation to do something such as play or pause. For example the PauseTimeline tells the animation timeline to pause playback.
 
 The following actions are supported:
 - PlayTimeline		- Plays a timeline
 - StopTimeline		- Stops a timeline from playing
 - PauseTimeline	- Pauses the timeline playback
 - SetTimeline		- Assigns the timeline to an actor or scene

 The above actions have the following XOML prototypes:
 - PlayTimeline(timeline-name, actor-or-scene-name(optional))
   - timeline-name - Name of timeline
   - actor-or-scene-name - Scene or actor that contains the timeline
 - StopTimeline(timeline-name, actor-or-scene-name(optional))
   - timeline-name - Name of timeline
   - actor-or-scene-name - Scene or actor that contains the timeline
 - PauseTimeline(timeline-name, actor-or-scene-name(optional))
   - timeline-name - Name of timeline
   - actor-or-scene-name - Scene or actor that contains the timeline
 - SetTimeline(timeline-name, actor-or-scene-name(optional))
   - timeline-name - Name of timeline
   - actor-or-scene-name - Scene or actor that contains the timeline

 For more information on the animation system see CzAnim
 
 */
 
class CzXomlActions_Animation : public IzXomlAction
{
public:
	enum eActionType
	{
		Action_PlayTimeline			= 0, 
		Action_StopTimeline			= 1, 
		Action_PauseTimeline		= 2, 
		Action_SetTimeline			= 3, 
		Action_Max					= 4, 
	};
protected:
	eActionType	Type;
	CzXomlActions_Animation() {}
public:
	CzXomlActions_Animation(eActionType type)
	{
		Type = type;
		switch (Type)
		{
		case Action_PlayTimeline:
			setActionName("playtimeline");
			break;
		case Action_StopTimeline:
			setActionName("stoptimeline");
			break;
		case Action_PauseTimeline:
			setActionName("pausetimeline");
			break;
		case Action_SetTimeline:
			setActionName("settimeline");
			break;
		}
	}
	void Execute(IzXomlResource* source, CzAction* action);
};

/// @}

#endif // _CZ_ACTIONS_ANIMATION_H_
