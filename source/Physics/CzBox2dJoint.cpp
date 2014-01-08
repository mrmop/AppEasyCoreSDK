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
#include "CzBox2dJoint.h"
#include "CzActor.h"
#include "CzScene.h"
#include "CzResources.h"
#include "CzHashes.h"
#include "CzMath.h"
#include "CzXomlResourceManager.h"


//
//
//
//
// CzBox2dJoint implementation
//
//
//
//

void CzBox2dJointWeld::Create(CzBox2dWorld* world)
{
	b2WeldJointDef jointDef;

	jointDef.bodyA = BodyA->getBody();
	jointDef.bodyB = BodyB->getBody();
	jointDef.localAnchorA = BodyA->getBody()->GetLocalPoint(AnchorA);
	jointDef.localAnchorB = BodyB->getBody()->GetLocalPoint(AnchorB);
	if (ReferenceAngle == 999999.0f)
		jointDef.referenceAngle = BodyB->getBody()->GetAngle() - BodyA->getBody()->GetAngle();
	else
		jointDef.referenceAngle = CzMath::DegToRad(ReferenceAngle);			// Degrees to radians;
	jointDef.collideConnected = SelfCollide;

	Joint = (b2WeldJoint*)world->getWorld()->CreateJoint(&jointDef);
}

void CzBox2dJointDistance::Create(CzBox2dWorld* world)
{
	b2DistanceJointDef jointDef;

	jointDef.bodyA = BodyA->getBody();
	jointDef.bodyB = BodyB->getBody();
	jointDef.localAnchorA = BodyA->getBody()->GetLocalPoint(AnchorA);
	jointDef.localAnchorB = BodyB->getBody()->GetLocalPoint(AnchorB);
	jointDef.collideConnected = SelfCollide;
	if (Length > 0)
		jointDef.length = Length;
	else
		jointDef.length = (AnchorB - AnchorA).Length();
	if (Frequency >= 0)
		jointDef.frequencyHz = Frequency;
	if (Damping >= 0)
		jointDef.dampingRatio = Damping;

	Joint = (b2DistanceJoint*)world->getWorld()->CreateJoint(&jointDef);
}

void CzBox2dJointRevolute::Create(CzBox2dWorld* world)
{
	b2RevoluteJointDef jointDef;

	jointDef.bodyA = BodyA->getBody();
	jointDef.bodyB = BodyB->getBody();
	jointDef.localAnchorA = BodyA->getBody()->GetLocalPoint(AnchorA);
	jointDef.localAnchorB = BodyB->getBody()->GetLocalPoint(AnchorB);
	if (ReferenceAngle == 999999.0f)
		jointDef.referenceAngle = BodyB->getBody()->GetAngle() - BodyA->getBody()->GetAngle();
	else
		jointDef.referenceAngle = CzMath::DegToRad(ReferenceAngle);			// Degrees to radians;
	jointDef.collideConnected = SelfCollide;

	if (LimitJoint)
	{
		jointDef.enableLimit = true;
		jointDef.lowerAngle = CzMath::DegToRad(LowerAngle);					// Degrees to radians
		jointDef.upperAngle = CzMath::DegToRad(UpperAngle);					// Degrees to radians
	}
	if (MotorEnabled)
	{
		jointDef.enableMotor = true;
		jointDef.motorSpeed = MotorSpeed;
		jointDef.maxMotorTorque = MaxMotorTorque;
	}

	Joint = (b2RevoluteJoint*)world->getWorld()->CreateJoint(&jointDef);
}

