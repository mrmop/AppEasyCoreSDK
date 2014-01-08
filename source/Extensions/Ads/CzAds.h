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

//
// Working ad providers
// --------------------
// Inner-active
// AdFonic
// VServ - Allows you to set up other networks including InMobi, BuzzCity, JumpTap, Zestadz and Inner-active
// Mojiva
// MillennialMedia - Allows you to set up other networks including AdMob, Amobee, JumpTap and Mojiva
// AdModa
//
// Ad providers that will never work
// ---------------------------------
// AdMob - Closed their REST API so will never be available
// MoPub - API is closed so will never be available
//
// Ad Providers with issues
// ------------------------
// MobClix - Cant find a REST API
// MobFox - Ads wont deliver over WiFi or Carrier and clicks are not registered
// InMobi - Cannot get a single ad, either get bad request error or no ad error
// Madvertise - Registering impressions but never returns any ads
// KomliMobile - Always receieve "your account  has been banned" message so gave up
//
// Ad Providers that need implementation
// -------------------------------------
// BuzzCity - 


#if !defined(_CZ_ADS_H_)
#define _CZ_ADS_H_

#include "CzUtil.h"
#include "CzHttp.h"
#include "CzImage.h"
#include "CzTime.h"

//#define _AD_DO_NOT_USE_

#define	CZ_ADS				(CzAds::getInstance())

#define	CZ_ADS_TIMEOUT		10000
#define	CZ_MAX_CACHED_ADS	4
#define	CZ_MAX_AD_AGE		(5 * 60000)

class CzAdsMediator;

//
//
// CzAd - The CzAd structure contains all data relating to the ad returned from the ad server
//
//
struct CzAd
{
	CzAd(): Image(NULL), isHtml(false), AdTime(0) {}
	virtual ~CzAd()
	{
		SAFE_DELETE(Image)
	}

	bool			isHtml;			// If isHtml is set to true then only Text is valid and that cnotains the Ad HTML
	bool			isText;			// If true then the ad is a text ad
	CzString		Text;			// Ad text
	CzString		ImageURI;		// URI of image
	CzString		LinkURI;		// URI to be called when user clicks the ad
	CzImage*		Image;			// 2D image that represents the banner Ad
	int				ImageFormat;	// 2D Image format
	int64			AdTime;			// Time ad was collected

	CzAd*			getCopy();
};

//
//
// CzAds - The CzAds class is responsible for streaming ad data
//
//
class CzAds
{
public:
	enum eAdProvider
	{
		InnerActive,
		AdFonic,
		VServ, 
		Mojiva,
		MillennialMedia, 
		AdModa, 
#if defined(_AD_DO_NOT_USE_)
		InMobi,
		MobClix,
		MobFox,
		Madvertise,
		KomliMobile, 
		BuzzCity, 
#endif	// _AD_DO_NOT_USE_
	};

	enum eAdsPortalType
	{
		PortalNone, 
		AndroidBanner, 
		AndroidText, 
		BadaBanner, 
		BadaText, 
		BlackberryBanner, 
		BlackberryText, 
		iPadBanner, 
		iPadText, 
		iPhoneBanner, 
		iPhoneText, 
		OVIBanner, 
		OVIText, 
		WebOSBanner, 
		WebOSText, 
		WinMobileBanner, 
		WinMobileText, 
		MobileWebBanner, 
		MobileWebText, 
	};
	enum eAdsGender
	{
		GenderInvalid, 
		GenderMale, 
		GenderFemale, 
	};
	enum eAdsError
	{
		ErrorNone, 
		ErrorHouseAd, 
		ErrorInternalError, 
		ErrorInvalidInput, 
		ErrorUnknownAppId, 
		ErrorNoAd, 
		ErrorHttp, 
		ErrorHttpImage, 
		ErrorInvalidImage, 
		ErrorRequestTimedOut, 
		ErrorInvalidAdData, 
	};
	enum eAdsImageFormat
	{
		ImageFormatInvalid, 
		ImageFormatPNG, 
		ImageFormatGIF, 
		ImageFormatJPG, 
	};

