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

#if !defined(_CCZ_PLATFORM_UI_H_)
#define _CCZ_PLATFORM_UI_H_

#include "IzPlatformUI.h"
#include "CzTypes.h"

class CzPlatformUI: public IzPlatformUI
{
protected:
	struct WebViewData
	{
		CzUIWebView* ui_view;
		CzWebView	view;
		int			x, y, w, h;
		WebViewData(CzWebView v, int x, int y, int w, int h)	{ view = v; this->x = x; this->y = y; this->w = w; this->h = h; ui_view = NULL; }
	};

	// Properties
protected:
	CzList<WebViewData*>*	WebViews;
public:
	// Properties end

protected:
public:
	CzPlatformUI() : WebViews(NULL) {}
	int		Init();
	void	Release();

	// Web view
	bool		isWebViewAvailable() const;
	CzWebView	CreateWebView(bool transparent = false);
	bool		CreateWebViewModal(const char* uri);
	void		DestroyWebView(CzWebView web_view);
	void		NavigateWebView(CzWebView web_view, const char* uri);
	void		SendJavascriptToWebView(CzWebView web_view, const char* script);
	void		ShowWebView(CzWebView web_view, bool visible);
	void		setWebViewDimensions(CzWebView web_view, int x, int y, int w, int h);
	void		setWebViewUIView(CzWebView web_view, CzUIWebView* ui_view);

	// Event notification
	void		NotifyPageLoaded(CzWebView view, const char* uri);
	void		NotifyPageLoading(CzWebView view, const char* uri);
	void		NotifyPageError(CzWebView view, const char* uri);
	void		NotifyJavascript(CzWebView view, const char* data);

	// Web View Callbacks
	static int32	WebViewErrorCallback(CzWebView view, void *systemData, void *userData);
	static int32	WebViewStartedLoadingCallback(CzWebView view, void *systemData, void *userData);
	static int32	WebViewLoadedCallback(CzWebView view, void *systemData, void *userData);
	static int32	WebViewJavascriptCallback(CzWebView view, void *systemData, void *userData);

};


#endif	// _CCZ_PLATFORM_UI_H_
