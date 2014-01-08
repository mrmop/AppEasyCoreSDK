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

#if !defined(_CZ_SCENE_H_)
#define _CZ_SCENE_H_

#include "CzSprite.h"
#include "CzSpriteManager.h"
#include "CzAnim.h"
#include "CzImage.h"
#include "CzUtil.h"
#include "CzString.h"
#include "CzXoml.h"
#include "CzResources.h"
#include "CzBox2d.h"
#include "CzActions.h"
#include "CzEvents.h"
#include "CzModifier.h"
#include "CzProgram.h"
#include "CzXomlBindings.h"
#include "CzUserProperty.h"
#include "CzTimer.h"

class CzActor;
class CzCamera;
class IzScriptEngine;

/**
 @addtogroup Scenes
 @{
 */

/**
 @class	CzScene

 @brief	A scene contains, handles and tracks a group of actors.

 <h1>Introduction</h1>
 Sometimes understanding concepts are much easier when compared to something that one is already familiar with. In this case we will compare the making of apps and
 games using XOML to the making of a movie. In the movie business a scene (or set) is a place where the action takes place, it contains all of the actors, the cameras and
 scenery. Each actor has its own specific instructions, behaviours and purpose. 

 A CzScene is responsible for the following functionality:
 
 - Setup and handling of the virtual canvas
 - Managing, updating and cleaning up actors
 - Managing, updating, rendering and cleaning up sprites
 - Managing and clean up of animation data
 - Managing and clean up of animation timelines
 - Managing and clean up of animation timers
 - Managing and clean up of events / actions
 - Managing and clean up of resources such as images and sounds
 - Managing and clean up of physics materials and shapes
 - Managing and clean up of XOML variables
 - Managing and clean up of XOML data bindings
 - Managing and clean up of XOML programs
 - Managing and clean up of scripts
 - Clipping sprites against the scenes visible extents
 - Updating the camera
 - Tracking actors that can potentially collide
 - Transitions between scenes
 - Updating the scenes animation timeline
 - Instantiating itself from XOML
 - Updating physics
 
 Its worth noting at this point that any actors that live inside a scene will be transformed by the same transform as the scene, so if you move, scale or spin the scene then 
 the contained actors will also move, scale and spin with it. The same can also be said about the base colour of the scene.
 
 <h1>Creating a Scene</h1>

 There are a number of ways to create a scene, lets take a look at some basic examples:

 @par XOML Example:
 @code
<?xml version="1.0"?>
<xml>
	<Scene Name="AppScene" Current="true" CanvasSize="800, 480">
	</Scene>
</xml>
 @endcode

 @par Code Example:
 @code
CzScene* scene = new CzScene();
scene->setName("AppScene");
scene->setAllowFocus(true);
scene->Init();
scene->setVirtualTransform(800, 400, 0, CzScene::Fit_None);
APP->addScene(scene);
APP->changeScene(scene);
 @endcode

 @par Code Example (using macros):
 @code
CZ_NEW_SCENE_CUSTOM(scene, "AppScene", 800, 400, CzScene::Fit_None);
APP->addScene(scene);
APP->changeScene(scene);
 @endcode

 @par Lua Script Example:
 @code
local my_scene = scene.create("Scene1", 800, 600, "best", "centre", false, false, "lua")
scene.setCurrent(my_scene)
 @endcode

 Here we create a scene called AppScene that has a canvas size of 800x480 pixels. Once a scene is set-up we can begin adding the actors and other content that form 
 our game or app.

 <h1>Scene Properties</h1>

 Now that we know how to create a scene lets take a look at the extensive list of properties that scenes support:

 <b>General Properties</b>
 - Name - The name of the scene is a very important property and should be chosen so that it does not conflict with names of other scenes that are present at the same time. 
 It is possible to have multiple scenes with the same name, but only one of those scenes can be created. It is possible to destroy a scene and create a new scene of the same 
 name. A scene name should also be memorable as you may want to refer to the scene from other areas of XOML or even script.
 - Type (whole number) - The type of scene is a simple number that can be used to identify specific scene types. For example, you could create multiple scenes that are all 
 related and assign them all the same type.
 - Extents (x, y, width, height) - A rectangular area that describes the extents of the scenes world
 - AllowSuspend (boolean) - Determines if the scene can be suspended when other scenes are activated
 - Clipping (x, y, width, height) - A rectangular area that represents the visible area of the scene
 - ClipStatic (boolean) – When true camera transform is not applied to the clipping rect
 - Active - The active state of a scene determines if the scene is processing its contents. If not active then scene processing will stop and any content that is contained 
 within the scene will also stop processing
 - Visible - If a scene is not visible then it will be hidden from view, although the scene and its contents will still be processed.
 - Layers (number, read-only) - The number of visible actor layers that the scene should use (10 is default value)
 - Current (boolean) - If true then the scene is made the current scene
 - Camera (camera name) - Current camera
 - Batch (boolean) - Tells the system to batch sprites for optimised rendering, default is disabled
 - AllowFocus (boolean) - If set to true then this scene will receive input focus events that the current scene would usually receive exclusively. This is useful if you have 
 a HUD overlay that has functionality but it cannot be the current scene as the game scene is currently the current scene
 - Style (style) - Sets scene properties from a pre-defined style (see Style tag)
 - UserProperties (object, read-only) – The scenes user properties list
 - TargetX (actor name, write-only) – Sets the actor that the camera will be used to track its x-axis
 - TargetY (actor name, write-only) – Sets the actor that the camera will be used to track its y-axis 
 - Bindings (bindings name) – Defines a bindings list that binds variables to scene properties. Reading this property will return a bindings object
 - TimeScale (number) – Changes the speed at which an attached timeline is played. This property cannot be set when initially declaring the scene

 <b>Visual Properties</b>
 - Position (x, y) - The position of the scene on screen
 - PositionX (number) - The X position of the scene on screen
 - PositionY (number) - The Y position of the scene on screen
 - Angle (degrees) - The orientation of the scene in degrees
 - Scale (scale) - The scale of the scene
 - CanvasSize (x, y) - The virtual canvas size of the screen
 - CanvasFit (none, best, both, width, or height) - The type of method to use when scaling the canvas to the devices native screen resolution
 - CanvasOrigin (top, left, topleft or centre) - Where to locate the canvas origin
 - Layer (whole number)- The visual layer that this scene should be rendered on (Valid values are 0 to 9)
 - Colour (r, g, b, a)- The colour of the scene, Each component red, green, blue and alpha should be between the value of 0 and 255 (0 is no colour, whilst 255 is full colour)
 - Opacity (opacity) - The opacity of the scene (how see-through the scene appears). Opacity value ranges from 0 (invisible) to 255 (fully visible)
 - Timeline (timeline) - The time line that should be used to animate the scene. Reading this property will  return a timeline object
 
 <b>Event Properties</b>
 - OnSuspend (actions list) - Provides an actions group that is called when the scene is suspended
 - OnResume (actions list) - Provides an actions group that is called when the scene is resumed
 - OnCreate (actions list) - Provides an actions group that is called when the scene is created
 - OnDestroy (actions list) - Provides an actions group that is called when the scene is destroyed
 - OnKeyBack (actions list) - Provides an actions group that is called when the user presses the back key
 - OnKeyMenu (actions list) - Provides an actions group that is called when the user presses the menu key
 - OnOrientationChanged (actions list) - Provides an actions group that is called when the user changes the devices orientation
 - OnTick (actions list) - Provides an actions group that is called every time the scene is updated (30 to 60 times per second)
 - OnDeviceSuspended (actions list, write-only) - Provides an actions group that is called when the app is suspended
 - OnDeviceResumed (actions list, write-only) - Provides an actions group that is called when the app is resumed
 - OnDeviceVolumeUp (actions list, write-only) - Provides an actions group that is called when the volume up button was pressed
 - OnDeviceVolumeDown (actions list, write-only) - Provides an actions group that is called when the volume down button was pressed
 - OnDeviceNetworkOnline (actions list, write-only) - Provides an actions group that is called when the network goes online
 - OnDeviceNetworkOffline (actions list, write-only) - Provides an actions group that is called when the network goes offline
 
 <b>Physical Properties</b>
 - Gravity (x, y) - Box2D directional world gravity
 - WorldScale (x, y) - Box2D world scale
 - DoSleep (boolean) - If set to true then actors that utilise physics will be allowed to sleep when they are not moving / interacting
 - Physics (boolean) - Enables or disables physics processing in the scene
 - PhysicsTimestep (number) - Sets how fast the physics engine updates the physics world. This value is specified in number of 1/60th's of a second. So passing a value of 
 1 will update physics as though the frame rate of the app is running at 60 frames per second. A value of 3 would would update physics at a rate of 20 frames per second. 
 Setting this to a value of 0 will use a variable time step based on the current frame rate of the app. The default value is 2.0 We will now take a closer look at how these 
 properties function.

 <h1>Scene Virtual Canvas</h1>
 Targeting a large selection of different phones, tablets and other devices with a variety of different screen sizes and aspect ratios can be difficult to manage. Luckily scenes 
 can take care of this for you. A scene is quite clever in that it can render itself to any sized / configuration display using the virtual canvas concept. A virtual canvas is 
 basically our own ideal screen size that we want to render to. The scene will scale and translate its visuals to fit our canvas onto the devices display allowing us to get
 on with developing our app using a static resolution. Lets take a look at an example that creates a scene with a virtual canvas of 800x480:

 @code
<xml>
	<!-- Create a scene to hold our game / app actors -->
	<Scene Name="AppScene" Current="true" CanvasSize="800, 480" CanvasFit="best" >
		<!-- Actors go here -->
	</Scene>
</xml>
 @endcode

 Using the AppScene above regardless of whatever size screen we run our app on our content will be scaled to fit an 800x480 pixel display resolution because we specified
 a CanvasSize of "800x480". We also told the scene to use its best judgement when scaling the scene by specifying CanvasFit="best". As an example if we run our app on an 
 iPhone retina display at 960x640 pixels and using a canvas fit method of best fit then our app will be scaled to best fit the 960x640 resolution, which would be 960x576 
 pixels. The scene would be centred on the screen leaving a little space at the top and bottom of the display.

 Scenes support a number of ways of fitting the the virtual canvas to the device screen including:
 - none - No scaling is performed. This is ideal if you want a 1:1 pixel ratio and mostly used by apps and games that use proportional sizing and positioning.
 - both - The scene is scaled to fit the exact size of the display ignoring aspect ratio (aspect ratio is the ratio of the screens width to the screens height). The problem 
 with this method of scaling is that the scene will be stretched on displays with different aspect ratios which can make certain apps look odd.
 - best - The scene is scaled to best fit the devices display size whilst maintaining aspect ratio. This method of scaling prevents stretched graphics, however it can leave 
 gaps to the top and bottom or left and right
 - width / height - This method of scaling is the same as best except scaling is locked to either the width or the height.

 Scenes also provide a way to allow you to specify how coordinates are interpreted allowing you to place content relative to either the centre, left, top-left or top of the
 screen. The scenes origin is by default set to the centre of the scene, but it can be changed by supplying the CanvasOrigin property when declaring the scene, e.g.:

 @code
<Scene Name="AppScene" Current="true" CanvasOrigin="topleft">
 @endcode

 This scene places actors relative to the top-left hand corner of the scene instead of relative to the centre. However, we find that having the scenes origin at the centre 
 is much better and feels more natural when designing apps an games for a variety of screen sizes as content can be allowed to extend from the centre and beyond the scenes 
 size. The default canvas size for a scene is the devices native display resolution

 <h1>Scene Layers and Actor Layering</h1>
 Because an app can contain multiple scenes that are visible at the same time, its sometimes required to sort the order in which these scenes are displayed. For
 example, you may have a game that has a background layer, a game object layer and a foreground layer. The visibility ordering of these scenes will be displayed in the
 opposite order to which you create them. For example, if you create the foreground scene and then the background scene afterwards then the background scene will be
 drawn on top of the foreground scene, which is the wrong way around. You could remedy this problem by adding the background scene first, but sometimes this is not
 practical. To eliminate the problem, scenes can be placed on a layer of depth by setting the Layer="layer number" (layer numbers 0 to 9 are supported with 9 being
 the highest layer) property of the scene. Scenes that are placed on the same layer will be sorted in the order they were added, with more recent additions appearing 
 on top. Each scene also determines how actors are sorted in a scene (we will discuss actors in great depth later). We can define how many layers are available for 
 the actor system to use by adding the Layers="number of layers" property to the scene definition. The default is 16 which is fine for most applications.

 An example has been provided that demonstrates scene layering (see SceneLayers example). Lets take a quick look at an extract of the XOML for this example:
 @code
<!-- Create scene 1 on highest layer -->
<Scene Name="Scene1" Layer="9">
</Scene>
<!-- Create scene 2 on middle layer -->
<Scene Name="Scene2" Layer="5">
</Scene>
<!-- Create scene 3 on highest layer -->
<Scene Name="Scene3" Layer="0">
</Scene>
 @endcode

 Here we create 3 scenes and assign each one a different layer.

 <h1>Current Scene</h1>
 An app generally consists of a number of different scenes that all contain their own objects. However only one scene can normally receive input. This scene is 
 known as the current or focus scene. When you declare a scene it can be made to be the current scene by adding Current="true" to the scene definition. By 
 comparison, you could think of all of the different windows on your PC or Mac as separate scenes, but only one of those windows can usually accept user input 
 at one time. When a scene is made current it is brought to the front of any scenes that are on the same layer (more on layers later).
 
 It is also possible to mark scenes so that more than one scene can receive input events at the same time. This can be useful if you have two scenes on lower 
 layers that need to also receive input. For example, you may have a heads-up display that has functional buttons on the highest layer and the game scene on a 
 lower layer that also needs to receive input events. To mark a scene to receive input events you need to add the AllowFocus="true" attribute to your scene 
 definition.

 <h1>Suspending and Resuming Scenes</h1>
 By default all active scenes in your app will be processed continuously. However this is not always required and for apps that contain a lot of active scenes 
 it can slow down your app. Scenes can be suspended so that they stop processing (they will still be visible). In fact, scenes that allow suspending / resuming 
 are automatically suspended when you change the current scene to another scene. To enable suspending / resuming for a scene you should set its 
 AllowSuspend="true" property. When a scene is suspended or resumed it will fire a suspend or resume event which allows you to perform actions (more on this later).

 The follownig two methods handle scene suspend / resume:
 - NotifySuspending(CzScene* new_scene);			- This event is called when this scene is being suspended
 - NotifyResuming(CzScene* old_scene);				- This event is called when this scene is being resumed

 Like all event handlers its possible to override these if you derive your own scene class from CzScene and handle these events as you see fit.

 Scenes can be suspended and resumed using the following actions in XOML:
 - ChangeScene - The previous scene will be suspended and the new scene will be resumed (provided that the scenes have AllowSuspend="true" defined)
 - SuspendScene - Suspends the scene even if suspend / resume is disabled for the scene
 - ResumeScene - Resumes the scene even if suspend / resume is disabled for the scene

 <h1>Scene Extents</h1>
 Scenes can be as large or as small as you like, they can even be many times larger than the devices screen size. This enables you to create content that exists 
 in a much larger space than the screen can display. You can then allow the user to pan around the scene to view more of the content. The size of your scenes 
 world can be set using the Extents property which accepts 4 values which represent the rectangular area that objects can exist in. Actors can be made so that 
 when they hit the edge of the scenes extents they re-appear on the opposite side (much like asteroids in the old Atari game Asteroids does), this is called 
 wrapping. An example called SceneExtents has been provided that shows an actor moving across the scene, it wraps back around to the opposite end of the scene 
 when it hits the scenes extents. Lets take a quick look at the XOML for this example:

 @code
<!-- Create scene with narrow extents -->
<Scene Name="Scene1" Extents="-100, -100, 200, 200" Current="true">
	<!-- Create a label object -->
	<Label Font="serif" Background="Button1Brush" BackgroundColour="255, 80, 80, 255" Text="Wrapping" WrapPosition="true" OnTick="Update">
		<Actions Name="Update">
			<Action Method="AddProperty" Param1="Position" Param2="2, 2" />
		</Actions>
	</Label>
</Scene>
 @endcode

 This example introduces some concepts that you have not yet met such as actions and events. You do not need to worry about these for now as we are only focusing 
 on the bold sections. Note how we have set the scenes extents using Extents="-100, -100, 200, 200". This collection of 4 value represent the left, top, width 
 and height of the scenes extents. Also note that our Label has been given a new property called WrapPosition which is set to true. This tells the Label actor 
 to wrap its position when it hits the edges of the scenes extents.

 <h1>Scene Clipping</h1>
 When a scene is smaller than the devices display size its sometimes useful to be able to ensure that anything that is drawn within the scene is not drawn outside 
 the scene. To accomplish this we need to clip content that falls outside the scenes area of coverage. To tell a Scene to clip its contents you add the Clipping 
 property which takes 4 values (left, top, width, height). An example has been provided called SceneClipping which demonstrates how to apply scene clipping. Lets 
 take a quick look at this example:

 @code
<!-- Create scene with narrow extents -->
<Scene Name="Scene1" Extents="-100, -100, 200, 200" Clipping="-100, -100, 200, 200" Current="true" />
 @endcode

 In this example we limit the scenes extents and set the clipping area to -100, -100, 200, 200. By default the scenes clipping area will move around with the 
 scene, although you can modify this behaviour to force the clipping area to remain in place on screen. To force clipping to remain in place add 
 ClipStatic="true" to your scene definition.

 <h1>Scene Events</h1>

 An event is something that occurs in your app that you may or may not want to know about and act upon. For example, when the scene is first created the 
 OnCreate event is raised. Using actions (explained later) you can react to these events and modify your apps behaviour. 

 Internally the scene handles events using the Events Manager (EventsManager). This manager handles the mapping between events and  actions lists that should 
 be called when the events take place. To add an event to the events manager we call 
 EventsManager->addEvent(event_name, action_list_name), e.g:

 @code
 EventsManager->addEvent("OnSuspend", "SuspendedActions");
 @endcode

 This example ties the SuspendedActions list that was previously defined in XOML using the Actions tag to the OnSuspend event. When an event takes place 
 the corresponding event notification method of CzScene is called, for example NotifyCreate() is called when the scene is created. This in turn calls 
 CzScene::ProcessEventActions() to handle the event. ProcessEventActions examines its list of subscribed events to see if the event has been subsribed to. 
 If it has then the corresponding actions list will be called. if you do not wish to use actions lists to respond to events then yuo can simply derive 
 your own sceen class from CzScene, override the event notification methods such as NotifyCreate() and write your own implementation.

 You can listen to events by adding an OnEvent property to the scene. Lets take a look at the list of events that can occur in a scene:
 - OnSuspend - This event is raised when the scene is suspended. A scene is suspended if it is the current scene and another scene becomes the current 
 scene (for scenes that support suspend / resume) or when a SuspendScene action is called
 - OnResume - This event is raised when a scene is resumed (become the current scene) or when a ResumeScene action is called
 - OnCreate - This event is raised when the scene is first created and gives you the opportunity to handle post scene creation logic. You could use this 
 event to for example play a sound or hide another scene etc..
 - OnDestroy - This event is raised when the scene is about to be destroyed, this gives you the opportunity to clean-up certain elements
 - OnKeyBack - This event is raised when the user pressed the back button
 - OnKeyMenu - This event is raised when the user pressed the menu button
 - OnOrientationChanged - This event is raised when the devices orientation changes an allows yo to for example modify the scenes layout.
 - OnTick - This event handler is raised every time the scene is updated (30 to 60 times per second)
 - OnDeviceSuspended (actions list, write-only) - Provides an actions group that is called when the app is suspended
 - OnDeviceResumed (actions list, write-only) - Provides an actions group that is called when the app is resumed
 - OnDeviceVolumeUp (actions list, write-only) - Provides an actions group that is called when the volume up button was pressed
 - OnDeviceVolumeDown (actions list, write-only) - Provides an actions group that is called when the volume down button was pressed
 - OnDeviceNetworkOnline (actions list, write-only) - Provides an actions group that is called when the network goes online
 - OnDeviceNetworkOffline (actions list, write-only) - Provides an actions group that is called when the network goes offline

 The SceneEvents example shows an example showing how to handle the scenes OnTick event.

 <h1>Scene Animation Properties</h1>
 Scenes can be animated using animations defined in XOML. Whilst not all properties of a scene can be animated many can including:
 - Position (x, y) - The position of the scene on screen
 - Angle (degrees) - The orientation of the scene in degrees
 - Scale (scale) - The scale of the scene
 - Colour (r, g, b, a)- The colour of the scene, Each component red, green, blue and alpha should be between the value of 0 and 255 (0 is no colour, whilst 255
 is full colour)
 - Opacity (opacity)- The opacity of the scene (how see-through the scene appears). Opacity value ranges from 0 (invisible) to 255 (fully visible)
 - Clipping (x, y, width, height) - The clipping area of the scene
 - Visible (boolean) - The visibility of the scene
 - Timeline (timeline) - The animation timeline that is currently being used to animate the scene. Timelines are great for creating complex scene transitions
 - Camera (camera) - The current camera that is viewing the scene The SceneAnimation example has been provided that shows how to apply animation to a scene
 - PhysicsTimestep (number) - This can be used to modify the speed at which physics updates allowing you to slow down or speed up physics.

 <h1>Scene Modifiable Properties</h1>
 Scenes can be queried and modified after their creation using actions, command and scripts. The following properties are includes:

 <b>General Properties</b>
 - Name (string)
 - Type (number)
 - Extents (x, y, width, height)
 - AllowSuspend (boolean)
 - Clipping (x, y, width, height)
 - Active (boolean)
 - Visible (boolean)
 - Current (boolean)
 - Camera (camera)
 - AllowFocus (boolean)

 <b>Visual Properties</b>
 - Layer (number)
 - Colour (r, g, b, a)
 - Opacity (opacity)
 - Timeline (timeline)
 - TimeScale (number) - Speed at which to play back any attached timelines

 <b>Event Properties</b>
 - OnCreate (actions list)
 - OnDestroy (actions list)
 - OnKeyBack (actions list)
 - OnKeyMenu (actions list)
 - OnOrientationChanged (actions list)
 - OnTick (actions list)

 <b>Physical Properties</b>
 - Gravity (x, y)
 - WorldScale (x, y)

 <h1>Scene Rendering</h1>
 A scene has no actual visible component, instead it renders all of its contained actor objects. All actor objects are moved, rotated and scaled by the 
 same amount that the scene is. For example, if you move the scene to the left, all contained actors will also moved to the left. If you rotate the scene 
 to the right 45 degrees then all actors also rotate 45 degrees to the right. In addition, all scene actors will be scaled by the scenes colour and opacity 
 settings, so if you fad down the scene all actors will also fade down with it.

 Scenes support a method of optimisation known as batch rendering. This is the process of combining actors together at render time to render them all in one go,
 which can substantially speed up rendering for scenes that contain many actors. To force a scene to use batch rendering add Batch="true" to the scene definition.
 However, scenes that are marked as batch enabled will not sort their contained actors properly with the likes of fonts, so you will need to use actor layers 
 to force the sorting.

 When a scene is hidden from view, all of its actors will also be hidden. When a scene is deleted all of its actors are also deleted.

 <h1>Scene Cameras</h1>
 A camera is a view into the scene from a specific position, angle and scale. In order to move, rotate and scale a scene a camera should be created and 
 attached to it. To create a camera you use the Camera XOML tag:

 @code
<!-- Create a camera -->
<Camera Name="Camera1" />
 @endcode

 To attach the camera to the scene we add the Camera attribute to the scene definition:
 @code
<Scene Name="Scene1" Current="true" Camera="Camera1">
 @endcode

 When we navigate a scene we are actually moving the camera view within the scene. When we move the camera objects tend to move in the opposite direction, for
 example moving the camera left moves the scene actors move to the right. If you think about how a real camera works, when you move the camera in one direction 
 the view seen by the camera moves in the opposite direction.

 Its possible to create a number of different cameras and switch between them to offer different views into the scene. Cameras offer a great out of the box 
 feature called touch panning, which enables the user to pan the camera a round a scene on the x and y axis by dragging their finger around the scene. Take a 
 look at the ScenePanning example for an example showing how to use touch panning.

 <h1>Scene Augmentation</h1>
 A scene once declared in XOML can later be updated / augmented with additional XOML elsewhere. For example, lets say that you declare some common scene that
 contains a basic background and some other elements that are common across a number of screens. You can later load the scene and then augment it by declaring 
 the scene again supplying the additional elements inside the newly declared scene:

 @code
<Scene Name="CommonScene" ............ >
	<Original_Element1 />
	<Original_Element2 />
	<Original_Element3 />
</Scene>
 @endcode

Now declare a 2nd scene with the same name:
 @code
<Scene Name="CommonScene">
	<Extra_Element1 />
	<Extra_Element2 />
	<Extra_Element3 />
</Scene>
 @endcode

In memory the scene now looks like this:
 @code
<Scene Name="CommonScene" ............ >
	<Original_Element1 />
	<Original_Element2 />
	<Original_Element3 />
	<Extra_Element1 />
	<Extra_Element2 />
	<Extra_Element3 />
</Scene>
 @endcode

 For a working example of augmented scenes take a look at the SceneAugmentation example.

 <h1>Scene Physics</h1>
 XOML scenes can run actors that are under the influence of the Box2D physics systems. The scene system allows you to specify some scene global information 
 about the physics simulation. The following scene global properties can be set by adding the relevant tags to the scene definition:
 - Gravity (x, y) - Box2D directional world gravity. Box2D uses directional gravity which means you can have gravity act in any direction.
 - WorldScale (x, y) - Box2D world scale. This value determines how your visible world scales to the Box2D world (default is 10, 10)
 - DoSleep (boolean) - If set to true then actors that utilise physics will be allowed to sleep when they are not moving / interacting, this can help speed up
 scenes that contain many actors that are under Box2D control.
 - Physics (boolean) - Enables or disables physics processing in the scene, enabled by default. Disable in scenes that do not use physics to maximise performance.
 - PhysicsTimestep (number) - Sets how fast the physics engine updates the physics world. This value is specified in number of 1/60th's of a second. So passing 
 a value of 1 will update physics as though the frame rate of the app is running at 60 frames per second. A value of 3 would would update physics at a rate of 
 20 frames per second. Setting this to a value of 0 will use a variable time step based on the current frame rate of the app. The default value is 2.0 To see 
 how scene physics is used please take a look at the ScenePhysics example.

 <h1>Creating a Custom Scene</h1>

 Generally and for the most part a standard CzScene can be used to accomplish most things with AppEasy scenes, especiallu if devloping most if not all of your aoo in 
 XOML / Lua, however for those that require a little more control its possible to derive your own scene from CzScene and implement the supplied virtual methods to 
 override certain functionality. The main 3 methods that you should consider re-implementing includes:
 - int	Init(int max_collidables = 128, int max_layers = 10, bool doSleep = true)
 - void	Update(float dt)
 - void	Draw()

 Here's a quick example:

 @code
class MyGameScene : public CzScene
{
public:
	MyGameScene() : CzScene() {}
	virtual ~MyGameScene();

	int		Init(int max_collidables = 128, int max_layers = 10, bool doSleep = true)
	{
		CzScene::Init(max_collidables, max_layers, doSleep);
	}
	void	Update(float dt)
	{
		CzScene::Update(dt);
	}
	void	Draw()
	{
		CzScene::Draw();
	}
};
 @endcode


 We have provided a very basic implementation of Init(), Update() and Draw() which call the base CzScene class methods so we keep its functionality in-tact.

 You can take the implementation one step further (or maybe two) by implementing both the IzXomlResource and  IzAnimTarget interfaces to allow instantiation of your custom class from XOML 
 and to allow your class to be a target for animation time lines.

 Firstly lets take a look at XOML enabling your custom scene class. To get AppEasy to recognise your class whilst parsing XOML files you need to do a few things:
 - Derive your class from IzXomlResource and implement the LoadFromXoml method
 - Create a class creator that creates an instance of your class then add this to the XOML engine

 Lets start by taking a look at step 1.

 Because we have derived our class from CzScene (which is derived from IzXomlResource) we already have the support for step 1. However we would like to insert our own custom attribute 
 tags so we need to make a few changes. 

 Lets take a look at our new class with those changes:

 @code
class MyGameScene : public CzScene
{
public:
	// Properties
protected:
	float		Gravity;
public:
	void		setGravity(float gravity)	{ Gravity = gravity; }
	float		getGravity() const			{ return Gravity; }
	// Properties End
public:
	MyGameScene() : CzScene(), Gravity(10.0f) {}
	virtual ~MyGameScene();

	virtual int		Init(int max_collidables = 128, int max_layers = 10, bool doSleep = true)
	{
		CzScene::Init(max_collidables, max_layers, doSleep);
	}
	virtual void	Update(float dt)
	{
		CzScene::Update(dt);
	}
	virtual void	Draw()
	{
		CzScene::Draw();
	}

	// Implementation of IzXomlResource interface
	bool			LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
	{
		if (!CzScene::LoadFromXoml(parent, load_children, node))
			return false;

		// Add our own custom attribute parsing
		for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
		{
			unsigned int name_hash = (*it)->getName().getHash();

			if (name_hash == CzString::CalculateHash("Gravity"))
			{
				setGravity((*it)->getValueAsFloat());
			}
		}

		return true;
	}
};
 @endcode

 Our new class now basically supports a Gravity attribute that we will eventually be able to set in XOML using something like:

 @code
<MyGameScene Name="GameScene" Gravity="9.8">
</MyGameScene>
 @endcode

 However, before we can do that we need to let the XOML system know about our new type of class (MyGameScene), so it can be instantiated when the XOM parser comes across it. To do 
 this we need to create a creator:

 @code
class MyGameSceneCreator : public IzXomlClassCreator
{
public:
	MyGameSceneCreator()
	{
		setClassName("MyGameScene");
	}
	IzXomlResource* CreateInstance(IzXomlResource* parent) { return new MyGameScene(); }
};
 @endcode

 The creator basically defines the tag name "MyGameScene" and returns an instance of the MyGameScene class when CreateInstance() is called. 

 To get the XOML system to recognise our creator we need to add it to the XOML parsing system using:

 @code
// Add custom MyGameScene to XOML system
CZ_XOML->addClass(new MyGameSceneCreator());
 @endcode

 Now XOML integration is out of the way, lets take a quick look at enabling our class as an animation target.

 To enable a class as an animation target we derive it from  IzAnimTarget and implement the UpdateFromAnimation() method. Luckily we derived our MyGameScene class from the CzScene 
 class which already provides this functionality. Lets take a quick look at how we extend the animation update method to account for animating our gravity variable.

 @code
bool MyGameScene::UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzScene::UpdateFromAnimation(animation))
		return true;

	// Add our own custom animating property
	unsigned int property_name = animation->getTargetPropertyHash();

	if (property_name == CzString::CalculateHash("Gravity"))
	{
		CzAnimFrameFloat* frame = (CzAnimFrameFloat*)animation->getCurrentData();
		setGravity(frame->data);
		return true;
	}

	return false;
}
 @endcode

 We added the above code to our MyGameScene class definition. We begin by calling the base UpdateFromAnimation() method so we can keep the existing animation properties of the 
 scene. We then add our own custom check for the Gravity variable. If the animation property matches Gravity then we set the gravity to the provided interpolated value.




 */

