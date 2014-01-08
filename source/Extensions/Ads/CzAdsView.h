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

#if !defined(_CZ_ADS_VIEW_H_)
#define _CZ_ADS_VIEW_H_

#include "CzUtil.h"
#include "CzHttp.h"
#include "CzImage.h"
#include "CzSprite.h"
#include "CzTime.h"
#include "CzAds.h"
#include "CzBitmapSprite.h"

#define	CZ_ADS_VIEW		(CzAdsView::getInstance())

class CzScene;
class CzAdsViewAnimator;

//
//
// CzAdsViewData - The CzAdsViewData structure contains data used to display a single ad uhit
//
//
struct CzAdsViewData
{
	CzIVec2						Position;			// Position of ad on screem
	float						Scale;				// Scale of the ad
	float						Angle;				// Scale of the ad
	bool						Visible;			// Ad visibility
	CzColour					Colour;				// Colour of ad
	CzList<CzAdsViewAnimator*>	Animators;			// Attached animators will animate the ad
	CzAds::eAdsError			Error;				// Contains error code if any if ad not received

	CzAd*						Ad;					// Ad that this view should draw
	CzBitmapSprite*				AdSprite;			// Sprite used by the ad view to display the ad
	bool						Tapped;
	CzIVec2						tv[4];				// Transformed vertices

	CzAdsViewData();
	virtual ~CzAdsViewData();
};

//
//
// CzAdsView - The CzAdsView class is responsible for collecting ads, rendering them, detecting clicks and launching the ad URL
//
//
class CzAdsView
{
public:
	CDEFINE_SINGLETON(CzAdsView)

	// Properties
protected:
	int							NewAdInterval;		// The number of seconds between collecting new ads (minimum is 5 seconds)
	CzAds::eAdsError	MinError;					// Minimum allowed error before displaying an ad
	CzAdsViewData				AdData[CZ_MAX_CACHED_ADS];
	int							NumAdsVisible;		// number of ads visible
	bool						Visible;			// Ad visibility
	bool						Looped;				// Loops animations if true
	CzAds::eAdProvider			AdProvider;			// Ad provider used for automated ad collection
public:
	void						setVisible(bool visible)					{ Visible = visible; }
	bool						isVisible() const							{ return Visible; }
	void						setVisible(int index, bool visible)			{ AdData[index].Visible = visible; }
	bool						isVisible(int index) const					{ return AdData[index].Visible; }
	void						setPosition(int index, int x, int y)		{ AdData[index].Position.x = x; AdData[index].Position.y = y; }
	CzIVec2&					getPosition(int index)						{ return AdData[index].Position; }
	void						setScale(int index, float scale)			{ AdData[index].Scale = scale; }
	float						getScale(int index) const					{ return AdData[index].Scale; }
	void						setAngle(int index, float angle)			{ AdData[index].Angle = angle; }
	float						getAngle(int index) const					{ return AdData[index].Angle; }
	void						setColour(int index, int r, int g, int b, int a) { AdData[index].Colour.r = r; AdData[index].Colour.g = g; AdData[index].Colour.b = b; AdData[index].Colour.a = a;}
	void						addAnimator(int index, CzAdsViewAnimator* animator)	{ AdData[index].Animators.push_back(animator); }
	void						setNumAdsVisible(int count)					{ NumAdsVisible = count; }
	int							getNumAdsVisible() const					{ return NumAdsVisible; }
	void						setAllowHouseAds(bool allow)				{ if (allow) MinError = CzAds::ErrorHouseAd; else MinError = CzAds::ErrorNone; }
	void						setNewAdInterval(int interval);	
	CzAdsViewData*				getAdViewData(int index)					{ return &AdData[index]; }
	void						setLooped(bool looped)						{ Looped = looped; }
	void						setAdProvider(CzAds::eAdProvider ad_provider)	{ AdProvider = ad_provider; }

	// Properties end

protected:
	bool						PreviousTouch;
	CzAdsViewData*				PrevTappedAd;
	CzTimer						NewAdTimer;				// Used to time new ad collection

	void						Transform();
	CzAdsViewData*				getOldestAdSlot();

public:
	virtual bool				Init(const char* id);						// Initialises the Ads view system (returns true if ads are supported)
	virtual void				Release();									// Releases data used by the Ads view system
	virtual void				Update(float dt = 1.0f);					// Updates the Ads view system, called every frame
	virtual void				Draw(CzScene* scene = NULL);				// Renders the ads view
	
	// Utility
	void						RequestNewAd(CzAds::eAdProvider ad_provider, bool text_ads = false);	// Forcesthe collection of a new ad
	void						ResetAllAnims();							// Resets all ad slot animations
	void						ResetAnims(CzAdsViewData* data);			// Resets ad slot animations

	// Internal

};



#endif	// _CZ_ADS_VIEW_H_