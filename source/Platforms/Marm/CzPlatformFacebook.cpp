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

#include "Marm/CzPlatformFacebook.h"
#include "s3e.h"

void IzPlatformFacebook::NotifyLoggedIn(CzFacebookRequest request)
{
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzString p1 = "loggedin";
		callback.ScriptEngine->CallFunctionRefWithObject(request, callback.FunctionRef, &p1, NULL, NULL, NULL);
	}
}

void IzPlatformFacebook::NotifyLoginError(CzFacebookRequest request)
{
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzString p1 = "logginerror";
		callback.ScriptEngine->CallFunctionRefWithObject(request, callback.FunctionRef, &p1, NULL, NULL, NULL);
	}
}

void IzPlatformFacebook::NotifyResponse(CzFacebookRequest request)
{
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzString p1 = "response";
		callback.ScriptEngine->CallFunctionRefWithObject(request, callback.FunctionRef, &p1, NULL, NULL, NULL);
	}
}

void IzPlatformFacebook::NotifyResponseError(CzFacebookRequest request)
{
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzString p1 = "responseerror";
		callback.ScriptEngine->CallFunctionRefWithObject(request, callback.FunctionRef, &p1, NULL, NULL, NULL);
	}
}

int CzPlatformFacebook::Init()
{
	Available = false;
	SessionStatus = FB_UNINITIALISED;
	CurrentSession = 0;

	if (s3eExtFacebookAvailable())
		Available = true;
	else
		return 0;

	Initialised = true;
	return 1;
}

void CzPlatformFacebook::Release()
{
	if (CurrentSession)
	{
		s3eFBTerminate(CurrentSession);
		CurrentSession = NULL;
	}
	Initialised = false;
}


static void FBLoginCallback(struct s3eFBSession* session, s3eResult* loginResult, void* userData)
{
//    IwTrace(FACEBOOK, ("FBLoginCallback %p %p %p", session, loginResult, userData));
	
	if (*loginResult == S3E_RESULT_SUCCESS)
	{
		((CzPlatformFacebook*)PLATFORM_FACEBOOK)->setSessionStatus(CzPlatformFacebook::FB_LOGGED_IN);
		PLATFORM_FACEBOOK->NotifyLoggedIn(NULL);
	}
	else
	{
		((CzPlatformFacebook*)PLATFORM_FACEBOOK)->setSessionStatus(CzPlatformFacebook::FB_LOGGED_OUT); // Note: this currently never occurs
		PLATFORM_FACEBOOK->NotifyLoginError(NULL);
	}

	((CzPlatformFacebook*)PLATFORM_FACEBOOK)->setWaitingForCallback(false);
}

void CzPlatformFacebook::WaitForCallback()
{
	WaitingForCallback = true;
	int loops = 100;	// 10 second timeout
	while (WaitingForCallback && !PLATFORM_SYS->CheckAppQuit() && loops > 0)
	{
		PLATFORM_SYS->YieldToOS(100);
//		if (SessionStatus != FB_LOGGING_IN)
//			loops--;
	}
	if (WaitingForCallback && loops <= 0)
	{
		WaitingForCallback = false;
		if (SessionStatus == FB_LOGGING_IN)
			NotifyLoginError(NULL);
		else
			NotifyResponseError(NULL);
	}
}

void CzPlatformFacebook::DeleteRequest(CzFacebookRequest request)
{
	if (request != NULL)
		s3eFBRequest_Delete(static_cast<s3eFBRequest*>(request));
}

bool CzPlatformFacebook::Login(const char** permissions, int num_permissions)
{
	if (!Available)
		return false;

	if (!CurrentSession)
		CurrentSession = s3eFBInit(AppID.c_str());

	if (CurrentSession)
	{
		// Set default log in behaviour
		s3eFBSessionLoginBehaviour login_behaviour = s3eFBSessionBehaviourWithFallbackToWebView;

		// Log in to Facebook using the session.
//		const char* permissions[] = { "read_stream", "publish_stream" };
		if (s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_IPHONE && ((s3eDeviceGetInt(S3E_DEVICE_OS_VERSION) >> 16) & 0xffff) >= 6)
			login_behaviour = s3eFBSessionBehaviourUseSystemAccountIfPresent;

		s3eFBSession_Login(CurrentSession, FBLoginCallback, NULL, permissions, num_permissions, login_behaviour);

		SessionStatus = FB_LOGGING_IN;

//		WaitForCallback();

//		s3eBool loggedIn = s3eFBSession_LoggedIn(CurrentSession);
	}
	else
	{
		PLATFORM_FACEBOOK->NotifyLoginError(NULL);
		SessionStatus = FB_UNINITIALISED;
		return false;
	}

	return true;
}

bool CzPlatformFacebook::Logout()
{
	s3eFBSession_Logout(CurrentSession);
	SessionStatus = FB_LOGGED_OUT;
	Release();

	return true;
}

static void FBReauthorizePermissionsCallback(struct s3eFBSession* session, s3eResult* loginResult, void* userData)
{
//    IwTrace(FACEBOOK, ("FBLoginCallback %p %p %p", session, loginResult, userData));
	
	((CzPlatformFacebook*)PLATFORM_FACEBOOK)->setSessionStatus(CzPlatformFacebook::FB_LOGGED_IN);
	if (*loginResult == S3E_RESULT_SUCCESS)
	{
		PLATFORM_FACEBOOK->NotifyLoggedIn(NULL);
	}
	else
	{
		PLATFORM_FACEBOOK->NotifyLoginError(NULL);
	}

	((CzPlatformFacebook*)PLATFORM_FACEBOOK)->setWaitingForCallback(false);
}

