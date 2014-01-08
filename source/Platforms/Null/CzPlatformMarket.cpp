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
// IzPlatformMarket implementation
//
//
//

void IzPlatformMarket::NotifyInfoAvailable()
{
	if (ActiveMarket == NULL)
		return;

	if (InfoAvailableHandler != NULL)
		InfoAvailableHandler(this, InfoAvailableHandlerData);
	ActiveMarket->setBusy(false);
}

void IzPlatformMarket::NotifyReceiptAvailable()
{
	if (ActiveMarket == NULL)
		return;

	if (ReceiptAvailableHandler != NULL)
		ReceiptAvailableHandler(this, ReceiptAvailableHandlerData);

	if (ActiveMarket != NULL)
		ActiveMarket->NotifyComplete();
	ActiveMarket->setBusy(false);
}

void IzPlatformMarket::NotifyError()
{
	if (ActiveMarket == NULL)
		return;

	if (ErrorHandler != NULL)
		ErrorHandler(this, ErrorHandlerData);

	if (ActiveMarket != NULL)
		ActiveMarket->NotifyError();
	ActiveMarket->setBusy(false);
}

void IzPlatformMarket::NotifyAlreadyPurchased()
{
	if (ActiveMarket == NULL)
		return;

	if (AlreadyPurchasedHandler != NULL)
		AlreadyPurchasedHandler(this, AlreadyPurchasedHandlerData);

	if (ActiveMarket != NULL)
		ActiveMarket->NotifyError();
	ActiveMarket->setBusy(false);
}

void IzPlatformMarket::NotifyBillingDisabled()
{
	if (ActiveMarket == NULL)
		return;

	if (BillingDisabledeHandler != NULL)
		BillingDisabledeHandler(this, BillingDisabledeHandlerData);

	if (ActiveMarket != NULL)
		ActiveMarket->NotifyBillingDisabled();
	ActiveMarket->setBusy(false);
}

void IzPlatformMarket::NotifyRefund()
{
	if (ActiveMarket == NULL)
		return;

	if (RefundHandler != NULL)
		RefundHandler(this, RefundHandlerData);

	if (ActiveMarket != NULL)
		ActiveMarket->NotifyRefund();
	ActiveMarket->setBusy(false);
}


//
//
//
// CzPlatformMarketiOS implementation
//
//
//
bool CzPlatformMarketiOS::isBillingEnabled()
{
	if (s3eIOSAppStoreBillingGetInt(S3E_IOSAPPSTOREBILLING_CAN_MAKE_PAYMENTS))
		return true;

	return false;
}

int CzPlatformMarketiOS::Init(void* id)
{
	Initialised = false;
	Available = false;
	ActiveMarket = NULL;
	Status = MS_IDLE;

	if (s3eIOSAppStoreBillingAvailable())
		Available = true;
	else
		return 0;

	s3eIOSAppStoreBillingStart(ProductInfoCallback, TransactionUpdateCallback, NULL);

	TransactionReceipt.m_ReceiptData = NULL;
	ProductInformation = NULL;
	PaymentTransaction = NULL;

	Initialised = true;
	return 1;
}

void CzPlatformMarketiOS::Release()
{
	ReleaseProductInfo();
	ReleaseReceipt();
	ReleasePayment();

	s3eIOSAppStoreBillingTerminate();
	Initialised = false;
	ActiveMarket = NULL;
}

bool CzPlatformMarketiOS::PurchaseProduct(const char* product_id)
{
	if (ActiveMarket == NULL || !Available || ActiveMarket->isBusy())
		return false;

	// Find the product
	CzMarketProduct* product = ActiveMarket->findProduct(product_id);
	if (product == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Product not found - ", product_id);
		return false;
	}

	// If product is not a consumable and is already purchased then we do not attempt to purchase again
	if (product->Purchased && !product->Consumable)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Product already purchased - ", product_id);
		NotifyAlreadyPurchased();
		return false;
	}
	getActiveMarket()->setCurrentProductID(product_id);

	// Setup product request
	strcpy(PaymentRequest.m_ProductID, product->ProductID.c_str());
	PaymentRequest.m_Quantity = 1;

	if (s3eIOSAppStoreBillingRequestPayment(&PaymentRequest) == S3E_RESULT_SUCCESS)
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Purchasing  - ", product_id);
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Purchasing failed - ", product_id);
		NotifyError();
		return false;
	}
	ActiveMarket->setBusy(true);

	return true;
}

