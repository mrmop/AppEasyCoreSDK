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

#if !defined(_CZ_BOX2D_JOINT_H_)
#define _CZ_BOX2D_JOINT_H_

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
// CzBox2dJoint - Box2D physics joint
//
//
//
//
enum eCzBox2dJointType
{
	JT_Weld, 
	JT_Distance, 
	JT_Revolute, 
	JT_Prismatic, 
	JT_Pulley, 
	JT_Wheel, 
};

struct IzBox2dJoint
{
	eCzBox2dJointType	Type;					///< Type of joint
#if defined (_DEBUG)
	CzString			Name;					///< Joint name
#endif	// _DEBUG
	unsigned int		NameHash;				///< Joint name hash
	CzBox2dBody*		BodyA;					///< Body A
	CzBox2dBody*		BodyB;					///< Body B
	b2Vec2				AnchorA;				///< Body A world anchor pos
	b2Vec2				AnchorB;				///< Body B world anchor pos
	bool				SelfCollide;			///< True to allow self collision

	virtual ~IzBox2dJoint() {}
	virtual void		Create(CzBox2dWorld* world) = 0;
	virtual b2Joint*	getJoint() = 0;
	void				setName(const char* name)
	{
#if defined (_DEBUG)
		Name = name;
#endif	// _DEBUG
		NameHash = CzString::CalculateHash(name);
	}
};

struct CzBox2dJointWeld : public IzBox2dJoint
{
	float				ReferenceAngle;		// Reference angle
	b2WeldJoint*		Joint;

	CzBox2dJointWeld() { Type = JT_Distance; }

	// Initialise the joint
	void				Create(CzBox2dWorld* world);
	b2Joint*			getJoint()		{ return Joint; }
};

struct CzBox2dJointDistance : public IzBox2dJoint
{
	float				Length;				// Max length between bosy A and B
	float				Frequency;			// Harmonic oscilation Frequency in Hz
	float				Damping;			// Oscillation damping (1.0 for no oscillation)
	b2DistanceJoint*	Joint;

	CzBox2dJointDistance() : Length(-1), Frequency(-1), Damping(-1) { Type = JT_Distance; }

	// Initialise the joint
	void				Create(CzBox2dWorld* world);
	b2Joint*			getJoint()		{ return Joint; }
};

struct CzBox2dJointRevolute : public IzBox2dJoint
{
	bool				LimitJoint;			// Imited joint rotation
	bool				MotorEnabled;		// Enable motor
	float				LowerAngle;			// Lower rotation limit
	float				UpperAngle;			// Upper rotation limit
	float				MotorSpeed;			// Motor speed
	float				MaxMotorTorque;		// Max motor torque
	float				ReferenceAngle;		// Reference angle
	b2RevoluteJoint*	Joint;

	CzBox2dJointRevolute() : LimitJoint(false), MotorEnabled(false), ReferenceAngle(0) { Type = JT_Revolute; }

	// Initialise the joint
	void				Create(CzBox2dWorld* world);
	b2Joint*			getJoint()		{ return Joint; }
};

struct CzBox2dJointPrismatic : public IzBox2dJoint
{
	b2Vec2				Axis;				// Axis of movement
	bool				LimitJoint;			// Limit joint translation
	bool				MotorEnabled;		// Emable motor
	float				LowerTranslation;	// Lower translation limit
	float				UpperTranslation;	// Upper translation limit
	float				MotorSpeed;			// Motor speed
	float				MaxMotorForce;		// Max motor force
	float				ReferenceAngle;		// Reference angle
	b2PrismaticJoint*	Joint;

	CzBox2dJointPrismatic() : LimitJoint(false), MotorEnabled(false), ReferenceAngle(0) { Type = JT_Prismatic; }

	// Initialise the joint
	void				Create(CzBox2dWorld* world);
	b2Joint*			getJoint()		{ return Joint; }
};

struct CzBox2dJointPulley : public IzBox2dJoint
{
	b2Vec2				GroundAnchorA;	// Body A world ground anchor pos
	b2Vec2				GroundAnchorB;	// Body B world ground anchor pos
	float				LengthA;		// Distance between Body A and Ground Anchor A
	float				LengthB;		// Distance between Body B and Ground Anchor B
	float				Ratio;			// Ratio between rate of change of both sides
	b2PulleyJoint*		Joint;

	CzBox2dJointPulley() : Ratio(1.0f), LengthA(0), LengthB(0) { Type = JT_Pulley; }

	// Initialise the joint
	void				Create(CzBox2dWorld* world);
	b2Joint*			getJoint()		{ return Joint; }
};

struct CzBox2dJointWheel : public IzBox2dJoint
{
	b2Vec2				Axis;			// Axis
	bool				MotorEnabled;	// Motor enabled
	float				MotorSpeed;		// Motor speed
	float				MaxMotorTorque;	// Max motor torque
	float				Frequency;		// Harmonic oscilation Frequency in Hz
	float				Damping;		// Oscillation damping (1.0 for no oscillation)
	b2WheelJoint*		Joint;

	CzBox2dJointWheel() : MotorEnabled(false) { Type = JT_Wheel; }

	// Initialise the joint
	void				Create(CzBox2dWorld* world);
	b2Joint*			getJoint()		{ return Joint; }
};

//
//
//
//
// CzBox2dJoints - Box2D physics joints XOML command
//
//
//
//
class CzBox2dJoints : public IzXomlResource
{
public:
	CzBox2dJoints() { setClassType("joints"); }

	// Implementation of IzXomlResource interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);
};

//
//  CzBox2dJointsCreator - Creates an instance of Box2DJoints object
//
class CzBox2dJointsCreator : public IzXomlClassCreator
{
public:
	CzBox2dJointsCreator()
	{
		setClassName("joints");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzBox2dJoints(); }
};



#endif // _CZ_BOX2D_JOINT_H_
