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

#include "CzActorText.h"
#include "CzTextSprite.h"
#include "CzHashes.h"
#include "CzUIBase.h"
#include "CzXomlResourceManager.h"

CzXomlClassDef* CzActorText::ActorTextClassDef = NULL;

/**
 @fn	void CzActorText::InitClass()

 @brief	Initialises the class.
    
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzActorText::InitClass()
{
	// Create class properties
	ActorTextClassDef = new CzXomlClassDef();
	ActorTextClassDef->addProperty(new CzXomlClassProperty("Text",				PT_String,		CzActorText::_setText,				CzActorText::_getText));
	ActorTextClassDef->addProperty(new CzXomlClassProperty("Font",				PT_Resource,	CzActorText::_setFont,				CzActorText::_getFont));
	ActorTextClassDef->addProperty(new CzXomlClassProperty("Rect",				PT_Vec4,		CzActorText::_setRect,				CzActorText::_getRect));
	ActorTextClassDef->addProperty(new CzXomlClassProperty("AlignH",			PT_String,		CzActorText::_setAlignH,			NULL));
	ActorTextClassDef->addProperty(new CzXomlClassProperty("AlignV",			PT_String,		CzActorText::_setAlignV,			NULL));
	ActorTextClassDef->addProperty(new CzXomlClassProperty("Wrap",				PT_Bool,		CzActorText::_setWrap,				NULL));
	ActorTextClassDef->addProperty(new CzXomlClassProperty("Skew",				PT_Vec4,		CzActorText::_setSkew,				CzActorText::_getSkew));
	ActorTextClassDef->addProperty(new CzXomlClassProperty("BeforeChildren",	PT_Bool,		CzActorText::_setBeforeChildren,	CzActorText::_getBeforeChildren));
	ActorTextClassDef->addProperty(new CzXomlClassProperty("Filter",			PT_Bool,		CzActorText::_setFilter,			CzActorText::_getFilter));
	ActorTextClassDef->addProperty(new CzXomlClassProperty("AlphaMode",			PT_String,		CzActorText::_setAlphaMode,			CzActorText::_getAlphaMode));
	ActorTextClassDef->addProperty(new CzXomlClassProperty("AutoHeight",		PT_String,		CzActorText::_setAutoHeight,		CzActorText::_getAutoHeight));
	ActorTextClassDef->addProperty(new CzXomlClassProperty("TextSize",			PT_Vec2,		NULL,								CzActorText::_getTextSize));

}
void CzActorText::ReleaseClass()
{
	if (ActorTextClassDef != NULL)
	{
		ActorTextClassDef->Release();
		delete ActorTextClassDef;
		ActorTextClassDef = NULL;
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


bool CzActorText::_setText(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorText* actor = (CzActorText*)target;

	if (add)
	{
		CzString text = actor->getText();
		text += (const char*)prop.p_data;
		actor->setText(text.c_str());
	}
	else
		actor->setText((const char*)prop.p_data);

	return true;
}

CzXomlProperty CzActorText::_getText(IzXomlResource* target)
{
	CzActorText* actor = (CzActorText*)target;
	return CzXomlProperty(actor->getText());
}

bool CzActorText::_setFont(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorText* actor = (CzActorText*)target;
	CzFont* font;
	if (prop.Type == PT_Resource)
		font = (CzFont*)prop.p_data;
	else
		font = (CzFont*)actor->getScene()->getResourceManager()->findResource((const char*)prop.p_data, CzHashes::Font_Hash);
	if (font != NULL)
		actor->setFont(font);
	else
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorText - setProperty - Font not found in resources - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());

	return true;
}

CzXomlProperty CzActorText::_getFont(IzXomlResource* target)
{
	CzActorText* actor = (CzActorText*)target;
	return CzXomlProperty(actor->getFont());
}

bool CzActorText::_setRect(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorText* actor = (CzActorText*)target;

	if (add)
	{
		CzIRect rc = actor->getRect();
		rc.x += (int)prop.p_vec[0];
		rc.y += (int)prop.p_vec[1];
		rc.w += (int)prop.p_vec[2];
		rc.h += (int)prop.p_vec[3];
		actor->setRect(rc);
	}
	else
	{
		CzIRect rc;
		rc.x = (int)prop.p_vec[0];
		rc.y = (int)prop.p_vec[1];
		rc.w = (int)prop.p_vec[2];
		rc.h = (int)prop.p_vec[3];
		actor->setRect(rc);
	}

	return true;
}

CzXomlProperty CzActorText::_getRect(IzXomlResource* target)
{
	CzActorText* actor = (CzActorText*)target;
	return CzXomlProperty(actor->getRect());
}

bool CzActorText::_setAlignH(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorText* actor = (CzActorText*)target;
	eCzAlignH align;

	unsigned int align_hash = CzString::CalculateHash((const char*)prop.p_data);
	if (align_hash == CzHashes::left_Hash)
		align = AlignH_Left;
	else
	if (align_hash == CzHashes::right_Hash)
		align = AlignH_Right;
	else
	if (align_hash == CzHashes::centre_Hash)
		align = AlignH_Centre;
	else
		return false;

	actor->setAlignH(align);

	return true;
}

bool CzActorText::_setAlignV(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorText* actor = (CzActorText*)target;
	eCzAlignV align;

	unsigned int align_hash = CzString::CalculateHash((const char*)prop.p_data);
	if (align_hash == CzHashes::top_Hash)
		align = AlignV_Top;
	else
	if (align_hash == CzHashes::bottom_Hash)
		align = AlignV_Bottom;
	else
	if (align_hash == CzHashes::middle_Hash)
		align = AlignV_Middle;
	else
		return false;

	actor->setAlignV(align);

	return true;
}

bool CzActorText::_setWrap(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorText* actor = (CzActorText*)target;

	if (prop.p_bool)
		actor->setFlags(CzFontFlags_None);
	else
		actor->setFlags(CzFontFlags_NoWrap);

	return true;
}

bool CzActorText::_setSkew(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorText* actor = (CzActorText*)target;
	CzTextSprite* visual = (CzTextSprite*)((CzActorText*)target)->getVisual();

	if (add)
	{
		CzVec4 skew = visual->getSkew();
		visual->setSkew(skew.x + prop.p_vec[0], skew.y + prop.p_vec[1], skew.z + prop.p_vec[2], skew.w + prop.p_vec[3]);
	}
	else
		visual->setSkew(prop.p_vec[0], prop.p_vec[1], prop.p_vec[2], prop.p_vec[3]);

	return true;
}

CzXomlProperty CzActorText::_getSkew(IzXomlResource* target)
{
	CzTextSprite* visual = (CzTextSprite*)((CzActorText*)target)->getVisual();
	return CzXomlProperty(visual->getSkew());
}

bool CzActorText::_setBeforeChildren(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorText* actor = (CzActorText*)target;
	CzTextSprite* visual = (CzTextSprite*)((CzActorText*)target)->getVisual();

	if (add)
		visual->setBeforeChildren(!visual->getBeforeChildren());
	else
		visual->setBeforeChildren(prop.p_bool);

	return true;
}

CzXomlProperty CzActorText::_getBeforeChildren(IzXomlResource* target)
{
	CzTextSprite* visual = (CzTextSprite*)((CzActorText*)target)->getVisual();
	return CzXomlProperty(visual->getBeforeChildren());
}

bool CzActorText::_setFilter(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorText* actor = (CzActorText*)target;
	CzTextSprite* visual = (CzTextSprite*)((CzActorText*)target)->getVisual();

	if (add)
		visual->setFilter(!visual->isFilter());
	else
		visual->setFilter(prop.p_bool);

	return true;
}

CzXomlProperty CzActorText::_getFilter(IzXomlResource* target)
{
	CzTextSprite* visual = (CzTextSprite*)((CzActorText*)target)->getVisual();
	return CzXomlProperty(visual->isFilter());
}

bool CzActorText::_setAlphaMode(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorText* actor = (CzActorText*)target;
	CzTextSprite* visual = (CzTextSprite*)((CzActorText*)target)->getVisual();

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
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorText - setProperty - Invalid value for AlphaMode - ", (const char*)prop.p_bool, actor->getDebugInfo().c_str());
#endif

	return true;
}

CzXomlProperty CzActorText::_getAlphaMode(IzXomlResource* target)
{
	CzTextSprite* visual = (CzTextSprite*)((CzActorText*)target)->getVisual();
	return CzXomlProperty(CzXoml::AlphaModeNames[visual->getAlphaMode()]);
}

bool CzActorText::_setAutoHeight(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorText* actor = (CzActorText*)target;
	if (add)
		actor->setAutoHeight(!actor->isAutoHeight());
	else
		actor->setAutoHeight(prop.p_bool);

	return true;
}

CzXomlProperty CzActorText::_getAutoHeight(IzXomlResource* target)
{
	CzActorText* actor = (CzActorText*)target;
	return CzXomlProperty(actor->isAutoHeight());
}

CzXomlProperty CzActorText::_getTextSize(IzXomlResource* target)
{
	CzTextSprite* sprite = (CzTextSprite*)((CzActorText*)target)->getVisual();
	CzFontPreparedText font_data = sprite->getPreparedData();
	int width = PLATFORM_FONT->GetTextWidth(font_data);
	int height = PLATFORM_FONT->GetTextHeight(font_data);

	return CzXomlProperty(CzVec2((float)width, (float)height));
}



/**
 @fn	const char* CzActorText::getText() const

 @brief	Returns the text associates with this actor.

 @return	null if no text, else the text.
 */

