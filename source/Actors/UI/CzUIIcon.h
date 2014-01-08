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

#if !defined(_CCZ_UI_ICON_H_)
#define _CCZ_UI_ICON_H_

#include "CzUIBase.h"

//
//
//
//
// CzUIIcon- An image based actor that acts as an icon
//
//
//
//
class CzUIIcon : public CzUIBase
{
	// Properties
protected:
public:
	// Properties end
protected:
public:
	CzUIIcon() : CzUIBase()	{ setActualClassType("icon"); }

//	bool				Update(float dt);
	// Implementation of IzXomlClass interface
//	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);
};

//
//  CzUIIconCreator - Creates an instance of an icon actor object
//
class CzUIIconCreator : public IzXomlClassCreator
{
public:
	CzUIIconCreator()
	{
		setClassName("icon");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzUIIcon(); }
};



#endif	// _CCZ_UI_ICON_H_
