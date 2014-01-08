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

#include "CzPlatformComms.h"
#include "IzPlatformDisplay.h"
#include "CzPlatform.h"


void	CzPlatformComms::Release()
{
}

CzHttpObject CzPlatformComms::HttpCreate()
{
	// Create http object

	return (CzHttpObject)NULL;
}

void CzPlatformComms::HttpDestroy(CzHttpObject http_object)
{
	// Destroy http object
}

void CzPlatformComms::HttpUpdate(CzHttpObject http_object)
{
	// Update the http object
}

void CzPlatformComms::HttpCancel(CzHttpObject http_object)
{
	if (http_object != NULL)
	{
		// Cancel any requests
	}
}

bool CzPlatformComms::HttpGetSuccess(CzHttpObject http_object)
{
	return false;
}

int CzPlatformComms::HttpGetContentReceived(CzHttpObject http_object)
{
	return -1;
}

int CzPlatformComms::HttpGetContentLength(CzHttpObject http_object)
{
	return -1;
}

int CzPlatformComms::HttpGetContentExpected(CzHttpObject http_object)
{
	return -1;
}

int CzPlatformComms::HttpReadContent(CzHttpObject http_object, char *buf, int max_bytes, s3eCallback callback, void *cb_data)
{
	return 0;
}

void CzPlatformComms::HttpSetRequestHeader(CzHttpObject http_object, const char* header_name, const char* header_data)
{
}

bool CzPlatformComms::HttpGetHeader(CzHttpObject http_object, const char* header_name, CzString& header_data)
{
	return false;
}

bool CzPlatformComms::HttpProcessRequest(CzHttpObject http_object, CzHttpRequest* request)
{
	if (!request->isPOST())
	{
		// Process the GET request
		request->ApplyHeaders();
	}
	else
	{
		// Process the POST request
		request->ApplyHeaders();
	}

	return false;
}

CzString CzPlatformComms::HttpDetermineUserAgent()
{
	int width = PLATFORM_DISPLAY->getCurrentWidth();
	int height = PLATFORM_DISPLAY->getCurrentHeight();

	// Determine user agent string

	return CzString("");
}

