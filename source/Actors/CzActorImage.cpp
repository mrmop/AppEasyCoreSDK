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

#include "IzPlatformDisplay.h"
#include "CzActorImage.h"
#include "CzBitmapSprite.h"
#include "CzBitmapSprite9.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"

CzXomlClassDef* CzActorImage::ActorImageClassDef = NULL;

/**
 @fn	void CzActorImage::InitClass()

 @brief	Initialises the class.
	
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzActorImage::InitClass()
{
	// Create class properties
	ActorImageClassDef = new CzXomlClassDef();
	ActorImageClassDef->addProperty(new CzXomlClassProperty("Size",				PT_Vec2,		CzActorImage::_setSize,				CzActorImage::_getSize));
	ActorImageClassDef->addProperty(new CzXomlClassProperty("SrcRect",			PT_Vec4,		CzActorImage::_setSrcRect,			CzActorImage::_getSrcRect));
	ActorImageClassDef->addProperty(new CzXomlClassProperty("Tiling",			PT_Vec2,		CzActorImage::_setSrcTiling,		CzActorImage::_getSrcTiling));
	ActorImageClassDef->addProperty(new CzXomlClassProperty("Tiled",			PT_Bool,		CzActorImage::_setSrcTiled,			CzActorImage::_getSrcTiled));
	ActorImageClassDef->addProperty(new CzXomlClassProperty("Image",			PT_Resource,	CzActorImage::_setImage,			CzActorImage::_getImage));
	ActorImageClassDef->addProperty(new CzXomlClassProperty("Brush",			PT_Resource,	CzActorImage::_setBrush,			CzActorImage::_getBrush));
	ActorImageClassDef->addProperty(new CzXomlClassProperty("FlipX",			PT_Bool,		CzActorImage::_setFlipX,			CzActorImage::_getFlipX));
	ActorImageClassDef->addProperty(new CzXomlClassProperty("FlipY",			PT_Bool,		CzActorImage::_setFlipY,			CzActorImage::_getFlipY));
	ActorImageClassDef->addProperty(new CzXomlClassProperty("Skew",				PT_Vec4,		CzActorImage::_setSkew,				CzActorImage::_getSkew));
	ActorImageClassDef->addProperty(new CzXomlClassProperty("BeforeChildren",	PT_Bool,		CzActorImage::_setBeforeChildren,	CzActorImage::_getBeforeChildren));
	ActorImageClassDef->addProperty(new CzXomlClassProperty("Filter",			PT_Bool,		CzActorImage::_setFilter,			CzActorImage::_getFilter));
	ActorImageClassDef->addProperty(new CzXomlClassProperty("AlphaMode",		PT_String,		CzActorImage::_setAlphaMode,		CzActorImage::_getAlphaMode));
	ActorImageClassDef->addProperty(new CzXomlClassProperty("AspectLock",		PT_String,		CzActorImage::_setAspectLock,		CzActorImage::_getAspectLock));
	ActorImageClassDef->addProperty(new CzXomlClassProperty("Geometry",			PT_Resource,	CzActorImage::_setGeometry,			CzActorImage::_getGeometry));

}
void CzActorImage::ReleaseClass()
{
	if (ActorImageClassDef != NULL)
	{
		ActorImageClassDef->Release();
		delete ActorImageClassDef;
		ActorImageClassDef = NULL;
	};
}

// //
// //
// //
// //
// //
// //
// //
// //
// XOML property setters and getters
// //
// //
// //
// //
// //
// //
// //
// //

bool CzActorImage::_setSize(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorImage* actor = (CzActorImage*)target;

	int width, height;

	if (add)
	{
		CzBitmapSprite* visual = (CzBitmapSprite*)actor->getVisual();
		width = (int)visual->getDestWidth();
		height = (int)visual->getDestHeight();
		width += (int)prop.p_vec[0];
		height += (int)prop.p_vec[1];
	}
	else
	{
		width = (int)prop.p_vec[0];
		height = (int)prop.p_vec[1];
	}
	actor->setOriginalSize(width, height);
	actor->CalculateSizes(width, height);
	actor->SetDestSize(width, height);

	return true;
}

CzXomlProperty CzActorImage::_getSize(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorImage*)target)->getSize());
}

bool CzActorImage::_setSrcRect(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorImage* actor = (CzActorImage*)target;

	CzBitmapSprite* visual = (CzBitmapSprite*)actor->getVisual();
	if (add)
	{
		CzIRect rect = visual->getSrcRect();
		rect.x += (int)prop.p_vec[0];
		rect.y += (int)prop.p_vec[1];
		rect.w += (int)prop.p_vec[2];
		rect.h += (int)prop.p_vec[3];
		visual->setSrcRect(&rect);
	}
	else
		visual->setSrcRect((int)prop.p_vec[0], (int)prop.p_vec[1], (int)prop.p_vec[2], (int)prop.p_vec[3]);

	return true;
}

CzXomlProperty CzActorImage::_getSrcRect(IzXomlResource* target)
{
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorImage*)target)->getVisual();
	return CzXomlProperty(visual->getSrcRect());
}

bool CzActorImage::_setSrcTiling(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorImage* actor = (CzActorImage*)target;

	CzBitmapSprite* visual = (CzBitmapSprite*)actor->getVisual();
	if (add)
	{
		CzVec2 tiling = visual->getSrcTile();
		tiling.x += (int)prop.p_vec[0];
		tiling.y += (int)prop.p_vec[1];
		visual->setSrcTile(tiling.x, tiling.y);
	}
	else
		visual->setSrcTile(prop.p_vec[0], prop.p_vec[1]);

	return true;
}

CzXomlProperty CzActorImage::_getSrcTiling(IzXomlResource* target)
{
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorImage*)target)->getVisual();
	return CzXomlProperty(visual->getSrcTile());
}

bool CzActorImage::_setSrcTiled(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorImage* actor = (CzActorImage*)target;

	CzBitmapSprite* visual = (CzBitmapSprite*)actor->getVisual();
	if (add)
		visual->setSrcTiled(!visual->getSrcTiled());
	else
		visual->setSrcTiled(prop.p_bool);

	return true;
}

CzXomlProperty CzActorImage::_getSrcTiled(IzXomlResource* target)
{
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorImage*)target)->getVisual();
	return CzXomlProperty(visual->getSrcTiled());
}

bool CzActorImage::_setImage(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorImage* actor = (CzActorImage*)target;
	if (prop.p_data == NULL)
		actor->SetImage(NULL);
	else
	{
		CzImage* image;
		if (prop.Type == PT_Resource)
			image = (CzImage*)prop.p_data;
		else
			image = (CzImage*)actor->getScene()->getResourceManager()->findResource(CzString::CalculateHash((const char*)prop.p_data), CzHashes::Image_Hash);
		if (image != NULL)
			actor->SetImage(image);
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - setProperty - Image not found in resources - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());
	}

	return true;
}

CzXomlProperty CzActorImage::_getImage(IzXomlResource* target)
{
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorImage*)target)->getVisual();
	return CzXomlProperty(visual->getImage());
}

bool CzActorImage::_setBrush(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorImage* actor = (CzActorImage*)target;

	CzBrushImage* brush;
	if (prop.Type == PT_Resource)
		brush = (CzBrushImage*)prop.p_data;
	else
		brush = (CzBrushImage*)actor->getScene()->getResourceManager()->findResource((const char*)prop.p_data, CzHashes::Brush_Hash);
	if (brush != NULL)
		actor->SetFromBrush(brush);
	else
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - setProperty - Brush not found in resources - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());

	return true;
}

CzXomlProperty CzActorImage::_getBrush(IzXomlResource* target)
{
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorImage*)target)->getVisual();
	return CzXomlProperty(visual->getBrush());
}

bool CzActorImage::_setFlipX(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorImage* actor = (CzActorImage*)target;
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorImage*)target)->getVisual();

	int transform = visual->getImageTransform();
	if (add)
	{
		if ((transform & CzBitmapSprite::ImageTransform_FlipX) != 0)
			transform &= 0xfffffff - CzBitmapSprite::ImageTransform_FlipX;
		else
			transform |= CzBitmapSprite::ImageTransform_FlipX;
			
	}
	else
	{
		if (prop.p_bool)
			transform |= CzBitmapSprite::ImageTransform_FlipX;
	}
	visual->setImageTransform((CzBitmapSprite::eImageTransform)transform);

	return true;
}

CzXomlProperty CzActorImage::_getFlipX(IzXomlResource* target)
{
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorImage*)target)->getVisual();
	return CzXomlProperty((visual->getImageTransform() & CzBitmapSprite::ImageTransform_FlipX) ? true : false);
}

bool CzActorImage::_setFlipY(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorImage* actor = (CzActorImage*)target;
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorImage*)target)->getVisual();

	int transform = visual->getImageTransform();
	if (add)
	{
		if ((transform & CzBitmapSprite::ImageTransform_FlipY) != 0)
			transform &= 0xfffffff - CzBitmapSprite::ImageTransform_FlipY;
		else
			transform |= CzBitmapSprite::ImageTransform_FlipY;
			
	}
	else
	{
		if (prop.p_bool)
			transform |= CzBitmapSprite::ImageTransform_FlipY;
	}
	visual->setImageTransform((CzBitmapSprite::eImageTransform)transform);

	return true;
}

CzXomlProperty CzActorImage::_getFlipY(IzXomlResource* target)
{
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorImage*)target)->getVisual();
	return CzXomlProperty((visual->getImageTransform() & CzBitmapSprite::ImageTransform_FlipY) ? true : false);
}

bool CzActorImage::_setSkew(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorImage* actor = (CzActorImage*)target;
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorImage*)target)->getVisual();

	if (add)
	{
		CzVec4 skew = visual->getSkew();
		visual->setSkew(skew.x + prop.p_vec[0], skew.y + prop.p_vec[1], skew.z + prop.p_vec[2], skew.w + prop.p_vec[3]);
	}
	else
		visual->setSkew(prop.p_vec[0], prop.p_vec[1], prop.p_vec[2], prop.p_vec[3]);

	return true;
}

CzXomlProperty CzActorImage::_getSkew(IzXomlResource* target)
{
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorImage*)target)->getVisual();
	return CzXomlProperty(visual->getSkew());
}

bool CzActorImage::_setBeforeChildren(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorImage* actor = (CzActorImage*)target;
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorImage*)target)->getVisual();

	if (add)
		visual->setBeforeChildren(!visual->getBeforeChildren());
	else
		visual->setBeforeChildren(prop.p_bool);

	return true;
}

CzXomlProperty CzActorImage::_getBeforeChildren(IzXomlResource* target)
{
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorImage*)target)->getVisual();
	return CzXomlProperty(visual->getBeforeChildren());
}

bool CzActorImage::_setFilter(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorImage* actor = (CzActorImage*)target;
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorImage*)target)->getVisual();

	if (add)
		visual->setFilter(!visual->isFilter());
	else
		visual->setFilter(prop.p_bool);

	return true;
}

CzXomlProperty CzActorImage::_getFilter(IzXomlResource* target)
{
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorImage*)target)->getVisual();
	return CzXomlProperty(visual->isFilter());
}

bool CzActorImage::_setAspectLock(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorImage* actor = (CzActorImage*)target;
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorImage*)target)->getVisual();

	unsigned int aspect_hash = CzString::CalculateHash((const char*)prop.p_data);

	if (aspect_hash == CZ_HASH("x"))
		actor->setAspectLock(AL_X);
	else
	if (aspect_hash == CZ_HASH("y"))
		actor->setAspectLock(AL_Y);
	else
	if (aspect_hash == CZ_HASH("none"))
		actor->setAspectLock(AL_None);
	else
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - setProperty - Invalid value for AspectLock expected x, y, or none - ", (const char*)prop.p_bool, actor->getDebugInfo().c_str());
	
	CzIVec2 size = actor->getSize();
	actor->SetDestSize(size.x, size.y);	// Cause aspect lock change

	return true;
}

CzXomlProperty CzActorImage::_getAspectLock(IzXomlResource* target)
{
	CzActorImage* actor = (CzActorImage*)target;
	return CzXomlProperty(CzXoml::AspectLockNames[actor->getAspectLock()]);
}

bool CzActorImage::_setAlphaMode(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorImage* actor = (CzActorImage*)target;
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorImage*)target)->getVisual();

	unsigned int alpha_mode_hash = CzString::CalculateHash((const char*)prop.p_data);

	if (alpha_mode_hash == CZ_HASH("none"))
		visual->setAlphaMode(AlphaMode_None);
	else
	if (alpha_mode_hash == CZ_HASH("half"))
		visual->setAlphaMode(AlphaMode_Half);
	else
	if (alpha_mode_hash == CZ_HASH("add"))
		visual->setAlphaMode(AlphaMode_Add);
	else
	if (alpha_mode_hash == CZ_HASH("sub"))
		visual->setAlphaMode(AlphaMode_Sub);
	else
	if (alpha_mode_hash == CZ_HASH("blend"))
		visual->setAlphaMode(AlphaMode_Blend);
#if defined(_DEBUG)
	else
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - setProperty - Invalid value for AlphaMode - ", (const char*)prop.p_bool, actor->getDebugInfo().c_str());
#endif

	return true;
}

CzXomlProperty CzActorImage::_getAlphaMode(IzXomlResource* target)
{
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorImage*)target)->getVisual();
	return CzXomlProperty(CzXoml::AlphaModeNames[visual->getAlphaMode()]);
}

bool CzActorImage::_setGeometry(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorImage* actor = (CzActorImage*)target;
	CzGeometry* geom;
	if (prop.Type == PT_Resource)
		geom = (CzGeometry*)prop.p_data;
	else
		geom = (CzGeometry*)actor->getScene()->getResourceManager()->findResource(CzString::CalculateHash((const char*)prop.p_data), CzHashes::Geometry_Hash);
	if (geom == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - setProperty - Could not find named geometry in the scene or in the global resources");
		return false;
	}
	actor->setGeometry(geom);

	return true;
}

CzXomlProperty CzActorImage::_getGeometry(IzXomlResource* target)
{
	CzActorImage* actor = (CzActorImage*)target;
	return CzXomlProperty(actor->getGeometry());
}


/**
 @fn	bool CzActorImage::InitWithImage(CzImage* image, int width, int height)

 @brief	Initialise the bitmap based actor from a brush

 Initialises the bitmap actor from an image, width and height. Note that this method will create a CzBitmapSprite visual and add it to the scenes sprite manager 

 @param [in,out]	image	If non-null, the image.
 @param	width			 	The display width of the actor.
 @param	height			 	The display height of the actor.

 @return	true if it succeeds, false if it fails.
 */

