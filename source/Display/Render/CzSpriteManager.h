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

#if !defined(_CCZ_SPRITE_MANAGER_H_)
#define _CCZ_SPRITE_MANAGER_H_

#include "CzSprite.h"

class CzSpriteManager;

//
//
// CzSpriteManager - A sprite manager 
//
// The sprite manager managers a collection of sprites, including drawing, tracking and clean up
// The sprite manager also carries its own visual transform that will be applied to all of its children, allowing the user to apply rotation, scaling ans translation to all child sprites
//
//
class CzSpriteManager
{
public:
	// Provide public access to iteration of the sprite list
	typedef CzList<CzSprite*>::iterator	Iterator;
	Iterator		begin() { return Sprites.begin(); }
	Iterator		end()	{ return Sprites.end(); }

	// Properties
protected:
	CzMatrix3				Transform;				// Transform
	CzMatrix3				TransformNoCamera;		// Transform without camera
	CzList<CzSprite*>		Sprites;				// Our list of sprites
	CzSlotArray<CzSprite*>*	Layers;					// Visible layers used in depth sorting
	bool					Batching;				// Enable sprite batching
	CzVec2					COP;					// Centre of projection
	CzVec2					ScreenCentre;			// Screen centre
	CzVec4					ScreenClipRect;			// Screen clipping rectangle
	CzVec4					ClipRect;				// Clipping rectangle
public:
	void				addSprite(CzSprite* sprite);
	void				removeSprite(CzSprite* sprite, bool delete_sprites = true);
	void				setTransform(const CzMatrix3& transform)	{ Transform = transform; DirtyChildTransforms(); }
	CzMatrix3&			getTransform() 							{ return Transform; }
	void				setTransformNoCamera(const CzMatrix3& transform)	{ TransformNoCamera = transform; DirtyChildTransforms(); }
	CzMatrix3&			getTransformNoCamera() 					{ return TransformNoCamera; }
	void				setBatching(bool batching)				{ Batching = batching; }
	bool				getBatching() const						{ return Batching; }
	void				setCOP(float x, float y)				{ COP.x = x; COP.y = y; }
	CzVec2				getCOP() const							{ return COP; }
	void				setScreenCentre(float x, float y)		{ ScreenCentre.x = x; ScreenCentre.y = y; }
	CzVec2				getScreenCentre() const					{ return ScreenCentre; }
	void				setScreenClipRect(CzVec4& rect)			{ ScreenClipRect = rect; }
	void				setScreenClipRect(float x, float y, float w, float h)	{ ScreenClipRect.x = x; ScreenClipRect.y = y; ScreenClipRect.z = w; ScreenClipRect.w = h; }
	CzVec4				getScreenClipRect() const				{ return ScreenClipRect; }
	void				setClipRect(CzVec4& rect)				{ ClipRect = rect; }
	void				setClipRect(float x, float y, float w, float h)	{ ClipRect.x = x; ClipRect.y = y; ClipRect.z = w; ClipRect.w = h; }
	CzVec4				getClipRect() const						{ return ClipRect; }
	// Properties End

protected:
	void				DirtyChildTransforms();		// Dirties all child transforms to force them to update
	int					MaxLayers;					// Maximum layers
	void				ClearLayers();				// Clears all visible layers ready for next frame

public:
	CzSpriteManager() : MaxLayers(0), Batching(true)
	{
	}
	virtual ~CzSpriteManager() { Release(); }

	void			Init(int max_layers = 10);
	void			Draw();
	void			Release(bool delete_sprites = true);
};



#endif // _CCZ_SPRITE_MANAGER_H_
