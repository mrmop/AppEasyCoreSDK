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

#if !defined(_CZ_BOX2D_MATERIAL_H_)
#define _CZ_BOX2D_MATERIAL_H_

#include "CzUtil.h"
#include "CzString.h"
#include "CzXoml.h"
#include "CzShapes.h"
#include "CzSlotArray.h"

#include "Box2D/Box2D.h"

//
//
//
//
// CzBox2dMaterial - Box2D physics material
//
//
//
//
class CzBox2dMaterial : public IzXomlResource
{
public:

protected:
	// Properties
	b2BodyType			BodyType;			// Physical body type
	float				Density;			// Density of body
	float				Friction;			// Amount of friction to apply
	float				Restitution;		// Bounciness of the object
	bool				Bullet;				// Is bullet type (is set then object will use continuous colilsion detection (slower)) - only use for very fast moviog objects
	bool				FixedRotation;		// Set to true if you do not want an objects raotation to change
	float				GravityScale;		// Specific gravity scale
public:
	void				setBodyType(b2BodyType type)		{ BodyType = type; }
	b2BodyType			getBodyType() const					{ return BodyType; }
	void				setDensity(float density)			{ Density = density; }
	float				getDensity() const					{ return Density; }
	void				setFriction(float friction)			{ Friction = friction; }
	float				getFriction() const					{ return Friction; }
	void				setRestitution(float restitution)	{ Restitution = restitution; }
	float				getRestitution() const				{ return Restitution; }
	void				setBullet(bool bullet)				{ Bullet = bullet; }
	bool				isBullet() const					{ return Bullet; }
	void				setFixedRotation(bool fixed)		{ FixedRotation = fixed; }
	bool				isFixedRotation() const				{ return FixedRotation; }
	void				setGravityScale(float scale)		{ GravityScale = scale; }
	float				getGravityScale() const				{ return GravityScale; }
	// Properties end

public:
	CzBox2dMaterial() : IzXomlResource(), Density(1.0f), Friction(1.0f), Restitution(0.1f), Bullet(false), FixedRotation(false), GravityScale(1000000.0f)  { setClassType("box2dmaterial"); }
	virtual ~CzBox2dMaterial() {}

	// Implementation of IzXomlResource interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);
};

//
//  CzBox2dMaterialCreator - Creates an instance of a box2d material object
//
class CzBox2dMaterialCreator : public IzXomlClassCreator
{
public:
	CzBox2dMaterialCreator()
	{
		setClassName("box2dmaterial");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzBox2dMaterial(); }
};



#endif // _CZ_BOX2D_MATERIAL_H_
