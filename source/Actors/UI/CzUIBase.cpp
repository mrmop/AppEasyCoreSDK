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
#include "CzUIBase.h"
#include "CzUtil.h"
#include "CzString.h"
#include "CzApp.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"

CzList<CzAnim*>	CzUIBase::PresetAnimations;

CzXomlClassDef* CzUIBase::UIBaseClassDef = NULL;

/**
 @fn	void CzActorImage::InitClass()

 @brief	Initialises the class.
	
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzUIBase::InitClass()
{
	// Create class properties
	UIBaseClassDef = new CzXomlClassDef();
	UIBaseClassDef->addProperty(new CzXomlClassProperty("Background",			PT_Resource,	CzUIBase::_setBackground,				CzUIBase::_getBackground));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("SelectedBackground",	PT_Resource,	CzUIBase::_setSelectedBackground,		CzUIBase::_getSelectedBackground));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("DisabledBackground",	PT_Resource,	CzUIBase::_setDisabledBackground,		CzUIBase::_getDisabledBackground));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("BackgroundColour",		PT_Vec4,		CzUIBase::_setBackgroundColour,			CzUIBase::_getBackgroundColour));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("SelectedColour",		PT_Vec4,		CzUIBase::_setSelectedColour,			CzUIBase::_getSelectedColour));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("DisabledColour",		PT_Vec4,		CzUIBase::_setDisabledColour,			CzUIBase::_getDisabledColour));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("Enabled",				PT_Bool,		CzUIBase::_setEnabled,					CzUIBase::_getEnabled));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("Selected",				PT_Bool,		CzUIBase::_setSelected,					CzUIBase::_getSelected));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("ScrollPos",			PT_Vec2,		CzUIBase::_setScrollPos,				CzUIBase::_getScrollPos));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("SelectType",			PT_String,		CzUIBase::_setSelectType,				CzUIBase::_getSelectType));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("Spring",				PT_Bool,		CzUIBase::_setSpring,					CzUIBase::_getSpring));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("OnToggledOn",			PT_String,		CzUIBase::_setOnToggledOn,				NULL));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("OnToggledOff",			PT_String,		CzUIBase::_setOnToggledOff,				NULL));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("OnBackKey",			PT_String,		CzUIBase::_setOnBackKey,				NULL));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("OnMenuKey",			PT_String,		CzUIBase::_setOnMenuKey,				NULL));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("ClipMargin",			PT_Vec4,		CzUIBase::_setClipMargin,				CzUIBase::_getClipMargin));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("ScrollRange",			PT_Vec4,		CzUIBase::_setScrollRange,				CzUIBase::_getScrollRange));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("ShowTimeline",			PT_Resource,	CzUIBase::_setShowTimeline,				CzUIBase::_getShowTimeline));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("HideTimeline",			PT_Resource,	CzUIBase::_setHideTimeline,				CzUIBase::_getHideTimeline));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("SizeToContent",		PT_String,		CzUIBase::_setSizeToContentAxis,		CzUIBase::_getSizeToContentAxis));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("KeyFocus",				PT_Bool,		CzUIBase::_setKeyFocus,					CzUIBase::_getKeyFocus));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("ToggledOn",			PT_Bool,		CzUIBase::_setToggledOn,				CzUIBase::_getToggledOn));
	UIBaseClassDef->addProperty(new CzXomlClassProperty("ColourOffset",			PT_Vec4,		CzUIBase::_setColourOffset,				CzUIBase::_getColourOffset));

}
void CzUIBase::ReleaseClass()
{
	if (UIBaseClassDef != NULL)
	{
		UIBaseClassDef->Release();
		delete UIBaseClassDef;
		UIBaseClassDef = NULL;
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
bool CzUIBase::_setBackground(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;
	IzBrush* brush;
	if (prop.Type == PT_Resource)
		brush = (IzBrush*)prop.p_data;
	else
		brush = (IzBrush*)actor->getScene()->getResourceManager()->findResource((const char*)prop.p_data, CzHashes::Brush_Hash);
	if (brush != NULL)
		actor->setBackgroundBrush(brush);
	else
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - setProperty - Background brush could not be found in resources - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());

	return true;
}

CzXomlProperty CzUIBase::_getBackground(IzXomlResource *target)
{
	return CzXomlProperty(((CzUIBase*)target)->getBackgroundBrush());
}

bool CzUIBase::_setSelectedBackground(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;
	IzBrush* brush;
	if (prop.Type == PT_Resource)
		brush = (IzBrush*)prop.p_data;
	else
		brush = (IzBrush*)actor->getScene()->getResourceManager()->findResource((const char*)prop.p_data, CzHashes::Brush_Hash);
	if (brush != NULL)
		actor->setSelectedBackgroundBrush(brush);
	else
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - setProperty - Selected background brush could not be found in resources - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());

	return true;
}

CzXomlProperty CzUIBase::_getSelectedBackground(IzXomlResource *target)
{
	return CzXomlProperty(((CzUIBase*)target)->getSelectedBackgroundBrush());
}

bool CzUIBase::_setDisabledBackground(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;
	IzBrush* brush;
	if (prop.Type == PT_Resource)
		brush = (IzBrush*)prop.p_data;
	else
		brush = (IzBrush*)actor->getScene()->getResourceManager()->findResource((const char*)prop.p_data, CzHashes::Brush_Hash);
	if (brush != NULL)
		actor->setDisabledBackgroundBrush(brush);
	else
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - setProperty - Disabled background brush could not be found in resources - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());

	return true;
}

CzXomlProperty CzUIBase::_getDisabledBackground(IzXomlResource *target)
{
	return CzXomlProperty(((CzUIBase*)target)->getDisabledBackgroundBrush());
}

bool CzUIBase::_setBackgroundColour(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;
	CzColour c;
	
	if (add)
	{
		c = actor->getBackgroundColour();
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
	actor->setBackgroundColour(c);

	return true;
}

CzXomlProperty CzUIBase::_getBackgroundColour(IzXomlResource *target)
{
	return CzXomlProperty(((CzUIBase*)target)->getBackgroundColour());
}

bool CzUIBase::_setSelectedColour(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;
	CzColour c;
	
	if (add)
	{
		c = actor->getSelectedBackgroundColour();
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
	actor->setSelectedBackgroundColour(c);

	return true;
}

CzXomlProperty CzUIBase::_getSelectedColour(IzXomlResource *target)
{
	return CzXomlProperty(((CzUIBase*)target)->getSelectedBackgroundColour());
}

bool CzUIBase::_setDisabledColour(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;
	CzColour c;
	
	if (add)
	{
		c = actor->getDisabledBackgroundColour();
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
	actor->setDisabledBackgroundColour(c);

	return true;
}

CzXomlProperty CzUIBase::_getDisabledColour(IzXomlResource *target)
{
	return CzXomlProperty(((CzUIBase*)target)->getDisabledBackgroundColour());
}

bool CzUIBase::_setEnabled(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;
	if (add)
		actor->setEnabled(!actor->isEnabled());
	else
		actor->setEnabled(prop.p_bool);

	return true;
}

CzXomlProperty CzUIBase::_getEnabled(IzXomlResource *target)
{
	return CzXomlProperty(((CzUIBase*)target)->isEnabled());
}

bool CzUIBase::_setSelected(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;
	if (add)
		actor->setSelected(!actor->isSelected());
	else
		actor->setSelected(prop.p_bool);

	return true;
}

CzXomlProperty CzUIBase::_getSelected(IzXomlResource *target)
{
	return CzXomlProperty(((CzUIBase*)target)->isSelected());
}

bool CzUIBase::_setScrollPos(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;
	CzIRect rc;


	if (add)
	{
		CzIRect rc = actor->getScrollRange();
		rc.x += (int)prop.p_vec[0];
		rc.y += (int)prop.p_vec[1];
		rc.w += (int)prop.p_vec[2];
		rc.h += (int)prop.p_vec[3];
		actor->setScrollRange(rc.x, rc.y, rc.w, rc.h);
	}
	else
		actor->setScrollRange((int)prop.p_vec[0], (int)prop.p_vec[1], (int)prop.p_vec[2], (int)prop.p_vec[3]);

	return true;
}

CzXomlProperty CzUIBase::_getScrollPos(IzXomlResource *target)
{
	return CzXomlProperty(((CzUIBase*)target)->getScrollPos());
}

bool CzUIBase::_setSelectType(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;

	unsigned int type_hash = CzString::CalculateHash((const char*)prop.p_data);

	if (type_hash == CzHashes::Toggle_Hash)
		actor->setSelectType(SelectType_Toggle);
	else
	if (type_hash == CzHashes::Sticky_Hash)
		actor->setSelectType(SelectType_Sticky);
	else
	if (type_hash == CzHashes::Normal_Hash)
		actor->setSelectType(SelectType_Normal);
	else
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - setProperty - Invalid value for SelectType expected toggle, sticky or normal - ", (const char*)prop.p_bool, actor->getDebugInfo().c_str());

	return true;
}

CzXomlProperty CzUIBase::_getSelectType(IzXomlResource *target)
{
	return CzXomlProperty(CzXoml::SelectTypeNames[((CzUIBase*)target)->getSelectType()]);
}

bool CzUIBase::_setSpring(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;

	if (add)
		actor->setEnableSpring(!actor->getEnableSpring());
	else
		actor->setEnableSpring(prop.p_bool);

	return true;
}

CzXomlProperty CzUIBase::_getSpring(IzXomlResource *target)
{
	return CzXomlProperty(((CzUIBase*)target)->getEnableSpring());
}

bool CzUIBase::_setOnToggledOn(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;

	actor->getEventsManager()->addEvent("OnToggledOn", (const char*)prop.p_data, true);

	return true;
}

bool CzUIBase::_setOnToggledOff(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;

	actor->getEventsManager()->addEvent("OnToggledOff", (const char*)prop.p_data, true);

	return true;
}

bool CzUIBase::_setOnBackKey(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;

	actor->getEventsManager()->addEvent("OnBackKey", (const char*)prop.p_data, true);

	return true;
}

bool CzUIBase::_setOnMenuKey(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;

	actor->getEventsManager()->addEvent("OnMenuKey", (const char*)prop.p_data, true);

	return true;
}

bool CzUIBase::_setClipMargin(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;

	if (add)
	{
		CzIRect rc = actor->getClippingMargin();
		rc.x += (int)prop.p_vec[0];
		rc.y += (int)prop.p_vec[1];
		rc.w += (int)prop.p_vec[2];
		rc.h += (int)prop.p_vec[3];
		actor->setClippingMargin(rc.x, rc.y, rc.w, rc.h);
	}
	else
		actor->setClippingMargin((int)prop.p_vec[0], (int)prop.p_vec[1], (int)prop.p_vec[2], (int)prop.p_vec[3]);

	return true;
}

CzXomlProperty CzUIBase::_getClipMargin(IzXomlResource *target)
{
	return CzXomlProperty(((CzUIBase*)target)->getClippingMargin());
}

bool CzUIBase::_setScrollRange(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;

	if (add)
	{
		CzIRect rc = actor->getScrollRange();
		rc.x += (int)prop.p_vec[0];
		rc.y += (int)prop.p_vec[1];
		rc.w += (int)prop.p_vec[2];
		rc.h += (int)prop.p_vec[3];
		actor->setScrollRange(rc.x, rc.y, rc.w, rc.h);
	}
	else
		actor->setScrollRange((int)prop.p_vec[0], (int)prop.p_vec[1], (int)prop.p_vec[2], (int)prop.p_vec[3]);

	return true;
}

CzXomlProperty CzUIBase::_getScrollRange(IzXomlResource *target)
{
	return CzXomlProperty(((CzUIBase*)target)->getScrollRange());
}

bool CzUIBase::_setShowTimeline(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;

	if (prop.Type == PT_Resource)
		actor->setShowTimeline((CzAnimTimeline*)prop.p_data);
	else
		actor->setShowTimeline((const char*)prop.p_data);

	return true;
}

CzXomlProperty CzUIBase::_getShowTimeline(IzXomlResource *target)
{
	return CzXomlProperty(((CzUIBase*)target)->getShowTimeline());
}

bool CzUIBase::_setHideTimeline(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;

	if (prop.Type == PT_Resource)
		actor->setHideTimeline((CzAnimTimeline*)prop.p_data);
	else
		actor->setHideTimeline((const char*)prop.p_data);

	return true;
}

CzXomlProperty CzUIBase::_getHideTimeline(IzXomlResource *target)
{
	return CzXomlProperty(((CzUIBase*)target)->getHideTimeline());
}

bool CzUIBase::_setSizeToContentAxis(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;

	unsigned int size_type = CzString::CalculateHash((const char*)prop.p_data);
	if (size_type == CZ_HASH("x"))
		actor->setSizeToContentAxis(Axis_X);
	else
	if (size_type == CZ_HASH("y"))
		actor->setSizeToContentAxis(Axis_Y);
	else
	if (size_type == CZ_HASH("XY"))
		actor->setSizeToContentAxis(Axis_XY);
	else
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - setProperty - Invalid value for SizeToContentAxis expected none, x, y or xy - ", (const char*)prop.p_bool, actor->getDebugInfo().c_str());

	return true;
}

CzXomlProperty CzUIBase::_getSizeToContentAxis(IzXomlResource *target)
{
	return CzXomlProperty(CzXoml::AxisNames[((CzUIBase*)target)->getSizeToContentAxis()]);
}

bool CzUIBase::_setKeyFocus(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;
	actor->getScene()->getParent()->setKeyFocus(actor);
	return true;
}

CzXomlProperty CzUIBase::_getKeyFocus(IzXomlResource *target)
{
	CzUIBase* actor = (CzUIBase*)target;
	return CzXomlProperty(actor->getScene()->getParent()->getKeyFocus() == actor);
}

bool CzUIBase::_setToggledOn(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;

	if (add)
		actor->setToggledOn(!actor->isToggledOn());
	else
		actor->setToggledOn(prop.p_bool);

	return true;
}

CzXomlProperty CzUIBase::_getToggledOn(IzXomlResource *target)
{
	return CzXomlProperty(((CzUIBase*)target)->isToggledOn());
}

bool CzUIBase::_setColourOffset(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIBase* actor = (CzUIBase*)target;
	
	if (add)
	{
		CzIRect c = actor->getColourOffset();
		c.x += (uint8)prop.p_vec[0];
		c.y += (uint8)prop.p_vec[1];
		c.w += (uint8)prop.p_vec[2];
		c.h += (uint8)prop.p_vec[3];
		actor->setColourOffset(c);
	}
	else
	{
		CzIRect c;
		c.x = (uint8)prop.p_vec[0];
		c.y = (uint8)prop.p_vec[1];
		c.w = (uint8)prop.p_vec[2];
		c.h = (uint8)prop.p_vec[3];
		actor->setColourOffset(c);
	}

	return true;
}

CzXomlProperty CzUIBase::_getColourOffset(IzXomlResource *target)
{
	return CzXomlProperty(((CzUIBase*)target)->getColourOffset());
}



//
//
//
//
// CzUIBase implementation
//
//
//
//
CzUIBase::CzUIBase() : CzActorImage(), BackgroundBrush(NULL), SelectedBackgroundBrush(NULL), DisabledBackgroundBrush(NULL), Enabled(true), Selected(false), SelectType(SelectType_Normal), 
	LayoutDirty(true), EnableSpring(true), ToggledOn(false), Tapped(false), ShowTimeline(NULL), HideTimeline(NULL), SizeToContentAxis(Axis_None), NeedsResize(false)
{
	setActualClassType("uibase");
	Category = AC_UI;
	BackgroundColour.r = 0xff;
	BackgroundColour.g = 0xff;
	BackgroundColour.b = 0xff;
	BackgroundColour.a = 0xff;
	SelectedBackgroundColour.r = 0xff;
	SelectedBackgroundColour.g = 0xff;
	SelectedBackgroundColour.b = 0xff;
	SelectedBackgroundColour.a = 0xff;
	DisabledBackgroundColour.r = 0x7f;
	DisabledBackgroundColour.g = 0x7f;
	DisabledBackgroundColour.b = 0x7f;
	DisabledBackgroundColour.a = 0x7f;
	ClippingMargin.x = 0;
	ClippingMargin.y = 0;
	ClippingMargin.w = 0;
	ClippingMargin.h = 0;
	OriginalClippingMargin.x = 0;
	OriginalClippingMargin.y = 0;
	OriginalClippingMargin.w = 0;
	OriginalClippingMargin.h = 0;
	PreviousScrollPos.x = 0;
	PreviousScrollPos.y = 0;
	ScrollPos.x = 0;
	ScrollPos.y = 0;
	ScrollVelocity.x = 0;
	ScrollVelocity.y = 0;
	ScrollRange.x = 0;
	ScrollRange.y = 0;
	ScrollRange.w = 0;
	ScrollRange.h = 0;
	InitialScrollPos.x = -999999;
	InitialScrollPos.y = -999999;
	ColourOffset.x = 0;
	ColourOffset.y = 0;
	ColourOffset.w = 0;
	ColourOffset.h = 0;
}

CzUIBase::~CzUIBase()
{
	if (ShowTimeline != NULL)
	{
		if (!ShowTimeline->isManaged())
			delete ShowTimeline;
	}
	if (HideTimeline != NULL)
	{
		if (!HideTimeline->isManaged())
			delete HideTimeline;
	}
}

/**
 @fn	void CzUIBase::ResizeToContent()

 @brief	Resizes the actor to fit its content.

 Recalculates docking and dirties the layout. Used mainly by layout actors to resize themselves to fit their new content. This method is usually overridden by a 
 derived class in order to calculate the new size based on the containers children. See CzUIStackPanel::ResizeToContent() for a simple example.

 */

