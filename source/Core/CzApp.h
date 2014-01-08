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

#if !defined(_CZ_APP_H_)
#define _CZ_APP_H_

/**
 @def	CZ_VERSION

 @brief	Current AppEasy Version.
 */
#define	CZ_VERSION		0x00410

#include "CzUtil.h"
#include "CzActor.h"
#include "CzScene.h"
#include "CzCamera.h"
#include "CzInput.h"
#include "CzAudio.h"
#include "CzXoml.h"
#include "CzResources.h"

/**
 @addtogroup Core
 @{
 */
 
class CzUIBase;

/**
 @class	CzApp

 @brief	CzApp acts as a controller for the whole game or app. You should derive your own version of this class.

 <h1>Introduction</h1>
 CzApp represents your app and manages all of the internals for you including the following:
 - Like Scenes manage Actors the CzApp class manages scenes
 - Managing and updating hardware sub systems such as user input, http, audio etc..
 - Managing and updating global resources
 - Stabilising frame rate
 - Watching for and notifying scenes about screen orientation / size change events
 - Layering and drawing scenes

 czApp is also a singleton so you can only ever have one instance of CzApp created and running at one time during your apps life time. It also makes it accessible from anywhere within your app.

 When you first begin using AppEasy Core your first job will be to create your own class that is derived from CzApp and implement the following methods:
 - Init() - App initialisation
 - Release() - App cleanup
 - Update() - Per frame app update
 - Draw() - Per frame app rendering

 <h1>Creating our own App</h1>
 Here is a short example showing how to derive your own CzApp Class:

 <b>App.h</b>
 @code
class App : public CzApp
{
	CDEFINE_SINGLETON(App)

public:
	void			Init(bool enable_http);
	void			Release();
	bool			Update();
	void			Draw();
};
 @endcode

 <b>App.cpp</b>
 @code
#include "App.h"

#define	APP	App::getInstance()	// Macros is used to provide more efficient access to the app singleton

CDECLARE_SINGLETON(App)

void App::Init(bool enable_http)
{
	CzApp::Init(enable_http);	// Init base
}

void App::Release()
{
	CzApp::Release();	// Release
}

bool App::Update()
{
	if (!CzApp::Update())	// Update the app
		return false;

	return true;
}

void App::Draw()
{
	CzApp::Draw();	// Draw the app
}
 @endcode

 And finally we create a main.cpp that creates and runs our app:

 <br>Main.cpp</b>
 @code
// App headers
#include "App.h"

// Include platform
#include "CzPlatform.h"

int main()
{
	CzPlatform_Create();	// Init platform

	App::Create();			// Init App
	APP->Init(true);

	while (!PLATFORM_SYS->CheckAppQuit())	// Main Loop
	{
		if (!APP->Update())	// Update App
			break;

		APP->Draw();		// Draw App
	}

	APP->Release();			// Shut down App
	App::Destroy();

	CzPlatform_Destroy();	// Shut down platform

	return 0;
}
 @endcode

 This style of init, update, draw, cleanup is used throughuot all AppEasy Core examples.

 <h1>System Variables</h1>
 CzApp handles all global resources as well as the global system variables which include:
 - system - System related variables sych as screen height and width
 - touches - Touch info for each available screen touch

 The system variables array is a special XOML array that contains information about the device that your app is running on. The array supports querying of the following information:
 - 0 - Display width
 - 1 - Display height
 - 2 - Size hint - This is a hint that can be used to separate resources into groups that can be used across different sized screens. The calculation is based on (width + height ) / 400
 - 3 - Standard graphics display mode name, for example HVGA, QXGVA etc..
 - 4 - Device type
  - Unsupported = -1
  - iPhone = 0
  - iPad = 1
  - Android = 2
  - Bada = 3
  - QNX = 4
  - Symbian = 5
  - WinMobile = 6
  - WebOS = 7
  - Windows = 8
  - OSX = 9
 - 5 - Multi-touch - If set to 1 then device supports multi-touch
 - 6 - Accelerometer - If set to 1 then device supports accelerometer
 - 7 - Compass - If set to 1 then device supports compass
 - 8 - Keys - If set to 1 then device supports keyboard
 - 9 - HasPointer - If set to 1 then the device has touch screen / pointer support
 - 10 – Last purchase ID – Contains the ID of the last item purchased via the Market, -1 for none

 Using the system array you can style your app to fit different device configurations.

 The system touches variables array is a special array that contains information about the position and state of up to 5 touches on the device. The array supports querying of the following information:
 - 0 – Touch 1 position x
 - 1 – Touch 1 position y
 - 2 – Touch 1 status (1 for touching, 0 for not touching)
 - 3 – Touch 2 position x
 - 4 – Touch 2 position y
 - 5 – Touch 2 status (1 for touching, 0 for not touching)
 - 6 – Touch 3 position x
 - 7 – Touch 3 position y
 - 8 – Touch 3 status (1 for touching, 0 for not touching)
 - 9 – Touch 4 position x
 - 10 – Touch 4 position y
 - 11 – Touch 4 status (1 for touching, 0 for not touching)
 - 12 – Touch 5 position x
 - 13 – Touch 5 position y
 - 14 – Touch 5 status (1 for touching, 0 for not touching)

 An example condition that checks for touch 1:
 
 @code
 <Variable Name="Touched1" Type="condition" Value="touches:2 == 1" />
 @endcode

 In this example Touched1 will evaluate to true if the user touches the screen

 System variables provide an easy way for XOML to change layouts and behaviours based on different system configurations.

 */
 
