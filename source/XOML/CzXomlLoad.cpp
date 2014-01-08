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

#include "CzXomlLoad.h"
#include "CzTemplates.h"
#include "CzXoml.h"
#include "CzXomlResourceManager.h"
#include "CzHashes.h"
#include "CzUtil.h"
#include "CzString.h"
#include "CzApp.h"
#include "CzScene.h"
#include "CzActor.h"

//
// 
// 
//
// CzXomlLoad Implementation
// 
// 
// 
//

/**
 @fn	int CzXomlLoad::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)

 @brief	XOML command that loads another XOML file.

 This method loads another XOML file into the parent XOML resource (for example a scene or actor). Supplying NULL for parent will load the XOML globally.

 @param [in,out]	parent	If non-null, the parent.
 @param	load_children	  	true to load children nodes.
 @param [in]		node  	If non-null, the node.
 */

int CzXomlLoad::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	CzScene* scene = NULL;
	if (parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

	CzString* file = NULL;
	CzString* condition = NULL;

	// Process LoadXoml specific attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::File_Hash)
			file = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Condition_Hash)
			condition = &(*it)->getValue();
	}

	if (condition != NULL)
	{
		// Find the condition variable
		bool condition_not = false;
		CzXomlVariable* var = NULL;
		if (*(condition->c_str()) == '!')
		{
			condition_not = true;
			CzString cond = condition->c_str() + 1;
			var = CzXomlVariable::GetVariable(cond, scene);
		}
		else
			var = CzXomlVariable::GetVariable(*condition, scene);
		if (var != NULL)
		{
			bool res = var->isTrue();
			if (condition_not)
				res = !res;
			if (!res)
				return -1;
		}
#if defined (_DEBUG)
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "LoadXOML - Condition variable not found - ", condition->c_str(), DebugInfo.c_str());
#endif // _DEBUG
	}


	if (file != NULL)
		CZ_XOML->Process(parent, file->c_str(), false);

	return -1;
}

/**
 @fn	bool CzXomlLoad::CreateInstance(IzXomlResource* parent, CzScene* scene,
    CzString* template_name, CzString* parameters)

 @brief	Instantiates XOML data from a template.

 Instantiates XOML data from the supplied template using the supplied parameters list. The scene represents the scene that will receive the created XOML objects whilst the parent 
 represents the place where to look for the template.

 @param [in]		parent		 	If non-null, the parent where the template resource is located.
 @param [in,out]	scene		 	If non-null, the scene where the template objects will be instantiated.
 @param [in]		template_name	If non-null, name of the template.
 @param [in]		parameters   	If non-null, template parameters.

 @return	The new instance.
 */

bool CzXomlLoad::CreateInstance(IzXomlResource* parent, CzScene* scene, CzString* template_name, CzString* parameters)
{
	if (template_name == NULL)
		return false;

	// Get the template
	CzTemplate* temp = (CzTemplate*)CzXomlResourceManager::FindResource(template_name->getHash(), CzHashes::Template_Hash, parent);
	if (temp != NULL)
	{
		if (parameters != NULL && !parameters->isEmpty())
		{
			// Create a set of XML attributes that will replace the template parameters
			CzXmlNode* replacements = new CzXmlNode();
			replacements->Managed = false;

			// Split parameter two into paramater=value pairs
			CzSlotArray<CzString*>* split = parameters->Split(':');
			CzSlotArray<CzString*>* strings = new CzSlotArray<CzString*>();
			for (int t = 0; t < split->getSize(); t++)
			{
				CzString* pair = split->element_at(t);
				if (pair != NULL)
				{
					// Split paramater / value pair
					pair->Split('=', strings);
					CzString* name = strings->element_at(0);
					CzString* value = strings->element_at(1);

					if (name != NULL && value != NULL)
					{
						// Set template paramater
						CzXmlAttribute* attrib = new CzXmlAttribute();
						attrib->Managed = false;
						attrib->setName(name->c_str());
						attrib->setValue(value->c_str());
						replacements->AddAttribute(attrib);
					}
				}
				strings->clear(true);
			}
			temp->Instantiate(scene, replacements);

			delete replacements;
			delete strings;
			split->clear(true);
			delete split;
		}
		else
			temp->Instantiate(scene, NULL);

	}
#if defined (_DEBUG)
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - FromTemplate - Cannot find template - ", template_name->c_str(), parent->getDebugInfo().c_str());
		return false;
	}
#endif

	return true;
}

