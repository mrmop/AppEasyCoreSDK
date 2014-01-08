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

#if !defined(_CZ_ACTIONS_MARKET_H_)
#define _CZ_ACTIONS_MARKET_H_

#include "CzActions.h"
#include "CzXoml.h"

/**
 @addtogroup Actions
 @{
 */
 
/**
 @class	CzXomlActions_Market

 @brief	Implements actions that relate to in-app purchasing (the Market)

 The CzXomlActions_Market class holds Actions (IzXomlAction) that deal with making in-app purchases such as purchasing a product. A market action is basically a command that 
 causes some kind of interaction with the Market system and / or app store in-app purchasing system.
 
 The following actions are supported:
 - Purchase		- Makes an in-app purchase request

 The above actions have the following XOML prototypes:
 - Purchase(product-name)
   - product-name - The name of the product as assigned in the Market resource

 For more information on the in-app purchasing system see CzMarket
 
 */

class CzXomlActions_Market : public IzXomlAction
{
public:
	enum eActionType
	{
		Action_Purchase			= 0, 
		Action_Max				= 1, 
	};
protected:
	eActionType	Type;
	CzXomlActions_Market() {}
public:
	CzXomlActions_Market(eActionType type)
	{
		Type = type;
		switch (Type)
		{
		case Action_Purchase:
			setActionName("purchase");
			break;
		}
	}
	void Execute(IzXomlResource* source, CzAction* action);
};

/// @}

#endif // _CZ_ACTIONS_MARKET_H_