bool CzPlatformFacebook::Reauthorise(const char** permissions, int num_permissions)
{
//printf("**** CzPlatformFacebook::Reauthorise\n");
	return s3eFBSession_ReauthorizeWithPublishPermissions(CurrentSession, permissions, num_permissions, s3eFBSessionDefaultAudienceEveryone, FBReauthorizePermissionsCallback) == S3E_TRUE;
}

static void FBRequestCallback(s3eFBRequest* request, s3eResult* requestResult, void* userData)
{
	if (*requestResult == S3E_RESULT_SUCCESS)
	{
		// Request successful
		PLATFORM_FACEBOOK->NotifyResponse((CzFacebookRequest)request);
	}
	else
	{
		// Request failed
		PLATFORM_FACEBOOK->NotifyResponseError((CzFacebookRequest)request);
	}

	// Cleanup the request
	((CzPlatformFacebook*)PLATFORM_FACEBOOK)->DeleteRequest((CzFacebookRequest)request);
}

CzFacebookRequest CzPlatformFacebook::InitRequest(const char* method_name, const char* http_method)
{
	s3eFBRequest* request = s3eFBRequest_WithMethodName(CurrentSession, method_name, http_method);
	if (request == NULL)
	{
		PLATFORM_FACEBOOK->NotifyResponseError((CzFacebookRequest)request);
		return NULL;
	}

	return (CzFacebookRequest)request;
}

CzFacebookRequest CzPlatformFacebook::InitGraphRequest(const char* graph_path, const char* http_method)
{
	s3eFBRequest* request = s3eFBRequest_WithGraphPath(CurrentSession, graph_path, http_method);
	if (request == NULL)
	{
		PLATFORM_FACEBOOK->NotifyResponseError((CzFacebookRequest)request);
		return NULL;
	}

	return (CzFacebookRequest)request;
}


bool CzPlatformFacebook::AddParamString(CzFacebookRequest request, const char* parameter, const char* value)
{
	if (request == NULL || s3eFBRequest_AddParamString(static_cast<s3eFBRequest*>(request), parameter, value) != S3E_RESULT_SUCCESS)
	{
		PLATFORM_FACEBOOK->NotifyResponseError((CzFacebookRequest)request);
		DeleteRequest(request);
		return false;
	}

	return true;
}

bool CzPlatformFacebook::AddParamNumber(CzFacebookRequest request, const char* parameter, int64 value)
{
	if (request == NULL || s3eFBRequest_AddParamNumber(static_cast<s3eFBRequest*>(request), parameter, value) != S3E_RESULT_SUCCESS)
	{
		PLATFORM_FACEBOOK->NotifyResponseError((CzFacebookRequest)request);
		DeleteRequest(request);
		return false;
	}

	return true;
}

bool CzPlatformFacebook::SendRequest(CzFacebookRequest request)
{
	if (request == NULL || s3eFBRequest_Send(static_cast<s3eFBRequest*>(request), FBRequestCallback, NULL) != S3E_RESULT_SUCCESS)
	{
		PLATFORM_FACEBOOK->NotifyResponseError((CzFacebookRequest)request);
		DeleteRequest(request);
		return false;
	}

	return true;
}

const char* CzPlatformFacebook::GetResponse(CzFacebookRequest request)
{
	return s3eFBRequest_ResponseRaw(static_cast<s3eFBRequest*>(request));
}

int CzPlatformFacebook::GetErrorCode(CzFacebookRequest request)
{
	return s3eFBRequest_ErrorCode(static_cast<s3eFBRequest*>(request));
}

const char* CzPlatformFacebook::GetError(CzFacebookRequest request)
{
	return s3eFBRequest_ErrorString(static_cast<s3eFBRequest*>(request));
}

const char* CzPlatformFacebook::GetAccessToken()
{
	return s3eFBSession_AccessToken(CurrentSession);
}



/*bool CzPlatformFacebook::PostWall(const char* message, const char* link_uri, const char* image_uri, const char* name, const char* description)
{
	if (!Available)
		return false;

	if (!isLoggedIn())
	{
		Login();
	}

	Request = s3eFBRequest_WithMethodName(CurrentSession, "facebook.stream.publish", "POST");

	if (!Request)
	{
		return false;
	}
	else
	{
		// Build JSON attachment
		CzString attachment;
		attachment.setAutoHash(false);
		attachment.allocString(1024);

		attachment += "{ ";
		attachment += "\"name\": \"";
		attachment += (char*)name;
		attachment += "\", ";
		attachment += "\"href\": \"";
		attachment += (char*)link_uri;
		attachment += "\", ";
		attachment += "\"description\": \"";
		attachment += (char*)description;
		attachment += "\", ";
		attachment += "\"media\": [ { \"type\": \"image\", ";
		attachment += "\"src\": \"";
		attachment += (char*)image_uri;
		attachment += "\", ";
		attachment += "\"href\": \"";
		attachment += (char*)link_uri;
		attachment += "\", ";
		attachment += "}] }";


		// Add a value to the request and send
		s3eFBRequest_AddParamString(Request, "message", message);
		s3eFBRequest_AddParamString(Request, "attachment", attachment.c_str());
				
		if (s3eFBRequest_Send(Request, FBRequestCallback, NULL) != S3E_RESULT_SUCCESS)
		{
			s3eFBRequest_Delete(Request);
			return false;
		}
		else
		{
		}
	}
	
	return true;
}*/


