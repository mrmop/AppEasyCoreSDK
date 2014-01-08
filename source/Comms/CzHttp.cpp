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

#include "IzPlatformComms.h"
#include "CzTime.h"
#include "CzHttp.h"

// ** IMPORTANT NOTE - Change the ping domain to your own domain
// ** IMPORTANT NOTE - Change the ping domain to your own domain
// ** IMPORTANT NOTE - Change the ping domain to your own domain
#define PING_DOMAIN	"ping.pocketeers.co.uk"


//
//
//
//
// CzHttpPostData - implementation
//
//
//
//
void CzHttpPostData::ApplyHeaders()
{
	for (CzHttpHeaderIterator it = Headers.begin(); it != Headers.end(); ++it)
	{
		PLATFORM_COMMS->HttpSetRequestHeader(CZ_HTTP_MANAGER->getHttpObject(), (*it)->getHeader().c_str(), (*it)->getHeaderData());
	}
}

//
//
//
//
// CzHttpRequest - implementation
//
//
//
//
void CzHttpRequest::EndRequest(int error)
{
	if (CZ_HTTP_MANAGER == NULL)
		return;

	Error = error;
	Processed = true;

	CZ_HTTP_MANAGER->RemoveRequest(this);				// Remove request from http manager queue

	if (ContentAvailableCallback != NULL)
		ContentAvailableCallback((void*)this, ContentAvailableCallbackData);

	CZ_HTTP_MANAGER->setNoRequest();

//	if (AutoDelete)
//		delete this;
}

void CzHttpRequest::ApplyHeaders()
{
	PostData->ApplyHeaders();
}


//
//
//
//
// CzHttpManager - implementation
//
//
//
//

CDECLARE_SINGLETON(CzHttpManager)

void CzHttpManager::Init()
{
	HttpObject = PLATFORM_COMMS->HttpCreate();

	CurrentRequest = NULL;

	// Generate user-agent and IP address
	PLATFORM_COMMS->HttpDetermineUserAgent();
//	PLATFORM_COMMS->HttpDetermineIPAddress();
}

void CzHttpManager::Release()
{
	// Cacnel pending request
	CancelRequests();

	// Clear penging requests
	Requests.clear();
	CurrentRequest = NULL;

	// Destopry HTTP platform object
	PLATFORM_COMMS->HttpDestroy(HttpObject);
}

bool CzHttpManager::GetHeader(const char* header_name, CzString& header_data)
{
	return PLATFORM_COMMS->HttpGetHeader(HttpObject, header_name, header_data);
}

void CzHttpManager::AddRequest(CzHttpRequest* request)
{
	Requests.push_back(request);
//	int len = request->getBody().getLength();
//	if (len > 0)
//		request->SetHeader("Content-Length", CzString(len).c_str());
//	request->SetHeader("Content-Type", "application/x-www-form-urlencoded");
	request->setProcessed(false);
}

void CzHttpManager::RemoveRequest(CzHttpRequest* request)
{
	Requests.remove(request);
}

void CzHttpManager::ClearRequests()
{
	Requests.clear();
}

bool CzHttpManager::isEmpty() const
{
	return Requests.size() > 0;
}

void CzHttpManager::Update()
{
	PLATFORM_COMMS->HttpUpdate(HttpObject);

	typedef CzList<CzHttpRequest*>::iterator _iterator;
	typedef CzList<CzHttpRequest*>::reverse_iterator _riterator;

	if (CurrentRequest == NULL)
	{
		for(_iterator it = Requests.begin(); it != Requests.end(); ++it)
		{
			CzHttpRequest* req = *it;

			// If request has not been processed then process it
			if (!req->getProcessed())
			{
				CurrentRequest = req;
				PLATFORM_COMMS->HttpProcessRequest(HttpObject, req);
				break;
			}
		}
	}
}

CzString CzHttpManager::DetermineUserAgent()
{
	return PLATFORM_COMMS->HttpDetermineUserAgent();
}

void CzHttpManager::CancelRequests()
{
	ClearRequests();
	PLATFORM_COMMS->HttpCancel(HttpObject);
}
	
