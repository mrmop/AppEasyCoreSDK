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
	enum eMarketVendor
	{
		MV_VENDOR_NONE,
		MV_VENDOR_APPLE,
		MV_VENDOR_BLACKBERRY,
		MV_VENDOR_WP8,
		MV_VENDOR_WS8,
		MV_VENDOR_TIZEN,
		MV_VENDOR_GOOGLE_PLAY,
		MV_VENDOR_AMAZON,
		MV_VENDOR_ANDROID_MARKET,
		MV_VENDOR_SAMSUNG,
		MV_VENDOR_MAX
	};

	enum eMarketStatus
	{
		MS_ERROR_CLIENT_INVALID,
		MS_ERROR_PAYMENT_CANCELLED,
		MS_ERROR_PAYMENT_INVALID,
		MS_ERROR_PAYMENT_NOT_ALLOWED,
		MS_ERROR_PURCHASE_UNKNOWN,
		MS_ERROR_PURCHASE_DISABLED,
		MS_ERROR_PURCHASE_CANCELED,
		MS_ERROR_NO_CONNECTION,
		MS_ERROR_RESTORE_FAILED,
		MS_ERROR_UNKNOWN_PRODUCT,
		MS_ERROR_DEVELOPER_ERROR,
		MS_ERROR_UNAVAILABLE,
		MS_ERROR_ALREADY_OWNED,
		MS_ERROR_NOT_OWNED,
		MS_ERROR_FAILED,
		MS_ERROR_PENDING,
		MS_ERROR_NOT_READY,
		MS_ERROR_SECURITY_FAILED,
	};

	// Properties
protected:
	eMarketVendor			CurrentVendor;
	CzMarket*				ActiveMarket;					///< Currently active market
public:
	CzMarket*				getActiveMarket()										{ return ActiveMarket; }
	void					setActiveMarket(CzMarket* market)						{ ActiveMarket = market; }
	// Properties end
public:
	IzPlatformMarket() : CurrentVendor(MV_VENDOR_NONE), ActiveMarket(NULL) {}

	// PLatform specific implementations
	virtual bool			isAvailable(eMarketVendor vendor) = 0;			///< Check vendor availability
	virtual int				Init(void* id, eMarketVendor vendor) = 0;		///< Market intialisation
	virtual void			Release() = 0;									///< Market cleanup
	virtual bool			QueryProducts(const char** product_ids, int num_products) = 0;		///< Query product information
	virtual bool			PurchaseProduct(const char* product_id) = 0;	///< Purchase a product
	virtual bool			RestoreProducts() = 0;							///< Restore previously purchased products
	virtual bool			ConsumeProduct(const char* purchase_token) = 0;	///< Comsumes a consumable product (Google Play only)
	virtual bool			FinishTransaction(const char* finish_data) = 0;	///< Finalises a transaction after purchase
	virtual void			setItemRange(int start, int end) = 0;
	virtual void			setPayload(const char* payload) = 0;
	virtual const char*		getPayload() = 0;
	virtual void			setTestMode(bool mode) = 0;

	// Event notification callbacks (platform agnostic, although you can override the event notification methods)
protected:
public:
	// Event notification

};





#endif	// _CIZ_PLATFORM_MARKET_H_
