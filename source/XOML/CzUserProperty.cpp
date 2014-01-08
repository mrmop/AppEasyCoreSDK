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

#include "CzUserProperty.h"
#include "CzHashes.h"
#include "CzUtil.h"
#include "CzString.h"
#include "CzActor.h"
#include "CzScene.h"

/**
 @fn	bool CzUserPropertyList::addProperty(CzXomlNamedProperty* prop)

 @brief	Adds a property to a property list.

 @param [in]	prop	If non-null, the property.

 @return	true if it succeeds, false if it fails.
 */

bool CzUserPropertyList::addProperty(CzXomlNamedProperty* prop)
{
	Properties.push_back(prop);
	return true;
}

/**
 @fn	void CzUserPropertyList::removeProperty(CzXomlNamedProperty* prop, bool destroy)

 @brief	Removes the property described by prop from the property list.

 @param [in]	prop	If non-null, the property.
 @param	destroy			true to destroy property.
 */

void CzUserPropertyList::removeProperty(CzXomlNamedProperty* prop, bool destroy)
{
	if (prop != NULL)
	{
		delete prop;
		Properties.remove(prop);
	}
}

/**
 @fn	void CzUserPropertyList::clearProperties(bool destroy)

 @brief	Clears the properties.

 @param	destroy	true to destroy all properties.
 */

void CzUserPropertyList::clearProperties(bool destroy)
{
	for (_Iterator it = begin(); it != end(); ++it)
		delete *it;
	Properties.clear();
}

/**
 @fn	CzXomlNamedProperty* CzUserPropertyList::findProperty(unsigned int name_hash)

 @brief	Searches the properties list for the named property.

 @param	name_hash	The propery name as a string hash.

 @return	null if it fails, else the found property.
 */

CzXomlNamedProperty* CzUserPropertyList::findProperty(unsigned int name_hash)
{
	for (_Iterator it = begin(); it != end(); ++it)
	{
		if ((*it)->Name.getHash() == name_hash)
			return *it;
	}

	return NULL;
}

/**
 @fn	CzXomlNamedProperty* CzUserPropertyList::findProperty(const char* name)

 @brief	Searches the properties list for the named property.

 @param	name	The property name.

 @return	null if it fails, else the found property.
 */

CzXomlNamedProperty* CzUserPropertyList::findProperty(const char* name)
{
	return findProperty(CzString::CalculateHash(name));
}

/**
 @fn	bool CzUserPropertyList::setProperty(const char* name, const CzString& data, bool delta)

 @brief	Sets the named property.

 @param	name 	The name of the property.
 @param	data 	The data to assign to the property.
 @param	delta	true to add value nistead of set.

 @return	true if it succeeds, false if it fails.
 */

bool CzUserPropertyList::setProperty(const char* name, const CzString& data, bool delta)
{
	return setProperty(CzString::CalculateHash(name), data, delta);
}

/**
 @fn	bool CzUserPropertyList::setProperty(unsigned int name_hash, const CzString& data,
    bool delta)

 @brief	Sets the named property.

 @param	name_hash 	The name of the property as a string hash.
 @param	data 	The data to assign to the property.
 @param	delta	true to add value nistead of set.

 @return	true if it succeeds, false if it fails.
 */

bool CzUserPropertyList::setProperty(unsigned int name_hash, const CzString& data, bool delta)
{
	CzXomlNamedProperty* prop = findProperty(name_hash);
	if (prop == NULL)
		return false;

	return prop->Property->setValue(data, delta);
}

/**
 @fn	bool CzUserPropertyList::setPropertyToVariable(unsigned int name_hash,
    CzXomlVariable& var)

 @brief	Copies the value of the property to a variable.

 @param	name_hash	   	The property name as a string hash.
 @param [out]	var	The target variable.

 @return	true if it succeeds, false if it fails.
 */

bool CzUserPropertyList::setPropertyToVariable(unsigned int name_hash, CzXomlVariable& var)
{
	CzXomlNamedProperty* prop = findProperty(name_hash);
	if (prop == NULL)
		return false;

	prop->Property->setToVariable(var);

	return true;
}

/**
 @fn	int CzUserPropertyList::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)

 @brief	Creates an instance of this class from XOML.

 LoadFromXoml is the main method used by all classes that can be instantiated from XOML mark-up. This method creates an instance of this class from the supplied XML node structure specified by node. 
 
 @param [in]	parent		If non-null, the parent.
 @param	load_children	  	true to load child nodes.
 @param [in]	node  		If non-null, the XOML markup node that defines this object

 @return	nergative value for error. if 0 then processing of further XOML will stop.
 */

int CzUserPropertyList::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Process animation attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
			setName((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
	}

	// Prrocess anim frames
	for (CzXmlNode::_Iterator it2 = node->begin(); it2 != node->end(); ++it2)
	{
		unsigned int name_hash = (*it2)->GetName().getHash();
		if (name_hash == CzHashes::Property_Hash)
		{
			CzString* name = NULL;
			CzString* type = NULL;
			CzString* value = NULL;

			for (CzXmlNode::_AttribIterator it = (*it2)->attribs_begin(); it != (*it2)->attribs_end(); ++it)
			{
				unsigned int attrib_hash = (*it)->getName().getHash();

				if (attrib_hash == CzHashes::Name_Hash)
					name = &(*it)->getValue();
				else
				if (attrib_hash == CzHashes::Type_Hash)
					type = &(*it)->getValue();
				else
				if (attrib_hash == CzHashes::Value_Hash)
					value = &(*it)->getValue();
			}

			if (name == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "UserProperty - Must be named", DebugInfo.c_str());
			if (type == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "UserProperty - Must have a type", DebugInfo.c_str());

			if (name != NULL && type != NULL)
			{
				CzXomlNamedProperty* nprop = new CzXomlNamedProperty();
				CzXomlProperty* prop = CzXomlProperty::createOfType(type->c_str());
				nprop->Name = name->c_str();
				nprop->Property = prop;
				addProperty(nprop);
				if (value != NULL)
					prop->setValue(*value, false);
			}
		}
	}

	// If we are declared inside an actor then property is local to the actor, if inside a scene then we are local to scene
	CzScene* scene = NULL;
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Actor_Hash)
		((CzActor*)parent)->setUserPropertyList(this);
	else
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		((CzScene*)parent)->setUserPropertyList(this);
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "UserProperty - Must be declared inside an actor or a scene", DebugInfo.c_str());
		return -1;
	}

	return 1;
}