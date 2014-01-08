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

#include "CzActorParticles.h"
#include "CzScene.h"
#include "CzCamera.h"
#include "CzXoml.h"
#include "CzActions.h"
#include "CzHashes.h"
#include "CzMath.h"
#include "CzDummySprite.h"
#include "CzXomlResourceManager.h"

CzXomlClassDef* CzActorParticle::ActorParticleClassDef = NULL;

/**
 @fn	void CzActorParticle::InitClass()

 @brief	Initialises the class.
    
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzActorParticle::InitClass()
{
	// Create class properties
	ActorParticleClassDef = new CzXomlClassDef();
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("Position",					PT_Vec2,		CzActorParticle::_setPosition,				CzActorParticle::_getPosition));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("Size",						PT_Vec2,		CzActorParticle::_setSize,					CzActorParticle::_getSize));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("Velocity",					PT_Vec2,		CzActorParticle::_setVelocity,				CzActorParticle::_getVelocity));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("VelocityDamping",			PT_Vec2,		CzActorParticle::_setVelocityDamping,		CzActorParticle::_getVelocityDamping));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("Gravity",					PT_Vec2,		CzActorParticle::_setGravity,				CzActorParticle::_getGravity));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("Scale",						PT_Vec2,		CzActorParticle::_setScale,					CzActorParticle::_getScale));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("ScaleVelocity",				PT_Vec2,		CzActorParticle::_setScaleVelocity,			CzActorParticle::_getScaleVelocity));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("ScaleVelocityDamping",		PT_Vec2,		CzActorParticle::_setScaleVelocityDamping,	CzActorParticle::_getScaleVelocityDamping));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("Angle",						PT_Float,		CzActorParticle::_setAngle,					CzActorParticle::_getAngle));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("AngularVelocity",			PT_Float,		CzActorParticle::_setAngularVelocity,		CzActorParticle::_getAngularVelocity));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("AngularVelocityDamping",	PT_Float,		CzActorParticle::_setAngularVelocityDamping,CzActorParticle::_getAngularVelocityDamping));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("Colour",					PT_Vec4,		CzActorParticle::_setColour,				CzActorParticle::_getColour));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("ColourVelocity",			PT_Vec4,		CzActorParticle::_setColourVelocity,		CzActorParticle::_getColourVelocity));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("ColourVelocityDamping",		PT_Vec4,		CzActorParticle::_setColourVelocityDamping,	CzActorParticle::_getColourVelocityDamping));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("Depth",						PT_Float,		CzActorParticle::_setDepth,					CzActorParticle::_getDepth));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("DepthVelocity",				PT_Float,		CzActorParticle::_setDepthVelocity,			CzActorParticle::_getDepthVelocity));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("DepthVelocityDamping",		PT_Float,		CzActorParticle::_setDepthVelocityDamping,	CzActorParticle::_getDepthVelocityDamping));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("Active",					PT_Bool,		CzActorParticle::_setActive,				CzActorParticle::_getActive));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("Visible",					PT_Bool,		CzActorParticle::_setVisible,				CzActorParticle::_getVisible));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("Duration",					PT_Float,		CzActorParticle::_setLifeSpan,				CzActorParticle::_getLifeSpan));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("SpawnDelay",				PT_Float,		CzActorParticle::_setSpawnDelay,			CzActorParticle::_getSpawnDelay));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("LifeTime",					PT_Float,		CzActorParticle::_setLifeTime,				CzActorParticle::_getLifeTime));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("Repeat",					PT_Int,			CzActorParticle::_setLives,					CzActorParticle::_getLives));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("OPosition",					PT_Vec2,		CzActorParticle::_setOPosition,				CzActorParticle::_getOPosition));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("OVelocity",					PT_Vec2,		CzActorParticle::_setOVelocity,				CzActorParticle::_getOVelocity));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("OScale",					PT_Vec2,		CzActorParticle::_setOScale,				CzActorParticle::_getOScale));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("OScaleVelocity",			PT_Vec2,		CzActorParticle::_setOScaleVelocity,		CzActorParticle::_getOScaleVelocity));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("OAngle",					PT_Float,		CzActorParticle::_setOAngle,				CzActorParticle::_getOAngle));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("OAngularVelocity",			PT_Float,		CzActorParticle::_setOAngularVelocity,		CzActorParticle::_getOAngularVelocity));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("OColour",					PT_Vec4,		CzActorParticle::_setOColour,				CzActorParticle::_getOColour));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("OColourVelocity",			PT_Vec4,		CzActorParticle::_setOColourVelocity,		CzActorParticle::_getOColourVelocity));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("ODepth",					PT_Float,		CzActorParticle::_setODepth,				CzActorParticle::_getODepth));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("ODepthVelocity",			PT_Float,		CzActorParticle::_setODepthVelocity,		CzActorParticle::_getODepthVelocity));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("Attached",					PT_Bool,		CzActorParticle::_setAttached,				CzActorParticle::_getAttached));
	ActorParticleClassDef->addProperty(new CzXomlClassProperty("SrcRect",					PT_Vec4,		CzActorParticle::_setSrcRect,				CzActorParticle::_getSrcRect));
}
void CzActorParticle::ReleaseClass()
{
	if (ActorParticleClassDef != NULL)
	{
		ActorParticleClassDef->Release();
		delete ActorParticleClassDef;
		ActorParticleClassDef = NULL;
	};
}

// //
// //
// //
// //
// //
// //
// //
// //
// XOML property setters and getters
// //
// //
// //
// //
// //
// //
// //
// //

bool CzActorParticle::_setPosition(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
	{
		p->Position.x += prop.p_vec[0];
		p->Position.y += prop.p_vec[1];
	}
	else
	{
		p->Position.x = prop.p_vec[0];
		p->Position.y = prop.p_vec[1];
	}

	return true;
}

CzXomlProperty CzActorParticle::_getPosition(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->Position);
}

bool CzActorParticle::_setSize(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
		p->Visual->setDestSize((int)(p->Visual->getDestWidth() + prop.p_vec[0]), (int)(p->Visual->getDestHeight() + prop.p_vec[1]));
	else
		p->Visual->setDestSize((int)prop.p_vec[0], (int)prop.p_vec[1]);

	return true;
}

CzXomlProperty CzActorParticle::_getSize(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->Visual->getDestSize());
}

bool CzActorParticle::_setVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
	{
		p->Velocity.x += prop.p_vec[0];
		p->Velocity.y += prop.p_vec[1];
	}
	else
	{
		p->Velocity.x = prop.p_vec[0];
		p->Velocity.y = prop.p_vec[1];
	}

	return true;
}

CzXomlProperty CzActorParticle::_getVelocity(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->Velocity);
}

bool CzActorParticle::_setVelocityDamping(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
	{
		p->VelocityDamping.x += prop.p_vec[0];
		p->VelocityDamping.y += prop.p_vec[1];
	}
	else
	{
		p->VelocityDamping.x = prop.p_vec[0];
		p->VelocityDamping.y = prop.p_vec[1];
	}

	return true;
}

CzXomlProperty CzActorParticle::_getVelocityDamping(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->VelocityDamping);
}

bool CzActorParticle::_setGravity(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
	{
		p->Gravity.x += prop.p_vec[0];
		p->Gravity.y += prop.p_vec[1];
	}
	else
	{
		p->Gravity.x = prop.p_vec[0];
		p->Gravity.y = prop.p_vec[1];
	}

	return true;
}

CzXomlProperty CzActorParticle::_getGravity(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->Gravity);
}

bool CzActorParticle::_setScale(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
	{
		p->Scale.x += prop.p_vec[0];
		p->Scale.y += prop.p_vec[1];
	}
	else
	{
		p->Scale.x = prop.p_vec[0];
		p->Scale.y = prop.p_vec[1];
	}

	return true;
}

CzXomlProperty CzActorParticle::_getScale(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->Scale);
}

bool CzActorParticle::_setScaleVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
	{
		p->ScaleVelocity.x += prop.p_vec[0];
		p->ScaleVelocity.y += prop.p_vec[1];
	}
	else
	{
		p->ScaleVelocity.x = prop.p_vec[0];
		p->ScaleVelocity.y = prop.p_vec[1];
	}

	return true;
}

CzXomlProperty CzActorParticle::_getScaleVelocity(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->ScaleVelocity);
}

bool CzActorParticle::_setScaleVelocityDamping(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
	{
		p->ScaleVelocityDamping.x += prop.p_vec[0];
		p->ScaleVelocityDamping.y += prop.p_vec[1];
	}
	else
	{
		p->ScaleVelocityDamping.x = prop.p_vec[0];
		p->ScaleVelocityDamping.y = prop.p_vec[1];
	}

	return true;
}

CzXomlProperty CzActorParticle::_getScaleVelocityDamping(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->ScaleVelocityDamping);
}

bool CzActorParticle::_setAngle(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
		p->Angle += prop.p_float;
	else
		p->Angle = prop.p_float;

	return true;
}

CzXomlProperty CzActorParticle::_getAngle(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->Angle);
}

bool CzActorParticle::_setAngularVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
		p->AngularVelocity += prop.p_float;
	else
		p->AngularVelocity = prop.p_float;

	return true;
}

CzXomlProperty CzActorParticle::_getAngularVelocity(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->AngularVelocity);
}

bool CzActorParticle::_setAngularVelocityDamping(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
		p->AngularVelocityDamping += prop.p_float;
	else
		p->AngularVelocityDamping = prop.p_float;

	return true;
}

CzXomlProperty CzActorParticle::_getAngularVelocityDamping(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->AngularVelocityDamping);
}

bool CzActorParticle::_setColour(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
	{
		p->Colour.x += prop.p_vec[0];
		p->Colour.y += prop.p_vec[1];
		p->Colour.z += prop.p_vec[2];
		p->Colour.w += prop.p_vec[3];
	}
	else
	{
		p->Colour.x = prop.p_vec[0];
		p->Colour.y = prop.p_vec[1];
		p->Colour.z = prop.p_vec[2];
		p->Colour.w = prop.p_vec[3];
	}

	return true;
}

CzXomlProperty CzActorParticle::_getColour(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->Colour);
}

bool CzActorParticle::_setColourVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
	{
		p->ColourVelocity.x += prop.p_vec[0];
		p->ColourVelocity.y += prop.p_vec[1];
		p->ColourVelocity.z += prop.p_vec[2];
		p->ColourVelocity.w += prop.p_vec[3];
	}
	else
	{
		p->ColourVelocity.x = prop.p_vec[0];
		p->ColourVelocity.y = prop.p_vec[1];
		p->ColourVelocity.z = prop.p_vec[2];
		p->ColourVelocity.w = prop.p_vec[3];
	}

	return true;
}

CzXomlProperty CzActorParticle::_getColourVelocity(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->ColourVelocity);
}

bool CzActorParticle::_setColourVelocityDamping(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
	{
		p->ColourVelocityDamping.x += prop.p_vec[0];
		p->ColourVelocityDamping.y += prop.p_vec[1];
		p->ColourVelocityDamping.z += prop.p_vec[2];
		p->ColourVelocityDamping.w += prop.p_vec[3];
	}
	else
	{
		p->ColourVelocityDamping.x = prop.p_vec[0];
		p->ColourVelocityDamping.y = prop.p_vec[1];
		p->ColourVelocityDamping.z = prop.p_vec[2];
		p->ColourVelocityDamping.w = prop.p_vec[3];
	}

	return true;
}

CzXomlProperty CzActorParticle::_getColourVelocityDamping(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->ColourVelocityDamping);
}

bool CzActorParticle::_setDepth(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
		p->Depth += prop.p_float;
	else
		p->Depth = prop.p_float;

	return true;
}

CzXomlProperty CzActorParticle::_getDepth(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->Depth);
}

bool CzActorParticle::_setDepthVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
		p->DepthVelocity += prop.p_float;
	else
		p->DepthVelocity = prop.p_float;

	return true;
}

CzXomlProperty CzActorParticle::_getDepthVelocity(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->DepthVelocity);
}

bool CzActorParticle::_setDepthVelocityDamping(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
		p->DepthVelocityDamping += prop.p_float;
	else
		p->DepthVelocityDamping = prop.p_float;

	return true;
}

CzXomlProperty CzActorParticle::_getDepthVelocityDamping(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->DepthVelocityDamping);
}

bool CzActorParticle::_setActive(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
		p->Active = !p->Active;
	else
		p->Active = prop.p_bool;

	return true;
}

CzXomlProperty CzActorParticle::_getActive(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->Active);
}

bool CzActorParticle::_setVisible(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
		p->Visible = !p->Visible;
	else
		p->Visible = prop.p_bool;

	return true;
}

CzXomlProperty CzActorParticle::_getVisible(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->Visible);
}

bool CzActorParticle::_setLifeSpan(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
		p->LifeSpan += prop.p_float;
	else
		p->LifeSpan = prop.p_float;

	return true;
}

CzXomlProperty CzActorParticle::_getLifeSpan(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->LifeSpan);
}

bool CzActorParticle::_setSpawnDelay(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
		p->SpawnDelay += prop.p_float;
	else
		p->SpawnDelay = prop.p_float;

	return true;
}

CzXomlProperty CzActorParticle::_getSpawnDelay(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->SpawnDelay);
}

bool CzActorParticle::_setLifeTime(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
		p->LifeTime += prop.p_float;
	else
		p->LifeTime = prop.p_float;

	return true;
}

CzXomlProperty CzActorParticle::_getLifeTime(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->LifeTime);
}

bool CzActorParticle::_setLives(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
		p->Lives += prop.p_int;
	else
		p->Lives = prop.p_int;

	return true;
}

CzXomlProperty CzActorParticle::_getLives(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->Lives);
}

bool CzActorParticle::_setOPosition(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
	{
		p->OPosition.x += prop.p_vec[0];
		p->OPosition.y += prop.p_vec[1];
	}
	else
	{
		p->OPosition.x = prop.p_vec[0];
		p->OPosition.y = prop.p_vec[1];
	}

	return true;
}

CzXomlProperty CzActorParticle::_getOPosition(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->OPosition);
}

bool CzActorParticle::_setOVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
	{
		p->OVelocity.x += prop.p_vec[0];
		p->OVelocity.y += prop.p_vec[1];
	}
	else
	{
		p->OVelocity.x = prop.p_vec[0];
		p->OVelocity.y = prop.p_vec[1];
	}

	return true;
}

CzXomlProperty CzActorParticle::_getOVelocity(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->OVelocity);
}

bool CzActorParticle::_setOScale(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
	{
		p->OScale.x += prop.p_vec[0];
		p->OScale.y += prop.p_vec[1];
	}
	else
	{
		p->OScale.x = prop.p_vec[0];
		p->OScale.y = prop.p_vec[1];
	}

	return true;
}

CzXomlProperty CzActorParticle::_getOScale(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->OScale);
}

bool CzActorParticle::_setOScaleVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
	{
		p->OScaleVelocity.x += prop.p_vec[0];
		p->OScaleVelocity.y += prop.p_vec[1];
	}
	else
	{
		p->OScaleVelocity.x = prop.p_vec[0];
		p->OScaleVelocity.y = prop.p_vec[1];
	}

	return true;
}

CzXomlProperty CzActorParticle::_getOScaleVelocity(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->OScaleVelocity);
}

bool CzActorParticle::_setOAngle(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
		p->OAngle += prop.p_float;
	else
		p->OAngle = prop.p_float;

	return true;
}

CzXomlProperty CzActorParticle::_getOAngle(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->OAngle);
}

bool CzActorParticle::_setOAngularVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
		p->OAngularVelocity += prop.p_float;
	else
		p->OAngularVelocity = prop.p_float;

	return true;
}

CzXomlProperty CzActorParticle::_getOAngularVelocity(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->OAngularVelocity);
}

bool CzActorParticle::_setOColour(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
	{
		p->OColour.x += prop.p_vec[0];
		p->OColour.y += prop.p_vec[1];
		p->OColour.z += prop.p_vec[2];
		p->OColour.w += prop.p_vec[3];
	}
	else
	{
		p->OColour.x = prop.p_vec[0];
		p->OColour.y = prop.p_vec[1];
		p->OColour.z = prop.p_vec[2];
		p->OColour.w = prop.p_vec[3];
	}

	return true;
}

CzXomlProperty CzActorParticle::_getOColour(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->OColour);
}

bool CzActorParticle::_setOColourVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
	{
		p->OColourVelocity.x += prop.p_vec[0];
		p->OColourVelocity.y += prop.p_vec[1];
		p->OColourVelocity.z += prop.p_vec[2];
		p->OColourVelocity.w += prop.p_vec[3];
	}
	else
	{
		p->OColourVelocity.x = prop.p_vec[0];
		p->OColourVelocity.y = prop.p_vec[1];
		p->OColourVelocity.z = prop.p_vec[2];
		p->OColourVelocity.w = prop.p_vec[3];
	}

	return true;
}

CzXomlProperty CzActorParticle::_getOColourVelocity(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->OColourVelocity);
}

bool CzActorParticle::_setODepth(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
		p->ODepth += prop.p_float;
	else
		p->ODepth = prop.p_float;

	return true;
}

CzXomlProperty CzActorParticle::_getODepth(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->ODepth);
}

bool CzActorParticle::_setODepthVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
		p->ODepthVelocity += prop.p_float;
	else
		p->ODepthVelocity = prop.p_float;

	return true;
}

CzXomlProperty CzActorParticle::_getODepthVelocity(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->ODepthVelocity);
}

bool CzActorParticle::_setAttached(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
		p->Attached = !p->Attached;
	else
		p->Attached = prop.p_bool;

	return true;
}

CzXomlProperty CzActorParticle::_getAttached(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->Attached);
}

bool CzActorParticle::_setSrcRect(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticle* p = (CzActorParticle*)target;

	if (add)
	{
		CzIRect rc = p->Visual->getSrcRect();
		rc.x += (int)prop.p_vec[0];
		rc.y += (int)prop.p_vec[1];
		rc.w += (int)prop.p_vec[2];
		rc.h += (int)prop.p_vec[3];
		p->Visual->setSrcRect(&rc);
	}
	else
	{
		CzIRect rc = p->Visual->getSrcRect();
		rc.x = (int)prop.p_vec[0];
		rc.y = (int)prop.p_vec[1];
		rc.w = (int)prop.p_vec[2];
		rc.h = (int)prop.p_vec[3];
		p->Visual->setSrcRect(&rc);
	}

	return true;
}

CzXomlProperty CzActorParticle::_getSrcRect(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticle*)target)->Visual->getSrcRect());
}


CzActorParticle::~CzActorParticle()
{
/*	if (Parent->getLinkedTo() == NULL)
	{
		if (Visual != NULL)
			Visual->getManager()->removeSprite(Visual);
	}*/
	if (Visual != NULL)
		Visual->getManager()->removeSprite(Visual);
}

