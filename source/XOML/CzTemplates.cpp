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

#include "CzTemplates.h"
#include "CzScene.h"
#include "CzResources.h"
#include "CzActor.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"

//
//
//
//
// CzTemplate implementation
//
//
//
//
int CzTemplate::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Parse template attributes
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

	// Only clone none empty template
	if (node->begin() != node->end())
	{
		Root = node->CloneTree(NULL);
	}

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

bool CzTemplate::Instantiate(IzXomlResource* parent, CzXmlNode* replacements)
{
	// We need to work on a copy of the tree so we do not overwtite the template
	CzXmlNode* object_node = getRoot();
	CzXmlNode* tree_copy = object_node->CloneTree(NULL);

	// Search copy of template replacing any attribute values with the provided values
	if (replacements != NULL)
		ReplaceTreeAttributes(tree_copy, replacements);

	// Process the XOML nodes
	if (!CZ_XOML->Process(parent, tree_copy))
	{
		delete tree_copy;
		return false;
	}

	// Remove temp tree
	delete tree_copy;

	return true;
}

void CzTemplate::ReplaceAttributes(CzXmlAttribute* attrib, CzXmlNode* replacements)
{
	// Only attempt to replace an attribute if it contains a dollar sign. Names enclosed in dollar signs are to be replaced by attributes from the template
	CzString& value = attrib->getValue();
//CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Checking - ", value.c_str());
	if (value.Contains('$') < 0)
		return;

	// Check attribute list 
	CzString var;
	for (CzXmlNode::_AttribIterator it = replacements->attribs_begin(); it != replacements->attribs_end(); it++)
	{
		CzString& prop_name = (*it)->getName();
		CzString& prop_value = (*it)->getValue();
		unsigned int prop_name_hash = prop_name.getHash();
		if (prop_name_hash != CzHashes::Template_Hash)
		{
			var = "$";
			var += prop_name;
			var += "$";
			if (value.Replace(var.c_str(), prop_value.c_str()) >= 0)
			{
//CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "  string - ", var.c_str());
//CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "  with - ", value.c_str());
			}
		}
	}
}

void CzTemplate::ReplaceTreeAttributes(CzXmlNode* node, CzXmlNode* replacements)
{
	// Replace any marked attribute values with replacements provided by the user
	for (CzXmlNode::_AttribIterator it2 = node->attribs_begin(); it2 != node->attribs_end(); it2++)
	{
		unsigned int name_hash = (*it2)->getName().getHash();

		ReplaceAttributes(*it2, replacements);
	}

	// Process child nodes
	for (CzXmlNode::_Iterator it = node->begin(); it != node->end(); it++)
	{
		ReplaceTreeAttributes(*it, replacements);
	}
}



//
// 
// 
//
// CzTemplateFrom Implementation
// 
// 
// 
//
int CzTemplateFrom::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	CzString temp;

	// Process LoadXoml specific attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Template_Hash)
		{
			temp = (*it)->getValue();
			break;
		}
	}

	if (!temp.isEmpty())
	{
		CzTemplate* templ = NULL;
		CzScene* scene = NULL;

		if (parent->getClassTypeHash() == CzHashes::Actor_Hash)
			scene = ((CzActor*)parent)->getScene();

		// Find the template
		if (scene != NULL)
			templ = (CzTemplate*)scene->getResourceManager()->findResource(temp.c_str(), CzHashes::Template_Hash);
		else
		if (parent->getClassTypeHash() == CzHashes::Scene_Hash)
		{
			scene = (CzScene*)parent;
			templ = (CzTemplate*)scene->getResourceManager()->findResource(temp.c_str(), CzHashes::Template_Hash);
		}

		if (templ == NULL)
			templ = (CzTemplate*)CZ_GLOBAL_RESOURCES->getResourceManager()->findResource(temp.c_str(), CzHashes::Template_Hash);

		// Template cant be found so return error
		if (templ == NULL)
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Template - Does not exist in scene or globally - ",  temp.c_str(), DebugInfo.c_str());
			return 0;
		}

		if (!templ->Instantiate(parent, node))
			return 0;
	}

	return -1;
}





