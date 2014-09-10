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

//
//  Sample user agents
//
//	UserAgent = "Mozilla/5.0 (SAMSUNG; SAMSUNG-GT-S8500/S8500XXJL2; U; Bada/1.2; fr-fr) AppleWebKit/533.1 (KHTML, like Gecko) Dolfin/2.2 Mobile WVGA SMM-MMS/1.2.0 OPN-B"	// Bada Sample
//  UserAgent = "Mozilla/5.0 (webOS/1.0; U; en-US) AppleWebKit/525.27.1 (KHTML, like Gecko) Version/1.0 Safari/525.27.1 Pre/1.0"											// WebOS Sample
//	UserAgent = "Mozilla/5.0 (iPad; U; CPU OS 3_2 like Mac OS X; en-us) AppleWebKit/531.21.10 (KHTML, like Gecko) Version/4.0.4 Mobile/7B367 Safari/531.21.10"				// iOS Sample
//	UserAgent = "Mozilla/5.0 (Linux; U; Android 2.1; en-gb; HTC DESIRE) AppleWebKit/530.17 (KHTML, like Gecko) Version/4.0 Mobile Safari/530.17";							// Android Sample
//	UserAgent = "Mozilla/5.0 (PlayBook; U; RIM Tablet OS 1.0.0; en-US) AppleWebKit/534.8+ (KHTML, like Gecko) Version/0.0.1 Safari/534.8+";									// Blackberry Tablet OS
//	UserAgent = "Mozilla/5.0 (SymbianOS/9.1; U; en-us) AppleWebKit/413 (KHTML, like Gecko) Safari/413";																		// Symbian Sample
//  UserAgent = "HTC_Touch_Pro_T7272 Opera/9.50 (Windows NT 5.1; U; en)";																									// Windows Mobile Sample
//  UserAgent = "Mozilla/4.0 (compatible; MSIE 4.01; Windows CE; PPC; 240x320)";																							// Win CE Sample
//  UserAgent = "Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1; Trident/6.0)";																							// Windows IE Sample
//  UserAgent = "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_6_8; de-at) AppleWebKit/533.21.1 (KHTML, like Gecko) Version/5.0.5 Safari/533.21.1";							// OSX Sample


#include "Marm/CzPlatformComms.h"
#include "s3eSocket.h"
#include "UserAgent.h"
#include "IzPlatformDisplay.h"
#include "CzPlatform.h"

//
// Marmalade Communication system
//

static int32 GotData(void*, void*)
{
	// This is the callback indicating that a ReadContent call has completed.  Either we've finished, or a bigger buffer is needed.  If the correct ammount of data was supplied initially,
	// then this will only be called once. However, it may well be called several times when using chunked encoding.
	if (CZ_HTTP_MANAGER == NULL)
		return 0;
	CzHttpRequest* req = CZ_HTTP_MANAGER->getCurrentRequest();
	if (req == NULL)
		return 0;

	CzHttpObject http_object = CZ_HTTP_MANAGER->getHttpObject();
	if (http_object == NULL)
		return 0;
//CIwHTTP* http = static_cast<CIwHTTP*>(http_object);

	// Firstly see if there's an error condition.
	if (!CzPlatformComms::HttpGetSuccess(http_object))
	{
		// Something has gone wrong
		req->EndRequest(-3);
		return -1;
	}
	else
	if (CzPlatformComms::HttpGetContentReceived(http_object) != CzPlatformComms::HttpGetContentLength(http_object))
	{
		// We have some data but not all of it. We need more space.
		int len = req->getContentLength() + 1;
		int old_len = len;

		// If iwhttp has a guess how big the next bit of data is (this basically means chunked encoding is being used), allocate that much space. Otherwise guess.
		if (len < CzPlatformComms::HttpGetContentExpected(http_object))
			len = CzPlatformComms::HttpGetContentExpected(http_object);
		else
			len += 1024;

		// Allocate some more space and fetch the data.
		req->reallocContent(len);
		CzPlatformComms::HttpReadContent(http_object, req->getContent().str() + old_len, len - old_len, GotData);
	}
	else
	{
		// All data retreived
		*(req->getContent().str() + req->getContentLength() - 1) = 0;
		req->EndRequest(0);
	}

	return 0;
}

