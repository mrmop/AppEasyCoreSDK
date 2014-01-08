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

#include "Marm/CzPlatformInput.h"
#include "CzInput.h"
#include "s3e.h"
#include "s3eOSReadString.h"
#include "IwGeom.h"
#include "s3ePointer.h"
#include "s3eKeyboard.h"
#include "s3eCompass.h"
#include "s3eOSReadString.h"


// 
//
// Key code mapping table - maps platform keys to AppEasy keys
//
//
//
uint8 CzPlatformInput::AppEasyToPlatformKeyCodeMapping[] = 
{
    s3eKeyFirst, 
    s3eKeyEsc, 
    s3eKeyTab, 
    s3eKeyBackspace, 
    s3eKeyEnter, 
    s3eKeyLeftShift, 
    s3eKeyLeftControl, 
    s3eKeyReserved, 
    s3eKeySpace, 
    s3eKeyLeft, 
    s3eKeyUp, 
    s3eKeyRight, 
    s3eKeyDown, 
    s3eKey0, 
    s3eKey1, 
    s3eKey2, 
    s3eKey3, 
    s3eKey4, 
    s3eKey5, 
    s3eKey6, 
    s3eKey7, 
    s3eKey8, 
    s3eKey9, 
    s3eKeyA, 
    s3eKeyB, 
    s3eKeyC, 
    s3eKeyD, 
    s3eKeyE, 
    s3eKeyF, 
    s3eKeyG, 
    s3eKeyH, 
    s3eKeyI, 
    s3eKeyJ, 
    s3eKeyK, 
    s3eKeyL, 
    s3eKeyM, 
    s3eKeyN, 
    s3eKeyO, 
    s3eKeyP, 
    s3eKeyQ, 
    s3eKeyR, 
    s3eKeyS, 
    s3eKeyT, 
    s3eKeyU, 
    s3eKeyV, 
    s3eKeyW, 
    s3eKeyX, 
    s3eKeyY, 
    s3eKeyZ, 
    s3eKeyF1, 
    s3eKeyF2, 
    s3eKeyF3, 
    s3eKeyF4, 
    s3eKeyF5, 
    s3eKeyF6, 
    s3eKeyF7, 
    s3eKeyF8, 
    s3eKeyF9, 
    s3eKeyF10, 
    s3eKeyNumPad0, 
    s3eKeyNumPad1, 
    s3eKeyNumPad2, 
    s3eKeyNumPad3, 
    s3eKeyNumPad4, 
    s3eKeyNumPad5, 
    s3eKeyNumPad6, 
    s3eKeyNumPad7, 
    s3eKeyNumPad8, 
    s3eKeyNumPad9, 
    s3eKeyNumPadPlus, 
    s3eKeyNumPadMinus, 
    s3eKeyNumPadEnter, 
    s3eKeyRSK, 
    s3eKeyLSK, 
    s3eKeyLS, 
    s3eKeyRS, 
    s3eKeyHash, 
    s3eKeyStar, 
    s3eKeyOk, 
    s3eKeyCLR, 
    s3eKeyVolUp, 
    s3eKeyVolDown, 
    s3eKeyCamera, 
    s3eKeyMic, 
    s3eKeyFn, 
    s3eKeySym, 
    s3eKeyAccept, 
    s3eKeyEnd, 
    s3eKeyHomePage, 
    s3eKeyButton1, 
    s3eKeyButton2, 
    s3eKeyButton3, 
    s3eKeyButton4, 
    s3eKeyButton5, 
    s3eKeyButton6, 
    s3eKeyButton7, 
    s3eKeyButton8, 
    s3eKeyF11, 
    s3eKeyF12, 
    s3eKeyLeftAlt, 
    s3eKeyRightControl, 
    s3eKeyRightAlt, 
    s3eKeyRightShift, 
    s3eKeyBacktick, 
    s3eKeyComma, 
    s3eKeyPeriod, 
    s3eKeySlash, 
    s3eKeyBackSlash, 
    s3eKeySemicolon, 
    s3eKeyApostrophe, 
    s3eKeyLeftBracket, 
    s3eKeyRightBracket, 
    s3eKeyEquals, 
    s3eKeyMinus, 
    s3eKeyCapsLock, 
    s3eKeyNumPadPeriod, 
    s3eKeyNumPadSlash, 
    s3eKeyNumLock, 
    s3eKeyInsert, 
    s3eKeyHome, 
    s3eKeyPageUp, 
    s3eKeyPageDown, 
    s3eKeyKbEnd, 
    s3eKeyDelete, 
    s3eKeyPause, 
    s3eKeyAt, 
    s3eKeyBack, 
    s3eKeyMenu, 
    s3eKeySearch, 
    s3eKey3DMode, 
    s3eKeyMyApps, 
    s3eKeyAbsGameA,
    s3eKeyAbsGameB,
    s3eKeyAbsGameC,
    s3eKeyAbsGameD,
    s3eKeyAbsUp,
    s3eKeyAbsDown,
    s3eKeyAbsLeft,
    s3eKeyAbsRight,
    s3eKeyAbsOk,
    s3eKeyAbsASK,
    s3eKeyAbsBSK,
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
//
// Input callback handlers
//
//
//
// HandleMultiTouchButtonCB - For multitouch devices the system will call this callback when the user touches the screen. This callback is called once for each screen touch
// 
void HandleMultiTouchButtonCB(s3ePointerTouchEvent* event)
{
	if (CZ_INPUT == NULL)
		return;

	// Check to see if the touch already exists
//	CzTouch* touch = CZ_INPUT->findTouch(event->m_TouchID);
	CzTouch* touch = CZ_INPUT->getTouch(event->m_TouchID);
	if (touch != NULL)
	{
		// Yes it does, so update the touch information
		touch->prev_active = touch->active;
		touch->active = event->m_Pressed != 0; 
		touch->x = event->m_x;
		touch->y = event->m_y;
		touch->px = event->m_x;
		touch->py = event->m_y;
		touch->id = event->m_TouchID;
	}
}
//
// HandleMultiTouchMotionCB - For multitouch devices the system will call this callback when the user moves their finger on the screen. This callback is called once for each screen touch
// 
void HandleMultiTouchMotionCB(s3ePointerTouchMotionEvent* event)
{
	if (CZ_INPUT == NULL)
		return;

	// Check to see if the touch already exists
//	CzTouch* touch = CZ_INPUT->findTouch(event->m_TouchID);
	CzTouch* touch = CZ_INPUT->getTouch(event->m_TouchID);
	if (touch != NULL)
	{
		// Updates the touches positional information
		touch->x = event->m_x;
		touch->y = event->m_y;
	}
}
//
// HandleSingleTouchButtonCB - The system will call this callback when the user touches the screen
// 
void HandleSingleTouchButtonCB(s3ePointerEvent* event)
{
	if (CZ_INPUT == NULL)
		return;

	CzTouch* touch = CZ_INPUT->getTouch(0);
	touch->prev_active = touch->active;
	touch->active = event->m_Pressed != 0;
	touch->x = event->m_x;
	touch->y = event->m_y;
	touch->px = event->m_x;
	touch->py = event->m_y;
}
//
// HandleSingleTouchMotionCB - The system will call this callback when the user moves their finger on the screen
// 
void HandleSingleTouchMotionCB(s3ePointerMotionEvent* event)
{
	if (CZ_INPUT == NULL)
		return;

	CzTouch* touch = CZ_INPUT->getTouch(0);
	touch->x = event->m_x;
	touch->y = event->m_y;
}

//
// Marmalade Input system
//
bool	CzPlatformInput::isPointerAvailable()				{ return s3ePointerGetInt(S3E_POINTER_AVAILABLE) ? true : false; }
bool	CzPlatformInput::isMultiTouch()						{ return s3ePointerGetInt(S3E_POINTER_MULTI_TOUCH_AVAILABLE) ? true : false; }
bool	CzPlatformInput::isKeysAvailable()					{ return ((s3eKeyboardGetInt(S3E_KEYBOARD_HAS_KEYPAD) || s3eKeyboardGetInt(S3E_KEYBOARD_HAS_ALPHA))) ? true : false; }
bool	CzPlatformInput::isOSKeyboardAvailable()			{ return s3eOSReadStringAvailable() == S3E_TRUE; }
bool	CzPlatformInput::isAccelerometerAvailable()			{ return s3eAccelerometerGetInt(S3E_ACCELEROMETER_AVAILABLE) == S3E_TRUE; }
bool	CzPlatformInput::isCompassAvailable()				{ return s3eCompassAvailable() == S3E_TRUE; }
bool	CzPlatformInput::wasBackButtonReleased()			{ return (s3eKeyboardGetState(s3eKeyBack) & S3E_KEY_STATE_RELEASED) == S3E_KEY_STATE_RELEASED || (s3eKeyboardGetState(s3eKeyAbsBSK) & S3E_KEY_STATE_RELEASED) == S3E_KEY_STATE_RELEASED; }
bool	CzPlatformInput::wasMenuButtonReleased()			{ return (s3eKeyboardGetState(s3eKeyMenu) & S3E_KEY_STATE_RELEASED) == S3E_KEY_STATE_RELEASED || (s3eKeyboardGetState(s3eKeyAbsASK) & S3E_KEY_STATE_RELEASED) == S3E_KEY_STATE_RELEASED; }
void	CzPlatformInput::getAccelerometer(CzVec3& data)		{ data.x = (float)s3eAccelerometerGetX(); data.y = (float)s3eAccelerometerGetY(); data.z = (float)s3eAccelerometerGetZ(); }
float	CzPlatformInput::getCompassDirection()				{ return (float)s3eCompassGet(); }

int		CzPlatformInput::Init()
{
	if (g_CzShowTracing > 1)
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Registering touch callbacks");

	// For multi-touch devices we handle touch and motion events using different callbacks
	if (isMultiTouch())
	{
		s3ePointerRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)HandleMultiTouchButtonCB, NULL);
		s3ePointerRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)HandleMultiTouchMotionCB, NULL);
	}
	else
	{
		s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleSingleTouchButtonCB, NULL);
		s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleSingleTouchMotionCB, NULL);
	}

	Initialised = true;

	return 1;
}