/**
 @fn	bool CzActorParticle::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the property.

 Returns the named property of the particle. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types. 

 @param	property_name		Name of the property.
 @param [out]	prop		The property data that.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActorParticle::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	int err = IzXomlResource::getProperty(ActorParticleClassDef, (IzXomlResource*)this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorParticleClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticle - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), Parent->getDebugInfo().c_str());
	}

	return true;
}

/**
 @fn	bool CzActorParticle::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the particle.
    
 Sets the named property of this particle. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzActorParticle::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	int err = IzXomlResource::setProperty(ActorParticleClassDef, (IzXomlResource*)this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorParticleClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticle - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), Parent->getDebugInfo().c_str());
	}

	return true;
}

/**
 @fn	bool CzActorParticle::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the particle.

 Sets the named property of this particle. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in]	data		The property data.
 @param	delta				true to property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActorParticle::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	int err = IzXomlResource::setProperty(ActorParticleClassDef, (IzXomlResource*)this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorParticleClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticle - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), Parent->getDebugInfo().c_str());
	}

	return true;
}


/**
 @fn	void CzActorParticle::Update(float dt)

 @brief	Updates this particle.

 Updates ths particle, nicluding:
 - applying physics and 
 - Handling life span
 - Updating the visual

 @param	dt	The number of seconds since the app was last updated

*/

