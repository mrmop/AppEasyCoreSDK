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

#if !defined(_CZ_BOX2D_H_)
#define _CZ_BOX2D_H_

#include "CzUtil.h"
#include "CzString.h"
#include "CzXoml.h"
#include "CzShapes.h"
#include "CzSlotArray.h"
#include "CzBox2dMaterial.h"
#include "CzBox2dJoint.h"
#include "CzBox2dFixture.h"

#include "Box2D/Box2D.h"


//
//
//
//
// CzBox2dCcollisionCallback - Box2D scene collision callback
//
//
//
//
class CzBox2dCcollisionCallback : public b2ContactListener
{
public:
	void				BeginContact(b2Contact* contact);		// Called when one body begins to collide with another
	void				EndContact(b2Contact* contact);			// Called when one body drops colliding with another
};

// Glopbally defined collision collback used to marshal cross actor collision calls
extern CzBox2dCcollisionCallback	g_CzBox2dCcollisionCallback;


//
//
//
//
// CzBox2dCollidable - Represents a collection of begin and end contact collisions. Objects that are part of the physics system usually derive from this class
//
//
//
//
//#define CZ_BOX2D_KEEP_CONTACTS
class CzBox2dCollidable
{
public:
	// Properties
protected:
	void*			UserData;									// Contains user data (usually container actor)
	CzSlotArray<CzBox2dCollidable*> CollisionsStarted;			// List of collidables that have just come into contact with this collidable
	CzSlotArray<CzBox2dCollidable*> CollisionsEnded;			// List of collidables that have just lost contact with this collidable
#if defined(CZ_BOX2D_KEEP_CONTACTS)
	CzSlotArray<CzBox2dCollidable*> CollisionContacts;			// List of collidables that this one is in collision with
#endif // CZ_BOX2D_KEEP_CONTACTS
public:
	virtual void	addCollisionStarted(CzBox2dCollidable* with)
	{
		CollisionsStarted.add(with);
#if defined(CZ_BOX2D_KEEP_CONTACTS)
		CollisionContacts.add(with);
#endif // CZ_BOX2D_KEEP_CONTACTS
	}
	void			clearCollisionsStarted()			{ CollisionsStarted.clear(); }
	int				getCollisionsStartedCount()			{ return CollisionsStarted.count(); }
	virtual void	addCollisionEnded(CzBox2dCollidable* with)
	{
		CollisionsEnded.add(with);
#if defined(CZ_BOX2D_KEEP_CONTACTS)
		CollisionContacts.remove(with);
#endif // CZ_BOX2D_KEEP_CONTACTS
	}
	void			clearCollisionsEnded()				{ CollisionsEnded.clear(); }
	int				getCollisionsEndedCount()			{ return CollisionsEnded.count(); }
#if defined(CZ_BOX2D_KEEP_CONTACTS)
	void			clearCollisionContacts()			{ CollisionContacts.clear(); }
	int				getCollisionContactsCount()			{ return CollisionContacts.count(); }
#endif // CZ_BOX2D_KEEP_CONTACTS

	void			clearAllCollisions()
	{
		CollisionsStarted.clear();
		CollisionsEnded.clear();
#if defined(CZ_BOX2D_KEEP_CONTACTS)
		clearCollisionContacts();
#endif // CZ_BOX2D_KEEP_CONTACTS
	}
	CzSlotArray<CzBox2dCollidable*>& getCollisionsStarted()		{ return CollisionsStarted; }
	CzSlotArray<CzBox2dCollidable*>& getCollisionsEnded()		{ return CollisionsEnded; }
#if defined(CZ_BOX2D_KEEP_CONTACTS)
	CzSlotArray<CzBox2dCollidable*>& getCollisionContacts()		{ return CollisionContacts; }
#endif // CZ_BOX2D_KEEP_CONTACTS
	void				setUserData(void* user_data)	{ UserData = user_data; }
	void*				getUserData()					{ return UserData; }
	// End of properties

	CzBox2dCollidable() : UserData(NULL) {}

	virtual ~CzBox2dCollidable()
	{
		clearAllCollisions();
	}
};


