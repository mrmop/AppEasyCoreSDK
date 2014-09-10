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

#if !defined(_CCZ_PLATFORM_FACEBOOK_H_)
#define _CCZ_PLATFORM_FACEBOOK_H_

#if defined(CZ_FACEBOOK_ENABLE)
#include "IzPlatformFacebook.h"
#include "CzString.h"
#include "s3eFacebook.h"

class CzPlatformFacebook: public IzPlatformFacebook
{
	// Properties
protected:
public:
	// Properties end

protected:

public:
	CzPlatformFacebook() {}
	int					Init();
	void				Release();

	bool				Login(const char** permissions, int num_permissions);
	bool				Logout();
	bool				Reauthorise(const char** permissions, int num_permissions);
	CzFacebookRequest	InitRequest(const char* method_name, const char* http_method);
	CzFacebookRequest	InitGraphRequest(const char* graph_path, const char* http_method);
	bool				AddParamString(CzFacebookRequest request, const char* parameter, const char* value);
	bool				AddParamNumber(CzFacebookRequest request, const char* parameter, int64 value);
	bool				SendRequest(CzFacebookRequest request);
	void				DeleteRequest(CzFacebookRequest request);
	const char*			GetResponse(CzFacebookRequest request);
	int					GetErrorCode(CzFacebookRequest request);
	const char*			GetError(CzFacebookRequest request);
	const char*			GetAccessToken();
	bool				IsLoggedIn() const							{ return SessionStatus == FB_LOGGED_IN; }

//	bool				PostWall(const char* message, const char* link_uri, const char* image_uri, const char* name, const char* description);


	// Internal
protected:
	eFBStatus			SessionStatus;
	s3eFBSession*		CurrentSession;
	bool				WaitingForCallback;
public:
	void				setWaitingForCallback(bool waiting)			{ WaitingForCallback = waiting; }
	void				setSessionStatus(eFBStatus status)			{ SessionStatus = status; }
	void				setCurrentSession(s3eFBSession* session)	{ CurrentSession = session; }
	void				WaitForCallback();
	
};
#endif	// CZ_FACEBOOK_ENABLE


#endif	// _CCZ_PLATFORM_FACEBOOK_H_
