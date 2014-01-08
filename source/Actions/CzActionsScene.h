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

#if !defined(_CZ_ACTIONS_SCENE_H_)
#define _CZ_ACTIONS_SCENE_H_

#include "CzActions.h"
#include "CzXoml.h"

/**
 @addtogroup Actions
 @{
 */
 
/**
 @class	CzXomlActions_Scene

 @brief	Implements actions that relate to scenes

 The CzXomlActions_Scene class holds Actions (IzXomlAction) that deal with scenes such as ChangeScene
 
 The following actions are supported:
 - CallActions
 - SetCurrentScene
 - BringtSceneToFront
 - ChangeScene
 - SuspendScene
 - ResumeScene
 - HideScene
 - ShowScene
 - KillScene
 - KillAllScenes
 - SetAllSceneTimelines

 The above actions have the following XOML prototypes:
 - CallActions(actions-list-name, parent-actor-name (optional), parent-scene-name (optional))
   - actions-list-name - Name of actions list to call
   - parent-actor-name - Actor that contains the actions list, if left out then the actor that contains this action will be used
   - parent-scene-name - Scene that contains the actions list, if left out then the scene that contains this action will be used. If parent-actor-name is 
   specified then parent-scene-name becomes the actors container scene. If parent-actor-name and parent-scene-name are not set then global actions list 
   will be searched.
 - SetCurrentScene(scene-name)
   - scene-name - Target scene name
 - BringtSceneToFront(scene-name)
   - scene-name - Target scene name
 - ChangeScene(scene-name)
   - scene-name - Target scene name
 - SuspendScene(scene-name)
   - scene-name - Target scene name (uses container scene if not specified)
 - ResumeScene(scene-name)
   - scene-name - Target scene name (uses container scene if not specified)
 - HideScene(scene-name)
   - scene-name - Target scene name (uses container scene if not specified)
 - ShowScene(scene-name)
   - scene-name - Target scene name (uses container scene if not specified)
 - KillScene(scene-name)
   - scene-name - Target scene name (uses container scene if not specified)
 - KillAllScenes(scene-name)
   - scene-name - Scene to exclude
 - SetAllSceneTimelines(timeline-name)
   - timeline-name - Name of timeline to assign to all scenes

 For more information on scenes see CzScene
 
 */
class CzXomlActions_Scene : public IzXomlAction
{
public:
	enum eActionType
	{
		Action_CallActions			= 0, 
		Action_SetCurrentScene		= 1, 
		Action_BringtSceneToFront	= 2, 
		Action_ChangeScene			= 3,
		Action_SuspendScene			= 4, 
		Action_ResumeScene			= 5, 
		Action_HideScene			= 6, 
		Action_ShowScene			= 7, 
		Action_KillScene			= 8, 
		Action_KillAllScenes		= 9, 
		Action_SetAllSceneTimelines	= 10, 
		Action_Max					= 11 
	};
protected:
	eActionType	Type;
	CzXomlActions_Scene() {}
public:
	CzXomlActions_Scene(eActionType type)
	{
		Type = type;
		switch (Type)
		{
		case Action_CallActions:
			setActionName("callactions");
			break;
		case Action_SetCurrentScene:
			setActionName("setcurrentscene");
			break;
		case Action_BringtSceneToFront:
			setActionName("bringscenetofront");
			break;
		case Action_ChangeScene:
			setActionName("changescene");
			break;
		case Action_SuspendScene:
			setActionName("suspendscene");
			break;
		case Action_ResumeScene:
			setActionName("resumescene");
			break;
		case Action_HideScene:
			setActionName("hidescene");
			break;
		case Action_ShowScene:
			setActionName("showscene");
			break;
		case Action_KillScene:
			setActionName("killscene");
			break;
		case Action_KillAllScenes:
			setActionName("killallscenes");
			break;
		case Action_SetAllSceneTimelines:
			setActionName("setallscenetimelines");
			break;
		}
	}
	void Execute(IzXomlResource* source, CzAction* action);
};

/// @}

#endif // _CZ_ACTIONS_SCENE_H_