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

#include "s3e.h"
#include "CzPlatformAds.h"
#include "IzPlatformDisplay.h"

#if defined(CZ_ADS_CHARTBOOST_ENABLE)
	#include "s3eChartBoost.h"
#endif // CZ_ADS_CHARTBOOST_ENABLE

#if defined(CZ_ADS_FLURRY_APPSPOT_ENABLE)
	#include "s3eFlurryAppSpot.h"
#endif // CZ_ADS_FLURRY_APPSPOT_ENABLE

#if defined(CZ_ADS_REVMOB_ENABLE)
	#include "RevMobEDK.h"
#endif // CZ_ADS_REVMOB_ENABLE

bool CzPlatformAds::isAvailable(eCzAdProvider provider)
{
	switch (provider)
	{
#if defined(CZ_ADS_CHARTBOOST_ENABLE)
	case Ads_Chartboost:
		{
			if (s3eChartBoostAvailable() == S3E_TRUE)
				return true;
			break;
		}
#endif // CZ_ADS_CHARTBOOST_ENABLE
#if defined(CZ_ADS_FLURRY_APPSPOT_ENABLE)
	case Ads_FlurryAppSpot:
		{
			if (s3eFlurryAppSpotAvailable() == S3E_TRUE)
				return true;
			break;
		}
#endif // CZ_ADS_FLURRY_APPSPOT_ENABLE
#if defined(CZ_ADS_REVMOB_ENABLE)
	case Ads_Revmob:
		{
			if (RevMobEDKAvailable() == S3E_TRUE)
				return true;
			break;
		}
#endif // CZ_ADS_REVMOB_ENABLE
	}

	return false;
}

int CzPlatformAds::Init(eCzAdProvider provider, const char* id, const char* data1, int data2)
{
	if (g_CzShowTracing > 1)
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising Ads");

	switch (provider)
	{
#if defined(CZ_ADS_CHARTBOOST_ENABLE)
	case Ads_Chartboost:
		{
			s3eChartBoostSetAppID(id);
			s3eChartBoostSetAppSignature(data1);
			s3eChartBoostRegister(S3E_CHARTBOOST_CALLBACK_REQUEST_RESPONSE, AdReceived, NULL);
			s3eChartBoostRegister(S3E_CHARTBOOST_CALLBACK_AD_CLOSED, AdClosed, NULL);
			s3eChartBoostRegister(S3E_CHARTBOOST_CALLBACK_AD_DISMISSED, AdClosed, NULL);
			s3eChartBoostRegister(S3E_CHARTBOOST_CALLBACK_AD_CLICKED, AdClicked, NULL);
			s3eChartBoostRegister(S3E_CHARTBOOST_CALLBACK_ERROR, AdFailed, NULL);
			s3eChartBoostStartSession();
			for (int t = 0; t < AdType_Max; t++)
				Providers[t] = Ads_Chartboost;
			break;
		}
#endif // CZ_ADS_CHARTBOOST_ENABLE
#if defined(CZ_ADS_FLURRY_APPSPOT_ENABLE)
	case Ads_FlurryAppSpot:
		{
			s3eFlurryAppSpotInitialize(id);
			s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADCLOSED, AdClosed, NULL);
			s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADSPACE_RECEIVED, AdReceived, NULL);
			s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADSPACE_RECEIVED_FAIL, AdFailed, NULL);
			s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADCLICKED, AdClicked, NULL);
			s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADVIDEOCOMPLETED, AdVideoEnded, NULL);
			s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADRENDERFAIL, AdFailed, NULL);
			for (int t = 0; t < AdType_Max; t++)
				Providers[t] = Ads_FlurryAppSpot;
			break;
		}
#endif // CZ_ADS_FLURRY_APPSPOT_ENABLE
#if defined(CZ_ADS_REVMOB_ENABLE)
	case Ads_Revmob:
		{
			RevMobStartSession(id);
			for (int t = 0; t < AdType_Max; t++)
				Providers[t] = Ads_Revmob;
			break;
		}
#endif // CZ_ADS_REVMOB_ENABLE
	}

	Initialised = true;

	return 1;
}

