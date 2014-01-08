#if !defined(_CCZ_ANIMATION_H_)
#define _CCZ_ANIMATION_H_

#include "CzUtil.h"
#include "CzXoml.h"
#include "CzEvents.h"

class CzAnimInstance;
class CzAnimTimeline;
class CzAnimTimelinesManager;

/**
 @addtogroup Animation
 @{
 */
 
/**
 @typedef	float (*CzTweenEffect)(float)

 @brief	CzTweenEffect - Animation tweening effect (used to tween between different frames).
 */

typedef float (*CzTweenEffect)(float);

/**
 @struct	CzAnimFrame

 @brief	Animation frame data.

 Base class of specific animation frame data types:
 - CzAnimFrameBool
 - CzAnimFrameFloat
 - CzAnimFrameVec2
 - CzAnimFrameVec3
 - CzAnimFrameVec4
 - CzAnimFrameRect
 - CzAnimFrameString

 A CzAnim is built from a number of these key frames. A key frame is the state of a property of the animations target at a specific point in time.

 */

struct CzAnimFrame
{
	/**
	 @enum	eAnimType
	
	 @brief	Values that represent different types of animations.
	 */
	enum eAnimType
	{
		FT_Invalid,
		FT_Bool,
		FT_Float,
		FT_Vec2,
		FT_Vec3,
		FT_Vec4,
		FT_Rect,
		FT_String,
		FT_Custom,
	};

	/**
	 @enum	eAnimEase
	
	 @brief	Values that represent different types of easing functions.
	 */
	enum eAnimEase
	{
		AT_Linear, 
		AT_QuadIn, 
		AT_QuadOut, 
		AT_CubicIn, 
		AT_CubicOut, 
		AT_QuarticIn, 
		AT_QuarticOut, 
	};

	float				Time;					///< Time at which this frame should be active
	CzTweenEffect		EasingFunc;				///< Easing function

	CzAnimFrame() : EasingFunc(NULL) {}
	virtual ~CzAnimFrame() {}

	void				setEasing(eAnimEase effect);
};

struct CzAnimFrameBool: public CzAnimFrame
{
	bool	data;	//< Frame data
	CzAnimFrameBool() : CzAnimFrame() {}
};

struct CzAnimFrameFloat: public CzAnimFrame
{
	float	data;	//< Frame data
	CzAnimFrameFloat() : CzAnimFrame() {}
};

struct CzAnimFrameVec2: public CzAnimFrame
{
	CzVec2 data;	//< Frame data
	CzAnimFrameVec2() : CzAnimFrame() {}
};

struct CzAnimFrameVec3: public CzAnimFrame
{
	CzVec3 data;	//< Frame data
	CzAnimFrameVec3() : CzAnimFrame() {}
};

struct CzAnimFrameVec4: public CzAnimFrame
{
	CzVec4 data;	//< Frame data
	CzAnimFrameVec4() : CzAnimFrame() {}
};

struct CzAnimFrameRect: public CzAnimFrame
{
	CzIRect data;	//< Frame data
	CzAnimFrameRect() : CzAnimFrame() {}
};

struct CzAnimFrameString: public CzAnimFrame
{
	CzString data;	//< Frame data
	CzAnimFrameString() : CzAnimFrame() {}
};

/**
 @class	IzAnimTarget

 @brief	An animation target object.

 Classes that derive from IzAnimTarget and implement UpdateFromAnimation() can have properties that can be the target of an animation. For example CzActor derives from IzAnimTarget and 
 implements CzActor:: UpdateFromAnimation(). This enables animations to target their properties such as position, angle, scale etc.. using timelines.

 */

class IzAnimTarget
{
public:
	virtual bool	UpdateFromAnimation(CzAnimInstance *animation) = 0;
};

