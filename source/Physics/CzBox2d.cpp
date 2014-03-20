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

#include "CzBox2d.h"
#include "CzActor.h"
#include "CzScene.h"
#include "CzResources.h"
#include "CzHashes.h"
#include "CzMath.h"
#include "CzXomlResourceManager.h"

// Glopbally defined collision collback used to marshal cross actor collision calls
CzBox2dCcollisionCallback	g_CzBox2dCcollisionCallback;

//
//
//
//
// CzBox2dCcollisionCallback - Box2D collision callback
//
// These methods basically notify each actor that a collision has either started or ended
//
//
//
//
void CzBox2dCcollisionCallback::BeginContact(b2Contact* contact)
{
	CzBox2dBody* bodyUserDataA = (CzBox2dBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	CzBox2dBody* bodyUserDataB = (CzBox2dBody*)contact->GetFixtureB()->GetBody()->GetUserData();
	if (bodyUserDataA != NULL && bodyUserDataB != NULL)
	{
		bodyUserDataA->addCollisionStarted(bodyUserDataB);
		bodyUserDataB->addCollisionStarted(bodyUserDataA);
	}
}
 
void CzBox2dCcollisionCallback::EndContact(b2Contact* contact)
{
	CzBox2dBody* bodyUserDataA = (CzBox2dBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	CzBox2dBody* bodyUserDataB = (CzBox2dBody*)contact->GetFixtureB()->GetBody()->GetUserData();
	if (bodyUserDataA != NULL && bodyUserDataB != NULL)
	{
		bodyUserDataA->addCollisionEnded(bodyUserDataB);
		bodyUserDataB->addCollisionEnded(bodyUserDataA);
	}
}

//
//
//
//
// CzBox2dWorld implementation
//
//
//
//
CzBox2dWorld::~CzBox2dWorld()
{
	SAFE_DELETE(World);
}

void CzBox2dWorld::setGravity(float x, float y)
{
	Gravity.x = x; Gravity.y = y;
	if (World != NULL)
		World->SetGravity(b2Vec2(Gravity.x, Gravity.y));
}

void CzBox2dWorld::InitWorld(bool doSleep)
{
	// create Box2D world
	World = new b2World(b2Vec2(Gravity.x, Gravity.y), doSleep);

	// Set up contact collision listener callback (this gets called whever a collision occurs between two bodies)
	World->SetContactListener(&g_CzBox2dCcollisionCallback);
}

void CzBox2dWorld::UpdateWorld(float dt)
{
	if (World != NULL && WorldScale.x != 0)
	{
		// Update the physics engine
		World->Step(dt, VelocityIterations, PositionIterations);
	}
}



//
//
//
//
// CzBox2dBody implementation
//
//
//
//

CzBox2dBody::~CzBox2dBody()
{
	ReleaseBody();
}

void CzBox2dBody::setBodyMaterial(CzBox2dMaterial* mat)
{
	BodyMaterial = mat;
	for (CzVector<b2Fixture*>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it)
	{
		(*it)->SetDensity(mat->getDensity());
		(*it)->SetFriction(mat->getFriction());
		(*it)->SetRestitution(mat->getRestitution());
	}
	if (Body != NULL)
	{
		Body->SetType(mat->getBodyType());
		Body->SetBullet(mat->isBullet());
		Body->SetFixedRotation(mat->isFixedRotation());
		if (mat->getGravityScale() < 1000000.0f)
			Body->SetGravityScale(mat->getGravityScale());
		Body->ResetMassData();
	}
}

b2Fixture* CzBox2dBody::addFixture(CzBox2dMaterial* body_mat, float width, float height, float com_x, float com_y)
{
	if (World == NULL || Body == NULL)
		return NULL;

	// Add basic fixture to the body
	b2Vec2 cm;
	cm.x = World->PixelToWorldX(com_x);
	cm.y = World->PixelToWorldX(com_y);
	b2FixtureDef fixture1;
	b2PolygonShape poly_shape;

	fixture1.shape = &poly_shape;
	poly_shape.SetAsBox(World->PixelToWorldX(width) / 2.0f, World->PixelToWorldY(height) / 2.0f, cm, 0);

	if (body_mat != NULL)
	{
		fixture1.density = body_mat->getDensity();
		fixture1.friction = body_mat->getFriction();
		fixture1.restitution = body_mat->getRestitution();
	}
	else
	{
		fixture1.density = 1.0f;
		fixture1.friction = 1.0f;
		fixture1.restitution = 0.1f;
	}
	b2Fixture* fixture = Body->CreateFixture(&fixture1);
	Fixtures.push_back(fixture);
	
	return fixture;
}

b2Fixture* CzBox2dBody::addFixture(CzShape* body_shape, CzBox2dMaterial* body_mat, float com_x, float com_y)
{
	if (World == NULL || Body == NULL)
	{
//		BodyShape = body_shape;
		return NULL;
	}

	// Add basic fixture to the body
	b2Vec2 cm;
	cm.x = World->PixelToWorldX(com_x);
	cm.y = World->PixelToWorldX(com_y);
	b2FixtureDef fixture1;
	b2PolygonShape poly_shape;
	b2CircleShape circ_shape;

	switch (body_shape->getShape()->ShapeType)
	{
	case CzGeomShape::ST_Box:
		{
			CzGeomShapeBox* shp = (CzGeomShapeBox*)body_shape->getShape();
			fixture1.shape = &poly_shape;
			poly_shape.SetAsBox(World->PixelToWorldX(shp->Width) / 2.0f, World->PixelToWorldY(shp->Height) / 2.0f, cm, 0);
		}
		break;
	case CzGeomShape::ST_Circle:
		{
			CzGeomShapeCircle* shp = (CzGeomShapeCircle*)body_shape->getShape();
			fixture1.shape = &circ_shape;
			circ_shape.m_radius = World->PixelToWorldX(shp->Radius);
			circ_shape.m_p.x = cm.x;
			circ_shape.m_p.y = cm.y;
		}
		break;
	case CzGeomShape::ST_Polygon:
		{
			CzGeomShapePolygon* shp = (CzGeomShapePolygon*)body_shape->getShape();
			fixture1.shape = &poly_shape;
			poly_shape.m_vertexCount = shp->NumVertices;
			CzVec2* points = shp->Vertices;
			int count = shp->NumVertices;
			if (count > 8)
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Fixture shape has more than 8 vertices - ", body_shape->getName().c_str());
				count = 8;
			}
			b2Vec2 verts[8];
			for (int t = 0; t < count; t++)
			{
				verts[t].x = World->PixelToWorldX(points->x) + cm.x;
				verts[t].y = World->PixelToWorldY(points->y) + cm.y;
				points++;
			}
			poly_shape.Set(verts, count);
		}
		break;
	}

	if (body_mat != NULL)
	{
		fixture1.density = body_mat->getDensity();
		fixture1.friction = body_mat->getFriction();
		fixture1.restitution = body_mat->getRestitution();
	}
	else
	{
		fixture1.density = 1.0f;
		fixture1.friction = 1.0f;
		fixture1.restitution = 0.1f;
	}
	b2Fixture* fixture = Body->CreateFixture(&fixture1);
	Fixtures.push_back(fixture);
	
	return fixture;
}

void CzBox2dBody::DestroyFixtures()
{
	// Destroy old fixtures
	for (CzVector<b2Fixture*>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it)
		Body->DestroyFixture(*it);
	Fixtures.clear();
}


void CzBox2dBody::setAsSensor(bool enable)
{
	for (CzVector<b2Fixture*>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it)
		(*it)->SetSensor(enable);
}

bool CzBox2dBody::isSensor() const
{
	for (CzVector<b2Fixture*>::const_iterator it = Fixtures.begin(); it != Fixtures.end(); ++it)
		if ((*it)->IsSensor())
			return true;
	
	return false;
}

void CzBox2dBody::setCollisionFlags(int category_flags, int mask_flags, int collision_group)
{
	b2Filter f;
	f.categoryBits = category_flags;
	f.maskBits = mask_flags;
	f.groupIndex = collision_group;

	for (CzVector<b2Fixture*>::iterator it = Fixtures.begin(); it != Fixtures.end(); ++it)
		(*it)->SetFilterData(f);
	CollisionFlags = (category_flags << 16) | (mask_flags << 8) | (collision_group);
}

IzBox2dJoint* CzBox2dBody::getJoint(int index)
{
	if (index < 0 || index > (int)Joints.size())
		return NULL;

	return Joints[index];
}

IzBox2dJoint* CzBox2dBody::findJoint(unsigned int name_hash)
{
	for (CzVector<IzBox2dJoint*>::iterator it = Joints.begin(); it != Joints.end(); ++it)
	{
		if ((*it)->NameHash == name_hash)
			return *it;
	}

	return NULL;
}

bool CzBox2dBody::removeJoint(IzBox2dJoint* joint)
{
	Body->GetWorld()->DestroyJoint((b2Joint*)joint->getJoint());

	for (CzVector<IzBox2dJoint*>::iterator it = Joints.begin(); it != Joints.end(); ++it)
	{
		if (*it == joint)
		{
			delete *it;
			Joints.erase(it);
			return true;
		}
	}

	return false;
}

int	CzBox2dBody::getCollisionCategory() const
{
	if (Fixtures.size() == 0)
		return 0;

	return (*Fixtures.begin())->GetFilterData().categoryBits;
}

int	CzBox2dBody::getCollisionMask() const
{
	if (Fixtures.size() == 0)
		return 0;

	return (*Fixtures.begin())->GetFilterData().maskBits;
}

int	CzBox2dBody::getCollisionGroup() const
{
	if (Fixtures.size() == 0)
		return 0;

	return (*Fixtures.begin())->GetFilterData().groupIndex;
}

void CzBox2dBody::InitBody(CzBox2dWorld* world, CzShape* body_shape, CzBox2dMaterial* body_mat, CzVec2* pos, float angle, float com_x, float com_y)
{
	if (world == NULL)
		return;

	// Set the box2d world
	World = world;

	b2BodyDef body_def;
	if (body_mat != NULL)
	{
		body_def.type = body_mat->getBodyType();
		body_def.position.Set(World->PixelToWorldX(pos->x), World->PixelToWorldX(pos->y));
		body_def.angle = CzMath::DegToRad(-angle);			// Degrees to radians
		Body = World->getWorld()->CreateBody(&body_def);	
		Body->SetUserData((void*)this);
		Body->SetBullet(body_mat->isBullet());
		Body->SetFixedRotation(body_mat->isFixedRotation());
		if (body_mat->getGravityScale() < 1000000.0f)
			Body->SetGravityScale(body_mat->getGravityScale());
	}
	else
	{
		body_def.type = b2_staticBody;
		body_def.position.Set(World->PixelToWorldX(pos->x), World->PixelToWorldX(pos->y));
		body_def.angle = CzMath::DegToRad(-angle);			// Degrees to radians
		Body = World->getWorld()->CreateBody(&body_def);	
		Body->SetUserData((void*)this);
	}

	if (body_shape != NULL)
		addFixture(body_shape, body_mat, com_x, com_y);
}

void CzBox2dBody::ReleaseBody()
{
	// Destroy fixtures
	DestroyFixtures();

	// Remove joints
	for (CzVector<IzBox2dJoint*>::iterator it = Joints.begin(); it != Joints.end(); ++it)
	{
		delete *it;
	}
	Joints.clear();

	// Destroy the body
	if (Body != NULL)
	{
		World->getWorld()->DestroyBody(Body);
		Body = NULL;
	}
}

void CzBox2dBody::ApplyForce(float force_x, float force_y, float world_pos_x, float world_pos_y)
{
	if (Body != NULL)
		Body->ApplyForce(b2Vec2(force_x, force_y), b2Vec2(world_pos_x, world_pos_y));
}
void CzBox2dBody::ApplyForceToCenter(float force_x, float force_y)
{
	if (Body != NULL)
		Body->ApplyForceToCenter(b2Vec2(force_x, force_y));
}
void CzBox2dBody::ApplyTorque(float torque)
{
	if (Body != NULL)
		Body->ApplyTorque(torque);
}
void CzBox2dBody::ApplyLinearImpulse(float impulse_x, float impulse_y, float world_pos_x, float world_pos_y)
{
	if (Body != NULL)
		Body->ApplyLinearImpulse(b2Vec2(impulse_x, impulse_y), b2Vec2(world_pos_x, world_pos_y));
}
void CzBox2dBody::ApplyAngularImpulse(float impulse)
{
	if (Body != NULL)
		Body->ApplyAngularImpulse(impulse);
}