void	CzPlatformInput::Release()
{
	if (isPointerAvailable())
	{
		// Unregister the pointer system callbacks
		if (isMultiTouch())
		{
			s3ePointerUnRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)HandleMultiTouchButtonCB);
			s3ePointerUnRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)HandleMultiTouchMotionCB);
		}
		else
		{
			s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleSingleTouchButtonCB);
			s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleSingleTouchMotionCB);
		}
	}
}

bool	CzPlatformInput::Update()
{
	// Update the pointer if it is available
	if (isPointerAvailable())
		s3ePointerUpdate();

	// Update key system if it is available
	s3eKeyboardUpdate();

	return true;
}

bool	CzPlatformInput::startAccelerometer()
{
	if (!isAccelerometerAvailable())
		return false;

	if (s3eAccelerometerStart() == S3E_RESULT_SUCCESS)
		return true;

	return false;
}

void	CzPlatformInput::stopAccelerometer()
{
	if (!isAccelerometerAvailable())
		return;

	s3eAccelerometerStop();
}

bool	CzPlatformInput::startCompass()
{
	if (!isCompassAvailable())
		return false;

	if (s3eCompassStart() == S3E_RESULT_SUCCESS)
		return true;

	return false;
}

void	CzPlatformInput::stopCompass()
{
	if (!isCompassAvailable())
		return;

	s3eCompassStop();
}