bool CzActorImage::InitWithImage(CzImage* image, int width, int height)
{
	// Create sprite
	CzBitmapSprite* sprite = new CzBitmapSprite();
	if (sprite == NULL)
		return false;
	sprite->Init();
	sprite->setOrphan(Orphan);
	sprite->setIgnoreCamera(IgnoreCamera);

	// Set sprite as visual
	Visual = sprite;
	Visual->setActor(this);

	OriginalSize.x = width;
	OriginalSize.y = height;
	OriginalMargin = Margin;
	CalculateSizes(width, height);
	CalculateMargins(Margin);

	// Add sprite to the sprite manager so it can be managed and drawn
	Scene->getSpriteManager()->addSprite(sprite);

	// Set-up LinkedTo
	if (LinkedTo != NULL)
		Visual->setLinkedTo(LinkedTo->getVisual());

	// Set sprite image
	sprite->setImage(image);
	SetSrcRect(0, 0, width, height);
	SetDestSize(width, height);

	return true;
}

/**
 @fn	bool CzActorImage::InitWithBrush(IzBrush* brush, int width, int height)

 @brief	Initialise the bitmap based actor from a brush

 Initialises the bitmap actor from a brush, width and height. The image and image's source rectangle will be taken from the brush. Note that this method will create a CzBitmapSprite 
 visual and add it to the scenes sprite manager if the brush type is of IzBrush::BT_Image. If the brush type is IzBrush::BT_9Patch then a CzBitmapSprite9 sprite will be created instead.

 @param [in]	brush		If non-null, the brush.
 @param	width			 	The width.
 @param	height			 	The height.

 @return	true if it succeeds, false if it fails.
 */

