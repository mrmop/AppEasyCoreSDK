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

#include "CzCamera.h"
#include "CzResources.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"

CzXomlClassDef* CzCamera::CameraClassDef = NULL;

/**
 @fn	void CzCamera::InitClass()

 @brief	Initialises the class.
    
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzCamera::InitClass()
{
	// Create class properties
	CameraClassDef = new CzXomlClassDef();
	CameraClassDef->addProperty(new CzXomlClassProperty("Position",				PT_Vec2,		CzCamera::_setPosition,					CzCamera::_getPosition));
	CameraClassDef->addProperty(new CzXomlClassProperty("Velocity",				PT_Vec2,		CzCamera::_setVelocity,					CzCamera::_getVelocity));
	CameraClassDef->addProperty(new CzXomlClassProperty("VelocityDamping",		PT_Vec2,		CzCamera::_setVelocityDamping,			CzCamera::_getVelocityDamping));
	CameraClassDef->addProperty(new CzXomlClassProperty("Angle",				PT_Float,		CzCamera::_setAngle,					CzCamera::_getAngle));
	CameraClassDef->addProperty(new CzXomlClassProperty("Scale",				PT_Float,		CzCamera::_setScale,					CzCamera::_getScale));
	CameraClassDef->addProperty(new CzXomlClassProperty("TouchPanX",			PT_Bool,		CzCamera::_setTouchPanX,				CzCamera::_getTouchPanX));
	CameraClassDef->addProperty(new CzXomlClassProperty("TouchPanY",			PT_Bool,		CzCamera::_setTouchPanY,				CzCamera::_getTouchPanY));
	CameraClassDef->addProperty(new CzXomlClassProperty("IgnoreActors",			PT_Bool,		CzCamera::_setIgnoreActors,				CzCamera::_getIgnoreActors));
	CameraClassDef->addProperty(new CzXomlClassProperty("TargetX",				PT_Resource,	CzCamera::_setTargetX,					NULL));
	CameraClassDef->addProperty(new CzXomlClassProperty("TargetY",				PT_Resource,	CzCamera::_setTargetY,					NULL));
	CameraClassDef->addProperty(new CzXomlClassProperty("FollowSpeed",			PT_Vec2,		CzCamera::_setFollowSpeed,				CzCamera::_getFollowSpeed));
	CameraClassDef->addProperty(new CzXomlClassProperty("Tag",					PT_String,		CzCamera::_setTag,						NULL));
}
void CzCamera::ReleaseClass()
{
	if (CameraClassDef != NULL)
	{
		CameraClassDef->Release();
		delete CameraClassDef;
		CameraClassDef = NULL;
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

bool CzCamera::_setName(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	if (add)
	{
		CzString name = ((CzCamera*)target)->getName();
		name += (const char*)prop.p_data;
		((CzCamera*)target)->setName(name.c_str());
	}
	else
		((CzCamera*)target)->setName((const char*)prop.p_data);

	return true;
}

CzXomlProperty CzCamera::_getName(IzXomlResource* target)
{
	return CzXomlProperty(((CzCamera*)target)->getName().c_str());
}

bool CzCamera::_setTag(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	((CzCamera*)target)->setTag((const char*)prop.p_data);

	return true;
}

bool CzCamera::_setPosition(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzCamera* cam = (CzCamera*)target;
	if (add)
	{
		CzVec2 pos = cam->getPosition();
		pos.x += prop.p_vec[0];
		pos.y += prop.p_vec[1];
		cam->setPosition(pos.x, pos.y);
	}
	else
		cam->setPosition(prop.p_vec[0], prop.p_vec[1]);

	return true;
}

CzXomlProperty CzCamera::_getPosition(IzXomlResource* target)
{
	return CzXomlProperty(((CzCamera*)target)->getPosition());
}

bool CzCamera::_setScale(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzCamera* cam = (CzCamera*)target;
	if (add)
		cam->setScale(cam->getScale() + prop.p_float);
	else
		cam->setScale(prop.p_float);

	return true;
}

CzXomlProperty CzCamera::_getScale(IzXomlResource* target)
{
	return CzXomlProperty(((CzCamera*)target)->getScale());
}

bool CzCamera::_setAngle(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzCamera* cam = (CzCamera*)target;
	if (add)
		cam->setAngle(cam->getAngle() + prop.p_float);
	else
		cam->setAngle(prop.p_float);

	return true;
}

CzXomlProperty CzCamera::_getAngle(IzXomlResource* target)
{
	return CzXomlProperty(((CzCamera*)target)->getAngle());
}

bool CzCamera::_setVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzCamera* cam = (CzCamera*)target;
	if (add)
	{
		CzVec2 pos = cam->getVelocity();
		pos.x += prop.p_vec[0];
		pos.y += prop.p_vec[1];
		cam->setVelocity(pos.x, pos.y);
	}
	else
		cam->setVelocity(prop.p_vec[0], prop.p_vec[1]);

	return true;
}

CzXomlProperty CzCamera::_getVelocity(IzXomlResource* target)
{
	return CzXomlProperty(((CzCamera*)target)->getVelocity());
}

bool CzCamera::_setVelocityDamping(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzCamera* cam = (CzCamera*)target;
	if (add)
	{
		CzVec2 pos = cam->getVelocityDamping();
		pos.x += prop.p_vec[0];
		pos.y += prop.p_vec[1];
		cam->setVelocityDamping(pos.x, pos.y);
	}
	else
		cam->setVelocityDamping(prop.p_vec[0], prop.p_vec[1]);

	return true;
}

CzXomlProperty CzCamera::_getVelocityDamping(IzXomlResource* target)
{
	return CzXomlProperty(((CzCamera*)target)->getVelocityDamping());
}

bool CzCamera::_setTouchPanX(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzCamera* cam = (CzCamera*)target;
	if (add)
		cam->setTouchPanX(!cam->isTouchPanX());
	else
		cam->setTouchPanX(prop.p_bool);

	return true;
}

CzXomlProperty CzCamera::_getTouchPanX(IzXomlResource* target)
{
	return CzXomlProperty(((CzCamera*)target)->isTouchPanX());
}

bool CzCamera::_setTouchPanY(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzCamera* cam = (CzCamera*)target;
	if (add)
		cam->setTouchPanY(!cam->isTouchPanY());
	else
		cam->setTouchPanY(prop.p_bool);

	return true;
}

CzXomlProperty CzCamera::_getTouchPanY(IzXomlResource* target)
{
	return CzXomlProperty(((CzCamera*)target)->isTouchPanY());
}

bool CzCamera::_setIgnoreActors(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzCamera* cam = (CzCamera*)target;
	if (add)
		cam->setIgnoreActors(!cam->getIgnoreActors());
	else
		cam->setIgnoreActors(prop.p_bool);

	return true;
}

CzXomlProperty CzCamera::_getIgnoreActors(IzXomlResource* target)
{
	return CzXomlProperty(((CzCamera*)target)->getIgnoreActors());
}

bool CzCamera::_setTargetX(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzCamera* cam = (CzCamera*)target;
	if (prop.Type == PT_Resource)
		cam->setTargetX((CzActor*)prop.p_data);
	else
		cam->setTargetX((const char*)prop.p_data);

	return true;
}

bool CzCamera::_setTargetY(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzCamera* cam = (CzCamera*)target;
	if (prop.Type == PT_Resource)
		cam->setTargetY((CzActor*)prop.p_data);
	else
		cam->setTargetY((const char*)prop.p_data);

	return true;
}

bool CzCamera::_setFollowSpeed(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzCamera* cam = (CzCamera*)target;
	if (add)
	{
		CzVec2 p = cam->getFollowSpeed();
		p.x += prop.p_vec[0];
		p.y += prop.p_vec[1];
		cam->setFollowSpeed(p.x, p.y);
	}
	else
		cam->setFollowSpeed(prop.p_vec[0], prop.p_vec[1]);

	return true;
}

CzXomlProperty CzCamera::_getFollowSpeed(IzXomlResource* target)
{
	return CzXomlProperty(((CzCamera*)target)->getFollowSpeed());
}

/**
 @fn	void CzCamera::setTargetX(const char* target_name)

 @brief	Sets target x-axis actor for camera to follow on its x-axis.

 @param	target_name	Name of the target.
 */