void CzUIBase::ResizeToContent()
{
	NeedsResize = false;
	if (SizeToContentAxis)
	{
		// Set docking
		if (Docking != CzScene::Dock_None)
			setDocking(Docking);
		setLayoutDirty(true);
	}
}

/**
 @fn	void CzUIBase::setVisible(bool visible, bool force_set)

 @brief	Makes an actor visible.

 Actors can be made visible / invisible by setting their visibility. An invisible actor will not be displayed but will still be processed if it is active.

 CzUIBase augments the original CzActor::setVisible() method by adding support for starting and stopping show and hide timelines. Show and hide timelines are timelines that 
 can if assigned to an actor will automatically call those timelines each time the actor is made visible or hidden. Any UI actor can be assigned show and hide timelines, for 
 example:

 @par XOML Example:
 @code
<Label Font="serif" Background="Button1Brush" Text="Im a label" ShowTimeline="show_timeline" HideTimeline="hide_timeline" />
 @endcode

 @param	visible  	true to show, false to hide.
 @param	force_set	unused at this time
 */

void CzUIBase::setVisible(bool visible, bool force_set)
{
	if (!force_set)
	{
		if (!IsVisible && visible)
		{
			if (ShowTimeline != NULL)
			{
				if (HideTimeline != NULL)
					HideTimeline->stop();
				ShowTimeline->restart();
				ShowTimeline->Update(0.1f);
			}
		}
		if (IsVisible && !visible)
		{
			if (HideTimeline != NULL)
			{
				if (ShowTimeline != NULL)
					ShowTimeline->stop();
				HideTimeline->restart();
				HideTimeline->Update(0.1f);
				return;
			}
		}
	}

	CzActor::setVisible(visible);
}

