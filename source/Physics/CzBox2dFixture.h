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

#if !defined(_CZ_BOX2D_FIXTURE_H_)
#define _CZ_BOX2D_FIXTURE_H_

#include "CzUtil.h"
#include "CzString.h"
#include "CzXoml.h"
#include "CzShapes.h"

#include "Box2D/Box2D.h"

class CzBox2dWorld;
class CzBox2dBody;

//
//
//
//
// CzBox2dFixtures - Box2D physics fixtures XOML command (needs to be declared inside an actor)
//
//
//
//
class CzBox2dFixtures : public IzXomlResource
{
public:
	CzBox2dFixtures() { setClassType("fixtures"); }

	// Implementation of IzXomlResource interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);
};

//
//  CzBox2dFixturesCreator - Creates an instance of Box2D Fixtures object
//
class CzBox2dFixturesCreator : public IzXomlClassCreator
{
public:
	CzBox2dFixturesCreator()
	{
		setClassName("fixtures");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzBox2dFixtures(); }
};



#endif // _CZ_BOX2D_FIXTURE_H_
