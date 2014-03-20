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
int	CzMarket::Init(const char* public_key)
{
	PLATFORM_MARKET->setActiveMarket(this);

	if (EventsManager == NULL)
		EventsManager = new CzEventManager();

	if (!PLATFORM_MARKET->isInitialised())
	{
		if (PLATFORM_MARKET->Init((void*)public_key) == 0)
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market could not be created");
			NotifyUnavailable();
			return 0;
		}
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

	if (PLATFORM_MARKET->isInitialised())
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

void CzMarket::setPurchased(const char* product_id, bool purchased)
{
	CzMarketProduct* product = findProduct(product_id);
	if (product != NULL)
	{
		product->Purchased = purchased;
		product->Save();
	}
	setCurrentProductID(product_id);
}

int CzMarket::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Process market attributes
	CzString* public_key = NULL;
	CzString* simulate = NULL;

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
		if (name_hash == CzHashes::OnRefund_Hash)
			EventsManager->addEvent("OnRefund", (*it)->getValue().c_str(), true);
		else
		if (name_hash == CzHashes::AndroidPublicKey_Hash)
			public_key = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Simulate_Hash)
			simulate = &(*it)->getValue();
	}

	CzScene* scene = NULL;
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

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
				if (attrib_hash == CzHashes::Consumable_Hash)
					product->Consumable = (*it)->getValueAsBool();
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

	if (simulate != NULL)
		PLATFORM_MARKET->setSimulation(simulate->getHash());

	if (scene != NULL)
		return scene->getResourceManager()->addResource(this) ? 1 : 0;
	else
		return CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this) ? 1 : 0;

	return 1;
}

bool CzMarket::QueryProduct(const char* product_id)
{
	if (PLATFORM_MARKET == NULL || !PLATFORM_MARKET->isAvailable())
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Not available", DebugInfo.c_str());
		return false;
	}
	if (!PLATFORM_MARKET->isInitialised())
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Not initialised", DebugInfo.c_str());
		return false;
	}

	if (!PLATFORM_MARKET->QueryProduct(product_id))
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Could not query product with ID - ", CzString(product_id).c_str(), DebugInfo.c_str());
		return false;
	}

	return true;
}

bool CzMarket::PurchaseProduct(const char* product_id)
{
	if (PLATFORM_MARKET == NULL || !PLATFORM_MARKET->isAvailable())
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Not available", DebugInfo.c_str());
		return false;
	}
	if (!PLATFORM_MARKET->isInitialised())
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Not initialised", DebugInfo.c_str());
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
	if (PLATFORM_MARKET == NULL || !PLATFORM_MARKET->isAvailable())
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Not available", DebugInfo.c_str());
		return false;
	}
	if (!PLATFORM_MARKET->isInitialised())
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Market - Not initialised", DebugInfo.c_str());
		return false;
	}

	if (!PLATFORM_MARKET->RestoreProducts())
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

void	CzMarket::NotifyError()
{
	Busy = false;
	ProcessEventActions(CZ_HASH("OnError"), Parent->getParent());
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzString p1 = "error";
		CzString p2 = CurrentProductID;
		callback.ScriptEngine->CallFunctionRef(callback.FunctionRef, &p1, &p2, NULL, NULL);
	}
}

void	CzMarket::NotifyUnavailable()
{
	Busy = false;
	ProcessEventActions(CZ_HASH("OnUnavailable"), Parent->getParent());
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzString p1 = "unavailable";
		CzString p2 = CurrentProductID;
		callback.ScriptEngine->CallFunctionRef(callback.FunctionRef, &p1, &p2, NULL, NULL);
	}
}

void	CzMarket::NotifyComplete()
{
	Busy = false;
	ProcessEventActions(CZ_HASH("OnComplete"), Parent->getParent());
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzString p1 = "purchased";
		CzString p2 = CurrentProductID;
		callback.ScriptEngine->CallFunctionRef(callback.FunctionRef, &p1, &p2, NULL, NULL);
	}
}

void	CzMarket::NotifyBillingDisabled()
{
	Busy = false;
	ProcessEventActions(CZ_HASH("OnBillingDisabled"), Parent->getParent());
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzString p1 = "disabled";
		CzString p2 = CurrentProductID;
		callback.ScriptEngine->CallFunctionRef(callback.FunctionRef, &p1, &p2, NULL, NULL);
	}
	Busy = false;
}

void	CzMarket::NotifyRefund()
{
	Busy = false;
	ProcessEventActions(CZ_HASH("OnRefund"), Parent->getParent());
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzString p1 = "refund";
		CzString p2 = CurrentProductID;
		callback.ScriptEngine->CallFunctionRef(callback.FunctionRef, &p1, &p2, NULL, NULL);
	}
}

void	CzMarket::NotifyInfoAvailable()
{
	Busy = false;
	ProcessEventActions(CZ_HASH("OnInfoAvailable"), Parent->getParent());
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzString p1 = "info";
		CzString p2 = CurrentProductID;
		callback.ScriptEngine->CallFunctionRef(callback.FunctionRef, &p1, &p2, NULL, NULL);
	}
}


