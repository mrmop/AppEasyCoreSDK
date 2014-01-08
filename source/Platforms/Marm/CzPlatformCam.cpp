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
#include "Marm/CzPlatformCam.h"
#include "CzImage.h"

s3eCameraQualityHint CzPlatformCam::toMarmalade(eCzCameraCaptureQuality hint)
{
	switch (hint)
	{
	case CCQ_Low:
		return S3E_CAMERA_STREAMING_QUALITY_HINT_LOW;
	case CCQ_Medium:
		return S3E_CAMERA_STREAMING_QUALITY_HINT_MED;
	case CCQ_High:
		return S3E_CAMERA_STREAMING_QUALITY_HINT_HIGH;
	}

	return S3E_CAMERA_STREAMING_QUALITY_HINT_LOW;
}

s3eCameraStreamingSizeHint CzPlatformCam::toMarmalade(eCzCameraCaptureSize hint)
{
	switch (hint)
	{
	case CCS_Small:
		return S3E_CAMERA_STREAMING_SIZE_HINT_SMALLEST;
	case CCS_Medium:
		return S3E_CAMERA_STREAMING_SIZE_HINT_MEDIUM;
	case CCS_Large:
		return S3E_CAMERA_STREAMING_SIZE_HINT_LARGEST;
	}

	return S3E_CAMERA_STREAMING_SIZE_HINT_SMALLEST;
}

s3eCameraType CzPlatformCam::toMarmalade(eCzCameraType type)
{
	switch (type)
	{
	case CCT_Rear:
		return S3E_CAMERA_TYPE_REAR;
	case CCT_Front:
		return S3E_CAMERA_TYPE_FRONT;
	}

	return S3E_CAMERA_TYPE_REAR;
}

s3eCameraStatus CzPlatformCam::toMarmalade(eCzCameraStatus status)
{
	switch (status)
	{
	case CS_Idle:
		return S3E_CAMERA_IDLE;
	case CS_Streaming:
		return S3E_CAMERA_STREAMING;
	case CS_Failed:
		return S3E_CAMERA_FAILED;
	case CS_Suspended:
		return S3E_CAMERA_SUSPENDED;
	case CS_Restarting:
		return S3E_CAMERA_RESTARTING;
	}

	return S3E_CAMERA_IDLE;
}

eCzCameraCaptureSize CzPlatformCam::fromMarmalade(s3eCameraStreamingSizeHint hint)
{
	switch (hint)
	{
	case S3E_CAMERA_STREAMING_SIZE_HINT_SMALLEST:
		return CCS_Small;
	case S3E_CAMERA_STREAMING_SIZE_HINT_MEDIUM:
		return CCS_Medium;
	case S3E_CAMERA_STREAMING_SIZE_HINT_LARGEST:
		return CCS_Large;
	}

	return CCS_Small;
}

eCzCameraCaptureQuality CzPlatformCam::fromMarmalade(s3eCameraQualityHint hint)
{
	switch (hint)
	{
	case S3E_CAMERA_STREAMING_QUALITY_HINT_LOW:
		return CCQ_Low;
	case S3E_CAMERA_STREAMING_QUALITY_HINT_MED:
		return CCQ_Medium;
	case S3E_CAMERA_STREAMING_QUALITY_HINT_HIGH:
		return CCQ_High;
	}

	return CCQ_Low;
}

eCzCameraType CzPlatformCam::fromMarmalade(s3eCameraType type)
{
	switch (type)
	{
	case S3E_CAMERA_TYPE_REAR:
		return CCT_Rear;
	case S3E_CAMERA_TYPE_FRONT:
		return CCT_Front;
	}

	return CCT_Front;
}

eCzCameraStatus CzPlatformCam::fromMarmalade(s3eCameraStatus status)
{
	switch (status)
	{
	case S3E_CAMERA_IDLE:
		return CS_Idle;
	case S3E_CAMERA_STREAMING:
		return CS_Streaming;
	case S3E_CAMERA_FAILED:
		return CS_Failed;
	case S3E_CAMERA_SUSPENDED:
		return CS_Suspended;
	case S3E_CAMERA_RESTARTING:
		return CS_Restarting;
	}

	return CS_Idle;
}

bool CzPlatformCam::isAvailable()
{
	return s3eCameraAvailable() != 0;
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

	return fromMarmalade((s3eCameraStatus)s3eCameraGetInt(S3E_CAMERA_STATUS));
}

