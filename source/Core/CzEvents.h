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

#if !defined(_CZ_EVENTS_H_)
#define _CZ_EVENTS_H_

#include "CzUtil.h"
#include "CzXoml.h"

/**
 @addtogroup Core
 @{
 */
 
/**
 @struct	CzEvent

 @brief	Represents a named event with an actions list.

 An event is something that occurs in your app that you may or may not want to know about and act upon. For example, when the scene is first created the OnCreate event is raised. 
 Using actions (explained in CzAction) you can react to these events and modify your apps behaviour. 

 CzEvent binds a named event with an actions list that will be called when the event takes place. All classes that benefit from receiving events from XOML contain a CzEventManager 
 which handles the binding of named events to actions lists. Lets take a look at q quick XOML example:

 @code
<Label Position="-200, 0" Size="150, 75" Font="serif" Background="Button1Brush" Text="Rock" OnTapped="Selected">
    <Actions Name="Selected">
        <Action Method="SetVar" Param1="Selected" Param2="0"/>
        <Action Method="ChangeProgram" Param1="Main" Param2="start"/>
    </Actions>
</Label>
 @endcode

 In the above example we create a label actor that handles the OnTapped event. When the user taps this label actor it will raise the OnTapped event from CzActor::NotifyTapped) by 
 calling ProcessEventActions(CzHashes::OnTapped_Hash). CzActor::ProcessEventActions() will search the actors events list to see if it is subsribed to the OnTapped event in XOML and 
 if it is then the asociated actions list will be called. In the above example it has the "Selected" actions list mapped to the OnTapped event so this will be called.

 Note that any class that can raise an OnXXXX event implements a method to raise the event called NotifyXXXX.


 */

struct CzEvent
{
	unsigned int	EventName;		///< Event method name as a string hash
	unsigned int	ActionsName;	///< Name of Actions collection as a string hash, to perform when event occurs
//#if defined(_DEBUG)
	CzString		_EventName;		///< Event method name (e.g. OnResume, OnTapped)
	CzString		_ActionsName;	///< Name of Actions collection to perform when event occurs
//#endif	//_DEBUG
};

/**
 @class	CzEventManager

 @brief	Manages a collection of events.

 An events manager handles the mapping of a collection of events to action lists. Any XOML object that can potentially raise an event has an events manager, this includes:
 - CzActor derived actors - Raises events such as OnTapped, OnCreate etc..
 - CzScene derived scenes - Raises events such as OnCreate, OnOrientationChange etc..
 - CzAnimInstance - Raises events such as OnStart, OnEnd etc..
 - CzRemoteReq - Raises events such as OnResponse, OnError etc..
 - CzVideoCam - Raises events such as OnError
 - CzMarket - Raises events such as OnComplete, OnBillingDisabled etc..


 */

class CzEventManager
{
public:
	// Public access to event iteration
	typedef CzList<CzEvent*>::iterator _Iterator;
	_Iterator					begin() { return Events.begin(); }
	_Iterator					end() { return Events.end(); }
protected:
	CzList<CzEvent*> Events;
public:
	CzEventManager() {}
	virtual ~CzEventManager();

	void		addEvent(const char* event_name, const char* actions_name, bool replace = false);
	void		addEvent(CzEvent* evt);
	CzEvent*	findEvent(unsigned int event_name_hash);
};

/// @}


#endif // _CZ_EVENTS_H_