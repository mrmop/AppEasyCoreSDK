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

#include "IzPlatformOther.h"
#include "IzPlatformDisplay.h"
#include "CzApp.h"
#include "CzRender.h"
#include "CzModifier.h"
#include "CzProgram.h"
#include "CzUIBase.h"
#include "CzFile.h"
#include "CzTemplates.h"
#include "CzSettings.h"
#include "CzCam.h"
#include "CzMarket.h"
#include "CzHashes.h"
#include "CzMath.h"

/**
 @fn	int32 CzApp_DeviceSuspend(void* caller, void* data)

 @brief	Device suspend collback

 This callback is called when the app is suspended, usually in response to the user taking a call or power saving pode kicks in.

 @param [in]	caller	If non-null, the caller.
 @param [in]	data  	If non-null, the data.

 @return	.
 */

int32 CzApp_DeviceSuspend(void* caller, void* data)
{
	if (CzGlobals::App == NULL)
		return 0; 

	// Notify all scenes of event
	for (CzApp::_Iterator it = CzGlobals::App->begin(); it != CzGlobals::App->end(); ++it)
		(*it)->NotifyDeviceSuspended();

	return 1;
}

/**
 @fn	int32 CzApp_DeviceResume(void* caller, void* data)

 @brief	Device resume collback

 This callback is called when the app resumes.

 @param [in]	caller	If non-null, the caller.
 @param [in]	data  	If non-null, the data.

 @return	.
 */

int32 CzApp_DeviceResume(void* caller, void* data)
{
	if (CzGlobals::App == NULL)
		return 0; 

	// Notify all scenes of event
	for (CzApp::_Iterator it = CzGlobals::App->begin(); it != CzGlobals::App->end(); ++it)
		(*it)->NotifyDeviceResumed();

	return 1;
}

/**
 @fn	int32 CzApp_DeviceVolumeUp(void* caller, void* data)

 @brief	Device volume up collback

 This callback is called when the user changes the devices volume upwards.

 @param [in]	caller	If non-null, the caller.
 @param [in]	data  	If non-null, the data.

 @return	.
 */

int32 CzApp_DeviceVolumeUp(void* caller, void* data)
{
	if (CzGlobals::App == NULL)
		return 0; 

	// Notify all scenes of event
	for (CzApp::_Iterator it = CzGlobals::App->begin(); it != CzGlobals::App->end(); ++it)
		(*it)->NotifyDeviceVolumeUp();

	return 1;
}

/**
 @fn	int32 CzApp_DeviceVolumeDown(void* caller, void* data)

 @brief	Device volume up collback

 This callback is called when the user changes the devices volume downwards.

 @param [in]	caller	If non-null, the caller.
 @param [in]	data  	If non-null, the data.

 @return	.
 */

int32 CzApp_DeviceVolumeDown(void* caller, void* data)
{
	if (CzGlobals::App == NULL)
		return 0; 

	// Notify all scenes of event
	for (CzApp::_Iterator it = CzGlobals::App->begin(); it != CzGlobals::App->end(); ++it)
		(*it)->NotifyDeviceVolumeDown();

	return 1;
}

/**
 @fn	int32 CzApp_DeviceNetworkOnline(void* caller, void* data)

 @brief	Device network online collback

 This callback is called when the device gains network access

 @param [in]	caller	If non-null, the caller.
 @param [in]	data  	If non-null, the data.

 @return	.
 */

int32 CzApp_DeviceNetworkOnline(void* caller, void* data)
{
	if (CzGlobals::App == NULL)
		return 0; 

	// Notify all scenes of event
	for (CzApp::_Iterator it = CzGlobals::App->begin(); it != CzGlobals::App->end(); ++it)
		(*it)->NotifyDeviceNetworkOnline();

	return 1;
}

/**
 @fn	int32 CzApp_DeviceNetworkOffline(void* caller, void* data)

 @brief	Device network offline collback

 This callback is called when the device loses network access

 @param [in]	caller	If non-null, the caller.
 @param [in]	data  	If non-null, the data.

 @return	.
 */

int32 CzApp_DeviceNetworkOffline(void* caller, void* data)
{
	if (CzGlobals::App == NULL)
		return 0; 

	// Notify all scenes of event
	for (CzApp::_Iterator it = CzGlobals::App->begin(); it != CzGlobals::App->end(); ++it)
		(*it)->NotifyDeviceNetworkOffline();

	return 1;
}

/**
 @fn	void CzApp::setTimePaused(bool paused)

 @brief	Pauses or un-pauses time.

 @param	paused	true to pause, false to un-pause.
 */

void CzApp::setTimePaused(bool paused)
{
	if (!paused)
	{
		// If unpausing time then make last frame time current time, this prevents a massive time jump when returning from paused
		LastFrameTime = PLATFORM_SYS->getTimeInMs();
	}
	TimePaused = paused; 
}


