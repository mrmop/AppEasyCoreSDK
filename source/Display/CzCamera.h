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

#if !defined(_CZ_CAMERA_H_)
#define _CZ_CAMERA_H_

#include "CzUtil.h"
#include "CzXoml.h"
#include "CzAnim.h"

/**
 @addtogroup Display
 @{
 */
 
/**
 @class	CzCamera

 @brief	A camera is generally used to display aa area of a game scene, usually tracking some actor.

 A camera is a view into the scene from a specific position, angle and scale. In order to move, rotate and scale a scene a camera should be created 
 and attached to it. To create a camera you use the Camera XOML tag:

 @par XOML Example:
 @code
<!-- Create a camera -->
<Camera Name="Camera1" />
 @endcode

 To attach the camera to the scene we add the Camera attribute to the scene definition:

 @code
<Scene Name="Scene1" Current="true" Camera="Camera1">
 @endcode

 When we move, scale or rotate a scene we are actually modifying the camera view within the scene. When we move the camera objects tend to move in 
 the opposite direction, for example moving the camera left moves the scene actors to the right. If you think about how a real camera works, when 
 you move the camera in one direction the view seen by the camera moves in the opposite direction.

 Its possible to create a number of different cameras and switch between them to offer different views into the scene.

 Cameras offer a great out of the box feature called touch panning, which enables the user to pan the camera around a scene on the x and y axis by 
 dragging their finger around the scene. Take a look at the ScenePanning example for an example showing how to use touch panning.

 Lets take a look at what properties the Camera tag supports:

 - Name (string) - Cameras resource name
 - Position (x, y) - Start position of the camera
 - Angle (degrees) - Rotation of the camera 
 - Scale (scale factor) - Scale of the camera (can be used for zoom effects)
 - TouchPanX / TouchPanY (boolean) - Setting true causes the camera to move around when the user drags their finger on the screen using velocity 
 obtained from the speed of the users drag. Separate X and Y axis panning can be enabled / disabled.
 - VelocityDamping (x, y) - Amount of damping to apply to the cameras velocity (x, y) each frame. This value when set to a value of less than 1.0, 
 1.0 will cause the camera to slow down over time.
 - IgnoreActors (boolean) - When set to true and touch panel is enabled, dragging a finger over an actor will still be classed as a touch pan drag. 
 This option basically allows the user to touch pan the scene regardless of what the user touches on the screen. 
 - TargetX (actor name, write-only) – An actor that the camera will be used to track its x-axis position
 - TargetY (actor name, write-only) – An actor that the camera will be used to track its y-axis position
 - FollowSpeed (x, y) – The speed at which the camera should track the specified actor(s). Higher values will catch the camera up to the actors 
 target position faster
 - Tag (string) - Group tag

 Some of these properties can be modified via the scene such as Position, Angle and Scale as changes to these properties go straight to the 
 attached camera. You can set all properties for a camera via LUA using the camera.set() function.


 */