bool CzActorImage::InitWithBrush(IzBrush* brush, int width, int height)
{
	// Calculate size on parent to screen if size not specified
	OriginalSize.x = width;
	OriginalSize.y = height;
	OriginalMargin = Margin;
	CalculateSizes(width, height);
	CalculateMargins(Margin);

	if (brush->getBrushType() == IzBrush::BT_Image)
	{
		// Create sprite
		CzBitmapSprite* sprite = new CzBitmapSprite();
		if (sprite == NULL)
			return false;
		sprite->Init();
		sprite->setOrphan(Orphan);
		sprite->setIgnoreCamera(IgnoreCamera);
			
		// Set sprite image
		sprite->setFromBrush((CzBrushImage*)brush);

		// Set sprite as visual
		Visual = sprite;
		Visual->setActor(this);

		// Set sprite size
		SetDestSize(width, height);

		// Add sprite to the sprite manager so it can be managed and drawn
		Scene->getSpriteManager()->addSprite(sprite);

		// Set-up LinkedTo
		if (LinkedTo != NULL)
			Visual->setLinkedTo(LinkedTo->getVisual());

	}
	else
	if (brush->getBrushType() == IzBrush::BT_9Patch)
	{
		// Create sprite
		CzBitmapSprite9* sprite = new CzBitmapSprite9();
		if (sprite == NULL)
			return false;
		sprite->Init();
		sprite->setOrphan(Orphan);
		sprite->setIgnoreCamera(IgnoreCamera);
			
		// Set UV's from brush
		sprite->setFromBrush((CzBrushImage9*)brush);

		// Set sprite as visual
		Visual = sprite;
		Visual->setActor(this);

		// Set sprite size
		SetDestSize(width, height);

		// Add sprite to the sprite manager so it can be managed and drawn
		Scene->getSpriteManager()->addSprite(sprite);

		// Set-up LinkedTo
		if (LinkedTo != NULL)
			Visual->setLinkedTo(LinkedTo->getVisual());
	}
	else
	{
		// Set destination size
		SetDestSize(width, height);
	}

	return true;
}