/**
 @fn	void CzUIBase::setShowTimeline(CzAnimTimeline* timeline)

 @brief	Sets show timeline deleting any previously assigned none managed show timeline.

 @param [in]	timeline	If non-null, the timeline.
 */

void CzUIBase::setShowTimeline(CzAnimTimeline* timeline)
{
	if (ShowTimeline != NULL)
	{
		if (!ShowTimeline->isManaged())
			delete ShowTimeline;
	}
	ShowTimeline = timeline;
}

/**
 @fn	void CzUIBase::setShowTimeline(CzAnimTimeline* timeline)

 @brief	Sets show timeline to the named timeline.

 Note that the timeline name can also include one of the preset timelines that are defined by PresetAnimations

 @param [in]	timeline	If non-null, the timeline.
 */
 
void CzUIBase::setShowTimeline(const char* timeline_name)
{
	CzAnimTimeline* timeline = NULL;
	unsigned int timeline_hash = CZ_HASH(timeline_name);

	// Check preeet animations first
	CzAnim* anim = FindPresetAnimation(timeline_hash);
	if (anim != NULL)
	{
		if (anim->getCategory() == 0)
			timeline = CzAnimUtil::CreateTimeline(timeline_name, anim, this, "PositionOrgX", 1, true);
		else
		if (anim->getCategory() == 1)
			timeline = CzAnimUtil::CreateTimeline(timeline_name, anim, this, "PositionOrgY", 1, true);
		else
		if (anim->getCategory() == 2)
			timeline = CzAnimUtil::CreateTimeline(timeline_name, anim, this, "Opacity", 1, false);
	}

	// Now check scene and global reources
	if (timeline == NULL)
		timeline = CzAnimTimelinesManager::FindTimeline(timeline_hash, this);

	if (timeline == NULL)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - Could not find named show timeline in the presets, scene or in the global resources - ", timeline_name, DebugInfo.c_str());
	else
	{
		setShowTimeline(timeline);
		timeline->setTargetElement(this);
	}
}

/**
 @fn	void CzUIBase::setHideTimeline(CzAnimTimeline* timeline)

 @brief	Sets hide timeline deleting any previously assigned none managed hide timeline.

 @param [in]	timeline	If non-null, the timeline.
 */

void CzUIBase::setHideTimeline(CzAnimTimeline* timeline)
{
	if (HideTimeline != NULL)
	{
		if (!HideTimeline->isManaged())
			delete HideTimeline;
	}
	HideTimeline = timeline;
}

/**
 @fn	void CzUIBase::setHideTimeline(const char* timeline_name)

 @brief	Sets hide timeline to the named timeline.

 Note that the timeline name can also include one of the preset timelines that are defined by PresetAnimations

 @param	timeline_name	Name of the timeline.
 */

void CzUIBase::setHideTimeline(const char* timeline_name)
{
	CzAnimTimeline* timeline = NULL;
	unsigned int timeline_hash = CZ_HASH(timeline_name);

	// Check preeet animations first
	CzAnim* anim = FindPresetAnimation(timeline_hash);
	if (anim != NULL)
	{
		if (anim->getCategory() == 0)
			timeline = CzAnimUtil::CreateTimeline(timeline_name, anim, this, "PositionOrgX", 1, true);
		else
		if (anim->getCategory() == 1)
			timeline = CzAnimUtil::CreateTimeline(timeline_name, anim, this, "PositionOrgY", 1, true);
		else
		if (anim->getCategory() == 2)
			timeline = CzAnimUtil::CreateTimeline(timeline_name, anim, this, "Opacity", 1, false);
	}

	// Now check scene and global reources
	if (timeline == NULL)
		timeline = CzAnimTimelinesManager::FindTimeline(timeline_hash, this);

	if (timeline == NULL)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - Could not find named hide timeline in the presets, scene or in the global resources - ", timeline_name, DebugInfo.c_str());
	else
	{
		setHideTimeline(timeline);
		timeline->setTargetElement(this);
	}
}

void CzUIBase::setNeedsResize(bool resize)
{
	NeedsResize = resize;
}


/**
 @fn	void CzUIBase::setBackgroundBrush(IzBrush* brush)

 @brief	Sets background brush of the actor.

 The background brush is the normal background that will be displayed when the actor is not selected or disabled.

 @param [in]	brush	If non-null, the brush.
 */

void CzUIBase::setBackgroundBrush(IzBrush* brush)
{
	if (BackgroundBrush == brush)
		return;
	if (brush == NULL)
	{
		BackgroundBrush = NULL;
		if (!Selected && Enabled)
			SetImage(NULL);
		return;
	}

	BackgroundBrush = brush;
	if (!Selected && Enabled)
		SetFromBrush(brush);
}