/**
 @class	CzAnim

 @brief	A collection of animation frames.

 <h1>Introduction</h1>
 There are plenty of great games and apps out there that do not feature any form of animation. However, most modern games do feature animations to enhance their 
 look and feel. Lets take "Angry Birds" as an example, Angry Birds uses animations all over the place, such as the animating birds  and pigs, even the menus 
 contain flashing and sweeping animations. 

 When we initially decided to implement an animation system we decided that it had to be very flexible and support any type of animation, be it animating the 
 position of an actor, animating the graphical frames of an actor or even animating a list of commands for an actor. From AppEasy's point of view, animation 
 refers to any variable or set of variables that can change over time.

 The AppEasy animation system currently supports the following features:
 - Time based frame and interpolated named animations
 - Delayed and looped animations
 - Support for boolean, float, vectors (2d, 3d and 4d), rect (for image frames), string and custom frame data types
 - Resource manager that tracks and manages sets of animations (scene local and global)
 - Animation data re-use using animation instances
 - Animation time lines to support multiple simultaneous animations that can target object specific properties such as the colour of a scene or the position 
 of an actor
 - Time line manager that tracks and manages sets of animation time lines
 - Callback notifications for animation instance started, stopped and looped events
 - Animations and animation time lines can be defined and attached to actors and scenes using XOML
 - Support for OnStart, OnEnd and OnRepeat events in code and in XOML
 - Linear, quadratic, quartic and cubic in and out easing

 The animation system is split into the following classes:
 - CzAnimFrame - Base animation frame class
 - IzAnimTarget - Interface for classes that act as an animtion target
 - CzAnim - A basic animation
 - CzAnimCreator - Class used to instsntiate an animation from XOML
 - CzAnimManager - Manages a collection of animations
 - CzAnimInstance - An instantiated running animation
 - CzAnimTimeline - A collection of running animation instances
 - CzAnimTimelineCreator - Class used to instantiate a collection of running animations from XOML
 - CzAnimTimelinesManager - Manages a collection of timelines

 <h1>Animation Frame Data</h1>
 Animation frame data is the backbone of all animations, it represents the actual discrete state of a variable at a given point in time (called a key frame). 
 An animation frame consists of two components:
 - Frame data - Frame data is the value or values of a specific variable at a specific point in time
 - Frame time - The time at which the frame has its value (in seconds)

 The base class for all animation frame data types is the CzAnimFrame. This class provides a means for the developer to implement their own custom animation 
 frame data types. The following animation frame data types are already implemented:
 - CzAnimFrameBool - Boolean variables such as actor visbility
 - CzAnimFrameFloat - Floating point variables such as angle and scale
 - CzAnimFrameVec2 - Floating point 2 parameter variables such as 2D position and velocity
 - CzAnimFrameVec3 - Floating point 2 parameter variables such as 3D position and velocity
 - CzAnimFrameVec4 - Floating point 4 parameter variables such as colour
 - CzAnimFrameRect - Integer 4 parameter variables such as image source rectangles
 - CzAnimFrameString - String based parameters such as narrative text, commands or even other timeline's

 <h1>Animations and the Resource Manager</h1>
 The resource manager is used to manage a group of resources (animations). The idea is that you create a group of animations then add them to the resource 
 manager and then forget about them. The resource manager will take care of cleaning them up when the scene or  main app is destroyed. The main app object 
 contains its own global resource manager whilst all scenes contain their own local resource manager.

 Note that when animations are created via XOML mark-up they are added to the resource manager. Animations that are created inside a scene will be added to the 
 scenes resource manager, whilst animations created outside of the scene will be added to the global resource manager.

 To find an animation within a scenes resource manager you would use could such as:

 @code
CzAnim* anim = (CzAnim*)scene->getResourceManager()->findResource("Player1Anim", CzHashes::Animation_Hash);
 @endcode

 Note that if the animation is not found within the scene then the system will automatically search the global resource manager for the animation. You can 
 prevent this global search by passing false as the third parameter to findResource().

 Timelines are stored in the timelines manager of a scene, actor or the global time lines manager, depending on where they are declared.

 <h1>Animation Instances</h1>
 CzAnim is basically used as a means to store the animation data in a convenient place. With this in mind you do not play a CzAnim directly, instead you 
 create an instance of it using CzAnimInstance:

 @code
// Create and set up an animation
CzAnimInstance* face_anim = new CzAnimInstance();
face_anim->setAnimation(anim);
face_anim->setTarget(actor, "SrcRect");
 @endcode

 Note the following line of code:

 @code
face_anim->setTarget(actor, "SrcRect");
 @endcode

 This line of code tells the animation to modify the "SrcRect" (the actors image atlas position) property of the actor object, causing the image to change.

 Animation instances can be played, paused, resumed, stopped and restarted. You can also tell an animation to delay playing for a finite period of time. 

 <h1>Animation Targets and Target properties</h1>
 
 AppEasy Core uses the concept of animation targets and animation target properties. An animation target is basically a class that contains a variable or 
 group of variables that can be targeted for modification by an animation. The actual variable or variables that are targeted are called target properties. 
 An example target would be an actor and example target property would be the actors position. When you create an instance of an animation you set the target 
 and target property that the animation will modify using code similar to that shown below:

 @code
face_anim->setTarget(actor, "SrcRect");
 @endcode

 Any class can be used as a target for animation as long as it derives from the IzAnimTarget interface and implements the the following pure virtual method:

 @code
	virtual bool	UpdateFromAnimation(CzAnimInstance *animation) = 0;
 @endcode

 When the animation updates it will call back this method passing in its data asking the method to update the state of the object. Both scene and actor classes 
 already have this functionality implemented.

 Lets take a quick look at a small section of CzScene::UpdateFromAnimation() to see how the CzScene class has implemented this method:

 @code
bool CzScene::UpdateFromAnimation(CzAnimInstance *animation)
{
	unsigned int property_name = animation->getTargetPropertyHash();
	bool delta = animation->isDelta();

	if (Camera != NULL)
	{
		if (property_name == CzHashes::Position_Hash)
		{
			CzAnimFrameVec2* frame = (CzAnimFrameVec2*)animation->getCurrentData();
			if (delta)
			{
				CzVec2 pos = Camera->getPosition();
				pos.x += frame->data.x;
				pos.y += frame->data.y;
				Camera->setPosition(pos.x, pos.y);
			}
			else
				Camera->setPosition(frame->data.x, frame->data.y);
			return true;
		}
		else
		if (property_name == CzHashes::Angle_Hash)
		{
			CzAnimFrameFloat* frame = (CzAnimFrameFloat*)animation->getCurrentData();
			if (delta)
				Camera->setAngle(Camera->getAngle() + frame->data);
			else
				Camera->setAngle(frame->data);
			return true;
		}
		else
		if (property_name == CzHashes::Scale_Hash)
		{
			CzAnimFrameFloat* frame = (CzAnimFrameFloat*)animation->getCurrentData();
			if (delta)
				Camera->setScale(Camera->getScale() + frame->data);
			else
				Camera->setScale(frame->data);
			return true;
		}
	}

	return false;
}
 @endcode

 The logic is quite simple, we check the name of the property that was passed in by the animation instance then check that against known property names of 
 the class. If it matches then we move the animation data from the animation instance into our classes local variable.

 As you can see implementing your own custom animation targets is a simple case of:
 - Deriving your class from  IzAnimTarget
 - Implement the  UpdateFromAnimation(CzAnimInstance *animation) method

 <h1>Animation Timeline's</h1>
 
 An animation time line is basically a way to group together multiple animation instances and play, pause, stop and resume them all together. The general 
 idea is that you create an animation time line then create animation instances and add them to the time line. You then attach the time line to your destination 
 object, be that a scene or an actor. The animation system will then take care of the rest for you. Here is an example showing how to create and use a time line:

@code
// Find our face animation
CzAnim* face_anim = (CzAnim*)scene->getResourceManager()->findResource("FaceAnim", CzHashes::Animation_Hash);

// Create and set up our animation instance
CzAnimInstance* instance = new CzAnimInstance();
instance->setAnimation(face_anim);
instance->setTarget(actor, "SrcRect");
timeline->addAnimation(instance);
timeline->play();

// Create an animation timeline to hold our image animation
CzAnimTimeline* timeline = new CzAnimTimeline();
timeline->addAnimation(instance);

// Attach timeline to the actor
actor->setTimeline(timeline);
 @endcode

 Defining and attaching animations is much easier and more intuitive if done declaratively using XOML mark-up. More on this this later

 Note that when you attach an animation time line to a scene or an actor, the scene / actor will automatically take over updating it for you.

 Time lines can be played, paused, resumed and stopped. All animation instances within the animation time line will be affected. 

 <h1>Resource Manager and Timelines</h1>
 
 The resource manager is generally responsible for managing the lifetimes of animations and animation time lines, in particular those created from XOML mark-up. 

 Each scene has its own local resource manager as well as a global resource manager. 

 <h1>Working with Animations</h1>
 
 The general work flow when working with the animation system has two potential paths:
 - Manual definition - You manually create all of the animation frames, animation classes, instances, timelines etc in code
 - XOML definition - You load a XOML file that contains the definitions, find the time lines and attach

 The first method is more difficult as it does require creatng and setting up animation classes yourself. Heres the basic flow for manual animation setup:
 - Create a CzAnim object and give it a meaningful name
 - Create and set-up animation frames
 - Add animation frames to the CzAnim object
 - Add the CzAnim object to the scene or global CzAnimManager
 - Later when you need to create an instance of the animation, search the animation manager for the animation by name
 - Create a CzAnimTimeline object
 - Create a CzAnimInstance object and attach the  CzAnim object
 - Set up any additional paramaters for the time line object
 - Add the time line object to the scene or global time line manager (not essential as scenes and actors can clean them up for you)
 - Set the actor or scenes time line using setTimeline()
 - Call the time line's play() method to start the time line playing

 The second method using XOML is much simpler:
 - Create a XOML file 
 - Add Animation and Timeline definitions
 - If your scene / actor is also defined in XOML then you simply set the Timeline property to the name of the timline and you are done. If not then continue 
 onto the next step
 - Search the scenes timelines for our named time line
 - Set the actor or scenes time line using setTimeline()
 - Call the time line's play() method to start the time line playing


 <h1>Creating a Basic Animation in Code</h1>
 
 AppEasy supports a number of different types of animation as previously explained. In our example we are going to create a basic floating point animation 
 that animates the rotation property of an actor object. 

@code
// Create an animation
CzAnim* anim = new CzAnim();
anim->setName("TestAnim");
anim->setDuration(3);
anim->setType(CzAnimFrame::FT_Float);

// Create and add frames
CzAnimFrameFloat* frame = new CzAnimFrameFloat();
frame->Time = 0;
frame->data = 0;
anim->addFrame(frame);
frame = new CzAnimFrameFloat();
frame->Time = 1;
frame->data = 45;
anim->addFrame(frame);
frame = new CzAnimFrameFloat();
frame->Time = 2;
frame->data = 90;
anim->addFrame(frame);

// Create animation instance
CzAnimInstance* instance = new CzAnimInstance();
instance->setAnimation(anim);
instance->setDelay(0);
instance->setRepeatCount(0);
instance->setTarget(actor, "Angle");

// Creata a time line
CzAnimTimeline* timeline = new CzAnimTimeline();
timeline->setName("Timeline1");
timeline->addAnimation(instance);

// Play the timeline
timeline->play();

// Attach timeline to the actor
actor->setTimeline(timeline);
 @endcode

 As you can see it is a little verbose, which is why we recommend using XOML mark-up for defining animations in particular.

 <h1>Creating a Basic Animation in XOML</h1>
 
 XOML is AppEasy's mark-up language that can be used to define and set-up actors, scenes, resource, animations and other game elements. Here is an example 
 showing how to set up the previous example using XOML syntax:

 @code
<xml>
    <Animation Name="TestAnim" Type="float" Duration="3" >
        <Frame Value="0"   Time="0.0" />
        <Frame Value="45"  Time="1.0" />
        <Frame Value="90"  Time="2.0" />
    </Animation>
    <Timeline Name="Timeline1" AutoPlay="true">
        <Animation Anim="TestAnim" Target="Angle" Repeat="0" StartAtTime="0"/>
    </Timeline>
</xml>

 @endcode

 As you can see it is incredibly easy to set up animations and time line's using XOML syntax.

 <b>XOML Anim tag properties</b>
 - Name (string) - Name of this animation resource
 - Tag (string) - Resource tag (used to group resources together)
 - Type (type) - The type of animation frame data, can be one of the following:
  - bool
  - float
  - vec2
  - vec3
  - vec4
  - rect
  - string
 - Duration (seconds) - The duration of the animation

 Type and Duration are required properties

 <b>XOML Anim Frame tag properties</b>
 - Time (seconds) - The time at which this frame is available
 - Value (any supported type) - The value of the frame at the above time. The value here depends on the type of animation that was defined. For example, 
 if you declared the animation as a string type then this value should contain a string. If you declared the animation as a vec2 then the value should 
 contain pairs of values
 - Easing (easing) - Type of easing to use, supported easing types include:
  - linear
  - quadin
  - quadout
  - cubicin
  - cubicout
  - quarticin
  - quarticout

 Time and Value are required properties

 <b>XOML Timeline tag properties</b>
 - Name (string) - Name of this animation timeline resource
 - Tag (string) - Resource tag (used to group resources together)
 - AutoPlay (boolean) - When set to true the timeline will begin animating as soon as it is attached to an actor or scene 
 - TimeScale (number) - This property can be used to change the speed at which the timelines animations are played back. For example, if we set this value 
 to 2.0 then the animations in the previous example would play back twice as quickly. This feature allows us to speed up and slow down animations without 
 having to create new instances of them
 - Local (boolean) - By default timelines declared inside an actor will be local to the actor and not the scene. By setting Local=”false” the timeline will 
 be placed into the containing scene instead. This is useful if you want to show a group of timeline across different actors but do not want to move the 
 timeline definition into the scene or global resource manager

 Timelines also contain child Animation tags which define which animations will appear in the timeline. Lets take a look at the properties of this inner tag:
 - Anim (animation) - Names an animation to be included into the timeline
 - Target - Sets the objects target property that should be updated by the animation. For example, Position would target an actor scene position updating 
 its position
 - Repeat (number) - The number of times that the animation should repeat before stopping (0 represents play forever, which is the default)
 - StartAtTime (seconds) - Setting this parameter will delay the start of the animation 
 - Delta (boolean) - When set to true, instead of directly setting the target objects property it adds to it instead (called delta animations)
 - Interpolate (boolean) - When set true (which id default) animation frames will be smoothly interpolated from one key frame to the next. When set to false 
 animation frames will suddenly switch when their time marker is reached. For most animation you will want to use interpolation, however some kinds of 
 animations are not suited to interpolation, for example, image sprite animations
 - OnStart (actions list) - Defines an actions list that will be called when this animation starts playing (only called for animations with a StartAtTime 
 value that is not 0
 - OnEnd (actions list) - Defines an actions list that will be called when this animation ends
 - OnRepeat (actions list) - Defines an actions list that will be called when this animation repeats

 Anim and Target are required properties

 Note that a time line can be assigned to as many objects you like but the time line will be played back the same on all objects. This is very useful if you 
 have a group of actors that need to run the same animations synchronously.

 <h1>Creating an Image Animation</h1>
 
 We are now going to take a look at creating an image based animation that we can attach to an actor:

 @code
// Create an animation
CzAnim* anim = new CzAnim();
anim->setName("TestAnim");
anim->setDuration(0.8f);
anim->GenerateAtlasFrames(8, 36, 40, 0, 0, 512, 40, 512, 0.1f);

// Create animation instance
CzAnimInstance* instance = new CzAnimInstance();
instance->setAnimation(anim);
instance->setDelay(0);
instance->setRepeatCount(0);
instance->setTarget(actor, "SrcRect");

// Creata a time line
CzAnimTimeline* timeline = new CzAnimTimeline();
timeline->setName("Timeline1");
timeline->addAnimation(instance);

// Play the timeline
timeline->play();

// Attach timeline to actor
actor->setTimeline(timeline);
 @endcode

 Setting up an image based animation is a little simpler because the CzAnim::GenerateAtlasFrames() method() automates the generation of frames for us. 

 Although the same can also be done in XOML which is even simpler, e.g:

 @code
<Animation Name="TestAnim" Type="rect" Duration="0.8" >
	<Atlas Count="8" Duration="0.1" Pitch="36, 40" Size="36, 40" Width="36" />
</Animation>
<Timeline Name="Timeline1" AutoPlay="true">
    <Animation Anim="TestAnim" Target="SrcRect" Repeat="0" StartAtTime="0"/>
</Timeline>
 @endcode

 The properties for the Atlas tag are as follows:
 - Count (number) - Number of frames to generate
 - Duration (seconds) - The amount of time to display each frame
 - Pitch (x, y) - The amount to step across and and right to get to the next frame in pixels
 - Size (x, y) - The width and height of each animation frame in pixels
 - Width (number) - The width of the image atlas in pixels
 - Position (x, y) - The position on the atlas where frames should start being taken from

 */