/**
 @fn	bool CzActorImage::Init(IzBrush* brush, int width, int height)

 @brief	Initialise the bitmap based actor from a brush

 Initialises the bitmap actor from a brush, width and height. The image and image's source rectangle will be taken from the brush. Note that this method will create a CzBitmapSprite 
 visual and add it to the scenes sprite manager if the brush type is of IzBrush::BT_Image. If the brush type is IzBrush::BT_9Patch then a CzBitmapSprite9 sprite will be created instead.

 @param [in]	brush		If non-null, the brush.
 @param	width			 	The width.
 @param	height			 	The height.

 @return	true if it succeeds, false if it fails.
 */

bool CzActorImage::Init(IzBrush* brush, int width, int height)
{
	CzActor::Init();

	// Create sprite
	if (!InitWithBrush(brush, width, height))
		return false;

	return true;
}

/**
 @fn	bool Init::Init(CzImage* image, int width, int height)

 @brief	Initialise the bitmap based actor from an image.

 Initialises the bitmap actor from an image, width and height. The bitmaps source rectangle will be made the same size as the width and height. Note that this method will create a 
 CzBitmapSprite visual and add it to the scenes sprite manager. 

 @param [in]	image		If non-null, the image.
 @param	width			 	The width.
 @param	height			 	The height.

 @return	true if it succeeds, false if it fails.
 */

bool CzActorImage::Init(CzImage* image, int width, int height)
{
	CzActor::Init();

	// Create sprite
	if (!InitWithImage(image, width, height))
		return false;

	return true;
}

/**
 @fn	bool CzActorImage::Init(CzImage* image, CzIRect* src_rect)

 @brief	Initialise the bitmap based actor from an image and a source rectangle.
	
 Initialises the bitmap actor from an image and a source rectangle. The actors visual size will be calculated from the width and height of the source rectangle. Note that this method 
 will create a CzBitmapSprite visual and add it to the scenes sprite manager.

 @param [in]	image		If non-null, the image.
 @param [in]	src_rect	The width.

 @return	true if it succeeds, false if it fails.

 ### param	height	The height.
 */

