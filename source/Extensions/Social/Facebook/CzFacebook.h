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

#if !defined(_CZ_FACEBOOK_H_)
#define _CZ_FACEBOOK_H_

#include "IzPlatformFacebook.h"
#include "CzUtil.h"
#include "CzActions.h"
#include "CzEvents.h"
#include "CzScript.h"

/**
 @class	CzFacebook

 @brief	A class that is used to interact with Facebook

 */

class CzFacebook : public IzXomlResource
{
public:
	// Properties
protected:
	CzEventManager*		EventsManager;			///< List of events that the camera handles
	CzScriptCallback	ScriptCallback;			///< Script call back
public:
	CzEventManager*		getEventsManager()				{ return EventsManager; }
	CzScriptCallback&	getScriptCallback()				{ return ScriptCallback; }
	// Properties end
protected:
	virtual void		ProcessEventActions(unsigned int event_name, IzXomlResource* parent);
public:
	CzFacebook() : IzXomlResource()
	{
		setClassType("facebook");
		EventsManager = new CzEventManager();
	}
	virtual ~CzFacebook()
	{
		SAFE_DELETE(EventsManager);
	}

	bool				IsLoggedn() const;
	bool				Login(const char* permissions[]);
	bool				Logout();
	bool				InitRequest(const char* method_name, const char* http_method);
	bool				AddParamString(const char* parameter, const char* value);
	bool				AddParamNumber(const char* parameter, int64 value);
	bool				SendRequest();

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Event handlers
	void				NotifyLoggedIn();
	void				NotifyLoginError();
	void				NotifyResponse();
	void				NotifyResponseError();

};

/**
 @class	CzFacebookCreator

 @brief	Creates an instance of a Facebook class

 */

class CzFacebookCreator : public IzXomlClassCreator
{
public:
	CzFacebookCreator()
	{
		setClassName("facebook");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzFacebook(); }
};






#endif // _CZ_FACEBOOK_H_