void CzCamera::setTargetX(const char* target_name)
{
	TargetXHash = CzString::CalculateHash(target_name);
	if (Scene != NULL)
		TargetX = Scene->findActor(TargetXHash);
}

/**
 @fn	void CzCamera::setTargetY(const char* target_name)

 @brief	Sets target y-axis actor for camera to follow on its y-axis.

 @param	target_name	Name of the target.
 */

void CzCamera::setTargetY(const char* target_name)
{
	TargetYHash = CzString::CalculateHash(target_name);
	if (Scene != NULL)
		TargetY = Scene->findActor(TargetYHash);
}

/**
 @fn	void CzCamera::setTargetX(CzActor* target)

 @brief	Sets target x-axis actor for camera to follow on its x-axis.

 @param [in]	target	If non-null, target for the.
 */

void CzCamera::setTargetX(CzActor* target)
{
	TargetX = target; 
	TargetXHash = target->getNameHash();
}

/**
 @fn	void CzCamera::setTargetY(CzActor* target)

 @brief	Sets target y-axis actor for camera to follow on its y-axis.

 @param [in]	target	If non-null, target for the.
 */

void CzCamera::setTargetY(CzActor* target)
{
	TargetY = target;
	TargetYHash = target->getNameHash();
}

/**
 @fn	void CzCamera::Update(float dt)

 @brief	Updates the given dt.

 @param	dt	The number of seconds since the app was last updated
 */