class CzScene : public IzXomlResource, public IzAnimTarget
{
public:
	// Public access to actor iteration
	typedef CzList<CzActor*>::iterator _Iterator;
	_Iterator					begin() { return Actors.begin(); }
	_Iterator					end() { return Actors.end(); }

	/**
	 @enum	eCanvasFit
	
	 @brief	Values that define how the virtual canvas is fit to the devices screen.
	 */
	enum eCanvasFit
	{
		Fit_None,		///< No scaling is performed. This is ideal if you want a 1:1 pixel ratio and mostly used by apps and games that use proportional sizing and positioning.
		Fit_Width,		///< This method of scaling is the same as best except scaling is locked to the width
		Fit_Height,		///< This method of scaling is the same as best except scaling is locked to the height
		Fit_Both,		///< The scene is scaled to fit the exact size of the display ignoring aspect ratio (aspect ratio is the ratio of the screens width to the screens height). The problem with this method of scaling is that the scene will be stretched on displays with different aspect ratios which can make certain apps look odd.
		Fit_Best,		///< The scene is scaled to best fit the devices display size whilst maintaining aspect ratio. This method of scaling prevents stretched graphics, however it can leave gaps to the top and bottom or left and right
	};

	/**
	 @enum	eCanvasOrigin
	
	 @brief	Values that define the placement of teh canvas origin.
	 */
	enum eCanvasOrigin
	{
		Origin_Centre,	///< Origin is at the centre of the screen
		Origin_Top,		///< Origin is at the top centre of the screen
		Origin_Left,	///< Origin is at the left middle of the screen
		Origin_TopLeft,	///< Origin is at the rop left of the screen
	};

