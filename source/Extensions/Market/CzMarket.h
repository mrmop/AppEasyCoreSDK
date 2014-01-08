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
	bool				Purchased;	// Purchased state
	bool				Consumable;	// True if this is a consumable item that can be repurchased when depleted
	float				Price;		// Price of product

	CzMarketProduct() : Purchased(false), Consumable(false), Price(0) {}

	bool				Save();
	bool				Load();
};


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
	CzList<CzMarketProduct*> Products;				///< Available products
	CzEventManager*			EventsManager;			///< List of events that the animation handles
	CzScriptCallback		ScriptCallback;			///< Script call back
	CzString				CurrentProductID;		///< Current Product ID (e.g. com.companyname.appname.productname)
	bool					Busy;					///< When true the market is busy
public:
	CzEventManager*			getEventsManager()									{ return EventsManager; }
	void					addProduct(CzMarketProduct* product)				{ Products.push_back(product); }
	void					removeProduct(CzMarketProduct* product);
	CzMarketProduct*		findProduct(const char* product_id);
	CzMarketProduct*		findProductByName(unsigned int name_hash);
	void					setCurrentProductID(const char* product_id);
	const char*				getCurrentProductID() const							{ return CurrentProductID.c_str(); }
	int						getProductCount() const								{ return Products.size(); }
	void					setPurchased(const char* product_id, bool purchased = true);
	CzScriptCallback&		getScriptCallback()									{ return ScriptCallback; }
	bool					isBusy() const										{ return Busy; }
	void					setBusy(bool busy)									{ Busy = busy; }
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
	bool					RestoreProducts();

	void					NotifyUnavailable();
	void					NotifyComplete();
	void					NotifyError();
	void					NotifyBillingDisabled();
	void					NotifyRefund();
	void					NotifyInfoAvailable();

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

