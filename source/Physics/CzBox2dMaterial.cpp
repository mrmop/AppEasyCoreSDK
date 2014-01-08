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

#include "CzBox2d.h"
#include "CzActor.h"
#include "CzScene.h"
#include "CzResources.h"
#include "CzHashes.h"
#include "CzMath.h"
#include "CzXomlResourceManager.h"

//
//
//
//
// CzBox2dMaterial implementation
//
//
//
//
int CzBox2dMaterial::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Process material attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
			setName((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Type_Hash)
		{
			unsigned int type_hash = (*it)->getValue().getHash();
			if (type_hash == CzHashes::static_Hash)
				setBodyType(b2_staticBody);
			else
			if (type_hash == CzHashes::dynamic_Hash)
				setBodyType(b2_dynamicBody);
			else
			if (type_hash == CzHashes::kinematic_Hash)
				setBodyType(b2_kinematicBody);
			else
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Box2dMaterial - Invalid body type for box2d material - ", (*it)->getValue().c_str(), DebugInfo.c_str());
			}
		}
		else
		if (name_hash == CzHashes::Density_Hash)
			setDensity((*it)->getValueAsFloat());
		else
		if (name_hash == CzHashes::Friction_Hash)
			setFriction((*it)->getValueAsFloat());
		else
		if (name_hash == CzHashes::Restitution_Hash)
			setRestitution((*it)->getValueAsFloat());
		else
		if (name_hash == CzHashes::IsBullet_Hash)
			setBullet((*it)->getValueAsBool());
		else
		if (name_hash == CzHashes::FixedRotation_Hash)
			setFixedRotation((*it)->getValueAsBool());
		else
		if (name_hash == CzHashes::GravityScale_Hash)
			setGravityScale((*it)->getValueAsFloat());
	}

	// If we are declared inside a scene then material is local to the scene
	CzScene* scene = NULL;
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Actor_Hash)
		scene = ((CzActor*)parent)->getScene();
	else
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

	if (scene != NULL)
		return scene->getResourceManager()->addResource(this) ? 1 : 0;
	else
		return CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this) ? 1 : 0;

	return 1;
}