void	CzPlatformAds::Release()
{
}

void	CzPlatformAds::Load(eCzAdType type, const char* data1, int data2)
{
	switch (Providers[type])
	{
#if defined(CZ_ADS_CHARTBOOST_ENABLE)
	case Ads_Chartboost:
		{
			switch (type)
			{
			case AdType_Interstitial:
				s3eChartBoostCacheInterstitial(NULL);
				InterstitialData1 = data1;
				InterstitialData2 = data2;
				break;
			case AdType_MoreApps:
				s3eChartBoostCacheMoreApps();
				InterstitialData1 = data1;
				InterstitialData2 = data2;
				break;
			}
			break;
		}
#endif // CZ_ADS_CHARTBOOST_ENABLE
#if defined(CZ_ADS_FLURRY_APPSPOT_ENABLE)
	case Ads_FlurryAppSpot:
		{
			switch (type)
			{
			case AdType_Banner:
				s3eFlurryAppSpotFetchAdForSpace(data1, (data2 != 0) ? S3E_FLURRYAPPSPOT_BANNER_TOP : S3E_FLURRYAPPSPOT_BANNER_BOTTOM);
				BannerData1 = data1;
				BannerData2 = data2;
				break;
			case AdType_Interstitial:
				s3eFlurryAppSpotFetchAdForSpace(data1, S3E_FLURRYAPPSPOT_FULLSCREEN);
				InterstitialData1 = data1;
				InterstitialData2 = data2;
				break;
			}
			break;
		}
#endif // CZ_ADS_FLURRY_APPSPOT_ENABLE
#if defined(CZ_ADS_REVMOB_ENABLE)
	case Ads_Revmob:
		{
			// Revmob does not support callbacks so we notify the system that the ad is available immediately so it can be shown
			AdReceived((void*)"revmob", 0);
			break;
		}
#endif // CZ_ADS_REVMOB_ENABLE
	}
}

void	CzPlatformAds::LoadShow(eCzAdType type, const char* data1, int data2)
{
	switch (Providers[type])
	{
#if defined(CZ_ADS_CHARTBOOST_ENABLE)
	case Ads_Chartboost:
		{
			switch (type)
			{
			case AdType_Interstitial:
				s3eChartBoostShowInterstitial(NULL);
				InterstitialData1 = data1;
				InterstitialData2 = data2;
				break;
			case AdType_MoreApps:
				s3eChartBoostShowMoreApps();
				InterstitialData1 = data1;
				InterstitialData2 = data2;
				break;
			}
			break;
		}
#endif // CZ_ADS_CHARTBOOST_ENABLE
#if defined(CZ_ADS_FLURRY_APPSPOT_ENABLE)
	case Ads_FlurryAppSpot:
		{
			switch (type)
			{
			case AdType_Banner:
				s3eFlurryAppSpotSetDisplayAdView(0,50,320,50);
				s3eFlurryAppSpotFetchAndDisplayAdForSpace(data1, (data2 != 0) ? S3E_FLURRYAPPSPOT_BANNER_TOP : S3E_FLURRYAPPSPOT_BANNER_BOTTOM);
				BannerData1 = data1;
				BannerData2 = data2;
				break;
			case AdType_Interstitial:
				s3eFlurryAppSpotFetchAndDisplayAdForSpace(data1, S3E_FLURRYAPPSPOT_FULLSCREEN);
				InterstitialData1 = data1;
				InterstitialData2 = data2;
				break;
			}
			break;
		}
#endif // CZ_ADS_FLURRY_APPSPOT_ENABLE
#if defined(CZ_ADS_REVMOB_ENABLE)
	case Ads_Revmob:
		{
			switch (type)
			{
			case AdType_Banner:
				RevMobShowBanner();
				break;
			case AdType_Interstitial:
				RevMobShowFullscreen();
				break;
			case AdType_Popup:
				RevMobShowPopup();
				break;
			}
			break;
		}
#endif // CZ_ADS_REVMOB_ENABLE
	}
}

