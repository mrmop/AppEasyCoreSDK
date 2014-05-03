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

#include "Marm/CzPlatformMarket.h"

#include "CzMarket.h"
#include "CzFile.h"
#include "CzScene.h"
#include "CzApp.h"
#include "CzHashes.h"
#include "s3e.h"

//
//
//
// CzPlatformMarket implementation
//
//
//
bool CzPlatformMarket::isAvailable(eMarketVendor vendor)
{
	return IwBilling::isAvailable((IwBilling::eIwBillingVendor)vendor);
}

int CzPlatformMarket::Init(void* id, eMarketVendor vendor)
{
	if (!IwBilling::Init(ReadyCallback, id, (IwBilling::eIwBillingVendor)vendor))
		return 0;
	IwBilling::setConsumeCallback(CzPlatformMarket::ConsumeCallback);
	IwBilling::setErrorCallback(CzPlatformMarket::ErrorCallback);
	IwBilling::setInfoAvailableCallback(CzPlatformMarket::ProductInfoAvailableCallback);
	IwBilling::setReceiptAvailableCallback(CzPlatformMarket::ReceiptAvailableCallback);
	return 1;
}

void CzPlatformMarket::Release()
{
	IwBilling::Terminate();
}

bool CzPlatformMarket::PurchaseProduct(const char* product_id)
{
	return IwBilling::PurchaseProduct(product_id);
}

bool CzPlatformMarket::QueryProducts(const char** product_ids, int num_products)
{
	return IwBilling::QueryProducts(product_ids, num_products);
}

bool CzPlatformMarket::RestoreProducts()
{
	return IwBilling::RestoreTransactions();
}

bool CzPlatformMarket::ConsumeProduct(const char* purchase_token)
{
	return IwBilling::ConsumeProduct(purchase_token);
}

bool CzPlatformMarket::FinishTransaction(const char* finish_data)
{
	return IwBilling::FinishTransaction((void*)finish_data);
}

void CzPlatformMarket::setItemRange(int start, int end)
{
	IwBilling::setItemRange(start, end);
}

void CzPlatformMarket::setPayload(const char* payload)
{
	IwBilling::setPayload(payload);
}

void CzPlatformMarket::setTestMode(bool mode)
{
	IwBilling::setTestMode(mode);
}


const char* CzPlatformMarket::getPayload()
{
	return IwBilling::getPayload().c_str();
}

void CzPlatformMarket::ReadyCallback(void* caller, void* data)
{
	if (PLATFORM_MARKET->getActiveMarket() != NULL)
		PLATFORM_MARKET->getActiveMarket()->NotifyReady();
}

void CzPlatformMarket::ErrorCallback(void* userdata, IwBilling::CIwBillingErrorData* data)
{
	if (PLATFORM_MARKET->getActiveMarket() != NULL)
	{
		PLATFORM_MARKET->getActiveMarket()->NotifyError(data->ProductID, data->Error);
	}
}

void CzPlatformMarket::ProductInfoAvailableCallback(void* userdata, IwBilling::CIwBillingInfoAvailableData* data)
{
	if (PLATFORM_MARKET->getActiveMarket() != NULL)
	{
		CzMarketProductData d;
		d.Title = data->Title;
		d.Description = data->Description;
		d.Price = data->Price;
		d.ProductID = data->ProductID;
		PLATFORM_MARKET->getActiveMarket()->NotifyInfoAvailable(&d);
	}
}

void CzPlatformMarket::ReceiptAvailableCallback(void* userdata, IwBilling::CIwBillingReceiptAvailableData* data)
{
	if (PLATFORM_MARKET->getActiveMarket() != NULL)
	{
		CzMarketProductReceipt d;
		d.ProductID = data->ProductID;
		d.TransactionID = data->TransactionID;
		d.Date = data->Date;
		d.SubscriptionStartDate = data->SubscriptionStartDate;
		d.SubscriptionEndDate = data->SubscriptionEndDate;
		d.Receipt = data->Receipt;
		d.ReceiptLength = data->ReceiptLength;
		d.FinaliseData = data->FinaliseData;
		d.PurchaseToken = data->PurchaseToken;
		d.Payload = data->Payload;
		d.Restored = data->Restored;
		PLATFORM_MARKET->getActiveMarket()->NotifyComplete(&d);
	}
}

void CzPlatformMarket::ConsumeCallback(void* userdata, void* data)
{
}

