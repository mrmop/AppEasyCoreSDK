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

#if !defined(_CZ_INPUT_H_)
#define _CZ_INPUT_H_

#include "IzPlatformInput.h"
#include "CzUtil.h"
#include "CzTime.h"

/**
 @addtogroup Core
 @{
 */
 
/**
 @def	CZ_MAX_TOUCHES

 @brief	A macro that defines maximum number of supported simultaneous touches.

 */

#define CZ_MAX_TOUCHES		10

/**
 @def	CZ_INPUT

 @brief	A macro that provides global access to the intput system.

 */

#define	CZ_INPUT	(CzInput::getInstance())

/**
 @enum	eCzKeyCode

 @brief	Values that represent key codes.
 */

enum eCzKeyCode
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

/**
 @struct	CzTouch

 @brief	Represents a single touch.

 */

struct CzTouch
{
public:
	int			x, y;			///< Touch position
	int			px, py;			///< Previous touch position
	int			dx, dy;			///< Delta position
	bool		touched;		///< Touched state
	bool		active;			///< Touch active state
	bool		prev_active;	///< Touch active state (last frame)
	int			id;				///< ID of touch - The system tracks multiple touches by assigning each one a unique ID

	CzTouch() : active(false), prev_active(false), id(-1), touched(false) {}
};