/**
 @fn	void CzApp::Init(bool enable_http, bool gl_conmpatible)

 @brief	Initialises the app

 Init carries out many app start up tasks and is the first method that is called after the singleton has been instantiated. Tasks that Init() carries out include:
 - Sets device callbacks
 - Loads any AppEasy settings located in the AppEasy.xml config file
 - Initialises the AppEasy hardware interface sub systems
 - Initialises global resources system
 - Initialises XOML system
 - Creates the system XOML arrays

 @param	enable_http		 	true to enable, false to disable http support.
 @param	gl_conmpatible   	true to enable, false to disable open GL compatibility mode (not used).
 */

void CzApp::Init(bool enable_http, bool gl_conmpatible)
{
	CzGlobals::App = this;

	// Set platform callbacks
	PLATFORM_SYS->setSuspendedCallback(CzApp_DeviceSuspend);
	PLATFORM_SYS->setResumedCallback(CzApp_DeviceResume);
	PLATFORM_SYS->setVolumeUpCallback(CzApp_DeviceVolumeUp);
	PLATFORM_SYS->setVolumeDownCallback(CzApp_DeviceVolumeDown);
	PLATFORM_SYS->setNetworkOnlineCallback(CzApp_DeviceNetworkOnline);
	PLATFORM_SYS->setNetworkOfflineCallback(CzApp_DeviceNetworkOffline);

	setClassType("Game");
	GLCompatible = gl_conmpatible;
	CurrentScene = NULL;
	NextScene = NULL;
	for (int t = 0; t < CZ_MAX_TOUCHES; t++)
	{
		TouchFocus[t] = NULL;
		LastTappedActor[t] = NULL;
	}
	KeyFocus = NULL;
	TouchPanEnabled = true;
	MaxTouches = 2;
	TotalFrames = 0;
	FrameRate = 0;
	TimePaused = false;

	// Get initial screen size and orientation
	ScreenSize.x = PLATFORM_DISPLAY->getCurrentWidth();
	ScreenSize.y = PLATFORM_DISPLAY->getCurrentHeight();
	ScreenOrientation = PLATFORM_DISPLAY->getOrientation();

	// Retrieve AppEasy settings
	CzSettings::Create();
	CZ_SETTINGS->Init();

	// Initialise the http manager (only if requested and not already initialised)
	if (enable_http && CZ_HTTP_MANAGER == NULL)
	{
		CzHttpManager::Create();
		CZ_HTTP_MANAGER->Init();
	}

	// Initialise the input system
	CzInput::Create();
	CZ_INPUT->Init();

	// Initialise the rsource manager (TODO: Remove support)
	PLATFORM_OTHER->InitResManager();

	// Set default background colour
	SetBackgroundColour(0, 0, 0, 0);

	// Get initial time stamp
	LastFrameTime = PLATFORM_SYS->getTimeInMs();

	// Initialise the global resource system
	CzGlobalResources::Create();
	CZ_GLOBAL_RESOURCES->Init(this);

	// Initialise modifiers system
	CzMods::Create();
	CZ_MODS->Init();

	// Initialise programs system
	CzPrograms::Create();
	CZ_PROGRAMS->Init();

	// Init the XOML system
	CzXoml::Create();
	CZ_XOML->Init();

	// Create system array variables
	CreateSystemArray();

	// Create global UI animations
	CzUIBase::InitPresetAnimations();

	// Seed RNG
	CzMath::SeedRand((unsigned int)PLATFORM_SYS->getTimeUTC());

	// Clear the screen
	PLATFORM_DISPLAY->Clear();
}

/**
 @fn	void CzApp::CreateSystemArray()

 @brief	Creates the system XOML array.

 The system XOML array is accessible from XOML. The system variables array is a special XOML array that contains information about the device that your app is running on. 

 */