	/**
	 @enum	eOrientation
	
	 @brief	Values that define the orientation of the screen.
	 */
	enum eOrientation
	{
		Orientation_Landscape,	///< Landscape orientation (width > height)
		Orientation_Portrait,	///< Portrait orientation (width < height)
	};

	/**
	 @enum	eDocking
	
	 @brief	Values that define acor docking.
	 */
	enum eDocking
	{
		Dock_None, 
		Dock_Top, 
		Dock_Bottom, 
		Dock_Left, 
		Dock_Right, 
		Dock_TopLeft, 
		Dock_TopRight, 
		Dock_BottomLeft, 
		Dock_BottomRight, 
	};
	static const char*		DockingNames[];


	// Properties
protected:
	CzSpriteManager*		SpriteManager;		///< Manages sprites for the whole scene
	CzXomlResourceManager*	ResourceManager;	///< Manages all types of resources
	CzXomlVariableManager*	VariableManager;	///< Manages XOML variables
	CzActionsManager*		ActionsManager;		///< Manages actions
	CzAnimTimelinesManager* TimelinesManager;	///< Manages timelines
	CzTimersManager*		TimersManager;		///< Manages attached timers
	CzProgramManager*		ProgramManager;		///< Manages the execution of a collection of programs
	CzXomlBindings*			Bindings;			///< Manages any attached bindings
	IzScriptEngine*			ScriptEngine;		///< Mnaages scene scripts
	CzApp*					Parent;				///< Parent game controller
	int						Type;				///< Type of scene
	eOrientation			Orientation;		///< Current display orientation
	CzIVec2					ScreenSize;			///< Native screen size
	CzIVec2					VirtualSize;		///< The virtual size is not the actual size of the scene. but a static pretend size that we can use to render to without having to cater for different sized displays
	CzMatrix3				VirtualTransform;	///< Virtual transform is used to scale, translate and rotate scene to fit different display sizes and orientations
	CzMatrix3				Transform;			///< Scene transform
	CzList<CzActor*>		Actors;				///< Collection of scene actors
	CzIRect					Extents;			///< Extents of scenes world
	CzCamera*				Camera;				///< Current camera
	CzActor**				Collidables;		///< List of collidable objects built this frame
	bool					AllowSuspend;		///< If true then this processing of this scene will be suspended when the scene manager changes away from this scene
	CzIRect					ClippingArea;		///< A rectangular area of the virtual screen that this scene will be clipped to
	bool					ClipStatic;			///< If ClipStatic is true then camera transform is not applied to clipping rect
	bool					IsActive;			///< Active state of scene
	bool					IsVisible;			///< Visible state of scene
	bool					AllowFocus;			///< If true this scene can receive input events if it is not the current scene
	CzColour				Colour;				///< Colour
	CzAnimTimeline*			Timeline;			///< Timeline, controls animation of the scene
	int						RunCount;			///< NUmber of times the scene has been updated
	bool					Destroyed;			///< A scene is marked asd destroyed when it has been marked for deletion
	int						Layer;				///< Scenes layer
	bool					PhysicsEnabled;		///< True if physics is enabled in this scene
	bool					IsDragging;			///< True if user is draggnig their finger on the scene
	bool					PrevTouching;		///< True if user is draggnig their finger on the scene
	CzVec2					PrevTouchVel;		///< Previous touch velocity
	float					ConstantTimeStep;	///< If 0 then time step will be calculated from frame rate
	CzBox2dWorld*			Box2dWorld;
	CzUserPropertyList*		UserPropertyList;	///< User properties list
	CzEventManager*			EventsManager;		///< List of events that the scene handles
	CzModifierManager*		Modifiers;			///< Class modifiers manager
public:

