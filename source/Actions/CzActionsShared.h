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

#if !defined(_CZ_ACTIONS_SHARED_H_)
#define _CZ_ACTIONS_SHARED_H_

#include "CzActions.h"
#include "CzXoml.h"

/**
 @addtogroup Actions
 @{
 */
 
/**
 @class	CzXomlActions_Shared

 @brief	Implements actions that are shared between actors and scenes

 The CzXomlActions_Shared class holds Actions (IzXomlAction) that deal with common functionality that is shared between scenes and actors
 
 The following actions are supported:
  - SetProperty	
  - AddProperty	
  - SetUserProp
  - AddUserProp
  - UserPropToVar
  - AddModifier	
  - ChangeModifier


 The above actions have the following XOML prototypes:
  - SetProperty(property-name, property-value, actor-name, scene-name)
    - property-name - Name of property
	- property-value - Value to assign to property
	- actor-name - Name of actor to set property of, if omitted then actor that the action is called from will be used (optional)
	- scene-name - Scene where the specific actor specified actor-name lives, if actor-name not specified then property of this scene will be set (optional)
  - AddProperty(property-name, property-value, actor-name, scene-name)
    - property-name - Name of property
	- property-value - Value to add to property
	- actor-name - Name of actor to set property of, if omitted then actor that the action is called from will be used (optional)
	- scene-name - Scene where the specific actor specified actor-name lives, if actor-name not specified then property of this scene will be set (optional)
  - SetUserProp(property-name, property-value, actor-name, scene-name)
    - property-name - Name of user property
	- property-value - Value to assign to user property
	- actor-name - Name of actor to set user property of, if omitted then actor that the action is called from will be used (optional)
	- scene-name - Scene where the specific actor specified actor-name lives, if actor-name not specified then user property of this scene will be set (optional)
  - AddUserProp(property-name, property-value, actor-name, scene-name)
    - property-name - Name of user property
	- property-value - Value to add to user property
	- actor-name - Name of actor to set user property of, if omitted then actor that the action is called from will be used (optional)
	- scene-name - Scene where the specific actor specified actor-name lives, if actor-name not specified then user property of this scene will be set (optional)
  - UserPropToVar(property-name, variable-name, actor-name, scene-name)
    - property-name - Name of user property
	- property-value - Name of value to receive the user property
	- actor-name - Name of actor to use, if omitted then actor that the action is called from will be used (optional)
	- scene-name - Scene where the specific actor specified actor-name lives, if actor-name not specified then this scene will be used (optional)
  - AddModifier(modifier-name, modifier-parameter, actor-name, scene-name)
    - modifier-name - Name of modifier
	- command - Specifies how the modifier is to be changed (activate, deactivate, toggle or remove)
	- actor-name - Name of actor to add the modifier to (optional)
	- scene-name - Scene where the specific actor specified actor-name lives, if actor-name not specified then modifier will be added to this scene(optional)
  - ChangeModifier(modifier-name, command, actor-name, scene-name)

 For more information on actors and scenes see CzActor and CzScene. For more information on Modifiers see IzModifier and CzModifierManager
 
 */
 
class CzXomlActions_Shared : public IzXomlAction
{
public:
	enum eActionType
	{
		Action_SetProperty			= 0, 
		Action_AddProperty			= 1, 
		Action_SetUserProp			= 2, 
		Action_AddUserProp			= 3, 
		Action_UserPropToVar		= 4, 
		Action_AddModifier			= 5, 
		Action_ChangeModifier		= 6, 
		Action_Max					= 7, 
	};
protected:
	eActionType	Type;
	CzXomlActions_Shared() {}
public:
	CzXomlActions_Shared(eActionType type)
	{
		Type = type;
		switch (Type)
		{
		case Action_SetProperty:
			setActionName("setproperty");
			break;
		case Action_AddProperty:
			setActionName("addproperty");
			break;
		case Action_SetUserProp:
			setActionName("setuserprop");
			break;
		case Action_UserPropToVar:
			setActionName("userproptovar");
			break;
		case Action_AddUserProp:
			setActionName("adduserprop");
			break;
		case Action_AddModifier:
			setActionName("addmodifier");
			break;
		case Action_ChangeModifier:
			setActionName("changemodifier");
			break;
		}
	}
	void Execute(IzXomlResource* source, CzAction* action);
};

/// @}

#endif // _CZ_ACTIONS_SHARED_H_