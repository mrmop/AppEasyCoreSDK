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
#include "CzBox2dFixture.h"
#include "CzActor.h"
#include "CzScene.h"
#include "CzResources.h"
#include "CzHashes.h"
#include "CzMath.h"
#include "CzXomlResourceManager.h"


int CzBox2dFixtures::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	CzActor* actor = NULL;
	CzScene* scene = NULL;
	if (parent->getClassTypeHash() == CzHashes::Actor_Hash)
	{
		actor = (CzActor*)parent;
		scene = actor->getScene();
	}
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Fixtures - Fixtures need to be declared inside an actor", DebugInfo.c_str());
		return 0;
	}

	for (CzXmlNode::_Iterator it = node->begin(); it != node->end(); ++it)
	{
		unsigned int	name_hash = (*it)->GetName().getHash();

		if (name_hash == CzHashes::Fixture_Hash)
		{
			CzString*		box2dmaterial_name = NULL, *shape_name = NULL;
			CzVec2			com(0, 0);
			CzVec3			collision_flags(0, 0, 0);
			bool			sensor = false;

			// Get joint data
			for (CzXmlNode::_AttribIterator it2 = (*it)->attribs_begin(); it2 != (*it)->attribs_end(); ++it2)
			{
				unsigned int attrib_hash = (*it2)->getName().getHash();

				if (attrib_hash == CzHashes::Box2dMaterial_Hash)
					box2dmaterial_name = &(*it2)->getValue();
				else
				if (attrib_hash == CzHashes::Shape_Hash)
					shape_name = &(*it2)->getValue();
				else
				if (attrib_hash == CzHashes::COM_Hash)
				{
					if (!(*it2)->getValueAsPoint(com))
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Fixture - Invalid value for Fixture::COM, expected vec2 - ", DebugInfo.c_str());
				}
				else
				if (attrib_hash == CzHashes::Sensor_Hash)
					sensor = (*it2)->getValueAsBool();
				else
				if (attrib_hash == CzHashes::CollisionFlags_Hash)
				{
					if (!(*it2)->getValueAsPoint3(collision_flags))
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Fixture - Invalid value for Fixture::CollisionFlags, expected vec3 - ", DebugInfo.c_str());
				}
			}

			// Check for shape and material are present
			CzShape* shape = NULL;
			if (shape_name != NULL && !shape_name->isEmpty() && box2dmaterial_name != NULL && !box2dmaterial_name->isEmpty())
			{
				shape = (CzShape*)scene->getResourceManager()->findResource(shape_name->c_str(), CzHashes::Shape_Hash);
				if (shape == NULL)
				{
					CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Fixture - Could not find named shape in the scene or in the global resources - ", shape_name->c_str(), DebugInfo.c_str());
				}
				else
				{
					CzBox2dMaterial* material = (CzBox2dMaterial*)scene->getResourceManager()->findResource(box2dmaterial_name->c_str(), CzHashes::Box2dMaterial_Hash);
					if (material == NULL)
						CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Fucture - Could not find named material in the scene or in the global resources - ", box2dmaterial_name->c_str(), DebugInfo.c_str());
					else
					{
						b2Fixture* fixture = actor->getBox2dBody()->addFixture(shape, material, com.x, com.y);
						fixture->SetSensor(sensor);
						b2Filter f;
						f.categoryBits = (int)collision_flags.x;
						f.maskBits = (int)collision_flags.y;
						f.groupIndex = (int)collision_flags.z;
						fixture->SetFilterData(f);
					}
				}
			}

		}
	}

	return -1;
}