void CzActorParticle::Update(float dt)
{
	Visible = false;

	if (Active)
	{
		LifeTime += dt;
		if (LifeTime >= SpawnDelay)
		{
			if (!Attached && (LifeTime - dt) < SpawnDelay && Visual != NULL)
				Parent->getVisual()->getTransform().Transform(&Position, &Position);

			Visible = true;

			// Update velocity
			Position += Velocity * dt;
			Velocity.x *= VelocityDamping.x;
			Velocity.y *= VelocityDamping.y;
			Velocity.x += Gravity.x * dt;
			Velocity.y += Gravity.y * dt;
			Angle += AngularVelocity * dt;
			AngularVelocity *= AngularVelocityDamping;
			Scale += ScaleVelocity * dt;
			ScaleVelocity.x *= ScaleVelocityDamping.x;
			ScaleVelocity.y *= ScaleVelocityDamping.y;
			Colour += ColourVelocity * dt;
			if (Colour.x < 0) Colour.x = 0;
			else
				if (Colour.x > 255) Colour.x = 255;
			if (Colour.y < 0) Colour.y = 0;
			else
				if (Colour.y > 255) Colour.y = 255;
			if (Colour.z < 0) Colour.z = 0;
			else
				if (Colour.z > 255) Colour.z = 255;
			if (Colour.w < 0) Colour.w = 0;
			else
				if (Colour.w > 255) Colour.w = 255;
			ColourVelocity.x *= ColourVelocityDamping.x;
			ColourVelocity.y *= ColourVelocityDamping.y;
			ColourVelocity.z *= ColourVelocityDamping.z;
			ColourVelocity.w *= ColourVelocityDamping.w;
			Depth += DepthVelocity * dt;
			DepthVelocity *= DepthVelocityDamping;

			// Check for timing out
			if (LifeTime >= (LifeSpan + SpawnDelay))
			{
				LifeTime -= (LifeSpan + SpawnDelay);		// Reset life timer
				SpawnDelay = 0;								// Spawn delay should only be used on first spawn
				if (Lives < 0)								// Repeat forever
				{
					Visible = true;
				}
				else
				if (Lives > 0)		// Only reset if enough lives left
				{
					if (Lives == 1)
					{
						// No lives left so hide
						Visible = false;
						Active = false;
					}
					else
						Lives--;
				}

				// Reset the particle
				if (Visible)
				{
/*					if (SpawnDelay == 0)
						Visible = true;
					else
						Visible = false;*/
					CopyFromOld();
					Parent->RegenParticle(this);
				}
			}
		}
	}
}

/**
 @fn	void CzActorParticle::CopyToOld()

 @brief	Makes a copy of particle transient data.

 */

