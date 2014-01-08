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

#include "CzPlatformUI.h"
#include "CzUIWebView.h"

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
	return false;
}

CzWebView CzPlatformUI::CreateWebView(bool transparent)
{
	return NULL;
}

bool CzPlatformUI::CreateWebViewModal(const char* uri)
{
	if (uri == NULL)
		return false;

	return false;
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
			delete *it;
			WebViews->erase(it);
			break;
		}
	}
	
	// Destroy web view
}

void CzPlatformUI::NavigateWebView(CzWebView web_view, const char* uri)
{
	if (uri == NULL || web_view == NULL)
		return;
}

void CzPlatformUI::SendJavascriptToWebView(CzWebView web_view, const char* script)
{
	if (script == NULL || web_view == NULL)
		return;
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
				break;
			}
		}
	}
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

