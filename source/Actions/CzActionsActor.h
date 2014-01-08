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

#if !defined(_CZ_ACTIONS_ACTOR_H_)
#define _CZ_ACTIONS_ACTOR_H_

#include "CzActions.h"
#include "CzXoml.h"

/**
 @addtogroup Actions
 @{
 */
 
/**
 @class	CzXomlActions_Actor

 @brief	Implements a collection of actor actions.

 The CzXomlActions_Actor class holds Actions (IzXomlAction) that can be carried out on Actors (CzActor). An action is basically a command that tells the actor to do something 
 such as modify its appearance or behaviour. For example the KillActor tells the actor to remove itself from the scene and destroy itself.
 
 The following actions are supported:
 - HideActor		- Hide an actor from view (continues processing)
 - ShowActor		- Shows a previously hidden actor
 - ActivateActor	- Activates an actor (resumes processing of the actor)
 - DeactivateActor	- Deactivates an actor (suspends processing but still displays it)
 - KillActor		- Removes the actor from its scene and destroys it, fires the actors OnDestroy event handler)
 - SetKeyFocus		- Sets actor as key focus (actor will receive input from key / button devices)
 - UpdateText		- Updates a text actor with the value of a variable (deprecated, property bindings are now used instead)

 The above actions have the following XOML prototypes:
 - HideActor(actor-name)
   - actor-name - Name of target actor (uses actions container actor if not specified)
 - ShowActor(actor-name)
   - actor-name - Name of target actor (uses actions container actor if not specified)
 - ActivateActor(actor-name)
   - actor-name - Name of target actor (uses actions container actor if not specified)
 - DeactivateActor(actor-name)
   - actor-name - Name of target actor (uses actions container actor if not specified)
 - KillActor(actor-name)
   - actor-name - Name of target actor (uses actions container actor if not specified)
 - SetKeyFocus(actor-name)
   - actor-name - Name of target actor (uses actions container actor if not specified)
 - UpdateText(actor-name, variable-name, parent-scene-name (optional))
   - actor-name - Name of target actor (uses actions container actor if not specified)
   - variable-name - Name of variable to write to the actor
   - parent-scene-name - Scene where target actor lives

 For more information on the actions system see CzAction
 
 */
class CzXomlActions_Actor : public IzXomlAction
{
public:
	enum eActionType
	{
		Action_HideActor		= 0,	
		Action_ShowActor		= 1,	
		Action_ActivateActor	= 2,	
		Action_DeactivateActor	= 3,	
		Action_KillActor		= 4,	
		Action_SetKeyFocus		= 5,	
		Action_UpdateText		= 6,	
		Action_Max				= 7,
	};
protected:
	eActionType	Type;
	CzXomlActions_Actor() {}
public:
	CzXomlActions_Actor(eActionType type)
	{
		Type = type;
		switch (Type)
		{
		case Action_HideActor:
			setActionName("hideactor");
			break;
		case Action_ShowActor:
			setActionName("showactor");
			break;
		case Action_ActivateActor:
			setActionName("activateactor");
			break;
		case Action_DeactivateActor:
			setActionName("deactivateactor");
			break;
		case Action_KillActor:
			setActionName("killactor");
			break;
		case Action_SetKeyFocus:
			setActionName("setkeyfocus");
			break;
		case Action_UpdateText:
			setActionName("updatetext");
			break;
		}
	}
	void Execute(IzXomlResource* source, CzAction* action);
};

/// @}

#endif // _CZ_ACTIONS_ACTOR_H_
