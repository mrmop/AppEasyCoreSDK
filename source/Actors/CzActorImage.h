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

#if !defined(_CZ_ACTOR_IMAGE_H_)
#define _CZ_ACTOR_IMAGE_H_

#include "CzActor.h"
#include "CzGeometry.h"

/**
 @addtogroup Actors
 @{
 */

/**
 @class	CzActorImage

 @brief	An image based game actor that can display a bitmap or portion of a bitmap.

 Image actors or actors derived from them form the backbone of most of the games and apps developed with XOML. An image actor is an actor that represents itself on screen using an image or
 portion of an image. All of the user interface actors are derived from an image actor. Image actors have the following specific properties:
 - Brush (brush) - Specifies a brush that is used to define the image and source rectangle
 - Image (image) - The image that is to be used as the actors visual (deprecated, use brushes instead, with exception to particle actors)
 - Size (x, y) - The world size of the actor
 - SrcRect (x, y, width, height) - The position and source of the source rectangle in the image atlas (x, y, w, h rect). Used for panning the portion of a sprite atlas shown allowing frame based animation. (deprecated, use brushes instead, with exception to particle actors)
 - Tiling (x, y) - Sets the UV tiling factor for this actor (default is 1,1), if tiling factor is > 1.0 on either axis then Tiled will automatically be set to true
 - Tiled (x, y) - Informs the system that this actor should be rendered with tiling enabled
 - FlipX (boolean) - If true then this actor is horizontally flipped
 - FlipY (boolean) - If true then this actor is vertically flipped
 - Skew (top, bottom, left, right) - Four parameter skewing, which allows the actor to be skewed in four different directions
 - BeforeChildren (boolean) - When set to true this actor will be rendered before its children, otherwise it will be rendered afterwards (default is true)
 - Filter (boolean) - When set to true this actor will rendered using filtering (default is true)
 - AlphaMode (alpha_mode) - Sets the mode to use when mixing transparency (alpha) from the image. AlphaMode can be one of none, half, add, sub and blend (default mode is blend)
 - AspectLock (lock_mode) - Locks the aspect ratio of the actor to fit to the screens aspect ratio lock_mode can be one of x, y or none (default is none) Notes:
 - Anchor (topleft or centre) - Sets the draw anchor (topleft causes the actor to be displayed relative to its top-left cornr whilst centre will use the actors centre)

 An image actor requires an Image or a Brush to be defined
 Because an image actor inherits from a basic actor, it inherits all of the basic actors properties as well as those properties shown above. Lets take a look at an image actor definition:

 @par XOML Example using brushes:
 @code
 <ActorImage Brush=”Brush1” Position="10, 20" Size="-100, -100" />
 @endcode

 @par XOML Example using image and src rectangle:
 @code
 <ActorImage Image=”Image1” SrcRect="0, 0, 64, 64" Position="10, 20" Size="-100, -100" />
 @endcode

 Note that it is usually much more convenient and better practice to define and use brushes instead of using image and bitmap source ractangles (the area opf the bitmap that will be drawn 
 onto the actor) as they can be re-used and easily modified later.

 */

class CzActorImage : public CzActor
{
	// Properties
protected:
	CzGeometry*			Geometry;				///< Visual geometry
public:
	bool				setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool				setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool				getProperty(unsigned int property_name, CzXomlProperty& prop);
	// Properties end
protected:
	CzIVec2				OldSrcSize;
	bool				InitWithImage(CzImage* image, int width, int height);
	bool				InitWithBrush(IzBrush* brush, int width, int height);
	bool				UpdateBinding(unsigned int property_name, CzXomlVariable* var);
	void				UpdateAspect();
public:
	CzActorImage() : CzActor(), Geometry(NULL)	{ setActualClassType("actorimage"); }
	virtual ~CzActorImage()			{}

	virtual bool		Init(IzBrush* brush, int width, int height);
	virtual bool		Init(CzImage* image, int width, int height);
	virtual bool		Init(CzImage* image, CzIRect* src_rect);
	virtual bool		Init(CzImage* image, int x, int y, int w, int h);
	bool				UpdateVisual();

	void				SetSrcDest(int x, int y, int w, int h);
	void				SetSrcRect(int x, int y, int w, int h);
	void				SetDestSize(int w, int h);
	virtual void		SetImage(CzImage* image);
	virtual void		SetFromBrush(IzBrush* brush, bool resize = true);
	CzGeometry*			getGeometry()			{ return Geometry; }
	void				setGeometry(CzGeometry* geom);

	// Event handlers
	virtual void		NotifyOrientationChange(CzScene::eOrientation old_orientation, CzScene::eOrientation new_orientation);

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Implementation of IzAnimTarget interface
	bool				UpdateFromAnimation(CzAnimInstance *animation);

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	ActorImageClassDef;								// XOML class definition

public:
	static void				InitClass();
	static void				ReleaseClass();

	static bool				_setSize(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSize(IzXomlResource* target);
	static bool				_setSrcRect(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSrcRect(IzXomlResource* target);
	static bool				_setSrcTiling(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSrcTiling(IzXomlResource* target);
	static bool				_setSrcTiled(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSrcTiled(IzXomlResource* target);
	static bool				_setImage(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getImage(IzXomlResource* target);
	static bool				_setBrush(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getBrush(IzXomlResource* target);
	static bool				_setFlipX(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getFlipX(IzXomlResource* target);
	static bool				_setFlipY(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getFlipY(IzXomlResource* target);
	static bool				_setSkew(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSkew(IzXomlResource* target);
	static bool				_setBeforeChildren(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getBeforeChildren(IzXomlResource* target);
	static bool				_setFilter(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getFilter(IzXomlResource* target);
	static bool				_setAspectLock(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getAspectLock(IzXomlResource* target);
	static bool				_setAlphaMode(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getAlphaMode(IzXomlResource* target);
	static bool				_setGeometry(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getGeometry(IzXomlResource* target);
};

/// @}


/**
 @class	CzActorImageCreator

 @brief	Creates an instance of an image based actor object.

 Used by the XOML system to instantiate a CzActorImage object.

 */

class CzActorImageCreator : public IzXomlClassCreator
{
public:
	CzActorImageCreator()
	{
		setClassName("actorimage");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzActorImage(); }
};



#endif // _CZ_ACTOR_IMAGE_H_