//
// Called when the response headers have been received
//
static int32 GotHeaders(void*, void*)
{
	if (CZ_HTTP_MANAGER == NULL)
		return 0;
	CzHttpRequest* req = CZ_HTTP_MANAGER->getCurrentRequest();
	if (req == NULL)
		return 0;
	CzHttpObject http_object = CZ_HTTP_MANAGER->getHttpObject();
	if (http_object == NULL)
		return 0;
//CIwHTTP* http = static_cast<CIwHTTP*>(http_object);

	if (!CzPlatformComms::HttpGetSuccess(http_object))
	{
		// Something has gone wrong
		req->EndRequest(-2);
		return -1;
	}
	else
	{
		req->setError(0);

		// Depending on how the server is communicating the content length, we may actually know the length of the content, or we may know the length of the first part of it, or we may
		// know nothing. ContentExpected always returns the smallest possible size of the content, so allocate that much space for now if it's non-zero. If it is of zero size, the server
		// has given no indication, so we need to guess. We'll guess at 1k.
		int len = CzPlatformComms::HttpGetContentExpected(http_object);
		if (len <= 0)
			len = 1024;

		req->allocContent(len);
		CzPlatformComms::HttpReadContent(http_object, req->getContent().str(), req->getContentLength(), GotData, NULL);
	}

	return 0;
}

int		CzPlatformComms::Init()
{
	Initialised = true;

	return 1;
}

void	CzPlatformComms::Release()
{
}

CzHttpObject CzPlatformComms::HttpCreate()
{
	return (CzHttpObject)new CIwHTTP;
}

void CzPlatformComms::HttpDestroy(CzHttpObject http_object)
{
	delete static_cast<CIwHTTP*>(http_object);
}

void CzPlatformComms::HttpUpdate(CzHttpObject http_object)
{
}

void CzPlatformComms::HttpCancel(CzHttpObject http_object)
{
	if (http_object != NULL)
		static_cast<CIwHTTP*>(http_object)->Cancel();
}

bool CzPlatformComms::HttpGetSuccess(CzHttpObject http_object)
{
	return static_cast<CIwHTTP*>(http_object)->GetStatus() == S3E_RESULT_SUCCESS;
}

int CzPlatformComms::HttpGetContentReceived(CzHttpObject http_object)
{
	return static_cast<CIwHTTP*>(http_object)->ContentReceived();
}

int CzPlatformComms::HttpGetContentLength(CzHttpObject http_object)
{
	return static_cast<CIwHTTP*>(http_object)->ContentLength();
}

int CzPlatformComms::HttpGetContentExpected(CzHttpObject http_object)
{
	return static_cast<CIwHTTP*>(http_object)->ContentExpected();
}

int CzPlatformComms::HttpReadContent(CzHttpObject http_object, char *buf, int max_bytes, s3eCallback callback, void *cb_data)
{
	static_cast<CIwHTTP*>(http_object)->ReadDataAsync(buf, max_bytes, 10000, callback);

	return 0;
}

void CzPlatformComms::HttpSetRequestHeader(CzHttpObject http_object, const char* header_name, const char* header_data)
{
	static_cast<CIwHTTP*>(http_object)->SetRequestHeader(header_name, header_data);
}

int CzPlatformComms::HttpGetResponseCode(CzHttpObject http_object)
{
	CIwHTTP* mhttp = (CIwHTTP*)http_object;
	return mhttp->GetResponseCode();

}

bool CzPlatformComms::HttpGetHeader(CzHttpObject http_object, const char* header_name, CzString& header_data)
{
	std::string type;
	if (!static_cast<CIwHTTP*>(http_object)->GetHeader(header_name, type))
		return false;

	header_data = type.c_str();

	return true;
}