class CzAnim : public IzXomlResource
{
public:

	/**
	 @enum	eAnimStatus
	
	 @brief	Values that represent the current state of the animation.
	 */
	enum eAnimStatus
	{
		AS_Stopped, //< Animation has stopped playing
		AS_Playing,	//< Animation is playing
		AS_Paused,	//< Animation is paused
	};
	typedef CzVector<CzAnimFrame*>::iterator _Iterator;
	_Iterator begin()					{ return Frames.begin(); }
	_Iterator end()						{ return Frames.end(); }

protected:
	// Properties
	CzAnimFrame::eAnimType		Type;						///< Type of animation
	float						Duration;					///< Duration of animation
	CzVector<CzAnimFrame*>		Frames;						///< Animation frames
	int							Category;					///< User defined category
	bool						AutoDelete;					///< If set to true thenm timeline will be deleted when finished

public:
	void						setType(CzAnimFrame::eAnimType type)			{ Type = type; }
	CzAnimFrame::eAnimType		getType() const									{ return Type; }
	float						getDuration() const								{ return Duration; }
	void						setDuration(float duration)						{ Duration = duration; }
	void						addFrame(CzAnimFrame* frame);
	bool						getCurrentFrame(CzAnimFrame** current, CzAnimFrame** next, float current_time);
	void						CalculateCurrentData(float current_time, CzAnimFrame* current_data, bool interpolate = true);
	int							getFrameCount()									{ return Frames.size(); }
	CzAnimFrame*				getFrame(int index)								{ return Frames[index]; }
	void						setCategory(int category)						{ Category = category; }
	int							getCategory() const								{ return Category; }
	void						setAutoDelete(bool enable)						{ AutoDelete = enable; }
	bool						isAutoDelete() const							{ return AutoDelete; }
	// Properties end

public:
	CzAnim() : IzXomlResource(), Duration(0), Category(0), AutoDelete(false) { setClassType("animation"); }
	virtual ~CzAnim()
	{
		for (_Iterator it = Frames.begin(); it != Frames.end(); it++)
		{
			delete (*it);
		}
		Frames.clear();
	}

