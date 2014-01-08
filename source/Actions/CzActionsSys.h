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

#if !defined(_CZ_ACTIONS_SYS_H_)
#define _CZ_ACTIONS_SYS_H_

#include "CzActions.h"
#include "CzXoml.h"

/**
 @addtogroup Actions
 @{
 */
 
/**
 @class	CzXomlActions_Sys

 @brief	Implements actions that are system related

 The CzXomlActions_Sys class holds Actions (IzXomlAction) that deal with system related functionality such as LoadXOML
 
 The following actions are supported:
 - LoadXOML		
 - FromTemplate	
 - Launch		
 - SetBGColour	
 - EnterValue	
 - Exit			
 - DebugText	
 - LoadFile		
 - PauseTime 

 The above actions have the following XOML prototypes:
 - LoadXOML(file-name, scene-name)
   - file-name - Name of XOML file to load
   - scene-name - Name of scene to load XOML data into, if not supplied then XOML is loaded globally (default)
 - FromTemplate(template-name, template-parameters, scene-name)
   - template-name - Name of template to instantiate
   - template-parameters - Parameters to pass to the template (p1=val1:p2=val2:p3=val3 etc)
   - scene-name - Scene where template should be instantiated (optional)
 - Launch(url)
   - url - URL to launch. The system interprets what should be ran from the protocol name and file name extension. For example passing “mailto:” will send an email, whilst passing “http:” will launch a web page etc..
 - SetBGColour(colour)
   - colour - Background colour in r, g, b, a format
 - EnterValue(message, target-variable, default-text)
   - message - Message to show to the user
   - target-variable- Variable to place entered text into
   - default-text - Default text, if you want to replace the variables value as the default text that is shown to the user
 - Exit()
 - DebugText(text, variable-name)
   - text - Text to output
   - variable-name - Variable value to output
 - LoadFile(file-resource-name, blocking, new-filename, scene-name)
   - file-resource-name - Name of file resource
   - blocking - If true then execution will pause until file is loaded
   - new-filename - New file name (optional). If new file name is supplied then the new file will be loaded into the file resource
   - scene-name - Scene in which file resource lives (optional)
 - PauseTime(pause-state)
   - pause-state - Pause or un-pause apps time progression

 */
class CzXomlActions_Sys : public IzXomlAction
{
public:
	enum eActionType
	{
		Action_LoadXOML			= 0, 
		Action_FromTemplate		= 1, 
		Action_Launch			= 2, 
		Action_SetBGColour		= 3, 
		Action_EnterValue		= 4, 
		Action_Exit				= 5, 
		Action_DebugText		= 6, 
		Action_LoadFile			= 7, 
		Action_PauseTime		= 8, 
		Action_Max				= 9, 
	};
protected:
	eActionType	Type;
	CzXomlActions_Sys() {}
public:
	CzXomlActions_Sys(eActionType type)
	{
		Type = type;
		switch (Type)
		{
		case Action_LoadXOML:
			setActionName("loadxoml");
			break;
		case Action_FromTemplate:
			setActionName("fromtemplate");
			break;
		case Action_Launch:
			setActionName("launch");
			break;
		case Action_SetBGColour:
			setActionName("setbgcolour");
			break;
		case Action_EnterValue:
			setActionName("entervalue");
			break;
		case Action_Exit:
			setActionName("exit");
			break;
		case Action_DebugText:
			setActionName("debugtext");
			break;
		case Action_LoadFile:
			setActionName("loadfile");
			break;
		case Action_PauseTime:
			setActionName("pausetime");
			break;
		}
	}
	void Execute(IzXomlResource* source, CzAction* action);
};

/// @}

#endif // _CZ_ACTIONS_SYS_H_
