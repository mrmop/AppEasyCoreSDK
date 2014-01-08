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

#if !defined(_CCZ_PLATFORM_COMMS_H_)
#define _CCZ_PLATFORM_COMMS_H_

#include "IzPlatformComms.h"
#include "IwHTTP.h"

class CzPlatformComms : public IzPlatformComms
{
	// Properties
protected:
public:
	// Properties end
public:
	int		Init();
	void	Release();

	CzHttpObject	HttpCreate();
	void			HttpDestroy(CzHttpObject http_object);
	void			HttpUpdate(CzHttpObject http_object);

	void			HttpCancel(CzHttpObject http_object);
	bool			HttpGetHeader(CzHttpObject http_object, const char* header_name, CzString& header_data);
	void			HttpSetRequestHeader(CzHttpObject http_object, const char* header_name, const char* header_data);
	bool			HttpProcessRequest(CzHttpObject http_object, CzHttpRequest* request);

	CzString		HttpDetermineUserAgent();

	// Internal
	static int		HttpReadContent(CzHttpObject http_object, char *buf, int max_bytes, s3eCallback callback = NULL, void *cb_data = NULL);
	static bool		HttpGetSuccess(CzHttpObject http_object);
	static int		HttpGetContentReceived(CzHttpObject http_object);
	static int		HttpGetContentLength(CzHttpObject http_object);
	static int		HttpGetContentExpected(CzHttpObject http_object);
};


#endif	// _CCZ_PLATFORM_COMMS_H_