	void				GenerateAtlasFrames(int count, int frame_w, int frame_h, int start_x, int start_y, int pitch_x, int pitch_y, int image_width, float frame_speed);

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parebt, bool load_children, CzXmlNode* node);
};

/**
 @class	CzAnimCreator

 @brief	Creates an instance of an CzAnim object.

 CzAnimCreator enables animations to be instantiated from XOML.

 */

class CzAnimCreator : public IzXomlClassCreator
{
public:
	CzAnimCreator()
	{
		setClassName("animation");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzAnim(); }
};

/**
 @class	CzAnimInstance

 @brief	An instance of an animation object.

 CzAnim is not played directly, instead you create an instance of it using CzAnimInstance:

 @code
// Create and set up an animation
CzAnimInstance* face_anim = new CzAnimInstance();
face_anim->setAnimation(anim);
face_anim->setTarget(actor, "SrcRect");
 @endcode

 Note the following line of code:

 @code
face_anim->setTarget(actor, "SrcRect");
 @endcode

 This line of code tells the animation to modify the "SrcRect" (the actors image atlas position) property of the actor object, causing the image to change.

 Animation instances can be played, paused, resumed, stopped and restarted. You can also tell an animation to delay playing for a finite period of time. 

 */

class CzAnimInstance
{
public:
protected:
	// Properties
	CzAnimTimeline*			Parent;					///< Parent timeline
	CzAnim*					Animation;				///< Animation to use
	bool					Managed;				///< True if managed by a CzAnimInstanceManager
	int						RepeatCount;			///< Number of times to replay the animation
	float					CurrentTime;			///< Current animation time (0 is start of animation)
	CzAnimFrame*			CurrentData;			///< The current interpolated data for the current time
	CzAnim::eAnimStatus		Status;					///< Status of the animation
	bool					Started;				///< If set to true then the animation has started playing
	IzAnimTarget*			Target;					///< Target object to update
#if defined(_DEBUG)
	CzString				TargetProperty;			///< Target object property name to update
#endif
	unsigned int			TargetPropertyHash;		///< Target object property name as a string hash to update
	float					Delay;					///< Amount of time to delay starting the animation
	bool					DelayExpired;			///< true if initial delay has expired
	CzCallback				StartedCallback;		///< Callback which is called when the animation starts playing
	CzCallback				StoppedCallback;		///< Callback which is called when the animation stops playing
	CzCallback				LoopedCallback;			///< Callback which is called when the animation loops
	bool					CurrentDataValid;		///< True if current frame data is valid
	bool					IsDelta;				///< Delta animation should update the target variables value instead of replacing it
	bool					Interpolate;			///< If true then the interpolated valiue between key frames will be calculated and applied
	CzEventManager*			EventsManager;			///< List of events that the animation handles

public:
	void				setParent(CzAnimTimeline* parent)			{ Parent = parent; }
	CzAnimTimeline*		getParent()									{ return Parent; }
	CzAnim*				getAnimation()								{ return Animation; }
	void				setAnimation(CzAnim* anim);
	void				setManaged(bool managed)					{ Managed = managed; }
	bool				isManaged() const							{ return Managed; }
	int					getRepeatCount() const						{ return RepeatCount; }
	void				setRepeatCount(int repeat_count)			{ RepeatCount = repeat_count; }
	void				setCurrentTime(float current_time)			{ CurrentTime = current_time; }
	float				getCurrentTime() const						{ return CurrentTime; }
	bool				isFinished() const							{ if (RepeatCount == 0) return false; else return RepeatedCount >= RepeatCount; }
	CzAnimFrame*		getCurrentData()							{ return CurrentData; }
#if defined(_DEBUG)
	CzString&			getTargetProperty()							{ return TargetProperty; }
#endif
	unsigned int		getTargetPropertyHash()						{ return TargetPropertyHash; }
	IzAnimTarget*		getTargetElement()							{ return Target; }
	void				setTarget(IzAnimTarget* element, const char* property_name)
	{
		Target = element;
#if defined(_DEBUG)
		TargetProperty = property_name;
#endif
		TargetPropertyHash = CzString::CalculateHash(property_name);
	}
	void				setTargetElement(IzAnimTarget* element)	{ Target = element; }
	float				getDelay() const								{ return Delay; }
	void				setDelay(float delay)							{ Delay = delay; }
	bool				isStarted() const								{ return Started; }
	bool				isStopped() const								{ return Status == CzAnim::AS_Stopped; }
	bool				isPlaying() const								{ return Status == CzAnim::AS_Playing; }
	bool				isPaused() const								{ return Status == CzAnim::AS_Paused; }
	bool				isDelayed() const								{ return !DelayExpired; }
	void				restart()										{ Status = CzAnim::AS_Playing; CurrentTime = 0; DelayExpired = false; RepeatedCount = 0; Started = true; }
	void				play()											{ Status = CzAnim::AS_Playing; Started = true; }
	void				stop()											{ Status = CzAnim::AS_Stopped; CurrentTime = 0; DelayExpired = false; RepeatedCount = 0; }
	void				pause()											{ Status = CzAnim::AS_Paused; }
	void				setStartedCallback(CzCallback callback)			{ StartedCallback = callback; }
	void				setStoppedCallback(CzCallback callback)			{ StoppedCallback = callback; }
	void				setLoopedCallback(CzCallback callback)			{ LoopedCallback = callback; }
	bool				isCurrentDataValid() const						{ return CurrentDataValid; }
	void				setDelta(bool is_delta)							{ IsDelta = is_delta; }
	bool				isDelta() const									{ return IsDelta; }
	void				setInterpolate(bool interpolate)				{ Interpolate = interpolate; }
	bool				isInterpolated() const							{ return Interpolate; }
	void				addEventsManager();
	CzEventManager*		getEventsManager()								{ return EventsManager; }
	// Properties end

protected:
	int					RepeatedCount;

public:
	CzAnimInstance() : Parent(NULL), Managed(false), Animation(NULL), RepeatCount(0), CurrentTime(0), RepeatedCount(0), Delay(0), DelayExpired(false), Started(false), Status(CzAnim::AS_Stopped), CurrentDataValid(false), 
							StartedCallback(NULL), StoppedCallback(NULL), LoopedCallback(NULL), IsDelta(false), Interpolate(true), EventsManager(NULL), CurrentData(NULL), Target(NULL), TargetPropertyHash(0) { }
	virtual ~CzAnimInstance();