/**
 @class	CzInput

 @brief	This class is responsible for handling all keyboard, pointer and other input devices input.

 <h1>Introduction</h1>

 AppEasy provides the CzInput singleton class to manage all game input. CzInput manages the following types of input:
 - Single and multi-touch input
 - Button and key states
 - On screen keyboard input
 - Acceleromter
 - Compass

 Access to input methods are provided via the CZ_INPUT macro, for example:

 @code
if (CZ_INPUT->getTouchCount() > 0)
{
}
 @endcode

 If you are using CzApp then you do not need to worry about initialising, updating or cleaning up the input system, however if you are rolling your own solution then you will need to take care 
 of these steps yourself, here's a quick example showing how to do this:

 @code
// Initialise the input system
CzInput::Create();
CZ_INPUT->Init();

// Main loop
while (1)
{
	// Update input system
	CZ_INPUT->Update();
}

// Shut down the input system
CZ_INPUT->Release();
CzInput::Destroy();
 @endcode

 <h1>Checking Availability</h1>
 As AppEasy is designed to work across multiple platforms you should check to ensure that a particular input system is available before you use it. Here’s a quick example showing how to check 
 that the pointer input is available:

 @code
// Check to see that the pointer is available
if (CZ_INPUT->isPointerAvailable())
{
	// Check to see if any touches have been made
	int num_touches = CZ_INPUT->getTouchCount();
}
 @endcode

 AppEasy provides a number of methods to check for particular input systems availability:

 @code
bool	isPointerAvailable() 	// Returns availability of the pointer
bool	isKeysAvailable()    	// Returns availability of keys
bool	isOSKeyboardAvailable() 	// Returns availability of on screen keyboard
bool	isAccelerometerAvailable() // Returns availability of accelerometer
bool	isCompassAvailable() 	// Returns true if compass is available
 @endcode

 <h1>Single and Multi-touch Touches</h1>
 CzInput supports single and multi-touch events, allowing you to check for multiple simultaneous touches. However many devices do not support multi-touch events so a method has been provided 
 to determine multi-touch support:

 @code
bool	isMultiTouch() 		// Returns multitouch capability
 @endcode

 If you are developing a game or app that relies on multi-touch then you should implement a fall back method that will work with single touch devices. Touch modes is a good solution that can 
 help mirror multi-touch functionality by putting the pointer into different modes, such as move, scale, rotate etc.. and allow the user to switch between them.

 No matter if you are using single or multi-touch functionality retrieving touches is done in very much the same way. 

 <h1>Working with Touches</h1>

 CzInput provides methods that enable you to detect and collect touch data. The usual process is to determine if any touches have been made by calling CZ_INPUT->getTouchCount() and then take 
 a look at the touches list to see what touch events occurred. Here's an example:

 @code
// Check to make sure that the pointer is available
if (CZ_INPUT->isPointerAvailable())
{
	// Get the total number of current touches
	int num_touches = CZ_INPUT->getTouchCount();
	if (num_touches != 0)
	{
		// Check list of touches to see which are active
		for (int t = 0; t < CZ_MAX_TOUCHES; t++)
		{
			// Get the touch data
			CzTouch* touch = CZ_INPUT->getTouch(t);
			if (touch->active)
			{
				// Do something with the touch
			}
		}
	}
}
 @endcode

 Note that getTouch() returns the CzTouch struct for the touch at the specified index. CzTouch looks like this:

 @code
struct CzTouch
{
public:
	int			x, y;			///< Touch position
	int			px, py;			///< Previous touch position
	int			dx, dy;			///< Delta position
	bool		touched;		///< Touched state
	bool		active;			///< Touch active state
	bool		prev_active;	///< Touch active state (last frame)
	int			id;				///< ID of touch - The system tracks multiple touches by assigning each one a unique ID

	CzTouch() : active(false), prev_active(false), id(-1), touched(false) {}
};
 @endcode

 If you want to track a touch to monitor its status then you should store its ID and use CzInput::getTouchByID(id) to find it again later. 

 <h1>Checking Key / Button States</h1>

 As you expand your list of supported devices for your products you will discover that devices come in all sorts of different configurations, some will even have hard keyboards / keypads and 
 buttons. For example, the Samsung Galaxy pro has a full QWERTY keyboard and almost all Android devices have hardware buttons for menu, home and back. 

 To query the state of a key / button (buttons are mapped to keys) you call the following methods of CzInput:

  @code
bool	isKeyDown(int key) const;						// Tests if a key is down
bool	isKeyUp(int key) const;							// Tests if a key is up
bool	wasKeyPressed(int key) const;					// Tests if a key was pressed
bool	wasKeyReleased(int key) const;					// Tests if a key was released
 @endcode

 Each method takes a platform independent eCzKeyCode as input.

 <h1>On Screen Keyboard</h1>

 As most devices do not have hardware keyboards an on screen keyboard is the only method of inputting text into the device. AppEasy provides access to this functionality via showOnScreenKeyboard():

 @code
const char*		showOnScreenKeyboard(const char* prompt, int flags = 0, const char* default_text = NULL);
 @endcode

 Calling this method will display a modal on screen keyboard with the provided prompt text and using the supplied default text (pass NULL if you do not require default text). Flags provides a hint 
 to the system to let it know what type of keyboard you want to display to the user (see eInputTypeHint). Passing 0 for flags will use the default keyboard. Once the on screen keyboard has been 
 dismissed the entered text will be returned as a string.

 <h1>Accelerometer Input</h1>

 An accelerometer is a device usually found inside mobile phones and tablets that measures acceleration.  This is great for gaming as you can use the actual angle or speed at which the user tilts 
 their device to affect game play. For example, you could for example use the accelerometer to allow the player to navigate a ball around a maze or maybe determine how hard the player wants to 
 hit a ball. However the accelerometer does have limitations. If the users phone is perpendicular to the floor then changes in reading may not be registered. 

 Accelerometer hardware is usually quite power hungry so in order ot use it you need to start it using:

 @code
CZ_INPUT->startAccelerometer();
 @endcode

 And when not in use you can turn it off using:

 @code
CZ_INPUT->stopAccelerometer();
 @endcode


 Per frame update of the accelerometer is automatically taken care of by CzInput. 

 To read the current position of the accelerometer you call:

 @code
CzVec3 accelerometer_pos = CZ_INPUT->getAccelerometerPosition();
 @endcode

 Because the user can potentially start a game with the phone held at any angle, reading accelerometer readings are best made from a frame of reference. This is usually the initial position that 
 the user is holding the device at when they start the game. To set the reference point for the accelerometer call:

 @code
CZ_INPUT->setAccelerometerReference();
 @endcode

 This will set the reference point for offset reads to the current position of the users phone. You may want to display a short instructions screen at this point that informs the user how to 
 hold the phone.

 To read the accelerometer position with respect to the reference point call:

 @code
CzVec3 accelerometer_pos = CZ_INPUT->getAccelerometerOffset();
 @endcode

 <h1>Compass Input</h1>

 The digital compass is a device that uses the Earth's ambient magnetic field to determine the orientation of the users mobile phone or tablet. This allows you to measure the angle of the device 
 and the direction in which its pointing. 

 Like the accelerometer hardware the compass is usually quite power hungry so in order to use it you need to start it using:

 @code
CZ_INPUT->startCompass();
 @endcode

 And when not in use you can turn it off using:

 @code
CZ_INPUT->stopCompass();
 @endcode

 Per frame update of the compass is automatically taken care of by CzInput. 

 To read the current orientation and heading of the compass you call:

 @code
CzVec3 compass_heading = CZ_INPUT->getCompassHeading();
float compass_directiom = CZ_INPUT->getCompassDirection();
 @endcode

 */

class CzInput
{
public:

	/**
	 @enum	eInputTypeHint
	
	 @brief	Passed to showOnScreenKeyboard() to hint at what type of keyboard entry to show.
	 */
	enum eInputTypeHint
	{
		Hint_Text		= 0, 
		Hint_Number		= 1, 
		Hint_Password	= 2, 
		Hint_Email		= 3, 
		Hint_URL		= 4
	};

	CDEFINE_SINGLETON(CzInput)

