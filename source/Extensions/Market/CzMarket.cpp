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
#include "CzFile.h"
#include "CzScene.h"
#include "CzApp.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"

//
//
//
// CzMarketProduct implementation
//
//
//
bool CzMarketProduct::Save()
{
	return false;
}

bool CzMarketProduct::Load()
{
	return false;
}

//
//
//
// CzMarket implementation
//
//
//

IzPlatformMarket::eMarketVendor	CzMarket::VendorFromText(const char* vendor)
{
	unsigned int vendor_hash = CZ_HASH(vendor);
	
	if (vendor_hash == CZ_HASH("amazon"))
		return IzPlatformMarket::MV_VENDOR_AMAZON;
	else
	if (vendor_hash == CZ_HASH("googleplay"))
		return IzPlatformMarket::MV_VENDOR_GOOGLE_PLAY;
	else
	if (vendor_hash == CZ_HASH("android"))
		return IzPlatformMarket::MV_VENDOR_ANDROID_MARKET;
	else
	if (vendor_hash == CZ_HASH("samsung"))
		return IzPlatformMarket::MV_VENDOR_SAMSUNG;
	else
	if (vendor_hash == CZ_HASH("blackberry"))
		return IzPlatformMarket::MV_VENDOR_BLACKBERRY;
	else
	if (vendor_hash == CZ_HASH("apple"))
		return IzPlatformMarket::MV_VENDOR_APPLE;
	else
	if (vendor_hash == CZ_HASH("wp8"))
		return IzPlatformMarket::MV_VENDOR_WP8;
	else
	if (vendor_hash == CZ_HASH("ws8"))
		return IzPlatformMarket::MV_VENDOR_WS8;

	return IzPlatformMarket::MV_VENDOR_NONE;
}

bool CzMarket::isAvailable()
{
	return PLATFORM_MARKET->isAvailable(Vendor);
}

int	CzMarket::Init(const char* public_key)
{
	PLATFORM_MARKET->setActiveMarket(this);

	if (EventsManager == NULL)
		EventsManager = new CzEventManager();

	if (PLATFORM_MARKET->Init((void*)public_key, Vendor) == 0)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market could not be created");
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, CzString(Vendor).c_str());
		NotifyUnavailable();
		return 0;
	}

	Busy = false;
	Load();

	return 1;
}

void	CzMarket::Release()
{
	// Delete products
	for (_ProductIterator it = Products.begin(); it != Products.end(); ++it)
		delete *it;

	// Delete events manager
	SAFE_DELETE(EventsManager)

	if (PLATFORM_MARKET->getActiveMarket() == this)
		PLATFORM_MARKET->setActiveMarket(NULL);

	PLATFORM_MARKET->Release();
}

CzMarketProduct* CzMarket::findProduct(const char* product_id)
{
	unsigned int id_hash = CzString::CalculateHash(product_id);
	for (_ProductIterator it = Products.begin(); it != Products.end(); ++it)
	{
		if (id_hash == (*it)->ProductID.getHash())
			return *it;
	}

	return NULL;
}

CzMarketProduct* CzMarket::findProductByName(unsigned int name_hash)
{
	for (_ProductIterator it = Products.begin(); it != Products.end(); ++it)
	{
		if (name_hash == (*it)->Name.getHash())
			return *it;
	}

	return NULL;
}

void CzMarket::removeProduct(CzMarketProduct* product)
{
	for (_ProductIterator it = Products.begin(); it != Products.end(); ++it)
	{
		if (product == *it)
		{
			delete *it;
			Products.erase(it);
			break;
		}
	}
}

void CzMarket::Save()
{
}

void CzMarket::Load()
{
}

void CzMarket::setCurrentProductID(const char* product_id)
{
	// If resource system iniitialised then update system variable with new id
	if (CZ_GLOBAL_RESOURCES != NULL)
	{
		CzXomlVariableArrayInt* system = (CzXomlVariableArrayInt*)CZ_GLOBAL_RESOURCES->getVariableManager()->findVariable("system");
		if (system != NULL)
			system->setValue(CzApp::SYS_LAST_PURCHASE_ID, product_id);
	}
	
	CurrentProductID = product_id;
}

void CzMarket::setItemRange(int start, int end)
{
	if (PLATFORM_MARKET == NULL || !PLATFORM_MARKET->isAvailable(Vendor))
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Not available", DebugInfo.c_str());
		return;
	}

	PLATFORM_MARKET->setItemRange(start, end);
}

void CzMarket::setPayload(const char* payload)
{
	if (PLATFORM_MARKET == NULL || !PLATFORM_MARKET->isAvailable(Vendor))
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Not available", DebugInfo.c_str());
		return;
	}

	PLATFORM_MARKET->setPayload(payload);
}