const char* CzActorText::getText() const
{
	CzTextSprite* visual = (CzTextSprite*)Visual;
	if (visual != NULL)
		return ((CzTextSprite*)Visual)->getText().c_str();

	return NULL;
}

/**
 @fn	void CzActorText::recalculateHeight()

 @brief	Recalculates the height of the text and sets a new size.

 */

void CzActorText::recalculateHeight()
{
	if (!AutoHeight || Visual == NULL)
		return;

	CzTextSprite* sprite = (CzTextSprite*)Visual;
	CzIRect rect = sprite->getRect();
	CzFontPreparedText font_data = sprite->getPreparedData();
	int height = PLATFORM_FONT->GetTextHeight(font_data);
	rect.h = height;
	setRect(rect);

//	if (LinkedTo != NULL && LinkedTo->getCategory() == CzActor::AC_UI)
//		((CzUIBase*)LinkedTo)->setLayoutDirty(true);

}


/**
 @fn	void CzActorText::setText(const char* text)

 @brief	Sets the text that will be displayed by this actor.

 @param	text	The text to display.
 */

void CzActorText::setText(const char* text)
{
	if (CzString::CalculateHash(text) == CzString::CalculateHash(getText()))
		return;

	CzTextSprite* visual = (CzTextSprite*)Visual;
	if (visual != NULL)
	{
		visual->setText(text);

		// Generate the prepared fnot data
		CzTextSprite* sprite = (CzTextSprite*)visual;
		if (sprite->getFont() != NULL && sprite->getFont()->isFontLoaded())
		{
			sprite->RebuildText();

			// Resize rect to fit the text coverage
			recalculateHeight();
		}
	}
}

