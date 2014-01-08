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

#include "CzUIStackPanel.h"
#include "CzUtil.h"
#include "CzString.h"
#include "CzTemplates.h"
#include "CzHashes.h"


CzXomlClassDef* CzUIStackPanel::UIStackPanelClassDef = NULL;

/**
 @fn	void CzUIStackPanel::InitClass()

 @brief	Initialises the class.
    
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzUIStackPanel::InitClass()
{
	// Create class properties
	UIStackPanelClassDef = new CzXomlClassDef();
	UIStackPanelClassDef->addProperty(new CzXomlClassProperty("Orientation",	PT_String,		CzUIStackPanel::_setOrientation,		CzUIStackPanel::_getOrientation));
	UIStackPanelClassDef->addProperty(new CzXomlClassProperty("AlignH",			PT_String,		CzUIStackPanel::_setAlignH,				NULL));
	UIStackPanelClassDef->addProperty(new CzXomlClassProperty("AlignV",			PT_String,		CzUIStackPanel::_setAlignV,				NULL));
}
void CzUIStackPanel::ReleaseClass()
{
	if (UIStackPanelClassDef != NULL)
	{
		UIStackPanelClassDef->Release();
		delete UIStackPanelClassDef;
		UIStackPanelClassDef = NULL;
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

bool CzUIStackPanel::_setOrientation(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIStackPanel* actor = (CzUIStackPanel*)target;
	eCzOrientation o;

	unsigned int o_hash = CzString::CalculateHash((const char*)prop.p_data);
	if (o_hash == CzHashes::Horizontal_Hash)
		o = Orientation_Horizontal;
	else
	if (o_hash == CzHashes::Vertical_Hash)
		o = Orientation_Vertical;
	else
		return false;

	actor->setOrientation(o);

	return true;
}

CzXomlProperty	CzUIStackPanel::_getOrientation(IzXomlResource* target)
{
	return CzXomlProperty(CzXoml::OrientationNames[((CzUIStackPanel*)target)->getOrientation()]);
}

bool CzUIStackPanel::_setAlignH(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIStackPanel* actor = (CzUIStackPanel*)target;
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

bool CzUIStackPanel::_setAlignV(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIStackPanel* actor = (CzUIStackPanel*)target;
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



CzUIStackPanel::~CzUIStackPanel()
{
	Children.clear();
}


bool CzUIStackPanel::UpdateLayout()
{
	// Stack all actors
	_Iterator _begin = begin();
	_Iterator _end = end();
	if (Children.size() != 0)
	{
		_Iterator it = _begin;
		CzIRect margin = (*it)->getMargin();
		CzIVec2 pos;
		
		// Calculate initial start position of content
		if (Orientation == Orientation_Horizontal)
		{
			pos = CzIVec2(-Size.x / 2, margin.w);
			if (AlignH == AlignH_Left)
				pos.x += (*it)->getSize().x / 2;
		}
		else
		{
			pos = CzIVec2(margin.x, -Size.y / 2);
			if (AlignV == AlignV_Top)
				pos.y += (*it)->getSize().y / 2;
		}

		// Arrange the content
		CzIVec2 opos = CzIVec2(pos.x, pos.y);
		_Iterator it2 = _begin;
		if (it2 != _end)
			it2++;
		for (it = _begin; it != _end; ++it)
		{
			if (Orientation == Orientation_Horizontal)
			{
				CzIRect margin = (*it)->getMargin();
				if (AlignV == AlignV_Top)
					pos.y = -Size.y / 2 + (*it)->getSize().y / 2 + margin.w;
				else
				if (AlignV == AlignV_Bottom)
					pos.y = Size.y / 2 - (*it)->getSize().y / 2 - margin.h;
				else
					pos.y = 0;
				pos.x += margin.x;
				(*it)->setPosition((float)pos.x, (float)pos.y);
				int d = (*it)->getSize().x / 2;
				if (it2 != _end)
				{
					d += (*it2)->getSize().x / 2;
					it2++;
				}
				pos.x += d + margin.y;
			}
			else
			{
				CzIRect margin = (*it)->getMargin();
				if (AlignH == AlignH_Left)
					pos.x = -Size.x / 2 + (*it)->getSize().x / 2 + margin.x;
				else
				if (AlignH == AlignH_Right)
					pos.x = Size.x / 2 - (*it)->getSize().x / 2 - margin.y;
				else
					pos.x = 0;
				pos.y += margin.w;
				(*it)->setPosition((float)pos.x, (float)pos.y);
				int d = (*it)->getSize().y / 2;
				if (it2 != _end)
				{
					d += (*it2)->getSize().y / 2;
					it2++;
				}
				pos.y += d + margin.h;
			}
		}

		// If content is aligned to right or bottom of stack panel then adjust positions to align them with edge
		if (Orientation == Orientation_Horizontal)
		{
			if (AlignH == AlignH_Right)
			{
				float dx = (float)(Size.x - (pos.x - opos.x));	// Calculate how much to adjust positions to align with right edge of stack panel
				for (it = _begin; it != _end; ++it)
				{
					CzVec2 pos2 = (*it)->getPosition();
					pos2.x += dx;
					(*it)->setPosition(pos2);
				}
			}
			else
			if (AlignH == AlignH_Centre)
			{
				float dx = (float)(((Size.x + (*_begin)->getSize().x / 2 - (pos.x - opos.x)) / 2));	// Calculate how much to adjust positions to align with centre of stack panel
				for (it = _begin; it != _end; ++it)
				{
					CzVec2 pos2 = (*it)->getPosition();
					pos2.x += dx;
					(*it)->setPosition(pos2);
				}
			}
		}
		else
		if (Orientation == Orientation_Vertical)
		{
			if (AlignV == AlignV_Bottom)
			{
				float dy = (float)(Size.y - (pos.y - opos.y));	// Calculate how much to adjust positions to align with bottom edge of stack panel
				for (it = _begin; it != _end; ++it)
				{
					CzVec2 pos2 = (*it)->getPosition();
					pos2.y += dy;
					(*it)->setPosition(pos2);
				}
			}
			else
			if (AlignV == AlignV_Middle)
			{
				float dy = (float)(((Size.y + (*_begin)->getSize().y / 2 - (pos.y - opos.y)) / 2));	// Calculate how much to adjust positions to align with middle of stack panel
				for (it = _begin; it != _end; ++it)
				{
					CzVec2 pos2 = (*it)->getPosition();
					pos2.y += dy;
					(*it)->setPosition(pos2);
				}
			}
		}
	}

	return CzUIPanel::UpdateLayout();
}

void CzUIStackPanel::ResizeToContent()
{
	if (SizeToContentAxis)
	{
		int w = 0, h = 0;
		for (_Iterator it = begin(); it != end(); ++it)
		{
			CzIRect margin = (*it)->getMargin();
			CzIVec2 size = (*it)->getSize();
			int x = size.x + margin.x + margin.y;
			int y = size.y + margin.w + margin.h;
			if (Orientation == Orientation_Horizontal)
			{
				w += x;
				if (y > h)
					h = y;
			}
			else
			{
				h += y;
				if (x > w)
					w = x;
			}
		}
		switch (SizeToContentAxis)
		{
		case Axis_X:
			Size.x = w;
			if (Visual != NULL)
				h = (int)Visual->getDestHeight();
			break;
		case Axis_Y:
			Size.y = h;
			if (Visual != NULL)
				w = (int)Visual->getDestWidth();
			break;
		case Axis_XY:
			Size.x = w;
			Size.y = h;
			break;
		}
		SetDestSize(w, h);
		setLayoutDirty(true);
	}

	CzUIPanel::ResizeToContent();
}

int CzUIStackPanel::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "StackPanel - Needs to be declared inside a scene or an actor - ", DebugInfo.c_str());
		return 0;
	}

	// Load main actor attributes
	int ret = CzUIPanel::LoadFromXoml(parent, false, node);
	if (ret <= 0)
		return ret;

	// Process StackPanel specific attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Orientation_Hash)
		{
			unsigned int hash = (*it)->getValue().getHash();
			if (hash == CzHashes::Vertical_Hash)
				Orientation = Orientation_Vertical;
		}
		else
		if (name_hash == CzHashes::AlignH_Hash)
		{
			unsigned int align_hash = (*it)->getValue().getHash();
			if (align_hash == CzHashes::left_Hash)
				AlignH = AlignH_Left;
			else
			if (align_hash == CzHashes::right_Hash)
				AlignH = AlignH_Right;
			else
				AlignH = AlignH_Centre;
		}
		else
		if (name_hash == CzHashes::AlignV_Hash)
		{
			unsigned int align_hash = (*it)->getValue().getHash();
			if (align_hash == CzHashes::top_Hash)
				AlignV = AlignV_Top;
			else
			if (align_hash == CzHashes::bottom_Hash)
				AlignV = AlignV_Bottom;
			else
				AlignV = AlignV_Middle;
		}
	}

	// Process inner tags
	if (load_children)
	{
		if (!CZ_XOML->Process(this, node))
			return 0;
	}

	ResizeToContent();

	return 1;
}

/**
 @fn	bool CzUIStackPanel::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the actor.

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types. Note that as 
 CzUIStackPanel is derived from CzUIPanel, all CzUIPanel properties will also be checked against.

 @param	property_name		Name of the property.
 @param [out]	prop	The property data that.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIStackPanel::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	if (CzUIPanel::getProperty(property_name, prop))
		return true;

	int err = IzXomlResource::getProperty(UIStackPanelClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIStackPanelClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "StackPanel - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIStackPanel::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the UIStackPanel.
    
 Sets the named property of this UIStackPanel. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzUIStackPanel::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	if (CzUIPanel::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(UIStackPanelClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIStackPanelClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "StackPanel - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIStackPanel::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced. Note that as CzUIStackPanel is derived from CzUIPanel, all CzUIPanel properties will also be checked against.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in]	data	The property data.
 @param	delta				true to property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIStackPanel::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	if (CzUIPanel::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(UIStackPanelClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIStackPanelClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "StackPanel - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIStackPanel::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates actor property from supplied animation.

 Updates the actors property that is specified by the supplied animation. When a timeline animation targets the property of an actor this method will be called by the animation to move the 
 animations frame data into its target property. Note that as CzUIStackPanel is derived from CzUIPanel, all CzUIPanel properties will also be checked against.

 @param [in]	animation	If non-null, the animation.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIStackPanel::UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzUIPanel::UpdateFromAnimation(animation))
		return true;

	int err = IzXomlResource::setProperty(UIStackPanelClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIStackPanelClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "StackPanel - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIStackPanel::UpdateBinding(unsigned int property_name, CzXomlVariable* var)

 @brief	Updates an actor property from the supplied XOML variable.

 Updates a single property from the value of the bound XOML variable. Note that as CzUIStackPanel is derived from CzUIPanel, all CzUIPanel properties will also be checked against.

 @param	property_name  	Name of the property.
 @param [in,out]	var	If non-null, the variable.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIStackPanel::UpdateBinding(unsigned int property_name, CzXomlVariable* var)
{
	if (!var->Modified || var->getValue().isEmpty())
		return false;

	if (CzUIPanel::UpdateBinding(property_name, var))
		return true;

	int err = IzXomlResource::setProperty(UIStackPanelClassDef, this, property_name, var);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIStackPanelClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "StackPanel - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}
