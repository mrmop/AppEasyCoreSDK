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

#include "CzUtil.h"
#include "CzXoml.h"
#include "CzRemoteReq.h"
#include "CzScene.h"
#include "CzString.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"


CzXomlClassDef* CzRemoteReq::RemoteRequestClassDef = NULL;

/**
 @fn	void CzRemoteReq::InitClass()

 @brief	Initialises the class.
    
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */
void CzRemoteReq::InitClass()
{
	// Create class properties
	RemoteRequestClassDef = new CzXomlClassDef();
	RemoteRequestClassDef->addProperty(new CzXomlClassProperty("Name",					PT_String,		CzRemoteReq::_setName,						CzRemoteReq::_getName));
	RemoteRequestClassDef->addProperty(new CzXomlClassProperty("Tag",					PT_String,		CzRemoteReq::_setTag,						NULL));
	RemoteRequestClassDef->addProperty(new CzXomlClassProperty("URL",					PT_String,		CzRemoteReq::_setURL,						CzRemoteReq::_getURL));
	RemoteRequestClassDef->addProperty(new CzXomlClassProperty("Data",					PT_String,		CzRemoteReq::_setData,						CzRemoteReq::_getData));
	RemoteRequestClassDef->addProperty(new CzXomlClassProperty("OnResponse",			PT_String,		CzRemoteReq::_setOnResponse,				NULL));
	RemoteRequestClassDef->addProperty(new CzXomlClassProperty("OnError",				PT_String,		CzRemoteReq::_setOnError,					NULL));
	RemoteRequestClassDef->addProperty(new CzXomlClassProperty("Post",					PT_Bool,		CzRemoteReq::_setPost,						CzRemoteReq::_getPost));
	RemoteRequestClassDef->addProperty(new CzXomlClassProperty("Variable",				PT_Resource,	CzRemoteReq::_setVariable,					CzRemoteReq::_getVariable));
}
void CzRemoteReq::ReleaseClass()
{
	if (RemoteRequestClassDef != NULL)
	{
		RemoteRequestClassDef->Release();
		delete RemoteRequestClassDef;
		RemoteRequestClassDef = NULL;
	};
}

// //
// //
// //
// //
// //
// //
// //
// //
// XOML property setters and getters
// //
// //
// //
// //
// //
// //
// //
// //

bool CzRemoteReq::_setName(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	if (add)
	{
		CzString name = ((CzRemoteReq*)target)->getName();
		name += (const char*)prop.p_data;
		((CzRemoteReq*)target)->setName(name.c_str());
	}
	else
		((CzRemoteReq*)target)->setName((const char*)prop.p_data);

	return true;
}

CzXomlProperty CzRemoteReq::_getName(IzXomlResource* target)
{
	return CzXomlProperty(((CzRemoteReq*)target)->getName().c_str());
}

bool CzRemoteReq::_setTag(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	((CzRemoteReq*)target)->setTag((const char*)prop.p_data);

	return true;
}

bool CzRemoteReq::_setURL(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	if (add)
	{
		CzString URL = ((CzRemoteReq*)target)->getUrl();
		URL += (const char*)prop.p_data;
		((CzRemoteReq*)target)->setUrl(URL.c_str());
	}
	else
		((CzRemoteReq*)target)->setUrl((const char*)prop.p_data);

	return true;
}

CzXomlProperty CzRemoteReq::_getURL(IzXomlResource* target)
{
	return CzXomlProperty(((CzRemoteReq*)target)->getUrl().c_str());
}

bool CzRemoteReq::_setData(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	if (add)
	{
		CzString Data = ((CzRemoteReq*)target)->getData();
		Data += (const char*)prop.p_data;
		((CzRemoteReq*)target)->setData(Data.c_str());
	}
	else
		((CzRemoteReq*)target)->setData((const char*)prop.p_data);

	return true;
}

CzXomlProperty CzRemoteReq::_getData(IzXomlResource* target)
{
	return CzXomlProperty(((CzRemoteReq*)target)->getData().c_str());
}

bool CzRemoteReq::_setOnResponse(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzRemoteReq* req = (CzRemoteReq*)target;
	req->getEventsManager()->addEvent("OnResponse", (const char*)prop.p_data, true);
	return true;
}

bool CzRemoteReq::_setOnError(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzRemoteReq* req = (CzRemoteReq*)target;
	req->getEventsManager()->addEvent("OnError", (const char*)prop.p_data, true);
	return true;
}

bool CzRemoteReq::_setPost(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzRemoteReq* req = (CzRemoteReq*)target;

	if (add)
		req->setPost(!req->isPost());
	else
		req->setPost(prop.p_bool);

	return true;
}

CzXomlProperty CzRemoteReq::_getPost(IzXomlResource* target)
{
	return CzXomlProperty(((CzRemoteReq*)target)->isPost());
}

bool CzRemoteReq::_setVariable(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	if (prop.Type == PT_Resource)
		((CzRemoteReq*)target)->setVariable((CzXomlVariable*)prop.p_data);
	else
		((CzRemoteReq*)target)->setVariable((const char*)prop.p_data);

	return true;
}

CzXomlProperty CzRemoteReq::_getVariable(IzXomlResource* target)
{
	return CzXomlProperty(((CzRemoteReq*)target)->getVariable());
}


//
//
//
//
// CzRemoteReq implementation
//
//
//
//