bool CzPlatformMarketiOS::QueryProduct(const char* product_id)
{
	if (ActiveMarket == NULL || !Available || ActiveMarket->isBusy())
		return false;

	// Find the product
	CzMarketProduct* product = ActiveMarket->findProduct(product_id);
	if (product == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Product not found - ", CzString(product_id).c_str());
		return false;
	}

	getActiveMarket()->setCurrentProductID(product_id);

	// Begin product info retrieval
	if (s3eIOSAppStoreBillingRequestProductInformation(&product_id, 1) == S3E_RESULT_SUCCESS)
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Requesting product info - ", product_id);
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Requesting product info failed - ", product_id);
		NotifyError();
		return false;
	}

	ActiveMarket->setBusy(true);

	return true;
}

s3ePaymentRequest* CzPlatformMarketiOS::getPaymentRequest()
{
	return &PaymentRequest;
}

void CzPlatformMarketiOS::setTransactionReceipt(void *receipt_data, uint receipt_size)
{
	TransactionReceipt.m_ReceiptSize = receipt_size;
	TransactionReceipt.m_ReceiptData = CZ_Malloc(receipt_size + 1);
	memcpy(TransactionReceipt.m_ReceiptData, receipt_data, receipt_size);
	PaymentTransaction->m_TransactionReceipt = &TransactionReceipt;
}

s3eTransactionReceipt* CzPlatformMarketiOS::getTransactionReceipt()
{
	return &TransactionReceipt;
}

void CzPlatformMarketiOS::setProductInformation(s3eProductInformation* info)
{
	ReleaseProductInfo();
	ProductInformation = (s3eProductInformation*)CZ_Malloc(sizeof(s3eProductInformation));
	memcpy(ProductInformation, info, sizeof(s3eProductInformation));
}

s3eProductInformation* CzPlatformMarketiOS::getProductInformation()
{
	return ProductInformation;
}

void CzPlatformMarketiOS::setPaymentTransaction(s3ePaymentTransaction* trans)
{
	ReleasePayment();
	PaymentTransaction = (s3ePaymentTransaction*)CZ_Malloc(sizeof(s3ePaymentTransaction));
	memcpy(PaymentTransaction, trans, sizeof(s3ePaymentTransaction));
	PaymentTransaction->m_Request = &PaymentRequest;
}

s3ePaymentTransaction*	CzPlatformMarketiOS::getPaymentTransaction()
{
	return PaymentTransaction;
}

void CzPlatformMarketiOS::ReleaseReceipt()
{
	if (TransactionReceipt.m_ReceiptData != NULL)
	{
		CZ_Free(TransactionReceipt.m_ReceiptData);
		TransactionReceipt.m_ReceiptData = NULL;
	}
}

void CzPlatformMarketiOS::ReleasePayment()
{
	if (PaymentTransaction != NULL)
	{
		CZ_Free(PaymentTransaction);
		PaymentTransaction = NULL;
	}
}

void CzPlatformMarketiOS::ReleaseProductInfo()
{
	if (ProductInformation != NULL)
	{
		CZ_Free(ProductInformation);
		ProductInformation = NULL;
	}
}

void CzPlatformMarketiOS::Update()
{
}