void CzActorParticle::CopyToOld()
{
	OPosition = Position;
	OVelocity = Velocity;
	OScale = Scale;
	OScaleVelocity = ScaleVelocity;
	OAngle = Angle;
	OAngularVelocity = AngularVelocity;
	OColour = Colour;
	OColourVelocity = ColourVelocity;
	ODepth = Depth;
	ODepthVelocity = DepthVelocity;
}

/**
 @fn	void CzActorParticle::CopyFromOld()

 @brief	Restores particle transient data. Used to reset a particle.

 */

void CzActorParticle::CopyFromOld()
{
	Position = OPosition;
	Velocity = OVelocity;
	Scale = OScale;
	ScaleVelocity = OScaleVelocity;
	Angle = OAngle;
	AngularVelocity = OAngularVelocity;
	Colour = OColour;
	ColourVelocity = OColourVelocity;
	Depth = ODepth;
	DepthVelocity = ODepthVelocity;
}

/**
 @fn	CzActorParticle* CzActorParticle::Clone()

 @brief	Clones the oarticle

 @return	null if it fails, else a copy of this particle.
 */

CzActorParticle* CzActorParticle::Clone()
{
	CzActorParticle* clone = new CzActorParticle();
	*clone = *this;
	clone->Visual = NULL;

	return clone;
}

CzXomlClassDef* CzActorParticles::ActorParticlesClassDef = NULL;

/**
 @fn	void CzActorParticles::InitClass()

 @brief	Initialises the class.
    
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzActorParticles::InitClass()
{
	// Create class properties
	ActorParticlesClassDef = new CzXomlClassDef();
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("ParticleCount",	PT_Int,			NULL,									CzActorParticles::_getParticleCount));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("Size",				PT_Vec2,		CzActorParticles::_setSize,				CzActorParticles::_getSize));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("Image",			PT_Resource,	CzActorParticles::_setImage,			CzActorParticles::_getImage));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("PosMode",			PT_String,		CzActorParticles::_setPosMode,			NULL));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("AngMode",			PT_String,		CzActorParticles::_setAngMode,			NULL));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("ScaleMode",		PT_String,		CzActorParticles::_setScaleMode,		NULL));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("DepthMode",		PT_String,		CzActorParticles::_setDepthMode,		NULL));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("AngVelMode",		PT_String,		CzActorParticles::_setAngVelMode,		NULL));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("VelMode",			PT_String,		CzActorParticles::_setVelMode,			NULL));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("ScaleVelMode",		PT_String,		CzActorParticles::_setScaleVelMode,		NULL));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("DepthVelMode",		PT_String,		CzActorParticles::_setDepthVelMode,		NULL));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("PositionRange",	PT_Vec2,		CzActorParticles::_setPositionRange,	NULL));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("AngleRange",		PT_Vec2,		CzActorParticles::_setAngleRange,		NULL));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("ScaleRange",		PT_Vec2,		CzActorParticles::_setScaleRange,		NULL));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("DepthRange",		PT_Vec2,		CzActorParticles::_setDepthRange,		NULL));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("VelRange",			PT_Vec4,		CzActorParticles::_setVelRange,			NULL));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("AngVelRange",		PT_Vec2,		CzActorParticles::_setAngVelRange,		NULL));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("ScaleVelRange",	PT_Vec2,		CzActorParticles::_setScaleVelRange,	NULL));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("DepthVelRange",	PT_Vec2,		CzActorParticles::_setDepthVelRange,	NULL));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("VelocityAll",		PT_Vec2,		CzActorParticles::_setVelocityAll,		NULL));
	ActorParticlesClassDef->addProperty(new CzXomlClassProperty("AutoDelete",		PT_Bool,		CzActorParticles::_setAutoDelete,		CzActorParticles::_getAutoDelete));

}
void CzActorParticles::ReleaseClass()
{
	if (ActorParticlesClassDef != NULL)
	{
		ActorParticlesClassDef->Release();
		delete ActorParticlesClassDef;
		ActorParticlesClassDef = NULL;
	};
}

// //
// //
// //
// //
// //
// //
// //
// //
// XOML property setters and getters
// //
// //
// //
// //
// //
// //
// //
// //

CzXomlProperty CzActorParticles::_getParticleCount(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticles*)target)->getParticleCount());
}

bool CzActorParticles::_setSize(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;

	CzIVec2 size = actor->getSize();

	if (add)
	{
		CzBitmapSprite* visual = (CzBitmapSprite*)actor->getVisual();
		size.x += (int)prop.p_vec[0];
		size.y += (int)prop.p_vec[1];
	}
	else
	{
		size.x = (int)prop.p_vec[0];
		size.y = (int)prop.p_vec[1];
	}
	actor->setOriginalSize(size.x, size.y);
	actor->CalculateSizes(size.x, size.y);
	actor->SetDestSize(size.x, size.y);

	return true;
}

CzXomlProperty CzActorParticles::_getSize(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticles*)target)->getSize());
}

bool CzActorParticles::_setImage(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;
	if (prop.p_data == NULL)
		actor->setImage(NULL);
	else
	{
		CzImage* image;
		if (prop.Type == PT_Resource)
			image = (CzImage*)prop.p_data;
		else
			image = (CzImage*)actor->getScene()->getResourceManager()->findResource(CzString::CalculateHash((const char*)prop.p_data), CzHashes::Image_Hash);
		if (image != NULL)
			actor->setImage(image);
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - setProperty - Image not found in resources - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());
	}

	return true;
}

CzXomlProperty CzActorParticles::_getImage(IzXomlResource* target)
{
	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActorParticles*)target)->getVisual();
	return CzXomlProperty(visual->getImage());
}

bool CzActorParticles::_setPosMode(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;
	if (CzString::CalculateHash((const char*)prop.p_data) == CzHashes::Random_Hash)
		actor->setPositionMode(CzActorParticles::PAM_Random);
	else
		actor->setPositionMode(CzActorParticles::PAM_Normal);
	return true;
}

bool CzActorParticles::_setAngMode(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;
	if (CzString::CalculateHash((const char*)prop.p_data) == CzHashes::Random_Hash)
		actor->setAngleMode(CzActorParticles::PAM_Random);
	else
		actor->setAngleMode(CzActorParticles::PAM_Normal);
	return true;
}

bool CzActorParticles::_setScaleMode(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;
	if (CzString::CalculateHash((const char*)prop.p_data) == CzHashes::Random_Hash)
		actor->setScaleMode(CzActorParticles::PAM_Random);
	else
		actor->setScaleMode(CzActorParticles::PAM_Normal);
	return true;
}

bool CzActorParticles::_setDepthMode(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;
	if (CzString::CalculateHash((const char*)prop.p_data) == CzHashes::Random_Hash)
		actor->setDepthMode(CzActorParticles::PAM_Random);
	else
		actor->setDepthMode(CzActorParticles::PAM_Normal);
	return true;
}

bool CzActorParticles::_setAngVelMode(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;
	if (CzString::CalculateHash((const char*)prop.p_data) == CzHashes::Random_Hash)
		actor->setAngVelocityMode(CzActorParticles::PAM_Random);
	else
		actor->setAngVelocityMode(CzActorParticles::PAM_Normal);
	return true;
}

bool CzActorParticles::_setVelMode(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;
	if (CzString::CalculateHash((const char*)prop.p_data) == CzHashes::Random_Hash)
		actor->setVelocityMode(CzActorParticles::PAM_Random);
	else
		actor->setVelocityMode(CzActorParticles::PAM_Normal);
	return true;
}

bool CzActorParticles::_setScaleVelMode(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;
	if (CzString::CalculateHash((const char*)prop.p_data) == CzHashes::Random_Hash)
		actor->setScaleVelocityMode(CzActorParticles::PAM_Random);
	else
		actor->setScaleVelocityMode(CzActorParticles::PAM_Normal);
	return true;
}

bool CzActorParticles::_setDepthVelMode(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;
	if (CzString::CalculateHash((const char*)prop.p_data) == CzHashes::Random_Hash)
		actor->setDepthVelocityMode(CzActorParticles::PAM_Random);
	else
		actor->setDepthVelocityMode(CzActorParticles::PAM_Normal);
	return true;
}

bool CzActorParticles::_setPositionRange(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;
	if (add)
	{
		CzVec2 v = actor->getPositionRange();
		v.x += prop.p_vec[0];
		v.y += prop.p_vec[1];
		actor->setPositionRange(v);
	}
	else
	{
		CzVec2 v = CzVec2(prop.p_vec[0], prop.p_vec[1]);
		actor->setPositionRange(v);
	}
	return true;
}

bool CzActorParticles::_setAngleRange(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;
	if (add)
	{
		CzVec2 v = actor->getAngleRange();
		v.x += prop.p_vec[0];
		v.y += prop.p_vec[1];
		actor->setAngleRange(v);
	}
	else
	{
		CzVec2 v = CzVec2(prop.p_vec[0], prop.p_vec[1]);
		actor->setAngleRange(v);
	}
	return true;
}

bool CzActorParticles::_setScaleRange(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;
	if (add)
	{
		CzVec2 v = actor->getScaleRange();
		v.x += prop.p_vec[0];
		v.y += prop.p_vec[1];
		actor->setScaleRange(v);
	}
	else
	{
		CzVec2 v = CzVec2(prop.p_vec[0], prop.p_vec[1]);
		actor->setScaleRange(v);
	}
	return true;
}

bool CzActorParticles::_setDepthRange(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;
	if (add)
	{
		CzVec2 v = actor->getDepthRange();
		v.x += prop.p_vec[0];
		v.y += prop.p_vec[1];
		actor->setDepthRange(v);
	}
	else
	{
		CzVec2 v = CzVec2(prop.p_vec[0], prop.p_vec[1]);
		actor->setDepthRange(v);
	}
	return true;
}

bool CzActorParticles::_setVelRange(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;
	if (add)
	{
		CzVec4 v = actor->getVelocityRange();
		v.x += prop.p_vec[0];
		v.y += prop.p_vec[1];
		v.z += prop.p_vec[2];
		v.w += prop.p_vec[3];
		actor->setVelocityRange(v);
	}
	else
	{
		CzVec4 v = CzVec4(prop.p_vec[0], prop.p_vec[1], prop.p_vec[2], prop.p_vec[3]);
		actor->setVelocityRange(v);
	}
	return true;
}

bool CzActorParticles::_setAngVelRange(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;
	if (add)
	{
		CzVec2 v = actor->getAngVelocityRange();
		v.x += prop.p_vec[0];
		v.y += prop.p_vec[1];
		actor->setAngVelocityRange(v);
	}
	else
	{
		CzVec2 v = CzVec2(prop.p_vec[0], prop.p_vec[1]);
		actor->setAngVelocityRange(v);
	}
	return true;
}

bool CzActorParticles::_setScaleVelRange(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;
	if (add)
	{
		CzVec2 v = actor->getScaleVelocityRange();
		v.x += prop.p_vec[0];
		v.y += prop.p_vec[1];
		actor->setScaleVelocityRange(v);
	}
	else
	{
		CzVec2 v = CzVec2(prop.p_vec[0], prop.p_vec[1]);
		actor->setScaleVelocityRange(v);
	}
	return true;
}

bool CzActorParticles::_setDepthVelRange(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;
	if (add)
	{
		CzVec2 v = actor->getDepthVelocityRange();
		v.x += prop.p_vec[0];
		v.y += prop.p_vec[1];
		actor->setDepthVelocityRange(v);
	}
	else
	{
		CzVec2 v = CzVec2(prop.p_vec[0], prop.p_vec[1]);
		actor->setDepthVelocityRange(v);
	}
	return true;
}

bool CzActorParticles::_setVelocityAll(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;
	actor->setVelocityAll(prop.p_vec[0], prop.p_vec[1], add);
	return true;
}

bool CzActorParticles::_setAutoDelete(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActorParticles* actor = (CzActorParticles*)target;

	if (add)
		actor->setAutoDelete(!actor->isAutoDelete());
	else
		actor->setAutoDelete(prop.p_bool);
	
	return true;
}

CzXomlProperty CzActorParticles::_getAutoDelete(IzXomlResource* target)
{
	return CzXomlProperty(((CzActorParticles*)target)->isAutoDelete());
}

/**
 @fn	void CzActorParticles::RegenParticle(CzActorParticle* particle)

 @brief	Regenerates a particle.
 
 @param [out]	particle	If non-null, the particle.
 */