class CzCamera : public IzXomlResource
{
public:
	// Properties
protected:
	CzScene*				Scene;				///< Parent scene
	CzMatrix3				Transform;			///< The combined camera transform
	CzVec2					Position;			///< Position of camera within scene
	CzVec2					Velocity;			///< Velocity of camera
	CzVec2					VelocityDamping;	///< Velocity cadmping applied to slow the camera
	CzVec2					FollowSpeed;		///< Speed at which to follow target (1.0 is max)
	float					Scale;				///< Cameras scale
	float					Angle;				///< Cameras angle
	bool					TransformDirty;		///< Marks camera transform needs rebuilding
	CzActor*				TargetX;			///< Object that the camera is targeting on the x axis
	CzActor*				TargetY;			///< Object that the camera is targeting on the y axis
	unsigned int			TargetXHash;		///< Hash of object that the camera is targeting on the x axis
	unsigned int			TargetYHash;		///< Hash of object that the camera is targeting on the y axis
	bool					TouchPanX;			///< If true then the camera will pan along the x-axis with the users touch
	bool					TouchPanY;			///< If true then the camera will pan along the y-axis with the users touch
	bool					IgnoreActors;		///< if an actor is currently focused then it will usually prevent touch panning, this will disable that
public:
	CzMatrix3&				getTransform()						{ return Transform; }
	void					setPosition(float x, float y)		{ Position.x = x; Position.y = y; TransformDirty = true; }
	CzVec2					getPosition() const					{ return Position; }
	void					setVelocity(float x, float y)		{ Velocity.x = x; Velocity.y = y; }
	CzVec2					getVelocity() const					{ return Velocity; }
	void					setVelocityDamping(float x, float y) { VelocityDamping.x = x; VelocityDamping.y = y; }
	CzVec2					getVelocityDamping() const			{ return VelocityDamping; }
	void					setFollowSpeed(float x, float y)	{ FollowSpeed.x = x; FollowSpeed.y = y; }
	CzVec2					getFollowSpeed() const				{ return FollowSpeed; }
	void					setScale(float scale)				{ Scale = scale; TransformDirty = true; }
	float					getScale() const					{ return Scale; }
	void					setAngle(float angle)				{ Angle = angle; TransformDirty = true; }
	float					getAngle() const					{ return Angle; }
	void					setTargetX(const char* target_name);
	void					setTargetX(CzActor* target);
	void					setTargetY(const char* target_name);
	void					setTargetY(CzActor* target);
	void					setTransformDirty(bool dirty)		{ TransformDirty = dirty; }
	bool					isTransformDirty() const			{ return TransformDirty; }
	void					setTouchPanX(bool pan)				{ TouchPanX = pan; }
	bool					isTouchPanX() const					{ return TouchPanX; }
	void					setTouchPanY(bool pan)				{ TouchPanY = pan; }
	bool					isTouchPanY() const					{ return TouchPanY; }
	void					setIgnoreActors(bool ignore)		{ IgnoreActors = ignore; }
	bool					getIgnoreActors() const				{ return IgnoreActors; }
	virtual bool			setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool					setProperty(const char* property_name, const CzString& data, bool delta);
	virtual bool			setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool					getProperty(const char* property_name, CzXomlProperty& prop);
	virtual bool			getProperty(unsigned int property_name, CzXomlProperty& prop);
	// Properties end

private:

public:
	CzCamera() : IzXomlResource(), Scene(NULL), Position(0, 0), Scale(1.0f), Angle(0), Velocity(0, 0), VelocityDamping(0.8f, 0.8f), FollowSpeed(1.0f, 1.0f), TargetX(0), TargetY(0), TargetXHash(0), TargetYHash(0), TransformDirty(true), TouchPanX(false), TouchPanY(false), IgnoreActors(true) { setClassType("camera"); }
	virtual ~CzCamera()	{}

	// Updates the camera
	virtual void	Update(float dt);

	// Implementation of IzXomlClass interface
	int				LoadFromXoml(IzXomlResource* parebt, bool load_children, CzXmlNode* node);

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	CameraClassDef;								// XOML class definition
public:
	static void				InitClass();
	static void				ReleaseClass();

	static bool				_setName(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getName(IzXomlResource* target);
	static bool				_setTag(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setPosition(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getPosition(IzXomlResource* target);
	static bool				_setScale(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getScale(IzXomlResource* target);
	static bool				_setAngle(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getAngle(IzXomlResource* target);
	static bool				_setVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getVelocity(IzXomlResource* target);
	static bool				_setVelocityDamping(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getVelocityDamping(IzXomlResource* target);
	static bool				_setTouchPanX(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getTouchPanX(IzXomlResource* target);
	static bool				_setTouchPanY(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getTouchPanY(IzXomlResource* target);
	static bool				_setIgnoreActors(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getIgnoreActors(IzXomlResource* target);
	static bool				_setTargetX(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setTargetY(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setFollowSpeed(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getFollowSpeed(IzXomlResource* target);

};

/**
 @class	CzCameraCreator

 @brief	Creates an instance of a camera

 */

class CzCameraCreator : public IzXomlClassCreator
{
public:
	CzCameraCreator()
	{
		setClassName("camera");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzCamera(); }
};


/// @}


#endif // _CZ_CAMERA_H_