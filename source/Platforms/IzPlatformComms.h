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

#if !defined(_CIZ_PLATFORM_COMMS_H_)
#define _CIZ_PLATFORM_COMMS_H_

#include "CzPlatform.h"
#include "CzHttp.h"

#define	PLATFORM_COMMS		PLATFORM->getComms()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IzPlatformComms
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IzPlatformComms
{
	// Properties
protected:
	bool		Initialised;
public:
	bool		isInitialised() const	{ return Initialised; }
	// Properties end
public:
	virtual ~IzPlatformComms() {}
	virtual int Init() = 0;
	virtual void Release() = 0;

	virtual CzHttpObject	HttpCreate() = 0;
	virtual void			HttpDestroy(CzHttpObject http_object) = 0;
	virtual void			HttpUpdate(CzHttpObject http_object) = 0;

	virtual void			HttpCancel(CzHttpObject http_object) = 0;
	virtual bool			HttpGetHeader(CzHttpObject http_object, const char* header_name, CzString& header_data) = 0;
	virtual	int				HttpGetResponseCode(CzHttpObject http_object) = 0;
	virtual void			HttpSetRequestHeader(CzHttpObject http_object, const char* header_name, const char* header_data) = 0;
	virtual bool			HttpProcessRequest(CzHttpObject http_object, CzHttpRequest* request) = 0;

	virtual CzString		HttpDetermineUserAgent() = 0;
};

#endif	// _CIZ_PLATFORM_COMMS_H_