void CzActorParticles::RegenParticle(CzActorParticle* particle)
{
	if (PositionMode == PAM_Random)
	{
		particle->Position.x = CzMath::Rand(PositionRange.x * 2) - PositionRange.x;
		particle->Position.y = CzMath::Rand(PositionRange.y * 2) - PositionRange.y;
	}
	if (AngleMode == PAM_Random)
	{
		particle->Angle = CzMath::Rand(AngleRange.y - AngleRange.x) + AngleRange.x;
	}
	if (ScaleMode == PAM_Random)
	{
		particle->Scale.x = CzMath::Rand(ScaleRange.y - ScaleRange.x) + ScaleRange.x;
		particle->Scale.y = particle->Scale.x;
	}
	if (DepthMode == PAM_Random)
	{
		particle->Depth = CzMath::Rand(DepthRange.y - DepthRange.x) + DepthRange.x;
	}
	if (VelocityMode == PAM_Random)
	{
		particle->Velocity.x = CzMath::Rand(VelocityRange.y - VelocityRange.x) + VelocityRange.x;
		particle->Velocity.y = CzMath::Rand(VelocityRange.w - VelocityRange.z) + VelocityRange.z;
	}
	if (AngVelocityMode == PAM_Random)
	{
		particle->AngularVelocity = CzMath::Rand(AngVelocityRange.y - AngVelocityRange.x) + AngVelocityRange.x;
	}
	if (ScaleVelocityMode == PAM_Random)
	{
		particle->ScaleVelocity.x = CzMath::Rand(ScaleVelocityRange.y - ScaleVelocityRange.x) + ScaleVelocityRange.x;
		particle->ScaleVelocity.y = particle->ScaleVelocity.x;
	}
	if (DepthVelocityMode == PAM_Random)
	{
		particle->DepthVelocity = CzMath::Rand(DepthVelocityRange.y - DepthVelocityRange.x) + DepthVelocityRange.x;
	}
	if (!particle->Attached)
	{
		if (Visual != NULL)
			Visual->getTransform().Transform(&particle->Position, &particle->Position);
	}
}

/**
 @fn	bool CzActorParticles::addParticle(CzActorParticle* particle, const CzIRect& src_rect,
    int count, float spawn_delay)

 @brief	Adds particles to the particle system.

 @param [in,out]	particle	If non-null, the particles list.
 @param	src_rect				Bitmap source rectangle.
 @param	count					Number of particles.
 @param	spawn_delay				The spawn delay.

 @return	true if it succeeds, false if it fails.
 */