/**
 @fn	void CzUIBase::setBackgroundColour(const CzColour& colour)

 @brief	Sets background colour of the actor.

 The background colour is the normal background colour that will be displayed when the actor is not selected or disabled. Note that the background colour will be used 
 to tint the background brush.

 @param	colour	The colour.
 */

void CzUIBase::setBackgroundColour(const CzColour& colour)
{
	BackgroundColour = colour;
	if (!Selected && Enabled)
		setColour(colour.r, colour.g, colour.b, colour.a); 
}

/**
 @fn	void CzUIBase::setSelectedBackgroundBrush(IzBrush* brush)

 @brief	Sets selected background brush of the actor.

 The selected background brush is the background that will be displayed when the actor is selected.

 @param [in]	brush	If non-null, the brush.
 */

void CzUIBase::setSelectedBackgroundBrush(IzBrush* brush)
{
	if (SelectedBackgroundBrush == brush)
		return;
	if (brush == NULL)
	{
		SelectedBackgroundBrush = NULL;
		if (Selected && Enabled)
			SetImage(NULL);
		return;
	}

	SelectedBackgroundBrush = brush;
	if (Selected && Enabled)
		SetFromBrush(brush);
}

/**
 @fn	void CzUIBase::setSelectedBackgroundColour(const CzColour& colour)

 @brief	Sets selected background colour of the actor.

 The selected background colour is the background colour that will be displayed when the actor is selected. Note that the background colour will be used to tint 
 the selected background brush.

 @param	colour	The colour.
 */

void CzUIBase::setSelectedBackgroundColour(const CzColour& colour)
{
	SelectedBackgroundColour = colour;
	if (Selected && Enabled)
		setColour(colour.r, colour.g, colour.b, colour.a); 
}

/**
 @fn	void CzUIBase::setDisabledBackgroundBrush(IzBrush* brush)

 @brief	Sets disabled background brush of the actor.

 The disabled background brush is the background that will be displayed when the actor is disabled.

 @param [in]	brush	If non-null, the brush.
 */

void CzUIBase::setDisabledBackgroundBrush(IzBrush* brush)
{
	if (DisabledBackgroundBrush == brush)
		return;
	if (brush == NULL)
	{
		DisabledBackgroundBrush = NULL;
		if (!Enabled)
			SetImage(NULL);
		return;
	}

	DisabledBackgroundBrush = brush;
	if (!Enabled)
		SetFromBrush(brush);
}

/**
 @fn	void CzUIBase::setDisabledBackgroundColour(const CzColour& colour)

 @brief	Sets disabled background colour of the actor.

 The disabled background colour is the background colour that will be displayed when the actor is disabled. Note that the background colour will be used to tint 
 the disabled background brush.

 @param	colour	The colour.
 */

void CzUIBase::setDisabledBackgroundColour(const CzColour& colour)
{
	DisabledBackgroundColour = colour;
	if (!Enabled)
		setColour(colour.r, colour.g, colour.b, colour.a); 
}

/**
 @fn	void CzUIBase::setEnabled(bool enabled)

 @brief	Enables or disables the actor

 When the actor is in a disabled state it cannot be selected and it will not receive touch input.

 setEnabled() is also used to set the correct current brush and colour by calling with the currently Enabled state.

 @param	enabled	true to enable, false to disable.
 */

void CzUIBase::setEnabled(bool enabled)
{
	if (!enabled)
	{
		if (DisabledBackgroundBrush != NULL)
			SetFromBrush(DisabledBackgroundBrush);
		if (ColourOffset.x != 0 || ColourOffset.y != 0 || ColourOffset.w != 0 || ColourOffset.h != 0)
		{
			CzIRect c;
			c.x = DisabledBackgroundColour.r + ColourOffset.x;
			c.y = DisabledBackgroundColour.g + ColourOffset.y;
			c.w = DisabledBackgroundColour.b + ColourOffset.w;
			c.h = DisabledBackgroundColour.a + ColourOffset.h;
			if (c.x < 0) c.x = 0; else if (c.x > 255) c.x = 255;
			if (c.y < 0) c.y = 0; else if (c.y > 255) c.y = 255;
			if (c.w < 0) c.w = 0; else if (c.w > 255) c.w = 255;
			if (c.h < 0) c.h = 0; else if (c.h > 255) c.h = 255;
			setColour((uint8)c.x, (uint8)c.y, (uint8)c.w, (uint8)c.h);
		}
		else
			setColour(DisabledBackgroundColour);
	}
	else
	{
		if (Selected)
		{
			if (SelectedBackgroundBrush != NULL)
				SetFromBrush(SelectedBackgroundBrush);
			if (ColourOffset.x != 0 || ColourOffset.y != 0 || ColourOffset.w != 0 || ColourOffset.h != 0)
			{
				CzIRect c;
				c.x = SelectedBackgroundColour.r + ColourOffset.x;
				c.y = SelectedBackgroundColour.g + ColourOffset.y;
				c.w = SelectedBackgroundColour.b + ColourOffset.w;
				c.h = SelectedBackgroundColour.a + ColourOffset.h;
				if (c.x < 0) c.x = 0; else if (c.x > 255) c.x = 255;
				if (c.y < 0) c.y = 0; else if (c.y > 255) c.y = 255;
				if (c.w < 0) c.w = 0; else if (c.w > 255) c.w = 255;
				if (c.h < 0) c.h = 0; else if (c.h > 255) c.h = 255;
				setColour((uint8)c.x, (uint8)c.y, (uint8)c.w, (uint8)c.h);
			}
			else
				setColour(SelectedBackgroundColour);
		}
		else
		{
			if (BackgroundBrush != NULL)
				SetFromBrush(BackgroundBrush);
			if (ColourOffset.x != 0 || ColourOffset.y != 0 || ColourOffset.w != 0 || ColourOffset.h != 0)
			{
				CzIRect c;
				c.x = BackgroundColour.r + ColourOffset.x;
				c.y = BackgroundColour.g + ColourOffset.y;
				c.w = BackgroundColour.b + ColourOffset.w;
				c.h = BackgroundColour.a + ColourOffset.h;
				if (c.x < 0) c.x = 0; else if (c.x > 255) c.x = 255;
				if (c.y < 0) c.y = 0; else if (c.y > 255) c.y = 255;
				if (c.w < 0) c.w = 0; else if (c.w > 255) c.w = 255;
				if (c.h < 0) c.h = 0; else if (c.h > 255) c.h = 255;
				setColour((uint8)c.x, (uint8)c.y, (uint8)c.w, (uint8)c.h);
			}
			else
				setColour(BackgroundColour);
		}
	}

	Enabled = enabled;
}

/**
 @fn	void CzUIBase::setScrollPos(float x, float y)

 @brief	Sets scroll position.

 Sets the scroll position of the content within the actor. UI actors can scroll their content around, for example, when a grids data is larger than the available space 
 assigned to the grid then scrolling can be used to enable the user to pan / scroll around the content. 

 @param	x	The x coordinate.
 @param	y	The y coordinate.
 */

void CzUIBase::setScrollPos(float x, float y)
{
	ScrollPos.x = x; ScrollPos.y = y;
	ScrollRangeCheck();
}

/**
 @fn	bool CzUIBase::ScrollRangeCheck()

 @brief	Prevents scroll position from giong out of bounds.

 Checks to see if scroll position has gone out of the scroll range. If so then it is snapped back into place. If spring is enabled then the scroller will spring back into 
 place instead of snapping back into place.

 @return	true if the scroll position was changed, false if not.
 */

