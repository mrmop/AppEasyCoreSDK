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

#include "CzPlatformFacebook.h"

int CzPlatformFacebook::Init()
{
	Available = false;
	SessionStatus = FB_UNINITIALISED;
	CurrentSession = 0;
	Request = 0;

	Initialised = true;

	return 0;
}

void CzPlatformFacebook::Release()
{
	Initialised = false;
}


bool CzPlatformFacebook::Login()
{
	if (!Available)
		return false;
	
	return false;
}

bool CzPlatformFacebook::Logout()
{
	return true;
}

bool CzPlatformFacebook::PostWall(const char* message, const char* link_uri, const char* image_uri, const char* name, const char* description)
{
	if (!Available)
		return false;

	if (!isLoggedIn())
	{
		Login();
	}

	return false;
}