const char* CzMarket::getPayload()
{
	if (PLATFORM_MARKET == NULL || !PLATFORM_MARKET->isAvailable(Vendor))
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Not available", DebugInfo.c_str());
		return NULL;
	}

	return PLATFORM_MARKET->getPayload();
}

void CzMarket::setTestMode(bool mode)
{
	if (PLATFORM_MARKET == NULL || !PLATFORM_MARKET->isAvailable(Vendor))
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Not available", DebugInfo.c_str());
		return;
	}

	PLATFORM_MARKET->setTestMode(mode);
}

bool CzMarket::FinishTransaction(const char* finish_data)
{
	if (PLATFORM_MARKET == NULL || !PLATFORM_MARKET->isAvailable(Vendor))
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Not available", DebugInfo.c_str());
		return false;
	}

	return PLATFORM_MARKET->FinishTransaction(finish_data);
}

int CzMarket::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Process market attributes
	CzString* public_key = NULL;

	if (EventsManager == NULL)
		EventsManager = new CzEventManager();

	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
			setName((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Vendor_Hash)
			setVendor(VendorFromText((*it)->getValue().c_str()));
		else
		if (name_hash == CzHashes::OnError_Hash)
			EventsManager->addEvent("OnError", (*it)->getValue().c_str(), true);
		else
		if (name_hash == CzHashes::OnUnavailable_Hash)
			EventsManager->addEvent("OnUnavailable", (*it)->getValue().c_str(), true);
		else
		if (name_hash == CzHashes::OnComplete_Hash)
			EventsManager->addEvent("OnComplete", (*it)->getValue().c_str(), true);
		else
		if (name_hash == CzHashes::OnBillingDisabled_Hash)
			EventsManager->addEvent("OnBillingDisabled", (*it)->getValue().c_str(), true);
		else
		if (name_hash == CzHashes::AndroidPublicKey_Hash)
			public_key = &(*it)->getValue();
	}

	CzScene* scene = NULL;
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;
	bool added = false;
	if (scene != NULL)
		added = scene->getResourceManager()->addResource(this) ? 1 : 0;
	else
		added = CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this) ? 1 : 0;
	if (!added)
		return 0;


	Init(public_key->c_str());

	// Process products
	eCzDeviceType os = PLATFORM_SYS->getDeviceType();
	for (CzXmlNode::_Iterator it2 = node->begin(); it2 != node->end(); ++it2)
	{
		bool valid = false;
		CzMarketProduct* product = new CzMarketProduct();

		unsigned int name_hash = (*it2)->GetName().getHash();
		if (name_hash == CzHashes::Product_Hash)
		{
			for (CzXmlNode::_AttribIterator it = (*it2)->attribs_begin(); it != (*it2)->attribs_end(); ++it)
			{
				unsigned int attrib_hash = (*it)->getName().getHash();

				if (attrib_hash == CzHashes::Name_Hash)
				{
					// Search for the product
					CzMarketProduct* p = findProductByName((*it)->getValue().getHash());
					if (p == NULL)
					{
						product->Name = (*it)->getValue();
						valid = true;
					}
					else
					{
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Market - Product with this name already exists - ", (*it)->getValue().c_str(), DebugInfo.c_str());
						valid = false;
						break;
					}
				}
				else
				if (attrib_hash == CzHashes::iOSId_Hash)
				{
					if (os == CzDeviceType_iPhone || os == CzDeviceType_iPad)
						product->ProductID = (*it)->getValue();
				}
				else
				if (attrib_hash == CzHashes::AndroidId_Hash)
				{
					if (os == CzDeviceType_Android)
						product->ProductID = (*it)->getValue();
				}
				else
				if (attrib_hash == CzHashes::Price_Hash)
					product->Price = (*it)->getValueAsFloat();
			}
		}
		if (valid)
		{
			addProduct(product);
			product->Load();
		}
		else
			delete product;
	}

	return 1;
}

bool CzMarket::QueryProduct(const char* product_id)
{
	if (PLATFORM_MARKET == NULL || !PLATFORM_MARKET->isAvailable(Vendor))
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Not available", DebugInfo.c_str());
		return false;
	}

	if (!PLATFORM_MARKET->QueryProducts(&product_id, 1))
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Could not query product with ID - ", CzString(product_id).c_str(), DebugInfo.c_str());
		return false;
	}

	return true;
}

bool CzMarket::PurchaseProduct(const char* product_id)
{
	if (PLATFORM_MARKET == NULL || !PLATFORM_MARKET->isAvailable(Vendor))
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Not available", DebugInfo.c_str());
		return false;
	}

	if (!PLATFORM_MARKET->PurchaseProduct(product_id))
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Could not purchase product with ID - ", CzString(product_id).c_str(), DebugInfo.c_str());
		return false;
	}

	return true;
}

