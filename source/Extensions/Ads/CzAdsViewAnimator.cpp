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

#include "IzPlatformDisplay.h"
#include "CzAdsViewAnimator.h"
#include "CzAdsView.h"
#include "CzMath.h"

void CzAdsViewAnimator::Init()
{
	AnimPhase = AnimPhaseIn;
	InAnim = AnimFadeIn;
	OutAnim = AnimFadeOut;
	StayAnim = AnimNone;
	InDuration = 1000;
	OutDuration = 1000;
	StayDuration = 20000;
	StartColour.r = 0;
	StartColour.g = 0;
	StartColour.b = 0;
	StartColour.a = 0;
	StartPosition.x = 0;
	StartPosition.y = 0;
	StartScale = 1.0f;
	StartAngle = 0;
	TargetColour.r = 0;
	TargetColour.g = 0;
	TargetColour.b = 0;
	TargetColour.a = 0;
	TargetPosition.x = 0;
	TargetPosition.y = 0;
	TargetScale = 1.0f;
	TargetAngle = 0;
	RestingPosition.x = 0;
	RestingPosition.y = 0;
	AdViewDataIndex = 0;
	Looped = false;
}

void CzAdsViewAnimator::Update(float dt)
{
	if (!CZ_ADS_VIEW->isVisible())
		return;

	Width = PLATFORM_DISPLAY->getCurrentWidth();
	float screen_scale = ((float)Width * 0.7f) / AdWidth;
//	setRestingPosition((AdWidth * screen_scale) / 2, (AdHeight * screen_scale) / 2);

	switch (AnimPhase)
	{
		case AnimPhaseIn:
			State_PhaseIn();
			break;
		case AnimPhaseOut:
			State_PhaseOut();
			break;
		case AnimPhaseStay:
			State_PhaseStay();
			break;
		case AnimPhaseDone:
			break;
	}
}

void CzAdsViewAnimator::Reset()
{
	switch (InAnim)
	{
	case AnimFadeIn:
		StartColour.r = 0;
		StartColour.g = 0;
		StartColour.b = 0;
		StartColour.a = 0;
		TargetColour.r = 255;
		TargetColour.g = 255;
		TargetColour.b = 255;
		TargetColour.a = 255;
		break;
	case AnimScaleIn:
		StartScale = 0.05f;
		TargetScale = 1.25f;
		break;
	case AnimSpinIn:
		StartAngle = -180.0f;
		TargetAngle = 0.0f;
		break;
	case AnimBottomSweepIn:
		StartPosition.x = 0;
		StartPosition.y = Height;
		TargetPosition.x = 0;
		TargetPosition.y = 0;
		break;
	case AnimTopSweepIn:
		StartPosition.x = 0;
		StartPosition.y = -Height;
		TargetPosition.x = 0;
		TargetPosition.y = 0;
		break;
	case AnimLeftSweepIn:
		StartPosition.x = -Width;
		StartPosition.y = 0;
		TargetPosition.x = 0;
		TargetPosition.y = 0;
		break;
	case AnimRightSweepIn:
		StartPosition.x = Width;
		StartPosition.y = Height / 2;
		TargetPosition.x = 0;
		TargetPosition.y = 0;
		break;
	}
	AnimTimer.setDuration(InDuration + OutDuration + StayDuration);
	AnimPhase = AnimPhaseIn;
}

void CzAdsViewAnimator::State_PhaseIn()
{
	if (AnimTimer.hasStopped())
		return;

	int elapsed = (int)AnimTimer.GetElapsedTime();
	int max = (elapsed > InDuration) ? InDuration : elapsed;

	switch (InAnim)
	{
	case AnimFadeIn:
		{
			// Interpolate  values
			int r = StartColour.r + ((TargetColour.r - StartColour.r) * max) / InDuration;
			int g = StartColour.g + ((TargetColour.g - StartColour.g) * max) / InDuration;
			int b = StartColour.b + ((TargetColour.b - StartColour.b) * max) / InDuration;
			int a = StartColour.a + ((TargetColour.a - StartColour.a) * max) / InDuration;
			CZ_ADS_VIEW->setColour(AdViewDataIndex, r, g, b, a);
		}
		break;
	case AnimScaleIn:
		{
			// Interpolate  values
			float scale = StartScale + ((TargetScale - StartScale) * max) / InDuration;
			CZ_ADS_VIEW->setScale(AdViewDataIndex, scale);
		}
		break;
	case AnimSpinIn:
		{
			// Interpolate  values
			float angle = StartAngle + ((TargetAngle - StartAngle) * max) / InDuration;
			CZ_ADS_VIEW->setAngle(AdViewDataIndex, angle);
		}
		break;
	case AnimBottomSweepIn:
	case AnimTopSweepIn:
	case AnimLeftSweepIn:
	case AnimRightSweepIn:
		{
			// Interpolate  values
			int x = StartPosition.x + ((TargetPosition.x - StartPosition.x) * max) / InDuration;
			int y = StartPosition.y + ((TargetPosition.y - StartPosition.y) * max) / InDuration;
			CZ_ADS_VIEW->setPosition(AdViewDataIndex, RestingPosition.x + x, RestingPosition.y + y);
		}
		break;
	}

	// Check for end of in phase
	if (elapsed > InDuration)
	{
		AnimPhase = AnimPhaseStay;
	}

	// Check to see if anim has timed out
	if (AnimTimer.HasTimedOut())
		AnimTimer.Stop();
}