void CzApp::CreateSystemArray()
{
	IzPlatformDisplay* display = PLATFORM_DISPLAY;

	// Create the system variable array
	int width = display->getCurrentWidth();
	int height = display->getCurrentHeight();
	CzXomlVariableArrayInt* system = new CzXomlVariableArrayInt();
	system->Init(SYS_MAX);
	system->Name = "system";
	system->setValue(SYS_SCREEN_WIDTH, CzString(width).c_str());
	system->setValue(SYS_SCREEN_HEIGHT, CzString(height).c_str());
	system->setValue(SYS_SCREEN_SIZE_HINT, CzString((width + height) / 400).c_str());
	system->setValue(SYS_SCREEN_MODE, CzString(CzUtils::GetGraphicModeIndex(width, height)).c_str());
	system->setValue(SYS_DEVICE_TYPE, CzString((int)PLATFORM_SYS->getDeviceType()).c_str());
	if (CZ_INPUT->isMultiTouch())
		system->setValue(SYS_HAS_MULTITOUCH, "1");
	else
		system->setValue(SYS_HAS_MULTITOUCH, "0");
	if (CZ_INPUT->isAccelerometerAvailable())
		system->setValue(SYS_HAS_ACCELEROMETER, "1");
	else
		system->setValue(SYS_HAS_ACCELEROMETER, "0");
	if (CZ_INPUT->isCompassAvailable())
		system->setValue(SYS_HAS_COMPASS, "1");
	else
		system->setValue(SYS_HAS_COMPASS, "0");
	if (CZ_INPUT->isKeysAvailable())
		system->setValue(SYS_HAS_KEYS, "1");
	else
		system->setValue(SYS_HAS_KEYS, "0");
	if (CZ_INPUT->isPointerAvailable())
		system->setValue(SYS_HAS_POINTER, "1");
	else
		system->setValue(SYS_HAS_POINTER, "0");
	system->setValue(SYS_LAST_PURCHASE_ID, "");
	system->setValue(SYS_SCREEN_ORIENTATION, CzString(PLATFORM_DISPLAY->getOrientation()).c_str());
	CZ_GLOBAL_RESOURCES->getVariableManager()->addVariable(system);

	// Create the touch input system array
	CzXomlVariableArrayInt* touches = new CzXomlVariableArrayInt();
	touches->Init(TOUCH_MAX);
	touches->Name = "touches";
	for (int t = 0; t < TOUCH_MAX; t++)
		touches->setValue(t, "0");
	CZ_GLOBAL_RESOURCES->getVariableManager()->addVariable(touches);

}

/**
 @fn	void CzApp::UpdateSystemTouches()

 @brief	Updates the system touches XOML array.

 Keeps the system touches array up to date.

 */

void CzApp::UpdateSystemTouches()
{
	CzXomlVariableArrayInt* arr = (CzXomlVariableArrayInt*)CZ_GLOBAL_RESOURCES->getVariableManager()->findVariable("touches");

	// Update system array
	int count = 1;
	if (CZ_INPUT->isMultiTouch())
		count = 5;
	CzString num;
	num.setAutoHash(false);
	int index = 0;
	for (int t = 0; t < 5; t++)
	{
		CzXomlVariableInt* var = (CzXomlVariableInt*)arr->getElement(index++);
		CzTouch* touch = CZ_INPUT->getTouch(t);
		if (touch->x != var->NativeValue)
		{
			var->NativeValue = touch->x;
			num = var->NativeValue;
			var->setValueText(num.c_str());
		}
		var = (CzXomlVariableInt*)arr->getElement(index++);
		if (touch->y != var->NativeValue)
		{
			var->NativeValue = touch->y;
			num = var->NativeValue;
			var->setValueText(num.c_str());
		}
		var = (CzXomlVariableInt*)arr->getElement(index++);
		if ((int)touch->touched != var->NativeValue)
		{
			var->NativeValue = (int)touch->touched;
			num = var->NativeValue;
			var->setValueText(num.c_str());
		}
	}
}

/**
 @fn	void CzApp::SetBackgroundColour(uint8 r, uint8 g, uint8 b, uint8 a)

 @brief	Sets background colour.

 Setsthe apps background colour, which is the colour of the display where nothing is rendered.

 @param	r	The uint8 to process.
 @param	g	The uint8 to process.
 @param	b	The uint8 to process.
 @param	a	The uint8 to process.
 */

void CzApp::SetBackgroundColour(uint8 r, uint8 g, uint8 b, uint8 a)
{
	// Set the default background clear colour
	PLATFORM_DISPLAY->setColour(r, g, b, a);
}

/**
 @fn	void CzApp::Release()

 @brief	Releases the app.

 Release the app, shutting dowen all sub systems, deletnig all scenes etc.. but does not delete the app singelton. To delete the app object call Destroy()

 */

