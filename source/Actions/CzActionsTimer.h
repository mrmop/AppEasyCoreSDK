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

#if !defined(_CZ_ACTIONS_TIMER_H_)
#define _CZ_ACTIONS_TIMER_H_

#include "CzActions.h"
#include "CzXoml.h"

/**
 @addtogroup Actions
 @{
 */
 
/**
 @class	CzXomlActions_Timer

 @brief	Implements actions that are timer related

 The CzXomlActions_Timer class holds Actions (IzXomlAction) that deal with timer related functionality such as ChangeTimer
 
 The following actions are supported:
 - ChangeTimer

 The above actions have the following XOML prototypes:
 - ChangeTimer(timer-name, container-name, command)
   - timer-name - Name of timer to change
   - container-name - Name of container that holds the timer
   - command-name - Name of command to pass to timer (start, stop, pause)

 For more information on timers see CzTimer.
 
 */
class CzXomlActions_Timer : public IzXomlAction
{
public:
	enum eActionType
	{
		Action_ChangeTimer		= 0, 
		Action_Max				= 1, 
	};
protected:
	eActionType	Type;
	CzXomlActions_Timer() {}
public:
	CzXomlActions_Timer(eActionType type)
	{
		Type = type;
		switch (Type)
		{
		case Action_ChangeTimer:
			setActionName("changetimer");
			break;
		}
	}
	void Execute(IzXomlResource* source, CzAction* action);
};

/// @}

#endif // _CZ_ACTIONS_TIMER_H_
