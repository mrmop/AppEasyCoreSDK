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

#include "CzActorConnector.h"
#include "CzScene.h"
#include "CzHashes.h"
#include "CzMath.h"

CzXomlClassDef* CzActorConnector::ActorConnectorClassDef = NULL;

/**
 @fn	void CzActorConnector::InitClass()

 @brief	Initialises the class.
    
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzActorConnector::InitClass()
{
	// Create class properties
	ActorConnectorClassDef = new CzXomlClassDef();
	ActorConnectorClassDef->addProperty(new CzXomlClassProperty("TargetA",			PT_Resource,	CzActorConnector::_setTargetA,			CzActorConnector::_getTargetA));
	ActorConnectorClassDef->addProperty(new CzXomlClassProperty("TargetB",			PT_Resource,	CzActorConnector::_setTargetB,			CzActorConnector::_getTargetB));
	ActorConnectorClassDef->addProperty(new CzXomlClassProperty("OffsetA",			PT_Vec2,		CzActorConnector::_setOffsetA,			CzActorConnector::_getOffsetA));
	ActorConnectorClassDef->addProperty(new CzXomlClassProperty("OffsetB",			PT_Vec2,		CzActorConnector::_setOffsetB,			CzActorConnector::_getOffsetB));

}
void CzActorConnector::ReleaseClass()
{
	if (ActorConnectorClassDef != NULL)
	{
		ActorConnectorClassDef->Release();
		delete ActorConnectorClassDef;
		ActorConnectorClassDef = NULL;
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

bool CzActorConnector::_setTargetA(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorConnector* actor = (CzActorConnector*)target;
	CzActor* t;
	if (prop.Type == PT_Resource)
		t = ((CzActor*)prop.p_data);
	else
		t = actor->getScene()->findActor((const char*)prop.p_data);

	if (t != NULL)
		actor->setTargetA(t);
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - TargetA actor not found - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());
		return false;
	}

	return true;
}

CzXomlProperty CzActorConnector::_getTargetA(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorConnector*)target)->getTargetA());
}

bool CzActorConnector::_setTargetB(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorConnector* actor = (CzActorConnector*)target;
	CzActor* t;
	if (prop.Type == PT_Resource)
		t = ((CzActor*)prop.p_data);
	else
		t = actor->getScene()->findActor((const char*)prop.p_data);
	if (t != NULL)
		actor->setTargetB(t);
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - TargetB actor not found - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());
		return false;
	}

	return true;
}

CzXomlProperty CzActorConnector::_getTargetB(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorConnector*)target)->getTargetB());
}

bool CzActorConnector::_setOffsetA(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorConnector* actor = (CzActorConnector*)target;
	actor->setOffsetA(prop.p_vec[0], prop.p_vec[1]);

	return true;
}

CzXomlProperty CzActorConnector::_getOffsetA(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorConnector*)target)->getOffsetA());
}

bool CzActorConnector::_setOffsetB(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorConnector* actor = (CzActorConnector*)target;
	actor->setOffsetB(prop.p_vec[0], prop.p_vec[1]);

	return true;
}

CzXomlProperty CzActorConnector::_getOffsetB(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorConnector*)target)->getOffsetB());
}

/**
 @fn	bool CzActorConnector::getProperty(unsigned int property_name, CzXomlProperty& prop)

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in,out]	prop	The property.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActorConnector::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	if (CzActorImage::getProperty(property_name, prop))
		return true;

	int err = IzXomlResource::getProperty(ActorConnectorClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorConnectorClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorConnector - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzActorConnector::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the ActorConnector.
    
 Sets the named property of this ActorConnector. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzActorConnector::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	if (CzActorImage::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(ActorConnectorClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorConnectorClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorConnector - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzActorConnector::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced. Note that as CzActorConnector is derived from CzActor, all CzActor properties will also be checked against.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in]	data		The property data.
 @param	delta				true to property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
 
bool CzActorConnector::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	if (CzActorImage::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(ActorConnectorClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorConnectorClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorConnector - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzActorConnector::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates actor property from supplied animation.

 Updates the actors property that is specified by the supplied animation. When a timeline animation targets the property of an actor this method will be called by the animation to move the 
 animations frame data into its target property. 

 @param [in]	animation	If non-null, the animation.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActorConnector::UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzActorImage::UpdateFromAnimation(animation))
		return true;

	int err = IzXomlResource::setProperty(ActorConnectorClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorConnectorClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorConnector - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzActorConnector::UpdateBinding(unsigned int property_name, CzXomlVariable* var)

 @brief	Updates an actor property from the supplied XOML variable.

 Updates a single property from the value of the bound XOML variable. 

 @param	property_name  	Name of the property.
 @param [in,out]	var	If non-null, the variable.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActorConnector::UpdateBinding(unsigned int property_name, CzXomlVariable* var)
{
	if (!var->Modified || var->getValue().isEmpty())
		return false;

	if (CzActorImage::UpdateBinding(property_name, var))
		return true;

	int err = IzXomlResource::setProperty(ActorConnectorClassDef, this, property_name, var);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorConnectorClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorConnector - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	int CzActorConnector::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)

 @brief	Creates an instance of this class from XML

 LoadFromXoml is the main method used by all classes that can be instantiated from XOML mark-up. This method creates an instance of this class from the supplied XML node structure specified by node. 
 if parent is not NULL then the created instance will utilise the supplied parent to determine where it should live within the app. For example, if the parent is a scene then the actor will be placed 
 in that scene. if the parent is another actor then this actor will be placed into the scene of the supplied actor and linked as a child to the parent actor. If load_chlldren is true then any child 
 nodes contained within the actor will also be loaded. Note that as CzActorConnector is derived from CzActor, CzActorImage::LoadFromXoml() will also be called to load CzActor specific properties. 

 @param [in]	parent		If non-null, the parent.
 @param	load_children	  	true to load child nodes.
 @param [in]	node  		If non-null, the XOML markup node that defines this object

 @return	negative value for error. if 0 then processing of further XOML will stop.
 */

