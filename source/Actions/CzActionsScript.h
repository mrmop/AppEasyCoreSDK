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

#if !defined(_CZ_ACTIONS_SCRIPT_H_)
#define _CZ_ACTIONS_SCRIPT_H_

#include "CzActions.h"
#include "CzXoml.h"

/**
 @addtogroup Actions
 @{
 */
 
/**
 @class	CzXomlActions_Script

 @brief	Implements actions that relate to scripts

 The CzXomlActions_Script class holds Actions (IzXomlAction) that deal with scripts such as CallScript
 
 The following actions are supported:
 - CallScript
 - Inline
 - CallGlobalScript

 The above actions have the following XOML prototypes:
 - CallScript(script-function-name, context-name, param2, param3, param4)
   - script-function-name - Script function name to call (must be in scene or global scope)
   - context-name - Scene or actor that should be passed as the first parameter to the script function. If not set then the actor or scene that the action is defined inside of will be passed.
   - Param2 to Param4 - Passed as the 2nd, 3rd and 4th parameters of the function (all optional)
 - Inline(script-code, scene-name)
   - script-code - Script code snipit to execute
   - scene-name - Name of scene that contains the script engine that should execute the script. By not passing this parameter the scene that contains the action will be used. If the action is declared outside of a scene then the global script engine will be used.
 - CallGlobalScript(script-function-name, context-name, param2, param3, param4)
   - script-function-name - Global script function name to call
   - Scene that should be passed as the first parameter to the script function. If not set then the main game object will be passed.
   - Param2 to Param4 - Passed as the 2nd, 3rd and 4th parameters of the function (all optional)

 For more information on scripting see CzScript and IzScriptEngine
 
 */

class CzXomlActions_Script : public IzXomlAction
{
public:
	enum eActionType
	{
		Action_CallScript			= 0, 
		Action_Inline				= 1, 
		Action_CallGlobalScript		= 2, 
		Action_Max					= 3, 
	};
protected:
	eActionType	Type;
	CzXomlActions_Script() {}
public:
	CzXomlActions_Script(eActionType type)
	{
		Type = type;
		switch (Type)
		{
		case Action_CallScript:
			setActionName("callscript");
			break;
		case Action_Inline:
			setActionName("inline");
			break;
		case Action_CallGlobalScript:
			setActionName("callglobalscript");
			break;
		}
	}
	void Execute(IzXomlResource* source, CzAction* action);
};

/// @}

#endif // _CZ_ACTIONS_SCRIPT_H_
