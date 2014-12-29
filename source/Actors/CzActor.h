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

#if !defined(_CZ_ACTOR_H_)
#define _CZ_ACTOR_H_

#include "CzUtil.h"
#include "CzScene.h"
#include "CzSprite.h"
#include "CzAnim.h"
#include "CzString.h"
#include "CzXoml.h"
#include "CzBox2d.h"
#include "CzShapes.h"
#include "CzActions.h"
#include "CzEvents.h"
#include "CzModifier.h"
#include "CzUserProperty.h"
#include "CzTimer.h"


/**
 @addtogroup Actors
 @{
 */
 
/**
 @class	CzActor

 @brief	A basic actor. We derive different types of game / app objects from this type and add them to be processed and rendered

 <h1>Introduction</h1>

 In the CzScene section we talked about scenes and compared them to the scenes of a movie set. Carrying on from our movie business analogy we can think of
 Actors as the actors and scenery objects that make up the movie scene with each actor having its own function and purpose within the scene. Actors are 
 the central focus of all AppEasy development as they provide the actual app or game functionality and interaction that makes your game or app what it is.  
 Actors are created inside a scene and belong to that scene. When the scene is destroyed its actors will also be destroyed.

 A CzActor is a very generic object that provides quite a lot of functionality out of the box. The idea is for developers to create their own actor types 
 from the base CzActor class then implement their own custom functionality within its Update() method. The basic functionality provided by CzActor includes:
 - Support for actor pooling to help reduce memory fragmentation
 - Unique names so they can be searched
 - Actor types
 - Size, Margin and Docking
 - Position, Depth, Origin, velocity and velocity damping
 - Angle, angular velocity and angular velocity damping
 - Scale and Colour
 - Draggable state
 - Layers
 - Active and visible states
 - A visual that represents it on screen
 - Animation timeline that can be attached to the visual
 - Collision size / rectangle
 - Wrapping at scenes extents
 - Instantiate itself from XOML
 - Other actor linkage (used to connect actors in a child / parent style system)
 - A Box2D physical body consisting of a material and shape
 - Box2D collision category, mask and group
 - Managing and clean up of actor local animation timelines
 - Managing and clean up of actor local animation timers
 - Managing and clean up of actor local events / actions
 - Data bindings

 CzActor is derived from the IzXomlResource and IzAnimTarget interfaces which enables the actor to be declared in XOML and enables its properties to be 
 targetted and modified by the animation system. The base CzActor has many base properties that define where, how and what the actor appears as in your 
 app / game. For example, all actors have position, angle, scale, colour, events list etc..

 AppEasy comes with many pre-defined actor types that each have their own specific purpose. For example, we have Text Actors (CzActorText) that display text, 
 Image Actors (CzActorImage) that display images, Icon actors (CzUIIcon) that act as buttons and many more. Usually actors have a visual attached which is 
 an object that is derived from CzSprite. CzSprite gives the actor its visual appearance on screen. For example CzBitmapSprite enables you to display the actor 
 as an image whilst CzTextSprite enables you to display the actor as text.

 Actors can also be augmented to give them additional functionality. Actors can be augmented in a number of ways including adding modifiers (IzModifier) 
 to them which modify their behaviour, they can respond to events (CzEvent) with actions (CzActions) which in turn can affect the scene and other actors 
 and they can run XOML programs (CzProgram) or call functions in script (CzScript) languages to create more complex behaviours. Actors can also take 
 full advantage of the built in physics (CzBox2dBody, CzBox2dJoints, CzBox2dWorld, CzBox2dMaterial, CzBox2dCollidable) and animation systems (CzAnim) to 
 truly bring them to life.

 AppEasy provides a large array of different types of actors out of the box, but all actors are derived from 2 basic types of actors:
 - ActorImage - This type of actor enables you to display images or more commonly portions of images within a scene
 - ActorText - This type of actor enables you to display text using a font within a scene

 All other actors that can be created are derived from these two types of actors. Now lets take a brief look at all of the other types of actors that XOML can use:
 - ActorImage - Basic image actor that can display an image or brush
 - ActorText - Basic text actor that can display text using specific fonts
 - ActorParticles - An actor that can generate particles used for special / spot effects
 - Icon - An Image actor that can also be used as a button or check box
 - Label - An image actor that contains a text actor, can also be used as buttons
 - VideoOverlay - An image actor that can display video content
 - TextBox - A label actor that allows text entry via on screen keyboard
 - Slider - Image actor that can be used as a slider control
 - ListBox - A complex actor that displays a list of child actors that can be selected / toggled etc
 - Grid - A complex actor that displays a grid / data grid and allows selection of cells
 - Image View / Text View - Displays an image or text area that can be pinch zoomed and panned
 - Web View - Displays web content
 - Tab Bar - A complex actor that can be used to create navigation between different views
 - Canvas, StackPanel and WrapPanel - Image actors that act as containers that arrange content in specific ways

 As you can see there is a large selection of actors that you can use as-is or as a starting point to create your own actors.

 <h1>Basic Actor Properties</h1>

 <b>General Properties:</b>
 - Name (string) - Name of the actor, used to refer to the actor from scripts and such. Note that no two actors in the same scene should share the same name.
 - Style (style) - Provides a style that this actor should use to style its properties
 - Type (number) - A numerical type that can be used to identify the type of this actor (default is 0)
 - Active (boolean) - Active state (default is true), actors that are not active will not be processed
 - Visible (boolean) - Visible state (default is true), actors that are not visible will not be shown on screen.
 - Tappable (boolean) - If true then this actor will receive touch events when touched (default is true). IsTappable in C++
 - Draggable (boolean) - When set to true the user can drag the actor around the world with their finger (default is false). IsTappable in C++
 - Timeline (timeline) - The time line that should be used to animate the actor
 - Bindings (bindings list) - Name of the bindings set that will be bound to this actors properties
 - Binding (binding) - A simple binding that is bound to a specific property of the actor
 - UserData (number) – A user data, can be used to store anything Visual Properties:
 - Position (x, y) - The actors position within the scene (default is 0, 0)
 - PositionX (number) - The actors position on the x-axis
 - PositionY (number) - The actors position on the y-axis
 - PositionOrg (x, y) - Allows setting of actors position based on the actors original position, the new position will be original_position + offset
 - PositionOrgX (number) - Allows setting of actors position on the x-axis based on the actors original position, the new position will be original_position + offset
 - PositionOrgY (number) - Allows setting of actors position on the y-axis based on the actors original position, the new position will be original_position + offset
 - PercPos (boolean) - When set to true positions are specificity as a percentage of the devices screen size (default is false)
 - Angle (degrees) - The angle of the actor (default is 0)
 - Origin (x, y) - Sets the offset around which the actor will rotate and scale (default is 0,0)
 - Depth (number) - Depth of the actor in 3D (larger values move the sprite further away, default is 1.0 for parent actors and 0.0 for child actors)
 - Scale (x, y) - The x and y axis scale of the actor (default is 1, 1, which represents no scaling)
 - ScaleX, ScaleY (number) - The separate x and y axis scale of the actor
 - Colour (r, g, b, a)- The colour of the actor, Each component red, green, blue and alpha should be between the value of 0 and 255 (0 is no colour, whilst 255
 is full colour, default)
 - Opacity (opacity) - The opacity of the actor (how see-through the actor appears). Opacity value ranges from 0 (invisible) to 255 (fully visible, default)
 - Layer (number) - The visible layer that the actor should appear on (maximum layer number is limited by the number of actor layers defined by the scene)
 - Docking (dock position) - When set will dock the actor to an edge of the screen or canvas, valid values are top, left, right, bottom, topleft, topright, 
 bottomleft and bottomright
 - Margin (left, right, top, bottom) - The amount of space to leave around the actor when placed in a container or docked
 - UseParentOpacity (boolean) - When set to true this actor will scale its own opacity by its parents opacity (default is true)
 - IgnoreCamera (boolean) – If set to true then this actor will ignore the cameras transformation staying in place when when the camera moves, scales or rotates
 (default is false)
 - Orphan (boolean) – If set to true then this actor will ignore its usual parentchild hierarchy when sorting by layer. This enables child actors to be sorted by
 layer with parent actors (default is false)

 <b>Physical Properties:</b>
 - Velocity (x, y) - Initial velocity of the actor
 - VelocityDamping (x, y) - The amount to dampen velocity each frame, values of less than 1.0 will slow the actor down over time, values of greater than 1.0 will
 speed the actor up over time.
 - AngularVelocity (number) - The rate at which the orientation of the actor changes in degrees per second
 - AngularVelocityDamping (number) - The amount of rotational velocity damping to apply each frame
 - WrapPosition (boolean) - If true then the actor will wrap at the edges of the canvas
 - Box2dMaterial (material) - Sets the physical material type used by the Box2D actor
 - Shape (shape) - Box2D fixture shape for that represents this actor during collisions
 - COM (x, y) - Centre of mass of Box2D body
 - Sensor (boolean) - Can be used to set the actor as a sensor (default is false)
 - CollisionFlags (category, mask, group) - Box2D collision flags

 <b>Event Properties:</b>
 - OnBeginTouch (actions list) - When the user begins to touch this actor it fires this event and calls the supplied actions list. Actor also supports
 OnBeginTouch2 to OnBeginTouch5 representing the 2nd to 5th touches on multi-touch devices.
 - OnEndTouch (actions list) - When the user stops touching this actor it fires this event and calls the supplied actions list. Actor also supports OnEndTouch2 to
 OnEndTouch5 representing the 2nd to 5th end touches on multi-touch devices.
 - OnTapped (actions list) - When the user taps this actor it fires this event and calls the supplied actions list. Actor also supports OnTapped2 to OnTapped5
 representing the 2nd to 5th taps on multi-touch devices.
 - OnCreate (actions list) - When this actor is first created it fires this event and calls the supplied actions list
 - OnDestroy (actions list) - When the actor is about to be destroyed it fires this event and calls the supplied actions list
 - OnOrientationChange (actions list) - When the devices orientation changes it fires this event and calls the supplied actions list
 - OnSizeChange (actions list) - When the devices screen size changes it fires this event and calls the supplied actions list (only valid for devices that support dynamic display / window size changing)
 - OnCollisionStart (actions list) - When two actor that have Box2D physics enabled start to collide it fires this event and calls the supplied actions list (only
 if the actor has the iw_notifycollision modifier attached)
 - OnCollisionEnd (actions list) - When two actor that have Box2D physics enabled stop colliding it fires this event and calls the supplied actions list (only
 if the actor has the iw_notifycollision modifier attached)
 - OnTick (actions list) - Provides an actions group that is called every time the scene is updated (30 to 60 times per second)
 - Bubbling (boolean) - When set to true touch events can bubble up from child actors

 <b>Miscellaneous Properties:</b>
 - GridPos (x, y) - Grid cell in which to place the actor in a grid
 - LinkedTo (actor) - Name of actor that this actor links to (advanced)

 <h1>Creating Actors</h1>
 The usual way to create an actor is to choose which type of actor is most appropriate to the task. For example, if you want a simple game character then an CzActorImage 
 will suffice, or maybe you want a button with a background and text, in which case a CzUILabel would be the better choice. Lets take a look at a few different ways to create 
 a label:

 @par XOML Example:
 @code
<Label Font="serif" Background="Button1Brush" Text="Im a label" />
 @endcode

 @par Code Example:
 @code
// Find the font and brush
CzFont* font = (CzFont*)CZ_GLOBAL_RESOURCE_MANAGER->findResource("serif", CzHashes::Font_Hash);
IzBrush* background_brush = (IzBrush*)CZ_GLOBAL_RESOURCE_MANAGER->findResource("Button1Brush", CzHashes::Brush_Hash);
// Create the label
CzUILabel* label = new CzUILabel();
label->setName("label1");
scene->addActor(label);
label->Init(background, 200, 70);
CzIRect rc = CzIRect(0, 0, -100, -100);
label->InitTextActor(font, rc, "Im a label", false);
 @endcode

 @par Code Example (using macros):
 @code
// Find the font and brush
CzFont* font = (CzFont*)CZ_GLOBAL_RESOURCE_MANAGER->findResource("serif", CzHashes::Font_Hash);
IzBrush* background_brush = (IzBrush*)CZ_GLOBAL_RESOURCE_MANAGER->findResource("Button1Brush", CzHashes::Brush_Hash);
// Create the label
CzIVec2 size(200, 70);
CZ_NEW_LABEL(scene, label, "label1", CzString("Im a label"), background_brush, font, size, false);
 @endcode

 @par Lua Script Example:
 @code
local font = resource.find("serif", "font");
local brush = resource.find("Button1Brush", "brush");
local label_actor = actor.create("label1", scene, font, "Im a label", brush, 200, 70);
 @endcode

 As you can see from all of the above examples, object creation in XOML is by far the easiest, most well organised and quickest approach. Lua is the 2nd, code macros the third 
 and pure code last. We focus mainly on the XOML approach throughout most of the AppEasy documentation as it is by far the most efficient.

 <h1>Actor Hierarchies</h1>

 In XOML actors can be declared inside others actors to form an hierarchy (called linking). In this hierarchy actors that are declared inside other actors are called 
 child actors, whilst the container actor is called the parent actor. You will see hierarchical actors used extensively throughout XOML, especially in regards to the 
 user interface system. This child / parent system enables the creation of complex multi-part actors that are built from many actors. In code to make an actor the child 
 of another actor you call setLinedTo() passing in the parent.

 When you place child actors inside a parent actor the child is modified in a number of ways:
 - Position, scale, rotation and depth become relative to the parent actor, so if the parent actor moves around then the child actors will follow. So for example if
 you set the child actors position to 0, 0 then it will be centered at the parent. If the parent rotates or scales then the child will also rotate and scale by the 
 same rate as the parent
 - The opacity of child actors will be scaled by the parent actors opacity if UseParentOpacity="true" was specified in the child actors definition. For example, 
 if you set the parents opacity to half then all child actors will also appear at half opacity
 - When a parent actor is hidden or made visible then all child actors will also be made hidden or visible
 - Child actors no longer obey layer ordering and are instead layered in the order in which they are declared inside. You can override this behaviour by adding
 Orphan=”true” property to the actor definition
 - Child actors will steal input events from the parent actor, if the parent actor is drawn before the child actors, unless the parent has Bubbling="true" set, in
 which case both the child actor and its parent will both receive the input event

 <h1>Absolute / Percentage Positioning and Sizing</h1>

 When you begin developing apps and games for mobile you will quickly discover that you need to deal with a large selection of different screen sizes, orientations 
 and aspect ratios. The scenes virtual canvas can go a long towards helping to alleviate this problem. However (for apps in particular) its often more appropriate 
 to render the user interface at the screens native resolution but use a series of clever layout panels to position and size content.

 Actors can be positioned in a scene using two methods:
 - Absolute Positioned - The position you specify is in absolute or actual scene coordinates
 - Percentage Positioned - The position you specify is a percentage of the devices screen size

 The default mode of actor positioning is absolute. To change an actors positioning mode to percentage based you need to add PercPos=”true” to the actors definition.

 Lets take a look at an example:
 @par XOML Example
 @code
<Label Font="serif" Text="Hello World" Position="10, 20" Size="210, 100" /> - Create a label at 10, 20 of size 200 x 100
<Label Font="serif" Text="Hello World" Position="10, 20" PercPos=”true” Size="210, 100" /> - Create a label at 10%, 20% of size 200 x 100
 @endcode

 As wall as absolute and percentage based positioning the size of an actor can be absolute or percentage based:
 - Absolute Size - The size that you specify is in absolute / actual width and height in scene coordinates
 - Percentage Size - The size that you specify is a percentage of the actors parents size or if the actor has no parent a percentage of the devices screen size

 The default mode of actor sizing is absolute. By passing a negative width or height you switch the actors sizing mechanism to percentage based. Lets take a look at 
 an example:

 @par XOML Example
 @code
<Label Font="serif" Text="Hello World" Position="0, 0" Size="-50, -50" /> - Create a label at 0, 0 of size 50% screen width and height
 @endcode

 Using percentage based positioning and sizing with layout panels enables production of device screen size independent apps and games.

 Its important to note that actors that are positioned / sized using percentages will change position / size when the devices screen orientation changes.

 <h1>Docking and Margins</h1>

 Its often very useful to place an actor somewhere on screen without worrying about the exact position where it needs to be placed. To solve this problem XOML
 introduces the concept of docking. Docking allows actors to be placed at the edge of the screen or the edge of a canvas. Lets take a look at a XOML example that 
 shows actor docking:

 @par XOML Example
 @code
<-- Create a bunch of docked labels:>;
<Label Font="serif" Background="Button1Brush" Size="100, 100" Text="Left" Docking="left" />
<Label Font="serif" Background="Button1Brush" Size="200, 200" Text="Right" Docking="right" />
<Label Font="serif" Background="Button1Brush" Size="150, 150" Text="Top" Docking="top" />
<Label Font="serif" Background="Button1Brush" Size="180, 180" Text="Docked" Docking="bottom" />
 @endcode

 You can see a working example of actor docking by taking a look at the ActorDocking example.

 Note that when the devices screen orientation changes docked actors will re-dock themselves to dock to the new screen edges. When an actor is docked at a screen 
 or canvas edge you do not always want the actor to be appear right up against the edge, sometimes it looks better to leave a little space. XOML provies the Margin 
 property that allows you to specify some space to leave around the actor. You can set the space to leave around an actor by adding Margin=”left space, right space, 
 top space, bottom space”, where left, right, top and bottom space is the amount of space to leave around the actor. Lets revisit our previous example and add a 
 margin to the first actor:

 @par XOML Example
 @code
<Label Font="serif" Background="Button1Brush" Size="100, 100" Text="Left" Docking="left" Margin="20, 0, 0, 0" />
 @endcode

 Although this actor is docked to the left hand side of the screen it is no longer clinging to the edge, instead it is pushed our by 20 units, leaving a nice gap.
 Margins sizes can also be specified using percentage sizing, by making margin values negative you force them to be percentage based. For example:

 @par XOML Example
 @code
<Label Font="serif" Background="Button1Brush" Size="100, 100" Text="Left" Docking="left" Margin="-5, 0, 0, 0" />
 @endcode

 This label now has a gap down its left hand side that is 5% of the screens width

 <h1>Layers</h1>

 Its customary in game and app development to have some kind of visual order in which objects appear. For example, a button should appear over the background that
 it sits on or the foreground in a game world should appear above the background. To accomplish this XOML uses actor layering. Each scene has a number of layers that
 actors can be placed on (layers 0 to 15 by default, although this can be changed in the scene definition). Actors that are placed on higher number layers will appear 
 above actors on lower number layers. To set an actors layer you add the Layer=”layer number” property to the actor definition. e.g:

 @par XOML Example
 @code
<Icon Name=”icon1” Background="Button1Brush" Size="250, 200" Text="Layer 7" Layer="7" />
<Icon Name=”icon2” Background="Button1Brush" Size="500, 200" Text="Layer 1" Layer="1" />
 @endcode

 In this example, icon1 will appear above icon2 because icon1 is on layer 7 whereas icon2 is on layer 1.

 <h1>Actor Origin</h1>

 Sometimes we want objects to move a little differently than the norm. All actors by default spin and scale around their centre when rotated or scaled. We can modify 
 this behaviour by moving the origin of the actor. The origin of an actor is the point around which the actor will spin and scale and by moving this origin we can 
 change the position around which it spins and scales.

 The ActorChildren example shows an example of moving the origin to change how one actor spins around another. If you pay attention to the small green cube that is
 orbiting Child 4 you will notice that it orbits Child 4 much like a space craft would orbit a planet. Lets take a look at the XOML for this green cube actor:

 <Icon Origin="0, 80" Background="Button1Brush" BackgroundColour="80, 255, 80, 255" Size="20, 20" AngularVelocity="7.5" />

 In this example, we add the Origin=”0, 80” to the icon definition which pushes its centre of rotation 80 units downwards, this in turn causes the actor to have an 
 orbital distance of 80 units from the centre of the parent actor.

 <h1>Animation</h1>

 Because there is a lot of focus on actors in XOML, XOML provides a number of ways to animate them. Actors can be assigned an animation timeline which contains many
 animations that target its properties for example (animations and timelines are covered in the animation section). Actors can also be placed under control of the
 physics system (covered in a later section) Besides timelines and physics, actors can be animated using linear and angular velocity. If we take a look our basic 
 actor properties again we notice two particularly interesting properties:
 - Velocity (x, y) - Initial velocity of the actor
 - AngularVelocity (number) - The rate at which the orientation of the actor changes in degrees per second

 These two properties allow us to set an initial linear velocity (moves the actor in a direction) and angular velocity (spins the actor at a set speed).
 If you one again take a look at the ActorChildren example, you will notice that each actor has an AngularVelocity property defined:

 @par XOML Example
 @code
<Label Position="0, 0" Font="serif" Background="Button1Brush" BackgroundColour="80, 80, 255, 255" Size="100, 100" Text="Parent" AngularVelocity="1">
 @endcode

 By setting this property we can et the actor off spinning as soon as it comes into the game world.

 <h1>Dragging Actors</h1>

 XOML provides a neat little feature for all actors called dragging. Any actor can be marked as dragabble by adding Draggable=”true” to the actor definition. 
 Marking the actor as draggable allows the user to drag the actor around the screen using their finger.

 <h1>Physics</h1>

 There's nothing like adding realistic physics to a game to bring it to life and increase its immersion factor. To that end XOML provides out of the box physics 
 via Box2D. Any actor can be made to use physics by simply assigning a shape and a Box2D material to it in the actor definition. This includes all actors including 
 user interface components! Lets have a quick recap of the available properties that affect the physics of our actor:
 - Velocity (x, y) - Initial velocity of the actor
 - VelocityDamping (x, y) - The amount to dampen velocity each frame, values of less than 1.0 will slow the actor down over time, values of greater than 1.0 will
 speed the actor up over time.
 - AngularVelocity (number) - The rate at which the orientation of the actor changes in degrees per second
 - AngularVelocityDamping (number) - The amount of rotational velocity damping to apply each frame
 - WrapPosition (boolean) - If true then the actor will wrap at the edges of the canvas
 - Box2dMaterial (material) - Sets the physical material type used by the Box2D actor
 - Shape (shape) - Box2D fixture shape for that represents this actor during collisions
 - COM (x, y) - Centre of mass of Box2D body
 - Sensor (boolean) - Can be used to set the actor as a sensor
 - CollisionFlags (category, mask, group) - Box2D collision flags
 
 Now would be a good to open up the ActorPhysics example that has been provided. Lets take a quick look at some of the important parts of the XOML for this example:

 @par XOML Example
 @code
<Scene Name="Scene1" Current="true" Physics="true" WorldScale="1, 1" Gravity="0, 30" Camera="Camera1"> - Create a scene with physics enabled
 @endcode

 Firstly we create a scene that can support physics by enabling physics, settings the worlds scale to 1, 1 (this means 1 scene unit is 1 physical unit) and then we 
 set the gravity to act downwards.

 @par XOML Example
 @code
<-- Create Box2D materials -->;
<Box2dMaterial Name="Bouncey" Type="dynamic" Density="1.0" Friction="0.3" Restitution="0.6" /> 
<Box2dMaterial Name="Heavy" Type="static" Density="2.0" Friction="0.8" Restitution="0.8" />
 @endcode

 Next we create two box2d materials. The first is quite boucey and will be used by our bouncey box. The second material is quite solid and will represent out floor. 
 It is also marked as a static materuial because we do not expect the floor to move. The first material is marked as dynamic becuase we expect our boncey box to move.

 @par XOML Example
 @code
<-- Create Box2D shapes -- >;
<Shape Name="Button" Type="box" Width="100" Height="100" />
<Shape Name="Floor" Type="box" Width="1000" Height="100" />
 @endcode

 Next we create two shapes (thes represent the physical shape of our objects). In this example we have a small box 100x100 units in size that represents our bouncey 
 box and a much larger 1000x100 box shape that represents our solid floor.

 @par XOML Example
 @code
<Label Position="0, 200" Font="serif" Background="Button1Brush" BackgroundColour="255, 80, 80, 255" Size="1000, 100" Text="Floor" Shape="Floor" Box2dMaterial="Heavy"
CollisionFlags="1, 1, 1" /> - Create the floor
<Label Position="-50, -180" Font="serif" Background="Button1Brush" BackgroundColour="80, 255, 255, 255" Size="100, 100" Text="Bouncey" Shape="Button" Box2dMaterial="Bouncey"
CollisionFlags="1, 1, 1" /> - Create an actor to drop onto the floor
 @endcode

 Lastly we create a bunch of actors (we have only included the floor and one box here). The first actor (Floor) represents our floor and is assigned the Floor shape and
 the Heavy material. The second actor (Bouncey) is assigned the Button shape and Bouncey material.

 You may by now have noticed an additional property called CollisionFlags. Collision flags are described as:
 - Category - The category bits describe what type of collision object the actor is
 - Mask - The mask bits describe what type of other collision objects this actor can collide with
 - Group - The group index flag can be used to override category and mask, but we generally do not need to use it and usually set it to 0 for all actors.

 The ActorCollisionFlags example has been provided to show how collision flags work.

 The centre of mass (COM) of an actor in simple terms is the point at which the mass of the object is balanced. If the centre of mass of an object is directly at its 
 centre then when hit towards its centre of mass by another object will generally cause it to spin around its centre. We can move the centre of mass of an actor to give 
 the impression that the mass is centred elsewhere. For example, we may want to move the centre of mass towards the bottom of a box to make it look like it is weighted 
 at the bottom. We can move an actors centre of mass by setting the actors COM attribute to the position of the centre of mass.
 
 Lastly, we can mark actors as being sensors instead of interactive physical objects. When an actor collides with a sensor the sensor does not affect the actor and the 
 actor does not physically affect the sensor. In essense a sensor is there just to detect that something has collided with it. This type of actor is good for actors that 
 represent switches and other types of none interactive actors. To mark an actor as a sensor set Sensor=”true” in the actor definition.

 <h1>Modifiers</h1>

 Modifiers can be thought of as small functional building blocks that can be stacked in an actor or scene to extend the functionality of that actor or scene. For example, 
 a typical modifier for a scene could be one that tracks the players scores / hi-scores, change day / night settings or detects special gestures. An actor modifier 
 example could be a modifier that allows the actor move around using heading and speed or even a modifier with functionality specific to your game such as make a baddy 
 that walks left and right on a platform. Lets take a look at how we add a modifier to an actor:

 @par XOML Example
 @code
<ActorImage Name="Car" ........ >
    <Modifiers>
        <Modifier Name="iw_notifycollision" Active="false" Param1="0" />
    </Modifiers>
</ActorImage>
 @endcode

 In this example we add the iw_notifycollision modifier which allows the actor to respond to collision events between actors.

 A modifier accepts the following properties:
 - Name - Name of the modifier
 - Active - Active state of the modifier
 - Param1 to Param4 - Parameters that can be passed to the modifier when it is initialised

 At the moment only a few different modifiers are available for actors but more will be added over time. Lets take a look at the modifiers that are currently 
 available.
 - Collision Notification Modifier - The iw_ notifycollision modifier when attached to an actor allows it to generate and respond to collision events between actors 
 using OnCollisionStart() and OnCollisionEnd() event handlers (See ActorModifier example for an example showing how to use this modifier). Ths modifier accepts a number 
 of parameters which include:
   - Param1 - An optional mask that can be used to mask collision with actors by their Type. The value supplied will mask actors by type and only allow collision events 
   to be called for those actors that pass the bit mask. For example actor 1 could have a mask of 3 and actor 2 a mask of 1. if the mask is set to 1 then both actors can 
   collide, but if the mask was set to 3 then they could not.
 - Script Modifier - the iw_callscript modifier when attached to an actor will call a function in a script each time the actor is updated. This modifier is useful as
 an alternative to using OnTick event handlers. This modifier accepts the following parameters:
   - Param1 - Script function name
   - Param2 to Param3 - Parameters to be passed to the script function

 Future out of the box functionality will be added to XOML using modifiers.

 <h1>Scripts</h1>

 Whilst XOML is very powerful it does have some limits when it comes to defining very complex application / app logic, for example path finding algorithms would be
 quite cumbersome to create using XOML. Actors support the calling of script functions via actions in response to various events occurring. Also, as shown in the previous 
 section, a modifier can be added to an actor that automatically calls script functions every time the actor is updated via the OnTick event, for example:

  @par XOML Example
 @code
<Label Font="serif" Text="OnTick Example" OnTick="Update">
	<Actions Name="Update">
		<Action Method="CallScript" Param1="Actor_OnTick" />
	</Actions>
</Label>
 @endcode

 <h1>Events</h1>

 Actors can receive and react to a number of different types of events including:
 - OnBeginTouch to OnBeginTouch5 - The user has began to touch the actor (up to 5 simultaneous touches)
 - OnEndTouch to OnEndTouch5 - The user has stopped touching the actor (up to 5 simultaneous touches)
 - OnTapped to OnTapped5 - The user tapped the actor (up to 5 simultaneous touches)
 - OnCreate - The actor has been created
 - OnDestroy - The actor is about to be destroyed
 - OnOrientationChange - A screen orientation or size change has just taken place
 - OnCollisionStart - A collision with another actor has begun
 - OnCollisionEnd - A collision with another actor has ended
 - OnTick - The actor is being updated

 All touch events can bubble up to parent actors if the parent actor has event bubbling enabled. Event bubbling is the process of passing the same event on up to actors further up 
 the parent-child heirearchy. Lets take a quick look at an example actor that responds to a number of different events:

 @par XOML Example
 @code
<Label Font="serif" Text="Tap Me" Background="Button1Brush" OnTapped="Tapped" OnCreate="Created">
	<Actions Name="Created">
		<Action ....... />
		<Action ....... />
	</Actions>
	<Actions Name="Tapped">
		<Action ....... />
		<Action ....... />
	</Actions>
</Label>
 @endcode
 
 <h1>Data Bindings</h1>
 AppEasy supports the binding of XOML variables to properties of actors. When the variable is changed the target property of the actor is automatically updated. Actors can use bindings 
 lists or simple bindings. A bindings list is a collection of bindings that bind specific variables to specific properties of the actor. Lets take a look at a short XOML example:

 @par XOML Example
 @code
<!-- Create a variable that holds the labels position -->;
<Variable Name="LabelPosition" Type="vec2" Value="0, 0" />

<!-- Create a variable that holds the labels angle -->;
<Variable Name="LabelAngle" Type="float" Value="0" />

<!-- Create a data bindings list -->;
<Bindings Name="LabelBindings">
	<Binding Property="Position" Variable="LabelPosition" />
	<Binding Property="Angle" Variable="LabelAngle" />
</Bindings>

<!-- Create a label with bound data -->;
<Label Font="serif" Text="Tap Me" Background="Button1Brush" Bindings="LabelBindings" />
 @endcode

 In the above example we create two variables, a bindings list and a label. In the bindings list we bind LabelPosition to the Position property and LabelAngle to the Angle property. 
 We then assign the bindings list to the label actor using Bindings="LabelBindings". Now any changes to LabelPosition / LabelAngle will automatically be applied to the actors Position 
 and Angle properties. 

 Simple data bindings work in much the same way except a simple binding is the binding of a single variable to a single property. Lets take a look:

 @par XOML Example
 @code
<!-- Create a variable to track number of collisions -->;
<Variable Name="num_collisions" Type="int" Value="0" />
		
<!-- Create a label to display number of collisions -->;
<Label Font="serif" Background="Button1Brush" Docking="topleft" Binding="[Text]num_collisions" />
 @endcode

 In this example we speciofy the property "Text" by including the prtoperty name in square brackets [Text], we then tag on the name of the bound variable. Only a single simple 
 binding may be attached to an actor. Actors can have a bindings list and a simple binding at the same time. Note that a simple binding bound to the same property as a bindings list 
 will take precedence over the property in the bindings list. Also note that some actor properties are two way bindings, so the actor can write values back to the variable. 

 <h1>Creating a Custom Actor</h1>
 Whilst CzScene can be instantiated and used as-is, CzActor is abstract and cannot. The actor system is designed this way to allow th developer to create their own custom actor 
 types that provide bespoke functionality that is specific to their game or app. Whilst AppEasy Core provides a multitide of actor types as well as ways of extending actors with the 
 likes of modifiers (see IzModifier), in some cases the required functionality could be better achieved by deriving a new type of actor from one of the base actor types.

 You begin the creation of a custom actor by deriving your own actor class from one of the actor classes then overloading the following methods to provide implementation:
- void Init()
- bool Update(float dt)
- bool UpdateVisual()

 Here's a quick example:

@code
class MyActor : public CzActor
{
public:
	MyActor() : CzActor() {}
	~MyActor() {}

	void		Init()
	{
		CzActor::Init();
	}
	
	bool		Update(float dt)
	{
		if (!CzActor::Update(dt))
			return false;

		// Here we put our actor specific implementation

		return true;
	}

	bool		UpdateVisual()
	{
		if (!CzActor::UpdateVisual())
			return false;

		// Here we put our actor specific rendering code (if any is needed)

		return true;
	}
};
@endcode

 We have provided a very basic implementation of Init(), Update() and UpdateVisual() which call the base CzActor class methods so we keep its functionality in-tact.

 You can take the implementation one step further by implementing both the IzXomlResource and  IzAnimTarget interfaces to allow instantiation of your custom actor class 
 from XOML and to allow your class to be a target for animation time lines.

 Firstly lets take a look at XOML enabling your custom actor class. To get AppEasy to recognise your actor class whilst parsing XOML files you need to do a few things:
 - Derive your class from IzXomlResource class and implement the LoadFromXoml method
 - Create a class creator that creates an instance of your class then add this to the XOML engine

 Lets start by taking a look at step 1.

 Because we have derived our class from CzActor (which is derived from IzXomlResource) we already have the support for step 1. However we would like to insert our own 
 custom attribute tags so we need to make a few changes.

 Lets take a look at our new class with thiose changes:

@code
class MyActor : public CzActor
{
public:
	// Properties
protected:
	int			NumberOfEyes;
public:
	void		setNumberOfEyes(int num_eyes)	{ NumberOfEyes = num_eyes; }
	float		getNumberOfEyes() const			{ return NumberOfEyes; }
	// Properties End
public:
	MyActor() : CzActor() {}
	~MyActor() {}

	void		Init()
	{
		CzActor::Init();
	}
	
	bool		Update(float dt)
	{
		if (!CzActor::Update(dt))
			return false;

		// Here we put our actor specific implementation

		return true;
	}

	bool		UpdateVisual()
	{
		if (!CzActor::UpdateVisual())
			return false;

		// Here we put our actor specific rendering code (if any is needed)

		return true;
	}

	// Implementation of IzXomlResource interface
	bool			LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
	{
		if (!CzActor::LoadFromXoml(parent, load_children, node))	// Notice how we call the base implementation of LoadFromXoml to ensure that any base actor properties also get loaded
			return false;

		// Add our own custom attribute parsing
		for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
		{
			unsigned int name_hash = (*it)->getName().getHash();

			if (name_hash == CzString::CalculateHash("NumberOfEyes"))
			{
				setNumberOfEyes((*it)->getValueAsInt());
			}
		}

		return true;
	}
};
@endcode


 Our new class now basically supports a new NumberOfEyes attribute that we will eventually be able to set in XOML using something like:

@code
<MyActor Name="AlienCritter" Position="100, 100" Size="100, 100" NumberOfEyes="3" />
@endcode

 However, before we can do that we need to let the XOML system know about our new type of class (MyActor), so it can be instantiated when the XOML parser comes across it. To do this 
 we need to create a XOML class creator:

@code
class MyActorCreator : public IzXomlClassCreator
{
public:
	MyActorCreator()
	{
		setClassName("MyActor");
	}
	IzXomlResource* CreateInstance(IzXomlResource* parent) { return new MyActor();	}
};
@endcode

 The creator basically defines the tag name "MyActor" and returns an instance of the MyActor class when CreateInstance() is called. 

 To get the XOML system to recognise our new creator we need to add it to the XOML parsing system using:

@code
// Add custom MyActor to XOML system
CZ_XOML->addClass(new MyActorCreator());
@endcode

 Now XOML integration is out of the way, lets take a quick look at enabling our class as an animation target.

 To enable a class as an animation target we derive it from IzAnimTarget and implement the UpdateFromAnimation() method. Luckily we derived our MyActor class from the CzActor class which 
 already provides this functionality. Lets take a quick look at how we extend the animation update method to account for animating our NumberOfEyes variable.

@code
bool			UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzActor::UpdateFromAnimation(animation))	// Notice how we call the base implementation of UpdateFromAnimation to ensure that any base actor properties also get animated
		return true;

	// Add our own custom animating property
	unsigned int property_name = animation->getTargetPropertyHash();

	if (property_name == CzString::CalculateHash("NumberOfEyes"))
	{
		CzAnimFrameFloat* frame = (CzAnimFrameFloat*)animation->getCurrentData();
		setNumberOfEyes((int)frame->data);
		return true;
	}

	return false;
}
@endcode

 We added the above code to our MyActor class definition. We begin by calling the base UpdateFromAnimation() method so we can keep the existing animation properties of the actor. 
 We then add our own custom check for the NumberOfEyes variable. If the animation property matches NumberOfEyes  then we set the number of eyes to the provided interpolated value.

 @todo
 - setProperty() needs to be replaced with CzXomlProperty instead of CzString as string conversion has to take place each time a property is set.
 - Refactor setProperty(), UpdateFromAnimation(). UpdateBinding() and LoadFromXoml() so they all use a centralised update property method.
 - Optimise setProperty(), getProperty(), UpdateFromAnimation(), UpdateBinding() and LoadFromXoml() to perform less comparisons (divide search by first character of property name.
 - Make property system more extensible by replacing current if-then-else system with a new class property system that stores a list of available properties along with which static methods 
 should be called to modify them. This will enable users to more easily extend derived actors.

 */
 
