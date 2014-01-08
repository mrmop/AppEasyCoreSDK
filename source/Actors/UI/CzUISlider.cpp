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

#include "CzUISlider.h"
#include "CzUtil.h"
#include "CzString.h"
#include "CzApp.h"
#include "CzHashes.h"

CzXomlClassDef* CzUISlider::UISliderClassDef = NULL;

/**
 @fn	void CzUISlider::InitClass()

 @brief	Initialises the class.
    
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzUISlider::InitClass()
{
	// Create class properties
	UISliderClassDef = new CzXomlClassDef();
	UISliderClassDef->addProperty(new CzXomlClassProperty("SliderType",		PT_String,		CzUISlider::_setSliderType,		CzUISlider::_getSliderType));
	UISliderClassDef->addProperty(new CzXomlClassProperty("Value",			PT_Float,		CzUISlider::_setValue,			CzUISlider::_getValue));
	UISliderClassDef->addProperty(new CzXomlClassProperty("ValueRange",		PT_Vec2,		CzUISlider::_setValueRange,		CzUISlider::_getValueRange));
	UISliderClassDef->addProperty(new CzXomlClassProperty("SliderSize",		PT_Int,			CzUISlider::_setSliderSize,		CzUISlider::_getSliderSize));
	UISliderClassDef->addProperty(new CzXomlClassProperty("OnValueChanged",	PT_String,		CzUISlider::_setOnValueChanged,	NULL));
}
void CzUISlider::ReleaseClass()
{
	if (UISliderClassDef != NULL)
	{
		UISliderClassDef->Release();
		delete UISliderClassDef;
		UISliderClassDef = NULL;
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

bool CzUISlider::_setSliderType(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUISlider* actor = (CzUISlider*)target;

	unsigned int hash = CzString::CalculateHash((const char*)prop.p_data);
	if (hash == CzHashes::Horizontal_Hash)
		actor->setSliderType(SliderType_Horizontal);
	else
	if (hash == CzHashes::Vertical_Hash)
		actor->setSliderType(SliderType_Vertical);
	else
		return false;

	return true;
}

CzXomlProperty CzUISlider::_getSliderType(IzXomlResource* target)
{
	return CzXomlProperty(CzXoml::OrientationNames[((CzUISlider*)target)->getSelectType()]);
}

bool CzUISlider::_setValue(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUISlider* actor = (CzUISlider*)target;

	actor->setValue(prop.p_float, true);

	return true;
}

CzXomlProperty CzUISlider::_getValue(IzXomlResource* target)
{
	return CzXomlProperty(((CzUISlider*)target)->getValue());
}

bool CzUISlider::_setValueRange(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUISlider* actor = (CzUISlider*)target;

	CzVec2 range = CzVec2(prop.p_vec[0], prop.p_vec[1]);
	actor->setValueRange(range);

	return true;
}

CzXomlProperty CzUISlider::_getValueRange(IzXomlResource* target)
{
	return CzXomlProperty(((CzUISlider*)target)->getValueRange());
}

bool CzUISlider::_setSliderSize(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUISlider* actor = (CzUISlider*)target;

	actor->setSliderSize(prop.p_int);

	return true;
}

CzXomlProperty CzUISlider::_getSliderSize(IzXomlResource* target)
{
	return CzXomlProperty(((CzUISlider*)target)->getSliderSize());
}

bool CzUISlider::_setOnValueChanged(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUISlider* actor = (CzUISlider*)target;

	actor->getEventsManager()->addEvent("OnValueChanged", (const char*)prop.p_data, true);

	return true;
}


void CzUISlider::setValue(float pos, bool affect_thumb)
{
	if (pos < ValueRange.x)
		pos = ValueRange.x;
	if (pos > ValueRange.y)
		pos = ValueRange.y;

	Value = pos;

	if (affect_thumb)
	{
		float range = ValueRange.y - ValueRange.x;
		if (SliderType == SliderType_Horizontal)
		{
			float x = ((Value - ValueRange.x) * SliderSize) / range - SliderSize / 2;
			setPosition(x, Position.y);
		}
		else
		if (SliderType == SliderType_Vertical)
		{
			float y = ((ValueRange.y - (Value - ValueRange.x)) * SliderSize) / range - SliderSize / 2;
			setPosition(Position.x, y);
		}
	}
}

void CzUISlider::setSliderSize(int size)
{
	SliderSize = size;
	OriginalSliderSize = SliderSize;
	UpdateSliderSize();
}

void CzUISlider::setSliderType(eSliderType type)
{
	SliderType = type;
	UpdateSliderSize();
}

void CzUISlider::setValueRange(CzVec2& range)
{
	ValueRange = range;

	float drange = ValueRange.y - ValueRange.x;

	if (SliderType == SliderType_Horizontal)
	{
		float x = ((Value - ValueRange.x) * SliderSize) / drange - SliderSize / 2;
		setPosition(x, Position.y);
	}
	else
	if (SliderType == SliderType_Vertical)
	{
		float y = ((ValueRange.y - (Value - ValueRange.x)) * SliderSize) / drange - SliderSize / 2;
		setPosition(Position.x, y);
	}

}

void CzUISlider::UpdateSliderSize()
{
	if (LinkedTo != NULL)
	{
		if (SliderSize < 0 && SliderType == SliderType_Horizontal)
			SliderSize = (LinkedTo->getSize().x * -SliderSize) / 100;
		else
		if (SliderSize < 0 && SliderType == SliderType_Vertical)
			SliderSize = (LinkedTo->getSize().y * -SliderSize) / 100;
	}
	else
	{
		// If negative size has been specified then base size on a percentage of virtual canvas
		if (SliderSize < 0 && SliderType == SliderType_Horizontal)
			SliderSize = (int)((Scene->getScreenSize().x / Scene->getVirtualTransform().m[0][0] * -SliderSize) / 100);
		else
		if (SliderSize < 0 && SliderType == SliderType_Vertical)
			SliderSize = (int)((Scene->getScreenSize().y / Scene->getVirtualTransform().m[1][1] * -SliderSize) / 100);
	}
}

int CzUISlider::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Slider - Needs to be declared inside a scene or an actor - ", DebugInfo.c_str());
		return 0;
	}

	// Load main actor attributes
	int ret = CzUIBase::LoadFromXoml(parent, false, node);
	if (ret <= 0)
		return ret;

	// Process slider specific attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Value_Hash)
		{
			Value = (*it)->getValueAsFloat();
		}
		else
		if (name_hash == CzHashes::ValueRange_Hash)
		{
			if (!(*it)->getValueAsPoint(ValueRange))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Slider - Invalid value for Slider::ValueRange, expected vec2 - ", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::SliderType_Hash)
		{
			unsigned int type_hash = (*it)->getValue().getHash();

			if (type_hash == CzHashes::Vertical_Hash)
				SliderType = SliderType_Vertical;
			else
				SliderType = SliderType_Horizontal;
		}
		else
		if (name_hash == CzHashes::SliderSize_Hash)
		{
			SliderSize = (*it)->getValueAsInt();
		}
		else
		if (name_hash == CzHashes::OnValueChanged_Hash)
			EventsManager->addEvent("OnValueChanged", (*it)->getValue().c_str());
	}

	OriginalSliderSize = SliderSize;

	UpdateSliderSize();

	setValue(Value, true);

	if (load_children)
	{
		// Process inner tags
		if (!CZ_XOML->Process(this, node))
			return 0;
	}
	return 1;
}

bool CzUISlider::Update(float dt)
{
	CzVec2 pos = Position;
	
	if (!CzUIBase::Update(dt))
		return false;
	
	float old = Value;
	float range = ValueRange.y - ValueRange.x;
	float x1 = -(float)SliderSize / 2;
	float x2 = (float)SliderSize / 2;

	if (SliderType == SliderType_Horizontal)
	{
		float x = Position.x;
		if (x < x1)
			x = x1;
		if (x > x2)
			x = x2;
		Value = ValueRange.x + ((x - x1) * range) / SliderSize;

		setPosition(x, pos.y);
	}
	else
	if (SliderType == SliderType_Vertical)
	{
		float y = Position.y;
		if (y < x1)
			y = x1;
		if (y > x2)
			y = x2;
		Value = (ValueRange.y - (ValueRange.x + ((y - x1) * range) / SliderSize));

		setPosition(pos.x, y);
	}

	if (fabs(old - Value) > 0.0001f)
	{
		ThumbChanged = true;
		NotifyValueChanged(old, Value);
	}
	else
		ThumbChanged = false;

	return true;
}

void CzUISlider::NotifyBeginTouch(int index, int x, int y, bool allow_bubble)
{
	if (TouchIndex >= 0)
		return;

	CzUIBase::NotifyBeginTouch(index, x, y, allow_bubble);
}

void CzUISlider::NotifyEndTouch(int index, int x, int y, bool allow_bubble)
{
	CzUIBase::NotifyEndTouch(index, x, y, allow_bubble);
}

void CzUISlider::NotifyValueChanged(float old_value, float new_value)
{
	ProcessEventActions(CzHashes::OnValueChanged_Hash);
}

void CzUISlider::NotifyOrientationChange(CzScene::eOrientation old_orientation, CzScene::eOrientation new_orientation)
{
	CzActorImage::NotifyOrientationChange(old_orientation, new_orientation);

	if (LinkedTo != NULL)
	{
		if (OriginalSliderSize < 0 && SliderType == SliderType_Horizontal)
			SliderSize = (LinkedTo->getSize().x * -OriginalSliderSize) / 100;
		else
		if (OriginalSliderSize < 0 && SliderType == SliderType_Vertical)
			SliderSize = (LinkedTo->getSize().y * -OriginalSliderSize) / 100;
	}
	else
	{
		// If negative size has been specified then base size on a percentage of virtual canvas
		if (OriginalSliderSize < 0 && SliderType == SliderType_Horizontal)
			SliderSize = (int)((Scene->getScreenSize().x / Scene->getVirtualTransform().m[0][0] * -OriginalSliderSize) / 100);
		else
		if (OriginalSliderSize < 0 && SliderType == SliderType_Vertical)
			SliderSize = (int)((Scene->getScreenSize().y / Scene->getVirtualTransform().m[1][1] * -OriginalSliderSize) / 100);
	}

}

/**
 @fn	bool CzUISlider::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the actor.

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types. Note that as 
 CzUISlider is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param	property_name		Name of the property.
 @param [out]	prop	The property data that.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUISlider::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	if (CzUIBase::getProperty(property_name, prop))
		return true;

	int err = IzXomlResource::getProperty(UISliderClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UISliderClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Slider - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUISlider::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the UISlider.
    
 Sets the named property of this UISlider. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzUISlider::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	if (CzUIBase::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(UISliderClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UISliderClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Slider - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUISlider::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced. Note that as CzUISlider is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in]	data	The property data.
 @param	delta				true to property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUISlider::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	if (CzUIBase::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(UISliderClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UISliderClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Slider - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUISlider::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates actor property from supplied animation.

 Updates the actors property that is specified by the supplied animation. When a timeline animation targets the property of an actor this method will be called by the animation to move the 
 animations frame data into its target property. Note that as CzUISlider is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param [in]	animation	If non-null, the animation.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUISlider::UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzUIBase::UpdateFromAnimation(animation))
		return true;

	int err = IzXomlResource::setProperty(UISliderClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UISliderClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Slider - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUISlider::UpdateBinding(unsigned int property_name, CzXomlVariable* var)

 @brief	Updates an actor property from the supplied XOML variable.

 Updates a single property from the value of the bound XOML variable. Note that as CzUISlider is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param	property_name  	Name of the property.
 @param [in,out]	var	If non-null, the variable.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUISlider::UpdateBinding(unsigned int property_name, CzXomlVariable* var)
{
	// Two way binding for Value (passes the value back to the variable)
	if (ThumbChanged)
	{
		if (property_name == CzHashes::Value_Hash)
		{
			if (var->Type == VT_Float)
				var->setValue(CzString(Value).c_str());			// Write Value back to bound variable
			else
			if (var->Type == VT_Int)
				var->setValue(CzString((int)Value).c_str());	// Write Value back to bound variable
			return true;
		}
	}

	if (!var->Modified || var->getValue().isEmpty())
		return false;
	if (CzUIBase::UpdateBinding(property_name, var))
		return true;

	int err = IzXomlResource::setProperty(UISliderClassDef, this, property_name, var);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UISliderClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Slider - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

