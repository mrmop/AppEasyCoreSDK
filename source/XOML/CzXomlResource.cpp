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

#include "CzXoml.h"
#include "CzHashes.h"
#include "CzUtil.h"
#include "CzString.h"
#include "CzAnim.h"
#include "CzXomlVariables.h"

//
//
//
//
// IzXomlResource implementation
//
//
//
//
/**
 @fn	void IzXomlResource::setName(const char* name)

 @brief	Sets the name of the resource. Note that resource names should be unique across the same resource type. For example, two images should not share the same name.

 @param	name	Resource name.
 */

void IzXomlResource::setName(const char* name)
{
//#if defined (_DEBUG)
	Name = name;
//#endif	// _DEBUG
	NameHash = CzString::CalculateHash(name);
}

/**
 @fn	void IzXomlResource::setClassType(const char* type_name)

 @brief	Sets the class type. 

 The class type is used to dtermine what type of category the class belongs to  (e.g. Scene, Actor, Animation, Game etc..)

 @param	type_name	Class tyoe name.
 */

void IzXomlResource::setClassType(const char* type_name)
{
#if defined (_DEBUG)
	ClassType = type_name;
	ActualClassType = type_name;
#endif	// _DEBUG
	ClassTypeHash = CzString::CalculateHash(type_name);
	ActualClassTypeHash = ClassTypeHash;
}

/**
 @fn	void IzXomlResource::setActualClassType(const char* type_name)

 @brief	Sets the actual class type.

 The actual class type is the actual name of the class without the Cz. For example CzAxtorImage would be actually called ActorImage and this is how it would be 
 referred to in XOML.

 @param	type_name	Name of the type.
 */

void IzXomlResource::setActualClassType(const char* type_name)
{
#if defined (_DEBUG)
	ActualClassType = type_name;
#endif	// _DEBUG
	ActualClassTypeHash = CzString::CalculateHash(type_name);
}

/**
 @fn	void IzXomlResource::setTag(const char* Tag)

 @brief	Sets the resource group tag.

 Its often useful to be able to determine which group certain resources belong to. For example, you may load a group of resources into the global resource manager 
 but later want to remove them. To save having to individually remove each resource you could remove all of those that fit into a specific group.

 @param	Tag	The tag.
 */

void IzXomlResource::setTag(const char* Tag)
{
#if defined (_DEBUG)
	Tag = Tag;
#endif	// _DEBUG
	TagHash = CzString::CalculateHash(Tag);
}

/**
 @fn	bool IzXomlResource::setProperty(CzXomlClassDef* class_def, IzXomlResource* target,
    unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the specified target resource.
    
 Sets the named property of this object. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property will be
 added to instead of replaced.

 @param [in]					class_def	If non-null, class definition used to determine properties
 @param [in,out]	target   	If non-null, target resource that contains the property.
 @param	property_name		 	Name of the target element as a string hash (faster searching).
 @param [in]	data		 	The data to set to the property.
 @param	delta				 	true if property should be added to instead of replaced.

 @return	See eSetPropertyError for list of possible return values
 */

int IzXomlResource::setProperty(CzXomlClassDef* class_def, IzXomlResource* target, unsigned int property_name, const CzString& data, bool delta)
{
	float	float_pool[8];

	CzXomlClassProperty* cprop = class_def->findProperty(property_name);
	if (cprop == NULL)
		return NotFound;
	if (cprop->getSetter() == NULL)
		return NoAccess;

	// Convert supplied property data from string to expected property type
	CzXomlProperty prop;
	prop.Type = cprop->getType();
	switch (cprop->getType())
	{
	case PT_String:
		prop.p_data = (void*)data.c_str();
		break;
	case PT_Bool:
		prop.p_bool = data.getAsBool();
		break;
	case PT_Float:
		prop.p_float = data.getAsFloat();
		break;
	case PT_Int:
		prop.p_int = data.getAsInt();
		break;
	case PT_Vec2:
		if (data.getAsListOfFloat(float_pool) != 2)
			return IncompatibleTypes;
		else
		{
			prop.p_vec[0] = float_pool[0];
			prop.p_vec[1] = float_pool[1];
		}
		break;
	case PT_Vec3:
		if (data.getAsListOfFloat(float_pool) != 3)
			return IncompatibleTypes;
		else
		{
			prop.p_vec[0] = float_pool[0];
			prop.p_vec[1] = float_pool[1];
			prop.p_vec[2] = float_pool[2];
		}
		break;
	case PT_Vec4:
		if (data.getAsListOfFloat(float_pool) != 4)
			return IncompatibleTypes;
		else
		{
			prop.p_vec[0] = float_pool[0];
			prop.p_vec[1] = float_pool[1];
			prop.p_vec[2] = float_pool[2];
			prop.p_vec[3] = float_pool[3];
		}
		break;
	case PT_Resource:
		prop.p_data = (void*)data.c_str();
		prop.Type = PT_String;
		if (cprop->getSetter()(target, prop, delta) == PT_Invalid)
			return IncompatibleTypes;
		return SearchForResource;
		break;
	default:
		return IncompatibleTypes;
		break;
	}

	// Set the property
	if (cprop->getSetter()(target, prop, delta) == PT_Invalid)
		return IncompatibleTypes;

	return Success;
}

