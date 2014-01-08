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

#include "CzXomlBindings.h"
#include "CzXomlResourceManager.h"
#include "CzActor.h"
#include "CzScene.h"
#include "CzHashes.h"

// 
// 
//
//
// CzXomlBindings implementation
//
//
//
//

void CzXomlBindings::addBinding(CzXomlBinding* binding)
{
	Bindings.push_back(binding);
}

void CzXomlBindings::addBinding(const CzString& property_name, CzXomlVariable* variable)
{
	CzXomlBinding* binding = new CzXomlBinding();
	binding->Property = property_name.getHash();
#if defined(_DEBUG)
	binding->_Property = property_name;
#endif	// _DEBUG
	binding->Variable = variable;
	Bindings.push_back(binding);
}


void CzXomlBindings::clearBindings()
{
	// Remove and delete all images from the manager
	for (_Iterator it = Bindings.begin(); it != Bindings.end(); ++it)
	{
		delete *it;
	}
	Bindings.clear();
}

int CzXomlBindings::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	CzScene* scene = NULL;
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Actor_Hash)
		scene = ((CzActor*)parent)->getScene();
	else
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

	// Process bindings attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
			setName((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
	}

	// Prrocess bindings
	for (CzXmlNode::_Iterator it2 = node->begin(); it2 != node->end(); ++it2)
	{
		unsigned int name_hash = (*it2)->GetName().getHash();
		if (name_hash == CzHashes::Binding_Hash)
		{
			CzXomlBinding* binding = new CzXomlBinding();

			// Get binding data
			for (CzXmlNode::_AttribIterator it = (*it2)->attribs_begin(); it != (*it2)->attribs_end(); ++it)
			{
				unsigned int attrib_hash = (*it)->getName().getHash();

				if (attrib_hash == CzHashes::Property_Hash)
				{
					(*it)->getValue().ToLower();
					binding->Property = (*it)->getValue().getHash();
#if defined(_DEBUG)
					binding->_Property = (*it)->getValue();
#endif	//_DEBUG
				}
				else
				if (attrib_hash == CzHashes::Variable_Hash)
				{
					binding->Variable = CzXomlVariable::GetVariable((*it)->getValue(), parent);
					if (binding->Variable == NULL)
						CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Bindings - Variable not found - ", (*it)->getValue().c_str(), DebugInfo.c_str());
				}
			}
			// Save action
			Bindings.push_back(binding);
		}
	}

	// If we are declared inside a scene then bindings are local to the scene
	if (scene != NULL)
		return scene->getResourceManager()->addResource(this) ? 1 : 0;
	else
		return CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this) ? 1 : 0;

	return 1;
}