void CzBox2dJointPrismatic::Create(CzBox2dWorld* world)
{
	b2PrismaticJointDef jointDef;

	jointDef.bodyA = BodyA->getBody();
	jointDef.bodyB = BodyB->getBody();
	jointDef.localAnchorA = BodyA->getBody()->GetLocalPoint(AnchorA);
	jointDef.localAnchorB = BodyB->getBody()->GetLocalPoint(AnchorB);
	if (ReferenceAngle == 999999.0f)
		jointDef.referenceAngle = BodyB->getBody()->GetAngle() - BodyA->getBody()->GetAngle();
	else
		jointDef.referenceAngle = CzMath::DegToRad(ReferenceAngle);			// Degrees to radians;
	jointDef.collideConnected = SelfCollide;
	jointDef.localAxisA = BodyA->getBody()->GetLocalVector(Axis);

	if (LimitJoint)
	{
		jointDef.enableLimit = true;
		jointDef.lowerTranslation = LowerTranslation;
		jointDef.upperTranslation = UpperTranslation;
	}
	if (MotorEnabled)
	{
		jointDef.enableMotor = true;
		jointDef.motorSpeed = MotorSpeed;
		jointDef.maxMotorForce = MaxMotorForce;
	}

	Joint = (b2PrismaticJoint*)world->getWorld()->CreateJoint(&jointDef);
}

void CzBox2dJointPulley::Create(CzBox2dWorld* world)
{
	b2PulleyJointDef jointDef;

	jointDef.bodyA = BodyA->getBody();
	jointDef.bodyB = BodyB->getBody();
	jointDef.localAnchorA = BodyA->getBody()->GetLocalPoint(AnchorA);
	jointDef.localAnchorB = BodyB->getBody()->GetLocalPoint(AnchorB);
	jointDef.groundAnchorA = GroundAnchorA;
	jointDef.groundAnchorB = GroundAnchorB;
	if (LengthA > 0)
		jointDef.lengthA = LengthA;
	else
		jointDef.lengthA = (AnchorA - GroundAnchorA).Length();
	if (LengthB > 0)
		jointDef.lengthB = LengthB;
	else
		jointDef.lengthB = (AnchorB - GroundAnchorB).Length();
	jointDef.ratio = Ratio;
	jointDef.collideConnected = SelfCollide;

	Joint = (b2PulleyJoint*)world->getWorld()->CreateJoint(&jointDef);
}

void CzBox2dJointWheel::Create(CzBox2dWorld* world)
{
	b2WheelJointDef jointDef;

	jointDef.bodyA = BodyA->getBody();
	jointDef.bodyB = BodyB->getBody();
	jointDef.localAnchorA = BodyA->getBody()->GetLocalPoint(AnchorA);
	jointDef.localAnchorB = BodyB->getBody()->GetLocalPoint(AnchorB);
	jointDef.collideConnected = SelfCollide;
	jointDef.dampingRatio = Damping;
	jointDef.frequencyHz = Frequency;
	jointDef.localAxisA = BodyA->getBody()->GetLocalVector(Axis);

	if (MotorEnabled)
	{
		jointDef.enableMotor = true;
		jointDef.motorSpeed = MotorSpeed;
		jointDef.maxMotorTorque = MaxMotorTorque;
	}

	Joint = (b2WheelJoint*)world->getWorld()->CreateJoint(&jointDef);
}

