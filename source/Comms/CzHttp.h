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

#if !defined(_CZ_HTTP_H_)
#define _CZ_HTTP_H_

#include "CzUtil.h"
#include "CzString.h"

/**
 @addtogroup Comms
 @{
 */

/**
 @def	CZ_HTTP_MANAGER()

 @brief	A short cut macro that calls the global HTTP manager singleton

 */
 
#define CZ_HTTP_MANAGER		CzHttpManager::getInstance()

class CzHttpManager;

typedef void* CzHttpObject;

/**
 @class	CzHttpHeader

 @brief	An HTTP header.

 */
class CzHttpHeader
{
	// Properties
private:
	CzString		Header;			///< Header name
	CzString		HeaderData;		///< header data
public:
	void			setHeader(const char* header)
	{
		Header.setString(header);
	}
	CzString&		getHeader()		{ return Header; }
	void			setHeaderData(const char* header_data)
	{
		HeaderData.setString(header_data);
	}
	const char*		getHeaderData() const { return HeaderData.c_str(); }
	// Properties end
public:
	CzHttpHeader()
	{
		Header.setAutoHash(true);
	}
	virtual ~CzHttpHeader()
	{
	}
};
typedef CzList<CzHttpHeader*>::iterator CzHttpHeaderIterator;

/**
 @class	CzHttpPostData

 @brief	HTTP POST data.

 */
class CzHttpPostData
{
private:
	CzList<CzHttpHeader*>	Headers;		///< Headers collection
	CzString				Body;			///< POST body
public:
	CzHttpPostData() {}
	virtual ~CzHttpPostData()
	{
		ClearHeaders();
	}

	void SetHeader(const char* header, const char* header_data)
	{
		unsigned int header_hash = CzString::CalculateHash(header);

		// Check to see if header already present, if so update the header info
		for (CzHttpHeaderIterator it = Headers.begin(); it != Headers.end(); it++)
		{
			if (header_hash == (*it)->getHeader().getHash())
			{
				(*it)->setHeaderData(header_data);
				return;
			}
		}

		// Header was not already present so add new header
		CzHttpHeader* head = new CzHttpHeader();
		head->setHeader(header);
		head->setHeaderData(header_data);
		Headers.push_back(head);
	}
	void ClearHeaders()
	{
		for (CzHttpHeaderIterator it = Headers.begin(); it != Headers.end();  ++it)
			delete (*it);
		Headers.clear();
	}
	void setBody(const char* body)
	{
		Body.setString(body);
	}
	const CzString& getBody() const { return Body; }

	void			ApplyHeaders();
};

/**
 @class	CzHttpRequest

 @brief	An HTTP request including the header, body and response.

 */
class CzHttpRequest
{
	// Properties
private:
	CzString			URI;							///< URI end point
	CzString			Content;						///< Receieved content
	int					ContentLength;					///< Received content lenngth
	bool				Processed;						///< Processed marker (becomes true when Http Manager has processed the request, this does not mean result is available)
	int					Error;							///< Any errors that occured or 0 for none
	CzHttpPostData*		PostData;						///< Collection of headers and the main body to POST
	CzCallback			ContentAvailableCallback;		///< Callback to be called when content is available
	void*				ContentAvailableCallbackData;	///< Data to be passed to the callback
	bool				Post;							///< True if request is a POST, otherwise a GET
	bool				AutoDelete;						///< Request is auto deleted if true
public:
	const CzString&		getURI() const							{ return URI; }
	void				setURI(const char* uri)
	{
		URI.setString(uri);
	}
	CzString&			getContent()							{ return Content; }
	void				setContent(char* content, int len)
	{
		Content.allocString(len);
		Content.Copy(content, 0, len);
		ContentLength = len;
	}
	void				allocContent(int len)
	{
		Content.allocString(len);
		Content.setLength(len);
		ContentLength = len;
	}
	void				reallocContent(int new_len)
	{
		Content.reallocString(new_len);
		Content.setLength(new_len);
		ContentLength = new_len;
	}
	int					getContentLength() const							{ return ContentLength; }
	bool				getProcessed() const								{ return Processed; }
	void				setProcessed(bool processed)						{ Processed = processed; }
	void				SetHeader(const char* header, const char* header_data) { PostData->SetHeader(header, header_data); }
	void				ClearHeaders()										{ PostData->ClearHeaders(); }
	void				setBody(const char* body)							{ PostData->setBody(body); }
	const CzString&		getBody() const										{ return PostData->getBody(); }
	void				setContentAvailableCallback(CzCallback callback, void *data)	{ ContentAvailableCallback = callback; ContentAvailableCallbackData = data; }
	void				setPOST()											{ Post = true; }
	void				setGET()											{ Post = false; }
	bool				isPOST() const										{ return Post; }
	// Properties end

private:

public:
	CzHttpRequest() : ContentLength(0), Processed(false), Error(0), ContentAvailableCallback(NULL), ContentAvailableCallbackData(NULL), Post(false), AutoDelete(false) { PostData = new CzHttpPostData(); }
	virtual ~CzHttpRequest()
	{
		SAFE_DELETE(PostData)
	}

	void	setError(int error)					{ Error = error; }
	int		getError() const					{ return Error; }	

	void	EndRequest(int error);
	void	ApplyHeaders();

};

/**
 @class	CzHttpManager

 @brief	Handles the queueing of HTTP requests.

 */
class CzHttpManager
{
	CDEFINE_SINGLETON(CzHttpManager)

protected:
	typedef CzList<CzHttpRequest*>::iterator _Iterator;
	CzList<CzHttpRequest*>	Requests;		///< Request queue (caller owns requests)

	// Properties
	CzHttpRequest*			CurrentRequest;	///< Current request thats being processed or null if not busy
	CzHttpObject			HttpObject;		///< The Marmalade SDK Http Object
	CzString				UserAgent;		///< Browser style user-agent
	CzString				IPAddress;		///< IP address of device
public:
	CzHttpRequest*			getCurrentRequest()						{ return CurrentRequest; }
	void					setNoRequest()							{ CurrentRequest = NULL; }
	bool					GetHeader(const char* header_name, CzString& header_data);
	CzHttpObject			getHttpObject()							{ return HttpObject; }
	void					setUserAgent(const char* user_agent)	{ UserAgent = user_agent; }	// Aoto determined but can be changed if needed
	CzString&				getUserAgent()							{ return UserAgent; }
	void					setIPAddress(const char* ip_address)	{ IPAddress = ip_address; }
	CzString&				getIPAddress()							{ return IPAddress; }
	// Properties end

protected:
	CzString				DetermineUserAgent();
//	bool					DetermineIPAddress();
public:
	void					Init();
	void					Release();
	void					AddRequest(CzHttpRequest* request);
	void					RemoveRequest(CzHttpRequest* request);
	void					ClearRequests();
	void					CancelRequests();
	
	void					Update();
	bool					isEmpty() const;

	// Internal
};

/// @}


#endif // _CZ_HTTP_H_