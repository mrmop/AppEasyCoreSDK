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

#if !defined(_CCZ_SPRITE_H_)
#define _CCZ_SPRITE_H_

#include "CzTypes.h"
#include "CzAnim.h"
#include "CzImage.h"
#include "CzFont.h"
#include "CzBrush.h"
#include "CzRender.h"
#include "CzGeometry.h"

class CzSpriteManager;
class CzActor;

/**
 @class	CzSprite

 @brief	A sprite is the visual representation of an on screen game object
    
 Can be thought of as an interface rather than a concrete class as other sprite types are created from this.

 */

class CzSprite
{
public:
	static uint16 SpriteIndices[];

	/**
	 @enum	eType
	
	 @brief	Values that represent the type of sprite to render.
	 */
	enum eType
	{
		ST_None,	///< Invalid type
		ST_Image,	///< Renders as an image sprite
		ST_Text,	///< Renders as a text sprite
		ST_Dummy,	///< A dummy sprite has no visible component and is only meant to provide invisible transforms
		ST_9Patch,	///< Renders as a 9-patch image sprite
		ST_Poly,	///< Renders as a poygon image sprite
	};
	enum eAnchor
	{
		Centre,		///< Sprite will be anchored around its centre
		TopLeft,	///< Sprite will be anchored around its top left corner
	};

	// Provide public access to iteration of the sprite list
	typedef CzList<CzSprite*>::iterator	Iterator;
	Iterator		begin() { return Children.begin(); }
	Iterator		end()	{ return Children.end(); }

