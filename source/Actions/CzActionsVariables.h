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

#if !defined(_CZ_ACTIONS_VARIABLE_H_)
#define _CZ_ACTIONS_VARIABLE_H_

#include "CzActions.h"
#include "CzXoml.h"

/**
 @addtogroup Actions
 @{
 */
 
/**
 @class	CzXomlActions_Variable

 @brief	Implements actions that are related to XOML variables

 The CzXomlActions_Variable class holds Actions (IzXomlAction) that deal with system related functionality such as LoadXOML
 
 The following actions are supported:
 - SetVariable
 - AddVariable

 The above actions have the following XOML prototypes:
 - SetVar(variable-name, variable-value, scene-name)
   - variable-name - Name of variable
   - variable-value - Valeu to assign to the variable
   - scene-name - Scene where variable lives (optional)
 - SetVar(variable-name, rand, scene-name, min, max)
   - variable-name - Name of variable
   - rand - “rand”, “randchar” - rand generates a random number, whilst randchar generates a random single character
   - scene-name - Scene where variable lives (optional)
   - min - Minimum random number / character
   - max - Maximum random number / character
 - AddVar(variable-name, variable-amount, limit)
   - variable-name - Name of variable
   - variable-amount - Amount or string to add
   - limit - Value limit (optional)

 For more information on XOML variables see CzXomlVariable
 
 */
class CzXomlActions_Variable : public IzXomlAction
{
public:
	enum eActionType
	{
		Action_SetVariable		= 0, 
		Action_AddVariable		= 1, 
		Action_Max				= 2, 
	};
protected:
	eActionType	Type;
	CzXomlActions_Variable() {}
public:
	CzXomlActions_Variable(eActionType type)
	{
		Type = type;
		switch (Type)
		{
		case Action_SetVariable:
			setActionName("setvar");
			break;
		case Action_AddVariable:
			setActionName("addvar");
			break;
		}
	}
	void Execute(IzXomlResource* source, CzAction* action);
};

/// @}

#endif // _CZ_ACTIONS_VARIABLE_H_