int CzBox2dJoints::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	CzActor* actor = NULL;
	CzScene* scene = NULL;
	if (parent->getClassTypeHash() == CzHashes::Actor_Hash)
	{
		actor = (CzActor*)parent;
		scene = actor->getScene();
	}
	else
	if (parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Box2dJoints - Joints need to be declared inside another actor or scene", DebugInfo.c_str());
		return 0;
	}

	for (CzXmlNode::_Iterator it = node->begin(); it != node->end(); ++it)
	{
		unsigned int name_hash = (*it)->GetName().getHash();
		if (name_hash == CzHashes::Joint_Hash)
		{
			unsigned int	type = 0;
			CzString*		name = NULL;
			CzString*		actor_a = NULL;
			CzString*		actor_b = NULL;
			CzVec2			offset_a(0, 0), offset_b(0, 0), axis(0, 1.0f), ground_a(0, 0), ground_b(0, 0);
			bool			self_collide = false;
			float			length = -1, frequency = -1, damping = -1, length_a = -1, length_b = -1, ratio = 1;
			bool			motor = false, limit = false;
			float			lower_limit = 0, upper_limit = 0, max_torque = 0, max_force = 0, motor_speed = 0, ref_angle = 999999.0f;

			// Get joint data
			for (CzXmlNode::_AttribIterator it3 = (*it)->attribs_begin(); it3 != (*it)->attribs_end(); ++it3)
			{
				unsigned int attrib_hash = (*it3)->getName().getHash();

				if (attrib_hash == CzHashes::Name_Hash)
					name = &(*it3)->getValue();
				else
				if (attrib_hash == CzHashes::Type_Hash)
					type = (*it3)->getValue().getHash();
				else
				if (attrib_hash == CzHashes::ActorA_Hash)
					actor_a = &(*it3)->getValue();
				else
				if (attrib_hash == CzHashes::ActorB_Hash)
					actor_b = &(*it3)->getValue();
				else
				if (attrib_hash == CzHashes::OffsetA_Hash)
				{
					if (!(*it3)->getValueAsPoint(offset_a))
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Box2dJoints - Invalid value for Joint::OffsetA, expected vec2", DebugInfo.c_str());
				}
				else
				if (attrib_hash == CzHashes::OffsetB_Hash)
				{
					if (!(*it3)->getValueAsPoint(offset_b))
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Box2dJoints - Invalid value for Joint::OffsetB, expected vec2", DebugInfo.c_str());
				}
				else
				if (attrib_hash == CzHashes::GroundA_Hash)
				{
					if (!(*it3)->getValueAsPoint(ground_a))
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Box2dJoints - Invalid value for Joint::GroundA, expected vec2", DebugInfo.c_str());
				}
				else
				if (attrib_hash == CzHashes::GroundB_Hash)
				{
					if (!(*it3)->getValueAsPoint(ground_b))
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Box2dJoints - Invalid value for Joint::GroundB, expected vec2", DebugInfo.c_str());
				}
				else
				if (attrib_hash == CzHashes::SelfCollide_Hash)
					self_collide = (*it3)->getValueAsBool();
				else
				if (attrib_hash == CzHashes::Length_Hash)
					length = (*it3)->getValueAsFloat();
				else
				if (attrib_hash == CzHashes::LengthA_Hash)
					length_a = (*it3)->getValueAsFloat();
				else
				if (attrib_hash == CzHashes::LengthB_Hash)
					length_b = (*it3)->getValueAsFloat();
				else
				if (attrib_hash == CzHashes::Frequency_Hash)
					frequency = (*it3)->getValueAsFloat();
				else
				if (attrib_hash == CzHashes::Damping_Hash)
					damping = (*it3)->getValueAsFloat();
				else
				if (attrib_hash == CzHashes::MotorEnabled_Hash)
					motor = (*it3)->getValueAsBool();
				else
				if (attrib_hash == CzHashes::LimitJoint_Hash)
					limit = (*it3)->getValueAsBool();
				else
				if (attrib_hash == CzHashes::LowerLimit_Hash)
					lower_limit = (*it3)->getValueAsFloat();
				else
				if (attrib_hash == CzHashes::UpperLimit_Hash)
					upper_limit = (*it3)->getValueAsFloat();
				else
				if (attrib_hash == CzHashes::MotorSpeed_Hash)
					motor_speed = (*it3)->getValueAsFloat();
				else
				if (attrib_hash == CzHashes::MaxMotorTorque_Hash)
					max_torque = (*it3)->getValueAsFloat();
				else
				if (attrib_hash == CzHashes::MaxMotorForce_Hash)
					max_force = (*it3)->getValueAsFloat();
				else
				if (attrib_hash == CzHashes::ReferenceAngle_Hash)
					ref_angle = (*it3)->getValueAsFloat();
				else
				if (attrib_hash == CzHashes::Ratio_Hash)
					ratio = (*it3)->getValueAsFloat();
				else
				if (attrib_hash == CzHashes::Axis_Hash)
				{
					if (!(*it3)->getValueAsPoint(axis))
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Box2dJoints - Invalid value for Joint::Axis, expected vec2", DebugInfo.c_str());
				}
			}

			if (actor_a != NULL)
			{
				actor = scene->findActor(actor_a->c_str());
				if (actor == NULL)
				{
					CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Box2dJoints - Joints actor_a does not exist", DebugInfo.c_str());
					return 0;
				}
			}

			if (actor == NULL)
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Box2dJoints - Joint - No valid ActorA", DebugInfo.c_str());
				return 0;
			}
			
			if (actor->getBox2dBody() == NULL)
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Box2dJoints - actor has no physics body assigned - ", DebugInfo.c_str());
				return 0;
			}

			CzBox2dWorld* world = actor->getBox2dBody()->getWorld();
			if (actor_b == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Box2dJoints - ActorB does not exist in the scene - ", actor_b->c_str(), DebugInfo.c_str());

			CzActor* actorb = scene->findActor(actor_b->c_str());
			if (actorb != NULL)
			{
				if (type == CzHashes::Weld_Hash)
				{
					CzBox2dJointWeld* joint = new CzBox2dJointWeld();
					joint->ReferenceAngle = ref_angle;
					joint->BodyA = actor->getBox2dBody();
					joint->BodyB = actorb->getBox2dBody();
					b2Vec2 anch_a = joint->BodyA->getBody()->GetWorldCenter();
					anch_a.x += world->PixelToWorldX(offset_a.x);
					anch_a.y += world->PixelToWorldY(offset_a.y);
					b2Vec2 anch_b = joint->BodyB->getBody()->GetWorldCenter();
					anch_b.x += world->PixelToWorldX(offset_b.x);
					anch_b.y += world->PixelToWorldY(offset_b.y);
					joint->AnchorA = anch_a;
					joint->AnchorB = anch_b;
					joint->SelfCollide = self_collide;
					joint->Create(world);
					actor->getBox2dBody()->addJoint(joint);
					if (name != NULL)
						joint->setName(name->c_str());
				}
				else
				if (type == CzHashes::Distance_Hash)
				{
					CzBox2dJointDistance* joint = new CzBox2dJointDistance();
					joint->BodyA = actor->getBox2dBody();
					joint->BodyB = actorb->getBox2dBody();
					b2Vec2 anch_a = joint->BodyA->getBody()->GetWorldCenter();
					anch_a.x += world->PixelToWorldX(offset_a.x);
					anch_a.y += world->PixelToWorldY(offset_a.y);
					b2Vec2 anch_b = joint->BodyB->getBody()->GetWorldCenter();
					anch_b.x += world->PixelToWorldX(offset_b.x);
					anch_b.y += world->PixelToWorldY(offset_b.y);
					joint->AnchorA = anch_a;
					joint->AnchorB = anch_b;
					joint->SelfCollide = self_collide;
					joint->Damping = damping;
					joint->Frequency = frequency;
					if (length > 0)
						joint->Length = world->PixelToWorldX((length));
					joint->Create(world);
					actor->getBox2dBody()->addJoint(joint);
					if (name != NULL)
						joint->setName(name->c_str());
				}
				else
				if (type == CzHashes::Revolute_Hash)
				{
					CzBox2dJointRevolute* joint = new CzBox2dJointRevolute();
					joint->BodyA = actor->getBox2dBody();
					joint->BodyB = actorb->getBox2dBody();
					b2Vec2 anch_a = joint->BodyA->getBody()->GetWorldCenter();
					anch_a.x += world->PixelToWorldX(offset_a.x);
					anch_a.y += world->PixelToWorldY(offset_a.y);
					b2Vec2 anch_b = joint->BodyB->getBody()->GetWorldCenter();
					anch_b.x += world->PixelToWorldX(offset_b.x);
					anch_b.y += world->PixelToWorldY(offset_b.y);
					joint->AnchorA = anch_a;
					joint->AnchorB = anch_b;
					joint->SelfCollide = self_collide;
					joint->LimitJoint = limit;
					joint->MotorEnabled = motor;
					joint->LowerAngle = lower_limit;
					joint->UpperAngle = upper_limit;
					joint->MotorSpeed = motor_speed;
					joint->MaxMotorTorque = max_torque;
					joint->ReferenceAngle = ref_angle;
					joint->Create(world);
					actor->getBox2dBody()->addJoint(joint);
					if (name != NULL)
						joint->setName(name->c_str());
				}
				else
				if (type == CzHashes::Prismatic_Hash)
				{
					CzBox2dJointPrismatic* joint = new CzBox2dJointPrismatic();
					joint->BodyA = actor->getBox2dBody();
					joint->BodyB = actorb->getBox2dBody();
					b2Vec2 anch_a = joint->BodyA->getBody()->GetWorldCenter();
					anch_a.x += world->PixelToWorldX(offset_a.x);
					anch_a.y += world->PixelToWorldY(offset_a.y);
					b2Vec2 anch_b = joint->BodyB->getBody()->GetWorldCenter();
					anch_b.x += world->PixelToWorldX(offset_b.x);
					anch_b.y += world->PixelToWorldY(offset_b.y);
					joint->AnchorA = anch_a;
					joint->AnchorB = anch_b;
					joint->Axis.x = axis.x;
					joint->Axis.y = axis.y;
					joint->SelfCollide = self_collide;
					joint->LimitJoint = limit;
					joint->MotorEnabled = motor;
					joint->LowerTranslation = world->PixelToWorldY(lower_limit);
					joint->UpperTranslation = world->PixelToWorldY(upper_limit);
					joint->MotorSpeed = motor_speed;
					joint->MaxMotorForce = max_force;
					joint->ReferenceAngle = ref_angle;
					joint->Create(world);
					actor->getBox2dBody()->addJoint(joint);
					if (name != NULL)
						joint->setName(name->c_str());
				}
				else
				if (type == CzHashes::Pulley_Hash)
				{
					CzBox2dJointPulley* joint = new CzBox2dJointPulley();
					joint->BodyA = actor->getBox2dBody();
					joint->BodyB = actorb->getBox2dBody();
					b2Vec2 anch_a = joint->BodyA->getBody()->GetWorldCenter();
					anch_a.x += world->PixelToWorldX(offset_a.x);
					anch_a.y += world->PixelToWorldY(offset_a.y);
					b2Vec2 anch_b = joint->BodyB->getBody()->GetWorldCenter();
					anch_b.x += world->PixelToWorldX(offset_b.x);
					anch_b.y += world->PixelToWorldY(offset_b.y);
					b2Vec2 grounda = joint->BodyA->getBody()->GetWorldCenter();
					grounda.x += world->PixelToWorldX(ground_a.x);
					grounda.y += world->PixelToWorldY(ground_a.y);
					b2Vec2 groundb = joint->BodyB->getBody()->GetWorldCenter();
					groundb.x += world->PixelToWorldX(ground_b.x);
					groundb.y += world->PixelToWorldY(ground_b.y);
					joint->AnchorA = anch_a;
					joint->AnchorB = anch_b;
					joint->GroundAnchorA = grounda;
					joint->GroundAnchorB = groundb;
					if (length_a > 0)
						joint->LengthA = length_a;
					if (length_b > 0)
						joint->LengthB = length_b;
					joint->Ratio = ratio;
					joint->SelfCollide = self_collide;
					joint->Create(world);
					actor->getBox2dBody()->addJoint(joint);
					if (name != NULL)
						joint->setName(name->c_str());
				}
				else
				if (type == CzHashes::Wheel_Hash)
				{
					CzBox2dJointWheel* joint = new CzBox2dJointWheel();
					joint->BodyA = actor->getBox2dBody();
					joint->BodyB = actorb->getBox2dBody();
					b2Vec2 anch_a = joint->BodyA->getBody()->GetWorldCenter();
					anch_a.x += world->PixelToWorldX(offset_a.x);
					anch_a.y += world->PixelToWorldY(offset_a.y);
					b2Vec2 anch_b = joint->BodyB->getBody()->GetWorldCenter();
					anch_b.x += world->PixelToWorldX(offset_b.x);
					anch_b.y += world->PixelToWorldY(offset_b.y);
					joint->Axis.x = axis.x;
					joint->Axis.y = axis.y;
					joint->AnchorA = anch_a;
					joint->AnchorB = anch_b;
					joint->SelfCollide = self_collide;
					joint->MotorEnabled = motor;
					joint->MotorSpeed = motor_speed;
					joint->MaxMotorTorque = max_torque;
					joint->Damping = damping;
					joint->Frequency = frequency;
					joint->Create(world);
					actor->getBox2dBody()->addJoint(joint);
					if (name != NULL)
						joint->setName(name->c_str());
				}
			}
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Box2dJoints - ActorB does not exist in the scene - ", actor_b->c_str(), DebugInfo.c_str());

		}
	}

	return -1;
}