/**
 @fn	void CzActorText::setRect(const CzIRect& rect)

 @brief	Sets a rectangular area that the text will be drawn inside of.

 Sets a rectangular area that the text will be drawn inside of, taking into account any text margins. If this actor has docking set then it will be re-docked taking into 
 account its new size.

 @param	rect	The rectangular area.
 */

void CzActorText::setRect(const CzIRect& rect)
{
	CzIRect rc = rect;
	// Adjust for text margin
	if (MarginIsPadding)
	{
		rc.x -= Margin.x;
		rc.y -= Margin.w;
		rc.w += Margin.x + Margin.y;
		rc.h += Margin.w + Margin.h;
	}
	else
	{
		rc.x += Margin.x;
		rc.y += Margin.w;
		rc.w -= Margin.x + Margin.y;
		rc.h -= Margin.w + Margin.h;
	}
	CzTextSprite* visual = (CzTextSprite*)Visual;
	if (visual != NULL)
		visual->setRect(rc);
	Size.x = rc.w;
	Size.y = rc.h;
	if (Docking != CzScene::Dock_None)
		setDocking(Docking);

}

/**
 @fn	CzIRect CzActorText::getRect() const

 @brief	Gets a rectangular area that the text will be drawn inside of.

 @return	The rectangular area.
 */

