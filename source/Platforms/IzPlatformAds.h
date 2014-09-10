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

#if !defined(_CIZ_PLATFORM_ADS_H_)
#define _CIZ_PLATFORM_ADS_H_

#include "CzPlatform.h"
#include "CzScript.h"

enum eCzAdProvider
{
#if defined(ADS_CHARTBOOST_ENABLE)
	Ads_Chartboost, 
#endif // ADS_FLURRY_APPSPOT_ENABLE
#if defined(ADS_FLURRY_APPSPOT_ENABLE)
	Ads_FlurryAppSpot, 
#endif // ADS_FLURRY_APPSPOT_ENABLE
#if defined(ADS_REVMOB_ENABLE)
	Ads_Revmob, 
#endif // ADS_REVMOB_ENABLE
};

enum eCzAdType
{
	AdType_Banner,			// Revmob, Flurry
	AdType_Interstitial,	// Revmob, Flurry, Chartboost
	AdType_Popup,			// Revmob
	AdType_MoreApps,		// Chartboost
	AdType_Max, 
};

#define	PLATFORM_ADS		PLATFORM->getAds()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IzPlatformAds
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IzPlatformAds
{
	// Properties
protected:
	eCzAdProvider			Providers[AdType_Max];
	bool					Initialised;				///< Platform system initialised state
	CzScriptCallback		ScriptCallback;				///< Script call back
public:
	bool					isInitialised() const					{ return Initialised; }
	void					setProvider(eCzAdType type, eCzAdProvider provider) { Providers[(int)type] = provider; }
	eCzAdProvider			getProvider(eCzAdType type) const		{ return Providers[(int)type]; }
	CzScriptCallback&		getScriptCallback()						{ return ScriptCallback; }
	// Properties end
public:
	virtual ~IzPlatformAds() {}
	virtual bool			isAvailable(eCzAdProvider provider) = 0;
	virtual int				Init(eCzAdProvider provider, const char* id, const char* data1, int data2) = 0;
	virtual void			Release() = 0;
	virtual void			NotifyAdEvent(const char* evt, const char* data1, int data2) = 0;

	virtual void			Load(eCzAdType type, const char* data1, int data2) = 0;
	virtual void			LoadShow(eCzAdType type, const char* data1, int data2) = 0;
	virtual void			Show(eCzAdType type) = 0;
	virtual void			Hide(eCzAdType type) = 0;

};

#endif	// _CIZ_PLATFORM_ADS_H_