/**
 @fn	bool IzXomlResource::setProperty(CzXomlClassDef* class_def, IzXomlResource* target,
    unsigned int property_name, const CzXomlProperty& prop, bool delta)

 @brief	Sets the named property of the specified target resource.
    
 Sets the named property of this object. The properties value (data) is supplied as a property and will be converted. If delta is set to true then the existing value of the property will be
 added to instead of replaced. if the class property is of type PT_Resource and the supplied property value is of type string then the this method will return SearchForResource which informs
 the caller to take the supplied property value as a string that contains the named string and shoudl search for it
 
 Note that only the following automatic type conversions are supported:
 - string - any type can be converted to string
 - bool - string, int and float
 - int - string, bool and float
 - float - string, bool and int
 - vec2 - string, vec3 and vec4
 - vec3 - string, vec4
 - vec4 - string

 @param [in]	class_def		If non-null, class definition used to determine properties.
 @param [in,out]	target   	If non-null, target resource that contains the property.
 @param	property_name		 	Name of the target element as a string hash (faster searching).
 @param [in]	prop		 	The data to set to the property.
 @param	delta				 	true if property should be added to instead of replaced.

 @return	See eSetPropertyError for list of possible return values
 */

int IzXomlResource::setProperty(CzXomlClassDef* class_def, IzXomlResource* target, unsigned int property_name, const CzXomlProperty& prop, bool delta)
{
	CzXomlClassProperty* cprop = class_def->findProperty(property_name);
	if (cprop == NULL)
		return NotFound;
	if (cprop->getSetter() == NULL)
		return NoAccess;

	// Get property types
	eCzXomlPropertyType ctype = cprop->getType();
	eCzXomlPropertyType type = prop.Type;

	// If target property type is resource and supplied type is string then we expect the Setter to perform a search for the named resource
	if (ctype == PT_Resource && type == PT_String)
	{
		if (cprop->getSetter()(target, prop, delta) == PT_Invalid)
			return IncompatibleTypes;
		return SearchForResource;
	}

	if (ctype != type)
	{
		// Convert property from supplied property type to the expected property type
		CzXomlProperty p;
		p.Type = ctype;
		CzString s;
		if (ctype == PT_String)
		{
			if (type == PT_Int)
				s.set(prop.p_int);
			else
			if (type == PT_Float)
			{
				if (prop.p_float == (int)prop.p_float)
					s.set((int)prop.p_float);
				else
					s.set(prop.p_float);
			}
			else
			if (type == PT_Bool)
				s.set(prop.p_bool);
			else
			if (type == PT_Vec2)
			{
				CzVec2 v = CzVec2(prop.p_vec[0], prop.p_vec[1]);
				s.set(v);
			}
			else
			if (type == PT_Vec3)
			{
				CzVec3 v = CzVec3(prop.p_vec[0], prop.p_vec[1], prop.p_vec[2]);
				s.set(v);
			}
			else
			if (type == PT_Vec4)
			{
				CzVec4 v = CzVec4(prop.p_vec[0], prop.p_vec[1], prop.p_vec[2], prop.p_vec[3]);
				s.set(v);
			}
			else
				return IncompatibleTypes;
			p.p_data = (void *)s.c_str();
		}
		else
		if (ctype == PT_Bool)
		{
			if (type == PT_String)
				p.p_bool = CzString((const char*)prop.p_data).getAsBool();
			else
			if (type == PT_Int)
				p.p_bool = prop.p_int > 0;
			else
			if (type == PT_Float)
				p.p_bool = prop.p_float > 0;
			else
				return IncompatibleTypes;
		}
		else
		if (ctype == PT_Float)
		{
			if (type == PT_String)
				p.p_float = CzString((const char*)prop.p_data).getAsFloat();
			else
			if (type == PT_Bool)
				p.p_float = prop.p_bool ? 1.0f : 0;
			else
			if (type == PT_Int)
				p.p_float = (float)prop.p_int;
			else
				return IncompatibleTypes;
		}
		else
		if (ctype == PT_Int)
		{
			if (type == PT_String)
				p.p_int = CzString((const char*)prop.p_data).getAsInt();
			else
			if (type == PT_Bool)
				p.p_int = prop.p_bool ? 1 : 0;
			else
			if (type == PT_Float)
				p.p_int = (int)prop.p_float;
			else
				return IncompatibleTypes;
		}
		else
		if (ctype == PT_Vec2)
		{
			if (type == PT_String)
			{
				float floats[8];
				CzString s = (const char*)prop.p_data;
				s.getAsListOfFloat(floats);
				p.p_vec[0] = floats[0];
				p.p_vec[1] = floats[1];
			}
			else
			if (type == PT_Vec3)
			{
				p.p_vec[0] = prop.p_vec[0];
				p.p_vec[1] = prop.p_vec[1];
			}
			else
			if (type == PT_Vec4)
			{
				p.p_vec[0] = prop.p_vec[0];
				p.p_vec[1] = prop.p_vec[1];
			}
			else
				return IncompatibleTypes;
		}
		else
		if (ctype == PT_Vec3)
		{
			if (type == PT_String)
			{
				float floats[8];
				CzString s = (const char*)prop.p_data;
				s.getAsListOfFloat(floats);
				p.p_vec[0] = floats[0];
				p.p_vec[1] = floats[1];
				p.p_vec[2] = floats[2];
			}
			else
			if (type == PT_Vec4)
			{
				p.p_vec[0] = prop.p_vec[0];
				p.p_vec[1] = prop.p_vec[1];
				p.p_vec[2] = prop.p_vec[2];
			}
			else
				return IncompatibleTypes;
		}
		else
		if (ctype == PT_Vec4)
		{
			if (type == PT_String)
			{
				float floats[8];
				CzString s = (const char*)prop.p_data;
				s.getAsListOfFloat(floats);
				p.p_vec[0] = floats[0];
				p.p_vec[1] = floats[1];
				p.p_vec[2] = floats[2];
				p.p_vec[3] = floats[3];
			}
			else
				return IncompatibleTypes;
		}
		else
			return IncompatibleTypes;

		if (cprop->getSetter()(target, p, delta) == PT_Invalid)
			return IncompatibleTypes;
	}
	else
	{
		if (cprop->getSetter()(target, prop, delta) == PT_Invalid)
			return IncompatibleTypes;
	}

	return Success;
}