/**
 @def	CZ_MAX_SCENE_LAYERS

 @brief	Default maximum scene layers.

 */
#define CZ_MAX_SCENE_LAYERS				10

class CzApp : public IzXomlResource
{
public:

	/**
	 @enum	eSysArrayIndex
	
	 @brief	Indices into the XOML system array
	 */

	enum eSysArrayIndex
	{
		SYS_SCREEN_WIDTH			= 0,	///< Current screen width
		SYS_SCREEN_HEIGHT			= 1,	///< Current screen height
		SYS_SCREEN_MODE				= 2,	///< Graphics mode, see CzUtils.cpp for list of modes
		SYS_SCREEN_SIZE_HINT		= 3,	///< Screen size hint (banded using (width + height) / 400 calculation to give rough idea of what size components such as fonts to use)
		SYS_DEVICE_TYPE				= 4,	///< Device type, see CzUtils.h for list of types
		SYS_HAS_MULTITOUCH			= 5,	///< Supports multi-touch
		SYS_HAS_ACCELEROMETER		= 6,	///< Supports accelerometer
		SYS_HAS_COMPASS				= 7,	///< Supports compass
		SYS_HAS_KEYS				= 8,	///< Supports keys
		SYS_HAS_POINTER				= 9,	///< Supports pointer
		SYS_LAST_PURCHASE_ID		= 10,	///< ID of last purchase attempt
		SYS_SCREEN_ORIENTATION		= 11,	///< Screen orientation in degrees
		SYS_MAX						= 12	///< Maximum array indices
	};

	/**
	 @enum	eSysTouchesArrayIndex
	
	 @brief	Indices into the XOML touches system array
	 */

	enum eSysTouchesArrayIndex
	{
		TOUCH_POS_X0				= 0, 	///< Touch x-position 0
		TOUCH_POS_Y0				= 1, 	///< Touch y-position 0
		TOUCH_STATE0				= 2, 	///< Touch state 0
		TOUCH_POS_X1				= 3, 	///< Touch x-position 0
		TOUCH_POS_Y1				= 4, 	///< Touch y-position 0
		TOUCH_STATE1				= 5, 	///< Touch state 1
		TOUCH_POS_X2				= 6, 	///< Touch x-position 0
		TOUCH_POS_Y2				= 7, 	///< Touch y-position 0
		TOUCH_STATE2				= 8, 	///< Touch state 2
		TOUCH_POS_X3				= 9, 	///< Touch x-position 0
		TOUCH_POS_Y3				= 10,	///< Touch y-position 0
		TOUCH_STATE3				= 11,	///< Touch state 3
		TOUCH_POS_X4				= 12,	///< Touch x-position 0
		TOUCH_POS_Y4				= 13,	///< Touch y-position 0
		TOUCH_STATE4				= 14,	///< Touch state 4
		TOUCH_MAX					= 15	///< Maximum array indices
	};

