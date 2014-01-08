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

#if !defined(_CZ_ACTIONS_COMMS_H_)
#define _CZ_ACTIONS_COMMS_H_

#include "CzActions.h"
#include "CzXoml.h"

/**
 @addtogroup Actions
 @{
 */
 
/**
 @class	CzXomlActions_Comms

 @brief	Implements a collection of comms actions.

 The CzXomlActions_Comms class holds Actions (IzXomlAction) that deal with communications such as sending remote requests. A comms action is basically a command that 
 causes some kind of communication between a server or another device to take place. 
 
 The following actions are supported:
 - RemoteReq		- Processes a remote rquest

 The above actions have the following XOML prototypes:
 - RemoteReq(remote-request-name, data-to-send(optional))
   - remote-request-name - Name of RemoteReq resource to call
   - data-to-send - Data to send to the request (replaced that which is supplied by the remote request definition)

 For more information on the remote request system see CzRemoteReq
 
 */

class CzXomlActions_Comms : public IzXomlAction
{
public:
	enum eActionType
	{
		Action_RemoteReq			= 0, 
		Action_Max					= 1, 
	};
protected:
	eActionType	Type;
	CzXomlActions_Comms() {}
public:
	CzXomlActions_Comms(eActionType type)
	{
		Type = type;
		switch (Type)
		{
		case Action_RemoteReq:
			setActionName("remotereq");
			break;
		}
	}
	void Execute(IzXomlResource* source, CzAction* action);
};

/// @}

#endif // _CZ_ACTIONS_COMMS_H_