/**
 @fn	int IzXomlResource::setProperty(CzXomlClassDef* class_def, IzXomlResource* target,
    unsigned int property_name, CzXomlVariable* variable)

 @brief	Sets the named property of the specified target resource from a variable.
    
 Sets the named property of this object. The properties value (data) is supplied as a variable and will be converted. If delta is set to true then the existing value of the property will be
 added to instead of replaced.

  Note that only the following automatic type conversions are supported:
 - string - any type can be converted to string
 - bool - string, int and float
 - int - string, bool and float
 - float - string, bool and int
 - vec2 - string, vec3 and vec4
 - vec3 - string and vec4
 - vec4 - string

@param [in]	class_def		If non-null, class definition used to determine properties.
 @param [in,out]	target   	If non-null, target resource that contains the property.
 @param	property_name		 	Name of the target element as a string hash (faster searching).
 @param [in,out]	variable 	If non-null, the suorce variable.

 @return	See eSetPropertyError for list of possible return values
 */

int IzXomlResource::setProperty(CzXomlClassDef* class_def, IzXomlResource* target, unsigned int property_name, CzXomlVariable* variable)
{
	CzXomlClassProperty* cprop = class_def->findProperty(property_name);
	if (cprop == NULL)
		return NotFound;
	if (cprop->getSetter() == NULL)
		return NoAccess;

	CzXomlProperty p;
	eCzXomlPropertyType ctype = cprop->getType();
	eCzXomlVariableType type = variable->Type;

	// Convert supplied variable to a property
	CzString s;
	if (ctype == PT_String)
	{
		p.Type = PT_String;
		p.p_data = (void *)variable->getValue().c_str();
	}
	else
	if (ctype == PT_Bool)
	{
		p.Type = PT_Bool;
		if (type == VT_Bool)
			p.p_bool = ((CzXomlVariableBool*)variable)->NativeValue;
		else
		if (type == VT_Int)
			p.p_bool = ((CzXomlVariableInt*)variable)->NativeValue != 0;
		else
		if (type == VT_Float)
			p.p_bool = ((CzXomlVariableFloat*)variable)->NativeValue != 0;
		else
		if (type == VT_String)
			p.p_bool = variable->getValue().getAsBool();
		else
			return IncompatibleTypes;
	}
	else
	if (ctype == PT_Float)
	{
		p.Type = PT_Float;
		if (type == VT_Float)
			p.p_float = ((CzXomlVariableFloat*)variable)->NativeValue;
		else
		if (type == VT_Int)
			p.p_float = (float)((CzXomlVariableInt*)variable)->NativeValue;
		else
		if (type == VT_Bool)
			p.p_float = (float)((CzXomlVariableBool*)variable)->NativeValue;
		else
		if (type == VT_String)
			p.p_float = variable->getValue().getAsFloat();
		else
			return IncompatibleTypes;
	}
	else
	if (ctype == PT_Int)
	{
		p.Type = PT_Int;
		if (type == VT_Int)
			p.p_int = ((CzXomlVariableInt*)variable)->NativeValue;
		else
		if (type == VT_Float)
			p.p_int = (int)((CzXomlVariableFloat*)variable)->NativeValue;
		else
		if (type == VT_Bool)
			p.p_int = (int)((CzXomlVariableBool*)variable)->NativeValue;
		else
		if (type == VT_String)
			p.p_int = variable->getValue().getAsInt();
		else
			return IncompatibleTypes;
	}
	else
	if (ctype == PT_Vec2)
	{
		p.Type = PT_Vec2;
		if (type == VT_Vec2)
		{
			CzXomlVariableVec2* var = (CzXomlVariableVec2*)variable;
			p.p_vec[0] = var->NativeValue.x;
			p.p_vec[1] = var->NativeValue.y;
		}
		else
		if (type == VT_Vec3)
		{
			CzXomlVariableVec3* var = (CzXomlVariableVec3*)variable;
			p.p_vec[0] = var->NativeValue.x;
			p.p_vec[1] = var->NativeValue.y;
		}
		else
		if (type == VT_Vec4)
		{
			CzXomlVariableVec4* var = (CzXomlVariableVec4*)variable;
			p.p_vec[0] = var->NativeValue.x;
			p.p_vec[1] = var->NativeValue.y;
		}
		else
		if (type == VT_String)
		{
			float float_pool[8];
			int num = variable->getValue().getAsListOfFloat(float_pool);
			if (num > 2) num = 2;
			for (int t = 0; t < num; t++)
				p.p_vec[t] = float_pool[t];
		}
		else
			return IncompatibleTypes;
	}
	else
	if (ctype == PT_Vec3)
	{
		p.Type = PT_Vec3;
		if (type == VT_Vec3)
		{
			CzXomlVariableVec3* var = (CzXomlVariableVec3*)variable;
			p.p_vec[0] = var->NativeValue.x;
			p.p_vec[1] = var->NativeValue.y;
			p.p_vec[2] = var->NativeValue.z;
		}
		else
		if (type == VT_Vec4)
		{
			CzXomlVariableVec4* var = (CzXomlVariableVec4*)variable;
			p.p_vec[0] = var->NativeValue.x;
			p.p_vec[1] = var->NativeValue.y;
			p.p_vec[2] = var->NativeValue.z;
		}
		else
		if (type == VT_String)
		{
			float float_pool[8];
			int num = variable->getValue().getAsListOfFloat(float_pool);
			if (num > 3) num = 3;
			for (int t = 0; t < num; t++)
				p.p_vec[t] = float_pool[t];
		}
		else
			return IncompatibleTypes;
	}
	else
	if (ctype == PT_Vec3)
	{
		p.Type = PT_Vec4;
		if (type == VT_Vec4)
		{
			CzXomlVariableVec4* var = (CzXomlVariableVec4*)variable;
			p.p_vec[0] = var->NativeValue.x;
			p.p_vec[1] = var->NativeValue.y;
			p.p_vec[2] = var->NativeValue.z;
			p.p_vec[3] = var->NativeValue.w;
		}
		else
		if (type == VT_String)
		{
			float float_pool[8];
			int num = variable->getValue().getAsListOfFloat(float_pool);
			if (num > 4) num = 4;
			for (int t = 0; t < num; t++)
				p.p_vec[t] = float_pool[t];
		}
	}
	else
		return IncompatibleTypes;

	// Set the property
	if (cprop->getSetter()(target, p, false) == PT_Invalid)
		return IncompatibleTypes;

	return Success;
}