int CzActorConnector::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Load the actor from XOML
	int ret = CzActorImage::LoadFromXoml(parent, load_children, node);
	if (ret <= 0)
		return ret;

	// Process InertActor specific attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::TargetA_Hash)
		{
			TargetA = Scene->findActor((*it)->getValue().c_str());
			if (TargetA == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorConnector - Could not find TargetA actor in scene - ", (*it)->getValue().c_str(), DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::TargetB_Hash)
		{
			TargetB = Scene->findActor((*it)->getValue().c_str());
			if (TargetB == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorConnector - Could not find TargetB actor in scene - ", (*it)->getValue().c_str(), DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::OffsetA_Hash)
		{
			if (!(*it)->getValueAsPoint(OffsetA))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorConnector - Invalid value for Vec2 ActorConnector::OffsetA - ", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::OffsetB_Hash)
		{
			if (!(*it)->getValueAsPoint(OffsetB))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorConnector - Invalid value for Vec2 ActorConnector::OffsetB - ", DebugInfo.c_str());
		}
	}

	return 1;
}

/**
 @fn	bool CzActorConnector::Update(float dt)

 @brief	Updates this actors logic

 Updates the actors logic, including calculating end points of the connector and the scale to fit to the specified size. In addition CzActorImage::Update() is called to perform the base actor update.

 @param	dt	The number of seconds since the app was last updated

 @return	true if it succeeds, false if it fails.
 */

bool CzActorConnector::Update(float dt)
{
	CzVec2 pos1 = CzVec2(0, 0); 
	CzVec2 pos2 = CzVec2(0, 0); 
	
	if (TargetA != NULL)
	{
		CzVec2 offs = TargetA->TransformPoint(OffsetA.x, OffsetA.y);
		pos1.x = offs.x;
		pos1.y = offs.y;
	}
	else
		pos1 = CzVec2(OffsetA.x, OffsetA.y);
	if (TargetB != NULL)
	{
		CzVec2 offs = TargetB->TransformPoint(OffsetB.x, OffsetB.y);
		pos2.x = offs.x;
		pos2.y = offs.y;
	}
	else
		pos2 = CzVec2(OffsetB.x, OffsetB.y);

	float dx = pos2.x - pos1.x;
	float dy = pos2.y - pos1.y;
	float d = dx * dx + dy * dy;
	d = CzMath::sqrt(d);
	if (d < 1.0f) d = 1.0f;
	if (d > 10000.0f)
		d = 1.0f;

	float theta = CzMath::RadToDeg((CzMath::atan2(-dy, dx)));

	setScale(d / 100.0f, Scale.y);
	setAngle(theta);
	setPosition((pos1.x + pos2.x) / 2.0f, (pos1.y + pos2.y) / 2.0f);

	return CzActorImage::Update(dt);
}