bool CzActorImage::Init(CzImage* image, CzIRect* src_rect)
{
	CzActor::Init();

	// Create sprite
	if (!InitWithImage(image, src_rect->w, src_rect->h))
		return false;
	SetSrcRect(src_rect->x, src_rect->y, src_rect->w, src_rect->h);

	return true;
}

/**
 @fn	bool CzActorImage::Init(CzImage* image, int x, int y, int w, int h)

 @brief	Initialise the bitmap based actor from an image and a source rectangle.
	
 Initialises the bitmap actor from an image and a source rectangle. The actors visual size will be calculated from the width and height of the source rectangle. Note that this method 
 will create a CzBitmapSprite visual and add it to the scenes sprite manager.

 @param [in,out]	image	If non-null, the image.
 @param	x				 	The x coordinate.
 @param	y				 	The y coordinate.
 @param	w				 	The width.
 @param	h				 	The height.

 @return	true if it succeeds, false if it fails.
 */

bool CzActorImage::Init(CzImage* image, int x, int y, int w, int h)
{
	CzActor::Init();

	// Create sprite
	if (!InitWithImage(image, w, h))
		return false;
	SetSrcRect(x, y, w, h);

	return true;
}

/**
 @fn	void CzActorImage::SetSrcDest(int x, int y, int w, int h)

 @brief	Sets bitmap source rectangle and visual size.

 Changes the source rectangle of the visual and changes its size to w, h.

 @param	x	The x coordinate.
 @param	y	The y coordinate.
 @param	w	The width.
 @param	h	The height.
 */

void CzActorImage::SetSrcDest(int x, int y, int w, int h)
{
	SetSrcRect(x, y, w, h);
	SetDestSize(w, h);
}

/**
 @fn	void CzActorImage::SetSrcRect(int x, int y, int w, int h)

 @brief	Sets bitmap source rectangle.

 Changes the source rectangle of the visual.

 @param	x	The x coordinate.
 @param	y	The y coordinate.
 @param	w	The width.
 @param	h	The height.
 */

void CzActorImage::SetSrcRect(int x, int y, int w, int h)
{
	if (Visual != NULL)
		((CzBitmapSprite*)Visual)->setSrcRect(x, y, w, h);
}

/**
 @fn	void CzActorImage::SetDestSize(int w, int h)

 @brief	Sets actors visual size.

 Sets the actor visual size in scene units.

 @param	w	The width in scene units.
 @param	h	The height in scene units.
 */

void CzActorImage::SetDestSize(int w, int h)
{
/*	// If aspect is locked then adjust none locked axis to fit parent or scene container aspect ratio
	if (AspectLock == AL_X)
	{
		CzIVec2 size;
		size.x = ((CzBitmapSprite*)Visual)->getSrcWidth();
		if (size.x == 0)
			size.x = ((CzBitmapSprite*)Visual)->getImage()->getWidth();
		size.y = ((CzBitmapSprite*)Visual)->getSrcHeight();
		if (size.y == 0)
			size.y = ((CzBitmapSprite*)Visual)->getImage()->getHeight();
//		size = Scene->getScreenSize();
		if (size.x != 0)
			h = (size.y * w) / size.x;
	}
	else
	if (AspectLock == AL_Y)
	{
		CzIVec2 size;
		size.x = ((CzBitmapSprite*)Visual)->getSrcWidth();
		if (size.x == 0)
			size.x = ((CzBitmapSprite*)Visual)->getImage()->getWidth();
		size.y = ((CzBitmapSprite*)Visual)->getSrcHeight();
		if (size.y == 0)
			size.y = ((CzBitmapSprite*)Visual)->getImage()->getHeight();
		if (size.y != 0)
			w = (size.x * h) / size.y;
	}*/
	Size.x = w;
	Size.y = h;
	if (Visual != NULL)
		((CzBitmapSprite*)Visual)->setDestSize(w, h);
	if (Docking != CzScene::Dock_None)
		setDocking(Docking);
}

/**
 @fn	void CzActorImage::UpdateAspect()

 @brief	Updates the aspect of this actor.

 if this actor has been marked so that it locks its aspect ratio to the x or y axix then this utility method will calculate the final size of the actor based on the aspect ratio of the 
 bitmap that is attached to this actors visual. Note that this method is automatically called by the actors Update() method.

 */

void CzActorImage::UpdateAspect()
{
	if (Visual == NULL || AspectLock == AL_None)
		return;
	CzIVec2 size;
	size.x = ((CzBitmapSprite*)Visual)->getSrcWidth();
	size.y = ((CzBitmapSprite*)Visual)->getSrcHeight();
	if (size.x == 0)
		size.x = ((CzBitmapSprite*)Visual)->getImage()->getWidth();
	if (size.y == 0)
		size.y = ((CzBitmapSprite*)Visual)->getImage()->getHeight();

	// If aspect is locked then adjust none locked axis to fit bitmap size
	if (AspectLock == AL_X)
	{
		if (size.x != 0)
			Size.y = (size.y * Size.x) / size.x;
	}
	else
	if (AspectLock == AL_Y)
	{
		if (size.y != 0)
			Size.x = (size.x * Size.y) / size.y;
	}

	if (Size != OldSrcSize)
	{
		((CzBitmapSprite*)Visual)->setDestSize(Size.x, Size.y);
		OldSrcSize.x = Size.x;
		OldSrcSize.y = Size.y;
	}
}

