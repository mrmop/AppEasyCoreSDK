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

#if !defined(_CZ_STYLES_H_)
#define _CZ_STYLES_H_

#include "CzUtil.h"
#include "CzXoml.h"

//
//
//
//
//  CzStyleProperty - A style property
//
//
//
//
struct CzStyleProperty
{
public:
	CzString	Name;					// Name of property
	CzString	Value;					// Value of the property

	CzStyleProperty() {}
	virtual ~CzStyleProperty() {}
};

//
//
//
//
//  CzStyle - A class that is used to style other classes
//
//
//
//
class CzStyle : public IzXomlResource
{
public:
	// Public access to actor iteration
	typedef CzList<CzStyleProperty*>::iterator _Iterator;
	_Iterator					begin() { return Properties.begin(); }
	_Iterator					end() { return Properties.end(); }
public:
	// Properties
protected:
	CzList<CzStyleProperty*>	Properties;
public:
	void						addProperty(CzStyleProperty* prop);
	void						addProperty(const char* property_name, const char* property_value);
	void						clearProperties();
	// Properties end

public:
	CzStyle() : IzXomlResource() { setClassType("style"); }
	virtual ~CzStyle()
	{
		clearProperties();
	}

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);
};

//
//
//
//
//  CzStyleCreator - Creates an instance of a style
//
//
//
//
class CzStyleCreator : public IzXomlClassCreator
{
public:
	CzStyleCreator()
	{
		setClassName("style");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzStyle(); }
};



#endif // _CZ_STYLES_H_