void CzApp::Release()
{
	// Reset preset animations
	CzUIBase::ReleasePresetAnimations();

	// Clean up scenes
	for (_Iterator it = Scenes.begin(); it != Scenes.end(); ++it)
		delete *it;
	Scenes.clear();
	SceneRemovals.clear();

	// Clean up settings
	CZ_SETTINGS->Release();
	CzSettings::Destroy();

	// Cleans up the modifiers system
	CZ_MODS->Release();
	CzMods::Destroy();

	// Cleans up the programs system
	CZ_PROGRAMS->Release();
	CzPrograms::Destroy();

	// Clean up the XOML system
	CZ_XOML->Release();
	CzXoml::Destroy();

#if defined(CZ_ENABLE_METRICS)
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "TotalSpritesProcessed - ", CzString(CzMetrics::TotalSpritesProcessed).c_str());
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "TotalSpritesCreated - ", CzString(CzMetrics::TotalSpritesCreated).c_str());
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "TotalSpritesDestroyed - ", CzString(CzMetrics::TotalSpritesDestroyed).c_str());
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "TotalActorsProcessed - ", CzString(CzMetrics::TotalActorsProcessed).c_str());
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "TotalActorsCreated - ", CzString(CzMetrics::TotalActorsCreated).c_str());
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "TotalActorsDestroyed - ", CzString(CzMetrics::TotalActorsDestroyed).c_str());
#endif

	// Shut down global resource system
	CZ_GLOBAL_RESOURCES->Release();
	CzGlobalResources::Destroy();

	// Shut down the input system
	CZ_INPUT->Release();
	CzInput::Destroy();

	// Shut down the resource manager
	PLATFORM_OTHER->ReleaseResManager();

	// Shut down http manager
	if (CZ_HTTP_MANAGER != NULL)
	{
		CZ_HTTP_MANAGER->Release();
		CzHttpManager::Destroy();
	}

	CzGlobals::App = NULL;
}

/**
 @fn	bool CzApp::Update()

 @brief	Updates the app.

 Updates the app including all sub systems, scenes and their contents etc.. Update() should be called each frame update. Note that Update() also takes care of deleting old unused 
 scenes. When a scene is removed from the app it is not instantly deleted, instead it is marked as destroyed. The scene is deleted by Update() on the next frame. 

 @return	true if it succeeds, false if it fails.
 */

bool CzApp::Update()
{
	IzPlatformDisplay* display = PLATFORM_DISPLAY;

	// If power saving mode disabled then turn back light on
	if (CZ_SETTINGS->getPreventPowerSaving())
		PLATFORM_SYS->BacklightOn();

	// Calculate how long the last game frame took - We use this to scale all of our transient variables that rely upon time so that everything moves at the same rate
	// regardless of our devices frame rate
	float dt = 0;
	if (!TimePaused)
	{
		uint64 current_time = PLATFORM_SYS->getTimeInMs();
		dt = (float)(current_time - LastFrameTime)  / 1000.0f;
		FrameRate = (FrameRate + (1.0f / dt)) / 2.0f;
//		float dt = (float)(PLATFORM_SYS->getTimeInMs() - LastFrameTime) / FRAME_SPEED_LOCK_MS;
//		if (dt < 0.00001f) dt = 0.00001f;
//		if (dt > 10.0f) dt = 10.0f;
		LastFrameTime = current_time;
	}

	// Update input system
	CZ_INPUT->Update();
	UpdateSystemTouches();

	// Check for device orientation / size change
	int screen_width = display->getCurrentWidth();
	int screen_height = display->getCurrentHeight();
	int angle = display->getOrientation();
	if (angle != ScreenOrientation)
	{
		ScreenOrientation = angle;
	}
	
	if (screen_width != ScreenSize.x || screen_height != ScreenSize.y)	// NB: When Marmalade orientation changes it doesnt appear to update screen width and height correctly so we use width and height instead.
	{
		NotifyOrientationChanged();
//		NotifySizeChanged();
	}

	// Update gloabl resource manager
	CZ_GLOBAL_RESOURCES->Update(dt);

	// Update all scene local variables
	for (_Iterator it = Scenes.begin(); it != Scenes.end(); ++it)
		(*it)->getVariableManager()->Update();

	// Run any global programs
	CZ_GLOBAL_RESOURCES->getProgramManager()->Execute(dt);

	// Check for scene change
	// Notify scenes that there is a change of circumstances
	if (CurrentScene != NextScene)
	{
		if (CurrentScene != NULL)
		{
			CurrentScene->NotifyLostFocus(NextScene);
			if (CurrentScene->getAllowSuspend())
				CurrentScene->NotifySuspending(NextScene);
		}
		if (NextScene != NULL)
		{
			NextScene->NotifyGainedFocus(CurrentScene);
			if (NextScene->getAllowSuspend())
				NextScene->NotifyResuming(CurrentScene);
		}
		CurrentScene = NextScene;
	}

	// Update all scenes that are not suspended
	// Note that because actions taken in scenes can change the order of scenes in the Scenes list we need to create a local termporary list that does not get updated
	SceneList.clear();
	for (_Iterator it = Scenes.begin(); it != Scenes.end(); ++it)
		SceneList.add(*it);
	for (int t = 0; t < SceneList.getSize(); t++)
	{
		CzScene* s = SceneList.element_at(t);
		if (s != NULL)
		{
			if (CurrentScene == s)
				s->Update(dt);
			else
			{
				if (!s->getAllowSuspend())
					s->Update(dt);
			}
		}
	}

	// Handle touch event
	if (CurrentScene != NULL)
		ProcessEvents(CurrentScene);

	// Process back key event for key focus actor
	if (KeyFocus != NULL)
	{
		CzScene* focus_scene = KeyFocus->getScene();
		if (focus_scene != NULL && focus_scene->isActive())
		{
			if (CZ_INPUT->isBackPressed())
				KeyFocus->NotifyBackKey();
			if (CZ_INPUT->isMenuPressed())
				KeyFocus->NotifyMenuKey();
		}
	}

	// Update http manager
	if (CZ_HTTP_MANAGER != NULL)
	{
		CZ_HTTP_MANAGER->Update();
	}

	CzAnimTimeline::UpdateRunCount();

	// Update audio
	PLATFORM_AUDIO->Update();

	// Update market 
	if (PLATFORM_MARKET != NULL)
		PLATFORM_MARKET->Update();

	// Remove any scenes that got deleted last frame
	for (CzList<CzScene*>::iterator it = SceneRemovals.begin(); it != SceneRemovals.end(); ++it)
	{
		for (_Iterator it2 = Scenes.begin(); it2 != Scenes.end(); ++it2)
		{
			if (*it2 == *it)
			{
				// If scene was current scene and had touch or key focus then reset the focus
				if (CurrentScene == *it2)
					CurrentScene = NULL;
				for (int t = 0; t < CZ_MAX_TOUCHES; t++)
				{
					if (TouchFocus[t] != NULL && TouchFocus[t]->getScene() == *it2)
						TouchFocus[t] = NULL;
				}
				if (KeyFocus != NULL && KeyFocus->getScene() == *it2)
					KeyFocus = NULL;

				if (NextScene == *it2)
					NextScene = NULL;
				delete *it2;
				Scenes.erase(it2);
				break;
			}
		}
	}
	SceneRemovals.clear();
	TotalFrames++;

	return true;
}