bool CzActorParticles::addParticle(CzActorParticle* particle, const CzIRect& src_rect, int count, float spawn_delay, CzSprite::eAnchor anchor)
{
	// Set particles parent
	particle->Parent = this;
	if (spawn_delay != 0)
		particle->SpawnDelay = spawn_delay;

	// Add particle to particles list
	if (!Particles->add(particle))
		return false;

	// Back up properties
	particle->CopyToOld();

	// Apply modes
	RegenParticle(particle);

	// Create a bitmap sprite
	CzBitmapSprite* sprite = new CzBitmapSprite();
	if (sprite == NULL)
		return false;
	sprite->Init();
	sprite->setOrphan(Orphan);
	sprite->setIgnoreCamera(IgnoreCamera);
	sprite->setAnchor(anchor);

	// Set particles visual
	particle->Visual = sprite;

	// Set sprite image
	sprite->setImage(Image);
	sprite->setSrcDest(src_rect.x, src_rect.y, src_rect.w, src_rect.h);

	// Add particles sprite to sprite manager
	Scene->getSpriteManager()->addSprite(sprite);

	// If count was specified then clone this particle count times
	if (count != 0)
	{
		float delay = 0;
		for (int t = 0; t < count - 1; t++)
		{
			addParticle(particle->Clone(), src_rect, 0, delay, anchor);
			delay += particle->SpawnDelay;
		}
	}

	return true;
}

/**
 @fn	CzActorParticle* CzActorParticles::getParticle(int index)

 @brief	Gets particle at index.

 @param	index	Zero-based index of the particle.

 @return	null if it fails, else the particle.
 */

CzActorParticle* CzActorParticles::getParticle(int index)
{
	if (Particles == NULL)
		return NULL;

	return Particles->element_at(index);
}

void CzActorParticles::removeAllParticles()
{
	Particles->clear(true);
}


/**
 @fn	bool CzActorParticles::Init(int max_particles)

 @brief	Initialises the particle system actor.

 @param	max_particles	The maximum particles the system can hold.

 @return	true if it succeeds, false if it fails.
 */

bool CzActorParticles::Init(int max_particles)
{
	CzActor::Init();

	// Create dummy sprite (we only need its attributes and do not need to render it)
	Visual = new CzDummySprite();
	Visual->Init();

	// Add dummy sprite to the sprite manager so it can be managed and drawn
	Scene->getSpriteManager()->addSprite(Visual);

	// Create space for particles
	Particles = new CzSlotArray<CzActorParticle*>(max_particles, 4);

	return true;
}

/**
 @fn	bool CzActorParticles::Update(float dt)

 @brief	Updates the particle system actor.

 @param	dt	The number of seconds since the app was last updated

 @return	true if it succeeds, false if it fails.
 */

bool CzActorParticles::Update(float dt)
{
	if (!CzActor::Update(dt))
		return false;

	// Update particles
	int num_active = 0;
	for (int t = 0; t < Particles->getSize(); t++)
	{
		CzActorParticle* particle = Particles->element_at(t);
		if (particle != NULL && particle->Active)
		{
			particle->Update(dt);
			num_active++;
		}
	}

	// If all particles are inactive then remove this actor
	if (num_active == 0)
	{
		if (AutoDelete)
			return false;
	}

	return true;
}

/**
 @fn	void CzActorParticles::setParticlesVisibility(bool visible)

 @brief	Sets particles visibility.

 @param	visible	true to show, false to hide.
 */

void CzActorParticles::setParticlesVisibility(bool visible)
{
	// Hide all particle visuals
	for (int t = 0; t < Particles->getSize(); t++)
	{
		CzActorParticle* particle = Particles->element_at(t);
		if (particle != NULL)
		{
			CzBitmapSprite* visual = particle->Visual;
			visual->setVisible(visible);
		}
	}
}

/**
 @fn	bool CzActorParticles::UpdateVisual()

 @brief	Updates the actors visuals.

 The particle system actor contains a collection of particles which each has its own visual. This method updates all particle visuals as well as the particle system actor itself.

 @return	true if it succeeds, false if it fails.
 */

bool CzActorParticles::UpdateVisual()
{
	if (!CzActor::UpdateVisual())
	{
		setParticlesVisibility(false);
		return false;
	}

	CzColour vc = Visual->getColour();
	// Update particle visuals
	for (int t = 0; t < Particles->getSize(); t++)
	{
		CzActorParticle* particle = Particles->element_at(t);
		if (particle != NULL)
		{
			CzBitmapSprite* visual = particle->Visual;

			if (particle->Visible)
			{
				if (LinkedTo != NULL)
					visual->setLinkedTo(LinkedTo->getVisual());
				float depth = Depth + particle->Depth;
//				if (depth < 0.1f)
//					depth = 0.1f;
				visual->setDepth(depth);

				// Transform particles position by actors transform
				CzVec2 pos = particle->Position;
				if (particle->Attached)
					Visual->getTransform().Transform(&pos, &pos);
			
				// Transfor actor properties to the visual
				visual->setPosAngScale(pos.x, pos.y, particle->Angle, particle->Scale.x, particle->Scale.y);

				CzColour c;
				c.r = (uint8)(((int)vc.r * particle->Colour.x) / 255);
				c.g = (uint8)(((int)vc.g * particle->Colour.y) / 255);
				c.b = (uint8)(((int)vc.b * particle->Colour.z) / 255);
				c.a = (uint8)(((int)vc.a * particle->Colour.w) / 255);
				visual->setColour(c, true);

				// Set visible layer
				visual->setLayer(Layer);
			}
			visual->setVisible(particle->Visible);
		}
	}

	return true;
}

/**
 @fn	void CzActorParticles::GenerateRandomParticles(int count, const CzIRect& src_rect, const CzVec4& colour, const CzVec4& colour_velocity, CzVec2& gravity, float duration, int repeat_count, float spawn_delay_change)

 @brief	Generates a number of random particles.

 @param	count			  	Number of particles to generate.
 @param	src_rect		  	Bitmap source rectangle.
 @param	colour			  	The colour.
 @param	colour_velocity   	The colour velocity.
 @param	gravity			  	The gravity to apply to the particle.
 @param	duration		  	Lifespan of particle ni seconds.
 @param	repeat_count	  	Number of times to repeat the particle respawn.
 @param	spawn_delay_change	The spawn delay change in seconds.
 */

void CzActorParticles::GenerateRandomParticles(int count, const CzIRect& src_rect, const CzVec4& colour, const CzVec4& colour_velocity, CzVec2& gravity, float duration, int repeat_count, float spawn_delay_change)
{
	float spawn_delay = 0;

	for (int t = 0; t < count; t++)
	{
		CzActorParticle* p = new CzActorParticle();
		p->LifeSpan = duration;
		p->Lives = repeat_count;
		p->SpawnDelay = spawn_delay;
		p->Gravity = gravity;
		p->Colour = colour;
		p->ColourVelocity = colour_velocity;

		spawn_delay += spawn_delay_change;

		addParticle(p, src_rect);
	}
}

/**
 @fn	void CzActorParticles::GenerateRandomParticles(int count, CzActorParticle* particle, const CzIRect& src_rect, float duration, int repeat_count, float spawn_delay_change)

  @brief	Generates a number of random particles based on an existing particle.

 @param	count					Number of particles.
 @param [in]	particle		If non-null, the particle to copy.
 @param	src_rect				Bitmap source rectangle.
 @param	duration		  		Lifespan of particle ni seconds.
 @param	repeat_count	  		Number of times to repeat the particle respawn.
 @param	spawn_delay_change		The spawn delay change in seconds.
 @param	spawn_delay_change  	The spawn delay change.
 */

void CzActorParticles::GenerateRandomParticles(int count, CzActorParticle* particle, const CzIRect& src_rect, float duration, int repeat_count, float spawn_delay_change)
{
	float spawn_delay = 0;

	for (int t = 0; t < count; t++)
	{
		CzActorParticle* p = particle->Clone();
		p->LifeSpan = duration;
		p->Lives = repeat_count;
		p->SpawnDelay = spawn_delay;

		spawn_delay += spawn_delay_change;

		addParticle(p, src_rect);
	}
}