bool CzMarket::RestoreProducts()
{
	if (PLATFORM_MARKET == NULL || !PLATFORM_MARKET->isAvailable(Vendor))
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Not available", DebugInfo.c_str());
		return false;
	}

	if (!PLATFORM_MARKET->RestoreProducts())
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Could not restore products", DebugInfo.c_str());
		return false;
	}

	return true;
}

bool CzMarket::ConsumeProduct(const char* purchase_token)
{
	if (PLATFORM_MARKET == NULL || !PLATFORM_MARKET->isAvailable(Vendor))
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Not available", DebugInfo.c_str());
		return false;
	}

	if (!PLATFORM_MARKET->ConsumeProduct(purchase_token))
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Could not restore products", DebugInfo.c_str());
		return false;
	}

	return true;
}

void CzMarket::ProcessEventActions(unsigned int event_name, IzXomlResource* parent)
{
	// Find the event
	CzEvent* evt = EventsManager->findEvent(event_name);
	if (evt != NULL)
	{
		// Find actions list
		CzActions* actions = CzActionsManager::FindActions(evt->ActionsName, parent);
		if (actions != NULL)
			actions->Execute(parent);
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Action not found - ", evt->_ActionsName.c_str(), DebugInfo.c_str());
	}
}

void	CzMarket::NotifyReady()
{
	Busy = false;
	ProcessEventActions(CZ_HASH("OnReady"), Parent->getParent());
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzList<const char*> props;
		CzList<const char*> vals;
		props.push_back("event");
		vals.push_back("ready");
		callback.ScriptEngine->CallFunctionRefWithTable(callback.FunctionRef, &props, &vals);
	}
}

void	CzMarket::NotifyError(const char* product_id, int error)
{
	Busy = false;
	ProcessEventActions(CZ_HASH("OnError"), Parent->getParent());
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzList<const char*> props;
		CzList<const char*> vals;
		props.push_back("event");
		vals.push_back("error");
		props.push_back("error");
		CzString err = CzString(error);
		vals.push_back(err.c_str());
		props.push_back("productID");
		vals.push_back(product_id);
		callback.ScriptEngine->CallFunctionRefWithTable(callback.FunctionRef, &props, &vals);
	}
}

void	CzMarket::NotifyUnavailable()
{
	Busy = false;
	ProcessEventActions(CZ_HASH("OnUnavailable"), Parent->getParent());
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzList<const char*> props;
		CzList<const char*> vals;
		props.push_back("event");
		vals.push_back("unavailable");
		callback.ScriptEngine->CallFunctionRefWithTable(callback.FunctionRef, &props, &vals);
	}
}

void	CzMarket::NotifyComplete(CzMarketProductReceipt* data)
{
	Busy = false;
	ProcessEventActions(CZ_HASH("OnComplete"), Parent->getParent());
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzList<const char*> props;
		CzList<const char*> vals;
		props.push_back("event");
		vals.push_back("purchased");
		props.push_back("productID");
		vals.push_back(data->ProductID);
		props.push_back("restored");
		CzString restored = CzString(data->Restored);
		vals.push_back(restored.c_str());
		props.push_back("payload");
		vals.push_back(data->Payload);
		props.push_back("date");
		vals.push_back(data->Date);
		props.push_back("subscriptionStartDate");
		vals.push_back(data->SubscriptionStartDate);
		props.push_back("subscriptionEndDate");
		vals.push_back(data->SubscriptionEndDate);
		props.push_back("purchaseToken");
		vals.push_back(data->PurchaseToken);
		props.push_back("transactionID");
		vals.push_back(data->TransactionID);
		callback.ScriptEngine->CallFunctionRefWithTable(callback.FunctionRef, &props, &vals);
		if (data->FinaliseData != NULL)
			FinishTransaction((const char*)data->FinaliseData);
	}
}

void	CzMarket::NotifyBillingDisabled()
{
	Busy = false;
	ProcessEventActions(CZ_HASH("OnBillingDisabled"), Parent->getParent());
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzList<const char*> props;
		CzList<const char*> vals;
		props.push_back("event");
		vals.push_back("disabled");
		callback.ScriptEngine->CallFunctionRefWithTable(callback.FunctionRef, &props, &vals);
	}
	Busy = false;
}

void	CzMarket::NotifyInfoAvailable(CzMarketProductData* data)
{
	Busy = false;
	ProcessEventActions(CZ_HASH("OnInfoAvailable"), Parent->getParent());
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzList<const char*> props;
		CzList<const char*> vals;
		props.push_back("event");
		vals.push_back("info");
		props.push_back("productID");
		vals.push_back(data->ProductID);
		props.push_back("title");
		vals.push_back(data->Title);
		props.push_back("description");
		vals.push_back(data->Description);
		props.push_back("price");
		vals.push_back(data->Price);
		callback.ScriptEngine->CallFunctionRefWithTable(callback.FunctionRef, &props, &vals);
	}
}