	virtual bool		Update(float dt, IzAnimTarget* target = NULL);	// Passing none NULL for target allows you to retarget the animation update to another object
	virtual void		UpdateTargetOnly(IzAnimTarget* target);
	virtual void		ForceUpdateTargetOnly(IzAnimTarget* target);

	// Event handlers
	virtual void		ProcessEventActions(unsigned int event_name);
	virtual void		NotifyStart();		///< This event is called when this aimation instance is started
	virtual void		NotifyEnd();		///< This event is called when this aimation instance stopped playing
	virtual void		NotifyRepeat();		///< This event is called when this aimation instance repeats
};

/**
 @class	CzAnimTimeline

 @brief	CzAnimTimeline - An animation timeline.

 An animation time line is basically a way to group together multiple animation instances and play, pause, stop and resume them all together. The general 
 idea is that you create an animation time line then create animation instances and add them to the time line. You then attach the time line to your destination 
 object, be that a scene or an actor. The animation system will then take care of the rest for you. Here is an example showing how to create and use a time line:

 @par XOML Example
 @code
<Animation Name="ang_anim" Duration="5" Type="float">
	<Frame Time="0" Value="0" />
	<Frame Time="2.5" Value="180" />
	<Frame Time="5" Value="0" />
</Animation>
<Timeline Name="spin_anim" AutoPlay="true">
	<Animation Anim="ang_anim" Repeat="0" Target="Angle" />
</Timeline>
<Label Font="serif" Text="Hello" Timeline="spin_anim" />
 @endcode

 @par Code Example
@code
// Create angle animation
CZ_NEW_ANIM(ang_anim, "ang_anim", 5, CzAnimFrame::FT_Float);
CZ_NEW_ANIM_FRAME_FLOAT(ang_anim, 0,	0,			CzAnimFrame::AT_Linear);
CZ_NEW_ANIM_FRAME_FLOAT(ang_anim, 2.5,	180,		CzAnimFrame::AT_Linear);
CZ_NEW_ANIM_FRAME_FLOAT(ang_anim, 5,	0,			CzAnimFrame::AT_Linear);
scene->getResourceManager()->addResource(ang_anim);
// Create animation timeline
CZ_NEW_TIMELINE(spin_anim, "spin_anim", ang_anim, 0, true, 0, "Angle");
label->getTimelinesManager()->addTimeline(spin_anim);

// Assign timeline to scene and start the animation
scene->setTimeline(spin_anim);
spin_anim->restart();
  @endcode

 In the above examples we create an animation then assign it to a timeline. We then assign the timeline to the label which causes it to spin over 5 seconds.

 Defining and attaching animations is much easier and more intuitive if done declaratively using XOML mark-up. 

 Note that when you attach an animation time line to a scene or an actor, the scene / actor will automatically take over updating it for you.

 Time lines can be played, paused, resumed and stopped. All animation instances within the animation time line will be affected. 

 */