//
// TransactionUpdateCallback
//
// When a request ismade to make a transaction with s3eIOSAppStoreBillingRequestPayment() this callback is called when the status of the 
// transaction changes or an error occurs
//
void CzPlatformMarketiOS::TransactionUpdateCallback(s3ePaymentTransaction* transaction, void* userData)
{
	CzPlatformMarketiOS* market = (CzPlatformMarketiOS*)PLATFORM_MARKET;
	if (market == NULL || market->getActiveMarket() == NULL)
		return;

	const char* payment_product_id = market->getPaymentRequest()->m_ProductID;
	const char* transaction_product_id = transaction->m_Request->m_ProductID;
	if (transaction_product_id == 0)
		return;
	
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "transaction->m_TransactionStatus - ", CzString((int)transaction->m_TransactionStatus).c_str());

	market->getActiveMarket()->setCurrentProductID(transaction_product_id);

	switch (transaction->m_TransactionStatus)
	{
		case S3E_PAYMENT_STATUS_PENDING:
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Buying in progress - ", transaction_product_id);
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Transaction ID is - ", transaction->m_TransactionID);
			market->setStatus(MS_PURCHASING);
			break;
		case S3E_PAYMENT_STATUS_PURCHASED:
		case S3E_PAYMENT_STATUS_RESTORED:
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "PaymentRequest.m_ProductID - ", payment_product_id);
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "PaymentTransaction->m_Request->m_ProductID - ", transaction_product_id);

			// Check product ID of completed transaction
			if (!strcmp(transaction_product_id, payment_product_id))
			{
				market->ReleaseReceipt();
				market->ReleasePayment();

				if (transaction->m_TransactionStatus == S3E_PAYMENT_STATUS_PURCHASED)
				{
					CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Purchase completed - ", transaction_product_id);
					market->setStatus(MS_PURCHASE_COMPLETED);
				}
				else
				{
					CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Restoring completed - ", transaction_product_id);
					market->setStatus(MS_RESTORE_COMPLETED);
				}

				// Copy transaction to local memory
				market->setPaymentTransaction(transaction);

				// Copy receipt data to memory
				market->setTransactionReceipt(transaction->m_TransactionReceipt->m_ReceiptData, transaction->m_TransactionReceipt->m_ReceiptSize);

				// Mark product as purchased
				market->getActiveMarket()->setPurchased(transaction_product_id);

				// Notify user that the purchase receipt is available
				market->NotifyReceiptAvailable();

				// Complete the transaction (if you wish to validate the receipt data then you need to remove this next line and call it after validation)
				s3eIOSAppStoreBillingCompleteTransaction(transaction, S3E_TRUE);

				break;
			}
			else
			{
				// We enter here if a transaction was started in a previous session
				if (transaction->m_TransactionStatus == S3E_PAYMENT_STATUS_PURCHASED)
				{
					CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Buying completed for unexpected product ID - ", transaction_product_id);
					market->setStatus(MS_PURCHASE_COMPLETED);
				}
				else
				{
					CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Restoring completed for unexpected product ID - ", transaction_product_id);
					market->setStatus(MS_RESTORE_COMPLETED);
				}

				// Find the product that was purchased / restored
				CzMarketProduct* product = market->getActiveMarket()->findProduct(transaction_product_id);
				if (product != NULL)
				{
					CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Product Purchased / Restored with ProductID - ", transaction_product_id);
					market->setPaymentTransaction(transaction);
					market->setTransactionReceipt(transaction->m_TransactionReceipt->m_ReceiptData, transaction->m_TransactionReceipt->m_ReceiptSize);
					market->getActiveMarket()->setPurchased(transaction_product_id);
					market->NotifyReceiptAvailable();
					s3eIOSAppStoreBillingCompleteTransaction(transaction, S3E_TRUE);
				}
				else
					market->NotifyError();	// Product was not found
			}
			break;

		case S3E_PAYMENT_STATUS_FAILED_CLIENT_INVALID:
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Buying failed. Client is not allowed to make the payment - ", transaction_product_id);
			market->setStatus(MS_ERROR_CLIENT_INVALID);
			market->NotifyError();
			break;

		case S3E_PAYMENT_STATUS_FAILED_PAYMENT_CANCELLED:
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Buying failed. User cancelled the purchase - ", transaction_product_id);
			market->setStatus(MS_ERROR_PAYMENT_CANCELLED);
			market->NotifyError();
			break;

		case S3E_PAYMENT_STATUS_FAILED_PAYMENT_INVALID:
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Buying failed. Invalid parameter/purchase ID - ", transaction_product_id);
			market->setStatus(MS_ERROR_PAYMENT_INVALID);
			market->NotifyError();
			break;

		case S3E_PAYMENT_STATUS_FAILED_PAYMENT_NOT_ALLOWED:
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Buying failed. Device is not allowed to make the payment (check restriction settings) - ", transaction_product_id);
			market->setStatus(MS_ERROR_PAYMENT_NOT_ALLOWED);
			market->NotifyError();
			break;

		default:
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Buying failed for unknown reason - ", transaction_product_id);
			market->setStatus(MS_ERROR_PURCHASE_UNKNOWN);
			market->NotifyError();
			break;
	}
}

