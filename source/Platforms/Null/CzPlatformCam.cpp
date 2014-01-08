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

#include "IzPlatformSys.h"
#include "CzPlatformCam.h"
#include "CzImage.h"

bool CzPlatformCam::isAvailable()
{
	return false;
}

int CzPlatformCam::Init()
{
	Started = false;
	Updated = false;
	ImageValid = false;
	ImageCreated = false;
	ImageBrush = NULL;
	LastFrameOrientation = 0;
	Initialised = true;

	return 1;
}

void CzPlatformCam::Release()
{
	Stop();
	if (ImageCreated)
		SAFE_DELETE(ImageBrush);
	ImageCreated = false;
	Updated = false;
	ImageValid = false;
	ImageCreated = false;
	LastFrameOrientation = 0;
}

void CzPlatformCam::setImageBrush(CzImage* brush)
{
	if (ImageBrush == brush)
		return;
	if (ImageCreated)
		SAFE_DELETE(ImageBrush)
	ImageCreated = false;
	ImageBrush = brush;
}

CzImage* CzPlatformCam::getImageBrush()
{
	return ImageBrush;
}

eCzCameraStatus CzPlatformCam::getStatus()
{
	if (!isAvailable() || !Started)
		return CS_Failed;

	return CS_Failed;
}

bool CzPlatformCam::setFrontFacing()
{
	CameraType = CCT_Front;

	return false;
}

bool CzPlatformCam::setRearFacing()
{
	CameraType = CCT_Rear;

	return false;
}

int CzPlatformCam::getLastFrameOrientation() const
{
	return LastFrameOrientation;
}

void CzPlatformCam::setLastFrameOrientation(int angle)
{
	LastFrameOrientation = angle;
}

eCzCameraType CzPlatformCam::getCameraType() const
{
	return CameraType;
}

static int32 CzPlatformCam_CameraUpdate(void* systemData, void* userData)
{
	if (!PLATFORM_CAM->isStarted())
		return 0;

	if (PLATFORM_CAM->getImageBrush() == NULL)
	{
		// Create image to hold the camera data
		CzImage* image = new CzImage();
		PLATFORM_CAM->setImageBrush(image);
		PLATFORM_CAM->setImageCreated(true);
	}

	if (!PLATFORM_CAM->isImageValid())
	{
		CzImage* image = PLATFORM_CAM->getImageBrush();

		// Inialise the image to the same size and format as the cam data

		PLATFORM_CAM->setImageValid(true);
	}

	// Copy the camera image data into the texture. Note that it does not get copied to VRAM at this point.
	if (PLATFORM_CAM->isImageValid())
	{
		CzImage* image = PLATFORM_CAM->getImageBrush();
		if (image != NULL)
		{
			// Copy camera pixels to image

			PLATFORM_CAM->setUpdated(true);
		}
	}

	int angle = 0;

	// Determine cameras angle of rotation

	if (PLATFORM_CAM->getCameraType() == CCT_Front)
		angle = -angle;
	PLATFORM_CAM->setLastFrameOrientation(angle);

	return 0;
}

bool CzPlatformCam::Start(eCzCameraCaptureSize size_hint, eCzCameraCaptureQuality quality)
{
	if (isAvailable() && !Started)
	{
		// if cam already started then stop it
		if (Started)
			Stop();

		if (ImageBrush != NULL)
		{
			// Release previous image data so image can be re-initialised
			setImageValid(false);
			ImageBrush->Release();
		}

		// Start the camera
	}

	return true;
}

void CzPlatformCam::Stop()
{
	if (Started)
	{
		// Stop the camera

		Started = false;
	}

}