	void					setParent(CzApp* parent)				{ Parent = parent; }
	CzApp*					getParent()								{ return Parent; }
	CzSpriteManager*		getSpriteManager()						{ return SpriteManager; }
	CzXomlResourceManager*	getResourceManager()					{ return ResourceManager; }
	CzActionsManager*		getActionsManager()						{ return ActionsManager; }
	CzAnimTimelinesManager* getTimelinesManager()					{ return TimelinesManager; }
	CzTimersManager*		getTimersManager()						{ return TimersManager; }
	CzXomlVariableManager*	getVariableManager()					{ return VariableManager; }
	CzProgramManager*		getProgramManager()						{ return ProgramManager; }
	IzScriptEngine*			getScriptEngine()						{ return ScriptEngine; }
	void					setScriptEngine(const CzString& type);
	void					setType(int type)						{ Type = type; }
	int						getType() const							{ return Type; }
	CzIVec2					getScreenSize() const					{ return ScreenSize; }
	eOrientation			getOrientation() const					{ return Orientation; }
	CzIVec2					getVirtualSize() const					{ return VirtualSize; }
	void					setVirtualTransform(int required_width, int required_height, float angle, bool fix_aspect = false, bool lock_width = false, eCanvasOrigin canvas_origin = Origin_Centre);
	void					setVirtualTransform(int required_width, int required_height, float angle, eCanvasFit fit = Fit_Best, eCanvasOrigin canvas_origin = Origin_Centre);
	CzMatrix3&				getVirtualTransform()					{ return VirtualTransform; }
	CzMatrix3&				getTransform()							{ return Transform; }
	void					setCanvasFit(eCanvasFit fit)			{ CanvasFit = fit; }
	eCanvasFit				getCanvasFit() const					{ return CanvasFit; }
	eCanvasOrigin			getCanvasOrigin() const					{ return CanvasOrigin; }
	void					setCanvasOrigin(eCanvasOrigin org)		{ CanvasOrigin = org; }
	CzList<CzActor*>&		getActors()								{ return Actors; }
	void					addActor(CzActor *actor);
	void					removeActorNoDelete(CzActor* actor);
	void					removeActor(CzActor* actor, bool instant_delete = false);
	void					removeActor(unsigned int name_hash, bool instant_delete = false);
	void					removeActors(unsigned int tag_hash);
	void					fixActors(CzActor* parent);
	void					removeLinkedActors(CzActor* actor, bool instant_delete = false);
	bool					findActorInRemovals(CzActor* actor);
	int						checkActorState(CzActor* actor);
	CzActor*				findActor(const char* name);
	CzActor*				findActor(unsigned int name_hash);
	CzActor*				findActor(int type);
	int						findActors(unsigned int tag_hash, CzVector<CzActor*>& actors);
	int						findActorsOfType(int type, CzVector<CzActor*>& actors);
	CzActor*				findClosestActor(int x, int y, int type);
	CzActor*				findFurthestActor(int x, int y, int type);
	void					clearActors(bool ignore_visuals = false);
	void					setExtents(int x, int y, int w, int h)	{ Extents.x = x; Extents.y = y; Extents.w = w; Extents.h = h; }
	CzIRect					getExtents() const						{ return Extents; }
	void					setCamera(CzCamera* camera);
	CzCamera*				getCamera()								{ return Camera; }
	bool					addCollideable(CzActor* actor);
	CzActor**				getCollidables()						{ return Collidables; }
	int						getTotalCollidables() const				{ return NextFreeCollidable; }
	void					setAllowSuspend(bool allow_suspend)		{ AllowSuspend = allow_suspend; }
	bool					getAllowSuspend() const					{ return AllowSuspend; }
	void					setClippingArea(int x, int y, int w, int h)		{ ClippingArea.x = x; ClippingArea.y = y; ClippingArea.w = w; ClippingArea.h = h; }
	CzIRect					getClippingArea()						{ return ClippingArea; }
	void					setClipStatic(bool is_static)			{ ClipStatic = is_static; }
	bool					isClipStatic() const					{ return ClipStatic; }
	void					setActive(bool active)					{ IsActive = active; }
	bool					isActive() const						{ return IsActive; }
	void					setVisible(bool visible)				{ IsVisible = visible; }
	bool					isVisible() const						{ return IsVisible; }
	void					setColour(const CzColour& colour)		{ Colour = colour; }
	void					setColour(uint8 r, uint8 g, uint8 b, uint8 a) { Colour.r = r; Colour.g = g; Colour.b = b; Colour.a = a; }
	void					setOpacity(int opacity)					{ Colour.a = opacity; }
	int						getOpacity() const						{ return Colour.a; }
	CzColour				getColour() const						{ return Colour; }
	void					setTimeline(CzAnimTimeline* timeline)	{ Timeline = timeline; }
	CzAnimTimeline*			getTimeline()							{ return Timeline; }
	void					setBindings(CzXomlBindings* bindings)	{ Bindings = bindings; }
	CzXomlBindings*			getBindings()							{ return Bindings; }
	void					setRunCount(int count)					{ RunCount = count; }
	int						getRunCount() const						{ return RunCount; }
	void					setDestroyed(bool destroyed)			{ Destroyed = destroyed; }
	bool					isDestroyed() const						{ return Destroyed; }
	void					setAllowFocus(bool allow)				{ AllowFocus = allow; }
	bool					getAllowFocus() const					{ return AllowFocus; }
	void					setLayer(int layer)						{ Layer = layer; }
	int						getLayer() const						{ return Layer; }
	bool					isDragging() const						{ return IsDragging; }
	void					setConstantTimeStep(float step)			{ ConstantTimeStep = step; }
	float					getConstantTimeStep() const				{ return ConstantTimeStep; }
	CzBox2dWorld*			getBox2dWorld()							{ return Box2dWorld; }
	void					setUserPropertyList(CzUserPropertyList* props)	{ SAFE_DELETE(UserPropertyList) UserPropertyList = props; }
	CzUserPropertyList*		getUserPropertyList()					{ return UserPropertyList; }
	CzEventManager*			getEventsManager()						{ return EventsManager; }
	void					setModifiers(CzModifierManager* mods)	{ Modifiers = mods; }
	CzModifierManager*		getModifiers()							{ return Modifiers; }
	bool					setProperty(const char* property_name, const CzString& data, bool delta);
	virtual bool			setProperty(unsigned int property_name, const CzString& data, bool delta);
	virtual bool			setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool					getProperty(const char* property_name, CzXomlProperty& prop);
	virtual bool			getProperty(unsigned int property_name, CzXomlProperty& prop);
	void					setBatching(bool enabled)				{ if (SpriteManager != NULL) SpriteManager->setBatching(enabled); }
	bool					isBatching() const						{ if (SpriteManager != NULL) return SpriteManager->getBatching(); return false; }
	int						getMaxLayers() const					{ return MaxLayers; }
	bool					getTickEnabled() const					{ return TickEnabled; }
	CzIVec2					getOriginalSize() const					{ return OriginalSize; }
	void					setPhysicsEnabled(bool enable)			{ PhysicsEnabled = enable; }
	bool					isPhysicsEnabled() const				{ return PhysicsEnabled; }
	// Properties end

protected:
	bool					TickEnabled;				///< true if OnTick event specified
	CzIVec2					OriginalSize;				///< Original size of scene
	eCanvasFit				CanvasFit;					///< Fit method for scalnig canvas
	eCanvasOrigin			CanvasOrigin;				///< Origin type
	CzVec2					TransformedClipRect[2];		///< Transformed clipping rect for the scene
	CzList<CzActor*>		Removals;
	int						MaxCollidables;				///< Maximum allowed collidables
	int						NextFreeCollidable;			///< Points to next free slot in sollidables list pool
	int						MaxLayers;					///< Maximum visible layers
	virtual void			UpdateBindings(bool force_modified = false);	///< Update data bindings
	void					UpdateActorResizing();		///< Updates the parents of any actors that have resized themselves

public:
	CzScene() :	IzXomlResource(), Parent(NULL), Collidables(NULL), ResourceManager(NULL), NextFreeCollidable(0), Camera(NULL), MaxCollidables(0), MaxLayers(0), AllowSuspend(false), IsActive(true), 
						IsVisible(true), RunCount(0), EventsManager(NULL), VariableManager(NULL), ActionsManager(NULL), Bindings(NULL), Type(0), AllowFocus(false), Destroyed(false), Timeline(NULL), 
						SpriteManager(NULL), ClipStatic(false), Layer(0), Modifiers(NULL), ConstantTimeStep(1.0f / 30.0f), CanvasFit(Fit_Best), CanvasOrigin(Origin_Centre), TimelinesManager(NULL), 
						ProgramManager(NULL), IsDragging(false), ScriptEngine(NULL), TickEnabled(false), PrevTouching(false), UserPropertyList(NULL), Box2dWorld(NULL), PhysicsEnabled(true), TimersManager(NULL)
	{
		setClassType("scene");
		EventsManager = new CzEventManager();
	}
	virtual ~CzScene();

