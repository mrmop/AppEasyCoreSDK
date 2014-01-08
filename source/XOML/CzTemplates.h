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

#if !defined(_CZ_TEMPLATES_H_)
#define _CZ_TEMPLATES_H_

#include "CzUtil.h"
#include "CzXoml.h"

//
//
//
//
//  CzTemplate - A XOML template which can be used to instantiate a custom version of a class
//
//
//
//
class CzTemplate : public IzXomlResource
{
public:
	// Properties
protected:
	unsigned int			TargetType;				// Type of class that this template can be applied to such as actor, scene etc..
	CzXmlNode*				Root;					// Root node
public:
	unsigned int			getTargetType() const					{ return TargetType; }
	CzXmlNode*				getRoot()								{ return Root; }
	// Properties end
protected:
	void				ReplaceAttributes(CzXmlAttribute* attrib, CzXmlNode* replacements);
	void				ReplaceTreeAttributes(CzXmlNode* node, CzXmlNode* replacements);
public:
	CzTemplate() : IzXomlResource(), Root(NULL) { setClassType("template"); }
	virtual ~CzTemplate()
	{
		SAFE_DELETE(Root)
	}

	bool				Instantiate(IzXomlResource* parent, CzXmlNode* replacements);

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);
};

//
//
//
//
//  CzTemplateCreator - Creates an instance of a template
//
//
//
//
class CzTemplateCreator : public IzXomlClassCreator
{
public:
	CzTemplateCreator()
	{
		setClassName("template");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzTemplate(); }
};

//
//
//
//
// CzTemplateFrom utility class to allow loading of XOMl files from XOML
// 
//
// 
//
class CzTemplateFrom : public IzXomlResource
{
public:
protected:
	// Properties
public:
	// Properties end
protected:

public:
	CzTemplateFrom() : IzXomlResource() { setClassType("fromtemplate"); }
	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parebt, bool load_children, CzXmlNode* node);
};

class CzTemplateFromCreator : public IzXomlClassCreator
{
public:
	CzTemplateFromCreator()
	{
		setClassName("fromtemplate");
	}
	IzXomlResource* CreateInstance(IzXomlResource* parent)	{ return new CzTemplateFrom(); }
};




#endif // _CZ_TEMPLATES_H_