/**
 @fn	void CzActorParticles::setVelocityAll(float x, float y, bool delta)

 @brief	Sets the velocity of all particles to the given velocity.

 @param	x	The x velcoity in scene coordinates.
 @param	y	The y velcoity  in scene coordinates.
 @param delta	If true then the velocity will be added to all particles
 */

void CzActorParticles::setVelocityAll(float x, float y, bool delta)
{
	for (int t = 0; t < Particles->getSize(); t++)
	{
		CzActorParticle* particle = Particles->element_at(t);
		if (particle != NULL)
		{
			if (delta)
			{
				particle->Velocity.x += x;
				particle->Velocity.y += y;
			}
			else
			{
				particle->Velocity.x = x;
				particle->Velocity.y = y;
			}
		}
	}
}

/**
 @fn	int CzActorParticles::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)

 @brief	Creates an instance of this class from XML

 LoadFromXoml is the main method used by all classes that can be instantiated from XOML mark-up. This method creates an instance of this class from the supplied XML node structure specified by node. 
 if parent is not NULL then the created instance will utilise the supplied parent to determine where it should live within the app. For example, if the parent is a scene then the actor will be placed 
 in that scene. if the parent is another actor then this actor will be placed into the scene of the supplied actor and linked as a child to the parent actor. If load_chlldren is true then any child 
 nodes contained within the actor will also be loaded. Note that as CzActorParticles is derived from CzActor, CzActor::LoadFromXoml() will also be called to load CzActor specific properties. 

 @param [in]	parent		If non-null, the parent.
 @param	load_children	  	true to load child nodes.
 @param [in]	node  		If non-null, the XOML markup node that defines this object

 @return	negative value for error. if 0 then processing of further XOML will stop.
 */

int CzActorParticles::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "ActorParticles - Actor needs to be declared inside a scene or an actor - ", DebugInfo.c_str());
		return 0;
	}

	// Load main actor attributes
	int ret = CzActor::LoadFromXoml(parent, false, node);
	if (ret <= 0)
		return ret;

	// Process ActorImage specific attributes
	CzString	image_name;
	CzIRect		src_rect(0, 0, 0, 0);
	CzString*	anchor = NULL;

	CzVec2 size;
	unsigned int pos_mode_hash = CzString::CalculateHash("posmode");
	unsigned int ang_mode_hash = CzString::CalculateHash("angmode");
	unsigned int scale_mode_hash = CzString::CalculateHash("scalemode");
	unsigned int depth_mode_hash = CzString::CalculateHash("depthmode");
	unsigned int vel_mode_hash = CzString::CalculateHash("velmode");
	unsigned int vang_mode_hash = CzString::CalculateHash("angvelmode");
	unsigned int vscale_mode_hash = CzString::CalculateHash("scalevelmode");
	unsigned int vdepth_mode_hash = CzString::CalculateHash("depthvelmode");
	unsigned int position_range_hash = CzString::CalculateHash("positionrange");
	unsigned int angle_range_hash = CzString::CalculateHash("anglerange");
	unsigned int scale_range_hash = CzString::CalculateHash("scalerange");
	unsigned int depth_range_hash = CzString::CalculateHash("depthrange");
	unsigned int vel_range_hash = CzString::CalculateHash("velrange");
	unsigned int angvel_range_hash = CzString::CalculateHash("angvelrange");
	unsigned int scalevel_range_hash = CzString::CalculateHash("scalevelrange");
	unsigned int depthvel_range_hash = CzString::CalculateHash("depthvelrange");
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Image_Hash)
		{
			image_name = (*it)->getValue();
		}
		else
		if (name_hash == CzHashes::Size_Hash)
		{
			if (!(*it)->getValueAsPoint(size))
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::Size, expected vec2 - ", DebugInfo.c_str());
			}
		}
		else
		if (name_hash == CzHashes::Anchor_Hash)
			anchor = &(*it)->getValue();
		else
		if (name_hash == pos_mode_hash)
		{
			if ((*it)->getValue() == "random")
				PositionMode = PAM_Random;
		}
		else
		if (name_hash == ang_mode_hash)
		{
			if ((*it)->getValue() == "random")
				AngleMode = PAM_Random;
		}
		else
		if (name_hash == scale_mode_hash)
		{
			if ((*it)->getValue() == "random")
				ScaleMode = PAM_Random;
		}
		else
		if (name_hash == depth_mode_hash)
		{
			if ((*it)->getValue() == "random")
				DepthMode = PAM_Random;
		}
		else
		if (name_hash == vel_mode_hash)
		{
			if ((*it)->getValue() == "random")
				VelocityMode = PAM_Random;
		}
		else
		if (name_hash == vang_mode_hash)
		{
			if ((*it)->getValue() == "random")
				AngVelocityMode = PAM_Random;
		}
		else
		if (name_hash == vscale_mode_hash)
		{
			if ((*it)->getValue() == "random")
				ScaleVelocityMode = PAM_Random;
		}
		else
		if (name_hash == vdepth_mode_hash)
		{
			if ((*it)->getValue() == "random")
				DepthVelocityMode = PAM_Random;
		}
		else
		if (name_hash == position_range_hash)
		{
			if (!(*it)->getValueAsPoint(PositionRange))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::PositionRange, expected vec2 - ", DebugInfo.c_str());
		}
		else
		if (name_hash == angle_range_hash)
		{
			if (!(*it)->getValueAsPoint(AngleRange))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::AngleRange, expected vec2 - ", DebugInfo.c_str());
		}
		else
		if (name_hash == scale_range_hash)
		{
			if (!(*it)->getValueAsPoint(ScaleRange))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::ScaleRange, expected vec2 - ", DebugInfo.c_str());
		}
		else
		if (name_hash == depth_range_hash)
		{
			if (!(*it)->getValueAsPoint(DepthRange))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::DepthRange, expected vec2 - ", DebugInfo.c_str());
		}
		else
		if (name_hash == vel_range_hash)
		{
			if (!(*it)->getValueAsPoint4(VelocityRange))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::VelocityRange, expected vec4 - ", DebugInfo.c_str());
		}
		else
		if (name_hash == angvel_range_hash)
		{
			if (!(*it)->getValueAsPoint(AngVelocityRange))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::AngVelocityRange, expected vec2 - ", DebugInfo.c_str());
		}
		else
		if (name_hash == scalevel_range_hash)
		{
			if (!(*it)->getValueAsPoint(ScaleVelocityRange))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::ScaleVelocityRange, expected vec2 - ", DebugInfo.c_str());
		}
		else
		if (name_hash == depthvel_range_hash)
		{
			if (!(*it)->getValueAsPoint(DepthVelocityRange))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::DepthVelocityRange, expected vec2 - ", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::AutoDelete_Hash)
			setAutoDelete((*it)->getValueAsBool());
	}

	// Search the scenes resources for the specified image
	CzImage* image = (CzImage*)Scene->getResourceManager()->findResource(image_name.c_str(), CzHashes::Image_Hash);
	if (image == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "ActorParticles - Could not find named image in the scene or in the global resources - ", image_name.c_str(), DebugInfo.c_str());
		return 0;
	}
	setImage(image);

	// Calculate number of particles
	int num_particles = node->Children.size();
