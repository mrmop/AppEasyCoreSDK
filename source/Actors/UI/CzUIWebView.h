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

#if !defined(_CCZ_UI_WEB_VIEW_H_)
#define _CCZ_UI_WEB_VIEW_H_

#include "CzActor.h"
#include "CzScene.h"
#include "CzBrush.h"
#include "CzInput.h"
#include "CzUIBase.h"


//
//
//
//
// CzUIWebView - A wrapper for the Marmalade WebView class providing access to web view display
//
//
//
//
class CzUIWebView : public CzUIBase
{
public:
	// Properties
protected:
	CzString			URI;				// Html to display
	CzString			CurrentURI;			// Current URI (URI last navigated to)
	CzString			Javascript;			// Javaxcript that was passed back from web view
	CzWebView			WebView;			// The created web view object
	bool				Modal;				// If true then takes over full screen and does not go back to app until dismissed

public:
	void				setURI(const char* uri);
	bool				setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool				setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool				getProperty(unsigned int property_name, CzXomlProperty& prop);
	void				setHtml(const char* html);
	void				sendJavascript(const char* js);
	bool				isModal() const				{ return Modal; }
	const CzString&		getCurrentURI() const		{ return CurrentURI; }
	const CzString&		getJavascript() const		{ return Javascript; }
	// Properties end
protected:
	CzString			TempFilename;
	bool				PrevVisible;
	CzVec2				OldPosition;
	CzVec2				OldSize;
	bool				UpdateBinding(unsigned int property_name, CzXomlVariable* var);

	// Event notification callbacks (platform agnostic, athough you can override the event notification methods)
protected:
	CzCallback			PageLoadedHandler;
	CzCallbackData		PageLoadedHandlerData;
	CzCallback			PageLoadingHandler;
	CzCallbackData		PageLoadingHandlerData;
	CzCallback			PageErrorHandler;
	CzCallbackData		PageErrorHandlerData;
	CzCallback			JavascriptHandler;
	CzCallbackData		JavascriptHandlerData;
public:
	void				setPageLoadedHandler(CzCallback handler, CzCallbackData data)	{ PageLoadedHandler = handler; PageLoadedHandlerData = data; }
	void				setPageLoadingHandler(CzCallback handler, CzCallbackData data)	{ PageLoadingHandler = handler; PageLoadingHandlerData = data; }
	void				setPageErrorHandler(CzCallback handler, CzCallbackData data)	{ PageErrorHandler = handler; PageErrorHandlerData = data; }
	void				setJavascriptHandler(CzCallback handler, CzCallbackData data)	{ JavascriptHandler = handler; JavascriptHandlerData = data; }

public:
	CzUIWebView() : CzUIBase(), WebView(NULL), Modal(false), PageLoadedHandler(NULL), PageLoadedHandlerData(NULL), PageLoadingHandler(NULL), PageLoadingHandlerData(NULL), 
					PageErrorHandler(NULL), PageErrorHandlerData(NULL), JavascriptHandler(NULL), JavascriptHandlerData(NULL)  {}
	virtual ~CzUIWebView();

	bool				InitView(bool modal, bool transparent, const char* java_script);
	bool				UpdateVisual();
	void				UpdatePosSize();

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Implementation of IzAnimTarget interface
	bool				UpdateFromAnimation(CzAnimInstance *animation);

	// Event notification
	void				NotifyPageLoaded(const char* url);
	void				NotifyPageLoading(const char* url);
	void				NotifyPageError(const char* url);
	void				NotifyJavascript(const char* data);
	void				NotifyOrientationChange(CzScene::eOrientation old_orientation, CzScene::eOrientation new_orientation);

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	WebViewClassDef;								// XOML class definition
public:
	static void				InitClass();
	static void				ReleaseClass();
	static bool				_setURI(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getJavascript(IzXomlResource* target);
	static bool				_setJavaScript(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setHtml(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getModal(IzXomlResource* target);
	static CzXomlProperty	_getCurrentURI(IzXomlResource* target);
	static bool				_setOnPageLoaded(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnPageLoading(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnPageError(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnJavascript(IzXomlResource* target, const CzXomlProperty& prop, bool add);

};

//
//  CzUIWebViewCreator - Creates an instance of a web view actor object
//
class CzUIWebViewCreator : public IzXomlClassCreator
{
public:
	CzUIWebViewCreator()
	{
		setClassName("webview");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzUIWebView(); }
};


#endif	// _CCZ_UI_WEB_VIEW_H_

