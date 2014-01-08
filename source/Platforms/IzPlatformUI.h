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

#if !defined(_CIZ_PLATFORM_UI_H_)
#define _CIZ_PLATFORM_UI_H_

#include "CzPlatform.h"

#define	PLATFORM_UI			PLATFORM->getUI()

class CzUIWebView;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IzPlatformUI
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IzPlatformUI
{
	// Properties
protected:
	bool		Initialised;
public:
	bool		isInitialised() const	{ return Initialised; }
	// Properties end
public:
	virtual int Init() = 0;
	virtual void Release() = 0;

	// Web view
	virtual	bool		isWebViewAvailable() const = 0;
	virtual	CzWebView	CreateWebView(bool transparent = false) = 0;
	virtual	bool		CreateWebViewModal(const char* uri) = 0;
	virtual	void		DestroyWebView(CzWebView web_view) = 0;
	virtual	void		NavigateWebView(CzWebView web_view, const char* uri) = 0;
	virtual	void		SendJavascriptToWebView(CzWebView web_view, const char* script) = 0;
	virtual	void		ShowWebView(CzWebView web_view, bool visible) = 0;
	virtual	void		setWebViewDimensions(CzWebView web_view, int x, int y, int w, int h) = 0;
	virtual	void		setWebViewUIView(CzWebView web_view, CzUIWebView* ui_view) = 0;

	// Event notification
	virtual	void		NotifyPageLoaded(CzWebView view, const char* uri) = 0;
	virtual	void		NotifyPageLoading(CzWebView view, const char* uri) = 0;
	virtual	void		NotifyPageError(CzWebView view, const char* uri) = 0;
	virtual void		NotifyJavascript(CzWebView view, const char* data) = 0;
};


#endif	// _CIZ_PLATFORM_UI_H_
