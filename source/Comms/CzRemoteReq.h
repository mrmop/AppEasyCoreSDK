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

#if !defined(_CZ_REMOTEREQ_H_)
#define _CZ_REMOTEREQ_H_


#include "CzUtil.h"
#include "CzXoml.h"
#include "CzXomlVariables.h"
#include "CzHttp.h"
#include "CzActions.h"
#include "CzEvents.h"

/**
 @addtogroup Comms
 @{
 */
 
/**
 @class	CzRemoteReq

 @brief	CzRemmoteReq - A remote request.

 XOML provides many ways of communicating with external data that is located on a web server / web site. Files, images, audio and fonts can for example reside 
 outside the app on some web site and can be loaded on demand by the XOML app. This is a very easy to use automated system that provides very powerful control 
 over what resources you ship with your apps and which you can host on a server and change at any time. 

 However, its often useful to be able to communicate with a web service or web service, such as send information about the user or the app. For example, your 
 app may store its users data on the server in a database. XOML offers the ability to use both HTTP POST and GET to send and retrieve data to and from a web server. 

 Heres a short example:

 @par XOML Example:
 @code
<RemoteReq Name="Request1" URL="http://www.mywebservice.com" Data="name=Mat" OnResponse="GotData" OnError="Error" Variable="ReqVar1">
	<Header Name="Content-Type" Value="application/x-www-form-urlencoded" />
</RemoteReq>
 @endcode

 This example creates a remote request definition that calls mywebservice.com and passes the data “name=Mat”. The inner header tag sets the Content-Type header. 
 The server will later send a response which is written into the variable ReqVar1 and calls the GotData actions list allowing you to perform actions when the 
 data I received. If an error occurs then Error actions list will be called instead. 

 Note that a RemoteReq is not automatically sent when it is created. You need to actually call the request from an action or program command, e.g.:

 @code
<Actions Name="GetData">
    <Action Method="RemoteReq" P1="Request1" />
</Actions>

<Program Name="Program1" AutoRun="true">
    <Command Method="remote_req" P1="Request1" />
</Program>
 @endcode

 A request will remain in memory for as long as the RemoteReq resource is available. A RemoteReq can be re-used.  

 */

class CzRemoteReq : public IzXomlResource
{
public:
	// Properties
protected:
	CzEventManager*		EventsManager;			// List of events
	CzXomlVariable*		Variable;				// Variable that is will receieve the response data
public:
	// Properties end
	void				setUrl(const char* url)				{ Request.setURI(url); }
	const CzString&		getUrl() const						{ return Request.getURI(); }
	CzEventManager*		getEventsManager()					{ return EventsManager; }
	void				setPost(bool post)					{ if (post) Request.setPOST(); else Request.setGET(); }
	bool				isPost() const						{ return Request.isPOST(); }
	void				setVariable(const char* variable_name);
	void				setVariable(CzXomlVariable* var)	{ Variable = var; }
	CzXomlVariable*		getVariable()						{ return Variable; }
	void				setData(const char* data)			{ Request.setBody(data); }
	const CzString&		getData() const						{ return Request.getBody(); }
protected:
	CzHttpRequest		Request;
	void				ProcessEventActions(unsigned int event_name);


public:
	CzRemoteReq() : IzXomlResource(), EventsManager(NULL), Variable(NULL)
	{
		setClassType("remotereq");
		EventsManager = new CzEventManager();
	}
	virtual ~CzRemoteReq();

	virtual bool		setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool				setProperty(const char* property_name, const CzString& data, bool delta);
	virtual bool		setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool				getProperty(const char* property_name, CzXomlProperty& prop);
	virtual bool		getProperty(unsigned int property_name, CzXomlProperty& prop);

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parebt, bool load_children, CzXmlNode* node);

	void				MakeRequest();
	void				DataReceived();

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	RemoteRequestClassDef;								// XOML class definition
public:
	static void				InitClass();
	static void				ReleaseClass();
	static bool				_setName(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getName(IzXomlResource* target);
	static bool				_setTag(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setURL(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getURL(IzXomlResource* target);
	static bool				_setData(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getData(IzXomlResource* target);
	static bool				_setOnResponse(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnError(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setPost(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getPost(IzXomlResource* target);
	static bool				_setVariable(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getVariable(IzXomlResource* target);
};

/**
 @class	CzRemoteReqCreator

 @brief	CzRemoteReqCreator - Creates an instance of a RemoteReq.

 */

class CzRemoteReqCreator : public IzXomlClassCreator
{
public:
	CzRemoteReqCreator()
	{
		setClassName("remotereq");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzRemoteReq(); }
};

/// @}


#endif	// _CZ_REMOTEREQ_H_
