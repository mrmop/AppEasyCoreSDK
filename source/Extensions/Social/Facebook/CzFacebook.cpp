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

#include "CzFacebook.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"
#include "CzScene.h"

//
//
//
//
// CzFacebook implementation
//
//
//
//
bool CzFacebook::IsLoggedn() const
{
	return PLATFORM_FACEBOOK->isLoggedIn();
}

bool CzFacebook::Login(const char* permissions[])
{
	if (!PLATFORM_FACEBOOK->Login(permissions))
	{
		NotifyLoginError();
		return false;
	}
	return true;
}

bool CzFacebook::Logout()
{
	return PLATFORM_FACEBOOK->Logout();
}

bool CzFacebook::InitRequest(const char* method_name, const char* http_method)
{
	if (!PLATFORM_FACEBOOK->InitRequest(method_name, http_method))
	{
		NotifyResponseError();
		return false;
	}
	return true;
}

bool CzFacebook::AddParamString(const char* parameter, const char* value)
{
	if (!PLATFORM_FACEBOOK->AddParamString(parameter, value))
	{
		NotifyResponseError();
		return false;
	}
	return true;
}

bool CzFacebook::AddParamNumber(const char* parameter, int64 value)
{
	if (!PLATFORM_FACEBOOK->AddParamNumber(parameter, value))
	{
		NotifyResponseError();
		return false;
	}
	return true;
}

bool CzFacebook::SendRequest()
{
	return PLATFORM_FACEBOOK->SendRequest();
}

int CzFacebook::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Parse attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
			setName((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnLoggedIn_Hash)
			EventsManager->addEvent("OnLoggedIn", (*it)->getValue().c_str(), true);
		else
		if (name_hash == CzHashes::OnLoginError_Hash)
			EventsManager->addEvent("OnLoginError", (*it)->getValue().c_str(), true);
		else
		if (name_hash == CzHashes::OnResponse_Hash)
			EventsManager->addEvent("OnResponse", (*it)->getValue().c_str(), true);
		else
		if (name_hash == CzHashes::OnResponseError_Hash)
			EventsManager->addEvent("OnResponseError", (*it)->getValue().c_str(), true);
	}

	CzScene* scene = NULL;
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

	if (scene != NULL)
	{
		if (!scene->getResourceManager()->addResource(this))
			return 0;
	}
	else
	{
		if (!CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this))
			return 0;
	}

	return 1;
}

void CzFacebook::ProcessEventActions(unsigned int event_name, IzXomlResource* parent)
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
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Facebook - Action not found - ", evt->_ActionsName.c_str(), DebugInfo.c_str());
	}
}

void CzFacebook::NotifyLoggedIn()
{
	ProcessEventActions(CZ_HASH("OnLoggedIn"), Parent->getParent());
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzString p1 = "loggedin";
		callback.ScriptEngine->CallFunctionRef(this, callback.FunctionRef, &p1, NULL, NULL, NULL);
	}
}

void CzFacebook::NotifyLoginError()
{
	ProcessEventActions(CZ_HASH("OnLoginError"), Parent->getParent());
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzString p1 = "logginerror";
		callback.ScriptEngine->CallFunctionRef(this, callback.FunctionRef, &p1, NULL, NULL, NULL);
	}
}

void CzFacebook::NotifyResponse()
{
	ProcessEventActions(CZ_HASH("OnResponse"), Parent->getParent());
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzString p1 = "response";
		callback.ScriptEngine->CallFunctionRef(this, callback.FunctionRef, &p1, NULL, NULL, NULL);
	}
}

void CzFacebook::NotifyResponseError()
{
	ProcessEventActions(CZ_HASH("OnResponseError"), Parent->getParent());
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzString p1 = "responseerror";
		callback.ScriptEngine->CallFunctionRef(this, callback.FunctionRef, &p1, NULL, NULL, NULL);
	}
}