/**
 @fn	int IzXomlResource::getProperty(CzXomlClassDef* class_def, IzXomlResource* target,
    unsigned int property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the resource.
    
 Returns the named property of the scene. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types.

 @param [in]	class_def		If non-null, class definition used to determine properties.
 @param [in]	target   		If non-null, target resource that contains the property.
 @param	property_name		 	Name of the target element as a string hash (faster searching).
 @param [out]	prop		 	The output property.

 @return	See eSetPropertyError for list of possible return values
 */

int IzXomlResource::getProperty(CzXomlClassDef* class_def, IzXomlResource* target, unsigned int property_name, CzXomlProperty& prop)
{
	CzXomlClassProperty* cprop = class_def->findProperty(property_name);
	if (cprop == NULL)
		return NotFound;
	if (cprop->getGetter() == NULL)
		return NoAccess;
	
	prop = cprop->getGetter()(target);
	if (prop.Type == PT_Invalid)
		return IncompatibleTypes;

	return Success;
}

/**
 @fn	bool IzXomlResource::setProperty(CzXomlClassDef* class_def, IzAnimTarget* target,
    CzAnimInstance *animation)

 @brief	Updates property from supplied animation.
    
 Sets the property that is specified by the supplied animation. When a timeline animation targets the property of the object this method will be called by the animation to move the
 animations frame data into its target property.

  Note that only the following automatic type conversions are supported:
 - string - any type can be converted to string
 - bool - int and float
 - int - bool and float
 - float - bool and int
 - vec2 - vec3 and vec4
 - vec3 - vec4
 - vec4 - none

    
 @param [in,out]	class_def	If non-null, class definition used to determine properties.
 @param [in,out]	target   	If non-null, target resource that contains the property.
 @param [in]	animation	 	If non-null, the animation.

 @return	See eSetPropertyError for list of possible return values
 */