	CDEFINE_SINGLETON(CzAds)

	// Properties
protected:
	CzString			Version;					// Protocol version string (for inner-active)
	CzString			ApplicationID;				// ID of the application thats making the request (you will need to be assigned this from your ad provider)
	CzString			OtherID;					// Extra ID information
	unsigned int		UDID;						// Unique ID of the device (used to identify device type, language and approx location to deliver more appropriate ads)
	int					Width, Height;				// Dimensions of device (used to collect more appropriate sized ads)
	eAdsPortalType		PortalType;					// Type of portal to collect ads from
	bool				TextAds;					// True if you want text ads to be returned, false if banner ads
	bool				HtmlAds;					// True if you want ads to be returned as html instead of xml (used by inner-active)
	int					UserAge;					// The users ages (optional)
	CzString			UserAgent;					// User agent string (system will build this but it can be replaced)
	CzString			IPAddress;					// IP Address of users device
	eAdsGender			UserGender;					// The users gender (optional)
	CzString			UserLocation;				// Location string – comma separated list of country, state/province, city (optional)
	CzString			UserGPSLocation;			// GPS Location string – ISO code location data in latitude, longitude format (optional)
	CzString			Category;					// Single word description of the application (optional)
	CzString			UserMobileNumber;			// Users mobile number - MSISDN format, with international prefix (optional)
	CzString			UserKeywords;				// Comma separated list of keywords relevant to this user’s specific session (optional)
	CzString			ExtraInfo;					// Pass in any extra pareneters as name vakue pairs, e.g. &city=london&ad_unit=1 (optional)
	bool				AdAvailable;				// True when ad is available
	CzCallback			AdAvailableCallback;		// Callback to be called when Ad is available
	eAdsError			Error;						// Comntains error code if any if ad not received
	CzString			ErrorString;				// Human readable error string
	CzAd				AdInfo;						// The returned ad info
	CzAdsMediator*		Mediator;					// Ad mediator (optional)

public:
	void				setVersion(const char* version)		{ Version = version; }
	CzString&			getVersion()						{ return Version; }
	void				setApplicationID(const char* id)	{ ApplicationID = id; }
	CzString&			getApplicationID()					{ return ApplicationID; }
	void				setOtherID(const char* id)			{ OtherID = id; }
	CzString&			getOtherID()						{ return OtherID; }
	void				setUDID(unsigned int udid)			{ UDID = udid; }
	unsigned int		getUDID() const						{ return UDID; }
	void				setSize(int width, int height)		{ Width = width; Height = height; }
	int					getWidth() const					{ return Width; }
	int					getHeight() const					{ return Height; }
	void				setPortalType(eAdsPortalType type) { PortalType = type; }
	eAdsPortalType		getPortaltype() const				{ return PortalType; }
	void				setTextAds(bool text_ads)			{ if (TextAds != text_ads) { TextAds = text_ads; PortalType = FindPortalType(TextAds); } }
	bool				getTextAds() const					{ return TextAds; }
	void				setHtmlAds(bool html_ads)			{ HtmlAds = html_ads; }
	bool				getHtmlAds() const					{ return HtmlAds; }
	void				setUserAgent(const char* user_agent) { UserAgent.setString(user_agent); }
	CzString&			getUserAgent() 						{ return UserAgent; }
	void				setUserAge(int age)					{ UserAge = age; }
	int					getUserAge() const					{ return UserAge; }
	void				setUserGender(eAdsGender gender)	{ UserGender = gender; }
	eAdsGender			getUserGender() const				{ return UserGender; }
	void				setUserLocation(const char* location) { UserLocation = location; }
	CzString&			getUserLocation()					{ return UserLocation; }
	void				setUserGPSLocation(const char* location) { UserGPSLocation = location; }
	CzString&			getUserGPSLocation()				{ return UserGPSLocation; }
	void				setMobileNumber(const char* number) { UserMobileNumber = number; }
	CzString&			getUserMobileNumber()				{ return UserMobileNumber; }
	void				setUserKeywords(const char* keywords) { UserKeywords = keywords; }
	CzString&			getUserKeywords()					{ return UserKeywords; }
	void				setCategory(const char* category)	{ Category = category; }
	CzString&			getCategory()						{ return Category; }
	void				setExtraInfo(const char* extras)	{ ExtraInfo = extras; }
	CzString&			getExtraInfo()						{ return ExtraInfo; }
	void				setAdAvailable(bool available)		{ AdAvailable = available; }
	bool				isAdAvailable() const				{ return AdAvailable; }
	void				setAdAvailableCallback(CzCallback callback)	{ AdAvailableCallback = callback; }
	void				setError(eAdsError error)		{ Error = error; }
	eAdsError			getError() const					{ return Error; }
	void				setErrorString(const char* error)	{ ErrorString.setString(error); }
	CzString&			getErrorString()					{ return ErrorString; }
	CzAd&				getAd()								{ return AdInfo; }
	int					getMaxAds() const					{ return CZ_MAX_CACHED_ADS; }
	void				setMediator(CzAdsMediator* med) { Mediator = med; }
	CzAdsMediator*		getMediator()						{ return Mediator; }

