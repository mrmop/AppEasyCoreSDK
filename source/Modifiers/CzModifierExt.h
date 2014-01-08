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

#if !defined(_CZ_MODIFIER_EXT_H_)
#define _CZ_MODIFIER_EXT_H_

#include "CzUtil.h"
#include "CzModifier.h"
#include "CzActor.h"

//
//
//  CzModFollowHeading - Modifier that calculates actors velocity based on heading and speed
//
//	Parameter1 - Heading in degrees (float)
//	Parameter2 - Speed (float)
//
//
class CzModFollowHeading : public IzModifier
{
	// Properties
protected:
	float		Heading;				// Direction
	float		Speed;					// Speed of travel

public:
	void		setHeading(float angle)				{ Heading = angle; }
	float		getHeading() const					{ return Heading; }
	void		setSpeed(float speed)				{ Speed = speed; }
	float		getSpeed() const					{ return Speed; }
	// Properties End

public:
	void InitModifier(IzXomlResource* target)
	{
		Heading = Params[0].getAsFloat();
		Speed = Params[1].getAsFloat();
	}
	void ReleaseModifier(IzXomlResource* target)
	{
	}
	bool UpdateModifier(IzXomlResource* target, float dt)
	{
		CzActor* actor = (CzActor*)target;
		float angle = (Heading * 3.1415927f) / 180.0f;

		actor->setVelocity(cosf(Heading) * Speed, sinf(Heading) * Speed);

		return true;
	}
};
class CzModFollowHeadingCreator : public IzModifierCreator
{
public:
	CzModFollowHeadingCreator()
	{
		setClassName("iw_followheading");
	}
	IzModifier* CreateInstance() { return new CzModFollowHeading(); }
};

//
//
//  CzModCollisionNotify - Modifier that adds Box2D collision notifications
//
//	Parameter1 - Collision mask, any actors that pass the mask are potential colliders
//
//
class CzModCollisionNotify : public IzModifier
{
	// Properties
protected:
	int			CollisionMask;					// Colision  mask, any actors that pass the mask are potential colliders

public:
	void		setCollisionMask(int mask)			{ CollisionMask = mask; }
	int			getCollisionMask() const			{ return CollisionMask; }
	// Properties End

public:
	void InitModifier(IzXomlResource* target)
	{
		if (Params[0].isEmpty())
			CollisionMask = 0;
		else
			CollisionMask = Params[0].getAsInt();
	}
	void ReleaseModifier(IzXomlResource* target)
	{
	}
	bool UpdateModifier(IzXomlResource* target, float dt);
};
class CzModCollisionNotifyCreator : public IzModifierCreator
{
public:
	CzModCollisionNotifyCreator()
	{
		setClassName("iw_notifycollision");
	}
	IzModifier* CreateInstance() { return new CzModCollisionNotify(); }
};

//
//
//  CzModScript - Modifier that calls a script to modify the object
//
//	Parameter1 - Script function name, any actors that pass the mask are potential colliders
//
//
class CzModScript : public IzModifier
{
	// Properties
protected:
	int			CollisionMask;					// Colision  mask, any actors that pass the mask are potential colliders

public:
	void		setCollisionMask(int mask)			{ CollisionMask = mask; }
	int			getCollisionMask() const			{ return CollisionMask; }
	// Properties End

public:
	void InitModifier(IzXomlResource* target)
	{
	}
	void ReleaseModifier(IzXomlResource* target)
	{
	}
	bool UpdateModifier(IzXomlResource* target, float dt);
};
class CzModScriptCreator : public IzModifierCreator
{
public:
	CzModScriptCreator()
	{
		setClassName("iw_callscript");
	}
	IzModifier* CreateInstance() { return new CzModScript(); }
};




#endif	// _CZ_MODIFIER_EXT_H_