class CzAnimTimeline : public IzXomlResource
{
public:
	typedef CzList<CzAnimInstance*>::iterator _Iterator;
	_Iterator begin()						{ return Animations.begin(); }
	_Iterator end()							{ return Animations.end(); }

protected:
	// Properties
	CzAnimTimelinesManager*		Manager;		///< Timeline manager that manages this timeline
	IzXomlResource*				Parent;			///< Parent container TODO: REMOVE Parent, this is stored in Container in IzXomlResource
	CzList<CzAnimInstance*>		Animations;		///< Animation intsnces
	float						TimeScale;		///< How to scale time (1 - no change, > 1 speed up time, < 1 slow down time)
	bool						AutoDelete;		///< If set to true thenm timeline will be deleted when finished
public:
	void						setManager(CzAnimTimelinesManager* manager)	{ Manager = manager; }
	CzAnimTimelinesManager*		getManager()							{ return Manager; }
	void						setParent(IzXomlResource* parent)		{ Parent = parent; }
	IzXomlResource*				getParent()								{ return Parent; }
	void						addAnimation(CzAnimInstance* anim) { Animations.push_back(anim); anim->setManaged(true); anim->setParent(this); }
	void						removeAnimation(CzAnimInstance* anim)
	{
		for (_Iterator it = begin(); it != end(); it++)
		{
			if (*it == anim)
			{
				delete (*it);
				Animations.erase(it);
				break;
			}
		}
	}
	CzAnimInstance*			findAnimation(const char* name);
	CzAnimInstance*			findAnimation(unsigned int name_hash);
	void					play();
	void					stop();
	void					pause();
	void					restart();
	int						getAnimationCount()					{ return Animations.size(); }
	void					setTargetElement(IzAnimTarget* target)
	{
		for (_Iterator it = begin(); it != end(); it++)
			(*it)->setTargetElement(target);
	}
	bool					isStopped();
	bool					isPlaying();
	void					setAutoDelete(bool enable)			{ AutoDelete = enable; }
	bool					isAutoDelete() const				{ return AutoDelete; }
	void					setTimeScale(float time_scale)		{ TimeScale = time_scale; }
	float					getTimeScale() const				{ return TimeScale; }
	// Properties end

private:
	static int				GlobalTimelineRunCount;			// Global timeline run count (used to prevent the same timeline being updated twice in one frame by different systems, which allows sharing of timeslines across objects)
	int						TimelineRunCount;

public:
	CzAnimTimeline() : IzXomlResource(), TimelineRunCount(-1), TimeScale(1.0f), AutoDelete(false), Manager(NULL) { setClassType("timeline"); }
	virtual ~CzAnimTimeline();

	virtual void		ForceUpdateTargetOnly(IzAnimTarget* target = NULL);
	virtual void		UpdateTargetOnly(IzAnimTarget* target = NULL);
	virtual bool		Update(float time_delta, IzAnimTarget* target = NULL);	// target will override the current set target for the animation only if the animation was not updated by some other ojject this frame

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	static void			UpdateRunCount()
	{
		GlobalTimelineRunCount++;
	}
	void				SetCurrentTime(float current_time)
	{
		for (_Iterator it = begin(); it != end(); it++)
			(*it)->setCurrentTime(current_time);
	}
	void				Remove();
};