bool CzPlatformCam::setFrontFacing()
{
	CameraType = CCT_Front;
	if (PLATFORM_SYS->getDeviceType() == CzDeviceType_Windows)
		return true;
	else
		return s3eCameraSetInt(S3E_CAMERA_TYPE, S3E_CAMERA_TYPE_FRONT) == S3E_RESULT_SUCCESS;
}

bool CzPlatformCam::setRearFacing()
{
	CameraType = CCT_Rear;
	if (PLATFORM_SYS->getDeviceType() == CzDeviceType_Windows)
		return true;
	else
		return s3eCameraSetInt(S3E_CAMERA_TYPE, S3E_CAMERA_TYPE_REAR) == S3E_RESULT_SUCCESS;
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
	{
//CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "****  Camera not started");
		return 0;
	}

	s3eCameraFrameData* frame = (s3eCameraFrameData*)systemData;
//CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "****  frame width is:");
//CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, CzString(frame->m_Width).c_str());
//CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "****  frame height is:");
//CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, CzString(frame->m_Height).c_str());

	if (PLATFORM_CAM->getImageBrush() == NULL)
	{
		// Create image to hold the camera data
		CzImage* image = new CzImage();
		PLATFORM_CAM->setImageBrush(image);
		PLATFORM_CAM->setImageCreated(true);
	}

	if (!PLATFORM_CAM->isImageValid())
	{
//CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "****  Image is valid!");
		CzImage* image = PLATFORM_CAM->getImageBrush();
		if (s3eCameraIsFormatSupported(S3E_CAMERA_PIXEL_TYPE_RGB565) || s3eCameraIsFormatSupported(S3E_CAMERA_PIXEL_TYPE_RGB565_CONVERTED))
			image->Init((uint8*)frame->m_Data, frame->m_Width, frame->m_Height, frame->m_Pitch, CzImage::Format_RGB565, true);
		else
		if (s3eCameraIsFormatSupported(S3E_CAMERA_PIXEL_TYPE_RGB888))
			image->Init((uint8*)frame->m_Data, frame->m_Width, frame->m_Height, frame->m_Pitch, CzImage::Format_RGB888, true);

		PLATFORM_CAM->setImageValid(true);
	}
//	else
//		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "****  Image is not valid!");

	// Copy the camera image data into the texture. Note that it does not get copied to VRAM at this point.
	if (PLATFORM_CAM->isImageValid())
	{
//CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "****  Image is valid");
		CzImage* image = PLATFORM_CAM->getImageBrush();
		if (image != NULL)
		{
//CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "****  Updated cam image");
			image->ChangePixels(frame->m_Data, image->getFormat());
			PLATFORM_CAM->setUpdated(true);
		}
//		else
//			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "****  Image is invalid");
	}

	int angle = 0;
	switch (frame->m_Rotation)
	{
	case S3E_CAMERA_FRAME_ROT90:
		angle = 90;
		break;
	case S3E_CAMERA_FRAME_ROT180:
		angle = 180;
		break;
	case S3E_CAMERA_FRAME_ROT270:
		angle = 270;
		break;
	}
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

		if (s3eCameraRegister(S3E_CAMERA_UPDATE_STREAMING, CzPlatformCam_CameraUpdate, 0) != S3E_RESULT_ERROR)
		{
			if (s3eCameraStart((s3eCameraStreamingSizeHint)size_hint, S3E_CAMERA_PIXEL_TYPE_RGB565_CONVERTED, (s3eCameraQualityHint)quality) == S3E_RESULT_SUCCESS)
				Started = true;
			else
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "CzPlatformCam::Start() -  Could not start camera");
				s3eCameraUnRegister(S3E_CAMERA_UPDATE_STREAMING, CzPlatformCam_CameraUpdate);
				return false;
			}
		}
		else
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "CzPlatformCam::Start() -  Could not register straming callback");
			return false;
		}
	}

	return true;
}

void CzPlatformCam::Stop()
{
	if (Started)
	{
		s3eCameraStop();
		s3eCameraUnRegister(S3E_CAMERA_UPDATE_STREAMING, CzPlatformCam_CameraUpdate);
		Started = false;
CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "****  Camera was STOPPED");
	}

}