/**
 @fn	CzActor* CzApp::FindTappedActor(int x, int y)

 @brief	Searches for the first tapped actor.

 Searches the current scene and all scenes that are allowed to receive focus to see if the supplied point lies within the boundaries of an actor. If one is found then it is returned. 
 Actors on higher layers will be chosen over actors on lower layers.

 @param	x	The x screen coordinate to test.
 @param	y	The y screen coordinate to test.

 @return	null if none fails, else the found tapped actor.
 */

CzActor* CzApp::FindTappedActor(int x, int y)
{
	CzActor* hit_actor = NULL;
	CzScene* hit = NULL;
	for (_Iterator it = Scenes.begin(); it != Scenes.end(); ++it)
	{
		if ((CurrentScene == *it || (*it)->getAllowFocus()) && (*it)->isActive())
		{
			if (hit == NULL || (hit->getLayer() <= (*it)->getLayer()))
			{
				if (!(*it)->isDragging())
				{
					CzActor* tapped_actor = (*it)->FindTappedActor(x, y);
					CzScene* new_hit = *it;
					if (tapped_actor != NULL)
					{
						hit_actor = tapped_actor;
						hit = *it;
					}
				}
			}
		}
	}

	return hit_actor;
}

/**
 @fn	void CzApp::releaseTouchFocus(int index, int x, int y, bool allow_bubble)

 @brief	Releases the touch focus.

 Release the apps current touch focus. When an actor is touched it is said to have the touch focus. Ordinarily this focus is maintained until the user lifts 
 their finger or drags too  far from the actor. Calling this method will cause the touch focus to be released and the actor that had touch focus to be notified 
 that it should release touch focus via its CzActor::NotifyEndTouch() handler.

 @param	index			Touch index.
 @param	x				The x coordinate.
 @param	y				The y coordinate.
 @param	allow_bubble	true to allow, false to deny event bubbling.
 */

void CzApp::releaseTouchFocus(int index, int x, int y, bool allow_bubble)
{
	if (TouchFocus[index] != NULL)
	{
		if (TouchFocus[index]->getTouchIndex() == index)
		{
			TouchFocus[index]->NotifyEndTouch(index, x, y, allow_bubble);
			TouchFocus[index] = NULL;
		}
	}
}

/**
 @fn	void CzApp::ProcessTouch(int index)

 @brief	Process a touch.

 Processes the specified touch. if the touch has just happened then the actor that was touched will be notified via its CzActor::NotifyBeginTouch() 
 handler. If the touch was actually a release (user taking their finger off the screen) then the focus actor will be notified via its CzActor::NotifyEndTouch() 
 handler. If the user released the touch on an actor then the actor will be notified that the user tapped it via its CzActor::NotifyTapped() handler.

 @param	index			 	Touch index.
 */

