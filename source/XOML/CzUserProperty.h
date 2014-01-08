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

#if !defined(_CZ_USER_PROPERTY_H_)
#define _CZ_USER_PROPERTY_H_

#include "CzString.h"
#include "CzUtil.h"
#include "CzXoml.h"


/**
 @addtogroup XOML
 @{
 */


/**
 @class	CzUserPropertyList

 @brief	A user property list.

 Property lists can be added to scenes and actors to provide a mechanism for users to track and modify their own properties

 */

class CzUserPropertyList : public IzXomlResource
{
	// Public access to class creator iteration
	typedef CzList<CzXomlNamedProperty*>::iterator _Iterator;
	_Iterator				begin()		{ return Properties.begin(); }
	_Iterator				end()		{ return Properties.end(); }

protected:
	CzList<CzXomlNamedProperty*>	Properties;
public:
	CzUserPropertyList() : IzXomlResource() { setClassType("userproperties"); }
	~CzUserPropertyList()	{ clearProperties(); }

	bool					addProperty(CzXomlNamedProperty* prop);
	void					removeProperty(CzXomlNamedProperty* prop, bool destroy = true);
	void					clearProperties(bool destroy = true);
	CzXomlNamedProperty*	findProperty(unsigned int name_hash);
	CzXomlNamedProperty*	findProperty(const char* name);
	bool					setProperty(const char* name, const CzString& data, bool delta);
	bool					setProperty(unsigned int name_hash, const CzString& data, bool delta);
	bool					setPropertyToVariable(unsigned int name_hash, CzXomlVariable& var);

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);
};

/**
 @class	CzUserPropertyListCreator

 @brief	Creates an instance of a user properties class.

 */

class CzUserPropertyListCreator : public IzXomlClassCreator
{
public:
	CzUserPropertyListCreator()
	{
		setClassName("userproperties");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzUserPropertyList(); }
};


/// @}

#endif	// _CZ_USER_PROPERTY_H_