/**
 @fn	void CzActorImage::SetImage(CzImage* image)

 @brief	Sets the image associated with the actors visual.

 Sets the image associated with the actors visual. if no visual is attached to this actor then a CzBitmapSprite is created and attached. 

 @param [in]	image	If non-null, the image.
 */

void CzActorImage::SetImage(CzImage* image)
{
	if (Visual == NULL)
	{
		CzBitmapSprite* sprite = new CzBitmapSprite();
		sprite->Init();
		sprite->setOrphan(Orphan);
		sprite->setIgnoreCamera(IgnoreCamera);

		// Set sprite image
		sprite->setImage(image);

		// Set sprite as visual
		Visual = sprite;
		Visual->setActor(this);

		// Add sprite to the sprite manager so it can be managed and drawn
		Scene->getSpriteManager()->addSprite(sprite);
	}
	else
	{
		CzBitmapSprite* sprite = (CzBitmapSprite*)Visual;

		// Set sprite image
		sprite->setImage(image);
	}
}

/**
 @fn	void CzActorImage::SetFromBrush(IzBrush* brush, bool resize)

 @brief	Sets the actirs brush.

 Sets the actors brush from the supplied brush. If resie is true then the actors visual size will be resized to the size of the new brush.

 @param [in]		brush	If non-null, the brush.
 @param	resize			 	true to resize.
 */

void CzActorImage::SetFromBrush(IzBrush* brush, bool resize)
{
	if (brush->getBrushType() == IzBrush::BT_Image || brush->getBrushType() == IzBrush::BT_9Patch)
	{
		CzBrushImage* b = (CzBrushImage*)brush;
		if (Visual != NULL)
			((CzBitmapSprite*)Visual)->setFromBrush(b);
		else
			SetImage(b->getImage());

		if (resize)
		{
			CzIRect rc = b->getSrcRect();
			if (OriginalSize.x == 0)
				Size.x = rc.w;
			else
			if (OriginalSize.x < 0)
			{
				if (LinkedTo != NULL)
					Size.x = (LinkedTo->getSize().x * -OriginalSize.x) / 100;
				else
					Size.x = (PLATFORM_DISPLAY->getCurrentWidth() * -OriginalSize.x) / 100;
			}
			if (OriginalSize.y == 0)
				Size.y = rc.h;
			else
			if (OriginalSize.y < 0)
			{
				if (LinkedTo != NULL)
					Size.y = (LinkedTo->getSize().y * -OriginalSize.y) / 100;
				else
					Size.y = (PLATFORM_DISPLAY->getCurrentHeight() * -OriginalSize.y) / 100;
			}
			SetDestSize(Size.x, Size.y);
		}
	}
}

/**
 @fn	void CzActorImage::setGeometry(CzGeometry* geom)

 @brief	Assigns a new geometry to the actor.

 @param [in]	geom	If non-null, the geometry.
 */

void CzActorImage::setGeometry(CzGeometry* geom)
{
	if (Visual->getSpriteType() == CzSprite::ST_9Patch)
	{
		Geometry = NULL;
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - Geometry is not compatible with 9-patch rendering, geometry was ignored");
	}
	else
	{
		Visual->setGeometry(Geometry);
		Geometry = geom;
	}
}


/**
 @fn	bool CzActorImage::UpdateVisual()

 @brief	Updates the actors visual.

 Transfers visual data from the actor to the visual such as position, angle, scale, colour etc.. 

 @return	true if it succeeds, false if it fails.
 */

bool CzActorImage::UpdateVisual()
{
	UpdateAspect();
	if (!CzActor::UpdateVisual())
		return false;

	return true;
}

/**
 @fn	bool CzActorImage::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the actor.

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types. Note that as 
 CzActorImage is derived from CzActor, all CzActor properties will also be checked against.

 @param	property_name		Name of the property.
 @param [out]	prop	The property data that.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActorImage::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	if (CzActor::getProperty(property_name, prop))
		return true;

	int err = IzXomlResource::getProperty(ActorImageClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorImageClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzActorImage::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the ActorImage.
	
 Sets the named property of this ActorImage. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzActorImage::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	if (CzActor::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(ActorImageClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorImageClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzActorImage::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced. Note that as CzActorImage is derived from CzActor, all CzActor properties will also be checked against.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in]	data	The property data.
 @param	delta				true to property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActorImage::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	if (CzActor::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(ActorImageClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorImageClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzActorImage::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates actor property from supplied animation.

 Updates the actors property that is specified by the supplied animation. When a timeline animation targets the property of an actor this method will be called by the animation to move the 
 animations frame data into its target property. Note that as CzActorImage is derived from CzActor, all CzActor properties will also be checked against.

 @param [in]	animation	If non-null, the animation.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActorImage::UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzActor::UpdateFromAnimation(animation))
		return true;

	int err = IzXomlResource::setProperty(ActorImageClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorImageClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzActorImage::UpdateBinding(unsigned int property_name, CzXomlVariable* var)

 @brief	Updates an actor property from the supplied XOML variable.

 Updates a single property from the value of the bound XOML variable. Note that as CzActorImage is derived from CzActor, all CzActor properties will also be checked against.

 @param	property_name  	Name of the property.
 @param [in,out]	var	If non-null, the variable.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActorImage::UpdateBinding(unsigned int property_name, CzXomlVariable* var)
{
	if (!var->Modified || var->getValue().isEmpty())
		return false;

	if (CzActor::UpdateBinding(property_name, var))
		return true;

	int err = IzXomlResource::setProperty(ActorImageClassDef, this, property_name, var);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorImageClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	int CzActorImage::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)

 @brief	Creates an instance of this class from XML

 LoadFromXoml is the main method used by all classes that can be instantiated from XOML mark-up. This method creates an instance of this class from the supplied XML node structure specified by node. 
 if parent is not NULL then the created instance will utilise the supplied parent to determine where it should live within the app. For example, if the parent is a scene then the actor will be placed 
 in that scene. if the parent is another actor then this actor will be placed into the scene of the supplied actor and linked as a child to the parent actor. If load_chlldren is true then any child 
 nodes contained within the actor will also be loaded. Note that as CzActorImage is derived from CzActor, CzActor::LoadFromXoml() will also be called to load CzActor specific properties. 

 @param [in]	parent		If non-null, the parent.
 @param	load_children	  	true to load child nodes.
 @param [in]	node  		If non-null, the XOML markup node that defines this object

 @return	negative value for error. if 0 then processing of further XOML will stop.
 */