CzIRect CzActorText::getRect() const
{
	CzTextSprite* visual = (CzTextSprite*)Visual;
	if (visual != NULL)
		return visual->getRect();

	return CzIRect();
}


/**
 @fn	void CzActorText::setFlags(eCzFontFlags flags)

 @brief	Sets text flags.

 Sets flags that modify how the text is rendered. 

 @param	flags	The flags.
 */

void CzActorText::setFlags(eCzFontFlags flags)
{
	CzTextSprite* visual = (CzTextSprite*)Visual;
	if (visual != NULL)
		visual->setFlags(flags);
}

/**
 @fn	void CzActorText::setFont(CzFont* font)

 @brief	Sets a font.

 Sets the font that is used to render the text.

 @param [in,out]	font	If non-null, the font.
 */

void CzActorText::setFont(CzFont* font)
{
	CzTextSprite* visual = (CzTextSprite*)Visual;
	if (visual != NULL)
		visual->setFont(font);
}

/**
 @fn	CzFont* CzActorText::getFont()

 @brief	Gets a font.
    
 Gets the font that is used to render the text.

 @return	null if it fails, else the font.
 */

CzFont* CzActorText::getFont()
{
	CzTextSprite* visual = (CzTextSprite*)Visual;
	if (visual != NULL)
		return visual->getFont();

	return NULL;
}

/**
 @fn	void CzActorText::setAlignH(eCzAlignH align)

 @brief	Sets horizontal alignment of the text.

 Sets how text will be horizontally aligned. Alignment options include:
 - AlignH_Left
 - AlignH_Centre
 - AlignH_Right

 @param	align	The horizontal alignment.
 */

void CzActorText::setAlignH(eCzAlignH align)
{
	CzTextSprite* visual = (CzTextSprite*)Visual;
	if (visual != NULL)
		visual->setAlignH(align);
}

/**
 @fn	void CzActorText::setAlignV(eCzAlignH align)

 @brief	Sets vertical alignment of the text.

 Sets how text will be vertically aligned. Alignment options include:
 - AlignV_Top
 - AlignV_Middle
 - AlignV_Bottom

 @param	align	The vertical alignment.
 */

void CzActorText::setAlignV(eCzAlignV align)
{
	CzTextSprite* visual = (CzTextSprite*)Visual;
	if (visual != NULL)
		visual->setAlignV(align);
}

/**
 @fn	bool CzActorText::Init(CzFont* font, const CzIRect& rect, const CzString& text, int flags)

 @brief	Initialise the text based actor from a font and text.

 Initialises the text actor from a font, a rect, text and flags. The size of the actor will be chosen from the with and height of th supplied rectangle. If negative width and height 
 values are supplied then these values will be taken as percntage based sizes. Note that this method will create a CzTextSprite visual and add it to the scenes sprite manager. 

 @param [in,out]	font	If non-null, the font.
 @param [in,out]	rect	The rectangular area.
 @param [in,out]	text	The text.
 @param	flags				The flags.

 @return	true if it succeeds, false if it fails.
 */

