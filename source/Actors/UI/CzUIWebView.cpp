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

#include "IzPlatformUI.h"
#include "CzUIWebView.h"
#include "CzHashes.h"

CzXomlClassDef* CzUIWebView::WebViewClassDef = NULL;

/**
 @fn	void CzUIWebView::InitClass()

 @brief	Initialises the class.
    
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzUIWebView::InitClass()
{
	// Create class properties
	WebViewClassDef = new CzXomlClassDef();
	WebViewClassDef->addProperty(new CzXomlClassProperty("URI",				PT_String,		CzUIWebView::_setURI,			NULL));
	WebViewClassDef->addProperty(new CzXomlClassProperty("Javascript",		PT_String,		CzUIWebView::_setJavaScript,	CzUIWebView::_getJavascript));
	WebViewClassDef->addProperty(new CzXomlClassProperty("Html",			PT_String,		CzUIWebView::_setHtml,			NULL));
	WebViewClassDef->addProperty(new CzXomlClassProperty("Modal",			PT_String,		NULL,							CzUIWebView::_getModal));
	WebViewClassDef->addProperty(new CzXomlClassProperty("CurrentURI",		PT_String,		NULL,							CzUIWebView::_getCurrentURI));
	WebViewClassDef->addProperty(new CzXomlClassProperty("OnPageLoaded",	PT_String,		CzUIWebView::_setOnPageLoaded,	NULL));
	WebViewClassDef->addProperty(new CzXomlClassProperty("OnPageLoading",	PT_String,		CzUIWebView::_setOnPageLoading,	NULL));
	WebViewClassDef->addProperty(new CzXomlClassProperty("OnPageError",		PT_String,		CzUIWebView::_setOnPageError,	NULL));
	WebViewClassDef->addProperty(new CzXomlClassProperty("OnJavascript",	PT_String,		CzUIWebView::_setOnPageError,	NULL));
}
void CzUIWebView::ReleaseClass()
{
	if (WebViewClassDef != NULL)
	{
		WebViewClassDef->Release();
		delete WebViewClassDef;
		WebViewClassDef = NULL;
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

bool CzUIWebView::_setURI(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIWebView* actor = (CzUIWebView*)target;

	actor->setURI((const char*)prop.p_data);

	return true;
}

CzXomlProperty	CzUIWebView::_getJavascript(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIWebView*)target)->getJavascript().c_str());
}

bool CzUIWebView::_setJavaScript(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIWebView* actor = (CzUIWebView*)target;

	actor->sendJavascript((const char*)prop.p_data);

	return true;
}

bool CzUIWebView::_setHtml(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIWebView* actor = (CzUIWebView*)target;

	actor->setHtml((const char*)prop.p_data);

	return true;
}

CzXomlProperty CzUIWebView::_getModal(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIWebView*)target)->isModal());
}

CzXomlProperty	CzUIWebView::_getCurrentURI(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIWebView*)target)->getCurrentURI().c_str());
}

bool CzUIWebView::_setOnPageLoaded(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIWebView* actor = (CzUIWebView*)target;
	actor->getEventsManager()->addEvent("OnPageLoaded", (const char*)prop.p_data, true);

	return true;
}

bool CzUIWebView::_setOnPageLoading(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIWebView* actor = (CzUIWebView*)target;
	actor->getEventsManager()->addEvent("OnPageLoading", (const char*)prop.p_data, true);

	return true;
}

bool CzUIWebView::_setOnPageError(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIWebView* actor = (CzUIWebView*)target;
	actor->getEventsManager()->addEvent("OnPageError", (const char*)prop.p_data, true);

	return true;
}

bool CzUIWebView::_setOnJavascript(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIWebView* actor = (CzUIWebView*)target;
	actor->getEventsManager()->addEvent("OnJavascript", (const char*)prop.p_data, true);

	return true;
}


//
//
//
//
// CzActor implementation
//
//
//
//
CzUIWebView::~CzUIWebView()
{
	// Delete temp html file
	if (!TempFilename.isEmpty())
	{
		CzFile::DeleteFile(TempFilename.c_str());
	}

	if (WebView != NULL)
		PLATFORM_UI->DestroyWebView(WebView);
}

void CzUIWebView::setURI(const char* uri)
{
	URI = uri;
	PLATFORM_UI->NavigateWebView(WebView, URI.c_str());
}

void CzUIWebView::setHtml(const char* html)
{
	if (html == NULL)
		return;

	CzString h = html;
	h.URLDecode();

	printf("****************************************\n");
	printf("%sn", h.c_str());
	printf("****************************************\n");

	if (!TempFilename.isEmpty())
	{
		CzFile::DeleteFile(TempFilename.c_str());
	}
	TempFilename = "ram://web_view_";
	TempFilename += Name;
//	TempFilename += CzString((int)(PLATFORM_SYS->getTimeUTC() & 0xffffffff));	// Ensure filename is unique
	TempFilename += ".html";
	CzFile file;
	if (file.Open(TempFilename.c_str(), "wb"))
	{
		file.Write((void*)h.c_str(), h.getLength());
		file.Close();
	}
	PLATFORM_UI->NavigateWebView(WebView, TempFilename.c_str());
}

void CzUIWebView::sendJavascript(const char* js)
{
	if (js != NULL)
		PLATFORM_UI->SendJavascriptToWebView(WebView, js);
}

bool CzUIWebView::InitView(bool modal, bool transparent, const char* java_script)
{
	PrevVisible = false;
	Modal = modal;
	if (PLATFORM_UI->isWebViewAvailable())
	{
		// If local html then display the local html instead
		if (!TempFilename.isEmpty())
			URI = TempFilename;
		if (Modal)
			PLATFORM_UI->CreateWebViewModal(URI.c_str());
		else
		{
			WebView = PLATFORM_UI->CreateWebView(transparent);
			PLATFORM_UI->setWebViewUIView(WebView, this);
			if (!URI.isEmpty())
				PLATFORM_UI->NavigateWebView(WebView, URI.c_str());
			if (java_script != NULL)
				PLATFORM_UI->SendJavascriptToWebView(WebView, java_script);
		}
	}
	else
		return false;

	OldPosition.x = 0;
	OldPosition.y = 0;
	PrevVisible = !IsVisible;
	UpdateVisual();

	return true;
}

void CzUIWebView::UpdatePosSize()
{
	float sw = (float)Scene->getScreenSize().x;
	float sh = (float)Scene->getScreenSize().y;
	float x;
	float y;
	if (PercentagePosition)
	{
		x = ((OriginalPosition.x * sw) / 100.0f);
		y = ((OriginalPosition.y * sh) / 100.0f);
	}
	else
	{
		x = OriginalPosition.x;
		y = OriginalPosition.y;
	}
	if (OriginalSize.x < 0)
		Size.x = (int)((-OriginalSize.x * sw) / 100.0f);
	if (OriginalSize.y < 0)
		Size.y = (int)((-OriginalSize.y * sh) / 100.0f);
	x += (sw - Size.x) / 2;
	y += (sh - Size.y) / 2;

	switch (Docking)
	{
	case CzScene::Dock_Top:
		Position.x = x;
		Position.y = (float)Margin.w;
		break;
	case CzScene::Dock_Bottom:
		Position.x = x;
		Position.y = sh - Size.y - Margin.h;
		break;
	case CzScene::Dock_Left:
		Position.x = (float)Margin.x;
		Position.y = y;
		break;
	case CzScene::Dock_Right:
		Position.x = sw - Size.x - Margin.y;
		Position.y = y;
		break;
	case CzScene::Dock_TopLeft:
		Position.x = (float)Margin.x;
		Position.y = (float)Margin.w;
		break;
	case CzScene::Dock_TopRight:
		Position.x = sw - Size.x - Margin.y;
		Position.y = (float)Margin.w;
		break;
	case CzScene::Dock_BottomLeft:
		Position.x = (float)Margin.x;
		Position.y = sh - Size.y - Margin.h;
		break;
	case CzScene::Dock_BottomRight:
		Position.x = sw - Size.x - Margin.y;
		Position.y = sh - Size.y - Margin.h;
		break;
	default:
		Position.x = x;
		Position.y = y;
	}
}

bool CzUIWebView::UpdateVisual()
{
	if (WebView == NULL)
		return true;

	UpdatePosSize();
	
	if (IsVisible && !PrevVisible)
	{
//		PLATFORM_UI->ShowWebView(WebView, true);
		PLATFORM_UI->setWebViewDimensions(WebView, (int)Position.x, (int)Position.y, (int)Size.x, (int)Size.y);
	}
	else
	if (!IsVisible && PrevVisible)
	{
		PLATFORM_UI->ShowWebView(WebView, false);
	}

	if (IsVisible)
	{
		if (OldPosition.x != Position.x || OldPosition.y != Position.y)
		{
			PLATFORM_UI->setWebViewDimensions(WebView, (int)Position.x, (int)Position.y, (int)Size.x, (int)Size.y);
		}
	}

	PrevVisible = IsVisible;
	OldPosition.x = Position.x;
	OldPosition.y = Position.y;

	return true;
}

int CzUIWebView::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "WebView - Needs to be declared inside a scene or an actor - ", DebugInfo.c_str());
		return 0;
	}

	// Load main actor attributes
	int ret = CzUIBase::LoadFromXoml(parent, false, node);
	if (ret <= 0)
		return ret;

	// Process WebView specific attributes
	bool modal = false;
	bool transparent = false;
	const char* java_script = NULL;

	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();
		if (name_hash == CzHashes::URI_Hash)
			URI = (*it)->getValue();
		else
		if (name_hash == CzHashes::Modal_Hash)
			modal = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::JavaScript_Hash)
			java_script = (*it)->getValue().c_str();
		else
		if (name_hash == CzHashes::Transparent_Hash)
			transparent = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Html_Hash)
			setHtml((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnError_Hash)
			EventsManager->addEvent("OnError", (*it)->getValue().c_str(), true);
		else
		if (name_hash == CzHashes::OnPageLoaded_Hash)
			EventsManager->addEvent("OnPageLoaded", (*it)->getValue().c_str(), true);
		else
		if (name_hash == CzHashes::OnPageLoading_Hash)
			EventsManager->addEvent("OnPageLoading", (*it)->getValue().c_str(), true);
		else
		if (name_hash == CzHashes::OnPageError_Hash)
			EventsManager->addEvent("OnPageError", (*it)->getValue().c_str(), true);
		else
		if (name_hash == CzHashes::OnJavascript_Hash)
			EventsManager->addEvent("OnJavascript", (*it)->getValue().c_str(), true);
	}

	if (!PLATFORM_UI->isWebViewAvailable())
	{
		ProcessEventActions(CZ_HASH("OnError"));
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "WebView - Not supported on this platform - ", DebugInfo.c_str());
		return -1;
	}

	InitView(modal, transparent, java_script);

	// Process inner tags
	if (load_children)
	{
		if (!CZ_XOML->Process(this, node))
			return 0;
	}

	return 1;
}

void CzUIWebView::NotifyOrientationChange(CzScene::eOrientation old_orientation, CzScene::eOrientation new_orientation)
{
	if (WebView != NULL)
		PLATFORM_UI->ShowWebView(WebView, false);
	PrevVisible = false;
	IsVisible = true;

	CzUIBase::NotifyOrientationChange(old_orientation, new_orientation);
}

void CzUIWebView::NotifyPageLoaded(const char* url)
{
	CurrentURI = url;
	ProcessEventActions(CzHashes::OnPageLoaded_Hash);
}

void CzUIWebView::NotifyPageLoading(const char* url)
{
	CurrentURI = url;
	ProcessEventActions(CzHashes::OnPageLoading_Hash);
}

void CzUIWebView::NotifyPageError(const char* url)
{
	CurrentURI = url;
	ProcessEventActions(CzHashes::OnPageError_Hash);
}

void CzUIWebView::NotifyJavascript(const char* data)
{
	Javascript = data;
	ProcessEventActions(CzHashes::OnJavascript_Hash);
}


/**
 @fn	bool CzUIWebView::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the actor.

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types. Note that as 
 CzUIWebView is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param	property_name		Name of the property.
 @param [out]	prop	The property data that.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIWebView::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	if (CzUIBase::getProperty(property_name, prop))
		return true;

	int err = IzXomlResource::getProperty(WebViewClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = WebViewClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "WebView - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIWebView::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the UIWebView.
    
 Sets the named property of this UIWebView. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzUIWebView::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	if (CzUIBase::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(WebViewClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = WebViewClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "WebView - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIWebView::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced. Note that as CzUIWebView is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in]	data	The property data.
 @param	delta				true to property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIWebView::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	if (CzUIBase::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(WebViewClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = WebViewClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "WebView - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIWebView::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates actor property from supplied animation.

 Updates the actors property that is specified by the supplied animation. When a timeline animation targets the property of an actor this method will be called by the animation to move the 
 animations frame data into its target property. Note that as CzUIWebView is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param [in]	animation	If non-null, the animation.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIWebView::UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzUIBase::UpdateFromAnimation(animation))
		return true;

	int err = IzXomlResource::setProperty(WebViewClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = WebViewClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "WebView - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIWebView::UpdateBinding(unsigned int property_name, CzXomlVariable* var)

 @brief	Updates an actor property from the supplied XOML variable.

 Updates a single property from the value of the bound XOML variable. Note that as CzUIWebView is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param	property_name  	Name of the property.
 @param [in,out]	var	If non-null, the variable.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIWebView::UpdateBinding(unsigned int property_name, CzXomlVariable* var)
{
	if (!var->Modified || var->getValue().isEmpty())
		return false;

	if (CzUIBase::UpdateBinding(property_name, var))
		return true;

	int err = IzXomlResource::setProperty(WebViewClassDef, this, property_name, var);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = WebViewClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "WebView - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

