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

#include "IzPlatformFacebook.h"
#include "CzString.h"

class CzPlatformFacebook: public IzPlatformFacebook
{
	// Properties
protected:
	bool			Available;
	CzString		AppID;
public:
	void			setAppID(const char* app_id)				{ AppID = app_id; }
	bool			isAvailable() const							{ return Available; }
	bool			isLoggedIn() const							{ return SessionStatus == FB_LOGGED_IN;; }
	// Properties end

public:
	CzPlatformFacebook() {}
	int				Init();
	void			Release();

	bool			Login();
	bool			Logout();
	bool			PostWall(const char* message, const char* link_uri, const char* image_uri, const char* name, const char* description);

	// Internal
protected:
public:
};


#endif	// _CCZ_PLATFORM_FACEBOOK_H_