bool CzPlatformComms::HttpProcessRequest(CzHttpObject http_object, CzHttpRequest* request)
{
	if (!request->isPOST())
	{
		// Process the GET request
		request->ApplyHeaders();
		if (static_cast<CIwHTTP*>(http_object)->Get(request->getURI().c_str(), GotHeaders, NULL) != S3E_RESULT_SUCCESS)
		{
			// Failed so set error and free up manager to process another request
			request->EndRequest(-1);
			return true;
		}
	}
	else
	{
		// Process the POST request
		request->ApplyHeaders();
		if (static_cast<CIwHTTP*>(http_object)->Post(request->getURI().c_str(), request->getBody().c_str(), request->getBody().getLength() + 1, GotHeaders, NULL) != S3E_RESULT_SUCCESS)
		{
			// Failed so set error and free up manager to process another request
			request->EndRequest(-1);
			return true;
		}
	}

	return false;
}

CzString CzPlatformComms::HttpDetermineUserAgent()
{
	int width = PLATFORM_DISPLAY->getCurrentWidth();
	int height = PLATFORM_DISPLAY->getCurrentHeight();
	int os = s3eDeviceGetInt(S3E_DEVICE_OS);
	CzString UserAgent;

	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "device_os = ", s3eDeviceGetString(S3E_DEVICE_OS));
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "device_class = ", s3eDeviceGetString(S3E_DEVICE_CLASS));
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "device_id = ", s3eDeviceGetString(S3E_DEVICE_ID));
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "device_architecture = ", s3eDeviceGetString(S3E_DEVICE_ARCHITECTURE));
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "device_os_version = ", s3eDeviceGetString(S3E_DEVICE_OS_VERSION));
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "device_chipset = ", s3eDeviceGetString(S3E_DEVICE_CHIPSET));
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "device_sdk_version = ", s3eDeviceGetString(S3E_DEVICE_SDK_VERSION));
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "device_locale = ", s3eDeviceGetString(S3E_DEVICE_LOCALE));
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "device_name = ", s3eDeviceGetString(S3E_DEVICE_NAME));

	if (UserAgentAvailable())
	{
		UserAgent = ::getUserAgent();	// Get user-agent from extension
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, ">>>>UserAgent: ", UserAgent.c_str());
	}

