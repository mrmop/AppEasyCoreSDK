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

#include "CzAdsView.h"
#include "CzFile.h"
#include "CzInput.h"
#include "CzImage.h"
#include "CzScene.h"
#include "CzSprite.h"
#include "CzUtil.h"
#include "CzAdsViewAnimator.h"

CDECLARE_SINGLETON(CzAdsView)

CzAdsViewData::CzAdsViewData()
{
	Tapped = false;
	Position.x = 0;
	Position.y = 0;
	Scale = 1.0f;
	Angle = 0;
	Visible = false;
	Error = CzAds::ErrorInvalidImage;
	Colour.r = 0xff;
	Colour.g = 0xff;
	Colour.b = 0xff;
	Colour.a = 0xff;
	Ad = NULL;
	AdSprite = new CzBitmapSprite();
	AdSprite->Init();
}

CzAdsViewData::~CzAdsViewData()
{
	SAFE_DELETE(Ad);
	for (CzList<CzAdsViewAnimator*>::iterator it = Animators.begin(); it != Animators.end(); ++it)
	{
		delete *it;
	}
	Animators.clear();

	SAFE_DELETE(AdSprite)
}

CzAdsViewData* CzAdsView::getOldestAdSlot()
{
	if (NumAdsVisible == 1)
		return &AdData[0];

	CzAdsViewData* oldest = NULL;

	// Only check visible ads
	for (int t = 0; t < NumAdsVisible; t++)
	{
		if (AdData[t].Ad == NULL)
		{
			// Ad slot not even used so return this one
			oldest = &AdData[t];
			break;
		}
		else
		{
			// Find oldest ad slot
			if (oldest == NULL)
			{
				oldest = &AdData[t];
			}
			else
			{
				if (AdData[t].Ad->AdTime < oldest->Ad->AdTime)
				{
					oldest = &AdData[t];
				}
			}
		}
	}

	return oldest;
}

void CzAdsView::setNewAdInterval(int interval)
{
	if (interval == 0)
	{
		NewAdTimer.Stop();
		return;
	}

	if (interval < 5)
		interval = 5;
	NewAdInterval = interval;

	NewAdTimer.setDuration(NewAdInterval * 1000);
}
	
bool CzAdsView::Init(const char* id)
{
	MinError = CzAds::ErrorHouseAd;
	PrevTappedAd = NULL;
	NumAdsVisible = 1;
	Visible = true;

	CzAds::Create();
	CZ_ADS->Init();
	CZ_ADS->setApplicationID(id);

	return true;
}

void CzAdsView::Release()
{
	CZ_ADS->Release();
	CzAds::Destroy();
}

