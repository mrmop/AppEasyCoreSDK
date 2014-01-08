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

#if !defined(_CZ_ACTIONS_PROGRAM_H_)
#define _CZ_ACTIONS_PROGRAM_H_

#include "CzActions.h"
#include "CzXoml.h"

/**
 @addtogroup Actions
 @{
 */
 
/**
 @class	CzXomlActions_Program

 @brief	Implements actions that relate to programs

 The CzXomlActions_Program class holds Actions (IzXomlAction) that deal with XOML programs such as ChangeProgram
 
 The following actions are supported:
 - ChangeProgram

 The above actions have the following XOML prototypes:
 - ChangeProgram(program-name, command, program-container-scene(optional), goto-command-name(optional))
   - program-name - Name of program to change
   - command - Command to pass to the program, which includes:
	 - start - Starts the program executing
	 - stop - Stops the program
	 - pause - Pauses execution of the program
	 - next - Forces program to its next command
	 - restart - Restarts the program to the beginning
	 - goto - Changes execution to the name command within the program
	 - priority - Sets the program as the priority program
   - program-container-scene - If supplied then program in this scene will be changed
   - goto-command-name - Used by goto to specify command name to change execution to

 For more information on programs see CzProgram
 
 */

class CzXomlActions_Program : public IzXomlAction
{
public:
	enum eActionType
	{
		Action_ChangeProgram	= 0, 
		Action_Max				= 1, 
	};
protected:
	eActionType	Type;
	CzXomlActions_Program() {}
public:
	CzXomlActions_Program(eActionType type)
	{
		Type = type;
		switch (Type)
		{
		case Action_ChangeProgram:
			setActionName("changeprogram");
			break;
		}
	}
	void Execute(IzXomlResource* source, CzAction* action);
};

/// @}

#endif // _CZ_ACTIONS_PROGRAM_H_