//
//
//
// TransactionUpdateCallback
//
// When a request is made to retrieve product information using s3eIOSAppStoreBillingRequestProductInformation() this callback is called when the product 
// informatin is available ot an error occurs
//
//
//
void CzPlatformMarketiOS::ProductInfoCallback(s3eProductInformation* productInfo, void* userData)
{
	CzPlatformMarketiOS* market = (CzPlatformMarketiOS*)PLATFORM_MARKET;
	if (productInfo == 0 || market == NULL || market->getActiveMarket() == NULL)
		return;

	market->setProductInformation(productInfo);

	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "ProductID - ", market->getProductInformation()->m_ProductID);
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Title - ",  market->getProductInformation()->m_LocalisedTitle);

	switch (market->getProductInformation()->m_ProductStoreStatus)
	{
		case S3E_PRODUCT_STORE_STATUS_VALID:
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Product - ", market->getProductInformation()->m_LocalisedDescription);
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Price - ", CzString((float)market->getProductInformation()->m_Price / 100.0f).c_str());
			if (s3eIOSAppStoreBillingGetInt(S3E_IOSAPPSTOREBILLING_CAN_MAKE_PAYMENTS) != 0)
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Purchasing is disabled on device - ", market->getProductInformation()->m_LocalisedDescription);
				market->setStatus(MS_ERROR_PURCHASE_DISABLED);
				market->NotifyBillingDisabled();
			}
			else
				market->NotifyInfoAvailable();
			break;
		}
		case S3E_PRODUCT_STORE_STATUS_NO_CONNECTION:
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Could not connect to store. Ongoing requests have been cancelled. Please check connection.");
			market->setStatus(MS_ERROR_NO_CONNECTION);
			market->NotifyError();
			break;
		case S3E_PRODUCT_STORE_STATUS_RESTORE_FAILED:
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Restore products failed. Please try again.");
			market->setStatus(MS_ERROR_RESTORE_FAILED);
			market->NotifyError();
			break;
		case S3E_PRODUCT_STORE_STATUS_RESTORE_COMPLETED:
			break;
		default: // Item not found
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Product not found in store - ", market->getActiveMarket()->getCurrentProductID());
			market->setStatus(MS_ERROR_UNKNOWN_PRODUCT);
			market->NotifyError();
	}
}

void CzPlatformMarketiOS::StopProductinfo()
{
	if (!Available)
		return;

	s3eIOSAppStoreBillingCancelProductInformationRequests();
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Product info request cancelled");
}

bool CzPlatformMarketiOS::RestoreProducts()
{
	if (!Available)
		return false;

	s3eIOSAppStoreBillingRestoreCompletedTransactions();
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Restoring products that were already purchased");

	return true;
}

//
//
//
// CzPlatformMarketAndroid implementation
//
//
//
bool CzPlatformMarketAndroid::isBillingEnabled()
{
	if (s3eAndroidMarketBillingIsSupported() == S3E_ANDROIDMARKETBILLING_UNAVAILABLE)
		return false;

	return true;
}

int CzPlatformMarketAndroid::Init(void* id)
{
	Initialised = false;
	Available = false;
	ActiveMarket = NULL;
	Status = MS_IDLE;

	setPublicKey((const char*)id);

	if (PublicKey.isEmpty())
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Please supply your public key to publicKey in s3eAndroidMarketBilling to be able to validate signatures");
		return 0;
	}

	if (s3eAndroidMarketBillingAvailable())
		Available = true;
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Android billing is not available");
		return 0;
	}

	s3eAndroidMarketBillingInit();

	if (isBillingEnabled())
	{
		Available = false;
		setStatus(MS_ERROR_PURCHASE_DISABLED);
		NotifyBillingDisabled();
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Android billing is disabled");
		return 0;
	}

	// Register for the S3E_ANDROIDMARKETBILLING_RESPONSE callback
	// This will inform us about the Android Market server's error codes
	s3eAndroidMarketBillingRegister(S3E_ANDROIDMARKETBILLING_RESPONSE, &AsyncResponse, NULL);

	// Register for the S3E_ANDROIDMARKETBILLING_ORDER_INFORMATION callback
	// This will inform us whenever a transaction changes status
	s3eAndroidMarketBillingRegister(S3E_ANDROIDMARKETBILLING_ORDER_INFORMATION, &OrderInformationChanged, NULL);

	// Supply our public key to allow signature checks
	if (!PublicKey.isEmpty())
	{
		s3eAndroidMarketBillingSetPublicKey(PublicKey.c_str());
	}
	else
	{
		Available = false;
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Please supply your public key to publicKey to be able to validate signatures");
		return 0;
	}

	Initialised = true;

	return 1;
}

void CzPlatformMarketAndroid::Release()
{
	s3eAndroidMarketBillingUnRegister(S3E_ANDROIDMARKETBILLING_ORDER_INFORMATION, OrderInformationChanged);
	s3eAndroidMarketBillingTerminate();

	Initialised = false;
	ActiveMarket = NULL;
}

