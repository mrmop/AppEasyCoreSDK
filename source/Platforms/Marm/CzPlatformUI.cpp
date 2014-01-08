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

#include "Marm/CzPlatformUI.h"
#include "CzUIWebView.h"
#include "s3eWebView.h"

int CzPlatformUI::Init()
{
	Initialised = true;

	WebViews = new CzList<WebViewData*>();

	return 1;
}

void CzPlatformUI::Release()
{
	// Clean up web view data
	if (WebViews != NULL)
	{
		for (CzList<WebViewData*>::iterator it = WebViews->begin(); it != WebViews->end(); ++it)
			delete *it;

		SAFE_DELETE(WebViews)
	}
}

//
//
//
//
//
//
// Web View UI
//
//
//
//
//
//
bool CzPlatformUI::isWebViewAvailable() const
{
	return s3eWebViewAvailable() == S3E_TRUE;
}

CzWebView CzPlatformUI::CreateWebView(bool transparent)
{
	if (!s3eWebViewAvailable())
		return NULL;

	CzWebView view = (CzWebView)s3eWebViewCreate(transparent);
	if (view == NULL)
		return NULL;

	WebViewData* data = new WebViewData(view, 0, 0, 0, 0);
	WebViews->push_back(data);
	s3eWebViewRegister(S3E_WEBVIEW_FAILED_LOADING, (s3eWebViewCallbackFn)CzPlatformUI::WebViewErrorCallback, NULL, (s3eWebView*)view);
	s3eWebViewRegister(S3E_WEBVIEW_STARTED_LOADING, (s3eWebViewCallbackFn)CzPlatformUI::WebViewStartedLoadingCallback, NULL, (s3eWebView*)view);
	s3eWebViewRegister(S3E_WEBVIEW_FINISHED_LOADING, (s3eWebViewCallbackFn)CzPlatformUI::WebViewLoadedCallback, NULL, (s3eWebView*)view);
	s3eWebViewRegister(S3E_WEBVIEW_FROM_JAVASCRIPT, (s3eWebViewCallbackFn)CzPlatformUI::WebViewJavascriptCallback, NULL, (s3eWebView*)view);

	return view;
}

bool CzPlatformUI::CreateWebViewModal(const char* uri)
{
	if (uri == NULL)
		return false;
	if (!s3eWebViewAvailable())
		return false;

	return s3eWebViewCreateModal(uri) == S3E_TRUE;
}

void CzPlatformUI::DestroyWebView(CzWebView web_view)
{
	if (web_view == NULL)
		return;
	// Remove web view data
	for (CzList<WebViewData*>::iterator it = WebViews->begin(); it != WebViews->end(); ++it)
	{
		if ((*it)->view == web_view)
		{
			s3eWebViewUnRegister(S3E_WEBVIEW_FAILED_LOADING, (s3eWebViewCallbackFn)CzPlatformUI::WebViewErrorCallback, (s3eWebView*)web_view);
			s3eWebViewUnRegister(S3E_WEBVIEW_STARTED_LOADING, (s3eWebViewCallbackFn)CzPlatformUI::WebViewStartedLoadingCallback, (s3eWebView*)web_view);
			s3eWebViewUnRegister(S3E_WEBVIEW_FINISHED_LOADING, (s3eWebViewCallbackFn)CzPlatformUI::WebViewLoadedCallback, (s3eWebView*)web_view);
			s3eWebViewUnRegister(S3E_WEBVIEW_FROM_JAVASCRIPT, (s3eWebViewCallbackFn)CzPlatformUI::WebViewJavascriptCallback, (s3eWebView*)web_view);
			delete *it;
			WebViews->erase(it);
			break;
		}
	}
	
	// Destroy web view
	s3eWebViewDestroy(static_cast<s3eWebView*>(web_view));
}

void CzPlatformUI::NavigateWebView(CzWebView web_view, const char* uri)
{
	if (uri == NULL || web_view == NULL)
		return;
	s3eWebViewNavigate(static_cast<s3eWebView*>(web_view), uri);
}

void CzPlatformUI::SendJavascriptToWebView(CzWebView web_view, const char* script)
{
	if (script == NULL || web_view == NULL)
		return;
	s3eWebViewSendJavaScript(static_cast<s3eWebView*>(web_view), script);
}

