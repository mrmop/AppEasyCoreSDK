#if !defined(_CPLATFORM_MARKET_H_)
#define _CPLATFORM_MARKET_H_

#include "IzPlatformSys.h"
#include "IzPlatformMarket.h"
#include "CzString.h"
#include "CzUtil.h"
#include "CzXoml.h"
#include "CzActions.h"
#include "CzEvents.h"
#include "CzScript.h"
#include "CzTime.h"

//
//
// CzMarketProduct - A product that can be purchased
// 
//
struct CzMarketProduct
{
public:
	CzString			Name;		// Name of product
	CzString			ProductID;	// External product ID (com.companyname.appname.productname for example)
	float				Price;		// Price of product

	CzMarketProduct() : Price(0) {}

	bool				Save();
	bool				Load();
};

typedef struct CzMarketProductData
{
	const char* ProductID;              ///< Product ID
	const char* Title;                  ///< Product title
	const char* Description;            ///< Product description
	const char* Price;                  ///< Product price
} CzMarketProductData;

typedef struct CzMarketProductReceipt
{
	const char* ProductID;              ///< Product ID
	const char* TransactionID;          ///< Transaction ID
	const char* Date;                   ///< Date of purchase
	const char* SubscriptionStartDate;  ///< Date of subscription start (Amazon / BlackBerry only)
	const char* SubscriptionEndDate;    ///< Date of subscription end (Amazon / BlackBerry only)
	char*       Receipt;                ///< Transaction receipt
	int         ReceiptLength;          ///< Transaction receipt length
	void*       FinaliseData;           ///< Data used to finalise the transaction
	const char* PurchaseToken;          ///< Purchase token used to consume items on Android
	const char* Payload;                ///< Developer payload (used by Google Play)
	bool        Restored;               ///< true if item was restored
} CzMarketProductReceipt;



//
//
//
//
// CzMarket - A market object that can be declared and instantiated from XOML
//
//
// 
//
class CzMarket : public IzXomlResource
{
public:
	typedef CzList<CzMarketProduct*>::iterator _ProductIterator;
	_ProductIterator				begin() { return Products.begin(); }
	_ProductIterator				end() { return Products.end(); }

protected:
	// Properties
	IzPlatformMarket::eMarketVendor	Vendor;			///< Market vendor
	CzList<CzMarketProduct*> Products;				///< Available products
	CzEventManager*			EventsManager;			///< List of events that the animation handles
	CzScriptCallback		ScriptCallback;			///< Script call back
	CzString				CurrentProductID;		///< Current Product ID (e.g. com.companyname.appname.productname)
	bool					Busy;					///< When true the market is busy
public:
	bool					isAvailable();
	void					setVendor(IzPlatformMarket::eMarketVendor vendor)	{ Vendor = vendor; }
	CzEventManager*			getEventsManager()									{ return EventsManager; }
	void					addProduct(CzMarketProduct* product)				{ Products.push_back(product); }
	void					removeProduct(CzMarketProduct* product);
	CzMarketProduct*		findProduct(const char* product_id);
	CzMarketProduct*		findProductByName(unsigned int name_hash);
	void					setCurrentProductID(const char* product_id);
	const char*				getCurrentProductID() const							{ return CurrentProductID.c_str(); }
	int						getProductCount() const								{ return Products.size(); }
	CzScriptCallback&		getScriptCallback()									{ return ScriptCallback; }
	bool					isBusy() const										{ return Busy; }
	void					setBusy(bool busy)									{ Busy = busy; }
	void					setItemRange(int start, int end);
	void					setPayload(const char* payload);
	const char*				getPayload();
	void					setTestMode(bool mode);
	// Properties end

protected:
	void					ProcessEventActions(unsigned int event_name, IzXomlResource* parent);

public:
	CzMarket() : IzXomlResource(), EventsManager(NULL), Busy(false) { setClassType("market"); }
	virtual ~CzMarket()
	{
		Release();
	}

	int						Init(const char* public_key);
	void					Release();

	void					Save();
	void					Load();

	// Implementation of IzXomlClass interface
	int						LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	bool					QueryProduct(const char* product_id);
	bool					PurchaseProduct(const char* product_id);
	bool					ConsumeProduct(const char* purchase_token);
	bool					FinishTransaction(const char* finish_data);
	bool					RestoreProducts();

	void					NotifyReady();
	void					NotifyUnavailable();
	void					NotifyComplete(CzMarketProductReceipt* data);
	void					NotifyError(const char* product_id, int error);
	void					NotifyBillingDisabled();
	void					NotifyInfoAvailable(CzMarketProductData* data);

	static IzPlatformMarket::eMarketVendor	VendorFromText(const char* vendor);
};

//
//  CzMarketCreator - Creates an instance of the market 
//
class CzMarketCreator : public IzXomlClassCreator
{
public:
	CzMarketCreator()
	{
		setClassName("market");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzMarket(); }
};


#endif	// _CPLATFORM_MARKET_H_

