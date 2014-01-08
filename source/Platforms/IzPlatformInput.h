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

#if !defined(_CIZ_PLATFORM_INPUT_H_)
#define _CIZ_PLATFORM_INPUT_H_

#include "CzPlatform.h"

#define	PLATFORM_INPUT		PLATFORM->getInput()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IzPlatformInput
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IzPlatformInput
{
	// Properties
protected:
	bool		Initialised;
public:
	bool		isInitialised() const	{ return Initialised; }
	// Properties end
public:
	// Init
	virtual int		Init() = 0;
	virtual void	Release() = 0;
	// Feature query
	virtual bool	isPointerAvailable() = 0;
	virtual bool	isMultiTouch() = 0;
	virtual bool	isKeysAvailable() = 0;
	virtual bool	isOSKeyboardAvailable() = 0;
	virtual bool	isAccelerometerAvailable() = 0;
	virtual bool	isCompassAvailable() = 0;
	// Buttons query
	virtual bool	wasBackButtonReleased() = 0;
	virtual bool	wasMenuButtonReleased() = 0;
	// Key query
	virtual uint8*	getKeyMappingTable() = 0;
	virtual bool	isKeyDown(int key) = 0;
	virtual bool	isKeyUp(int key) = 0;
	virtual bool	wasKeyPressed(int key) = 0;
	virtual bool	wasKeyReleased(int key) = 0;
	virtual int		getLastKey() const = 0;
	// Acclerometer
	virtual bool	startAccelerometer() = 0;
	virtual void	stopAccelerometer() = 0;
	virtual void	getAccelerometer(CzVec3& data) = 0;
	// Compass
	virtual bool	startCompass() = 0;
	virtual void	stopCompass() = 0;
	virtual float	getCompassDirection() = 0;
	virtual bool	getCompassHeading(CzVec3& data) = 0;
	// OS Keyboard
	virtual const char*	OsKeyboardRead(const char* prompt, const char* default_text, int flags) = 0;

	// Update
	virtual bool	Update() = 0;
};

#endif	// _CIZ_PLATFORM_INPUT_H_