int32 RemoteReqDataRetrievedCallback(void* caller, void *data)
{
	CzHttpRequest* request = (CzHttpRequest*)caller;
	CzRemoteReq* req = (CzRemoteReq*)data;

	if (request->getProcessed())									// Check to see if our request was processed by the http manager
	{
		req->DataReceived();
//		CZ_HTTP_MANAGER->RemoveRequest(request);				// Remove request from http manager queue
	}

	return 0;
}

void CzRemoteReq::setVariable(const char* variable_name)
{
	if (variable_name != NULL)
	{
		CzString var_name = variable_name;
		Variable = CzXomlVariable::GetVariable(var_name, Parent->getParent());
		if (Variable == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "RemoteReq - Target variable not found - ", variable_name, DebugInfo.c_str());
	}
}


void CzRemoteReq::DataReceived()
{
	if (Request.getError() != 0)
		ProcessEventActions(CzHashes::OnError_Hash);
	else
	{
		if (Variable != NULL)
			Variable->setValue(Request.getContent().c_str());

		ProcessEventActions(CzHashes::OnResponse_Hash);
	}
}

CzRemoteReq::~CzRemoteReq()
{
	CZ_HTTP_MANAGER->RemoveRequest(&Request);
	SAFE_DELETE(EventsManager)
}

void CzRemoteReq::ProcessEventActions(unsigned int event_name)
{
	// Find the event
	CzEvent* evt = EventsManager->findEvent(event_name);
	if (evt != NULL)
	{
		// Find actions list
		IzXomlResource* parent = Parent->getParent();
		CzActions* actions = CzActionsManager::FindActions(evt->ActionsName, parent);
		if (actions != NULL)
			actions->Execute(parent);
	}
}

int CzRemoteReq::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	CzString* var_name = NULL;
	CzString* data = NULL;
	CzScene* scene = NULL;
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
			setName((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::URL_Hash)
			setUrl((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Data_Hash)
			Request.setBody((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnResponse_Hash)
			EventsManager->addEvent("OnResponse", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnError_Hash)
			EventsManager->addEvent("OnError", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Post_Hash)
		{
			if ((*it)->getValueAsBool())
				Request.setPOST();
			else
				Request.setGET();
		}
		else
		if (name_hash == CzHashes::Variable_Hash)
			var_name = &(*it)->getValue();
	}

	// Collect and set headers
	for (CzXmlNode::_Iterator it2 = node->begin(); it2 != node->end(); ++it2)
	{
		unsigned int name_hash = (*it2)->GetName().getHash();
		if (name_hash == CzHashes::Header_Hash)
		{
			CzString* header_name = NULL;
			CzString* header_value = NULL;
			for (CzXmlNode::_AttribIterator it = (*it2)->attribs_begin(); it != (*it2)->attribs_end(); ++it)
			{
				unsigned int attrib_hash = (*it)->getName().getHash();

				if (attrib_hash == CzHashes::Name_Hash)
					header_name = &(*it)->getValue();
				else
				if (attrib_hash == CzHashes::Value_Hash)
					header_value = &(*it)->getValue();
			}

			if (header_name == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "RemoteReq - header name not supplied", (*it2)->getParserFileName(), (*it2)->Line);
			if (header_value == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "RemoteReq - header value not supplied", (*it2)->getParserFileName(), (*it2)->Line);

			if (header_name != NULL && header_value != NULL)
				Request.SetHeader(header_name->c_str(), header_value->c_str());
		}
	}

	Request.setContentAvailableCallback(&RemoteReqDataRetrievedCallback, this);

	// Find and attach the variable
	if (var_name != NULL)
	{
		CzXomlVariable* var = CzXomlVariable::GetVariable(*var_name, scene);
		if (var == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "RemoteReq - Target variable not found - ", var_name->c_str(), DebugInfo.c_str());
		Variable = var;
	}


	if (scene != NULL)
		return scene->getResourceManager()->addResource(this) ? 1 : 0;
	else
		return CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this) ? 1 : 0;

	return 1;
}

void CzRemoteReq::MakeRequest()
{
	CZ_HTTP_MANAGER->AddRequest(&Request);
}

/**
 @fn	bool CzRemoteReq::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the request.

 Returns the named property of the request. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types.

 @param	property_name		Name of the property.
 @param [out]	prop	The property data.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzRemoteReq::getProperty(const char* property_name, CzXomlProperty& prop)
{
	return getProperty(CZ_HASH(property_name), prop);
}

/**
 @fn	bool CzRemoteReq::getProperty(unsigned int property_name, CzXomlProperty& prop)

 Returns the named property of the request. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in,out]	prop	The property.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzRemoteReq::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	int err = IzXomlResource::getProperty(RemoteRequestClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = RemoteRequestClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Remote Request - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzRemoteReq::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the Remote Request.
    
 Sets the named property of this Remote Request. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzRemoteReq::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	int err = IzXomlResource::setProperty(RemoteRequestClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = RemoteRequestClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Remote Request - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzRemoteReq::setProperty(const char* property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the request.

 Sets the named property of this request. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced.

 @param	property_name		Name of the property.
 @param [in]	data	The property data.
 @param	delta				true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzRemoteReq::setProperty(const char* property_name, const CzString& data, bool delta)
{
	return setProperty(CzString::CalculateHash(property_name), data, delta);
}

/**
 @fn	bool CzRemoteReq::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the request.
    
 Sets the named property of this request. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzRemoteReq::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	int err = IzXomlResource::setProperty(RemoteRequestClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = RemoteRequestClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Remote Request - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}