void CzApp::ProcessTouch(int index)
{
	CzTouch* touch_info = CZ_INPUT->getTouch(index);
	CzIVec2 pos(touch_info->x, touch_info->y);

	// Handle begin touch event
	if (TouchFocus[index] == NULL)
	{
		if (!touch_info->prev_active && touch_info->active)
		{
			CzActor* tapped_actor = FindTappedActor(pos.x, pos.y);
			if (tapped_actor != NULL)
			{
				tapped_actor->NotifyBeginTouch(index, pos.x, pos.y, true);
				TouchFocus[index] = tapped_actor;
			}
		}
	}
	else
	{
		// If user moves finger too far from actor then release it
		if (TouchFocus[index]->isOutsideFocusRange((float)pos.x, (float)pos.y, 1.0f))
		{
			releaseTouchFocus(index, pos.x, pos.y, true);
		}
	}

	// Handle end touch event (only actor that received begin touch event needs to handle this)
	if (touch_info->prev_active && !touch_info->active)
	{
		if (TouchFocus[index] != NULL)
		{
			if (TouchFocus[index]->HitTest((float)pos.x, (float)pos.y))
			{
				TouchFocus[index]->NotifyTapped(index, pos.x, pos.y, true);
				LastTappedActor[index] = TouchFocus[index];
			}
			TouchFocus[index]->NotifyEndTouch(index, pos.x, pos.y, true);
		}
		TouchFocus[index] = NULL;
	}
}

/**
 @fn	void CzApp::ProcessEvents(CzScene *scene)

 @brief	Processes key and touch events.

 Processes key events for the supplied scene. Also processes touch events for all scenes.

 @param [in]	scene	If non-null, the scene.
 */

void CzApp::ProcessEvents(CzScene *scene)
{
	// Process touches
	for (int t = 0; t < CZ_MAX_TOUCHES; t++)
		LastTappedActor[t] = NULL;
	for (int t = 0; t < MaxTouches; t++)
		ProcessTouch(t);

	if (scene != NULL && scene->isActive() && scene->isVisible())
	{
		// Update keys
		if (CZ_INPUT->isBackPressed())
			scene->NotifyKeyBack();
		if (CZ_INPUT->isMenuPressed())
			scene->NotifyKeyMenu();
	}
}

/**
 @fn	void CzApp::Draw()

 @brief	Draws the app.

 Renders all scenes and their contents that are currently visible. lso performs post draw update, display buffer swapping and OS yielding.

 */

void CzApp::Draw()
{
	PLATFORM_RENDER->Begin();
#if defined(CZ_ENABLE_METRICS)
	CzMetrics::TotalSpritesProcessed = 0;
	CzMetrics::TotalActorsProcessed = 0;
#endif

	// Calculate max layers, also draw any scenes that have negative layers
	int max_layers = 0;
	for (_Iterator it = Scenes.begin(); it != Scenes.end(); ++it)
	{
		CzScene* s = *it;
		if (!s->isDestroyed())
		{
			int layer = s->getLayer();
			if (layer < 0)
				s->Draw();
			else
			if (layer < CZ_MAX_SCENE_LAYERS && layer > max_layers)
				max_layers = layer;
		}
	}

	// Draw all scenes in layer order
	int num_scenes = Scenes.size();
	for (int t = 0; t <= max_layers; t++)
	{
		for (_Iterator it = Scenes.begin(); it != Scenes.end(); ++it)
		{
			CzScene* s = *it;
			if (!s->isDestroyed())
			{
				if (s->getLayer() == t)
					s->Draw();
			}
		}
	}
	// Draw any scenes that have layers that are out of range last
	for (_Iterator it = Scenes.begin(); it != Scenes.end(); ++it)
	{
		CzScene* s = *it;
		if (!s->isDestroyed())
		{
			int layer = s->getLayer();
			if (layer >= CZ_MAX_SCENE_LAYERS)
				s->Draw();
		}
	}

	// Take care of any post draw rendering
	PostDraw();

#if defined(CZ_ENABLE_METRICS)
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "TotalSpritesProcessed - ", CzString(CzMetrics::TotalSpritesProcessed).c_str());
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "TotalSpritesCreated - ", CzString(CzMetrics::TotalSpritesCreated).c_str());
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "TotalSpritesDestroyed - ", CzString(CzMetrics::TotalSpritesDestroyed).c_str());
#endif
	PLATFORM_RENDER->End();

	// Swap display buffers
	PLATFORM_DISPLAY->Swap();

	// Clear the screen
	PLATFORM_DISPLAY->Clear();

	// Yield to the operating system
	PLATFORM_SYS->YieldToOS(0);
}

/**
 @fn	void CzApp::PostDraw()

 @brief	Carries out post drawing.

 */

void CzApp::PostDraw()
{
}

/**
 @fn	void CzApp::addScene(CzScene *scene, bool bring_to_front)

 @brief	Adds a scene to the app.

 Adds the supplied scene to the app. If bring_to_front is true then the scene will be brought to the front of the scene stack (does not change layer ordering).
 
 @param [in]			scene	If non-null, the scene.
 @param	bring_to_front   	true to bring scene to front.
 */