	// After creating the scene, call Init() to initialise it, passing the maximum number of actors that you expect can collide 
	virtual int		Init(int max_collidables = 128, int max_layers = 10, bool doSleep = true);

	// Update physics
	void			UpdatePhysics(float dt);

	// Update() will update the scene and all of its contained actors
	virtual void	Update(float dt);

	// Draw() will draw all of the scenes actors
	virtual void	Draw();

	// PreDestroy() is called when the scene has been marked for deletion
	virtual void	PreDestroy() {}

	// Coordinate transform helper method
	CzVec2			ScreenToVirtual(float screen_x, float screen_y, bool ignore_translation = false);	///< Returns a virtual position from a screen position
	CzVec2			VirtualToScreen(float virtual_x, float virtual_y, bool ignore_translation = false);	///< Returns a screen position frmo a virtual position
	CzVec2			ScreenToCamera(float pos_x, float pos_y, bool ignore_translation = false);			///< Returns a virtual position from a position taking into account camera position

	// Event handlers (Only implement these if you need to know about these specific events)
	void			ProcessEventActions(unsigned int event_name);
	virtual void	NotifyPan();									///< This event is called when this scene is panned
	virtual void	NotifyCreate();									///< This event is called when this scene is being created
	virtual void	NotifyDestroy();								///< This event is called when this scene is being destroyed
	virtual void	NotifySuspending(CzScene* new_scene);			///< This event is called when this scene is being suspended
	virtual void	NotifyResuming(CzScene* old_scene);				///< This event is called when this scene is being resumed
	virtual void	NotifyLostFocus(CzScene* new_scene);			///< This event is called when this scene has just lost focus
	virtual void	NotifyGainedFocus(CzScene* old_scene);			///< This event is called when this scene has just gained focus
	virtual void	NotifyKeyBack();								///< This event is called when this scene receives a back key press
	virtual void	NotifyKeyMenu();								///< This event is called when this scene receives a menu key press
	virtual void	NotifyOrientationChanged();						///< This event is called when this scene receives an orientatin changed event
	virtual void	NotifySizeChanged();							///< This event is called when this scene receives a size changed event
	virtual void	NotifyDeviceSuspended();						///< This event is called when this scene receives a device suspended event
	virtual void	NotifyDeviceResumed();							///< This event is called when this scene receives a device resumed event
	virtual void	NotifyDeviceVolumeUp();							///< This event is called when this scene receives a device volume up event
	virtual void	NotifyDeviceVolumeDown();						///< This event is called when this scene receives a device volume down event
	virtual void	NotifyDeviceNetworkOnline();					///< This event is called when this scene receives a network online event
	virtual void	NotifyDeviceNetworkOffline();					///< This event is called when this scene receives a network offline event

