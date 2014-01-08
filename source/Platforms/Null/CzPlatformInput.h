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

#if !defined(_CCZ_PLATFORM_INPUT_H_)
#define _CCZ_PLATFORM_INPUT_H_

#include "IzPlatformInput.h"
#include "CzUtil.h"

class CzPlatformInput : public IzPlatformInput
{
	// Properties
protected:
	static uint8 AppEasyToPlatformKeyCodeMapping[];
	static uint8 PlatformToAppeasyKeyCodeMapping[];

public:
	// Properties end
public:
	// Init
	int		Init();
	void	Release();
	// Feature query
	bool	isPointerAvailable();
	bool	isMultiTouch();
	bool	isKeysAvailable();
	bool	isOSKeyboardAvailable();
	bool	isAccelerometerAvailable();
	bool	isCompassAvailable();
	// Buttons query
	bool	wasBackButtonReleased();
	bool	wasMenuButtonReleased();
	// Key query
	uint8*	getKeyMappingTable();
	bool	isKeyDown(int key);
	bool	isKeyUp(int key);
	bool	wasKeyPressed(int key);
	bool	wasKeyReleased(int key);
	int		getLastKey() const;
	// Acclerometer
	bool	startAccelerometer();
	void	stopAccelerometer();
	void	getAccelerometer(CzVec3& data);
	// Compass
	bool	startCompass();
	void	stopCompass();
	float	getCompassDirection();
	bool	getCompassHeading(CzVec3& data);
	// OS Keyboard
	const char*	OsKeyboardRead(const char* prompt, const char* default_text, int flags);
	// Update
	bool	Update();
};


#endif	// _CCZ_PLATFORM_INPUT_H_