void CzPlatformMarketAndroid::Update()
{
}

bool CzPlatformMarketAndroid::QueryProduct(const char* product_id)
{
	getActiveMarket()->setCurrentProductID(product_id);
	NotifyError();

	return false;
}

bool CzPlatformMarketAndroid::PurchaseProduct(const char* product_id)
{
	if (ActiveMarket == NULL || !Available || ActiveMarket->isBusy())
		return false;

	// Find the product
	CzMarketProduct* product = ActiveMarket->findProduct(product_id);
	if (product == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Product not found - ", CzString(product_id).c_str());
		return false;
	}

	// If product is not a consumable and is already purchased then we do not attempt to purchase again
	if (product->Purchased && !product->Consumable)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Product already purchased - ", CzString(product_id).c_str());
		NotifyAlreadyPurchased();
		return false;
	}
	getActiveMarket()->setCurrentProductID(product_id);

	// Start purchase request
CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Purchasing - ", product_id);
	if (s3eAndroidMarketBillingRequestPurchase(product_id) != S3E_ANDROIDMARKETBILLING_RESULT_OK)
	{
		NotifyError();
		return false;
	}

	ActiveMarket->setBusy(true);

	return true;
}

bool CzPlatformMarketAndroid::RestoreProducts()
{
	if (!Available)
		return false;

	if (s3eAndroidMarketBillingRestoreTransactions() != S3E_ANDROIDMARKETBILLING_RESULT_OK)
	{
		NotifyError();
		return false;
	}

	return true;
}

// Callback function
int32 CzPlatformMarketAndroid::AsyncResponse(void* system, void* user)
{
	CzPlatformMarketAndroid* market = (CzPlatformMarketAndroid*)PLATFORM_MARKET;
	if (market == NULL || market->getActiveMarket() == NULL)
		return 0;

	s3eAndroidMarketBillingResult result = ((s3eAndroidMarketBillingResponse*)system)->m_ResponseCode;
	switch (result)
	{
	case S3E_ANDROIDMARKETBILLING_RESULT_OK:
		market->setStatus(MS_IDLE);
		break;
	case S3E_ANDROIDMARKETBILLING_RESULT_USER_CANCELLED:
		market->setStatus(MS_ERROR_PAYMENT_CANCELLED);
		market->NotifyError();
		break;
	case S3E_ANDROIDMARKETBILLING_RESULT_SERVICE_UNAVAILABLE:
		market->setStatus(MS_ERROR_NO_CONNECTION);
		market->NotifyError();
		break;
	case S3E_ANDROIDMARKETBILLING_RESULT_DEVELOPER_ERROR:
		market->setStatus(MS_ERROR_DEVELOPER_ERROR);
		market->NotifyError();
		break;
	case S3E_ANDROIDMARKETBILLING_RESULT_BILLING_UNAVAILABLE:
		market->setStatus(MS_ERROR_PURCHASE_DISABLED);
		market->NotifyError();
		break;
	case S3E_ANDROIDMARKETBILLING_RESULT_ITEM_UNAVAILABLE:
		market->setStatus(MS_ERROR_UNKNOWN_PRODUCT);
		market->NotifyError();
		break;
	case S3E_ANDROIDMARKETBILLING_RESULT_ERROR:
		market->setStatus(MS_ERROR_PURCHASE_UNKNOWN);
		market->NotifyError();
		break;
	case S3E_ANDROIDMARKETBILLING_UNAVAILABLE:
		market->setStatus(MS_ERROR_UNAVAILABLE);
		market->NotifyError();
		break;
	}

	return 0;
}