bool CzUIBase::ScrollRangeCheck()
{
	bool changed = false;

	if (ScrollRange.w != 0)
	{
		if (ScrollPos.x < (float)ScrollRange.x)
		{
			if (EnableSpring)
				ScrollPos.x += ((float)ScrollRange.x - ScrollPos.x) * 0.5f;
			else
			{
				ScrollPos.x = (float)ScrollRange.x;
				ScrollVelocity.x = 0;
			}
			changed = true;
		}
		else
		if (ScrollPos.x > (float)ScrollRange.x + ScrollRange.w)
		{
			if (EnableSpring)
				ScrollPos.x += ((float)(ScrollRange.x + ScrollRange.w) - ScrollPos.x) * 0.5f;
			else
			{
				ScrollPos.x = (float)ScrollRange.x + ScrollRange.w;
				ScrollVelocity.x = 0;
			}
			changed = true;
		}
	}
	else
		ScrollPos.x = (float)ScrollRange.x;

	if (ScrollRange.h != 0)
	{
		if (ScrollPos.y < (float)ScrollRange.y)
		{
			if (EnableSpring)
				ScrollPos.y += ((float)ScrollRange.y - ScrollPos.y) * 0.5f;
			else
			{
				ScrollPos.y = (float)ScrollRange.y;
				ScrollVelocity.y = 0;
			}
			changed = true;
		}
		else
		if (ScrollPos.y > (float)ScrollRange.y + ScrollRange.h)
		{
			if (EnableSpring)
				ScrollPos.y += ((float)(ScrollRange.y + ScrollRange.h) - ScrollPos.y) * 0.5f;
			else
			{
				ScrollPos.y = (float)ScrollRange.y + ScrollRange.h;
				ScrollVelocity.y = 0;
			}
			changed = true;
		}
	}
	else
		ScrollPos.y = (float)ScrollRange.y;

	return changed;
}

/**
 @fn	bool CzUIBase::Init(IzBrush* brush, int width, int height)

 @brief	Initialises this UI actor.

 Carries out some basic UI actor initialisation. Also assigns a brush and render size to the actor.

 @param [in]	brush	If non-null, the brush.
 @param	width			 	The width of the UI actor.
 @param	height			 	The height of the UI actor.

 @return	true if it succeeds, false if it fails.
 */

bool CzUIBase::Init(IzBrush* brush, int width, int height)
{
	if (!CzActorImage::Init(brush, width, height))
		return false;
	BackgroundBrush = brush;

	return true;
}

/**
 @fn	void CzUIBase::setLayoutDirty(bool dirty)

 @brief	Sets layout as dirty.

 Settinge the layout dirty focres contained actors to recalculate their layouts

 @param	dirty	true to dirty.
 */

void CzUIBase::setLayoutDirty(bool dirty)
{
	LayoutDirty = dirty;
	if (LinkedTo != NULL && LinkedTo->getCategory() == CzActor::AC_UI)
		((CzUIBase*)LinkedTo)->setLayoutDirty(true);
}

/**
 @fn	bool CzUIBase::Update(float dt)

@brief	Updates this actors logic

 Updates the UI actors logic, including:
 - Processing event actions
 - Processing any attached modifiers
 - Updating dragging state
 - Updating physics
 - Updating the attached timeline
 - Updating any show / hide timelines
 - Updating the scroller
 - Checking for an updating layout if dirty

 This method should be overriden by derived actors to provide the UI actor specific logic. 

 Note that dt is used to scale actor internals that rely on time. This value enables the actor run in an environment where time (the frame rate of the device) can vary allowing 
 animation to keep up without slowing down or speeding up when the frame rate varies. 

 Note that if an actor returns false then it will automatically be removed from the scene.

 @param	dt	The number of seconds since the app was last updated

 @return	true if it succeeds, false if it fails.
 */

bool CzUIBase::Update(float dt)
{
	if (!IsVisible)
		return CzActor::Update(dt);

	if (Visual != NULL)
	{
		if (HideTimeline != NULL)
		{
			bool is_playing = HideTimeline->isPlaying();
			if (is_playing)
				HideTimeline->Update(dt);
			if (is_playing && !HideTimeline->isPlaying())
				CzActor::setVisible(false);
		}
		if (ShowTimeline != NULL)
			ShowTimeline->Update(dt);
	}

	// Handle touch scrolling
	PreviousScrollPos = ScrollPos;
	if (ScrollRange.w != 0 || ScrollRange.h != 0)
	{
		if (!IsDraggable)
		{
			if (IsDragging)
			{
				if (InitialScrollPos.x < -10000)
					InitialScrollPos = ScrollPos;
				CzIVec2 drag_delta  = CZ_INPUT->getDragDelta(TouchIndex);
				if (drag_delta.x != 0 || drag_delta.y != 0)
				{
					CzMatrix3& transform = Visual->getFinalTransform();
					CzVec2 drag = CzVec2(((float)drag_delta.x) / transform.m[0][0], ((float)drag_delta.y) / transform.m[1][1]);
					if (ScrollRange.w != 0)
					{
						if (drag.x >= -1 && drag.x <= 1)
							drag.x = 0;
						ScrollVelocity.x = drag.x;
						if (drag.x != 0)
						{
							ScrollPos.x -= (float)drag.x;
							ScrollVelocity.x = (float)drag.x / dt;
							setLayoutDirty(true);
						}
					}
					if (ScrollRange.h != 0)
					{
						if (drag.y >= -1 && drag.y <= 1)
							drag.y = 0;
						ScrollVelocity.y = drag.y;
						if (drag.y != 0)
						{
							ScrollPos.y -= (float)drag.y;
							ScrollVelocity.y = (float)drag.y / dt;
							setLayoutDirty(true);
						}
					}

					// If we move scroll too much then release focus
					CzVec2 pos_diff = InitialScrollPos - ScrollPos;
					if (pos_diff.MagSquared() > 32)
					{
						CzActor* focus = Scene->getParent()->getTouchFocus(TouchIndex);
						if (focus != this && focus != NULL && !focus->getHoldFocus())
						{
							Scene->getParent()->releaseTouchFocus(TouchIndex, 999999, 999999, false);	// Use stupidly large values so they are invalid
							Scene->getParent()->setTouchFocus(TouchIndex, this);
						}
					}
				}
			}
			else
				InitialScrollPos.x = -999999;
		}

		if (!IsDragging && (ScrollVelocity.x != 0 || ScrollVelocity.y != 0))
		{
			ScrollPos -= ScrollVelocity * dt;
			setLayoutDirty(true);
		}

		if (EnableSpring)
		{
			if (!IsDragging)
			{
				if (ScrollRangeCheck())
					setLayoutDirty(true);
			}
		}
		else
		{
			if (ScrollRangeCheck())
				setLayoutDirty(true);
		}

		ScrollVelocity *= 0.9f;
		if (ScrollVelocity.x > -0.5f && ScrollVelocity.x < 0.5f)
			ScrollVelocity.x = 0;
		if (ScrollVelocity.y > -0.5f && ScrollVelocity.y < 0.5f)
			ScrollVelocity.y = 0;
	}

	if (LayoutDirty)
		UpdateLayout();

	if (!CzActor::Update(dt))
		return false;

	return true;
}

/**
 @fn	bool CzUIBase::UpdateLayout()

 @brief	Updates the actors layout.

 Recalculates the actors layout. Used mainly by container actors to modify the layout of their child content. This method is usually overridden by a 
 derived class in order to update the layout of its children.. See CzUIStackPanel::UpdateLayout() for a simple example.

 @return	true if it succeeds, false if it fails.
 */

bool CzUIBase::UpdateLayout()
{
	LayoutDirty = false;

/*	if (SizeToContentAxis != Axis_None && LinkedTo != NULL)
	{
		((CzUIBase*)LinkedTo)->setLayoutDirty(true);
	}*/

//	ResizeToContent();

	return true;
}


/**
 @fn	int CzUIBase::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)

 @brief	Creates an instance of this class from XML

 LoadFromXoml is the main method used by all classes that can be instantiated from XOML mark-up. This method creates an instance of this class from the supplied XML node structure specified by node. 
 if parent is not NULL then the created instance will utilise the supplied parent to determine where it should live within the app. For example, if the parent is a scene then the actor will be placed 
 in that scene. if the parent is another actor then this actor will be placed into the scene of the supplied actor and linked as a child to the parent actor. If load_chlldren is true then any child 
 nodes contained within the actor will also be loaded. Note that as CzUIBase is derived from CzActorImage, CzActorImage::LoadFromXoml() will also be called to load CzActor specific properties. 

 @param [in]	parent		If non-null, the parent.
 @param	load_children	  	true to load child nodes.
 @param [in]	node  		If non-null, the XOML markup node that defines this object

 @return	negative value for error. if 0 then processing of further XOML will stop.
 */

