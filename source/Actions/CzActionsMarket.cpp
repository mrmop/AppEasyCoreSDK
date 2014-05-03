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

#include "CzMarket.h"
#include "CzActionsMarket.h"
#include "CzHashes.h"
#include "CzApp.h"
#include "CzScene.h"
#include "CzActor.h"

/**
 @fn	void CzXomlActions_Market::Execute(IzXomlResource* source, CzAction* action)

 @brief	The main actions executor for market related actions. All market (in-app purchase) specific actions are implemented by this method

 @param [in]	source - The source of the action
 @param [in]	action - The action to execute
 */
void CzXomlActions_Market::Execute(IzXomlResource* source, CzAction* action)
{
	if (source == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Source was not found for action - ", action->_MethodName.c_str(), action->Parent->getDebugInfo().c_str());
#endif
		return;
	}

	// Determine the container
	CzApp* game;
	CzScene* scene;
	CzActor* actor;
	IzXomlResource* cont = IzXomlAction::getContainer(source, game, scene, actor);

	switch (Type)
	{
	case Action_Purchase:
		{
			CzString* p1 = NULL;

			if (!action->Params[0].isEmpty())
				p1 = &action->getParameter1(cont);
			
			if (p1 == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - Purchase - Product invalid - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
			else
			{
				if (PLATFORM_MARKET == NULL)
					CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Action - Purchase - Market has not been created", action->Parent->getDebugInfo().c_str());
				else
				if (PLATFORM_MARKET->getActiveMarket() == NULL)
					CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Action - Purchase - No active market available", action->Parent->getDebugInfo().c_str());
				else
				{
					CzMarketProduct* product = PLATFORM_MARKET->getActiveMarket()->findProductByName(p1->getHash());
					if (product != NULL)
						PLATFORM_MARKET->getActiveMarket()->PurchaseProduct(product->ProductID.c_str());
					else
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - Purchase - Market product not found - ", p1->c_str(), action->Parent->getDebugInfo().c_str());
				}
			}
		}
		break;
	}
}
