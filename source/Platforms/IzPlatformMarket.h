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

#if !defined(_CIZ_PLATFORM_MARKET_H_)
#define _CIZ_PLATFORM_MARKET_H_

#include "CzPlatform.h"
#include "CzMarket.h"

#define	PLATFORM_MARKET			PLATFORM->getMarket()

class CzMarket;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IzPlatformMarket
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IzPlatformMarket
{
public:
	enum eMarketStatus
	{
		MS_IDLE, 
		MS_PURCHASING, 
		MS_PURCHASE_COMPLETED, 
		MS_RESTORE_COMPLETED, 
		MS_ERROR_CLIENT_INVALID, 
		MS_ERROR_PAYMENT_CANCELLED, 
		MS_ERROR_PAYMENT_INVALID, 
		MS_ERROR_PAYMENT_NOT_ALLOWED, 
		MS_ERROR_PURCHASE_UNKNOWN, 
		MS_ERROR_PURCHASE_DISABLED, 
		MS_ERROR_NO_CONNECTION, 
		MS_ERROR_RESTORE_FAILED, 
		MS_ERROR_UNKNOWN_PRODUCT, 
		MS_ERROR_DEVELOPER_ERROR, 
		MS_ERROR_UNAVAILABLE 
	};

	// Properties
protected:
	bool					Available;						///< Is available on this platform
	bool					Initialised;					///< Initialised state
	eMarketStatus			Status;							///< Markets current status
	CzMarket*				ActiveMarket;					///< Currently active market
	unsigned int			SimulationHash;					///< Name of sinulation to perform
public:
	bool					isAvailable() const										{ return Available; }
	bool					isInitialised() const									{ return Initialised; }
	eMarketStatus			getStatus() const										{ return Status; }
	void					setStatus(eMarketStatus status)							{ Status = status; }
	CzMarket*				getActiveMarket()										{ return ActiveMarket; }
	void					setActiveMarket(CzMarket* market)						{ ActiveMarket = market; }
	void					setSimulation(unsigned int sim)							{ SimulationHash = sim; }
	unsigned int			getSimulation() const									{ return SimulationHash; }
	virtual bool			isBillingEnabled() = 0;			///< Returns true if billing enabled on device
	// Properties end
public:
	IzPlatformMarket() : SimulationHash(0), Available(false), Initialised(false), ActiveMarket(NULL), Status(MS_IDLE) 	{}

	// PLatform specific implementations
	virtual int				Init(void* id) = 0;								///< Market intialisation
	virtual void			Release() = 0;									///< Market cleanup
	virtual void			Update() = 0;									///< Per frame update
	virtual bool			QueryProduct(const char* product_id) = 0;		///< Query product information
	virtual bool			PurchaseProduct(const char* product_id) = 0;	///< Purchase a product
	virtual bool			RestoreProducts() = 0;							///< Restore previously purchased products

	// Event notification callbacks (platform agnostic, although you can override the event notification methods)
protected:
public:
	// Event notification

};





#endif	// _CIZ_PLATFORM_MARKET_H_
