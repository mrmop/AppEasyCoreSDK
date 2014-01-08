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

#if !defined(_CZ_ADS_MEDIATOR_H_)
#define _CZ_ADS_MEDIATOR_H_

#include "CzUtil.h"
#include "CzAds.h"

//
//
// CzAdParty - The CzAdParty structure represents a single ad mediation party which can request ads
//
//
struct CzAdsParty
{
	CzAds::eAdProvider	Provider;					// Ad provider
	CzString			ApplicationID;				// ID of the application thats making the request (you will need to be assigned this from inner-active)
	CzString			OtherID;					// Extra ID information
	CzString			ExtraInfo;					// Pass in any extra pareneters as name vakue pairs, e.g. &city=london&ad_unit=1 (optional)

	CzAdsParty() {}
	virtual ~CzAdsParty()
	{
	}
};

//
//
// CzAdsMediator - The CzAdsMediator class is responsible for mediating ad requests between different ad providers to help impriove fill rates and monetisation
//
//
class CzAdsMediator
{
public:
	// Public access for scene iteration
	typedef CzVector<CzAdsParty*>::iterator	_Iterator;
	_Iterator				begin() { return AdParties.begin(); }
	_Iterator				end() { return AdParties.end(); }

	// Properties
protected:
	CzVector<CzAdsParty*>	AdParties;
	int						NextAdParty;

public:
	void					addAdParty(CzAdsParty* party)
	{
		AdParties.push_back(party);
	}
	void					removeAdParty(CzAdsParty* party)
	{
		for (_Iterator it = AdParties.begin(); it != AdParties.end(); ++it)
		{
			if ((*it) == party)
			{
				delete *it;
				AdParties.erase(it);
				break;
			}
		}
	}
	void					clearParties()
	{
		for (_Iterator it = AdParties.begin(); it != AdParties.end(); ++it)
			delete *it;
		AdParties.clear();
	}
	CzAdsParty*		getNextAdParty()
	{
		int max = AdParties.size();
		if (max == 0)
			return NULL;

		// If we ran out of ad partys then start again from first
		if (NextAdParty >= max)
			NextAdParty = 0;

		return AdParties[NextAdParty++];
	}
	void					reset()
	{
		NextAdParty = 0;
	}

	// Properties end

protected:


public:
	CzAdsMediator() : NextAdParty(0)	{}
	virtual ~CzAdsMediator()
	{
		clearParties();
	}
	
};


#endif	// _CZ_ADS_MEDIATOR_H_