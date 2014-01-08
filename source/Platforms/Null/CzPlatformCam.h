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

#if !defined(_CCZ_PLATFORM_CAM_H_)
#define _CCZ_PLATFORM_CAM_H_

#include "IzPlatformCam.h"

class CzPlatformCam: public IzPlatformCam
{
protected:

	// Properties
private:
	bool					Started;
	eCzCameraCaptureSize	SizeHint;
	eCzCameraCaptureQuality Quality;
	bool					ImageValid;
	bool					ImageCreated;
	bool					Updated;
	CzImage*				ImageBrush;
	int						LastFrameOrientation;
	eCzCameraType			CameraType;
public:
	bool					isAvailable();
	bool					isStarted() const { return Started; }
	void					setUpdated(bool updated) { Updated = updated; }
	bool					isUpdated() const { return Updated; }
	void					setImageValid(bool valid) { ImageValid = valid; }
	bool					isImageValid() const { return ImageValid; }
	void					setImageCreated(bool valid) { ImageCreated = valid; }
	bool					isImageCreated() const { return ImageCreated; }
	eCzCameraCaptureSize	getSizeHint() const { return SizeHint; }
	eCzCameraCaptureQuality getQuality() const { return Quality; }

	void					setImageBrush(CzImage* brush);
	CzImage*				getImageBrush();
	eCzCameraStatus			getStatus();
	bool					setFrontFacing();
	bool					setRearFacing();
	int						getLastFrameOrientation() const;
	void					setLastFrameOrientation(int angle);
	eCzCameraType			getCameraType() const;
	// Properties end

protected:
public:
	CzPlatformCam() : ImageBrush(NULL), Started(false),ImageValid(false), ImageCreated(false), Updated(false), LastFrameOrientation(0)  {}
	int			Init();
	void		Release();

	bool		Start(eCzCameraCaptureSize size_hint, eCzCameraCaptureQuality quality = CCQ_Medium);
	void		Stop();
};


#endif	// _CCZ_PLATFORM_CAM_H_
