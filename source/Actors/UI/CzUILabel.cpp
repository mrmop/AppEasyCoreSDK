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
#include "CzBitmapSprite.h"
#include "CzTextSprite.h"
#include "CzUILabel.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"

CzXomlClassDef* CzUILabel::LabelClassDef = NULL;

/**
 @fn	void CzUILabel::InitClass()

 @brief	Initialises the class.
    
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzUILabel::InitClass()
{
	// Create class properties
	LabelClassDef = new CzXomlClassDef();
	LabelClassDef->addProperty(new CzXomlClassProperty("Text",					PT_String,		CzUILabel::_setText,				CzUILabel::_getText));
	LabelClassDef->addProperty(new CzXomlClassProperty("SelectedText",			PT_String,		CzUILabel::_setSelectedText,		CzUILabel::_getSelectedText));
	LabelClassDef->addProperty(new CzXomlClassProperty("DisabledText",			PT_String,		CzUILabel::_setDisabledText,		CzUILabel::_getDisabledText));
	LabelClassDef->addProperty(new CzXomlClassProperty("TextColour",			PT_Vec4,		CzUILabel::_setTextColour,			CzUILabel::_getTextColour));
	LabelClassDef->addProperty(new CzXomlClassProperty("SelectedTextColour",	PT_Vec4,		CzUILabel::_setSelectedTextColour,	CzUILabel::_getSelectedTextColour));
	LabelClassDef->addProperty(new CzXomlClassProperty("DisabledTextColour",	PT_Vec4,		CzUILabel::_setDisabledTextColour,	CzUILabel::_getDisabledTextColour));
	LabelClassDef->addProperty(new CzXomlClassProperty("AlignH",				PT_String,		CzUILabel::_setAlignH,				NULL));
	LabelClassDef->addProperty(new CzXomlClassProperty("AlignV",				PT_String,		CzUILabel::_setAlignV,				NULL));
	LabelClassDef->addProperty(new CzXomlClassProperty("Wrap",					PT_Bool,		CzUILabel::_setWrap,				NULL));
	LabelClassDef->addProperty(new CzXomlClassProperty("TextSkew",				PT_Vec4,		CzUILabel::_setTextSkew,			CzUILabel::_getTextSkew));
	LabelClassDef->addProperty(new CzXomlClassProperty("AutoHeight",			PT_Bool,		CzUILabel::_setAutoHeight,			CzUILabel::_getAutoHeight));
	LabelClassDef->addProperty(new CzXomlClassProperty("TextFilter",			PT_Bool,		CzUILabel::_setTextFilter,			CzUILabel::_getTextFilter));
	LabelClassDef->addProperty(new CzXomlClassProperty("ActorText",				PT_String,		NULL,								CzUILabel::_getActorText));
	LabelClassDef->addProperty(new CzXomlClassProperty("TextMargin",			PT_Vec4,		CzUILabel::_setTextMargin,			CzUILabel::_getTextMargin));
	LabelClassDef->addProperty(new CzXomlClassProperty("Rect",					PT_Vec4,		CzUILabel::_setRect,				CzUILabel::_getRect));
	LabelClassDef->addProperty(new CzXomlClassProperty("TextSize",				PT_Vec2,		NULL,								CzUILabel::_getTextSize));
}
void CzUILabel::ReleaseClass()
{
	if (LabelClassDef != NULL)
	{
		LabelClassDef->Release();
		delete LabelClassDef;
		LabelClassDef = NULL;
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

bool CzUILabel::_setText(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUILabel* actor = (CzUILabel*)target;

	if (add)
	{
		CzString s = actor->getText();
		s += (const char*)prop.p_data;
		actor->setText(s.c_str());
	}
	else
		actor->setText((const char*)prop.p_data);

	return true;
}

CzXomlProperty CzUILabel::_getText(IzXomlResource* target)
{
	return CzXomlProperty(((CzUILabel*)target)->getText().c_str());
}

bool CzUILabel::_setSelectedText(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUILabel* actor = (CzUILabel*)target;

	if (add)
	{
		CzString s = actor->getSelectedText();
		s += (const char*)prop.p_data;
		actor->setSelectedText(s.c_str());
	}
	else
		actor->setSelectedText((const char*)prop.p_data);

	return true;
}

CzXomlProperty CzUILabel::_getSelectedText(IzXomlResource* target)
{
	return CzXomlProperty(((CzUILabel*)target)->getSelectedText().c_str());
}

bool CzUILabel::_setDisabledText(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUILabel* actor = (CzUILabel*)target;

	if (add)
	{
		CzString s = actor->getDisabledText();
		s += (const char*)prop.p_data;
		actor->setDisabledText(s.c_str());
	}
	else
		actor->setDisabledText((const char*)prop.p_data);

	return true;
}

CzXomlProperty CzUILabel::_getDisabledText(IzXomlResource* target)
{
	return CzXomlProperty(((CzUILabel*)target)->getDisabledText().c_str());
}

bool CzUILabel::_setTextColour(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUILabel* actor = (CzUILabel*)target;

	CzColour c;
	if (add)
	{
		c = actor->getTextColour();
		c.r += (uint8)prop.p_vec[0];
		c.g += (uint8)prop.p_vec[1];
		c.b += (uint8)prop.p_vec[2];
		c.a += (uint8)prop.p_vec[3];
	}
	else
	{
		c.r = (uint8)prop.p_vec[0];
		c.g = (uint8)prop.p_vec[1];
		c.b = (uint8)prop.p_vec[2];
		c.a = (uint8)prop.p_vec[3];
	}
	actor->setTextColour(c);

	return true;
}

CzXomlProperty CzUILabel::_getTextColour(IzXomlResource* target)
{
	return CzXomlProperty(((CzUILabel*)target)->getTextColour());
}

bool CzUILabel::_setSelectedTextColour(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUILabel* actor = (CzUILabel*)target;

	CzColour c;
	if (add)
	{
		c = actor->getSelectedTextColour();
		c.r += (uint8)prop.p_vec[0];
		c.g += (uint8)prop.p_vec[1];
		c.b += (uint8)prop.p_vec[2];
		c.a += (uint8)prop.p_vec[3];
	}
	else
	{
		c.r = (uint8)prop.p_vec[0];
		c.g = (uint8)prop.p_vec[1];
		c.b = (uint8)prop.p_vec[2];
		c.a = (uint8)prop.p_vec[3];
	}
	actor->setSelectedTextColour(c);

	return true;
}

CzXomlProperty CzUILabel::_getSelectedTextColour(IzXomlResource* target)
{
	return CzXomlProperty(((CzUILabel*)target)->getSelectedTextColour());
}

bool CzUILabel::_setDisabledTextColour(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUILabel* actor = (CzUILabel*)target;

	CzColour c;
	if (add)
	{
		c = actor->getDisabledTextColour();
		c.r += (uint8)prop.p_vec[0];
		c.g += (uint8)prop.p_vec[1];
		c.b += (uint8)prop.p_vec[2];
		c.a += (uint8)prop.p_vec[3];
	}
	else
	{
		c.r = (uint8)prop.p_vec[0];
		c.g = (uint8)prop.p_vec[1];
		c.b = (uint8)prop.p_vec[2];
		c.a = (uint8)prop.p_vec[3];
	}
	actor->setDisabledTextColour(c);

	return true;
}

CzXomlProperty CzUILabel::_getDisabledTextColour(IzXomlResource* target)
{
	return CzXomlProperty(((CzUILabel*)target)->getDisabledTextColour());
}

bool CzUILabel::_setAlignH(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUILabel* actor = (CzUILabel*)target;
	CzActorText* ta = actor->getTextActor();
	if (ta == NULL)
		return false;
	return CzActorText::_setAlignH(target, prop, add);
}

bool CzUILabel::_setAlignV(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUILabel* actor = (CzUILabel*)target;
	CzActorText* ta = actor->getTextActor();
	if (ta == NULL)
		return false;
	return CzActorText::_setAlignV(target, prop, add);
}

bool CzUILabel::_setWrap(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUILabel* actor = (CzUILabel*)target;
	CzActorText* ta = actor->getTextActor();
	if (ta == NULL)
		return false;
	return CzActorText::_setWrap(ta, prop, add);
}

bool CzUILabel::_setTextSkew(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUILabel* actor = (CzUILabel*)target;
	CzActorText* ta = actor->getTextActor();
	if (ta == NULL)
		return false;
	return CzActorText::_setSkew(ta, prop, add);
}

CzXomlProperty CzUILabel::_getTextSkew(IzXomlResource* target)
{
	CzUILabel* actor = (CzUILabel*)target;
	CzActorText* ta = actor->getTextActor();
	if (ta == NULL)
		return false;
	return CzXomlProperty(CzActorText::_getSkew(target));
}

bool CzUILabel::_setTextMargin(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUILabel* actor = (CzUILabel*)target;
	CzActorText* ta = actor->getTextActor();
	if (ta == NULL)
		return false;
	if (!CzActor::_setMargin(ta, prop, add))
		return false;
	ta->setMarginIsPadding(true);

	return true;
}

CzXomlProperty CzUILabel::_getTextMargin(IzXomlResource* target)
{
	CzUILabel* actor = (CzUILabel*)target;
	CzActorText* ta = actor->getTextActor();
	if (ta == NULL)
		return CzXomlProperty();

	return CzActorText::_getMargin(target);
}

bool CzUILabel::_setRect(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUILabel* actor = (CzUILabel*)target;
	CzActorText* ta = actor->getTextActor();
	if (ta == NULL)
		return false;

	CzActorText::_setRect(ta, prop, add);

	return true;
}

CzXomlProperty CzUILabel::_getRect(IzXomlResource* target)
{
	CzUILabel* actor = (CzUILabel*)target;
	CzActorText* ta = actor->getTextActor();
	if (ta == NULL)
		return CzXomlProperty();
	return CzActorText::_getRect(target);
}

bool CzUILabel::_setAutoHeight(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUILabel* actor = (CzUILabel*)target;
	CzActorText* ta = actor->getTextActor();
	if (ta == NULL)
		return false;
	if (!CzActorText::_setAutoHeight(target, prop, add))
		return false;
	actor->UpdateSizeFromText();
	return true;
}

CzXomlProperty CzUILabel::_getAutoHeight(IzXomlResource* target)
{
	CzUILabel* actor = (CzUILabel*)target;
	CzActorText* ta = actor->getTextActor();
	if (ta == NULL)
		return CzXomlProperty();
	return CzActorText::_getAutoHeight(target);
}

bool CzUILabel::_setTextFilter(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUILabel* actor = (CzUILabel*)target;
	CzActorText* ta = actor->getTextActor();
	return CzActorText::_setFilter(target, prop, add);
}

CzXomlProperty CzUILabel::_getTextFilter(IzXomlResource* target)
{
	CzUILabel* actor = (CzUILabel*)target;
	CzActorText* ta = actor->getTextActor();

	if (ta == NULL)
		return CzXomlProperty();

	if (ta->getVisual() == NULL)
		return CzXomlProperty();

	return CzActorText::_getFilter(target);
}

CzXomlProperty CzUILabel::_getActorText(IzXomlResource* target)
{
	return CzXomlProperty(((CzUILabel*)target)->getTextActor());
}

CzXomlProperty CzUILabel::_getTextSize(IzXomlResource* target)
{
	CzTextSprite* sprite = (CzTextSprite*)(((CzUILabel*)target)->getTextActor())->getVisual();
	CzFontPreparedText font_data = sprite->getPreparedData();
	int width = PLATFORM_FONT->GetTextWidth(font_data);
	int height = PLATFORM_FONT->GetTextHeight(font_data);

	return CzXomlProperty(CzVec2((float)width, (float)height));
}

//
//
//
//
//
// CzUILabel implementation
//
//
//
//
//
CzUILabel::CzUILabel() : CzUIBase(), TextActor(NULL)
{
	setActualClassType("label");
	TextColour.r = 0xff;
	TextColour.g = 0xff;
	TextColour.b = 0xff;
	TextColour.a = 0xff;
	SelectedTextColour.r = 0xff;
	SelectedTextColour.g = 0xff;
	SelectedTextColour.b = 0xff;
	SelectedTextColour.a = 0xff;
	DisabledTextColour.r = 0xff;
	DisabledTextColour.g = 0xff;
	DisabledTextColour.b = 0xff;
	DisabledTextColour.a = 0xff;
}

void CzUILabel::UpdateSizeFromText()
{
	if (!TextActor->isAutoHeight())
		return;

	SetDestSize(TextActor->getSize().x, TextActor->getSize().y);

	// Notify parent that it needs to resize
	NotifyParentsResize();
}

void CzUILabel::setTextColour(const CzColour& colour)
{
	TextColour = colour;
	if (!Selected && Enabled)
	{
		if (TextActor != NULL)
			TextActor->setColour(colour);
	}
}

void CzUILabel::setText(const char* text)
{
	if (text == NULL || CzString::CalculateHash(text) == Text.getHash())
		return;

	Text = text;
	if (!Selected && Enabled)
	{
		if (TextActor != NULL)
		{
			TextActor->setText(text);
			// Text was changed so update container size if need be
			UpdateSizeFromText();
		}
	}
}

void CzUILabel::setSelectedTextColour(const CzColour& colour)
{
	SelectedTextColour = colour;
	if (Selected && Enabled)
	{
		if (TextActor != NULL)
			TextActor->setColour(colour);
	}
}

void CzUILabel::setSelectedText(const char* text)
{
	if (text == NULL || CzString::CalculateHash(text) == SelectedText.getHash())
		return;

	SelectedText = text;
	if (Selected && Enabled)
	{
		if (TextActor != NULL)
			TextActor->setText(text);
	}
}

void CzUILabel::setDisabledTextColour(const CzColour& colour)
{
	DisabledTextColour = colour;
	if (!Enabled)
	{
		if (TextActor != NULL)
			TextActor->setColour(colour);
	}
}

void CzUILabel::setDisabledText(const char* text)
{
	if (text == NULL || CzString::CalculateHash(text) == DisabledText.getHash())
		return;

	DisabledText = text;
	if (!Enabled)
	{
		if (TextActor != NULL)
			TextActor->setText(text);
	}
}

void CzUILabel::setEnabled(bool enabled)
{
	CzUIBase::setEnabled(enabled);

	if (TextActor == NULL)
		return;

	if (!enabled)
	{
		if (!DisabledText.isEmpty())
			TextActor->setText(DisabledText.c_str());
		TextActor->setColour(DisabledTextColour);
	}
	else
	{
		if (Selected)
		{
			if (!SelectedText.isEmpty())
				TextActor->setText(SelectedText.c_str());
			TextActor->setColour(SelectedTextColour);
		}
		else
		{
			if (!Text.isEmpty())
				TextActor->setText(Text.c_str());
			TextActor->setColour(TextColour);
		}
	}
}

void CzUILabel::setIgnoreCamera(bool enable)
{
	IgnoreCamera = enable;
	TextActor->setIgnoreCamera(IgnoreCamera);
}

bool CzUILabel::InitTextActor(CzFont* font, const CzIRect& rect, const CzString& text, int flags, eCzAlignH align_h, eCzAlignV align_v, const CzVec4& skew, const CzIRect& text_margin, bool auto_height, bool text_use_parent_opacity, bool filter)
{
	TextActor = new CzActorText();
	Scene->addActor(TextActor);
	TextActor->setOrphan(Orphan);
	TextActor->setLinkedTo(this);

	if (text_margin.x != 0 && text_margin.y != 0 && text_margin.w != 0  && text_margin.h != 0)
		TextActor->setMarginIsPadding(true);
	TextActor->setAutoHeight(auto_height);
	TextActor->setMargin(text_margin.x, text_margin.y, text_margin.w, text_margin.h);
	TextActor->Init(font, rect, text, flags);
	TextActor->setAlignH(align_h);
	TextActor->setAlignV(align_v);
	TextActor->setColour(TextColour);
//	TextActor->setRect(rect);
	TextActor->setDepth(0);
	TextActor->setLayer(Layer);
	TextActor->setIgnoreCamera(IgnoreCamera);
	TextActor->setUseParentOpacity(text_use_parent_opacity);
	TextActor->getVisual()->setSkew(skew.x, skew.y, skew.z, skew.w);

	UpdateSizeFromText();

	return true;
}

void CzUILabel::SetFromBrush(IzBrush* brush, bool resize)
{
	if (brush->getBrushType() == IzBrush::BT_Image || brush->getBrushType() == IzBrush::BT_9Patch)
	{
		CzBrushImage* b = (CzBrushImage*)brush;
		if (Visual != NULL)
			((CzBitmapSprite*)Visual)->setFromBrush(b);
		else
			SetImage(b->getImage());

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
			// Only calculate size from original size if autho height is not set
			if (!(TextActor != NULL && TextActor->isAutoHeight()))
			{
				if (LinkedTo != NULL)
					Size.y = (LinkedTo->getSize().y * -OriginalSize.y) / 100;
				else
					Size.y = (PLATFORM_DISPLAY->getCurrentHeight() * -OriginalSize.y) / 100;
			}
		}
		SetDestSize(Size.x, Size.y);
	}
}


bool CzUILabel::InitTextActor(CzFont* font, const CzIRect& rect, const CzString& text, bool auto_height, bool text_use_parent_opacity, bool filter)
{
	CzIRect		text_margin = CzIRect(0, 0, 0, 0);
	CzVec4		skew = CzVec4(0, 0, 0, 0);

	return InitTextActor(font, rect, text, 0, AlignH_Centre, AlignV_Middle, skew, text_margin, auto_height, text_use_parent_opacity, filter);
}

bool CzUILabel::Update(float dt)
{
	bool font_loaded = TextActor->isFontLoaded();

	if (!CzUIBase::Update(dt))
		return false;

	if (!font_loaded && TextActor->isFontLoaded())
		NotifyParentsResize();
	else
	{
		if (TextActor != NULL)
		{
			if (TextActor->getSize().y != Size.y)
				UpdateSizeFromText();
		}
	}

	return true;
}

int CzUILabel::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Label - Label needs to be declared inside a scene or an actor - ", DebugInfo.c_str());
		return 0;
	}

	// Load main actor attributes
	int ret = CzUIBase::LoadFromXoml(parent, false, node);
	if (ret <= 0)
		return ret;

	// Process label specific attributes
	CzString		font_name;
	eCzAlignH		alignh = AlignH_Centre;
	eCzAlignV		alignv = AlignV_Middle;
	int				flags = 0;
	CzIRect			rect = CzIRect(0, 0, -100, -100);
	CzIRect			text_margin = CzIRect(0, 0, 0, 0);
	CzVec4			skew = CzVec4(0, 0, 0, 0);
	bool			text_colour_set = false;
	bool			sel_text_colour_set = false;
	bool			text_auto_height = false;
	bool			text_use_parent_opacity = false;
	bool			text_filter = false;
	CzString*		anchor = NULL;

	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Font_Hash)
			font_name = (*it)->getValue();
		else
		if (name_hash == CzHashes::Text_Hash)
			Text = (*it)->getValue();
		else
		if (name_hash == CzHashes::Anchor_Hash)
			anchor = &(*it)->getValue();
		else
		if (name_hash == CzHashes::SelectedText_Hash)
			SelectedText = (*it)->getValue();
		else
		if (name_hash == CzHashes::DisabledText_Hash)
			DisabledText = (*it)->getValue();
		else
		if (name_hash == CzHashes::TextFilter_Hash)
			text_filter = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::TextColour_Hash)
		{
			if (!(*it)->getValueAsColour(TextColour))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Label - Invalid value for Label::TextColour, expected colour - ", DebugInfo.c_str());
			else
				text_colour_set = true;
		}
		else
		if (name_hash == CzHashes::SelectedTextColour_Hash)
		{
			if (!(*it)->getValueAsColour(SelectedTextColour))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Label - Invalid value for Label::SelectedTextColour, expected colour - ", DebugInfo.c_str());
			else
				sel_text_colour_set = true;
		}
		else
		if (name_hash == CzHashes::DisabledTextColour_Hash)
		{
			if (!(*it)->getValueAsColour(DisabledTextColour))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Label - Invalid value for Label::DisabledTextColour, expected colour - ", DebugInfo.c_str());
		}
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
				alignv = AlignV_Bottom;
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
		if (name_hash == CzHashes::rect_Hash)
		{
			if (!(*it)->getValueAsRect(rect))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Label - Invalid value for Label::Rect, expected rect - ", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::TextSkew_Hash)
		{
			if (!(*it)->getValueAsPoint4(skew))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Label - Invalid value for Label::TextSkew, expected vec4 - ", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::TextMargin_Hash)
		{
			if (!(*it)->getValueAsRect(text_margin))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Label - Invalid value for Label::TextMargin, expected rect - ", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::AutoHeight_Hash)
			text_auto_height = (*it)->getValueAsBool();
		else
			if (name_hash == CzHashes::TextUseParentOpacity_Hash)
			text_use_parent_opacity = (*it)->getValueAsBool();
	}

	if (text_colour_set && !sel_text_colour_set)
		SelectedTextColour = TextColour;

	// if base bacjground nto supplied then set size to parents size
	if (LinkedTo != NULL)
	{
		if (Size.x == 0)
			Size.x = LinkedTo->getSize().x;
		if (Size.y == 0)
			Size.y = LinkedTo->getSize().y;
	}

	// Search the scenes resources for the specified font
	CzFont* font = (CzFont*)Scene->getResourceManager()->findResource(font_name.c_str(), CzHashes::Font_Hash);
	if (font == NULL)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Label - Could not find label font in the scene or in the global resources - ", font_name.c_str(), DebugInfo.c_str());

	// Initialise the text component actor
	if (rect.w == 0)
	{
		CzIRect rc = CzIRect(-Size.x / 2, -Size.y / 2, Size.x, Size.y);
		InitTextActor(font, rc, Text, flags, alignh, alignv, skew, text_margin, text_auto_height, text_use_parent_opacity, text_filter);
	}
	else
		InitTextActor(font, rect, Text, flags, alignh, alignv, skew, text_margin, text_auto_height, text_use_parent_opacity, text_filter);
	setTextColour(TextColour);

	if (anchor != NULL)
	{
		if (anchor->getHash() != CzHashes::centre_Hash)
			TextActor->getVisual()->setAnchor(CzSprite::TopLeft);
	}

	if (load_children)
	{
		// Process inner tags
		if (!CZ_XOML->Process(this, node))
			return 0;
	}
	return 1;
}

/**
 @fn	bool CzUILabel::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the actor.

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types. Note that as 
 CzUILabel is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param	property_name		Name of the property.
 @param [out]	prop	The property data that.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUILabel::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	if (CzUIBase::getProperty(property_name, prop))
		return true;

	int err = IzXomlResource::getProperty(LabelClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = LabelClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Label - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUILabel::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the UILabel.
    
 Sets the named property of this UILabel. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzUILabel::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	if (CzUIBase::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(LabelClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = LabelClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Label - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUILabel::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced. Note that as CzUILabel is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in]	data	The property data.
 @param	delta				true to property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
 
bool CzUILabel::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	if (CzUIBase::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(LabelClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = LabelClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Label - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUILabel::UpdateBinding(unsigned int property_name, CzXomlVariable* var)

 @brief	Updates an actor property from the supplied XOML variable.

 Updates a single property from the value of the bound XOML variable. Note that as CzUILabel is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param	property_name  	Name of the property.
 @param [in,out]	var	If non-null, the variable.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUILabel::UpdateBinding(unsigned int property_name, CzXomlVariable* var)
{
	if (!var->Modified || var->getValue().isEmpty())
		return false;

	if (CzUIBase::UpdateBinding(property_name, var))
		return true;

	int err = IzXomlResource::setProperty(LabelClassDef, this, property_name, var);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = LabelClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Label - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;}

/**
 @fn	bool CzUILabel::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates actor property from supplied animation.

 Updates the actors property that is specified by the supplied animation. When a timeline animation targets the property of an actor this method will be called by the animation to move the 
 animations frame data into its target property. Note that as CzUILabel is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param [in]	animation	If non-null, the animation.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUILabel::UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzUIBase::UpdateFromAnimation(animation))
		return true;

	int err = IzXomlResource::setProperty(LabelClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = LabelClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Label - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}


