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

#if !defined(_CCZ_PLATFORM_MARKET_H_)
#define _CCZ_PLATFORM_MARKET_H_

#include "IzPlatformMarket.h"

#include "IwBilling.h"

class CzPlatformMarket : public IzPlatformMarket
{
	// Properties
protected:
	// Properties end

protected:

public:

	bool					isAvailable(eMarketVendor vendor);
	int						Init(void* id, eMarketVendor vendor);
	void					Release();
	bool					QueryProducts(const char** product_ids, int num_products);
	bool					PurchaseProduct(const char* product_id);
	bool					RestoreProducts();
	bool					ConsumeProduct(const char* purchase_token);
	bool					FinishTransaction(const char* finish_data);
	void					setItemRange(int start, int end);
	void					setPayload(const char* payload);
	const char*				getPayload();
	void					setTestMode(bool mode);

	// Internal
	static void ReadyCallback(void* caller, void* data);
	static void ErrorCallback(void* userdata, IwBilling::CIwBillingErrorData* data);
	static void ProductInfoAvailableCallback(void* userdata, IwBilling::CIwBillingInfoAvailableData* data);
	static void ReceiptAvailableCallback(void* userdata, IwBilling::CIwBillingReceiptAvailableData* data);
	static void ConsumeCallback(void* userdata, void* data);

};



#endif	// _CCZ_PLATFORM_MARKET_H_