	// Properties
protected:
	CzActor*				Actor;					///< The actor that this sprite is attached to
	CzGeometry*				Geometry;				///< Geometry used to display the sprite
	CzSprite*				Parent;					///< Parent sprite
	CzList<CzSprite*>		Children;				///< List of children sprites
	eType					SpriteType;				///< Type of sprite
	CzSpriteManager*		Manager;				///< Parent sprite manager
	float					Width, Height;			///< Destination width and height (used to represent the visible extents of the sprite on screen)
	float					Depth;					///< Depth
	CzMatrix3				Transform;				///< Transform
	CzVec2					Position;				///< Position of the sprite
	CzVec2					Origin;					///< Origin of sprite (0, 0 is sprites centre)
	CzVec4					Skew;					///< 4 vertex skew offsets (3 bit fixed)
	float					Angle;					///< Rotation of sprite (degrees)
	float					ScaleX;					///< X axis scale of sprite
	float					ScaleY;					///< X axis scale of sprite
	CzColour				Colour;					///< Colour of sprite
	bool					Visible;				///< Sprites visible state
	bool					Pooled;					///< Tells system if we belong to a sprite pool or not
	bool					InUse;					///< Used in a memory pooling system to mark this sprite as in use
	bool					BeforeChildren;			///< When true th parent sprite will be drawn before all children otherwise after
	int						Layer;					///< Depth layer
	CzSprite*				LinkedTo;				///< When a sprite is linked to another it takes on that sprites transform etc..
	IzBrush*				Brush;					///< Current brush assigned to this sprite
	CzVec4					ClipRect;				///< Local clipping rect used to clip this sprites children
	bool					Orphan;					///< When set to true sprites are orphaned outside the usual parent / child hierarchy and layered as independent objects
	bool					IgnoreCamera;			///< When set to true sprite ignores camera transform
	eAnchor					Anchor;					///< Position the sprite is anchored around
public:
	void			setActor(CzActor* owner)				{ Actor = owner; }
	void			setParent(CzSprite* parent)				{ Parent = parent; }
	CzSprite*		getParent()								{ return Parent; }
	void			addChild(CzSprite* sprite);
	void			removeChild(CzSprite* sprite, bool delete_sprites = true);
	void			setSpriteType(eType type)				{ SpriteType = type; }
	eType			getSpriteType() const					{ return SpriteType; }
	void			setManager(CzSpriteManager* manager)	{ Manager = manager; TransformDirty = true; }
	CzSpriteManager* getManager()							{ return Manager; }
	void			setDestSize(int width, int height)
	{
		Width = (float)width;
		Height = (float)height;
		TransformDirty = true;
	}
	void			setDepth(float depth)		{ if (depth != Depth) TransformDirty = true; Depth = depth; }
	float			getDestWidth() const		{ return Width; }
	float			getDestHeight() const		{ return Height; }
	CzVec2			getDestSize() const			{ return CzVec2(Width, Height); }
	float			getDepth() const			{ return Depth; }
	void			setPosAngScale(float x, float y, float angle, float scale)
	{
		if (x != Position.x || y != Position.y || angle != Angle || scale != ScaleX || scale != ScaleY)
		{
			Position.x = x;
			Position.y = y;
			Angle = angle;
			ScaleX = ScaleY = scale;
			TransformDirty = true;
		}
	}
	void			setPosAngScale(float x, float y, float angle, float scale_x, float scale_y)
	{
		if (x != Position.x || y != Position.y || angle != Angle || scale_x != ScaleX || scale_y != ScaleY)
		{
			Position.x = x;
			Position.y = y;
			Angle = angle;
			ScaleX = scale_x;
			ScaleY = scale_y;
			TransformDirty = true;
		}
	}
	void			setPosition(float x, float y)
	{
		if (x != Position.x || y != Position.y)
		{
			Position.x = x;
			Position.y = y;
			TransformDirty = true;
		}
	}
	CzVec2			getPosition() const			{ return Position; }
	void			setOrigin(float x, float y)
	{
		if (x != Origin.x || y != Origin.y)
		{
			Origin.x = x;
			Origin.y = y;
			TransformDirty = true;
		}
	}
	CzVec2			getOrigin() const			{ return Origin; }
	void			setSkew(float x1, float x2, float y1, float y2)
	{
		if (Skew.x != x1 || Skew.y != x2 || Skew.z != y1 || Skew.w != y2)
		{
			Skew.x = x1;
			Skew.y = x2;
			Skew.z = y1;
			Skew.w = y2;
			TransformDirty = true;
		}
	}
	CzVec4			getSkew() const			{ return Skew; }
	void			setAngle(float angle)
	{
		if (angle != Angle)
		{
			Angle = angle;
			TransformDirty = true;
		}
	}
	float			getAngle() const			{ return Angle; }		
	void			setScale(float scale)
	{
		if (scale != ScaleX || scale != ScaleY)
		{
			ScaleX = scale;
			ScaleY = scale;
			TransformDirty = true;
		}
	}
	void			setScale(float scale_x, float scale_y)
	{
		if (scale_x != ScaleX || scale_y != ScaleY)
		{
			ScaleX = scale_x;
			ScaleY = scale_y;
			TransformDirty = true;
		}
	}
	float			getScale() const			{ return ScaleX; }
	float			getScaleX() const			{ return ScaleX; }
	float			getScaleY() const			{ return ScaleY; }
	virtual void	setColour(const CzColour& colour, bool use_parent = false)
	{
		Colour = colour;
		if (use_parent && LinkedTo != NULL)
			Colour.a = (uint8)(((int)colour.a * LinkedTo->getOpacity()) / 255);
		if (Colour.a == 0 && colour.a != 0)
			TransformDirty = true;
	}
	CzColour		getColour() const			{ return Colour; }
	int				getOpacity() const			{ return Colour.a; }
	void			setVisible(bool show)		{ Visible = show; }
	bool			isVisible() const			{ return Visible; }
	void			setFilter(bool enable)		{ Material->Filter = enable; }
	bool			isFilter() const			{ return Material->Filter; }
	bool			isVisibleWithParents() const;
	void			forceTransformDirty()		{ TransformDirty = true; }
	void			setPooled(bool pooled)		{ Pooled = pooled; }
	bool			isPooled() const			{ return Pooled; }
	void			setInUse(bool in_use)		{ InUse = in_use; }
	bool			isInUse() const				{ return InUse; }
	void			setLayer(int layer)			{ Layer = layer; }
	int				getLayer() const			{ return Layer; }
	CzMatrix3&		getTransform()				{ if (TransformDirty) RebuildTransform(); return Transform; }
	void			setLinkedTo(CzSprite* sprite);
	CzSprite*		getLinkedTo()				{ return LinkedTo; }
	float			getAccumDepth() const		{ return AccumDepth; }
	bool			isTransformDirty() const	{ return TransformDirty; }
	IzBrush*		getBrush()					{ return Brush; }
	int				getVertexCount() const		{ return Prim->VertCount; }
	void			setClipRect(CzVec4& rc);
	CzVec4			getClipRect() const			{ return ClipRect; }
	CzVec4			getScreenClipRect() const	{ return ScreenClipRect; }
	void			setBeforeChildren(bool before) { BeforeChildren = before; }
	bool			getBeforeChildren() const	{ return BeforeChildren; }
	CzMatrix3&		getFinalTransform()			{ return FinalTransform; }
	bool			affectsClip() const			{ return ClipRect.w >= 0; }
	CzVec4			FindFirstClipRect();
	CzVec4			FindFirstScreenClipRect();
	CzVec4			FindFirstScreenClipRect2();
	virtual bool	isClipped();
	CzVec2*			getScreenV()				{ return Prim->Verts; }
	void			setAlphaMode(eCzAlphaMode mode)		{ Material->AlphaMode = mode; }
	eCzAlphaMode	getAlphaMode() const					{ return Material->AlphaMode; }
	void			setOrphan(bool enable)				{ Orphan = enable; }
	bool			getOrphan() const					{ return Orphan; }
	void			setIgnoreCamera(bool enable)		{ IgnoreCamera = enable; }
	bool			getIgnoreCamera() const				{ return IgnoreCamera; }
	void			setTiled(bool tiled)				{ Material->Tiled = tiled; }
	bool			isTiled() const						{ return Material->Tiled; }
	CzRenderPrim*	getPrim()							{ return Prim; }
	CzRenderMaterial* getMaterial()						{ return Material; }
	eAnchor			getAnchor() const					{ return Anchor; }
	void			setAnchor(eAnchor anchor)			{ if (Anchor != anchor) TransformDirty = true; Anchor = anchor; }
	CzGeometry*		getGemoetry()						{ return Geometry; }
	virtual void	setGeometry(CzGeometry* geom);

