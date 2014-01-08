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

#if !defined(_CIZ_PLATFORM_OTHER_H_)
#define _CIZ_PLATFORM_OTHER_H_

#include "CzPlatform.h"

#define	PLATFORM_OTHER		PLATFORM->getOther()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IzPlatformOther
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IzPlatformOther
{
	// Properties
protected:
	bool		Initialised;
public:
	bool		isInitialised() const	{ return Initialised; }
	// Properties end
public:
	virtual int Init() = 0;
	virtual void Release() = 0;

	// Vibration
	virtual void VibrateStart(float priority, float duration) = 0;
	virtual void VibrateStop() = 0;



	// Marmalade dependencies - TODO: Remove dependency
	virtual void InitResManager() = 0;
	virtual void ReleaseResManager() = 0;
};


#endif	// _CIZ_PLATFORM_OTHER_H_