class CzActor : public IzXomlResource, public IzAnimTarget
{
public:
	/**
	 @enum	eCzActorCategory
	
	 @brief	Values that define the category of the actor
	 */
	enum eCzActorCategory
	{
		AC_Generic,								///< Actor is a generic type of actor (CzActorImage or CzActorText)
		AC_UI,									///< Actor is a UI actor such as CzUIIcon, CzUILabel etc..
	};


protected:
	// Properties
	CzScene*			Scene;					///< Scene that actor lives in
	eCzActorCategory	Category;				///< Category of actor
	int					Type;					///< User defined type of Actor (use to distinguish beteeen different actor types)
	CzIVec2				Size;					///< Visual size
	CzVec2				OriginalPosition;		///< Original position of actor in the scene (when actor was first spawned)
	CzVec2				Position;				///< Current position of actor in the scene
	CzScene::eDocking	Docking;				///< Docking position
	CzIRect				Margin;					///< Margin (spacing around the actor)
	float				Depth;					///< Parallax depth
	CzVec2				Origin;					///< Origin of actor
	CzVec2				Velocity;				///< Current velocity of actor
	CzVec2				VelocityDamping;		///< Dampens the velocity (Values less than 1.0f will reduce velocity over time, whilst values larger than 1.0f will increase velocity over time)
	float				OriginalAngle;			///< Original angle in scene (when first spawned)
	float				Angle;					///< Orientation in scene (degrees)
	float				AngularVelocity;		///< Angular velocity
	float				AngularVelocityDamping;	///< Angular velocity damping
	CzVec2				OriginalScale;			///< Original scale of actor in the scene (when actor was first spawned)
	CzVec2				Scale;					///< Scale
	CzColour			OriginalColour;			///< Original colour
	CzColour			Colour;					///< Colour
	int					Layer;					///< Depth layer
	bool				Orphan;					///< Actors orphaned status
	bool				Used;					///< Used is used when actors pooled to reduce memory fragmentation
	bool				IsActive;				///< Active state of actor
	bool				IsVisible;				///< Visible state of actor
	bool				IsCollidable;			///< Collidable state of actor
	bool				IsTappable;				///< Tappable state of actor
	bool				IsDraggable;			///< Draggable state of actor
	bool				IsDragging;				///< Dragging state of actor
	bool				ReceiveEventFromChild;	///< If true then this actor will receieve events from its children
	bool				WrapPosition;			///< If true then if position exits the extents of the scene actor will wrap back around to the opposite side of the scene
	bool				Destroyed;				///< An actor is marked asd destroyed when it has been marked for deletion
	bool				ScreenDocking;			///< if true then the actors docking value will be applied to the screen and not any parent containers that support edge docking
	bool				HoldFocus;				///< When set to true this actor will not lose focus when the user moves out of its range
	bool				UseParentOpacity;		///< If true then this actors visuals opacity will be scaled by its parent, if it has one
	bool				PercentagePosition;		///< If true then position 
	bool				IgnoreCamera;			///< If true then actor will not move with scene camera
	int					RenderAs;				///< Type of object to render as (0 - no override, 1 - arc, 2 - rect)
	eCzAspectLock		AspectLock;				///< Determines which acis to use to lock aspect ratio of actors size
	CzSprite*			Visual;					///< Visual element that represents the actor
	CzAnimTimeline*		Timeline;				///< Timeline, controls animation of the actor
	CzXomlBindings*		Bindings;				///< Manages any attached bindings
	CzXomlBinding*		SimpleBinding;			///< A simple one property binding
	int					CollisionSize;			///< Size of collision area
	CzIRect				CollisionRect;			///< Spherical collision size
	float				PreviousAngle;			///< Previous updates angle
	CzVec2				PreviousPosition;		///< Previous updates position
	CzActor*			LinkedTo;				///< Actor that this actor links to. Linked actors will utilise their target links colour, visibility and transform etc..
	CzActor*			Target;					///< An actor target
	int					TouchIndex;				///< Index of last touch on this actor
	CzBox2dBody*		Box2dBody;				///< Box2D physics body
	CzUserPropertyList*	UserPropertyList;		///< User properties list
	CzActionsManager*	ActionsManager;			///< Manages actions
	CzAnimTimelinesManager* TimelinesManager;	///< Manages timelines
	CzTimersManager*	TimersManager;			///< Manages attached timers
	CzEventManager* EventsManager;				///< List of events that the actor handles
	CzModifierManager*	Modifiers;				///< Class modifiers manager
	unsigned int		GridPos;				///< x, y position in grid
	void*				UserData;				///< Extra custom data
	bool				TickEnabled;			///< True if OnTick event specified
public:
	void				setCategory(eCzActorCategory cat)		{ Category = cat; }
	eCzActorCategory	getCategory() const						{ return Category; }
	void				setUsed(bool in_use)					{ Used = in_use; }
	bool				isUsed() const							{ return Used; }
	void				setScene(CzScene* scene)				{ Scene = scene; }
	CzScene*			getScene()								{ return Scene; }
	void				setType(int type)						{ Type = type; }
	int					getType() const							{ return Type; }
	CzIVec2				getSize() const							{ return Size; }
	void				setOriginalPosition(float x, float y)	{ OriginalPosition.x = x; OriginalPosition.y = y; }
	CzVec2				getOriginalPosition()					{ return OriginalPosition; }
	void				setPosition(float x, float y);
	void				setPosition(const CzVec2& pos)			{ setPosition(pos.x, pos.y); }
	void				setPositionX(float x)					{ setPosition(x, Position.y); }
	void				setPositionY(float y)					{ setPosition(Position.x, y); }
	void				setDocking(CzScene::eDocking docking);
	void				setDocking(CzScene::eDocking docking, int width, int height);
	CzScene::eDocking getDocking()								{ return Docking; }
	void				setMargin(int left, int right, int top, int bottom)	{ Margin.x = left, Margin.y = right, Margin.w = top, Margin.h = bottom; }
	CzIRect				getMargin() const						{ return Margin; }
	void				setOriginalMargin(int left, int right, int top, int bottom)	{ OriginalMargin.x = left, OriginalMargin.y = right, OriginalMargin.w = top, OriginalMargin.h = bottom; }
	CzVec2				getPosition()							{ return Position; }
	void				setDepth(float depth)					{ Depth = depth; }
	float				getDepth() const						{ return Depth; }
	void				setOrigin(float x, float y)				{ Origin.x = x; Origin.y = y; }
	CzVec2				getOrigin()								{ return Origin; }
	void				setOriginalAngle(float angle)			{ OriginalAngle = angle; }
	float				getOriginalAngle()						{ return OriginalAngle; }
	void				setAngle(float angle);
	float				getAngle() const						{ return Angle; }
	void				setVelocity(float x, float y);
	CzVec2				getVelocity() const						{ return Velocity; }
	void				setVelocityDamping(float x, float y);	// When using Box2d, the y value is ignored
	CzVec2				getVelocityDamping() const				{ return VelocityDamping; }
	void				setVelocityDamping(float damping);
	void				setAngularVelocity(float velocity);
	float				getAngularVelocity() const				{ return AngularVelocity; }
	void				setAngularVelocityDamping(float damping);
	float				getAngularVelocityDamping() const		{ return AngularVelocityDamping; }
	void				setOriginalScale(float x, float y)		{ OriginalScale.x = x; OriginalScale.y = y; }
	CzVec2				getOriginalScale()						{ return OriginalScale; }
	void				setScale(const CzVec2& scale)			{ Scale = scale; }
	void				setScale(float x, float y)				{ Scale.x = x; Scale.y = y; }
	void				setScale(float scale)					{ Scale.x = Scale.y = scale; }
	CzVec2				getScale() const						{ return Scale; }
	void				setColour(uint8 r, uint8 g, uint8 b, uint8 a)	{ Colour.r = r; Colour.g = g; Colour.b = b; Colour.a = a; }
	void				setColour(const CzColour& colour)		{ Colour = colour; }
	CzColour			getColour() const						{ return Colour; }
	CzColour			getOriginalColour() const				{ return OriginalColour; }
	void				setOpacity(uint8 opacity)				{ Colour.a = opacity; }
	int					getOpacity() const						{ return Colour.a; }
	void				setLayer(int layer)						{ Layer = layer; }
	int					getLayer() const						{ return Layer; }
	void				setActive(bool active);
	bool				isActive() const						{ return IsActive; }
	virtual void		setVisible(bool visible, bool force_set = false);
	bool				isVisible() const						{ return IsVisible; }
	void				setCollidable(bool collidable);
	bool				isCollidable() const					{ return IsCollidable; }
	void				setTappable(bool tappable)				{ IsTappable = tappable; }
	bool				isTappable() const						{ return IsTappable; }
	void				setDraggable(bool draggable)			{ IsDraggable = draggable; }
	bool				isDraggable() const						{ return IsDraggable; }
	bool				isDragging() const						{ return IsDragging; }
	void				setReceiveEventFromChild(bool enable)	{ ReceiveEventFromChild = enable; }
	bool				canReceiveEventFromChild() const		{ return ReceiveEventFromChild; }
	void				setVisual(CzSprite* visual)				{ Visual = visual; }
	CzSprite*			getVisual()									{ return Visual; }
	void				removeVisual();
	void				setTimeline(CzAnimTimeline* timeline);
	CzAnimTimeline*		getTimeline()							{ return Timeline; }
	void				setBindings(CzXomlBindings* bindings);
	CzXomlBindings*		getBindings()							{ return Bindings; }
	void				setSimpleBinding(CzXomlBinding* binding);
	CzXomlBinding*		getSimpleBinding()						{ return SimpleBinding; }
	void				setCollisionRect(const CzIRect& rect);
	void				setCollisionSize(int size);
	CzIRect				getCollisionRect() const				{ return CollisionRect; }
	int					getCollisionSize() const				{ return CollisionSize; }
	void				setPreviousPosition(float x, float y)	{ PreviousPosition.x = x; PreviousPosition.y = y; }
	CzVec2				getPreviousPosition() const				{ return PreviousPosition; }
	void				setPreviousAngle(float angle)			{ PreviousAngle = angle; }
	float				getPreviousAngle() const				{ return PreviousAngle; }
	void				setWrapPosition(bool enable)			{ WrapPosition = enable; }
	bool				getWrapPosition() const					{ return WrapPosition; }
	void				setLinkedTo(CzActor* actor);
	CzActor*			getLinkedTo()							{ return LinkedTo; }
	void				setTarget(CzActor* target)				{ Target = target; }
	CzActor*			getTarget()								{ return Target; }
	void				setDestroyed(bool destroyed)			{ Destroyed = destroyed; if (Destroyed) NotifyDestroy(); }
	bool				isDestroyed() const						{ return Destroyed; }
	void				setBox2dBody(CzBox2dBody* body)	{ Box2dBody = body; Box2dBody->setUserData(this); }
	CzBox2dBody*		getBox2dBody()							{ return Box2dBody; }
	CzActionsManager*	getActionsManager()						{ return ActionsManager; }
	CzAnimTimelinesManager* getTimelinesManager()				{ return TimelinesManager; }
	CzTimersManager*	getTimersManager()						{ return TimersManager; }
	CzEventManager*		getEventsManager()						{ return EventsManager; }
	void				setUserPropertyList(CzUserPropertyList* props)	{ SAFE_DELETE(UserPropertyList) UserPropertyList = props; }
	CzUserPropertyList*	getUserPropertyList()					{ return UserPropertyList; }
	void				setModifiers(CzModifierManager* mods)	{ Modifiers = mods; }
	CzModifierManager*	getModifiers()							{ return Modifiers; }
	virtual bool		setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool				setProperty(const char* property_name, const CzString& data, bool delta);
	virtual bool		setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool				getProperty(const char* property_name, CzXomlProperty& prop);
	virtual bool		getProperty(unsigned int property_name, CzXomlProperty& prop);
	void				setGridPos(int x, int y);
	unsigned int		getGridPos() const						{ return GridPos; }
	void				setScreenDocking(bool enabled)			{ ScreenDocking = enabled; }
	bool				getScreenDocking() const				{ return ScreenDocking; }
	void				setHoldFocus(bool hold)					{ HoldFocus = hold; }
	bool				getHoldFocus() const					{ return HoldFocus; }
	void				setTouchindex(int index)				{ TouchIndex = index; }
	int					getTouchIndex() const					{ return TouchIndex; }
	void				setUseParentOpacity(bool enable)		{ UseParentOpacity = enable; }
	bool				getUseParentOpacity() const				{ return UseParentOpacity; }
	void				setAspectLock(eCzAspectLock lock)		{ AspectLock = lock; }
	eCzAspectLock		getAspectLock() const					{ return AspectLock; }
	void				setPercPos(bool enable)					{ PercentagePosition = enable; }
	bool				getPercPos() const						{ return PercentagePosition; }
	void*				getUserData()							{ return UserData; }
	void				setUserData(void *data)					{ UserData = data; }
	bool				getOrphan() const						{ return Orphan; }
	void				setOrphan(bool enable)					{ Orphan = enable; if (Visual != NULL) Visual->setOrphan(Orphan); }
	bool				getIgnoreCamera() const					{ return IgnoreCamera; }
	virtual void		setIgnoreCamera(bool enable)			{ IgnoreCamera = enable; }
	void				setTickEnabled(bool enabled)			{ TickEnabled = enabled; }
	bool				isTickEnabled() const					{ return TickEnabled; }
	void				setOriginalSize(int w, int h)			{ OriginalSize.x = w; OriginalSize.y = h; }
	void				setRenderAs(int render_as)				{ RenderAs = render_as; }
	int					getRenderAs() const						{ return RenderAs; }
	// Properties end
protected:
	CzIVec2				OriginalSize;										///< Original visual size
	CzIRect				OriginalMargin;										///< Original margin
	CzIVec2				DragAnchor;
	virtual bool		UpdateBinding(unsigned int property_name, CzXomlVariable* var);
	virtual void		UpdateBindings(bool force_modified = false);		///< Update data bindings
	virtual void		LinkChanged(CzActor* child, bool remove) {}			///< Called from an actor when an actor that links to this actor unlinks itself
	CzActor*			FindEventDependentParent();							///< Events can be set up to bubble down to parents, this method finds parents that is enabled to receive touch events
	CzActor*			FindClipper();										///< Tracks back through actor parents to find any that change the clipping rect
	void				UpdateMargins();									///< Updates margin szie from original margin (used when orientation changes take place)

public:
	CzActor() : IzXomlResource(), Used(false), Scene(NULL), Visual(NULL), Timeline(NULL), EventsManager(NULL), Modifiers(NULL), LinkedTo(NULL), Bindings(NULL), SimpleBinding(NULL), ActionsManager(NULL), TimelinesManager(NULL), 
	PercentagePosition(false), UserData(NULL), Orphan(false), IgnoreCamera(false), UserPropertyList(NULL) , Box2dBody(NULL), TimersManager(NULL), RenderAs(0)
	{
		setClassType("actor");
		Reset();
		ActionsManager = new CzActionsManager();
		ActionsManager->setParent(this);
		TimelinesManager = new CzAnimTimelinesManager();
		TimelinesManager->setParent(this);
		TimersManager = new CzTimersManager();
		TimersManager->setParent(this);
		EventsManager = new CzEventManager();
#if defined(CZ_ENABLE_METRICS)
		CzMetrics::TotalActorsCreated++;
#endif
	}
	virtual				~CzActor();