int CzActorImage::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "ActorImage - Actor needs to be declared inside a scene or an actor", DebugInfo.c_str());
		return 0;
	}

	// Load main actor attributes
	int ret = CzActor::LoadFromXoml(parent, false, node);
	if (ret <= 0)
		return ret;

	// Process ActorImage specific attributes
	CzString*	image_name = NULL;
	CzString*	brush_name = NULL;
	CzVec2		tiling = CzVec2(1.0f, 1.0f);
	CzVec2		size = CzVec2(0, 0);
	CzIRect		src_rect(0, 0, 0, 0);
	int			image_transform = 0;
	CzVec4		skew = CzVec4(0, 0, 0, 0);
	bool		before = true;
	bool		filter = true;
	bool		tiled = false;
	bool		tiled_set = false;
	eCzAlphaMode alpha_mode = AlphaMode_Blend;
	CzString*	anchor = NULL;
	CzString*	geometry = NULL;
	CzVec2		com(0, 0);
	bool		sensor = false;
	CzString*	box2dmaterial_name = NULL;

	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Image_Hash)
			image_name = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Size_Hash)
		{
			if (!(*it)->getValueAsPoint(size))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - Invalid value for ActorImage::Size expected vec2", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::SrcRect_Hash)
		{
			if (!(*it)->getValueAsRect(src_rect))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - Invalid value for ActorImage::SrcRect expected rect", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::Tiling_Hash)
		{
			if (!(*it)->getValueAsPoint(tiling))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - Invalid value for ActorImage::Tiling expected vec2", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::Tiled_Hash)
		{
			tiled = (*it)->getValueAsBool();
			tiled_set = true;
		}
		else
		if (name_hash == CzHashes::Anchor_Hash)
			anchor = &(*it)->getValue();
		else
		if (name_hash == CzHashes::FlipX_Hash)
		{
			if ((*it)->getValueAsBool())
				image_transform |= CzBitmapSprite::ImageTransform_FlipX;
		}
		else
		if (name_hash == CzHashes::FlipY_Hash)
		{
			if ((*it)->getValueAsBool())
				image_transform |= CzBitmapSprite::ImageTransform_FlipY;
		}
		else
		if (name_hash == CzHashes::Brush_Hash || name_hash == CzHashes::Background_Hash)
		{
			brush_name = &(*it)->getValue();
		}
		else
		if (name_hash == CzHashes::Skew_Hash)
		{
			if (!(*it)->getValueAsPoint4(skew))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - Invalid value for ActorImage::Skew expected vec4", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::BeforeChildren_Hash)
			before = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Filter_Hash)
			filter = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::AlphaMode_Hash)
		{
			unsigned int alpha_mode_hash = (*it)->getValue().getHash();

			if (alpha_mode_hash == CZ_HASH("none"))
				alpha_mode = AlphaMode_None;
			else
			if (alpha_mode_hash == CZ_HASH("half"))
				alpha_mode = AlphaMode_Half;
			else
			if (alpha_mode_hash == CZ_HASH("add"))
				alpha_mode = AlphaMode_Add;
			else
			if (alpha_mode_hash == CZ_HASH("sub"))
				alpha_mode = AlphaMode_Sub;
			else
			if (alpha_mode_hash == CZ_HASH("blend"))
				alpha_mode = AlphaMode_Blend;
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - Invalid value for ActorImage::AlphaMode", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::AspectLock_Hash)
		{
			unsigned int aspect_hash = (*it)->getValue().getHash();

			if (aspect_hash == CZ_HASH("x"))
				AspectLock = AL_X;
			else
			if (aspect_hash == CZ_HASH("y"))
				AspectLock = AL_Y;
			else
			if (aspect_hash == CZ_HASH("none"))
				AspectLock = AL_None;
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - Invalid value for ActorImage::AspectLock expected x, y, or none", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::Geometry_Hash)
			geometry = &(*it)->getValue();
	}

	// Check for geometry
	if (geometry != NULL)
	{
		Geometry = (CzGeometry*)Scene->getResourceManager()->findResource(geometry->getHash(), CzHashes::Geometry_Hash);
		if (Geometry == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - Could not find named geometry in the scene or in the global resources - ", geometry->c_str(), DebugInfo.c_str());
	}

	CzBrushImage* brush = NULL;
	CzImage* image = NULL;
	if (brush_name != NULL && !brush_name->isEmpty())
	{
		brush = (CzBrushImage*)Scene->getResourceManager()->findResource(brush_name->getHash(), CzHashes::Brush_Hash);
		if (brush != NULL)
		{
			image = brush->getImage();
			src_rect = brush->getSrcRect();
		}
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - Could not find named brush in the scene or in the global resources - ", brush_name->c_str(), DebugInfo.c_str());
	}
	else
	{
		// Search the scenes resources for the specified image
		if (image_name != NULL)
		{
			image = (CzImage*)Scene->getResourceManager()->findResource(image_name->c_str(), CzHashes::Image_Hash);
			if (image == NULL)
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - Could not find named image in the scene or in the global resources - ", image_name->c_str(), DebugInfo.c_str());
			}
		}
	}

	// If src_rect is specified and no size then take size from the src_rect
	// If both src_rect and size are 0 then set size to 100% of screen width and height
	if (src_rect.w != 0 && size.x == 0)
		size.x = (float)src_rect.w;
	else
	if (src_rect.w == 0 && size.x == 0)
		size.x = -100;

	if (src_rect.h != 0 && size.y == 0)
		size.y = (float)src_rect.h;
	else
	if (src_rect.h == 0 && size.y == 0)
		size.y = -100;

	// Initialise the image actor
	CzBitmapSprite* visual;
	if (brush != NULL)
	{
		Init(brush, (int)size.x, (int)size.y);
		visual = (CzBitmapSprite*)Visual;
	}
	else
	{
		Init(image, (int)size.x, (int)size.y);
		visual = (CzBitmapSprite*)Visual;;
		if (src_rect.w != 0)
		{
			if (Visual != NULL)
				visual->setSrcRect(&src_rect);
		}
	}
	if (Visual != NULL)
	{
		visual->setImageTransform((CzBitmapSprite::eImageTransform)image_transform);
		visual->setSkew(skew.x, skew.y, skew.z, skew.w);
		visual->setBeforeChildren(before);
		visual->setFilter(filter);
		visual->setAlphaMode(alpha_mode);
		visual->setSrcTile(tiling.x, tiling.y);
		if (tiled_set)
			visual->setSrcTiled(tiled);

		if (anchor != NULL)
		{
			if (anchor->getHash() != CzHashes::centre_Hash)
				visual->setAnchor(CzSprite::TopLeft);
		}
		if (Geometry != NULL)
		{
			if (visual->getSpriteType() == CzSprite::ST_9Patch)
			{
				Geometry = NULL;
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorImage - Geometry is not compatible with 9-patch rendering, geometry was ignored - ", geometry->c_str(), DebugInfo.c_str());
			}
			else
				visual->setGeometry(Geometry);
		}
	}

	// Set docking
	if (Docking != CzScene::Dock_None)
		setDocking(Docking);

	// Process inner tags
	if (load_children)
	{
		if (!CZ_XOML->Process(this, node))
			return 0;

	}

	return 1;
}

/**
 @fn	void CzActorImage::NotifyOrientationChange(CzScene::eOrientation old_orientation,  CzScene::eOrientation new_orientation)

 @brief	Handles the orientation change event.

 When a screen orientation or size change occurs all actors will receive this event emabling them to modify themselves to take advantage of the new screen orientation / size. the default 
 implementation for CzActorImage updates margin and visual sizes if they are precentage based. It also calls CzActor::NotifyOrientationChange() to update the position. 

 @param	old_orientation	The old orientation.
 @param	new_orientation	The new orientation.
 */
 
void CzActorImage::NotifyOrientationChange(CzScene::eOrientation old_orientation, CzScene::eOrientation new_orientation)
{
	CzIVec2		size = OriginalSize;
	
	UpdateMargins();
	if (LinkedTo != NULL)
	{
		if (OriginalSize.x < 0)
			size.x = (LinkedTo->getSize().x * -OriginalSize.x) / 100;
		if (OriginalSize.y < 0)
			size.y = (LinkedTo->getSize().y * -OriginalSize.y) / 100;

		if (size.x == 0)
			size.x = LinkedTo->getSize().x;
		if (size.y == 0)
			size.y = LinkedTo->getSize().y;
	}
	else
	{
		// If negative size has been specified then base size on a percentage of virtual canvas
		if (OriginalSize.x < 0)
			size.x = (int)((Scene->getScreenSize().x / Scene->getVirtualTransform().m[0][0] * -OriginalSize.x) / 100);
		if (OriginalSize.y < 0)
			size.y = (int)((Scene->getScreenSize().y / Scene->getVirtualTransform().m[1][1] * -OriginalSize.y) / 100);
	}

	SetDestSize(size.x, size.y);

	CzActor::NotifyOrientationChange(old_orientation, new_orientation);
}





