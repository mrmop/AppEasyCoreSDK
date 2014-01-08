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

#if !defined(_CZ_CAM_H_)
#define _CZ_CAM_H_

//
// Classes in this file
//
//  CzVideoCam			- A class that is used to control the video camera
//  CzVideoCamCreator	- Creates an instance of a VideoCam
//

#include "IzPlatformCam.h"
#include "CzUtil.h"
#include "CzImage.h"
#include "CzActions.h"
#include "CzEvents.h"

/**
 @class	CzVideoCam

 @brief	A class that is used to control the video camera.

 */

class CzVideoCam : public IzXomlResource
{
public:
	// Properties
protected:
	eCzCameraCaptureQuality		Quality;
	eCzCameraCaptureSize		Resolution;
	eCzCameraType				CameraType;
	CzEventManager*				EventsManager;			// List of events that the camera handles
public:
	eCzCameraCaptureQuality	getQuality() const					{ return Quality; }
	void						setQuality(eCzCameraCaptureQuality quality)		{ Quality = quality; }
	void						setResolution(eCzCameraCaptureSize resolution)	{ Resolution = resolution; }
	eCzCameraCaptureSize		getResolution() const			{ return Resolution; }
	void						setFrontFacing() 				{ CameraType = CCT_Front; }
	void						setRearFacing()					{ CameraType = CCT_Rear;  }
	eCzCameraType				getCameraType() const			{ return CameraType; }
	CzEventManager*				getEventsManager()				{ return EventsManager; }
	// Properties end

public:
	CzVideoCam() : IzXomlResource(), Quality(CCQ_Medium), Resolution(CCS_Medium), CameraType(CCT_Rear)
	{
		setClassType("videocam");
		EventsManager = new CzEventManager();
	}
	virtual ~CzVideoCam()
	{
		Stop();
/*		if (PLATFORM_CAM != NULL)
		{
			PLATFORM_CAM->Release();
		}*/
		SAFE_DELETE(EventsManager);
	}

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	void				Stop();
	bool				Start();

	// Event handlers
	virtual void		ProcessEventActions(unsigned int event_name, IzXomlResource* parent);
};

/**
 @class	CzVideoCamCreator

 @brief	Creates an instance of a VideoCam.

 */

class CzVideoCamCreator : public IzXomlClassCreator
{
public:
	CzVideoCamCreator()
	{
		setClassName("videocam");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzVideoCam(); }
};




#endif // _CZ_CAM_H_
