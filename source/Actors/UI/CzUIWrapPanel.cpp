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

#include "CzUIWrapPanel.h"
#include "CzUtil.h"
#include "CzString.h"
#include "CzTemplates.h"
#include "CzHashes.h"

CzXomlClassDef* CzUIWrapPanel::UIWrapPanelClassDef = NULL;

/**
 @fn	void CzUIWrapPanel::InitClass()

 @brief	Initialises the class.
    
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzUIWrapPanel::InitClass()
{
	// Create class properties
	UIWrapPanelClassDef = new CzXomlClassDef();
	UIWrapPanelClassDef->addProperty(new CzXomlClassProperty("Orientation",	PT_String,		CzUIWrapPanel::_setOrientation,		CzUIWrapPanel::_getOrientation));
}
void CzUIWrapPanel::ReleaseClass()
{
	if (UIWrapPanelClassDef != NULL)
	{
		UIWrapPanelClassDef->Release();
		delete UIWrapPanelClassDef;
		UIWrapPanelClassDef = NULL;
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

bool CzUIWrapPanel::_setOrientation(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIWrapPanel* actor = (CzUIWrapPanel*)target;
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

CzXomlProperty	CzUIWrapPanel::_getOrientation(IzXomlResource* target)
{
	return CzXomlProperty(CzXoml::OrientationNames[((CzUIWrapPanel*)target)->getOrientation()]);
}



//
//
//
//
//
// CzUIWrapPanel implementation
//
//
//
//
//

CzUIWrapPanel::~CzUIWrapPanel()
{
	Children.clear();
}

bool CzUIWrapPanel::UpdateLayout()
{
	int left = -Size.x / 2 + ClippingMargin.x;
	int right = Size.x / 2 - ClippingMargin.y;
	int top = -Size.y / 2 + ClippingMargin.w;
	int bottom = Size.y / 2 - ClippingMargin.h;

	ScrollRange.w = 0;
	ScrollRange.h = 0;

	// Stack all actors
	_Iterator _begin = begin();
	_Iterator _end = end();
	if (Children.size() != 0)
	{
		_Iterator it = _begin;
		CzIRect margin = (*it)->getMargin();
		CzIVec2 pos;
		
		// Calculate initial start position of content
		pos = CzIVec2(-Size.x / 2 - (int)ScrollPos.x, -Size.y / 2 - (int)ScrollPos.y);
		CzIVec2 opos = pos;
		pos.x += (*it)->getSize().x / 2;
		pos.y += (*it)->getSize().y / 2;

		// Arrange the content
		_Iterator it2 = _begin;
		if (it2 != _end)
			it2++;

		int max_height = 0, max_width = 0;
		int max_x = left, max_y = top;

		for (it = _begin; it != _end; ++it)
		{
			CzActor* actor = *it;
			CzIVec2 size = actor->getSize();
			CzIRect margin = actor->getMargin();
			if (Orientation == Orientation_Horizontal)
			{
				pos.x += margin.x;
				if ((pos.x + (size.x / 2)) > right)
				{
					pos.x = opos.x + margin.x + size.x / 2;
					opos.y += max_height;
					max_height = 0;
				}
				int height = size.y + margin.w + margin.h;
				if (height > max_height)
					max_height = height;
				int dy = (size.y / 2) + margin.w;
				int y = opos.y + dy;
				actor->setPosition((float)pos.x, (float)y);
				if ((y + dy) < top || (y - (size.y / 2)) > bottom)
					actor->setVisible(false);
				else
					actor->setVisible(true);
				int y2 = y + margin.h + size.y / 2;
				if (y2 > max_y)
					max_y = y2;
				int d = size.x / 2;
				if (it2 != _end)
				{
					d += (*it2)->getSize().x / 2;
					it2++;
				}
				pos.x += d + margin.y;
			}
			else
			{
				pos.y += margin.w;
				if ((pos.y + (size.y / 2)) > bottom)
				{
					pos.y = opos.y + margin.w + size.y / 2;
					opos.x += max_width;
					max_width = 0;
				}
				int width = size.x + margin.x + margin.y;
				if (width > max_width)
					max_width = width;
				int dx = (size.x / 2) + margin.x;
				int x = opos.x + dx;
				actor->setPosition((float)x, (float)pos.y);
				if ((x + dx) < left || (x - (size.x / 2)) > right)
					actor->setVisible(false);
				else
					actor->setVisible(true);
				int x2 = x + margin.y + size.x / 2;
				if (x2 > max_x)
					max_x = x2;
				int d = size.y / 2;
				if (it2 != _end)
				{
					d += (*it2)->getSize().y / 2;
					it2++;
				}
				pos.y += d + margin.h;
			}
		}
		if (Orientation == Orientation_Horizontal)
		{
			// Calculate vertical scroll range 
			ScrollRange.h = (max_y + (int)ScrollPos.y) - top - Size.y;
			if (ScrollRange.h < 0)
				ScrollRange.h = 0;
		}
		else
		{
			// Calculate horizontal scroll range 
			ScrollRange.w = (max_x + (int)ScrollPos.x) - left - Size.x;
			if (ScrollRange.w < 0)
				ScrollRange.w = 0;
		}
	}

	return CzUIBase::UpdateLayout();
}

bool CzUIWrapPanel::Update(float dt)
{
	if (!CzUIBase::Update(dt))
		return false;

	// Set clip rect for the list box
	CzVec4 rc;
	rc.x = (float)((-Size.x >> 1) + ClippingMargin.x);
	rc.y = (float)((-Size.y >> 1) + ClippingMargin.w);
	rc.z = (float)(Size.x - (ClippingMargin.x + ClippingMargin.y));
	rc.w = (float)(Size.y - (ClippingMargin.w + ClippingMargin.h));
	Visual->setClipRect(rc);

	return true;
}

int CzUIWrapPanel::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "WrapPanel - Needs to be declared inside a scene or an actor - ", DebugInfo.c_str());
		return 0;
	}

	// Load main actor attributes
	int ret = CzUIBase::LoadFromXoml(parent, false, node);
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
 @fn	bool CzUIWrapPanel::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the actor.

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types. Note that as 
 CzUIWrapPanel is derived from CzUIPanel, all CzUIPanel properties will also be checked against.

 @param	property_name		Name of the property.
 @param [out]	prop	The property data that.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIWrapPanel::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	if (CzUIPanel::getProperty(property_name, prop))
		return true;

	int err = IzXomlResource::getProperty(UIWrapPanelClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIWrapPanelClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "WrapPanel - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIWrapPanel::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the UIWrapPanel.
    
 Sets the named property of this UIWrapPanel. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzUIWrapPanel::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	if (CzUIPanel::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(UIWrapPanelClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIWrapPanelClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "WrapPanel - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIWrapPanel::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced. Note that as CzUIWrapPanel is derived from CzUIPanel, all CzUIPanel properties will also be checked against.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in]	data	The property data.
 @param	delta				true to property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIWrapPanel::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	if (CzUIPanel::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(UIWrapPanelClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIWrapPanelClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "WrapPanel - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIWrapPanel::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates actor property from supplied animation.

 Updates the actors property that is specified by the supplied animation. When a timeline animation targets the property of an actor this method will be called by the animation to move the 
 animations frame data into its target property. Note that as CzUIWrapPanel is derived from CzUIPanel, all CzUIPanel properties will also be checked against.

 @param [in]	animation	If non-null, the animation.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIWrapPanel::UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzUIPanel::UpdateFromAnimation(animation))
		return true;

	int err = IzXomlResource::setProperty(UIWrapPanelClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIWrapPanelClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "WrapPanel - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIWrapPanel::UpdateBinding(unsigned int property_name, CzXomlVariable* var)

 @brief	Updates an actor property from the supplied XOML variable.

 Updates a single property from the value of the bound XOML variable. Note that as CzUIWrapPanel is derived from CzUIPanel, all CzUIPanel properties will also be checked against.

 @param	property_name  	Name of the property.
 @param [in,out]	var	If non-null, the variable.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIWrapPanel::UpdateBinding(unsigned int property_name, CzXomlVariable* var)
{
	if (!var->Modified || var->getValue().isEmpty())
		return false;

	if (CzUIPanel::UpdateBinding(property_name, var))
		return true;

	int err = IzXomlResource::setProperty(UIWrapPanelClassDef, this, property_name, var);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIWrapPanelClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "WrapPanel - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}