bool CzActorText::Init(CzFont* font, const CzIRect& rect, const CzString& text, int flags)
{
	CzActor::Init();
	CzIRect rc = rect;

	OriginalSize.x = rc.w;
	OriginalSize.y = rc.h;
	OriginallRect = rc;
	OriginalMargin = Margin;
	CalculateMargins(Margin);
	if (LinkedTo != NULL)
	{
		// If negative size has been specified and part of a linked actor then base size on a percentage of parent
		if (rc.w < 0)
		{
			rc.w = (LinkedTo->getSize().x * -rc.w) / 100;
			rc.x = -rc.w / 2;
		}
		if (rc.h < 0)
		{
			rc.h = (LinkedTo->getSize().y * -rc.h) / 100;;
			rc.y = -rc.h / 2;
		}
	}
	else
	{
		// If negative size has been specified then base size on a percentage of virtual canvas
		if (rc.w < 0)
		{
			rc.w = (int)((Scene->getScreenSize().x / Scene->getVirtualTransform().m[0][0] * -rc.w) / 100);
			rc.x = -rc.w / 2;
		}
		if (rc.h < 0)
		{
			rc.h = (int)((Scene->getScreenSize().y / Scene->getVirtualTransform().m[1][1] * -rc.h) / 100);
			rc.y = -rc.h / 2;
		}
	}

	Size.x = rc.w;
	Size.y = rc.h;

	// Create sprite
	CzTextSprite* sprite = new CzTextSprite();
	if (sprite == NULL)
		return false;
	sprite->Init();
	sprite->setOrphan(Orphan);
	sprite->setIgnoreCamera(IgnoreCamera);

	// Set sprite parammeters
	sprite->setFont(font);
	sprite->setRect(rc);
	sprite->setFlags((eCzFontFlags)flags);
	sprite->setDestSize(rc.w, rc.h);
//	sprite->setText(text.c_str());

	// Set sprite as visual
	Visual = sprite;
	Visual->setActor(this);
	setText(text.c_str());

	// Add sprite to the sprite manager so it can be managed and drawn
	Scene->getSpriteManager()->addSprite(sprite);

	// Set-up LinkedTo
	if (LinkedTo != NULL)
		Visual->setLinkedTo(LinkedTo->getVisual());

	// Handle docking
	if (Docking != CzScene::Dock_None)
		setDocking(Docking);

	return true;
}

/**
 @fn	bool CzActorText::UpdateVisual()

 @brief	Updates the actors visual.

 Transfers visual data from the actor to the visual such as position, angle, scale, colour etc.. 

 @return	true if it succeeds, false if it fails.
 */

bool CzActorText::UpdateVisual()
{
	if (!CzActor::UpdateVisual())
		return false;

	if (!FontLoaded && AutoHeight && Visual != NULL)
	{
		CzTextSprite* sprite = (CzTextSprite*)Visual;
		if (sprite->getFont() != NULL && sprite->getFont()->isFontLoaded())
		{
			sprite->RebuildText();

			// Resize rect to fit the text coverage
			recalculateHeight();
			FontLoaded = true;
		}
	}

	return true;
}

/**
 @fn	bool CzActorText::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the actor.

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types. Note that as 
 CzActorText is derived from CzActor, all CzActor properties will also be checked against.

 @param	property_name		Name of the property.
 @param [out]	prop	The property data that.

 @return	true if it succeeds, false if property does not exist.
 */
 