	// Initialise the actor
	virtual void		Init();
	virtual void		SetFromBrush(IzBrush* brush);
	
	// Reset the actor (used by memory pooling systems to save actor re-allocation, usually called after re-allocation to reset the object to a default state)
	virtual void		Reset();

	// Update the actor
	virtual bool		Update(float dt);

	// Update the visual that represents this actor on screen
	virtual bool		UpdateVisual();

	// Actors are responsible for carrying out there own collision checks. Called after all actor updates to check and resolve any collisions
	virtual void		ResolveCollisions();

	// NotifyCollision is called by another actor when it collides with this actor
	virtual	void		NotifyCollision(CzActor* other);

	// Hit test
	virtual CzActor*	HitTest(float x, float y);

	virtual bool		isOutsideFocusRange(float x, float y, float scale = 1.0f);

	// Checks to see if another actor is colliding with this actor
	bool				TestOverlap(CzActor* other);
	bool				TestOverlapRect(CzActor* other);

	// Event handlers
	virtual void		ProcessEventActions(unsigned int event_name);
	virtual void		NotifyTapped(int index, int x, int y, bool allow_bubble);
	virtual void		NotifyBeginTouch(int index, int x, int y, bool allow_bubble);
	virtual void		NotifyEndTouch(int index, int x, int y, bool allow_bubble);
	virtual void		NotifyCreate();
	virtual void		NotifyDestroy();
	virtual void		NotifyOrientationChange(CzScene::eOrientation old_orientation, CzScene::eOrientation new_orientation);
	virtual void		NotifySizeChange();

