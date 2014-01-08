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

#if !defined(_CZ_ADS_VIEW_ANIMATOR_H_)
#define _CZ_ADS_VIEW_ANIMATOR_H_

#include "CzUtil.h"
#include "CzTime.h"
#include "CzTypes.h"

//
//
// CzAdsViewAnimator - The CzAdsViewAnimator class is responsible for animating a CzAdsView
//
//
class CzAdsViewAnimator
{
public:
	enum eCzAdsAnimIn
	{
		AnimFadeIn,				// Fades in
		AnimScaleIn,			// Scales in
		AnimSpinIn,				// Spins in
		AnimBottomSweepIn,		// Sweeps in from bottom
		AnimTopSweepIn,			// Sweeps in from bottom
		AnimLeftSweepIn,		// Sweeps in from left
		AnimRightSweepIn,		// Sweeps in from right
	};
	enum eCzAdsAnimOut
	{
		AnimFadeOut,			// Fades in
		AnimScaleOut,			// Scales in
		AnimSpinOut,			// Spins in
		AnimBottomSweepOut,		// Sweeps in from bottom
		AnimTopSweepOut,		// Sweeps in from bottom
		AnimLeftSweepOut,		// Sweeps in from left
		AnimRightSweepOut,		// Sweeps in from right
	};
	enum eCzAdsAnimStay
	{
		AnimNone,				// No anim
		AnimWobble,				// Wobble
		AnimScale,				// Scale
	};
	enum eCzAdsAnimPhase
	{
		AnimPhaseIn,			// Animation phase in
		AnimPhaseStay,			// Animation phase stay
		AnimPhaseOut,			// Animation phase out
		AnimPhaseDone,			// Animation done
	};

	// Properties
protected:
	CzIVec2						RestingPosition;	// resting position where ad should stop
	eCzAdsAnimIn				InAnim;				// Animation to show the ad
	eCzAdsAnimOut				OutAnim;			// Animation to hide the ad
	eCzAdsAnimStay				StayAnim;			// Animation shilst ad is visible
	int							InDuration;			// Amount of time to spend bringing the ad in
	int							OutDuration;		// Amount of time to spend sending the ad out
	int							StayDuration;		// Amount of time to ad shoud stay on screen
	int							Width, Height;		// Canvas width and height
	int							AdWidth, AdHeight;	// Ad width and height
	int							AdViewDataIndex;	// The index of this animations parent ad view data object in the ad view
	bool						Looped;				// if true then animation will loop continually
public:
	void						setRestingPosition(int x, int y)						{ RestingPosition.x = x; RestingPosition.y = y; }
	void						setInAnim(eCzAdsAnimIn anim, int duration_ms)			{ InAnim = anim; InDuration = duration_ms; Reset(); }
	void						setOutAnim(eCzAdsAnimOut anim, int duration_ms)			{ OutAnim = anim; OutDuration = duration_ms; }
	void						setStayAnim(eCzAdsAnimStay anim)						{ StayAnim = anim; }
	void						setStayDuration(int duration_ms)						{ StayDuration = duration_ms; }
	void						setCanvasSize(int width, int height)					{ Width = width; Height = height; }
	void						setAdSize(int width, int height)						{ AdWidth = width; AdHeight = height; }
	void						setAdViewDataIndex(int index)							{ AdViewDataIndex = index; }
	void						setLooped(bool looped)									{ Looped = looped; }
	eCzAdsAnimPhase				getPhase() const										{ return AnimPhase; }
	// Properties end

protected:
	CzIVec2						TargetPosition;
	float						TargetAngle;
	float						TargetScale;
	CzColour					TargetColour;
	CzIVec2						StartPosition;
	float						StartAngle;
	float						StartScale;
	CzColour					StartColour;
	eCzAdsAnimPhase				AnimPhase;
	CzTimer						AnimTimer;
	void						State_PhaseIn();
	void						State_PhaseOut();
	void						State_PhaseStay();

public:
	CzAdsViewAnimator() 		{}
	virtual ~CzAdsViewAnimator()		{}
	virtual void				Init();
	virtual void				Update(float dt);	// Updates the anim view manager
	virtual void				Reset();			// Reset the animation
	
	// Utility
};



#endif	// _CZ_ADS_VIEW_ANIMATOR_H_