	// Utility 
	void			Suspend(CzScene* new_scene);
	void			Resume(CzScene* old_scene);
	CzActor*		FindTappedActor(int x, int y);
	void			CleanupRemovedActors();
	void			CheckforInvalidRemovals(const char* check_point);

	// Implementation of IzXomlResource interface
	int				LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);
	bool			PostLoadFromXoml(IzXomlResource* parent, CzXmlNode* node);

	// Implementation of IzAnimTarget interface
	bool			UpdateFromAnimation(CzAnimInstance *animation);

	// Internal
	void			RestoreClipping();

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	ClassDef;								// XOML class definition
public:
	static void		InitClass();
	static void		ReleaseClass();
	// XOML property settings / getters
	static bool					_setName(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getName(IzXomlResource* target);
	static bool					_setType(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getType(IzXomlResource* target);
	static bool					_setLayer(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getLayer(IzXomlResource* target);
	static bool					_setPosition(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getPosition(IzXomlResource* target);
	static bool					_setPositionX(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getPositionX(IzXomlResource* target);
	static bool					_setPositionY(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getPositionY(IzXomlResource* target);
	static bool					_setAngle(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getAngle(IzXomlResource* target);
	static bool					_setScale(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getScale(IzXomlResource* target);
	static bool					_setColour(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getColour(IzXomlResource* target);
	static bool					_setOpacity(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getOpacity(IzXomlResource* target);
	static bool					_setTargetX(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setTargetY(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setCanvasSize(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getCanvasSize(IzXomlResource* target);
	static bool					_setCanvasFit(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getCanvasFit(IzXomlResource* target);
	static bool					_setCanvasOrigin(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getCanvasOrigin(IzXomlResource* target);
	static bool					_setExtents(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getExtents(IzXomlResource* target);
	static bool					_setAllowSuspend(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getAllowSuspend(IzXomlResource* target);
	static bool					_setClipping(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getClipping(IzXomlResource* target);
	static bool					_setClipStatic(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getClipStatic(IzXomlResource* target);
	static bool					_setActive(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getActive(IzXomlResource* target);
	static bool					_setVisible(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getVisible(IzXomlResource* target);
	static CzXomlProperty		_getLayers(IzXomlResource* target);
	static bool					_setCurrent(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getCurrent(IzXomlResource* target);
	static bool					_setTimeline(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getTimeline(IzXomlResource* target);
	static bool					_setBindings(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getBindings(IzXomlResource* target);
	static bool					_setTimeScale(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getTimeScale(IzXomlResource* target);
	static bool					_setCamera(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getCamera(IzXomlResource* target);
	static bool					_setBatch(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getBatch(IzXomlResource* target);
	static bool					_setAllowFocus(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getAllowFocus(IzXomlResource* target);
	static bool					_setOnSuspend(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setOnResume(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setOnLostFocus(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setOnGainedFocus(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setOnCreate(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setOnDestroy(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setOnKeyBack(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setOnKeyMenu(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setOnOrientationChange(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setOnTick(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setOnPan(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setOnDeviceSuspended(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setOnDeviceResumed(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setOnDeviceVolumeUp(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setOnDeviceVolumeDown(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setOnDeviceNetworkOnline(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setOnDeviceNetworkOffline(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setScriptEngine(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getScriptEngine(IzXomlResource* target);
	static bool					_setWorldScale(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getWorldScale(IzXomlResource* target);
	static bool					_setPhysics(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getPhysics(IzXomlResource* target);
	static bool					_setGravity(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getGravity(IzXomlResource* target);
	static bool					_setPhysicsTimeStep(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getPhysicsTimeStep(IzXomlResource* target);
	static CzXomlProperty		_getUserProperties(IzXomlResource* target);

};

/**
 @class	CzSceneCreator

 @brief	Creates an instance of a scene ovject.

 CzSceneCreator enables scenes to be instantiated from XOML.

 */

class CzSceneCreator : public IzXomlClassCreator
{
public:
	CzSceneCreator()
	{
		setClassName("scene");
	}
	IzXomlResource* CreateInstance(IzXomlResource* parent) { return new CzScene(); }
};

//
//
//
//
//  Helper macros
//
//
//
//

/**
 @def	CZ_NEW_SCENE(scene, name)

 @brief	Helper macros that simplifies creating a scene in code.

 This macros can be used to create a scene using a snigle line of code. Note that once the scene has been created it should be attached to the app using 
 CzApp::addScene()

 @param	scene	The scene.
 @param	name 	The name of the scene.
 */

#define CZ_NEW_SCENE(scene, name)						\
	CzScene* scene = new CzScene();						\
	scene->setName(name);								\
	scene->setAllowFocus(true);							\
	scene->Init();										\
	scene->setVirtualTransform(PLATFORM_DISPLAY->getCurrentWidth(), PLATFORM_DISPLAY->getCurrentHeight(), 0, CzScene::Fit_Best);

/**
 @def	CZ_NEW_SCENE_CUSTOM(scene, name, width, height, fit) CzScene* scene = new CzScene();

 @brief	Helper macros that simplifies creating a custom scene in code.
    
 This macros can be used to create a custom scene using a snigle line of code. Note that once the scene has been created it should be attached to the app using CzApp::addScene()

 @author	Mat
 @date	08/01/2013

 @param	scene 	The scene.
 @param	name  	The name of the scene.
 @param	width 	The virtual canvas width.
 @param	height	The virtual canvas height.
 @param	fit   	The virtual canvas fit method.
 */

#define CZ_NEW_SCENE_CUSTOM(scene, name, width, height, fit)	\
	CzScene* scene = new CzScene();						\
	scene->setName(name);								\
	scene->setAllowFocus(true);							\
	scene->Init();										\
	scene->setVirtualTransform(width, height, 0, fit);

/**
 @def	CZ_NEW_SCENE_WITH_CAM(scene, name)

 @brief	Helper macros that simplifies creating a scene in code.

 This macros can be used to create a scene containing a camera using a snigle line of code. The created camera is called "Cam". Note that once the scene has been 
 created it should be attached to the app using CzApp::addScene()

 @param	scene	The scene.
 @param	name 	The name of the scene.
 */

#define CZ_NEW_SCENE_WITH_CAM(scene, name)				\
	CzCamera* cam = new CzCamera();						\
	cam->setName("Cam");								\
	CzScene* scene = new CzScene();						\
	scene->setName(name);								\
	scene->setAllowFocus(true);							\
	scene->Init();										\
	scene->setCamera(cam);								\
	scene->setVirtualTransform(PLATFORM_DISPLAY->getCurrentWidth(), PLATFORM_DISPLAY->getCurrentHeight(), 0, CzScene::Fit_Best);


/// @}

#endif // _CZ_SCENE_H_