void	CzPlatformAds::Show(eCzAdType type)
{
	switch (Providers[type])
	{
#if defined(CZ_ADS_CHARTBOOST_ENABLE)
	case Ads_Chartboost:
		{
			switch (type)
			{
			case AdType_Interstitial:
				s3eChartBoostShowInterstitial(NULL);
				break;
			case AdType_MoreApps:
				s3eChartBoostShowMoreApps();
				break;
			}
			break;
		}
#endif // CZ_ADS_CHARTBOOST_ENABLE
#if defined(CZ_ADS_FLURRY_APPSPOT_ENABLE)
	case Ads_FlurryAppSpot:
		{
			switch (type)
			{
			case AdType_Banner:
				s3eFlurryAppSpotSetDisplayAdView(0,50,320,50);
				s3eFlurryAppSpotDisplayAdForSpace(BannerData1.c_str());
				break;
			case AdType_Interstitial:
				s3eFlurryAppSpotDisplayAdForSpace(InterstitialData1.c_str());
				break;
			}
			break;
		}
#endif // CZ_ADS_FLURRY_APPSPOT_ENABLE
#if defined(CZ_ADS_REVMOB_ENABLE)
	case Ads_Revmob:
		{
			switch (type)
			{
			case AdType_Banner:
				RevMobShowBanner();
				break;
			case AdType_Interstitial:
				RevMobShowFullscreen();
				break;
			case AdType_Popup:
				RevMobShowPopup();
				break;
			}
			break;
		}
#endif // CZ_ADS_REVMOB_ENABLE
	}
}

void	CzPlatformAds::Hide(eCzAdType type)
{
	switch (Providers[type])
	{
#if defined(CZ_ADS_CHARTBOOST_ENABLE)
	case Ads_Chartboost:
		break;
#endif // CZ_ADS_CHARTBOOST_ENABLE
#if defined(CZ_ADS_FLURRY_APPSPOT_ENABLE)
	case Ads_FlurryAppSpot:
		{
			switch (type)
			{
			case AdType_Banner:
				s3eFlurryAppSpotRemoveAdFromSpace(BannerData1.c_str());
				break;
			case AdType_Interstitial:
				s3eFlurryAppSpotRemoveAdFromSpace(InterstitialData1.c_str());
				break;
			}
			break;
		}
#endif // CZ_ADS_FLURRY_APPSPOT_ENABLE
#if defined(CZ_ADS_REVMOB_ENABLE)
	case Ads_Revmob:
		{
			switch (type)
			{
			case AdType_Banner:
				RevMobHideBanner();
				break;
			case AdType_Interstitial:
				break;
			case AdType_Popup:
				break;
			}
			break;
		}
#endif // CZ_ADS_REVMOB_ENABLE
	}
}

void	CzPlatformAds::NotifyAdEvent(const char* evt, const char* data1, int data2)
{
	CzScriptCallback& callback = getScriptCallback();
	if (callback.Valid)
	{
		CzString p1 = evt;
		CzString p2 = data1;
		CzString p3 = data2;
		callback.ScriptEngine->CallFunctionRef(callback.FunctionRef, &p1, &p2, &p3, NULL);
	}
}


int32	CzPlatformAds::AdClosed(void* systemData, void* userData)
{
	PLATFORM_ADS->NotifyAdEvent("ad_closed", (const char*)systemData, 0);
	return 0;
}

int32	CzPlatformAds::AdReceived(void* systemData, void* userData)
{
	PLATFORM_ADS->NotifyAdEvent("ad_received", (const char*)systemData, 0);
	return 0;
}

int32	CzPlatformAds::AdFailed(void* systemData, void* userData)
{
	PLATFORM_ADS->NotifyAdEvent("ad_failed", (const char*)systemData, 0);
	return 0;
}

int32	CzPlatformAds::AdClicked(void* systemData, void* userData)
{
	PLATFORM_ADS->NotifyAdEvent("ad_clicked", (const char*)systemData, 0);
	return 0;
}

int32	CzPlatformAds::AdVideoEnded(void* systemData, void* userData)
{
	PLATFORM_ADS->NotifyAdEvent("ad_video_ended", (const char*)systemData, 0);
	return 0;
}