bool CzActorText::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	if (CzActor::getProperty(property_name, prop))
		return true;

	int err = IzXomlResource::getProperty(ActorTextClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorTextClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorText - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzActorText::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the ActorText.
    
 Sets the named property of this ActorText. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzActorText::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	if (CzActor::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(ActorTextClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorTextClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorText - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzActorImage::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced. Note that as CzActorText is derived from CzActor, all CzActor properties will also be checked against.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in]	data		The property data.
 @param	delta				true to property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
 
bool CzActorText::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	if (CzActor::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(ActorTextClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorTextClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorText - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzActorText::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates actor property from supplied animation.

 Updates the actors property that is specified by the supplied animation. When a timeline animation targets the property of an actor this method will be called by the animation to move the 
 animations frame data into its target property. Note that as CzActorText is derived from CzActor, all CzActor properties will also be checked against.

 @param [in]	animation	If non-null, the animation.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActorText::UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzActor::UpdateFromAnimation(animation))
		return true;

	int err = IzXomlResource::setProperty(ActorTextClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorTextClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorText - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;

}

/**
 @fn	bool CzActorText::UpdateBinding(unsigned int property_name, CzXomlVariable* var)

 @brief	Updates an actor property from the supplied XOML variable.

 Updates a single property from the value of the bound XOML variable. Note that as CzActorText is derived from CzActor, all CzActor properties will also be checked against.

 @param	property_name  	Name of the property.
 @param [in,out]	var	If non-null, the variable.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActorText::UpdateBinding(unsigned int property_name, CzXomlVariable* var)
{
	if (!var->Modified || var->getValue().isEmpty())
		return false;

	if (CzActor::UpdateBinding(property_name, var))
		return true;

	int err = IzXomlResource::setProperty(ActorTextClassDef, this, property_name, var);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorTextClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorText - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	int CzActorText::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)

 @brief	Creates an instance of this class from XML

 LoadFromXoml is the main method used by all classes that can be instantiated from XOML mark-up. This method creates an instance of this class from the supplied XML node structure specified by node. 
 if parent is not NULL then the created instance will utilise the supplied parent to determine where it should live within the app. For example, if the parent is a scene then the actor will be placed 
 in that scene. if the parent is another actor then this actor will be placed into the scene of the supplied actor and linked as a child to the parent actor. If load_chlldren is true then any child 
 nodes contained within the actor will also be loaded. Note that as CzActorText is derived from CzActor, CzActor::LoadFromXoml() will also be called to load CzActor specific properties. 

 @param [in]	parent		If non-null, the parent.
 @param	load_children	  	true to load child nodes.
 @param [in]	node  		If non-null, the XOML markup node that defines this object

 @return	negative value for error. if 0 then processing of further XOML will stop.
 */

int CzActorText::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "ActorText - Needs to be declared inside a scene or an actor - ", DebugInfo.c_str());
		return 0;
	}

	// Load main actor attributes
	int ret = CzActor::LoadFromXoml(parent, false, node);
	if (ret <= 0)
		return ret;

	// Process Actor Font specific attributes
	CzString*		font_name = NULL;
	CzString*		text = NULL;
	CzIRect			rect = CzIRect(0, 0, -100, -100);
	eCzAlignH		alignh = AlignH_Centre;
	eCzAlignV		alignv = AlignV_Middle;
	int				flags = 0;
	CzVec4			skew = CzVec4(0, 0, 0, 0);
	bool			before = true;
	bool			filter = true;
	CzString*		anchor = NULL;
	eCzAlphaMode	alpha_mode = AlphaMode_Blend;

	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Font_Hash)
			font_name = &(*it)->getValue();
		else
		if (name_hash == CzHashes::rect_Hash)
		{
			if (!(*it)->getValueAsRect(rect))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorText - Invalid value for ActorText::Rect - ", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::Text_Hash)
			text = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Anchor_Hash)
			anchor = &(*it)->getValue();
		else
		if (name_hash == CzHashes::AlignH_Hash)
		{
			unsigned int align_hash = (*it)->getValue().getHash();
			if (align_hash == CzHashes::left_Hash)
				alignh = AlignH_Left;
			else
			if (align_hash == CzHashes::centre_Hash)
				alignh = AlignH_Centre;
			else
			if (align_hash == CzHashes::right_Hash)
				alignh = AlignH_Right;
		}
		else
		if (name_hash == CzHashes::AlignV_Hash)
		{
			unsigned int align_hash = (*it)->getValue().getHash();
			if (align_hash == CzHashes::top_Hash)
				alignv = AlignV_Top;
			else
			if (align_hash == CzHashes::middle_Hash)
				alignv = AlignV_Middle;
			else
			if (align_hash == CzHashes::bottom_Hash)
				alignv = AlignV_Top;
		}
		else
		if (name_hash == CzHashes::Wrap_Hash)
		{
			if ((*it)->getValueAsBool())
				flags |= CzFontFlags_None;
			else
				flags |= CzFontFlags_NoWrap;
		}
		else
		if (name_hash == CzHashes::Italic_Hash)
		{
		}
		else
		if (name_hash == CzHashes::Underline_Hash)
		{
		}
		else
		if (name_hash == CzHashes::Skew_Hash)
		{
			if (!(*it)->getValueAsPoint4(skew))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorText - Invalid value for ActorText::Skew - ", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::BeforeChildren_Hash)
			before = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Filter_Hash)
			filter = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::AutoHeight_Hash)
			AutoHeight = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::AlphaMode_Hash)
		{
			unsigned int alpha_hash = (*it)->getValue().getHash();

			if (alpha_hash == CZ_HASH("none"))
				alpha_mode = AlphaMode_None;
			else
			if (alpha_hash == CZ_HASH("half"))
				alpha_mode = AlphaMode_Half;
			else
			if (alpha_hash == CZ_HASH("add"))
				alpha_mode = AlphaMode_Add;
			else
			if (alpha_hash == CZ_HASH("sub"))
				alpha_mode = AlphaMode_Sub;
			else
			if (alpha_hash == CZ_HASH("blend"))
				alpha_mode = AlphaMode_Blend;
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorText - Invalid value for ActorImage::AlphaMode - ", DebugInfo.c_str());
		}
	}

	// Search the scenes resources for the specified font
	CzFont* font = NULL;
	if (font_name != NULL)
	{
		font = (CzFont*)Scene->getResourceManager()->findResource(font_name->c_str(), CzHashes::Font_Hash);
		if (font == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorText - Could not find named font in the scene or in the global resources - ", font_name->c_str(), DebugInfo.c_str());
	}

	// Initialise the image actor
	if (text == NULL)
	{
		CzString text;
		Init(font, rect, text, flags);
	}
	else
		Init(font, rect, *text, flags);

	// Set alignment
	setAlignH(alignh);
	setAlignV(alignv);

	// Set skewing
	Visual->setSkew(skew.x, skew.y, skew.z, skew.w);
	Visual->setBeforeChildren(before);
	Visual->setFilter(filter);
	Visual->setAlphaMode(alpha_mode);
	if (anchor != NULL)
	{
		if (anchor->getHash() != CzHashes::centre_Hash)
			Visual->setAnchor(CzSprite::TopLeft);
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
 @fn	void CzActorText::NotifyOrientationChange(CzScene::eOrientation old_orientation,  CzScene::eOrientation new_orientation)

 @brief	Handles the orientation change event.

 When a screen orientation or size change occurs all actors will receive this event emabling them to modify themselves to take advantage of the new screen orientation / size. the 
 default implementation for CzActorText updates margin and visual sizes if they are precentage based. It also calls CzActor::NotifyOrientationChange() to update the position. 

 @param	old_orientation	The old orientation.
 @param	new_orientation	The new orientation.
 */
 
void CzActorText::NotifyOrientationChange(CzScene::eOrientation old_orientation, CzScene::eOrientation new_orientation)
{
//	CzIRect		rect = ((CzTextSprite*)Visual)->getRect();
	CzIRect		rect = OriginallRect;

	UpdateMargins();
	if (LinkedTo != NULL)
	{
		// If negative size has been specified and part of a linked actor then base size on a percentage of parent
		if (OriginalSize.x < 0)
		{
			rect.w = (LinkedTo->getSize().x * -OriginalSize.x) / 100;
			rect.x = -rect.w / 2;
		}
		if (OriginalSize.y < 0)
		{
			rect.h = (LinkedTo->getSize().y * -OriginalSize.y) / 100;;
			rect.y = -rect.h / 2;
		}

		if (rect.w == 0)
			rect.w = LinkedTo->getSize().x;
		if (rect.h == 0)
			rect.h = LinkedTo->getSize().y;
	}
	else
	{
		// If negative size has been specified then base size on a percentage of virtual canvas
		if (OriginalSize.x < 0)
		{
			rect.w = (int)((Scene->getScreenSize().x / Scene->getVirtualTransform().m[0][0] * -OriginalSize.x) / 100);
			rect.x = -rect.w / 2;
		}
		if (OriginalSize.y < 0)
		{
			rect.h = (int)((Scene->getScreenSize().y / Scene->getVirtualTransform().m[1][1] * -OriginalSize.y) / 100);
			rect.y = -rect.h / 2;
		}
	}

	// Adjust for text margin
	rect.x += Margin.x;
	rect.y += Margin.w;
	rect.w -= Margin.x + Margin.y;
	rect.h -= Margin.w + Margin.h;
	setRect(rect);

	// Resize rect to fit the text coverage
	if (AutoHeight && Visual != NULL)
	{
		CzTextSprite* sprite = (CzTextSprite*)Visual;
		sprite->RebuildText();
		recalculateHeight();
	}

	CzActor::NotifyOrientationChange(old_orientation, new_orientation);
}





