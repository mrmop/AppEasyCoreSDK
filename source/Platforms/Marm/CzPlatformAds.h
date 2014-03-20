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

#if !defined(_CCZ_PLATFORM_ADS_H_)
#define _CCZ_PLATFORM_ADS_H_

#include "IzPlatformAds.h"
#include "CzString.h"

class CzPlatformAds : public IzPlatformAds
{
	// Properties
protected:
	CzString	BannerData1;
	int			BannerData2;
	CzString	InterstitialData1;
	int			InterstitialData2;
	CzString	PopupData1;
	int			PopupData2;
public:
	// Properties end
public:
	bool			isAvailable(eCzAdProvider provider);
	int				Init(eCzAdProvider provider, const char* id, const char* dat11, int data2);
	void			Release();
	void			Update();

	void			Load(eCzAdType type, const char* data1, int data2);
	void			LoadShow(eCzAdType type, const char* data1, int data2);
	void			Show(eCzAdType type);
	void			Hide(eCzAdType type);

	void			NotifyAdEvent(const char* evt, const char* data1, int data2);

	static int32 AdClosed(void* systemData, void* userData);
	static int32 AdReceived(void* systemData, void* userData);
	static int32 AdFailed(void* systemData, void* userData);
	static int32 AdClicked(void* systemData, void* userData);
	static int32 AdVideoEnded(void* systemData, void* userData);
};


#endif	// _CCZ_PLATFORM_ADS_H_
