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

#if !defined(_CZ_XOML_LOAD_H_)
#define _CZ_XOML_LOAD_H_

#include "CzString.h"
#include "CzUtil.h"
#include "CzXoml.h"
#include "CzScene.h"

/**
 @class	CzXomlLoad

 @brief	Utility class to allow loading of XOML files from within another XOML file.

 CzXomlLoad implements the LoadXOML tag, which enables a XOML file to load another XOML file into its heirarchy.

 Loading a XOML file from XOML is incredibly simple:

 @code
<LoadXOML File="UIStyle.xml" />
 @endcode

 Or from an action:

 @code
<Action Method="LoadXOML" P1="Scene2.xml" />
 @endcode

 */
class CzXomlLoad : public IzXomlResource
{
public:
protected:
	// Properties
public:
	// Properties end
public:
	CzXomlLoad() : IzXomlResource() { setClassType("loadxoml"); }
	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parebt, bool load_children, CzXmlNode* node);

	static bool			CreateInstance(IzXomlResource* parent, CzScene* scene, CzString* template_name, CzString* parameters);
};

/**
 @class	CzXomlLoadCreator

 @brief	Creates an instance of CzXomlLoad.

 Used by the XOMl system to create an instance of CzXomlLoad.

 */
class CzXomlLoadCreator : public IzXomlClassCreator
{
public:
	CzXomlLoadCreator()
	{
		setClassName("loadxoml");
	}
	IzXomlResource* CreateInstance(IzXomlResource* parent)	{ return new CzXomlLoad(); }
};

/// @}

#endif	// _CZ_XOML_LOAD_H_
