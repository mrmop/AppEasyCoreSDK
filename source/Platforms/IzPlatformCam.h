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

#if !defined(_CIZ_PLATFORM_CAM_H_)
#define _CIZ_PLATFORM_CAM_H_

#include "CzPlatform.h"

class CzImage;

#define	PLATFORM_CAM			PLATFORM->getCam()

enum eCzCameraCaptureSize
{
	CCS_Small,		// Approx 1/4 screen size
	CCS_Medium,		// Approx 1/2 screen size
	CCS_Large, 		// Approx full screen size
};

enum eCzCameraCaptureQuality
{
	CCQ_Low, 
	CCQ_Medium, 
	CCQ_High, 
};

enum eCzCameraType
{
	CCT_Rear, 
	CCT_Front, 
};

enum eCzCameraStatus
{
	CS_Idle,			// Camera is idle
	CS_Streaming,		// Camera is streaming
	CS_Failed,			// Error occurred during capture.
	CS_Suspended,		// Camera is paused due to device suspend
	CS_Restarting		// Camera is being restarted after a device suspend
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IzPlatformCam
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IzPlatformCam
{
	// Properties
protected:
	bool					Initialised;
public:
	bool					isInitialised() const	{ return Initialised; }
	virtual bool			isAvailable() = 0;
	virtual bool			isStarted() const = 0;
	virtual void			setUpdated(bool updated) = 0;
	virtual bool			isUpdated() const = 0;
	virtual void			setImageValid(bool valid) = 0;
	virtual bool			isImageValid() const = 0;
	virtual void			setImageCreated(bool valid) = 0;
	virtual bool			isImageCreated() const = 0;
	virtual eCzCameraCaptureSize	getSizeHint() const = 0;
	virtual eCzCameraCaptureQuality getQuality() const = 0;
	virtual void			setImageBrush(CzImage* brush) = 0;
	virtual CzImage*		getImageBrush() = 0;
	virtual eCzCameraStatus	getStatus() = 0;
	virtual bool			setFrontFacing() = 0;
	virtual bool			setRearFacing() = 0;
	virtual int				getLastFrameOrientation() const = 0;
	virtual void			setLastFrameOrientation(int angle) = 0;
	virtual eCzCameraType	getCameraType() const = 0;
	// Properties end
protected:
public:
	virtual ~IzPlatformCam() {}
	virtual int		Init() = 0;
	virtual void	Release() = 0;

	virtual bool	Start(eCzCameraCaptureSize size_hint, eCzCameraCaptureQuality quality = CCQ_Medium) = 0;
	virtual void	Stop() = 0;
};


#endif	// _CIZ_PLATFORM_CAM_H_