void CzPlatformUI::ShowWebView(CzWebView web_view, bool visible)
{
	if (web_view == NULL)
		return;
	if (visible)
	{
		// Update web view dimensions
		for (CzList<WebViewData*>::iterator it = WebViews->begin(); it != WebViews->end(); ++it)
		{
			if ((*it)->view == web_view)
			{
				s3eWebViewShow(static_cast<s3eWebView*>(web_view), (*it)->x, (*it)->y, (*it)->w, (*it)->h);
				break;
			}
		}
	}
	else
		s3eWebViewHide(static_cast<s3eWebView*>(web_view));
}

void CzPlatformUI::setWebViewDimensions(CzWebView web_view, int x, int y, int w, int h)
{
	if (web_view == NULL)
		return;
	// Update web view dimensions
	for (CzList<WebViewData*>::iterator it = WebViews->begin(); it != WebViews->end(); ++it)
	{
		if ((*it)->view == web_view)
		{
			(*it)->x = x;
			(*it)->y = y;
			(*it)->w = w;
			(*it)->h = h;
			break;
		}
	}

	s3eWebViewShow(static_cast<s3eWebView*>(web_view), x, y, w, h);
}

void CzPlatformUI::setWebViewUIView(CzWebView web_view, CzUIWebView* ui_view)
{
	if (web_view == NULL)
		return;
	for (CzList<WebViewData*>::iterator it = WebViews->begin(); it != WebViews->end(); ++it)
	{
		if ((*it)->view == web_view)
			(*it)->ui_view = ui_view;
	}
}

void CzPlatformUI::NotifyPageLoaded(CzWebView view, const char* url)
{
	if (view == NULL)
		return;
	for (CzList<WebViewData*>::iterator it = WebViews->begin(); it != WebViews->end(); ++it)
	{
		if ((*it)->view == view)
		{
			if ((*it)->ui_view != NULL)
				(*it)->ui_view->NotifyPageLoaded(url);
		}
	}
}

void CzPlatformUI::NotifyPageLoading(CzWebView view, const char* url)
{
	if (view == NULL)
		return;
	for (CzList<WebViewData*>::iterator it = WebViews->begin(); it != WebViews->end(); ++it)
	{
		if ((*it)->view == view)
		{
			if ((*it)->ui_view != NULL)
				(*it)->ui_view->NotifyPageLoading(url);
		}
	}
}

void CzPlatformUI::NotifyPageError(CzWebView view, const char* url)
{
	if (view == NULL)
		return;
	for (CzList<WebViewData*>::iterator it = WebViews->begin(); it != WebViews->end(); ++it)
	{
		if ((*it)->view == view)
		{
			if ((*it)->ui_view != NULL)
				(*it)->ui_view->NotifyPageError(url);
		}
	}
}

void CzPlatformUI::NotifyJavascript(CzWebView view, const char* data)
{
	if (view == NULL)
		return;
	for (CzList<WebViewData*>::iterator it = WebViews->begin(); it != WebViews->end(); ++it)
	{
		if ((*it)->view == view)
		{
			if ((*it)->ui_view != NULL)
				(*it)->ui_view->NotifyJavascript(data);
		}
	}
}

int32 CzPlatformUI::WebViewErrorCallback(CzWebView view, void *systemData, void *userData)
{
	PLATFORM_UI->NotifyPageError(view, (const char*)systemData);
	return 1;
}

int32 CzPlatformUI::WebViewStartedLoadingCallback(CzWebView view, void *systemData, void *userData)
{
	PLATFORM_UI->NotifyPageLoading(view, (const char*)systemData);
	return 1;
}

int32 CzPlatformUI::WebViewLoadedCallback(CzWebView view, void *systemData, void *userData)
{
	PLATFORM_UI->NotifyPageLoaded(view, (const char*)systemData);
	return 1;
}

int32 CzPlatformUI::WebViewJavascriptCallback(CzWebView view, void *systemData, void *userData)
{
	PLATFORM_UI->NotifyJavascript(view, (const char*)systemData);
	return 1;
}
