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

#if !defined(_CIZ_PLATFORM_FACEBOOK_H_)
#define _CIZ_PLATFORM_FACEBOOK_H_

#include "CzPlatform.h"
#include "CzString.h"
#include "CzScript.h"

#define	PLATFORM_FACEBOOK			PLATFORM->getFacebook()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IzPlatformFacebook
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IzPlatformFacebook
{
public:
	enum eFBStatus 
	{
		FB_UNINITIALISED,
		FB_LOGGED_OUT,
		FB_LOGGING_IN,
		FB_LOGGED_IN
	};

	// Properties
protected:
	bool				Available;			///< Facebook available flag
	bool				Initialised;		///< Classes iniitialised state
	CzString			AppID;				///< Facebook App iD
	CzScriptCallback	ScriptCallback;		///< Script call back
public:
	bool				isAvailable() const							{ return Available; }
	bool				isInitialised() const						{ return Initialised; }
	void				setAppID(const char* app_id)				{ AppID = app_id; }
	CzScriptCallback&	getScriptCallback()							{ return ScriptCallback; }
	// Properties end

public:
	virtual ~IzPlatformFacebook() {}
	virtual int			Init() = 0;
	virtual void		Release() = 0;

	virtual bool		Login(const char** permissions, int num_permissions) = 0;
	virtual bool		Logout() = 0;
	virtual bool		Reauthorise(const char** permissions, int num_permissions) = 0;
	virtual CzFacebookRequest	InitRequest(const char* method_name, const char* http_method) = 0;
	virtual CzFacebookRequest	InitGraphRequest(const char* graph_path, const char* http_method) = 0;
	virtual bool		AddParamString(CzFacebookRequest request, const char* parameter, const char* value) = 0;
	virtual bool		AddParamNumber(CzFacebookRequest request, const char* parameter, int64 value) = 0;
	virtual bool		SendRequest(CzFacebookRequest request) = 0;
	virtual void		DeleteRequest(CzFacebookRequest request) = 0;
	virtual const char* GetResponse(CzFacebookRequest request) = 0;
	virtual int			GetErrorCode(CzFacebookRequest request) = 0;
	virtual const char* GetError(CzFacebookRequest request) = 0;
	virtual const char*	GetAccessToken() = 0;
	virtual bool		IsLoggedIn() const = 0;
	virtual void		WaitForCallback() = 0;

//	virtual bool	PostWall(const char* message, const char* link_uri, const char* image_uri, const char* name, const char* description) = 0;

	// Event handlers
	void				NotifyLoggedIn(CzFacebookRequest request);
	void				NotifyLoginError(CzFacebookRequest request);
	void				NotifyResponse(CzFacebookRequest request);
	void				NotifyResponseError(CzFacebookRequest request);
};


#endif	// _CIZ_PLATFORM_FACEBOOK_H_