//
//
//
//
// CzBox2dWorld - Box2D physics world
//
//
//
//
class CzBox2dWorld
{
public:

protected:
	// Properties
	b2World*				World;					// Physical world
	CzVec2					WorldScale;				// Scaling between physical and viusal worlds (set to 0, 0 to disable physics update)
	int						VelocityIterations;		// Number of internal iterations used when computing velocities
	int						PositionIterations;		// Number of internal iterations used when computing positions
	CzVec2					Gravity;				// Scene gravity
	float					TimeStep;				// Physics time step in seconds
public:
	void					setWorldScale(float x, float y)			{ WorldScale.x = x; WorldScale.y = y; }
	CzVec2					getWorldScale()							{ return WorldScale; }
	b2World*				getWorld()								{ return World; }
	void					setVelocityIterations(int count)		{ VelocityIterations = count; }
	int						getVelocityIterations() const			{ return VelocityIterations; }
	void					setPositionIterations(int count)		{ PositionIterations = count; }
	int						getPositionIterations() const			{ return PositionIterations; }
	void					setGravity(float x, float y);
	CzVec2					getGravity() const						{ return Gravity; }
	// Properties end

public:
	CzBox2dWorld() : World(NULL), WorldScale(10.0f, 10.0f), VelocityIterations(6), PositionIterations(3), Gravity(0.0f, 15.0f) {}
	virtual ~CzBox2dWorld();

	// World initialisation
	virtual void			InitWorld(bool doSleep = true);

	// World update
	virtual void			UpdateWorld(float dt);

	// Box2D world to pixel conversions
	float			WorldToPixelX(float x)						{ return x * WorldScale.x; }
	float			WorldToPixelY(float y)						{ return y * WorldScale.y; }
	float			PixelToWorldX(float x)						{ return x / WorldScale.x; }
	float			PixelToWorldY(float y)						{ return y / WorldScale.y; }

};


//
//
//
//
// CzBox2dBody - Box2D physics body
//
//
//
//
 class CzActor;
class CzBox2dBody :  public CzBox2dCollidable
{
public:
protected:
	// Properties
	CzBox2dWorld*			World;					// World that this body is attached to
	b2Body*					Body;					// Physical body
	CzVector<b2Fixture*>	Fixtures;				// The physical fixtures (managed by Box2D)
	CzBox2dMaterial*		BodyMaterial;			// Physical body attributes that are applied to this actors body
	CzVector<CzShape*>		BodyShapes;				// Physical body shapes
	CzVector<IzBox2dJoint*>	Joints;					// Physical joints
	int						CollisionFlags;			// Collision flags
public:
	void				setWorld(CzBox2dWorld* world)			{ World = world; }
	CzBox2dWorld*		getWorld()								{ return World; }
	b2Body*				getBody()								{ return Body; }
	b2Fixture*			addFixture(CzBox2dMaterial* body_mat, float width, float height, float com_x, float com_y);
	b2Fixture*			addFixture(CzShape* body_shape, CzBox2dMaterial* body_mat, float com_x, float com_y);
	int					getFixturesCount()						{ return Fixtures.size(); }
	b2Fixture*			getFixture(int index)					{ return Fixtures[index]; }
	void				DestroyFixtures();
	void				setBodyMaterial(CzBox2dMaterial* mat);	
	CzBox2dMaterial*	getBodyMaterial()						{ return BodyMaterial; }
	CzVector<IzBox2dJoint*>& getJointsList()					{ return Joints; }
	void				addJoint(IzBox2dJoint* joint)			{ Joints.push_back(joint); }
	IzBox2dJoint*		getJoint(int index);
	IzBox2dJoint*		findJoint(unsigned int name_hash);
	bool				removeJoint(IzBox2dJoint* joint);
	void				setAsSensor(bool enable);
	bool				isSensor() const;
	void				setCollisionFlags(int category_flags, int mask_flags, int collision_group);
	int					getCollisionFlags()	const				{ return CollisionFlags; }
	int					getCollisionCategory() const;
	int					getCollisionMask() const;
	int					getCollisionGroup() const;
	void				SetAwake(bool awake)					{ Body->SetAwake(awake); }
	bool				IsAwake() const							{ return Body->IsAwake(); }
	void				SetActive(bool active)					{ Body->SetActive(active); }
	bool				IsActive() const						{ return Body->IsActive(); }
	// Properties end

public:
	CzBox2dBody() : CzBox2dCollidable(), World(NULL), Body(NULL), BodyMaterial(NULL), CollisionFlags(0)			{}
	virtual ~CzBox2dBody();

	// Initialise the physical body
	void				InitBody(CzBox2dWorld* world, CzShape* body_shape, CzBox2dMaterial* body_mat, CzVec2* pos, float angle, float com_x, float com_y);

	void				ReleaseBody();

	// Force application
	void				ApplyForce(float force_x, float force_y, float world_pos_x, float world_pos_y);
	void				ApplyForceToCenter(float force_x, float force_y);
	void				ApplyTorque(float torque);
	void				ApplyLinearImpulse(float impulse_x, float impulse_y, float world_pos_x, float world_pos_y);
	void				ApplyAngularImpulse(float impulse);
};


#endif // _CZ_BOX2D_H_