/*	if (UserAgent.isEmpty())
	{
		CzString device_id(s3eDeviceGetString(S3E_DEVICE_ID));
		CzString locale(s3eDeviceGetString(S3E_DEVICE_LOCALE));

		// Format locale string
		locale.ToLower();
		locale.Replace('_', '-');

		switch (os)
		{
		case S3E_OS_ID_WINMOBILE:
			UserAgent = device_id;
			UserAgent += " Opera/9.50 (Windows NT 5.1; U; ";
			UserAgent += locale;
			UserAgent += ")";
			break;
		case S3E_OS_ID_WINCE:
			UserAgent += "Mozilla/4.0 (compatible; MSIE 4.01; Windows CE; ";
			UserAgent = device_id;
			UserAgent += "; ";
			UserAgent += locale;
			UserAgent += "; )";
			UserAgent += CzString(width);
			UserAgent += "x";
			UserAgent += CzString(height);
			UserAgent += ")";
			break;
		case S3E_OS_ID_QNX:
			UserAgent = "Mozilla/5.0 (";
			UserAgent += s3eDeviceGetString(S3E_DEVICE_OS);
			UserAgent += "/";
			UserAgent += device_id;
			UserAgent += "; U; ";
			UserAgent += locale;
			UserAgent += ") AppleWebKit/534.8+ (KHTML, like Gecko) Version/0.0.1 Safari/534.8+";
			break;
		case S3E_OS_ID_ANDROID:
			UserAgent = "Mozilla/5.0 (Linux; U; Android 2.1; ";
			UserAgent += locale;
			UserAgent += "; ";
			UserAgent += device_id;
			UserAgent += ") AppleWebKit/530.17 (KHTML, like Gecko) Version/4.0 Mobile Safari/530.17";
			break;
		case S3E_OS_ID_IPHONE:
			UserAgent = "Mozilla/5.0 (";
			UserAgent += s3eDeviceGetString(S3E_DEVICE_OS);
			UserAgent += "/";
			UserAgent += device_id;
			UserAgent += "; U; ";
			UserAgent += locale;
			UserAgent += "; like Mac OS X; ";
			UserAgent += locale;
			UserAgent += ") AppleWebKit/533.17.9 (KHTML, like Gecko) Version/5.0.2 Mobile/8C148 Safari/6533.18.5";
			break;
		case S3E_OS_ID_WEBOS:
			UserAgent = "Mozilla/5.0 (webOS/1.0; U; ";
			UserAgent += locale;
			UserAgent += ") AppleWebKit/525.27.1 (KHTML, like Gecko) Version/1.0 Safari/525.27.1 ";
			UserAgent += device_id;
			break;
		case S3E_OS_ID_WINDOWS:
			UserAgent = "Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.1; Trident/6.0; ";
			UserAgent += locale;
			UserAgent += ")";
			break;
		case S3E_OS_ID_LINUX:
			UserAgent = "Mozilla/5.0 (";
			UserAgent += s3eDeviceGetString(S3E_DEVICE_OS);
			UserAgent += "/";
			UserAgent += device_id;
			UserAgent += "; U; ";
			UserAgent += locale;
			UserAgent += ") AppleWebKit/534.8+ (KHTML, like Gecko) Version/0.0.1 Safari/534.8+";
			break;
		case S3E_OS_ID_OSX:
			UserAgent = "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_6_8; ";
			UserAgent += locale;
			UserAgent += ") AppleWebKit/533.21.1 (KHTML, like Gecko) Version/5.0.5 Safari/533.21.1";
			break;
		default:
			UserAgent = "Mozilla/5.0 (";
			UserAgent += s3eDeviceGetString(S3E_DEVICE_OS);
			UserAgent += "/";
			UserAgent += device_id;
			UserAgent += "; U; Bada/1.2; ";
			UserAgent += locale;
			UserAgent += ") AppleWebKit/533.1 (KHTML, like Gecko) Dolfin/2.2 Mobile ";
			const char* mode_name = CzUtils::GetGraphicModeName(width, height);
			if (mode_name != NULL)
				UserAgent += mode_name;
			UserAgent += " SMM-MMS/1.2.0 OPN-B";
		}
	}*/

	return CzString(UserAgent.c_str());
}

/*bool CzHttpManager::HttpDetermineIPAddress()
{
	s3eSocket* sock = s3eSocketCreate(S3E_SOCKET_TCP, 0);
	if (sock != NULL)
	{
		s3eInetAddress addr;
		memset(&addr, 0, sizeof(addr));
		if (s3eInetLookup(PING_DOMAIN, &addr, NULL, NULL) != S3E_RESULT_ERROR)
		{
			addr.m_Port = s3eInetHtons(80);

			uint64 start_time = CzTimer::GetCurrentTimeMs();
			while (1)
			{
				s3eResult res = s3eSocketConnect(sock, &addr, NULL, NULL);
				if (res == S3E_RESULT_SUCCESS)
				{
					s3eInetAddress address;
					memset(&address, 0, sizeof(address));
					if (s3eSocketGetLocalName(sock, &address) != S3E_RESULT_ERROR)
					{
						IPAddress.allocString(32);
						IPAddress = s3eInetNtoa(address.m_IPAddress, IPAddress.str(), 32);
						CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "IPAddress: ", IPAddress.c_str());
						s3eSocketClose(sock);
						return true;
					}
					s3eSocketClose(sock);
					return false;
				}
				else
				{
					switch (s3eSocketGetError())
					{
						case S3E_SOCKET_ERR_INPROGRESS:
						case S3E_SOCKET_ERR_ALREADY:
						case S3E_SOCKET_ERR_WOULDBLOCK:
							break;
						default:
							s3eSocketClose(sock);
							return false;
					}
				}
				// Socket request timed out
				if ((CzTimer::GetCurrentTimeMs() - start_time) > 10000)
				{
					s3eSocketClose(sock);
					return false;
				}
				s3eDeviceYield(30);
			}
		}
	}

	return false;
}*/