void CzApp::addScene(CzScene *scene, bool bring_to_front)
{
	Scenes.push_back(scene);
	scene->setParent(this);

	// If another scene has focus then bring it back to the top of the scene stack
/*	if (CurrentScene != NULL && !bring_to_front)
	{
		BringSceneToFront(CurrentScene);
	}*/
}

/**
 @fn	void CzApp::removeScene(CzScene* scene)

 @brief	Removes the scene.

 Removes and marks for deletion the supplied scene and all of its contents.

 @param [in]	scene	If non-null, the scene.
 */

void CzApp::removeScene(CzScene* scene)
{
	// Mark scene for removal
	if (!scene->isDestroyed())
	{
		SceneRemovals.push_back(scene);
		scene->setDestroyed(true);
		scene->PreDestroy();
	}
}

/**
 @fn	void CzApp::removeScene(unsigned int name_hash)

 @brief	Removes the named scene.

 Removes and marks for deletion the named scene and all of its contents.

 @param	name_hash	The scene name as a string hash.
 */

void CzApp::removeScene(unsigned int name_hash)
{
	// Find the scene
	for (_Iterator it = Scenes.begin(); it != Scenes.end(); ++it)
	{
		if ((*it)->getNameHash() == name_hash && !(*it)->isDestroyed())
		{
			// Mark scene for removal
			SceneRemovals.push_back(*it);
			(*it)->setDestroyed(true);
			(*it)->PreDestroy();
			break;
		}
	}
}

/**
 @fn	void CzApp::removeAllScenes(const CzVector<CzScene*>& exclude_scenes)

 @brief	Removes all scenes except those specified.
    
 Removes and marks for deletion all scenes in the app except the specified scenes. This method is useful for cleaning up multiple scenes at once.

 @param [in]	exclude_scenes	If non-null, the vector of scenes to exclude.
 */

void CzApp::removeAllScenes(const CzVector<CzScene*>& exclude_scenes)
{
	// Mark scenes for removal
	for (_Iterator it = Scenes.begin(); it != Scenes.end(); ++it)
	{
		bool found = false;
		for (CzVector<CzScene*>::const_iterator it2 = exclude_scenes.begin(); it2 != exclude_scenes.end(); ++it2)
		{
			if (*it == *it2)
				found = true;
		}
		if (!found)
		{
			// Mark scene for removal
			SceneRemovals.push_back(*it);
			(*it)->setDestroyed(true);
			(*it)->PreDestroy();
		}
	}
}

/**
 @fn	int CzApp::findScenesOfType(int type, CzVector<CzScene*>& scenes)

 @brief	builds a list of scenes of the specified type.

 @param	type			  	The scene type.
 @param [out]	scenes	[out] If non-null, the scenes list.

 @return	Number of found scenes.
 */

int CzApp::findScenesOfType(int type, CzVector<CzScene*>& scenes)
{
	for (_Iterator it = Scenes.begin(); it != Scenes.end(); ++it)
	{
		if ((*it)->getType() == type && !(*it)->isDestroyed())
			scenes.push_back(*it);
	}

	return scenes.size();
}

/**
 @fn	CzScene* CzApp::findScene(unsigned int name_hash)

 @brief	Searches the app for a named scene.

 @param	name_hash	The scene name as a string hash.

 @return	null if it fails, else the found scene.
 */

CzScene* CzApp::findScene(unsigned int name_hash)
{
	for (_Iterator it = Scenes.begin(); it != Scenes.end(); ++it)
	{
//		if ((*it)->getNameHash() == name_hash && !(*it)->isDestroyed())
		if ((*it)->getNameHash() == name_hash && !(*it)->isDestroyed())
		{
			return *it;
		}
	}

	return NULL;
}

/**
 @fn	CzScene* CzApp::findScene(const char* name)

 @brief	Searches the app for a named scene.

 @param	name	The scene name.

 @return	null if it fails, else the found scene.
 */

CzScene* CzApp::findScene(const char* name)
{
	return findScene(CzString::CalculateHash(name));
}

/**
 @fn	CzScene* CzApp::findScene(int type)

 @brief	Searches for the first scene of the specified type.

 Searches for the first scene of the specified type. Its often useful to be able to search for as scene by its type, especially if you do not know its name.

 @param	type	The scene type.

 @return	null if it fails, else the found scene.
 */

CzScene* CzApp::findScene(int type)
{
	for (_Iterator it = Scenes.begin(); it != Scenes.end(); ++it)
	{
		if ((*it)->getType() == type)
		{
			return *it;
		}
	}

	return NULL;
}

/**
 @fn	CzScene* CzApp::getScene(int index)

 @brief	Gets a scene.

 Returns the scene at the specified index.

 @param	index	Index of the scene.

 @return	null if it fails, else the scene.
 */