void CzAdsView::Update(float dt)
{
	if (!Visible)
		return;

	// Update the ads system
	CZ_ADS->Update();

	// Check to see if new ad has arrived
	CzAdsViewData* data = getOldestAdSlot();
	if (CZ_ADS->isAdAvailable())
	{
		CzAds::eAdsError error = CZ_ADS->getError();
		if (CZ_ADS->getError() <= MinError)
		{
			// No error so update oldest ad data slot with new ad data
			SAFE_DELETE(data->Ad)
			data->Ad = CZ_ADS->getAd().getCopy();
			if (data->Ad != NULL)
			{
				data->AdSprite->setImage(data->Ad->Image);
				data->AdSprite->setDestSize(data->Ad->Image->getWidth(), data->Ad->Image->getHeight());
				data->AdSprite->setSrcRect(0, 0, data->Ad->Image->getWidth(), data->Ad->Image->getHeight());
				data->Error = error;
				ResetAnims(data);
			}
		}
		else
		{
			// We ignore any faulty ad requests
		}
		CZ_ADS->setAdAvailable(false);	// Allow the next ad request
	}
	else
	{
	}

	// Process a tapped ad
	int num_touches = CZ_INPUT->getTouchCount();
	if (PrevTappedAd != NULL && num_touches == 0)
	{
		if (PrevTappedAd->AdSprite->HitTestNoClip((float)CZ_INPUT->getTouch(0)->x, (float)CZ_INPUT->getTouch(0)->y))
		{
			// Launch the ad in the web browser
			if (PrevTappedAd->Ad != NULL)
				PLATFORM_SYS->ExecuteViaOS(PrevTappedAd->Ad->LinkURI.c_str(), false);
		}
		PrevTappedAd->Colour.a = 0xff;
		PrevTappedAd->Tapped = false;
		PrevTappedAd = NULL;
	}
	
	// Process ad view data
	int64 current_time = PLATFORM_SYS->getTimeInMs();
	for (int t = 0; t < NumAdsVisible; t++)
	{
		data = &AdData[t];

		// Mark any old or errored ads as invisible
		if (data->Ad != NULL)
		{
			int64 ad_age = current_time - data->Ad->AdTime;
			if (data->Error > MinError || ad_age >= CZ_MAX_AD_AGE)
			{
				data->Visible = false;
			}
			else
			{
				data->Visible = true;
				if (PrevTappedAd == NULL)
				{
					// Check for tap inside area of ad
					if (num_touches != 0)
					{
						data->Tapped = true;
						if (data->AdSprite->HitTestNoClip((float)CZ_INPUT->getTouch(0)->x, (float)CZ_INPUT->getTouch(0)->y))
						{
							data->Colour.a = 0x7f;
							PrevTappedAd = data;
						}
					}
				}

				// Update attached animators
				int total_phases_done = 0;
				for (CzList<CzAdsViewAnimator*>::iterator it = data->Animators.begin(); it != data->Animators.end(); ++it)
				{
					(*it)->Update(dt);
					if (Looped)
					{
						if ((*it)->getPhase() == CzAdsViewAnimator::AnimPhaseDone)
						{
							total_phases_done++;
						}
					}
				}

				// If animation is looped then reset if done
				if (Looped)
				{
					if (data->Animators.size() == (unsigned int)total_phases_done)
					{
						ResetAnims(data);
					}
				}
			}
		}
		else
		{
			data->Visible = false;
		}
	}

	// Request a new ad
	if (NewAdTimer.hasStarted())
	{
		if (NewAdTimer.HasTimedOut())
		{
			CZ_ADS->RequestAd(AdProvider);
			NewAdTimer.setDuration(NewAdInterval * 1000);
		}
	}
}

void CzAdsView::Draw(CzScene* scene)
{
	if (!Visible)
		return;

	for (int t = 0; t < NumAdsVisible; t++)
	{
		CzAdsViewData* data = &AdData[t];

		data->AdSprite->setVisible(data->Visible);
		if (data->Visible)
		{
			data->AdSprite->setPosition((float)data->Position.x, (float)data->Position.y);
			data->AdSprite->setScale(data->Scale);
			data->AdSprite->setLayer(9);
			data->AdSprite->setAngle(data->Angle);
			data->AdSprite->setColour(data->Colour);
			if (scene != NULL)
			{
				data->AdSprite->setManager(scene->getSpriteManager());
			}
			data->AdSprite->Update();
			data->AdSprite->Draw();
		}
	}
}

void CzAdsView::RequestNewAd(CzAds::eAdProvider ad_provider, bool text_ads)
{
//	if (!Visible)
//		return;

	CZ_ADS->setTextAds(text_ads);
	CZ_ADS->RequestAd(ad_provider);
}

void CzAdsView::ResetAnims(CzAdsViewData* data)
{
	for (CzList<CzAdsViewAnimator*>::iterator it = data->Animators.begin(); it != data->Animators.end(); ++it)
	{
		(*it)->setAdSize(data->AdSprite->getSrcWidth(), data->AdSprite->getSrcHeight());
		(*it)->Reset();
	}
}

void CzAdsView::ResetAllAnims()
{
	for (int t = 0; t < CZ_MAX_CACHED_ADS; t++)
	{
		if (AdData[t].Ad != NULL)
		{
			ResetAnims(&AdData[t]);
		}
	}
}