	// Public access for scene iteration
	typedef CzVector<CzScene*>::iterator	_Iterator;
	_Iterator					begin() { return Scenes.begin(); }
	_Iterator					end() { return Scenes.end(); }

protected:
	// Properties
	CzIVec2					ScreenSize;								///< Native screen size
	int						ScreenOrientation;						///< Orientation of screen in degrees
	CzScene*				CurrentScene;							///< Scene that has current input focus
	CzScene*				NextScene;								///< Scene that we wish to switch focus to
	CzVector<CzScene*>		Scenes;									///< A collection of game scenes
	CzActor*				TouchFocus[CZ_MAX_TOUCHES];				///< Actor that has the current touch focus
	CzActor*				LastTappedActor[CZ_MAX_TOUCHES];		///< Actor that was last tapped
	CzUIBase*				KeyFocus;								///< Actor that has the current key focus
	CzColour				BackgroundColour;						///< Background clear colour
	bool					TouchPanEnabled;						///< Global touch pan enabled
	int						MaxTouches;								///< Maximum number of tuoches to support
	bool					GLCompatible;							///< Determine if this game should be compatible with OpenGL commands
	float					FrameRate;								///< Estimated frame rate in frames per second
	unsigned int			TotalFrames;							///< Total number of frames that have been processed since the app began
	bool					TimePaused;								///< Pauses time of set to true

public:
	void					addScene(CzScene *scene, bool bring_to_front = true);
	void					removeScene(CzScene* scene);
	void					removeScene(unsigned int name_hash);
	void					removeAllScenes(const CzVector<CzScene*>& exclude_scenes);
	int						findScenesOfType(int type, CzVector<CzScene*>& scenes);
	CzScene*				findScene(unsigned int name_hash);
	CzScene*				findScene(const char* name);
	CzScene*				findScene(int type);
	CzScene*				getScene(int index);
	void					clearScenes();
	void					changeScene(CzScene *new_scene);
	bool					changeScene(unsigned int name_hash);
	CzScene*				getCurrentScene()						{ return CurrentScene; }
	void					setTouchFocus(int index, CzActor* focus)	{ TouchFocus[index] = focus; }
	CzActor*				getTouchFocus(int index)				{ return TouchFocus[index]; }
	void					releaseTouchFocus(int index, int x, int y, bool allow_bubble = true);
	CzActor*				getLastTappedActor(int index)			{ return LastTappedActor[index]; }
	void					setKeyFocus(CzUIBase* actor)			{ KeyFocus = actor; }
	CzUIBase*				getKeyFocus()							{ return KeyFocus; }
	void					setTouchPanEnabled(bool enabled)		{ TouchPanEnabled = enabled; }
	bool					isTouchPanEnabled() const				{ return TouchPanEnabled; }
	void					setMaxTouches(int max)					{ MaxTouches = max; }
	int						getMaxTouches() const					{ return MaxTouches; }
	void					setGLCompatible(bool enable)			{ GLCompatible = enable; }
	bool					getGLCompatible() const					{ return GLCompatible; }
	const CzIVec2&			getScreenSize() const					{ return ScreenSize; }
	int						getScreenOrientation() const			{ return ScreenOrientation; }
	float					getFrameRate() const					{ return FrameRate; }
	unsigned int			getTotalFrames() const					{ return TotalFrames; }
	void					setTimePaused(bool paused);
	bool					isTimePaused() const					{ return TimePaused; }
	// Properties end

protected:
	void					CreateSystemArray();	// Creates an array that contains system variables that user can access from XOML
	CzList<CzScene*>		SceneRemovals;			// Scenes that were removed during this game frame
	uint64					LastFrameTime;			// The time at which the last frame ended
	void					ProcessTouch(int touch);
	void					ProcessEvents(CzScene *scane);
	CzActor*				FindTappedActor(int x, int y);
	void					UpdateSystemTouches();
	void					GetAppEasySettings();
	CzSlotArray<CzScene*>	SceneList;				// Temp scene list, stored here to provent many allocs / frees

public:
	virtual void			Init(bool enable_http = false, bool gl_conmpatible = false);
	virtual void			Release();
	virtual bool			Update();
	virtual void			Draw();
	virtual void			PostDraw();			// Called by Draw() after all rendering is finished, but before the frame bugger is swapped
	virtual void			Save() {}
	virtual void			Load() {}

public:
	void					SetBackgroundColour(uint8 r, uint8 g, uint8 b, uint8 a);
	void					BringSceneToFront(CzScene* scene);
	void					DisableFocus();									// Disables focus for all scenes
	void					SetAllTimelines(CzAnimTimeline* timeline);		// Sets the timelines of all active scenes

	// Implementation of IzXomlClass interface
	int						LoadFromXoml(IzXomlResource* parebt, bool load_children, CzXmlNode* node);

	// Event handlers
	virtual void			NotifyOrientationChanged();						// This event is called when the app receives an orientation changed event
	virtual void			NotifySizeChanged();							// This event is called when the app window resizes

};

class CzAppCreator : public IzXomlClassCreator
{
public:
	CzAppCreator()
	{
		setClassName("app");
	}
	IzXomlResource* CreateInstance(IzXomlResource* parent);
};

/// @}

#endif // _CZ_APP_H_