void CzCamera::Update(float dt)
{
	CzVec2 old_pos = Position;

	// If targetting an actor then follow it
	if (Scene != NULL)
	{
		// Cache the target actors so we dont have to keep searching for them
		if (TargetXHash != 0)
		{
			if (TargetX == NULL)
				TargetX = Scene->findActor(TargetXHash);
			if (TargetX != NULL)
			{
				if (FollowSpeed.x == 0)
				{
					Position.x = TargetX->getPosition().x;
					Velocity.x = 0;
				}
				else
					Velocity.x += (TargetX->getPosition().x - Position.x) * FollowSpeed.x;
			}
		}
		if (TargetYHash != 0)
		{
			if (TargetXHash != TargetYHash)
			{
				if (TargetY == NULL)
					TargetY = Scene->findActor(TargetYHash);
			}
			else
				TargetY = TargetX;
			if (TargetY != NULL)
			{
				if (FollowSpeed.y == 0)
				{
					Position.y = TargetY->getPosition().y;
					Velocity.y = 0;
				}
				else
					Velocity.y += (TargetY->getPosition().y - Position.y) * FollowSpeed.y;
			}
		}
	}

	// Apply camera velocity
	Position += Velocity * dt;
	Velocity.x *= VelocityDamping.x;
	Velocity.y *= VelocityDamping.y;
	if (Velocity.x > -0.01f && Velocity.x < 0.01f)
		Velocity.x = 0;
	if (Velocity.y > -0.01f && Velocity.y < 0.01f)
		Velocity.y = 0;

	if (old_pos.x != Position.x || old_pos.y != Position.y)
		TransformDirty = true;

	if (TransformDirty)
	{
		// Update camera transform
		CzMatrix3 scale;
		scale.Scale(Scale, Scale);
		Transform.Rotate(Angle);
		Transform.MultiplyPost(&scale);
		Transform.TranslateSet(-Position.x, -Position.y);
	}
}

/**
 @fn	int CzCamera::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)

 @brief	Creates an instance of this class from XOML

 LoadFromXoml is the main method used by all classes that can be instantiated from XOML mark-up. This method creates an instance of this class from the supplied XML node structure specified by node. 
 if parent is not NULL then the created instance will utilise the supplied parent to determine where it should live within the app. For example, if the parent is a scene then the camera will be placed 
 in that scene. If load_chlldren is true then any child nodes contained within the node will also be loaded.

 @param [in]	parent		If non-null, the parent.
 @param	load_children	  	true to load child nodes.
 @param [in]	node  		If non-null, the XOML markup node that defines this object

 @return	nergative value for error. if 0 then processing of further XOML will stop.
 */

int CzCamera::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Process attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
			setName((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Position_Hash)
		{
			if (!(*it)->getValueAsPoint(Position))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Camera - Invalid value for Camera::Position", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::Angle_Hash)
			Angle = (*it)->getValueAsFloat();
		else
		if (name_hash == CzHashes::Scale_Hash)
			Scale = (*it)->getValueAsFloat();
		else
		if (name_hash == CzHashes::TargetX_Hash)
			setTargetX((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::TargetY_Hash)
			setTargetY((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::VelocityDamping_Hash)
		{
			if (!(*it)->getValueAsPoint(VelocityDamping))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Camera - Invalid value for Camera::VelocityDamping", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::FollowSpeed_Hash)
		{
			if (!(*it)->getValueAsPoint(FollowSpeed))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Camera - Invalid value for Camera::FollowSpeed", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::TouchPanX_Hash)
			setTouchPanX((*it)->getValueAsBool());
		else
		if (name_hash == CzHashes::TouchPanY_Hash)
			setTouchPanY((*it)->getValueAsBool());
		else
		if (name_hash == CzHashes::IgnoreActors_Hash)
			setIgnoreActors((*it)->getValueAsBool());
	}

	// If we are declared inside a scene then shape is local to the scene
	CzScene* scene = NULL;
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Actor_Hash)
		scene = ((CzActor*)parent)->getScene();
	else
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;
	Scene = scene;

	bool ret = false;
	if (scene != NULL)
		ret = scene->getResourceManager()->addResource(this);
	else
		ret = CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this);

	if (!ret)
		return 0;

	Update(0);

	return 1;
}

/**
 @fn	bool CzCamera::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the actor.

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types.

 @param	property_name		Name of the property.
 @param [out]	prop	The property data.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzCamera::getProperty(const char* property_name, CzXomlProperty& prop)
{
	return getProperty(CZ_HASH(property_name), prop);
}

/**
 @fn	bool CzCamera::getProperty(unsigned int property_name, CzXomlProperty& prop)

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in,out]	prop	The property.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzCamera::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	int err = IzXomlResource::getProperty(CameraClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = CameraClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Camera - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzCamera::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the Camera.
    
 Sets the named property of this Camera. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzCamera::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	int err = IzXomlResource::setProperty(CameraClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = CameraClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Camera - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzCamera::setProperty(const char* property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced.

 @param	property_name		Name of the property.
 @param [in]	data	The property data.
 @param	delta				true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzCamera::setProperty(const char* property_name, const CzString& data, bool delta)
{
	return setProperty(CzString::CalculateHash(property_name), data, delta);
}

/**
 @fn	bool CzCamera::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.
    
 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzCamera::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	int err = IzXomlResource::setProperty(CameraClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = CameraClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Camera - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}