// Callback function
int32 CzPlatformMarketAndroid::OrderInformationChanged(void* system, void* user)
{
	CzPlatformMarketAndroid* market = (CzPlatformMarketAndroid*)PLATFORM_MARKET;
	if (market == NULL || market->getActiveMarket() == NULL)
		return 0;

	s3eAndroidMarketBillingOrderInformation *info = (s3eAndroidMarketBillingOrderInformation*)system;
CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "CzPlatformMarketAndroid::OrderInformationChanged");

	for (int i = 0; i < info->m_NumOrders; i++)
	{
		s3eAndroidMarketBillingOrder o = info->m_Orders[i];
		market->getActiveMarket()->setCurrentProductID(o.m_ProductID);

		if (o.m_PurchaseState == S3E_ANDROIDMARKETBILLING_PURCHASE_STATE_PURCHASED)
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Item is purchased, confirming this to Market");
			market->NotifyReceiptAvailable();
			market->getActiveMarket()->setPurchased(o.m_ProductID);
		}
		else
		if (o.m_PurchaseState == S3E_ANDROIDMARKETBILLING_PURCHASE_STATE_CANCELLED)
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Item is cancelled, confirming this to Market");
			market->setStatus(MS_ERROR_PAYMENT_CANCELLED);
			market->NotifyError();
		}
		else
		if (o.m_PurchaseState == S3E_ANDROIDMARKETBILLING_PURCHASE_STATE_REFUNDED)
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Item is refunded, confirming this to Market");
			market->NotifyRefund();
		}

		s3eAndroidMarketBillingConfirmNotifications(&o.m_NotificationID,1);
	}

	return 0;
}

void CzPlatformMarketAndroid::NotifyRefund()
{
	if (!Available || ActiveMarket == NULL)
		return;

	getActiveMarket()->setPurchased(getActiveMarket()->getCurrentProductID(), false);

	if (RefundHandler != NULL)
		RefundHandler(this, RefundHandlerData);

	if (ActiveMarket != NULL)
		ActiveMarket->NotifyRefund();

	ActiveMarket->setBusy(false);
}


//
//
//
// CzPlatformMarketTest implementation
//
//
//
int CzPlatformMarketTest::Init(void * id)
{
	Available = true;
	Initialised = true;

	return 1;
}

void CzPlatformMarketTest::Release()
{
	Initialised = false;
	ActiveMarket = NULL;
}

void CzPlatformMarketTest::Update()
{
	if (!Initialised || !Available || ActiveMarket == NULL)
		return;

	if (PurchaseTimer.hasStarted() && PurchaseTimer.HasTimedOut())
	{
		ActiveMarket->setBusy(false);
		if (SimulationHash != 0)
		{
			if (SimulationHash == CzHashes::Error_Hash)
				NotifyError();				// Simulate an error during purchasing
			else
			if (SimulationHash == CzHashes::Refund_Hash)
				NotifyRefund();				// Simulate a refund
			else
			if (SimulationHash == CzHashes::BillingDisabled_Hash)
				NotifyBillingDisabled();	// Simulate billing disabled
			else
			if (SimulationHash == CzHashes::Complete_Hash)
				NotifyReceiptAvailable();	// Simulate successful purchase
		}
		else
		{
			ActiveMarket->setPurchased(ActiveMarket->getCurrentProductID());
			NotifyReceiptAvailable();	// Simulate successful purchase
		}
	}
}

bool CzPlatformMarketTest::QueryProduct(const char* product_id)
{
	if (!Available || ActiveMarket == NULL || ActiveMarket->isBusy())
		return false;

	// Find the product
	CzMarketProduct* product = ActiveMarket->findProduct(product_id);
	if (product == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Product not found - ", product_id);
		return false;
	}

	ActiveMarket->setCurrentProductID(product_id);
	NotifyInfoAvailable();	// Simulate info available

	return true;
}

bool CzPlatformMarketTest::PurchaseProduct(const char* product_id)
{
	if (!Available || ActiveMarket == NULL || ActiveMarket->isBusy())
		return false;

	// Find the product
	CzMarketProduct* product = ActiveMarket->findProduct(product_id);
	if (product == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Product not found - ", product_id);
		return false;
	}

	// If product is not a consumable and is already purchased then we do not attempt to purchase again
	if (product->Purchased && !product->Consumable)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Product already purchased - ", product_id);
		return false;
	}

	ActiveMarket->setCurrentProductID(product_id);

	PurchaseTimer.setDuration(2000);
	ActiveMarket->setBusy(true);

	return true;
}

bool CzPlatformMarketTest::RestoreProducts()
{
	if (!Available)
		return false;

	getActiveMarket()->setPurchased(getActiveMarket()->getCurrentProductID(), false);

	if (RefundHandler != NULL)
		RefundHandler(this, RefundHandlerData);

	if (ActiveMarket != NULL)
		ActiveMarket->NotifyRefund();

	ActiveMarket->setBusy(false);

	return true;
}

void CzPlatformMarketTest::ForceError(eMarketStatus status)
{
	setStatus(status);
	NotifyError();	// Simulate an error
}

bool CzPlatformMarketTest::isBillingEnabled()
{
	return true;
}

