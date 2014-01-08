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

#include "CzStyles.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzResources.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"

//
//
//
//
// CzStyle implementation
//
//
//
//
void CzStyle::addProperty(CzStyleProperty* prop)
{
	Properties.push_back(prop);
}

void CzStyle::addProperty(const char* property_name, const char* property_value)
{
	CzStyleProperty* prop = new CzStyleProperty();
	prop->Name = property_name;
	prop->Value = property_value;
	Properties.push_back(prop);
}

void CzStyle::clearProperties()
{
	for (_Iterator it = begin(); it != end(); ++it)
	{
		delete *it;
	}
	Properties.clear();
}

int CzStyle::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Parse Style attributes
	CzString name;
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
		{
			setName((*it)->getValue().c_str());
			name = (*it)->getValue();
		}
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
	}

	// Parse properties
	CzString property_name;
	CzString property_value;
	for (CzXmlNode::_Iterator it2 = node->begin(); it2 != node->end(); ++it2)
	{
		unsigned int name_hash = (*it2)->GetName().getHash();
		if (name_hash == CzHashes::Set_Hash)
		{
			// Get setter data
			for (CzXmlNode::_AttribIterator it3 = (*it2)->attribs_begin(); it3 != (*it2)->attribs_end(); ++it3)
			{
				unsigned int attrib_hash = (*it3)->getName().getHash();

				if (attrib_hash == CzHashes::Property_Hash)
					property_name = (*it3)->getValue();
				else
				if (attrib_hash == CzHashes::Value_Hash)
					property_value = (*it3)->getValue();
			}
		}
		if (property_name.isEmpty() || property_value.isEmpty())
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Style - Invalid property name or value for Style - ", name.c_str(), DebugInfo.c_str());
			return 0;
		}
		else
		{
			// Create and add the style property
			property_name.ToLower();
			addProperty(property_name.c_str(), property_value.c_str());
		}
	}

	// If we are declared inside a scene then shape is local to the scene
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