	// Properties
private:
	uint8*			KeyCodeMapping;					///< Table that maps platform key codes to AppEasy key codes
	bool			PointerAvailable;				///< true if a pointer is present
	bool			KeysAvailable;					///< true if a key input is present
	bool			OSKeyboardAvailable;			///< true if on screen keyboard is available
	bool			AccelerometerAvailable;			///< true if accelerometer is available
	bool			CompassAvailable;				///< true if compass is available
	bool			IsMultiTouch;					///< true if multitouch is enabled
	CzTouch			Touches[CZ_MAX_TOUCHES];		///< List of potential touches
	CzTouch*		FirstTouch;						///< First touch that was made
	CzIVec2			DragDelta;						///< Amount dragged
	CzIVec2			TouchedPos;						///< Position user touched initially
	bool			Touched;						///< true when user is touching the screen
	bool			Tapped;							///< true when user taps screen (cleared in next update)
	bool			BackPressed;					///< Back key pressed state
	bool			MenuPressed;					///< Menu key pressed state
	bool			AccelerometerActive;			///< Active state of accelerometer
	bool			CompassActive;					///< Active state of compass
	CzVec3			AccelerometerReference;			///< Accelerometer reference position
	CzVec3			AccelerometerPosition;			///< Current accelerometer position
	CzVec3			CompassHeading;					///< Current compass heading
	float			CompassDirection;				///< Current compass direction
public:
	// Availability query
	bool			isPointerAvailable() const		{ return PointerAvailable; }	///< Returns availability of the pointer
	bool			isKeysAvailable() const			{ return KeysAvailable; }		///< Returns availability of keys
	bool			isOSKeyboardAvailable() const	{ return OSKeyboardAvailable; }	///< Returns availability of on screen keyboard
	bool			isAccelerometerAvailable() const { return AccelerometerAvailable; } ///< Returns availability of accelerometer
	bool			isCompassAvailable() const		{ return CompassAvailable; }	///< Returns true if compass is available
	bool			isMultiTouch() const			{ return IsMultiTouch; }		///< Returns multitouch capability

	// Pointer
	CzTouch*		getTouchByID(int id);											///< Returns the touch identified by its id
	CzTouch*		getTouch(int index)				{ if (index < 0 || index >= CZ_MAX_TOUCHES) return NULL; else return &Touches[index]; }		///< Gets a specific touch
	CzTouch*		getFirstTouch()					{ return FirstTouch; }			///< Returns last touch that was made
	CzTouch*		findTouch(int id);												///< Finds a specific touch by its id
	int				getTouchCount() const;											///< Get number of touches this frame
	bool			hasTapped() const				{ return Tapped; }				///< Returns tapped status
	bool			isTouching(int index) const		{ return Touches[index].touched; }							///< Returns true if specified touch at the specified index is touching
	CzIVec2			getTouchedPos(int index) const	{ return CzIVec2(Touches[index].x, Touches[index].y); }		///< Returns the touch position of the touch at the specified index
	CzIVec2			getDragDelta(int index) const	{ return CzIVec2(Touches[index].dx, Touches[index].dy); }	///< Returns tapped status of the touch at the specified index
	bool			isDragging(int index) const;																///< Returns true if the user has moved the touch at the specified index

	// keys / Buttons
	void			setKeyCodeMapping(uint8* mapping_table)	{ KeyCodeMapping = mapping_table; }		///< Sets the hardware to AppEasy key nappnig table
	bool			isKeyDown(int key) const;										///< Tests if a key is down
	bool			isKeyUp(int key) const;											///< Tests if a key is up
	bool			wasKeyPressed(int key) const;									///< Tests if a key was pressed
	bool			wasKeyReleased(int key) const;									///< Tests if a key was released
	const char*		showOnScreenKeyboard(const char* prompt, int flags = 0, const char* default_text = NULL);	//< Displays the on screen keyboard
	bool			isBackPressed()					{ return BackPressed; }			//< Returns true if the back key is pressed
	void			resetBackPressed()				{ BackPressed = false; }		//< Resets the back key pressed status
	bool			isMenuPressed()					{ return MenuPressed; }			//< Returns true if the menu key is pressed
	void			resetMenuPressed()				{ MenuPressed = false; }		//< Resets the home key pressed status
	int				getLastKey() const;												//< Returns the last key pressed

	// Accelerometer
	bool			startAccelerometer();											///< Start accelerometer input
	void			stopAccelerometer();											///< Stop accelerometer input
	void			setAccelerometerReference();									///< Sets the current accelerometer position as a reference posisition 
	CzVec3			getAccelerometerPosition() const { return AccelerometerPosition; }	///< Get current accelerometer position
	CzVec3			getAccelerometerOffset() const;									///< Get current accelerometer offset from the reference position

	// Compass
	bool			startCompass();													///< Start compass input
	void			stopCompass();													///< Stop compass input
	CzVec3			getCompassHeading() const { return CompassHeading; }			///< Get current compass heading
	float			getCompassDirection() const { return CompassDirection; }		///< Get current compass direction (0 to 360 degrees)

	// Properties end

private:
	CzTimer			TapTimer;
	int				TouchID;

public:
	bool			Init();							///< Initialises the input system (returns true if pointer is supported)
	void			Release();						///< Releases data used by the input system
	void			Update();						///< Updates the input system, called every frame
};


/// @}


#endif	// _CZ_INPUT_H_