/**
 @class	CzAnimTimelineCreator

 @brief	Creates an instance of an animation object.

 CzAnimTimelineCreator enables animation timelines to be instantiated from XOML.

 */

class CzAnimTimelineCreator : public IzXomlClassCreator
{
public:
	CzAnimTimelineCreator()
	{
		setClassName("timeline");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzAnimTimeline(); }
};

/**
 @class	CzAnimTimelinesManager

 @brief	Manages a collection of timelines.

 The timelines manager manages a group of of timelines. Actors and Scenes each have their own timelines manager as well as a global timelines manager that is 
 used for global timelines.

 To retrieve an actors timelines manager you call CzActor::getTimelinesManager() and for a scene you would call CzScene::getTimelinesManager(). To retrieve 
 the global timelines manager you would use the CZ_GLOBAL_TIMELINE_MANAGER macro.

 */

class CzAnimTimelinesManager
{
public:
	// Public access to iteration
	typedef CzList<CzAnimTimeline*>::iterator _Iterator;
	_Iterator				begin()		{ return Timelines.begin(); }
	_Iterator				end()		{ return Timelines.end(); }

protected:
	// Properties
	CzList<CzAnimTimeline*>	Timelines;			// A collection of timelines
	IzXomlResource*			Parent;				// Parent container
public:
	void					addTimeline(CzAnimTimeline* timeline);
	void					removeTimeline(CzAnimTimeline* timeline);
	void					removeTimeline(unsigned int name_hash);
	CzAnimTimeline*			findTimeline(unsigned int name_hash);
	CzAnimTimeline*			findTimeline(const char* name);
	void					clearTimelines();
	void					setParent(IzXomlResource* scene)		{ Parent = scene; }
	IzXomlResource*			getParent()								{ return Parent; }
	// Properties end

public:
	CzAnimTimelinesManager() : Parent(NULL) {}
	virtual ~CzAnimTimelinesManager() { clearTimelines(); }

	// Utility
	static CzAnimTimeline*	FindTimeline(const char* name, IzXomlResource* container);
	static CzAnimTimeline*	FindTimeline(unsigned int name_hash, IzXomlResource* container);
	static CzAnimTimeline*	FindTimeline(unsigned int name_hash, CzScene* container, unsigned int actor_hash);
	static CzAnimTimeline*	FindTimeline(const char* name, CzScene* container, const char* actor_name);
};

/**
 @class	CzAnimUtil

 @brief	Animation utility class.

 Static utility methods that can be used to create simple animations.

 */

class CzAnimUtil
{
public:
	static CzAnim*			CreateFloatAnim(const char* name, float source, float target, float duration, CzAnimFrame::eAnimEase easing, int category = 0);
	static CzAnim*			CreateVec2Anim(const char* name, const CzVec2& source, const CzVec2& target, float duration, CzAnimFrame::eAnimEase easing, int category = 0);
	static CzAnim*			CreateVec3Anim(const char* name, const CzVec3& source, const CzVec3& target, float duration, CzAnimFrame::eAnimEase easing, int category = 0);
	static CzAnim*			CreateVec4Anim(const char* name, const CzVec4& source, const CzVec4& target, float duration, CzAnimFrame::eAnimEase easing, int category = 0);
	static CzAnim*			CreateRectAnim(const char* name, const CzIRect& source, const CzIRect& target, float duration, CzAnimFrame::eAnimEase easing, int category = 0);
	static CzAnimTimeline*	CreateTimeline(const char* name, CzAnim* anim, IzAnimTarget* target, const char* target_property, int repeat = 1, bool delta = false, float start_at_time = 0, bool interpolate = true);

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
 @def	CZ_NEW_ANIM(anim, name, duration, type)

 @brief	A macro that creates a simple animation