int IzXomlResource::setProperty(CzXomlClassDef* class_def, IzAnimTarget* target, CzAnimInstance *animation)
{
	unsigned int property_name = animation->getTargetPropertyHash();
	CzXomlClassProperty* cprop = class_def->findProperty(property_name);
	if (cprop == NULL)
		return NotFound;
	if (cprop->getSetter() == NULL)
		return NoAccess;

	bool delta = animation->isDelta();

	CzAnimFrame::eAnimType anim_type = animation->getAnimation()->getType();
	eCzXomlPropertyType type = cprop->getType();
	CzXomlProperty prop;

	switch (type)
	{
	case PT_Bool:
		prop.Type = PT_Bool;
		if (anim_type == CzAnimFrame::FT_Bool)
			prop.p_bool = ((CzAnimFrameBool*)animation->getCurrentData())->data;
		else
		if (anim_type == CzAnimFrame::FT_Float)
			prop.p_bool = ((CzAnimFrameFloat*)animation->getCurrentData())->data > 0;
		else
			return IncompatibleTypes;
		break;
	case PT_Float:
		prop.Type = PT_Float;
		if (anim_type == CzAnimFrame::FT_Float)
			prop.p_float = ((CzAnimFrameFloat*)animation->getCurrentData())->data;
		else
		if (anim_type == CzAnimFrame::FT_Bool)
			prop.p_float = ((CzAnimFrameBool*)animation->getCurrentData())->data ? 1.0f : 0;
		else
			return IncompatibleTypes;
		break;
	case PT_Vec2:
		{
			prop.Type = PT_Vec2;
			if (anim_type == CzAnimFrame::FT_Vec2)
			{
				CzAnimFrameVec2* frame = (CzAnimFrameVec2*)animation->getCurrentData();
				prop.p_vec[0] = frame->data.x;
				prop.p_vec[1] = frame->data.y;
			}
			else
			if (anim_type == CzAnimFrame::FT_Vec3)
			{
				CzAnimFrameVec3* frame = (CzAnimFrameVec3*)animation->getCurrentData();
				prop.p_vec[0] = frame->data.x;
				prop.p_vec[1] = frame->data.y;
			}
			else
			if (anim_type == CzAnimFrame::FT_Vec4)
			{
				CzAnimFrameVec4* frame = (CzAnimFrameVec4*)animation->getCurrentData();
				prop.p_vec[0] = frame->data.x;
				prop.p_vec[1] = frame->data.y;
			}
			else
				return IncompatibleTypes;
		}
		break;
	case PT_Vec3:
		{
			prop.Type = PT_Vec3;
			if (anim_type == CzAnimFrame::FT_Vec3)
			{
				CzAnimFrameVec3* frame = (CzAnimFrameVec3*)animation->getCurrentData();
				prop.p_vec[0] = frame->data.x;
				prop.p_vec[1] = frame->data.y;
				prop.p_vec[2] = frame->data.z;
			}
			else
			if (anim_type == CzAnimFrame::FT_Vec4)
			{
				CzAnimFrameVec4* frame = (CzAnimFrameVec4*)animation->getCurrentData();
				prop.p_vec[0] = frame->data.x;
				prop.p_vec[1] = frame->data.y;
				prop.p_vec[2] = frame->data.z;
			}
			else
				return IncompatibleTypes;
		}
		break;
	case PT_Vec4:
		{
			prop.Type = PT_Vec4;
			if (anim_type == CzAnimFrame::FT_Vec4)
			{
				CzAnimFrameVec4* frame = (CzAnimFrameVec4*)animation->getCurrentData();
				prop.p_vec[0] = frame->data.x;
				prop.p_vec[1] = frame->data.y;
				prop.p_vec[2] = frame->data.z;
				prop.p_vec[3] = frame->data.w;
			}
			else
			if (anim_type == CzAnimFrame::FT_Rect)
			{
				CzAnimFrameRect* frame = (CzAnimFrameRect*)animation->getCurrentData();
				prop.p_vec[0] = (float)frame->data.x;
				prop.p_vec[1] = (float)frame->data.y;
				prop.p_vec[2] = (float)frame->data.w;
				prop.p_vec[3] = (float)frame->data.h;
			}
			else
				return IncompatibleTypes;
		}
		break;
	case PT_String:
		if (type != PT_String)
			return IncompatibleTypes;
		prop.Type = PT_String;
		prop.p_data = (void*)((CzAnimFrameString*)animation->getCurrentData())->data.c_str();
		break;
	default:
		return IncompatibleTypes;
		break;
	}

	IzXomlResource* res = dynamic_cast<IzXomlResource*>(target);
	if (res == NULL)
		return IncompatibleTypes;

	if (cprop->getSetter()(res, prop, delta) == PT_Invalid)
		return IncompatibleTypes;

	return Success;
}

/**
 @fn	void IzXomlResource::Remove()

 @brief	Removes and destroys this resource.

 */
void IzXomlResource::Remove()
{
	if (Parent != NULL)
		Parent->removeResource(this);
	else
		delete this;
}