	// Properties end

protected:
	static unsigned int		ResponseCodes[];
	static int				PortalIDs[];

	CzString				ContentType;
	CzString				UserIP;
	CzString				ClientID;
	CzString				RequestURI;
	CzHttpRequest			AdRequest;
	CzTimer					BusyTimer;
	eAdProvider				AdProvider;

	bool					ExtractAd(CzAd& ad, CzString& ad_body);
	void					ErrorFromResponse(const char* error, int error_len);
	bool					RequestBannerImage(CzAd& ad);
	void					NotifyAdAvailable();
	eAdsImageFormat			GetImageFormatFromHeader();
	bool					RequestAdInnerActive();
	bool					RequestAdInMobi();
	bool					RequestAdMobClix();
	bool					RequestAdMobFox();
	bool					RequestAdAdFonic();
	bool					RequestAdMadvertise();
	bool					RequestAdMojiva();
	bool					RequestAdMillennialMedia();
	bool					RequestAdVServ();
	bool					RequestAdKomliMobile();
	bool					RequestAdAdModa();
	bool					ExtractLinkAndImageFromtHTML(CzAd& ad, CzString& html);
	bool					ExtractAdInnerActive(CzAd& ad, CzString& ad_body);
	bool					ExtractAdInMobi(CzAd& ad, CzString& ad_body);
	bool					ExtractAdMobClix(CzAd& ad, CzString& ad_body);
	bool					ExtractAdMobFox(CzAd& ad, CzString& ad_body);
	bool					ExtractAdAdFonic(CzAd& ad, CzString& ad_body);
	bool					ExtractAdMadvertise(CzAd& ad, CzString& ad_body);
	bool					ExtractAdMojiva(CzAd& ad, CzString& ad_body);
	bool					ExtractAdMillennialMedia(CzAd& ad, CzString& ad_body);
	bool					ExtractAdVServ(CzAd& ad, CzString& ad_body);
	bool					ExtractAdKomliMobile(CzAd& ad, CzString& ad_body);
	bool					ExtractAdAdModa(CzAd& ad, CzString& ad_body);


public:
	bool					Init();												// Initialises the Ads system (returns true if ads are supported)
	void					Release();											// Releases data used by the Ads system
	void					Update();											// Update ads
	bool					RequestAd(eAdProvider provider, bool reset_mediator = true);	// Requests an ad from the specified ad provider
	
	// Utility
	static eAdsPortalType	FindPortalType(bool text_ad = false);				// Auto find portal type from OS type and ad type
	static int				getPortalID(eAdsPortalType type);					// Return portal iD

	// Internal
	void					AdImageReceived(CzHttpRequest* request, int error);	// Called by the http callback internally when an ad image is received
	void					AdReceived(CzHttpRequest* request, int error);		// Called by the http callback internally when an ad is received

};


#endif	// _CZ_ADS_H_