/*	if (num_particles == 0)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Particle actor has no particles - ", DebugInfo.c_str());
		return 1;
	}*/

	// Initialise the actor
	Init(num_particles);
	CzSprite::eAnchor _anchor = CzSprite::Centre;
	if (anchor != NULL && anchor->getHash() != CzHashes::centre_Hash)
		_anchor = CzSprite::TopLeft;

	// Prrocess particles
	for (CzXmlNode::_Iterator it2 = node->begin(); it2 != node->end(); ++it2)
	{
		unsigned int name_hash = (*it2)->GetName().getHash();
		if (name_hash == CzHashes::Particle_Hash)
		{
			CzActorParticle* particle = new CzActorParticle();

			int count = 0;

			// Get particle data
			for (CzXmlNode::_AttribIterator it = (*it2)->attribs_begin(); it != (*it2)->attribs_end(); ++it)
			{
				unsigned int attrib_hash = (*it)->getName().getHash();

				if (attrib_hash == CzHashes::Count_Hash)
				{
					count = (*it)->getValueAsInt();
				}
				else
				if (attrib_hash == CzHashes::Position_Hash)
				{
					if (!(*it)->getValueAsPoint(particle->Position))
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::Position, expected vec2 - ", DebugInfo.c_str());
				}
				else
				if (attrib_hash == CzHashes::Velocity_Hash)
				{
					if (!(*it)->getValueAsPoint(particle->Velocity))
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::Velocity, expected vec2 - ", DebugInfo.c_str());
				}
				else
				if (attrib_hash == CzHashes::VelocityDamping_Hash)
				{
					if (!(*it)->getValueAsPoint(particle->VelocityDamping))
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::VelocityDamping, expected vec2 - ", DebugInfo.c_str());
				}
				else
				if (attrib_hash == CzHashes::Angle_Hash)
					particle->Angle = (*it)->getValueAsFloat();
				else
				if (attrib_hash == CzHashes::AngularVelocity_Hash)
					particle->AngularVelocity = (*it)->getValueAsFloat();
				else
				if (attrib_hash == CzHashes::AngularVelocityDamping_Hash)
				{
					particle->AngularVelocityDamping = (*it)->getValueAsFloat();
				}
				else
				if (attrib_hash == CzHashes::Depth_Hash)
					particle->Depth = (*it)->getValueAsFloat();
				else
				if (attrib_hash == CzHashes::DepthVelocity_Hash)
				{
					particle->DepthVelocity = (*it)->getValueAsFloat();
				}
				else
				if (attrib_hash == CzHashes::DepthVelocityDamping_Hash)
				{
					particle->DepthVelocityDamping = (*it)->getValueAsFloat();
				}
				else
				if (attrib_hash == CzHashes::Scale_Hash)
				{
					if (!(*it)->getValueAsPoint(particle->Scale))
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::Scale, expected vec2 - ", DebugInfo.c_str());
				}
				else
				if (attrib_hash == CzHashes::ScaleVelocity_Hash)
				{
					if (!(*it)->getValueAsPoint(particle->Scale))
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::ScaleVelocity, expected vec2 - ", DebugInfo.c_str());
				}
				else
				if (attrib_hash == CzHashes::ScaleVelocityDamping_Hash)
				{
					if (!(*it)->getValueAsPoint(particle->Scale))
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::ScaleVelocityDamping, expected vec2 - ", DebugInfo.c_str());
				}
				else
				if (attrib_hash == CzHashes::SrcRect_Hash)
				{
					if (!(*it)->getValueAsRect(src_rect))
					{
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticle::SrcRect, expected rect - ", DebugInfo.c_str());
					}
				}
				else
				if (attrib_hash == CzHashes::Colour_Hash || attrib_hash == CzHashes::Color_Hash)
				{
					if (!(*it)->getValueAsPoint4(particle->Colour))
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::Colour, expected vec4 - ", DebugInfo.c_str());
				}
				else
				if (attrib_hash == CzHashes::ColourVelocity_Hash)
				{
					if (!(*it)->getValueAsPoint4(particle->ColourVelocity))
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::ColourVelocity, expected vec4 - ", DebugInfo.c_str());
				}
				else
				if (attrib_hash == CzHashes::ColourVelocityDamping_Hash)
				{
					if (!(*it)->getValueAsPoint4(particle->ColourVelocityDamping))
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::ColourVelocityDamping, expected vec4 - ", DebugInfo.c_str());
				}
				else
				if (attrib_hash == CzHashes::Repeat_Hash)
					particle->Lives = (*it)->getValueAsInt();
				else
				if (attrib_hash == CzHashes::SpawnDelay_Hash)
					particle->SpawnDelay = (*it)->getValueAsFloat();
				else
				if (attrib_hash == CzHashes::Duration_Hash)
					particle->LifeSpan = (*it)->getValueAsFloat();
				else
				if (attrib_hash == CzHashes::Active_Hash)
					particle->Active = (*it)->getValueAsBool();
				else
				if (attrib_hash == CzHashes::Visible_Hash)
					particle->Visible = (*it)->getValueAsBool();
				else
				if (attrib_hash == CzHashes::Gravity_Hash)
				{
					if (!(*it)->getValueAsPoint(particle->Gravity))
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - Invalid value for ActorParticles::Gravity, expected vec2 - ", DebugInfo.c_str());
				}
				else
				if (attrib_hash == CzHashes::Attached_Hash)
					particle->Attached = (*it)->getValueAsBool();
			}
			addParticle(particle, src_rect, count, 0, _anchor);
		}
	}

	if (anchor != NULL)
		Visual->setAnchor(_anchor);

	OriginalSize.x = (int)size.x;
	OriginalSize.y = (int)size.y;
	if (size.x == 0)
		size.x = -100;
	if (size.y == 0)
		size.y = -100;
	int width = (int)size.x, height = (int)size.y;
	CalculateSizes(width, height);
	Size.x = width;
	Size.y = height;

	// Set dockign mode
	if (Docking != CzScene::Dock_None)
		setDocking(Docking);

	// Process inner tags
	if (load_children)
	{
		if (!CZ_XOML->Process(this, node))
			return 0;
	}

	return 1;
}

/**
 @fn	bool CzActorParticles::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the actor.

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types. Note that as 
 CzActorParticles is derived from CzActor, all CzActor properties will also be checked against.

 @param	property_name		Name of the property.
 @param [out]	prop	The property data that.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActorParticles::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	if (CzActor::getProperty(property_name, prop))
		return true;

	int err = IzXomlResource::getProperty(ActorParticlesClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorParticlesClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzActorParticles::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the ActorParticles.
    
 Sets the named property of this ActorParticles. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzActorParticles::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	if (CzActor::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(ActorParticlesClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorParticlesClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzActorParticles::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced. Note that as CzActorParticles is derived from CzActor, all CzActor properties will also be checked against.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in]	data	The property data.
 @param	delta				true to property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActorParticles::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	if (CzActor::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(ActorParticlesClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorParticlesClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzActorParticles::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates actor property from supplied animation.

 Updates the actors property that is specified by the supplied animation. When a timeline animation targets the property of an actor this method will be called by the animation to move the 
 animations frame data into its target property. Note that as CzActorParticles is derived from CzActor, all CzActor properties will also be checked against.

 @param [in]	animation	If non-null, the animation.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActorParticles::UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzActor::UpdateFromAnimation(animation))
		return true;

	int err = IzXomlResource::setProperty(ActorParticlesClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorParticlesClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;

}

/**
 @fn	bool CzActorParticles::UpdateBinding(unsigned int property_name, CzXomlVariable* var)

 @brief	Updates an actor property from the supplied XOML variable.

 Updates a single property from the value of the bound XOML variable. Note that as CzActorParticles is derived from CzActor, all CzActor properties will also be checked against.

 @param	property_name  	Name of the property.
 @param [in,out]	var	If non-null, the variable.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActorParticles::UpdateBinding(unsigned int property_name, CzXomlVariable* var)
{
	if (!var->Modified || var->getValue().isEmpty())
		return false;

	if (CzActor::UpdateBinding(property_name, var))
		return true;

	int err = IzXomlResource::setProperty(ActorParticlesClassDef, this, property_name, var);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorParticlesClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ActorParticles - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;

}