int CzUIBase::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "UIBase - uibase needs to be declared inside a scene or an actor - ", DebugInfo.c_str());
		return 0;
	}

	// Load main actor attributes
	int ret = CzActorImage::LoadFromXoml(parent, false, node);
	if (ret <= 0)
		return ret;

	// Process attributes
	CzString*	brush_name = NULL;
	CzString*	selected_brush_name = NULL;
	CzString*	disabled_brush_name = NULL;
	CzString*	show_timeline = NULL;
	CzString*	hide_timeline = NULL;
	bool		enabled = true;
	bool		selected = false;
	bool		key_focus = false;
	CzVec2		com(0, 0);
	bool		sensor = false;
	CzString*	box2dmaterial_name = NULL;

	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Brush_Hash || name_hash == CzHashes::Background_Hash)
			brush_name = &(*it)->getValue();
		else
		if (name_hash == CzHashes::SelectedBackground_Hash)
			selected_brush_name = &(*it)->getValue();
		else
		if (name_hash == CzHashes::DisabledBackground_Hash)
			disabled_brush_name = &(*it)->getValue();
		else
		if (name_hash == CzHashes::BackgroundColour_Hash)
		{
			if (!(*it)->getValueAsColour(BackgroundColour))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - Invalid value for UIBase::BackgroundColour expected colour - ", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::SelectedColour_Hash)
		{
			if (!(*it)->getValueAsColour(SelectedBackgroundColour))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - Invalid value for UIBase::SelectedColour expected colour - ", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::DisabledColour_Hash)
		{
			if (!(*it)->getValueAsColour(DisabledBackgroundColour))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - Invalid value for UIBase::DisabledColour expected colour - ", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::Enabled_Hash)
			enabled = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::SelectType_Hash)
		{
			unsigned int type_hash = (*it)->getValue().getHash();

			if (type_hash == CzHashes::Toggle_Hash)
				SelectType = SelectType_Toggle;
			else
			if (type_hash == CzHashes::Sticky_Hash)
				SelectType = SelectType_Sticky;
			else
				SelectType = SelectType_Normal;
		}
		else
		if (name_hash == CzHashes::Selected_Hash)
			selected = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Spring_Hash)
			EnableSpring = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::OnToggledOn_Hash)
			EventsManager->addEvent("OnToggledOn", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnToggledOff_Hash)
			EventsManager->addEvent("OnToggledOff", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnBackKey_Hash)
			EventsManager->addEvent("OnBackKey", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnMenuKey_Hash)
			EventsManager->addEvent("OnMenuKey", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::ClipMargin_Hash)
		{
			if (!(*it)->getValueAsRect(ClippingMargin))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - Invalid value for UIBase::ClipMargin expected rect - ", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::ScrollRange_Hash)
		{
			if (!(*it)->getValueAsRect(ScrollRange))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - Invalid value for UIBase::ScrollRange expected rect - ", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::ScrollPos_Hash)
		{
			if (!(*it)->getValueAsPoint(ScrollPos))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - Invalid value for UIBase::ScrollPos expected vec2 - ", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::ShowTimeline_Hash)
			show_timeline = &(*it)->getValue();
		else
		if (name_hash == CzHashes::HideTimeline_Hash)
			hide_timeline = &(*it)->getValue();
		else
		if (name_hash == CzHashes::SizeToContent_Hash)
		{
			unsigned int size_type = (*it)->getValue().getHash();
			if (size_type == CZ_HASH("x"))
				SizeToContentAxis = Axis_X;
			else
			if (size_type == CZ_HASH("y"))
				SizeToContentAxis = Axis_Y;
			else
			if (size_type == CZ_HASH("XY"))
				SizeToContentAxis = Axis_XY;
		}
		else
		if (name_hash == CzHashes::KeyFocus_Hash)
			key_focus = (*it)->getValueAsBool();
		else
		{	// We need to read these again as well as in CzActor as we do not store these values
			if (name_hash == CzHashes::Sensor_Hash)	
				sensor = (*it)->getValueAsBool();
			else
			if (name_hash == CzHashes::COM_Hash)
			{
				if (!(*it)->getValueAsPoint(com))
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Invalid value for Actor::COM, expected vec2 - ", DebugInfo.c_str());
			}
			else
			if (name_hash == CzHashes::Box2dMaterial_Hash)
				box2dmaterial_name = &(*it)->getValue();
		}
	}

	CzBrushImage* brush = NULL;
	if (brush_name != NULL && !brush_name->isEmpty())
	{
		brush = (CzBrushImage*)Scene->getResourceManager()->findResource(brush_name->getHash(), CzHashes::Brush_Hash);
		if (brush != NULL)
			BackgroundBrush = brush;
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - Could not find brush in the scene or in the global resources - ", brush_name->c_str(), DebugInfo.c_str());
	}
	if (selected_brush_name != NULL)
	{
		brush = (CzBrushImage*)Scene->getResourceManager()->findResource(selected_brush_name->getHash(), CzHashes::Brush_Hash);
		if (brush != NULL)
			SelectedBackgroundBrush = brush;
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - Could not find selected brush in the scene or in the global resources - ", selected_brush_name->c_str(), DebugInfo.c_str());
	}
	if (disabled_brush_name != NULL)
	{
		brush = (CzBrushImage*)Scene->getResourceManager()->findResource(disabled_brush_name->getHash(), CzHashes::Brush_Hash);
		if (brush != NULL)
			DisabledBackgroundBrush = brush;
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - Could not find disabled brush in the scene or in the global resources - ", disabled_brush_name->c_str(), DebugInfo.c_str());
	}

	// Search the scenes resources for the specified timelines
	if (show_timeline != NULL)
		setShowTimeline(show_timeline->c_str());
	if (hide_timeline != NULL)
		setHideTimeline(hide_timeline->c_str());

	// Calculate clipping 
	OriginalClippingMargin = ClippingMargin;
	if (ClippingMargin.x < 0)
		ClippingMargin.x = (Size.x * -ClippingMargin.x) / 100;
	if (ClippingMargin.y < 0)
		ClippingMargin.y = (Size.x * -ClippingMargin.y) / 100;
	if (ClippingMargin.w < 0)
		ClippingMargin.w = (Size.y * -ClippingMargin.w) / 100;
	if (ClippingMargin.h < 0)
		ClippingMargin.h = (Size.y * -ClippingMargin.h) / 100;

	setBackgroundColour(BackgroundColour);

	if (SelectType != SelectType_Normal)
		Selected = selected;

	setEnabled(enabled);

	// If set as key focus then notify the game
	if (key_focus)
		Scene->getParent()->setKeyFocus(this);

	if (load_children)
	{
		// Process inner tags
		if (!CZ_XOML->Process(this, node))
			return 0;
	}

	// if a material was specified but no shape then create a default one
	int	fixtures_count = Box2dBody == NULL ? -1 : Box2dBody->getFixturesCount();
	if (Box2dBody != NULL && Box2dBody->getWorld() != NULL && fixtures_count == 0 && box2dmaterial_name != NULL && !box2dmaterial_name->isEmpty())
	{
		CzBox2dMaterial* material = (CzBox2dMaterial*)Scene->getResourceManager()->findResource(box2dmaterial_name->c_str(), CzHashes::Box2dMaterial_Hash);
		if (material != NULL)
		{
			b2Fixture* fixture = NULL;
			if (Geometry != NULL && Geometry->Type == PrimType_Poly)
			{
				// If actor has geometry then use geometry as shape
				CzGeomShapePolygon* g = new CzGeomShapePolygon();
				g->Vertices = new CzVec2[Geometry->VertCount]();
				g->NumVertices = Geometry->VertCount;
				for (int t = 0; t < g->NumVertices; t++)
				{
					g->Vertices[t].x = Geometry->Verts[t].x;
					g->Vertices[t].y = Geometry->Verts[t].y;
				}
				CzShape shape;
				shape.setShape(g);
				fixture = Box2dBody->addFixture(&shape, material, com.x, com.y);
			}
			if (fixture == NULL)
				fixture = Box2dBody->addFixture(material, (float)Size.x * Scale.x, (float)Size.y * Scale.y, com.x, com.y);
			fixture->SetSensor(sensor);
			b2Filter f;
			f.categoryBits = Box2dBody->getCollisionCategory();
			f.maskBits = Box2dBody->getCollisionMask();
			f.groupIndex = Box2dBody->getCollisionGroup();
			fixture->SetFilterData(f);
		}
	}

	return 1;
}

/**
 @fn	bool CzUIBase::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the actor.

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types. Note that as 
 CzUIBase is derived from CzActorImage, all CzActorImage properties will also be checked against.

 @param	property_name		Name of the property.
 @param [out]	prop		The property data that.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIBase::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	if (CzActorImage::getProperty(property_name, prop))
		return true;

	int err = IzXomlResource::getProperty(UIBaseClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIBaseClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;

}

/**
 @fn	bool CzUIBase::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the actor.
	
 Sets the named property of this actor. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzUIBase::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	if (property_name == CzHashes::Size_Hash)
		setLayoutDirty(true);

	if (CzActorImage::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(UIBaseClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIBaseClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIBase::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced. Note that as CzUIBase is derived from CzActorImage, all CzActorImage properties will also be checked against.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in]	data		The property data.
 @param	delta				true to property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIBase::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	if (property_name == CzHashes::Size_Hash)
		setLayoutDirty(true);

	if (CzActorImage::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(UIBaseClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIBaseClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;

}

/**
 @fn	bool CzUIBase::UpdateBinding(unsigned int property_name, CzXomlVariable* var)

 @brief	Updates an actor property from the supplied XOML variable.

 Updates a single property from the value of the bound XOML variable. Note that as CzUIBase is derived from CzActorImage, all CzActorImage properties will also be checked against.

 @param	property_name  	Name of the property.
 @param [in,out]	var	If non-null, the variable.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIBase::UpdateBinding(unsigned int property_name, CzXomlVariable* var)
{
	if (!var->Modified || var->getValue().isEmpty())
		return false;

	if (property_name == CzHashes::Size_Hash)
		setLayoutDirty(true);

	if (CzActorImage::UpdateBinding(property_name, var))
		return true;

	int err = IzXomlResource::setProperty(UIBaseClassDef, this, property_name, var);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIBaseClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;

}

/**
 @fn	bool CzUIBase::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates actor property from supplied animation.

 Updates the actors property that is specified by the supplied animation. When a timeline animation targets the property of an actor this method will be called by the animation to move the 
 animations frame data into its target property. Note that as CzUIBase is derived from CzActorImage, all CzActorImage properties will also be checked against.

 @param [in]	animation	If non-null, the animation.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIBase::UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzActorImage::UpdateFromAnimation(animation))
	{
		unsigned int property_name = animation->getTargetPropertyHash();
		if (property_name == CzHashes::Size_Hash)
			setLayoutDirty(true);

		return true;
	}

	int err = IzXomlResource::setProperty(UIBaseClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIBaseClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "UIBase - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;

}

/**
 @fn	void CzUIBase::NotifyBeginTouch(int index, int x, int y, bool allow_bubble)

 @brief	OnBeginTouch event handler

 The OnBeginTouch event handler is called whenever the user begins to touch the actor. Actors can process up to 5 simultaneous touch events OnBeginTouch to OnBeginTouch5. if event 
 bubbling is enabled then the event will also be passed up to the parent. 

 This version of this method enables actors to utilise a selected state

 @param	index			Index of the touch.
 @param	x				The touched x coordinate.
 @param	y				The touched y coordinate.
 @param	allow_bubble	true to allow event bubbling, false to deny event bubbling.
 */

void CzUIBase::NotifyBeginTouch(int index, int x, int y, bool allow_bubble)
{
//	if (TouchIndex >= 0)
//		return;

	if (!Enabled)
		return;

	CzActor::NotifyBeginTouch(index, x, y, allow_bubble);

	if (SelectType != SelectType_Toggle)
	{
		setSelected(true);
	}
}

/**
 @fn	void CzUIBase::NotifyEndTouch(int index, int x, int y, bool allow_bubble)

 @brief	OnEndTouch event handler

 The OnEndTouch event handler is called whenever the user stops touching the actor. This can occur for a number fo reasons including:
 - if the user lifts their finger off the actor
 - If the user moves their finger too from the actor (loses focus)
 - if the user scrolls the container (list boc for example)
 - The scene is switched

 Actors can process up to 5 simultaneous touch events OnEndTouch to OnEndTouch5. if event bubbling is enabled then the event will also be passed up to the parent. 

 This version of this method enables actors to utilise a selected state

 @param	index			Index of the touch.
 @param	x				The touched x coordinate.
 @param	y				The touched y coordinate.
 @param	allow_bubble	true to allow event bubbling, false to deny event bubbling.
 */

void CzUIBase::NotifyEndTouch(int index, int x, int y, bool allow_bubble)
{
	if (!Enabled)
		return;

	CzActor::NotifyEndTouch(index, x, y, allow_bubble);

	if (SelectType == SelectType_Normal)
		setSelected(false);
	else
	if (!Tapped && !ToggledOn)
		setSelected(false);

	Tapped = false;
}

/**
 @fn	void CzUIBase::NotifyTapped(int index, int x, int y, bool allow_bubble)

 @brief	OnTapped event handler

 The OnTapped event handler is called whenever the actor is tapped on by the user. Actors can process up to 5 simultaneous touch events OnTapped to OnTapped5. if event bubbling is enabled 
 then the event will also be passed up to the parent. 

 This version of this method enables actors to utilise a selected and toggled state

 @param	index			Index of the touch.
 @param	x				The touched x coordinate.
 @param	y				The touched y coordinate.
 @param	allow_bubble	true to allow event bubbling, false to deny event bubbling.
 */

void CzUIBase::NotifyTapped(int index, int x, int y, bool allow_bubble)
{
	if (!Enabled)
		return;

	CzActor::NotifyTapped(index, x, y, allow_bubble);

	if (SelectType == SelectType_Sticky)
	{
		setSelected(Selected);
		if (!ToggledOn)
			setToggledOn(true);
	}
	else
	if (SelectType == SelectType_Toggle)
	{
		setSelected(!Selected);
		setToggledOn(!ToggledOn);
	}

	Tapped = true;
}

/**
 @fn	void CzUIBase::setSelected(bool selected, bool force_select)

 @brief	Sets actors selected state.

 @param	selected		true if selected.
 @param	force_select	true to force select.
 */

void CzUIBase::setSelected(bool selected, bool force_select)
{
	PrevSelected = Selected;

	Selected = selected;
	setEnabled(Enabled);
}

/**
 @fn	void CzUIBase::setToggledOn(bool toggled_on)

 @brief	Sets actores toggled state.

 If the toggle state changes then the OnToggledOn or OnToggledOff events will be raised

 @param	toggled_on	true to toggle on, false to toggle off
 */

void CzUIBase::setToggledOn(bool toggled_on)
{
	if (SelectType == SelectType_Normal)
		return;

	if (!ToggledOn && toggled_on)
		NotifyToggledOn();
	else
	if (ToggledOn && !toggled_on)
		NotifyToggledOff();

	ToggledOn = toggled_on;
}

/**
 @fn	void CzActor::NotifyOrientationChange(CzScene::eOrientation old_orientation,  CzScene::eOrientation new_orientation)

 @brief	Handles the orientation change event.

 When a screen orientation or size change occurs all actors will receive this event enabling them to modify themselves to take advantage of the new screen orientation / size.  

 @param	old_orientation	The old orientation.
 @param	new_orientation	The new orientation.
 */

void CzUIBase::NotifyOrientationChange(CzScene::eOrientation old_orientation, CzScene::eOrientation new_orientation)
{
	CzActorImage::NotifyOrientationChange(old_orientation, new_orientation);

	// Calculate clipping 
	if (ClippingMargin.x < 0)
		ClippingMargin.x = (Size.x * -OriginalClippingMargin.x) / 100;
	if (ClippingMargin.y < 0)
		ClippingMargin.y = (Size.x * -OriginalClippingMargin.y) / 100;
	if (ClippingMargin.w < 0)
		ClippingMargin.w = (Size.y * -OriginalClippingMargin.w) / 100;
	if (ClippingMargin.h < 0)
		ClippingMargin.h = (Size.y * -OriginalClippingMargin.h) / 100;

	setLayoutDirty(true);
}

/**
 @fn	void CzUIBase::NotifyToggledOn()

 @brief	Handles the toggled on event

 */

void CzUIBase::NotifyToggledOn()
{
	ProcessEventActions(CzHashes::OnToggledOn_Hash);
}

/**
 @fn	void CzUIBase::NotifyToggledOff()

 @brief	Handles the toggled off event
 */
 
void CzUIBase::NotifyToggledOff()
{
	ProcessEventActions(CzHashes::OnToggledOff_Hash);
}

/**
 @fn	void CzUIBase::NotifyBackKey()

 @brief	Handles the back key press evente
 */
 
void CzUIBase::NotifyBackKey()
{
	ProcessEventActions(CzHashes::OnBackKey_Hash);
}

/**
 @fn	void CzUIBase::NotifyMenuKey()

 @brief	Handles the menu key press evente
 */
void CzUIBase::NotifyMenuKey()
{
	ProcessEventActions(CzHashes::OnMenuKey_Hash);
}

/**
 @fn	void CzUIBase::NotifyParentsResize()

 @brief	Notifies the parent to resize its layout.

 Used when this actor changes in a way that could potentially change the parents layout.
 */

void CzUIBase::NotifyParentsResize()
{
	NeedsResize = true;
	// Notify parent that it needs to resize
	if (LinkedTo != NULL && LinkedTo->getCategory() == CzActor::AC_UI)
	{
		CzUIBase* ui = (CzUIBase*)LinkedTo;
		if (ui->getSizeToContentAxis())
		{
			ui->setNeedsResize(true);
			ui->NotifyParentsResize();
		}
	}
}




//
//
//
//
//
//
// Sttatic preset animations
//
//
//
//
//
//
//

/**
 @fn	void CzUIBase::InitPresetAnimations()

 @brief	Initialises the preset animations.

 Static method that creates a group of preset animations.
 */

void CzUIBase::InitPresetAnimations()
{
	float width = (float)PLATFORM_DISPLAY->getCurrentWidth();
	float height = (float)PLATFORM_DISPLAY->getCurrentHeight();

	//
	// Create generic common animations
	// 
	float v1 = -width;
	float v2 = 0;
	PresetAnimations.push_back(CzAnimUtil::CreateFloatAnim("PA_ScrollOnFromLeft", v1, v2, 0.5f, CzAnimFrame::AT_QuadIn, 0));
	v1 = 0;
	v2 = -width;
	PresetAnimations.push_back(CzAnimUtil::CreateFloatAnim("PA_ScrollOffToLeft", v1, v2, 0.5f, CzAnimFrame::AT_QuadIn, 0));
	v1 = width;
	v2 = 0;
	PresetAnimations.push_back(CzAnimUtil::CreateFloatAnim("PA_ScrollOnFromRight", v1, v2, 0.5f, CzAnimFrame::AT_QuadIn, 0));
	v1 = 0;
	v2 = width;
	PresetAnimations.push_back(CzAnimUtil::CreateFloatAnim("PA_ScrollOffToRight", v1, v2, 0.5f, CzAnimFrame::AT_QuadIn, 0));
	v1 = -height;
	v2 = 0;
	PresetAnimations.push_back(CzAnimUtil::CreateFloatAnim("PA_ScrollOnFromTop", v1, v2, 0.5f, CzAnimFrame::AT_QuadIn, 1));
	v1 = 0;
	v2 = -height;
	PresetAnimations.push_back(CzAnimUtil::CreateFloatAnim("PA_ScrollOffToTop", v1, v2, 0.5f, CzAnimFrame::AT_QuadIn, 1));
	v1 = height;
	v2 = 0;
	PresetAnimations.push_back(CzAnimUtil::CreateFloatAnim("PA_ScrollOnFromBottom", v1, v2, 0.5f, CzAnimFrame::AT_QuadIn, 1));
	v1 = 0;
	v2 = height;
	PresetAnimations.push_back(CzAnimUtil::CreateFloatAnim("PA_ScrollOffToBottom", v1, v2, 0.5f, CzAnimFrame::AT_QuadIn, 1));
	PresetAnimations.push_back(CzAnimUtil::CreateFloatAnim("PA_FadeOn", 1, 255.0f, 0.5f, CzAnimFrame::AT_QuadIn, 2));
	PresetAnimations.push_back(CzAnimUtil::CreateFloatAnim("PA_FadeOff", 255.0f, 1, 0.5f, CzAnimFrame::AT_QuadIn, 2));
}

/**
 @fn	void CzUIBase::ReleasePresetAnimations()

 @brief	Initialises the preset animations.
	
 Static method that detroys a group of preset animations.
 */

void CzUIBase::ReleasePresetAnimations()
{
	for (CzList<CzAnim*>::iterator it = PresetAnimations.begin(); it != PresetAnimations.end(); ++it)
		delete *it;
	PresetAnimations.clear();
}

/**
 @fn	void CzUIBase::UpdatePresetAnimations()

 @brief	Updates the preset animations.

 Static method that updates a group of preset animations.
 */

void CzUIBase::UpdatePresetAnimations()
{
	float width = (float)PLATFORM_DISPLAY->getCurrentWidth();
	float height = (float)PLATFORM_DISPLAY->getCurrentHeight();

	CzAnim* anim = FindPresetAnimation(CZ_HASH("PA_ScrollOnFromLeft"));
	((CzAnimFrameVec2*)anim->getFrame(0))->data.x = -width;
	anim = FindPresetAnimation(CZ_HASH("PA_ScrollOffToLeft"));
	((CzAnimFrameVec2*)anim->getFrame(1))->data.x = -width;
	anim = FindPresetAnimation(CZ_HASH("PA_ScrollOnFromRight"));
	((CzAnimFrameVec2*)anim->getFrame(0))->data.x = width;
	anim = FindPresetAnimation(CZ_HASH("PA_ScrollOffToRight"));
	((CzAnimFrameVec2*)anim->getFrame(1))->data.x = width;
	anim = FindPresetAnimation(CZ_HASH("PA_ScrollOnFromTop"));
	((CzAnimFrameVec2*)anim->getFrame(0))->data.y = -height;
	anim = FindPresetAnimation(CZ_HASH("PA_ScrollOffToTop"));
	((CzAnimFrameVec2*)anim->getFrame(1))->data.y = -height;
	anim = FindPresetAnimation(CZ_HASH("PA_ScrollOnFromBottom"));
	((CzAnimFrameVec2*)anim->getFrame(0))->data.y = height;
	anim = FindPresetAnimation(CZ_HASH("PA_ScrollOffToBottom"));
	((CzAnimFrameVec2*)anim->getFrame(1))->data.y = height;
}

/**
 @fn	CzAnim* CzUIBase::FindPresetAnimation(unsigned int name_hash)

 @brief	Searches for the first preset animation.

 @param	name_hash	The animation name as a string hash.

 @return	null if it fails, else the found preset animation.
 */

CzAnim* CzUIBase::FindPresetAnimation(unsigned int name_hash)
{
	for (CzList<CzAnim*>::iterator it = PresetAnimations.begin(); it != PresetAnimations.end(); ++it)
	{
		if ((*it)->getNameHash() == name_hash)
			return *it;
	}

	return NULL;
}