void CzAdsViewAnimator::State_PhaseStay()
{
	if (AnimTimer.hasStopped())
		return;

	int elapsed = (int)AnimTimer.GetElapsedTime() - InDuration;
	int it = 0;

	// Do a quick flash half way through stay phase
	int start_ms = StayDuration / 2;
	int end_ms = start_ms + 2000;
	if (elapsed >= start_ms && elapsed <= end_ms)
	{
		int wobble = (int)(CzMath::sin(((((elapsed - start_ms) * 720.0f) / 2000.0f) * PI) / 180.0f) * 25.0f);

		switch (StayAnim)
		{
		case AnimWobble:
			{
				CZ_ADS_VIEW->setPosition(AdViewDataIndex, RestingPosition.x + wobble, RestingPosition.y);
			}
			break;
		case AnimScale:
			{
			}
			break;
		}
	}

	// Wait until we reach out phase of animation
	elapsed = (int)AnimTimer.GetElapsedTime();
	if (elapsed >= InDuration + StayDuration)
		AnimPhase = AnimPhaseOut;

	if (AnimTimer.HasTimedOut())
		AnimTimer.Stop();
}

void CzAdsViewAnimator::State_PhaseOut()
{
	if (AnimTimer.hasStopped())
		return;

	int elapsed = (int)AnimTimer.GetElapsedTime() - InDuration - StayDuration;
	int max = (elapsed > OutDuration) ? OutDuration : elapsed;

	switch (OutAnim)
	{
	case AnimFadeOut:
		{
			// Interpolate values
			int r = TargetColour.r - ((TargetColour.r - StartColour.r) * max) / OutDuration;
			int g = TargetColour.g - ((TargetColour.g - StartColour.g) * max) / OutDuration;
			int b = TargetColour.b - ((TargetColour.b - StartColour.b) * max) / OutDuration;
			int a = TargetColour.a - ((TargetColour.a - StartColour.a) * max) / OutDuration;
			CZ_ADS_VIEW->setColour(AdViewDataIndex, r, g, b, a);
		}
		break;
	case AnimScaleOut:
		{
			// Interpolate  values
			float scale = TargetScale - ((TargetScale - StartScale) * max) / OutDuration;
			CZ_ADS_VIEW->setScale(AdViewDataIndex, scale);
		}
		break;
	case AnimSpinOut:
		{
			// Interpolate  values
			float angle = TargetAngle - ((TargetAngle - StartAngle) * max) / OutDuration;
			CZ_ADS_VIEW->setAngle(AdViewDataIndex, angle);
		}
		break;
	case AnimBottomSweepOut:
	case AnimTopSweepOut:
	case AnimLeftSweepOut:
	case AnimRightSweepOut:
		{
			// Interpolate  values
			int x = TargetPosition.x - ((TargetPosition.x - StartPosition.x) * max) / OutDuration;
			int y = TargetPosition.y - ((TargetPosition.y - StartPosition.y) * max) / OutDuration;
			CZ_ADS_VIEW->setPosition(AdViewDataIndex, RestingPosition.x + x, RestingPosition.y + y);
		}
		break;
	}

	// Check to see if anim has timed out
	if (elapsed > OutDuration || AnimTimer.HasTimedOut())
	{
		AnimPhase = AnimPhaseDone;
		AnimTimer.Stop();
		if (Looped)
		{
			Reset();
		}
	}
}