	// Properties End
protected:
	bool			ChildChangeClip;			///< Set to true if child changes clip rect
	CzMatrix3		FinalTransform;				///< Final transform
	bool			TransformDirty;				///< Dirty when transform changed
	CzRenderPrim*	Prim;						///< 2D renderer primitive used to render this sprite
	CzRenderMaterial* Material;					///< Material used to render this sprite
	float			AccumDepth;					///< Accumulated depth
	CzVec4			ScreenClipRect;				///< Transformed screen clipping rect for this sprite

	void			TransformClipRect();		///< Transforms the local clipping rect to screen coords
	virtual void	RebuildTransform();			///< Rebuilds the display transform
	virtual void	BuildFinalTransform();		///< Buiolds the final transform
	virtual void	TransformVertices();		///< Builds a list of transformed vertices
	void			UpdateClipping();

public:
	CzSprite() : Pooled(false), Prim(NULL), Material(NULL), Actor(NULL), Geometry(NULL)
	{
		setSpriteType(ST_None);
#if defined(CZ_ENABLE_METRICS)
		CzMetrics::TotalSpritesCreated++;
#endif
	}
	virtual ~CzSprite();
	virtual void	Init(int vertex_count = 4);			// Called to initialise the sprite, used after construction or to reset the sprite in a pooled sprite system
	virtual bool	Update();							// Updates the sprite
	virtual void	Draw() = 0;							// Pure virtual, need to implement in derived classes
	virtual void	DrawChildren();						// Draws the sprites children
	virtual bool	HitTest(float x, float y);			// Check to see if point is within area covered by transformed sprite
	virtual bool	HitTestNoClip(float x, float y);	// Check to see if point is within area covered by transformed sprite
	virtual bool	isOutsideFocusRange(float x, float y, float scale = 1.0f);
	virtual CzVec2	TransformPoint(float x, float y);	// Transform point by sprites local angle / scale transform
	virtual CzVec2	TransformPointToScreen(float x, float y);	// Transform point by sprites final transform
	void			BringToFront();

	virtual void	RebuildTransformNow();				// Rebuilds the display transform immediately

	bool			isClippedByManager(uint16 *indices = NULL, int count = 4);
	virtual bool	SimpleTestOverlap(CzSprite* other);
	virtual bool	TestOverlap(CzSprite* other, int i1, int i2, int i3);
	virtual bool	TestOverlap(CzSprite* other);
};





#endif // _CCZ_SPRITE_H_