CzScene* CzApp::getScene(int index)
{
	if (index < 0 || index >= (int)Scenes.size())
		return NULL;

	return Scenes[index];
}

void CzApp::clearScenes()
{
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "ERROR: CzApp::clearScenes() is deprecated please use CzApp::removeAllScenes() instead");

/*	for (_Iterator it = Scenes.begin(); it != Scenes.end(); ++it)
		delete *it;

	Scenes.clear();*/
}

/**
 @fn	void CzApp::changeScene(CzScene *new_scene)

 @brief	Change current scene.

 Tells the app to change the current scene to the supplied scene on its next update.

 @param [in]	new_scene	If non-null, the new scene.
 */

void CzApp::changeScene(CzScene *new_scene)
{
	NextScene = new_scene;
}

/**
 @fn	bool CzApp::changeScene(unsigned int name_hash)

 @brief	Change current scene.

 Tells the app to change the current scene to the named scene on its next update.

 @param	name_hash	The scenes name as a string hash.

 @return	true if it succeeds, false if it fails.
 */

bool CzApp::changeScene(unsigned int name_hash)
{
	NextScene = findScene(name_hash);
	if (NextScene == NULL)
		return false;

	return true;
}

/**
 @fn	void CzApp::BringSceneToFront(CzScene* scene)

 @brief	Bring scene to front of scene chain.

 In apps that do not use layers for ordering, its useful to be able to bring a scene to the front of the scene chain displaying it on top of all other scenes.


 @param [in]	scene	If non-null, the scene.
 */

void CzApp::BringSceneToFront(CzScene* scene)
{
	for (_Iterator it = begin(); it != end(); it++)
	{
		if (*it == scene)
		{
			Scenes.erase(it);
			Scenes.push_back(scene);
			break;
		}
	}
}

/**
 @fn	void CzApp::DisableFocus()

 @brief	Disables focus for all scenes.

 Prevents all scenes from receiving input focus. This can be useful if you want to temporarily prevent user input to all scenes.

 */

void CzApp::DisableFocus()
{
	for (_Iterator it = begin(); it != end(); it++)
	{
		(*it)->setAllowFocus(false);
	}
}

/**
 @fn	void CzApp::SetAllTimelines(CzAnimTimeline* timeline)

 @brief	Sets the timeline of all scenes.

 Sets the animation timeline of all scenes to the supplied timeline. This is often useful if you want to fade or scroll all scenes out together.

 @param [in]	timeline	If non-null, the timeline.
 */

void CzApp::SetAllTimelines(CzAnimTimeline* timeline)
{
	for (_Iterator it = begin(); it != end(); it++)
	{
		if (!(*it)->isDestroyed())
		{
			(*it)->setTimeline(timeline);
		}
	}
}

int CzApp::LoadFromXoml(IzXomlResource* parebt, bool load_children, CzXmlNode* node)
{
	// Process inner tags
	CZ_XOML->Process(this, node);

	return 1;
}

/**
 @fn	void CzApp::NotifyOrientationChanged()

 @brief	Notifies scenes that the screen orientation changed.

 This method is called by app update when it detects a screen size / orientation change. This method notify all scenes about the event.

 */

void CzApp::NotifyOrientationChanged()
{
	CzXomlVariableArrayInt* arr = (CzXomlVariableArrayInt*)CZ_GLOBAL_RESOURCES->getVariableManager()->findVariable("system");
	arr->getElement(SYS_SCREEN_ORIENTATION)->setValue(CzString(ScreenOrientation).c_str());

	IzPlatformDisplay* display = PLATFORM_DISPLAY;
	CzUIBase::UpdatePresetAnimations();
	ScreenSize.x = display->getCurrentWidth();
	ScreenSize.y = display->getCurrentHeight();

	for (_Iterator it = begin(); it != end(); it++)
	{
		if (!(*it)->isDestroyed())
		{
			(*it)->NotifyOrientationChanged();
		}
	}
}

/**
 @fn	void CzApp::NotifySizeChanged()

 @brief	Notifies scnes that the screen size changed.

 This method is called by app update when it detects a screen size change. This method notify all scenes about the event.

 */

void CzApp::NotifySizeChanged()
{
	IzPlatformDisplay* display = PLATFORM_DISPLAY;
	CzUIBase::UpdatePresetAnimations();
	ScreenSize.x = display->getCurrentWidth();
	ScreenSize.y = display->getCurrentHeight();

	for (_Iterator it = begin(); it != end(); it++)
	{
		if (!(*it)->isDestroyed())
		{
			(*it)->NotifySizeChanged();
		}
	}
}

IzXomlResource* CzAppCreator::CreateInstance(IzXomlResource* parent)
{
	return NULL;
}