	// Implementation of IzXomlResource interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);
	bool				PostLoadFromXoml(IzXomlResource* parent, CzXmlNode* node);

	// Implementation of IzAnimTarget interface
	bool				UpdateFromAnimation(CzAnimInstance *animation);

	// Utility
	void				RemoveBody();
	CzVec2				PercPosToPos(float x, float y);
	float				getDistanceBetween(CzActor* actor);
	float				getAngleBetween(CzActor* actor);
	int					FindChildren(CzVector<CzActor*>& children);
	void				RemoveChildren();
	void				BringActorToFront();
	void				CalculateSizes(int& width, int& height);			///< Calculate size of actor if parent or screen if size not specified
	void				CalculateMargins(CzIRect& margin);					///< Calculate size of margin from parent or screen if size not specified

	// Coordinate tranoform
	CzVec2				TransformPoint(float x, float y);			// Transform point by actors local angle / scale transform
	CzVec2				TransformPointToScreen(float x, float y);	// Transform point by actors local angle / scale transform to screen coordinates

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	ActorClassDef;								// XOML class definition
public:
	static void				InitClass();
	static void				ReleaseClass();
	static bool				_setName(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getName(IzXomlResource* target);
	static bool				_setType(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getType(IzXomlResource* target);
	static bool				_setTag(IzXomlResource* target, const CzXomlProperty& prop, bool add);
//	static CzXomlProperty	_getTag(IzXomlResource* target);
	static bool				_setUserData(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getUserData(IzXomlResource* target);
	static CzXomlProperty	_getUserProperties(IzXomlResource* target);
	static bool				_setPosition(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getPosition(IzXomlResource* target);
	static bool				_setPositionX(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getPositionX(IzXomlResource* target);
	static bool				_setPositionY(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getPositionY(IzXomlResource* target);
	static bool				_setPositionOrg(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setPositionOrgX(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setPositionOrgY(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setDocking(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getDocking(IzXomlResource* target);
	static bool				_setMargin(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getMargin(IzXomlResource* target);
	static bool				_setDepth(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getDepth(IzXomlResource* target);
	static bool				_setOrigin(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getOrigin(IzXomlResource* target);
	static bool				_setVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getVelocity(IzXomlResource* target);
	static bool				_setVelocityDamping(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getVelocityDamping(IzXomlResource* target);
	static bool				_setAngle(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getAngle(IzXomlResource* target);
	static bool				_setAngularVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getAngularVelocity(IzXomlResource* target);
	static bool				_setAngularVelocityDamping(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getAngularVelocityDamping(IzXomlResource* target);
	static bool				_setScale(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getScale(IzXomlResource* target);
	static bool				_setScaleX(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getScaleX(IzXomlResource* target);
	static bool				_setScaleY(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getScaleY(IzXomlResource* target);
	static bool				_setColour(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getColour(IzXomlResource* target);
	static bool				_setOpacity(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getOpacity(IzXomlResource* target);
	static bool				_setLayer(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getLayer(IzXomlResource* target);
	static bool				_setOrphan(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getOrphan(IzXomlResource* target);
	static bool				_setActive(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getActive(IzXomlResource* target);
	static bool				_setVisible(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getVisible(IzXomlResource* target);
	static CzXomlProperty	_getScene(IzXomlResource* target);
	static bool				_setCollidable(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getCollidable(IzXomlResource* target);
	static bool				_setTappable(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getTappable(IzXomlResource* target);
	static bool				_setDraggable(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getDraggable(IzXomlResource* target);
	static bool				_setBubbling(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getBubbling(IzXomlResource* target);
	static CzXomlProperty	_getDestroyed(IzXomlResource* target);
	static bool				_setWrapPosition(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getWrapPosition(IzXomlResource* target);
	static bool				_setScreenDocking(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getScreenDocking(IzXomlResource* target);
	static bool				_setHoldFocus(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getHoldFocus(IzXomlResource* target);
	static bool				_setUseParentOpacity(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getUseParentOpacity(IzXomlResource* target);
	static bool				_setPercPos(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getPercPos(IzXomlResource* target);
	static bool				_setIgnoreCamera(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getIgnoreCamera(IzXomlResource* target);
	static bool				_setTimeline(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getTimeline(IzXomlResource* target);
	static bool				_setTimeScale(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getTimeScale(IzXomlResource* target);
	static bool				_setBindings(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getBindings(IzXomlResource* target);
	static bool				_setBinding(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getBinding(IzXomlResource* target);
	static bool				_setLinkedTo(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getLinkedTo(IzXomlResource* target);
	static bool				_setTarget(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getTarget(IzXomlResource* target);
	static bool				_setGridPos(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getGridPos(IzXomlResource* target);
	static bool				_setLinearImpulse(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setAngularImpulse(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setForce(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setTorque(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setBox2dMaterial(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getBox2dMaterial(IzXomlResource* target);
	static bool				_setGravityScale(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setShape(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setSensor(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSensor(IzXomlResource* target);
	static bool				_setCollisionFlags(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getCollisionFlags(IzXomlResource* target);
	static bool				_setOnTapped(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnTapped2(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnTapped3(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnTapped4(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnTapped5(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnBeginTouch(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnBeginTouch2(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnBeginTouch3(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnBeginTouch4(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnBeginTouch5(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnEndTouch(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnEndTouch2(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnEndTouch3(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnEndTouch4(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnEndTouch5(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnCreate(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnDestroy(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnOrientationChange(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnCollisionStart(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnCollisionEnd(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnTick(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setBodyAwake(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getBodyAwake(IzXomlResource* target);
};

/// @}




#endif // _CZ_ACTOR_H_
