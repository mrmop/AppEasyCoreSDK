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

#include "CzPlatformInput.h"
#include "CzInput.h"


// 
//
// Key code mapping table - maps platform keys to AppEasy keys
//
//
//
uint8 CzPlatformInput::AppEasyToPlatformKeyCodeMapping[] = 
{

};

uint8 CzPlatformInput::PlatformToAppeasyKeyCodeMapping[] = 
{
	eKeyNone, 
	eKeyEsc, 
	eKeyTab, 
	eKeyBackspace, 
	eKeyEnter, 
	eKeyLeftShift, 
	eKeyLeftControl, 
	eKeyReserved, 
	eKeySpace, 
	eKeyLeft, 
	eKeyUp, 
	eKeyRight, 
	eKeyDown, 
	eKey0, 
	eKey1, 
	eKey2, 
	eKey3, 
	eKey4, 
	eKey5, 
	eKey6, 
	eKey7, 
	eKey8, 
	eKey9, 
	eKeyA, 
	eKeyB, 
	eKeyC, 
	eKeyD, 
	eKeyE, 
	eKeyF, 
	eKeyG, 
	eKeyH, 
	eKeyI, 
	eKeyJ, 
	eKeyK, 
	eKeyL, 
	eKeyM, 
	eKeyN, 
	eKeyO, 
	eKeyP, 
	eKeyQ, 
	eKeyR, 
	eKeyS, 
	eKeyT, 
	eKeyU, 
	eKeyV, 
	eKeyW, 
	eKeyX, 
	eKeyY, 
	eKeyZ, 
	eKeyF1, 
	eKeyF2, 
	eKeyF3, 
	eKeyF4, 
	eKeyF5, 
	eKeyF6, 
	eKeyF7, 
	eKeyF8, 
	eKeyF9, 
	eKeyF10, 
	eKeyNumPad0, 
	eKeyNumPad1, 
	eKeyNumPad2, 
	eKeyNumPad3, 
	eKeyNumPad4, 
	eKeyNumPad5, 
	eKeyNumPad6, 
	eKeyNumPad7, 
	eKeyNumPad8, 
	eKeyNumPad9, 
	eKeyNumPadPlus, 
	eKeyNumPadMinus, 
	eKeyNumPadEnter, 
	eKeyRSK, 
	eKeyLSK, 
	eKeyLS, 
	eKeyRS, 
	eKeyHash, 
	eKeyStar, 
	eKeyOk, 
	eKeyCLR, 
	eKeyVolUp, 
	eKeyVolDown, 
	eKeyCamera, 
	eKeyMic, 
	eKeyFn, 
	eKeySym, 
	eKeyAccept, 
	eKeyEnd, 
	eKeyHomePage, 
	eKeyButton1, 
	eKeyButton2, 
	eKeyButton3, 
	eKeyButton4, 
	eKeyButton5, 
	eKeyButton6, 
	eKeyButton7, 
	eKeyButton8, 
	eKeyF11, 
	eKeyF12, 
	eKeyLeftAlt, 
	eKeyRightControl, 
	eKeyRightAlt, 
	eKeyRightShift, 
	eKeyBacktick, 
	eKeyComma, 
	eKeyPeriod, 
	eKeySlash, 
	eKeyBackSlash, 
	eKeySemicolon, 
	eKeyApostrophe, 
	eKeyLeftBracket, 
	eKeyRightBracket, 
	eKeyEquals, 
	eKeyMinus, 
	eKeyCapsLock, 
	eKeyNumPadPeriod, 
	eKeyNumPadSlash, 
	eKeyNumLock, 
	eKeyInsert, 
	eKeyHome, 
	eKeyPageUp, 
	eKeyPageDown, 
	eKeyKbEnd, 
	eKeyDelete, 
	eKeyPause, 
	eKeyAt, 
	eKeyBack, 
	eKeyMenu, 
	eKeySearch, 
	eKey3DMode, 
	eKeyMyApps, 
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
    eKeyAbsGameA, 
    eKeyAbsGameB, 
    eKeyAbsGameC, 
    eKeyAbsGameD, 
    eKeyAbsUp, 
    eKeyAbsDown, 
    eKeyAbsLeft, 
    eKeyAbsRight, 
    eKeyAbsOk, 
    eKeyAbsASK, 
    eKeyAbsBSK, 
};

//
// Marmalade Input system
//
bool	CzPlatformInput::isPointerAvailable()				{ return false; }
bool	CzPlatformInput::isMultiTouch()						{ return false; }
bool	CzPlatformInput::isKeysAvailable()					{ return false; }
bool	CzPlatformInput::isOSKeyboardAvailable()			{ return false; }
bool	CzPlatformInput::isAccelerometerAvailable()			{ return false; }
bool	CzPlatformInput::isCompassAvailable()				{ return false; }
bool	CzPlatformInput::wasBackButtonReleased()			{ return false; }
bool	CzPlatformInput::wasMenuButtonReleased()			{ return false; }
void	CzPlatformInput::getAccelerometer(CzVec3& data)		{ }
float	CzPlatformInput::getCompassDirection()				{ return 0; }

int		CzPlatformInput::Init()
{
	Initialised = true;

	return 1;
}

void	CzPlatformInput::Release()
{
}

bool	CzPlatformInput::Update()
{
	// Update the pointer if it is available

	// Update key system if it is available

	return true;
}

bool	CzPlatformInput::startAccelerometer()
{
	if (!isAccelerometerAvailable())
		return false;

	return false;
}

void	CzPlatformInput::stopAccelerometer()
{
	if (!isAccelerometerAvailable())
		return;
}

bool	CzPlatformInput::startCompass()
{
	if (!isCompassAvailable())
		return false;

	return false;
}

void	CzPlatformInput::stopCompass()
{
	if (!isCompassAvailable())
		return;
}

bool	CzPlatformInput::getCompassHeading(CzVec3& data)
{
	return false;
}

const char*	CzPlatformInput::OsKeyboardRead(const char* prompt, const char* default_text, int flags)
{
	return NULL;
}

uint8*	CzPlatformInput::getKeyMappingTable()
{
	return AppEasyToPlatformKeyCodeMapping;
}

bool	CzPlatformInput::isKeyDown(int key)
{
	if (!isKeysAvailable())
		return false;

	return false;
}

bool	CzPlatformInput::isKeyUp(int key)
{
	if (!isKeysAvailable())
		return false;

	return false;
}

bool	CzPlatformInput::wasKeyPressed(int key)
{
	if (!isKeysAvailable())
		return false;

	return false;
}

bool	CzPlatformInput::wasKeyReleased(int key)
{
	if (!isKeysAvailable())
		return false;

	return false;
}

int		CzPlatformInput::getLastKey() const
{
	return 0;
}