bool	CzPlatformInput::getCompassHeading(CzVec3& data)
{
	s3eCompassHeading heading = { 0, 0, 0 };
	if (s3eCompassGetHeading(&heading) == S3E_RESULT_SUCCESS)
	{
		data.x = (float)heading.m_X;
		data.y = (float)heading.m_Y;
		data.z = (float)heading.m_Z;

		return true;
	}

	return false;
}

const char*	CzPlatformInput::OsKeyboardRead(const char* prompt, const char* default_text, int flags)
{
	if (!isOSKeyboardAvailable())
		return NULL;

	switch (flags)
	{
	case CzInput::Hint_Text:		flags = 0; break;
	case CzInput::Hint_Number:		flags = S3E_OSREADSTRING_FLAG_NUMBER; break;
	case CzInput::Hint_Password:	flags = S3E_OSREADSTRING_FLAG_PASSWORD; break;
	case CzInput::Hint_Email:		flags = S3E_OSREADSTRING_FLAG_EMAIL; break;
	case CzInput::Hint_URL:			flags = S3E_OSREADSTRING_FLAG_URL; break;
	}

	// Show on screen keyboard and return the input string
	if (default_text != NULL)
		return s3eOSReadStringUTF8WithDefault(prompt, default_text, flags);
	else
		return s3eOSReadStringUTF8(prompt, flags);
}

uint8*	CzPlatformInput::getKeyMappingTable()
{
	return AppEasyToPlatformKeyCodeMapping;
}

bool	CzPlatformInput::isKeyDown(int key)
{
	if (!isKeysAvailable())
		return false;

	return (s3eKeyboardGetState((s3eKey)key) & S3E_KEY_STATE_DOWN) == S3E_KEY_STATE_DOWN;
}

bool	CzPlatformInput::isKeyUp(int key)
{
	if (!isKeysAvailable())
		return false;

	return (s3eKeyboardGetState((s3eKey)key) & S3E_KEY_STATE_UP) == S3E_KEY_STATE_UP;
}

bool	CzPlatformInput::wasKeyPressed(int key)
{
	if (!isKeysAvailable())
		return false;

	return (s3eKeyboardGetState((s3eKey)key) & S3E_KEY_STATE_PRESSED) == S3E_KEY_STATE_PRESSED;
}

bool	CzPlatformInput::wasKeyReleased(int key)
{
	if (!isKeysAvailable())
		return false;

	return (s3eKeyboardGetState((s3eKey)key) & S3E_KEY_STATE_RELEASED) == S3E_KEY_STATE_RELEASED;
}

int		CzPlatformInput::getLastKey() const
{
	return PlatformToAppeasyKeyCodeMapping[s3eKeyboardAnyKey()];	
}







