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

#include "s3eIOSAppStoreBilling.h"
#include "s3eAndroidMarketBilling.h"

//
//
//
//
// CzPlatformMarketiOS - Handles in-app purchasing for iOS
//
//
// 
//
class CzPlatformMarketiOS : public IzPlatformMarket
{
	// Properties
protected:
	s3ePaymentRequest*		getPaymentRequest();
	void					setTransactionReceipt(void *receipt_data, uint receipt_size);
	s3eTransactionReceipt*	getTransactionReceipt();
	void					setProductInformation(s3eProductInformation* info);
	s3eProductInformation*	getProductInformation();
	void					setPaymentTransaction(s3ePaymentTransaction* trans);
	s3ePaymentTransaction*	getPaymentTransaction();
	bool					isBillingEnabled();
	// Properties end

protected:
	s3ePaymentRequest		PaymentRequest;
	s3eTransactionReceipt	TransactionReceipt;
	s3eProductInformation*	ProductInformation;
	s3ePaymentTransaction*	PaymentTransaction;

public:

	int						Init(void* id);
	void					Release();
	void					Update();
	bool					QueryProduct(const char* product_id);
	bool					PurchaseProduct(const char* product_id);
	bool					RestoreProducts();

	void					StopProductinfo();
	void					GetReceipt();

	// Internal
	static void				TransactionUpdateCallback(s3ePaymentTransaction* transaction, void* userData);
	static void				ProductInfoCallback(s3eProductInformation* productInfo, void* userData);
	void					ReleaseReceipt();
	void					ReleasePayment();
	void					ReleaseProductInfo();

};

//
//
//
//
// CzPlatformMarketAndroid - Handles in-app purchasing fro Android
//
//
// 
//
class CzPlatformMarketAndroid : public IzPlatformMarket
{
	// Properties
protected:
	CzString				PublicKey;
	CzString				RefundedID;
public:
	void					setPublicKey(const char* key)	{ PublicKey = key; }
	void					setRefundedID(const char* id)	{ RefundedID = id; }
	CzString&				getRefundedID()					{ return RefundedID; }
	bool					isBillingEnabled();
	// Properties end

protected:

public:

	int						Init(void* id);
	void					Release();
	void					Update();
	bool					QueryProduct(const char* product_id);		// Does nothing
	bool					PurchaseProduct(const char* product_id);
	bool					RestoreProducts();

	void					NotifyRefund();


	// Internal
	static int32			AsyncResponse(void* system, void* user);
	static int32			OrderInformationChanged(void* system, void* user);

};

//
//
//
//
// CzPlatformMarketTest - A test class that simply allows testing of in-app purchase without performing any actual purchasing
//
//
// 
//
class CzPlatformMarketTest : public IzPlatformMarket
{
	// Properties
protected:
	bool					isBillingEnabled();
	// Properties end

protected:
	CzTimer					PurchaseTimer;

public:

	int						Init(void* id);
	void					Release();
	void					Update();
	bool					QueryProduct(const char* product_id);
	bool					PurchaseProduct(const char* product_id);
	bool					RestoreProducts();

	void					ForceError(eMarketStatus status);
};



#endif	// _CCZ_PLATFORM_MARKET_H_