 @param	anim		The animation.
 @param	name		The name of the animation.
 @param	duration	The duration of the animation in seconds.
 @param	type		The type of animation.
 */

#define CZ_NEW_ANIM(anim, name, duration, type)					\
	CzAnim* anim = new CzAnim();								\
	anim->setName(name);										\
	anim->setDuration(duration);								\
	anim->setType(type);

/**
 @def	CZ_NEW_ANIM_FRAME_VEC2(anim, time, _x, _y, easing)

 @brief	A macro that creates a vec2 animation frame.

 @param	anim  	The animation where the frame will be added.
 @param	time  	The time of the frame in seconds.
 @param	_x	  	The x coordinate.
 @param	_y	  	The y coordinate.
 @param	easing	The easing method.
 */

#define CZ_NEW_ANIM_FRAME_VEC2(anim, time, _x, _y, easing)			\
	{																\
		CzAnimFrameVec2* frame = new CzAnimFrameVec2();				\
		frame->Time = time;											\
		frame->data.x = _x;											\
		frame->data.y = _y;											\
		frame->setEasing(easing);									\
		anim->addFrame(frame);										\
	}

/**
 @def	CZ_NEW_ANIM_FRAME_VEC3(anim, time, _x, _y, _z, easing)

 @brief	A macro that creates a vec3 animation frame.

 @param	anim  	The animation where the frame will be added.
 @param	time  	The time of the frame in seconds.
 @param	_x	  	The x coordinate.
 @param	_y	  	The y coordinate.
 @param	_z	  	The z coordinate.
 @param	easing	The easing method.
 */

#define CZ_NEW_ANIM_FRAME_VEC3(anim, time, _x, _y, _z, easing)		\
	{																\
		CzAnimFrameVec3* frame = new CzAnimFrameVec3();				\
		frame->Time = time;											\
		frame->data.x = _x;											\
		frame->data.y = _y;											\
		frame->data.z = _z;											\
		frame->setEasing(easing);									\
		anim->addFrame(frame);										\
	}

/**
 @def	CZ_NEW_ANIM_FRAME_VEC4(anim, time, _x, _y, _z, _w, easing)

@brief	A macro that creates a vec4 animation frame.

 @param	anim  	The animation where the frame will be added.
 @param	time  	The time of the frame in seconds.
 @param	_x	  	The x coordinate.
 @param	_y	  	The y coordinate.
 @param	_z	  	The z coordinate.
 @param	_w	  	The w coordinate.
 @param	easing	The easing method.
  */

#define CZ_NEW_ANIM_FRAME_VEC4(anim, time, _x, _y, _z, _w, easing)	\
	{																\
		CzAnimFrameVec4* frame = new CzAnimFrameVec4();				\
		frame->Time = time;											\
		frame->data.x = _x;											\
		frame->data.y = _y;											\
		frame->data.z = _z;											\
		frame->data.w = _w;											\
		frame->setEasing(easing);									\
		anim->addFrame(frame);										\
	}

/**
 @def	CZ_NEW_ANIM_FRAME_RECT(anim, time, _x, _y, _w, _h, easing)

@brief	A macro that creates a rect animation frame.

 @param	anim  	The animation where the frame will be added.
 @param	time  	The time of the frame in seconds.
 @param	_x	  	The x coordinate.
 @param	_y	  	The y coordinate.
 @param	_w	  	The width.
 @param	_h	  	The height.
 @param	easing	The easing method.
 */

#define CZ_NEW_ANIM_FRAME_RECT(anim, time, _x, _y, _w, _h, easing)	\
	{																\
		CzAnimFrameRect* frame = new CzAnimFrameRect();				\
		frame->Time = time;											\
		frame->data.x = _x;											\
		frame->data.y = _y;											\
		frame->data.w = _w;											\
		frame->data.h = _h;											\
		frame->setEasing(easing);									\
		anim->addFrame(frame);										\
	}

/**
 @def	CZ_NEW_ANIM_FRAME_FLOAT(anim, time, _d, easing)

@brief	A macro that creates a floating point animation frame.

 @param	anim  	The animation where the frame will be added.
 @param	time  	The time of the frame in seconds.
 @param	_d	  	The data.
 @param	easing	The easing method.
 */

#define CZ_NEW_ANIM_FRAME_FLOAT(anim, time, _d, easing)				\
	{																\
		CzAnimFrameFloat* frame = new CzAnimFrameFloat();			\
		frame->Time = time;											\
		frame->data = _d;											\
		frame->setEasing(easing);									\
		anim->addFrame(frame);										\
	}

/**
 @def	CZ_NEW_ANIM_FRAME_BOOL(anim, time, _d, easing)

@brief	A macro that creates a boolean animation frame.

 @param	anim  	The animation where the frame will be added.
 @param	time  	The time of the frame in seconds.
 @param	_d	  	The data.
 @param	easing	The easing method.
 */

#define CZ_NEW_ANIM_FRAME_BOOL(anim, time, _d, easing)				\
	{																\
		CzAnimFrameBool* frame = new CzAnimFrameBool();				\
		frame->Time = time;											\
		frame->data = _d;											\
		frame->setEasing(easing);									\
		anim->addFrame(frame);										\
	}

/**
 @def	CZ_NEW_ANIM_FRAME_STRING(anim, time, _d, easing)

@brief	A macro that creates a string animation frame.

 @param	anim  	The animation where the frame will be added.
 @param	time  	The time of the frame in seconds.
 @param	_d	  	The data.
 @param	easing	The easing method.
 */

#define CZ_NEW_ANIM_FRAME_STRING(anim, time, _d, easing)			\
	{																\
		CzAnimFrameString* frame = new CzAnimFrameString();			\
		frame->Time = time;											\
		frame->data = _d;											\
		frame->setEasing(easing);									\
		anim->addFrame(frame);										\
	}

/**
 @def	CZ_NEW_TIMELINE(timeline, name, animation, delay, interpolate, repeat, target_property)

 @brief	A macro that creates a new animation instance then adds it to a new timeline.

 @param	timeline	   	The timeline.
 @param	name		   	The name of the timeline.
 @param	animation	   	The animation to attach.
 @param	delay		   	The start delay in seconds.
 @param	interpolate	   	The interpolate option.
 @param	repeat		   	The repeat count.
 @param	target_property	Target property name.
 */

#define CZ_NEW_TIMELINE(timeline, name, animation, delay, interpolate, repeat, target_property)	\
	CzAnimTimeline* timeline = new CzAnimTimeline();											\
	{																							\
		CzAnimInstance* inst = new CzAnimInstance();											\
		inst->setAnimation(animation);															\
		inst->setDelay(delay);																	\
		inst->setInterpolate(interpolate);														\
		inst->setRepeatCount(repeat);															\
		inst->setTarget(NULL, target_property);													\
		timeline->setName(name);																\
		timeline->addAnimation(inst);															\
	}

/**
 @def	CZ_ADD_TIMELINE(timeline, animation, delay, interpolate, repeat, target_property)

 @brief	A macro that creates a new animation instance then adds its to an existing timeline.

 @param	timeline	   	The timeline.
 @param	animation	   	The animation to attach.
 @param	delay		   	The start delay in seconds.
 @param	interpolate	   	The interpolate option.
 @param	repeat		   	The repeat count.
 @param	target_property	Target property name.
 */

#define CZ_ADD_TIMELINE(timeline, animation, delay, interpolate, repeat, target_property)		\
	{																							\
		CzAnimInstance* inst = new CzAnimInstance();											\
		inst->setAnimation(animation);															\
		inst->setDelay(delay);																	\
		inst->setInterpolate(interpolate);														\
		inst->setRepeatCount(repeat);															\
		inst->setTarget(NULL, target_property);													\
		timeline->addAnimation(inst);															\
	}

/// @}


#endif	// _CANIM_H_