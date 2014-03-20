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

#include <math.h>

#include "IzPlatformDisplay.h"
#include "CzActor.h"
#include "CzCamera.h"
#include "CzXoml.h"
#include "CzActions.h"
#include "CzApp.h"
#include "CzBrush.h"
#include "CzUIBase.h"
#include "CzHashes.h"
#include "CzMath.h"
#include "CzColour.h"
#include "CzXomlResourceManager.h"

CzXomlClassDef* CzActor::ActorClassDef = NULL;

/**
 @fn	void CzActor::InitClass()

 @brief	Initialises the class.
	
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */
void CzActor::InitClass()
{
	// Create class properties
	ActorClassDef = new CzXomlClassDef();
	ActorClassDef->addProperty(new CzXomlClassProperty("Position",				PT_Vec2,		CzActor::_setPosition,					CzActor::_getPosition));
	ActorClassDef->addProperty(new CzXomlClassProperty("X",						PT_Float,		CzActor::_setPositionX,					CzActor::_getPositionX));
	ActorClassDef->addProperty(new CzXomlClassProperty("Y",						PT_Float,		CzActor::_setPositionY,					CzActor::_getPositionY));
	ActorClassDef->addProperty(new CzXomlClassProperty("UserProperties",		PT_String,		NULL,									CzActor::_getUserProperties));
	ActorClassDef->addProperty(new CzXomlClassProperty("Velocity",				PT_Vec2,		CzActor::_setVelocity,					CzActor::_getVelocity));
	ActorClassDef->addProperty(new CzXomlClassProperty("Angle",					PT_Float,		CzActor::_setAngle,						CzActor::_getAngle));
	ActorClassDef->addProperty(new CzXomlClassProperty("AngularVelocity",		PT_Float,		CzActor::_setAngularVelocity,			CzActor::_getAngularVelocity));
	ActorClassDef->addProperty(new CzXomlClassProperty("Scale",					PT_Vec2,		CzActor::_setScale,						CzActor::_getScale));
	ActorClassDef->addProperty(new CzXomlClassProperty("ScaleX",				PT_Float,		CzActor::_setScaleX,					CzActor::_getScaleX));
	ActorClassDef->addProperty(new CzXomlClassProperty("ScaleY",				PT_Float,		CzActor::_setScaleY,					CzActor::_getScaleY));
	ActorClassDef->addProperty(new CzXomlClassProperty("Colour",				PT_Vec4,		CzActor::_setColour,					CzActor::_getColour));
	ActorClassDef->addProperty(new CzXomlClassProperty("Opacity",				PT_Float,		CzActor::_setOpacity,					CzActor::_getOpacity));
	ActorClassDef->addProperty(new CzXomlClassProperty("PositionOrg",			PT_Vec2,		CzActor::_setPositionOrg,				NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("PositionOrgX",			PT_Float,		CzActor::_setPositionOrgX,				NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("PositionOrgY",			PT_Float,		CzActor::_setPositionOrgY,				NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnCollisionStart",		PT_String,		CzActor::_setOnCollisionStart,			NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnCollisionEnd",		PT_String,		CzActor::_setOnCollisionEnd,			NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnTick",				PT_String,		CzActor::_setOnTick,					NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnTapped",				PT_String,		CzActor::_setOnTapped,					NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnBeginTouch",			PT_String,		CzActor::_setOnBeginTouch,				NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnEndTouch",			PT_String,		CzActor::_setOnEndTouch,				NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("Docking",				PT_String,		CzActor::_setDocking,					CzActor::_getDocking));
	ActorClassDef->addProperty(new CzXomlClassProperty("Margin",				PT_Vec4,		CzActor::_setMargin,					CzActor::_getMargin));
	ActorClassDef->addProperty(new CzXomlClassProperty("Depth",					PT_Float,		CzActor::_setDepth,						CzActor::_getDepth));
	ActorClassDef->addProperty(new CzXomlClassProperty("Origin",				PT_Vec2,		CzActor::_setOrigin,					CzActor::_getOrigin));
	ActorClassDef->addProperty(new CzXomlClassProperty("Layer",					PT_Float,		CzActor::_setLayer,						CzActor::_getLayer));
	ActorClassDef->addProperty(new CzXomlClassProperty("Active",				PT_Bool,		CzActor::_setActive,					CzActor::_getActive));
	ActorClassDef->addProperty(new CzXomlClassProperty("Visible",				PT_Bool,		CzActor::_setVisible,					CzActor::_getVisible));
	ActorClassDef->addProperty(new CzXomlClassProperty("UserData",				PT_Int,			CzActor::_setUserData,					CzActor::_getUserData));
	ActorClassDef->addProperty(new CzXomlClassProperty("Scene",					PT_String,		NULL,									CzActor::_getScene));
	ActorClassDef->addProperty(new CzXomlClassProperty("Collidable",			PT_Bool,		CzActor::_setCollidable,				CzActor::_getCollidable));
	ActorClassDef->addProperty(new CzXomlClassProperty("Tappable",				PT_Bool,		CzActor::_setTappable,					CzActor::_getTappable));
	ActorClassDef->addProperty(new CzXomlClassProperty("Draggable",				PT_Bool,		CzActor::_setDraggable,					CzActor::_getDraggable));
	ActorClassDef->addProperty(new CzXomlClassProperty("WrapPosition",			PT_Bool,		CzActor::_setWrapPosition,				CzActor::_getWrapPosition));
	ActorClassDef->addProperty(new CzXomlClassProperty("ScreenDocking",			PT_Bool,		CzActor::_setScreenDocking,				CzActor::_getScreenDocking));
	ActorClassDef->addProperty(new CzXomlClassProperty("Timeline",				PT_Resource,	CzActor::_setTimeline,					CzActor::_getTimeline));
	ActorClassDef->addProperty(new CzXomlClassProperty("TimeScale",				PT_Float,		CzActor::_setTimeScale,					CzActor::_getTimeScale));
	ActorClassDef->addProperty(new CzXomlClassProperty("Bindings",				PT_Resource,	CzActor::_setBindings,					CzActor::_getBindings));
	ActorClassDef->addProperty(new CzXomlClassProperty("Binding",				PT_String,		CzActor::_setBinding,					CzActor::_getBinding));
	ActorClassDef->addProperty(new CzXomlClassProperty("LinkedTo",				PT_Resource,	CzActor::_setLinkedTo,					CzActor::_getLinkedTo));
	ActorClassDef->addProperty(new CzXomlClassProperty("Target",				PT_Resource,	CzActor::_setTarget,					CzActor::_getTarget));
	ActorClassDef->addProperty(new CzXomlClassProperty("GridPos",				PT_Vec2,		CzActor::_setGridPos,					CzActor::_getGridPos));
	ActorClassDef->addProperty(new CzXomlClassProperty("LinearImpulse",			PT_Vec4,		CzActor::_setLinearImpulse,				NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("AngularImpulse",		PT_Float,		CzActor::_setAngularImpulse,			NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("Force",					PT_Vec4,		CzActor::_setForce,						NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("Torque",				PT_Float,		CzActor::_setTorque,					NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("VelocityDamping",		PT_Vec2,		CzActor::_setVelocityDamping,			CzActor::_getVelocityDamping));
	ActorClassDef->addProperty(new CzXomlClassProperty("AngularVelocityDamping",PT_Float,		CzActor::_setAngularVelocityDamping,	CzActor::_getAngularVelocityDamping));
	ActorClassDef->addProperty(new CzXomlClassProperty("Box2dMaterial",			PT_Resource,	CzActor::_setBox2dMaterial,				CzActor::_getBox2dMaterial));
	ActorClassDef->addProperty(new CzXomlClassProperty("Shape",					PT_Resource,	CzActor::_setShape,						NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("Sensor",				PT_Bool,		CzActor::_setSensor,					CzActor::_getSensor));
	ActorClassDef->addProperty(new CzXomlClassProperty("CollisionFlags",		PT_Vec3,		CzActor::_setCollisionFlags,			CzActor::_getCollisionFlags));
	ActorClassDef->addProperty(new CzXomlClassProperty("Bubbling",				PT_Bool,		CzActor::_setBubbling,					CzActor::_getBubbling));		// ReceiveEventFromChild
	ActorClassDef->addProperty(new CzXomlClassProperty("Orphan",				PT_Bool,		CzActor::_setOrphan,					CzActor::_getOrphan));
	ActorClassDef->addProperty(new CzXomlClassProperty("HoldFocus",				PT_Bool,		CzActor::_setHoldFocus,					CzActor::_getHoldFocus));
	ActorClassDef->addProperty(new CzXomlClassProperty("UseParentOpacity",		PT_Bool,		CzActor::_setUseParentOpacity,			CzActor::_getUseParentOpacity));
	ActorClassDef->addProperty(new CzXomlClassProperty("PercPos",				PT_Bool,		CzActor::_setPercPos,					CzActor::_getPercPos));			// PercentagePosition
	ActorClassDef->addProperty(new CzXomlClassProperty("IgnoreCamera",			PT_Bool,		CzActor::_setIgnoreCamera,				CzActor::_getIgnoreCamera));
	ActorClassDef->addProperty(new CzXomlClassProperty("Destroyed",				PT_Bool,		NULL,									CzActor::_getDestroyed));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnTapped2",				PT_String,		CzActor::_setOnTapped2,					NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnTapped3",				PT_String,		CzActor::_setOnTapped3,					NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnTapped4",				PT_String,		CzActor::_setOnTapped4,					NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnTapped5",				PT_String,		CzActor::_setOnTapped5,					NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnBeginTouch2",			PT_String,		CzActor::_setOnBeginTouch2,				NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnBeginTouch3",			PT_String,		CzActor::_setOnBeginTouch3,				NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnBeginTouch4",			PT_String,		CzActor::_setOnBeginTouch4,				NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnBeginTouch5",			PT_String,		CzActor::_setOnBeginTouch5,				NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnEndTouch2",			PT_String,		CzActor::_setOnEndTouch2,				NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnEndTouch3",			PT_String,		CzActor::_setOnEndTouch3,				NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnEndTouch4",			PT_String,		CzActor::_setOnEndTouch4,				NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnEndTouch5",			PT_String,		CzActor::_setOnEndTouch5,				NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnCreate",				PT_String,		CzActor::_setOnCreate,					NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnDestroy",				PT_String,		CzActor::_setOnDestroy,					NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("OnOrientationChange",	PT_String,		CzActor::_setOnOrientationChange,		NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("BodyAwake",				PT_Bool,		CzActor::_setBodyAwake,					CzActor::_getBodyAwake));
	ActorClassDef->addProperty(new CzXomlClassProperty("Name",					PT_String,		CzActor::_setName,						CzActor::_getName));
	ActorClassDef->addProperty(new CzXomlClassProperty("Type",					PT_Int,			CzActor::_setType,						CzActor::_getType));
	ActorClassDef->addProperty(new CzXomlClassProperty("Tag",					PT_String,		CzActor::_setTag,						NULL));
	ActorClassDef->addProperty(new CzXomlClassProperty("PositionX",				PT_Float,		CzActor::_setPositionX,					CzActor::_getPositionX));
	ActorClassDef->addProperty(new CzXomlClassProperty("PositionY",				PT_Float,		CzActor::_setPositionY,					CzActor::_getPositionY));
}
void CzActor::ReleaseClass()
{
	if (ActorClassDef != NULL)
	{
		ActorClassDef->Release();
		delete ActorClassDef;
		ActorClassDef = NULL;
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

bool CzActor::_setName(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	if (add)
	{
		CzString name = ((CzActor*)target)->getName();
		name += (const char*)prop.p_data;
		((CzActor*)target)->setName(name.c_str());
	}
	else
		((CzActor*)target)->setName((const char*)prop.p_data);

	return true;
}

CzXomlProperty CzActor::_getName(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getName().c_str());
}

bool CzActor::_setType(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	if (add)
		((CzActor*)target)->setType(((CzActor*)target)->getType() + prop.p_int);
	else
		((CzActor*)target)->setType(prop.p_int);

	return true;
}

CzXomlProperty CzActor::_getType(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getType());
}

bool CzActor::_setTag(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	((CzActor*)target)->setTag((const char*)prop.p_data);

	return true;
}

/*CzXomlProperty CzActor::_getTag(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getTag().c_str());
}*/

bool CzActor::_setUserData(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->setUserData(prop.p_data);

	return true;
}

CzXomlProperty CzActor::_getUserData(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getUserData());
}

CzXomlProperty CzActor::_getUserProperties(IzXomlResource* target)
{
	CzActor* actor = (CzActor*)target;
	CzUserPropertyList* props = actor->getUserPropertyList();
	return CzXomlProperty(props);
}

bool CzActor::_setPosition(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	if (add)
	{
		CzVec2 op = actor->getPosition();
		if (actor->getPercPos())
		{
			CzVec2 pos = actor->PercPosToPos(prop.p_vec[0], prop.p_vec[1]);
			actor->setPosition(op.x + pos.x, op.y + pos.y);
		}
		else
			actor->setPosition(op.x + prop.p_vec[0], op.y + prop.p_vec[1]);
	}
	else
	{
		if (actor->getPercPos())
		{
			CzVec2 pos = actor->PercPosToPos(prop.p_vec[0], prop.p_vec[1]);
			actor->setPosition(pos);
		}
		else
			actor->setPosition(prop.p_vec[0], prop.p_vec[1]);
	}

	return true;
}

CzXomlProperty CzActor::_getPosition(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getPosition());
}

bool CzActor::_setPositionX(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	if (add)
	{
		CzVec2 op = actor->getPosition();
		if (actor->getPercPos())
			actor->setPositionX(actor->PercPosToPos(op.x + prop.p_float, 0).x);
		else
			actor->setPositionX(op.x + prop.p_float);
	}
	else
	{
		if (actor->getPercPos())
			actor->setPositionX(actor->PercPosToPos(prop.p_float, 0).x);
		else
			actor->setPositionX(prop.p_float);
	}

	return true;
}

CzXomlProperty CzActor::_getPositionX(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getPosition().x);
}

bool CzActor::_setPositionY(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	if (add)
	{
		CzVec2 op = actor->getPosition();
		if (actor->getPercPos())
			actor->setPositionY(actor->PercPosToPos(0, op.y + prop.p_float).y);
		else
			actor->setPositionY(op.y + prop.p_float);
	}
	else
	{
		if (actor->getPercPos())
			actor->setPositionY(actor->PercPosToPos(0, prop.p_float).y);
		else
			actor->setPositionY(prop.p_float);
	}

	return true;
}

CzXomlProperty CzActor::_getPositionY(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getPosition().y);
}

bool CzActor::_setPositionOrg(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	if (add)
	{
		CzVec2 op = actor->getOriginalPosition();
		if (actor->getPercPos())
		{
			CzVec2 pos = actor->PercPosToPos(op.x + prop.p_vec[0], op.y + prop.p_vec[1]);
			actor->setPosition(pos);
		}
		else
			actor->setPosition(op.x + prop.p_vec[0], op.y + prop.p_vec[1]);
	}

	return true;
}

bool CzActor::_setPositionOrgX(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	if (add)
	{
		CzVec2 op = actor->getOriginalPosition();
		if (actor->getPercPos())
			actor->setPositionX(actor->PercPosToPos(op.x + prop.p_float, 0).x);
		else
			actor->setPositionX(op.x + prop.p_float);
	}

	return true;
}

bool CzActor::_setPositionOrgY(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	if (add)
	{
		CzVec2 op = actor->getOriginalPosition();
		if (actor->getPercPos())
			actor->setPositionY(actor->PercPosToPos(0, op.y + prop.p_float).y);
		else
			actor->setPositionY(op.y + prop.p_float);
	}

	return true;
}

bool CzActor::_setDocking(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	unsigned int hash = CzString::CalculateHash((const char*)prop.p_data);

	if (hash == CzString::CalculateHash("top"))
		actor->setDocking(CzScene::Dock_Top);
	else
	if (hash == CzString::CalculateHash("left"))
		actor->setDocking(CzScene::Dock_Left);
	else
	if (hash == CzString::CalculateHash("right"))
		actor->setDocking(CzScene::Dock_Right);
	else
	if (hash == CzString::CalculateHash("bottom"))
		actor->setDocking(CzScene::Dock_Bottom);
	else
	if (hash == CzString::CalculateHash("topleft"))
		actor->setDocking(CzScene::Dock_TopLeft);
	else
	if (hash == CzString::CalculateHash("topright"))
		actor->setDocking(CzScene::Dock_TopRight);
	else
	if (hash == CzString::CalculateHash("bottomleft"))
		actor->setDocking(CzScene::Dock_BottomLeft);
	else
	if (hash == CzString::CalculateHash("bottomright"))
		actor->setDocking(CzScene::Dock_BottomRight);
	else
	if (hash == CzString::CalculateHash("none"))
		actor->setDocking(CzScene::Dock_None);
	else
		return false;

	return true;
}

CzXomlProperty CzActor::_getDocking(IzXomlResource* target)
{
	return CzXoml::DockingNames[(int)((CzActor*)target)->getDocking()];
}

bool CzActor::_setMargin(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;

	if (add)
	{
		CzIRect m = actor->getMargin();
		m.x += (int)prop.p_vec[0];
		m.y += (int)prop.p_vec[1];
		m.w += (int)prop.p_vec[2];
		m.h += (int)prop.p_vec[3];
		actor->setMargin(m.x, m.y, m.w, m.h);
	}
	else
		actor->setMargin((int)prop.p_vec[0], (int)prop.p_vec[1], (int)prop.p_vec[2], (int)prop.p_vec[3]);

	CzIRect m = actor->getMargin();
	actor->setOriginalMargin(m.x, m.y, m.w, m.h);
	actor->CalculateMargins(m);

	return true;
}

CzXomlProperty CzActor::_getMargin(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getMargin());
}

bool CzActor::_setDepth(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	if (add)
		actor->setDepth(actor->getDepth() + prop.p_float);
	else
		actor->setDepth(prop.p_float);

	return true;
}

CzXomlProperty CzActor::_getDepth(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getDepth());
}

bool CzActor::_setOrigin(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	if (add)
	{
		CzVec2 org = actor->getOrigin();
		actor->setOrigin(org.x + prop.p_vec[0], org.x + prop.p_vec[1]);
	}
	else
		actor->setOrigin(prop.p_vec[0], prop.p_vec[1]);

	return true;
}

CzXomlProperty CzActor::_getOrigin(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getOrigin());
}

bool CzActor::_setVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	if (add)
	{
		CzVec2 vel = actor->getVelocity();
		actor->setVelocity(vel.x + prop.p_vec[0], vel.x + prop.p_vec[1]);
	}
	else
		actor->setVelocity(prop.p_vec[0], prop.p_vec[1]);
	
	return true;
}

CzXomlProperty CzActor::_getVelocity(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getVelocity());
}

bool CzActor::_setVelocityDamping(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	if (add)
	{
		CzVec2 vd = actor->getVelocityDamping();
		actor->setVelocityDamping(vd.x + prop.p_vec[0], vd.y + prop.p_vec[1]); 
	}
	else
		actor->setVelocityDamping(prop.p_vec[0], prop.p_vec[1]); 

	return true;
}

CzXomlProperty CzActor::_getVelocityDamping(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getVelocityDamping());
}

bool CzActor::_setAngle(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	if (add)
		actor->setAngle(-actor->getAngle() + prop.p_float);
	else
		actor->setAngle(prop.p_float);

	return true;
}

CzXomlProperty CzActor::_getAngle(IzXomlResource* target)
{
	return CzXomlProperty(-((CzActor*)target)->getAngle());
}

bool CzActor::_setAngularVelocity(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	if (add)
		actor->setAngularVelocity(actor->getAngularVelocity() + prop.p_float);
	else
		actor->setAngularVelocity(prop.p_float);

	return true;
}

CzXomlProperty CzActor::_getAngularVelocity(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getAngularVelocity());
}

bool CzActor::_setAngularVelocityDamping(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	if (add)
		actor->setAngularVelocityDamping(actor->getAngularVelocityDamping() + prop.p_float);
	else
		actor->setAngularVelocityDamping(prop.p_float);

	return true;
}

CzXomlProperty CzActor::_getAngularVelocityDamping(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getAngularVelocityDamping());
}

bool CzActor::_setScale(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	if (add)
	{
		CzVec2 scale = actor->getScale();
		actor->setScale(scale.x + prop.p_vec[0], scale.y + prop.p_vec[1]);
	}
	else
		actor->setScale(prop.p_vec[0], prop.p_vec[1]);

	return true;
}

CzXomlProperty CzActor::_getScale(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getScale());
}

bool CzActor::_setScaleX(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	CzVec2 scale = actor->getScale();
	if (add)
		actor->setScale(scale.x + prop.p_float, scale.y);
	else
		actor->setScale(prop.p_float, scale.y);

	return true;
}

CzXomlProperty CzActor::_getScaleX(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getScale().x);
}

bool CzActor::_setScaleY(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	CzVec2 scale = actor->getScale();
	if (add)
		actor->setScale(scale.x, scale.y + prop.p_float);
	else
		actor->setScale(scale.x, prop.p_float);

	return true;
}

CzXomlProperty CzActor::_getScaleY(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getScale().y);
}

bool CzActor::_setColour(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	if (add)
	{
		CzColour c = actor->getColour();
		actor->setColour(c.r + (uint8)prop.p_vec[0], c.g + (uint8)prop.p_vec[1], c.b + (uint8)prop.p_vec[2], c.a + (uint8)prop.p_vec[3]);
	}
	else
		actor->setColour((uint8)prop.p_vec[0], (uint8)prop.p_vec[1], (uint8)prop.p_vec[2], (uint8)prop.p_vec[3]);

	return true;
}

CzXomlProperty CzActor::_getColour(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getColour());
}

bool CzActor::_setOpacity(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;

	if (add)
		actor->setOpacity(actor->getOpacity() + (uint8)prop.p_float);
	else
		actor->setOpacity((uint8)prop.p_float);

	return true;
}

CzXomlProperty CzActor::_getOpacity(IzXomlResource* target)
{
	return CzXomlProperty((float)((CzActor*)target)->getOpacity());
}

bool CzActor::_setLayer(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;

	if (add)
		actor->setLayer(actor->getLayer() + (int)prop.p_float);
	else
		actor->setLayer((int)prop.p_float);

	return true;
}

CzXomlProperty CzActor::_getLayer(IzXomlResource* target)
{
	return CzXomlProperty((float)((CzActor*)target)->getLayer());
}

bool CzActor::_setOrphan(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;

	if (add)
		actor->setOrphan(!actor->getOrphan());
	else
		actor->setOrphan(prop.p_bool);

	return true;
}

CzXomlProperty CzActor::_getOrphan(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getOrphan());
}

bool CzActor::_setActive(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;

	if (add)
		actor->setActive(!actor->isActive());
	else
		actor->setActive(prop.p_bool);

	return true;
}

CzXomlProperty CzActor::_getActive(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->isActive());
}

bool CzActor::_setVisible(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;

	if (add)
		actor->setVisible(!actor->isVisible());
	else
		actor->setVisible(prop.p_bool);

	return true;
}

CzXomlProperty CzActor::_getVisible(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->isVisible());
}

CzXomlProperty CzActor::_getScene(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getScene());
}

bool CzActor::_setCollidable(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;

	if (add)
		actor->setCollidable(!actor->isCollidable());
	else
		actor->setCollidable(prop.p_bool);

	return true;
}

CzXomlProperty CzActor::_getCollidable(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->isCollidable());
}

bool CzActor::_setTappable(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;

	if (add)
		actor->setTappable(!actor->isTappable());
	else
		actor->setTappable(prop.p_bool);

	return true;
}

CzXomlProperty CzActor::_getTappable(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->isTappable());
}

bool CzActor::_setDraggable(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;

	if (add)
		actor->setDraggable(!actor->isDraggable());
	else
		actor->setDraggable(prop.p_bool);

	return true;
}

CzXomlProperty CzActor::_getDraggable(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->isDraggable());
}

bool CzActor::_setBubbling(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;

	if (add)
		actor->setReceiveEventFromChild(!actor->canReceiveEventFromChild());
	else
		actor->setReceiveEventFromChild(prop.p_bool);

	return true;
}

CzXomlProperty CzActor::_getBubbling(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->canReceiveEventFromChild());
}

CzXomlProperty CzActor::_getDestroyed(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->isDestroyed());
}

bool CzActor::_setWrapPosition(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;

	if (add)
		actor->setWrapPosition(!actor->getWrapPosition());
	else
		actor->setWrapPosition(prop.p_bool);

	return true;
}

CzXomlProperty CzActor::_getWrapPosition(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getWrapPosition());
}

bool CzActor::_setScreenDocking(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;

	if (add)
		actor->setScreenDocking(!actor->getScreenDocking());
	else
		actor->setScreenDocking(prop.p_bool);

	return true;
}

CzXomlProperty CzActor::_getScreenDocking(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getScreenDocking());
}

bool CzActor::_setHoldFocus(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;

	if (add)
		actor->setHoldFocus(!actor->getHoldFocus());
	else
		actor->setHoldFocus(prop.p_bool);

	return true;
}

CzXomlProperty CzActor::_getHoldFocus(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getHoldFocus());
}

bool CzActor::_setUseParentOpacity(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;

	if (add)
		actor->setUseParentOpacity(!actor->getUseParentOpacity());
	else
		actor->setUseParentOpacity(prop.p_bool);

	return true;
}

CzXomlProperty CzActor::_getUseParentOpacity(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getUseParentOpacity());
}

bool CzActor::_setPercPos(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;

	if (add)
		actor->setPercPos(!actor->getPercPos());
	else
		actor->setPercPos(prop.p_bool);

	return true;
}

CzXomlProperty CzActor::_getPercPos(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getPercPos());
}

bool CzActor::_setIgnoreCamera(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;

	if (add)
		actor->setIgnoreCamera(!actor->getIgnoreCamera());
	else
		actor->setIgnoreCamera(prop.p_bool);

	return true;
}

CzXomlProperty CzActor::_getIgnoreCamera(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getIgnoreCamera());
}

bool CzActor::_setTimeline(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	if (prop.p_data == NULL)
	{
		actor->setTimeline(NULL);
		return true;
	}

	CzAnimTimeline* timeline;
	if (prop.Type == PT_Resource)
		timeline = (CzAnimTimeline*)prop.p_data;
	else
		timeline = CzAnimTimelinesManager::FindTimeline((const char*)prop.p_data, actor);

	if (timeline != NULL)
	{
		if (timeline != actor->getTimeline())
		{
			actor->setTimeline(timeline);
			timeline->setTargetElement(actor);
		}
	}
#if defined(_DEBUG)
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - Could not find named timeline in the actor, scene or in the global resources - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());
		return false;
	}
#endif

	return true;
}

CzXomlProperty CzActor::_getTimeline(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getTimeline());
}

bool CzActor::_setTimeScale(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	CzAnimTimeline* timeline = actor->getTimeline();
	if (timeline == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - No timeline attached to set TimeScale - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());
		return false;
#endif
	}
	if (add)
		timeline->setTimeScale(timeline->getTimeScale() + prop.p_float);
	else
		timeline->setTimeScale(prop.p_float);

	return true;
}

CzXomlProperty CzActor::_getTimeScale(IzXomlResource* target)
{
	CzActor* actor = (CzActor*)target;
	CzAnimTimeline* timeline = actor->getTimeline();
	if (timeline == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - getProperty - No timeline attached to get TimeScale - ", actor->getDebugInfo().c_str());
		return false;
#endif
	}

	return CzXomlProperty(timeline->getTimeScale());
}

bool CzActor::_setBindings(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;

	if (prop.p_data == NULL)
	{
		actor->setBindings(NULL);
		return true;
	}
	CzXomlBindings* bindings; 
	if (prop.Type == PT_Resource)
		bindings = (CzXomlBindings*)prop.p_data;
	else
		bindings = (CzXomlBindings*)actor->getScene()->getResourceManager()->findResource((const char*)prop.p_data, CzHashes::Bindings_Hash);

	if (bindings == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - Could not find named bindings in the scene or in the global resources - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());
		return false;
	}
	else
		actor->setBindings(bindings);

	return true;
}

CzXomlProperty CzActor::_getBindings(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getBindings());
}

bool CzActor::_setBinding(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	CzString var_name, prop_name;
	CzString index_name;
	int index = -1;
	CzString data = (const char*)prop.p_data;
	if (((CzString&)data).SplitPropVarIndex(prop_name, var_name, index, index_name))
	{
		CzXomlVariable* var = CzXomlVariable::GetVariable(var_name, actor->getScene());
		if (var == NULL)
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - Bound simple binding variable does not exist - ", var_name.c_str(), actor->getDebugInfo().c_str());
			return false;
		}
		else
		{
			CzXomlBinding* sb = new CzXomlBinding();
			sb->Index = index;
#if defined(_DEBUG)
			sb->_Property = prop_name;
#endif // _DEBUG
			sb->Property = prop_name.getHash();
			sb->Variable = var;
			if (!index_name.isEmpty())
				sb->VIndex = index_name;
			actor->setSimpleBinding(sb);
		}
	}
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - Invalid format of simple binding, format should be [property]variable:array_index - ", data.c_str(), actor->getDebugInfo().c_str());
		return false;
	}

	return true;
}

CzXomlProperty CzActor::_getBinding(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getSimpleBinding());
}

bool CzActor::_setLinkedTo(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	CzActor* parent;
	if (prop.Type == PT_Resource)
		parent = (CzActor*)prop.p_data;
	else
		parent = actor->getScene()->findActor((const char*)prop.p_data);
	actor->setLinkedTo(parent);

	return true;
}

CzXomlProperty CzActor::_getLinkedTo(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getLinkedTo());
}

bool CzActor::_setTarget(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	CzActor* ta;
	if (prop.Type == PT_Resource)
		ta = (CzActor*)prop.p_data;
	else
		ta = actor->getScene()->findActor((const char*)prop.p_data);
	if (ta != NULL)
		actor->setTarget(ta);
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - Target actor not found - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());
		return false;
	}

	return true;
}

CzXomlProperty CzActor::_getTarget(IzXomlResource* target)
{
	return CzXomlProperty(((CzActor*)target)->getTarget());
}

bool CzActor::_setGridPos(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->setGridPos((int)prop.p_vec[0], (int)prop.p_vec[1]);

	return true;
}

CzXomlProperty CzActor::_getGridPos(IzXomlResource* target)
{
	CzVec2 pos;
	int gp = ((CzActor*)target)->getGridPos();
	pos.x = (float)(gp >> 16);
	pos.y = (float)(gp & 0xffff);
	return CzXomlProperty(pos);
}

bool CzActor::_setLinearImpulse(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	CzBox2dBody* body = actor->getBox2dBody();
	if (body != NULL)
	{
		CzVec2 pos = actor->getPosition();
		body->ApplyLinearImpulse(prop.p_vec[0], prop.p_vec[1], body->getWorld()->PixelToWorldX(pos.x + prop.p_vec[2]), body->getWorld()->PixelToWorldY(pos.y + prop.p_vec[3]));
	}
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - LinearImpulse, actor has no body attached - ", actor->getName().c_str(), actor->getDebugInfo().c_str());
		return false;
	}

	return true;
}

bool CzActor::_setAngularImpulse(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	CzBox2dBody* body = actor->getBox2dBody();
	if (body != NULL)
		body->ApplyAngularImpulse(prop.p_float);
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - AngularImpulse, actor has no body attached - ", actor->getName().c_str(), actor->getDebugInfo().c_str());
		return false;
	}

	return true;
}

bool CzActor::_setForce(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	CzBox2dBody* body = actor->getBox2dBody();
	if (body != NULL)
	{
		CzVec2 pos = actor->getPosition();
		body->ApplyForce(prop.p_vec[0], prop.p_vec[1], body->getWorld()->PixelToWorldX(pos.x + prop.p_vec[2]), body->getWorld()->PixelToWorldY(pos.y + prop.p_vec[3]));
	}
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - Force, actor has no body attached - ", actor->getName().c_str(), actor->getDebugInfo().c_str());
		return false;
	}

	return true;
}

bool CzActor::_setTorque(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	CzBox2dBody* body = actor->getBox2dBody();
	if (body != NULL)
		body->ApplyTorque(prop.p_float);
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - Torque, actor has no body attached - ", actor->getName().c_str(), actor->getDebugInfo().c_str());
		return false;
	}

	return true;
}

bool CzActor::_setBox2dMaterial(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	if (prop.p_data == NULL || *((char*)prop.p_data) == 0)
	{
		actor->RemoveBody();
		return true;
	}

	CzBox2dMaterial* material;
	if (prop.Type == PT_Resource)
		material = (CzBox2dMaterial*)prop.p_data;
	else
		material = (CzBox2dMaterial*)actor->getScene()->getResourceManager()->findResource((const char*)prop.p_data, CzHashes::Box2dMaterial_Hash);
	if (material == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - Box2dMaterial not found - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());
		return false;
	}

	CzBox2dBody* body = actor->getBox2dBody();
	if (body != NULL)
		body->setBodyMaterial(material);

	return true;
}

CzXomlProperty CzActor::_getBox2dMaterial(IzXomlResource* target)
{
	CzActor* actor = (CzActor*)target;
	CzBox2dBody* body = actor->getBox2dBody();
	if (body == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - getProperty - Box2dMaterial, actor has no body attached - ", actor->getName().c_str(), actor->getDebugInfo().c_str());
		return false;
	}

	return CzXomlProperty(body->getBodyMaterial());
}

bool CzActor::_setShape(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	if (prop.p_data == NULL || *((char*)prop.p_data) == 0)
	{
		actor->RemoveBody();
		return true;
	}

	CzShape* shape;
	if (prop.Type == PT_Resource)
		shape = (CzShape*)prop.p_data;
	else
		shape = (CzShape*)actor->getScene()->getResourceManager()->findResource((const char*)prop.p_data, CzHashes::Shape_Hash);
	if (shape == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - Shape not found - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());
		return false;
	}

	CzBox2dBody* body = actor->getBox2dBody();
	if (body != NULL)
	{
		if (!add)
			body->DestroyFixtures();
		body->addFixture(shape, body->getBodyMaterial(), 0, 0);
	}

	return true;
}

bool CzActor::_setSensor(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	CzBox2dBody* body = actor->getBox2dBody();
	if (body == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - Sensor, actor has no body attached - ", actor->getName().c_str(), actor->getDebugInfo().c_str());
		return false;
	}
	
	if (add)
		body->setAsSensor(!body->isSensor());
	else
		body->setAsSensor(prop.p_bool);

	return true;
}

CzXomlProperty CzActor::_getSensor(IzXomlResource* target)
{
	CzActor* actor = (CzActor*)target;
	CzBox2dBody* body = actor->getBox2dBody();
	if (body == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - getProperty - Sensor, actor has no body attached - ", actor->getName().c_str(), actor->getDebugInfo().c_str());
		return false;
	}
	
	return CzXomlProperty(body->isSensor());
}

bool CzActor::_setCollisionFlags(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	CzBox2dBody* body = actor->getBox2dBody();
	if (body == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - CollisionFlags, actor has no body attached - ", actor->getName().c_str(), actor->getDebugInfo().c_str());
		return false;
	}

	body->setCollisionFlags((int)prop.p_vec[0], (int)prop.p_vec[1], (int)prop.p_vec[2]);
	
	return true;
}

CzXomlProperty CzActor::_getCollisionFlags(IzXomlResource* target)
{
	CzActor* actor = (CzActor*)target;
	CzBox2dBody* body = actor->getBox2dBody();
	if (body == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - CollisionFlags, actor has no body attached - ", actor->getName().c_str(), actor->getDebugInfo().c_str());
		return false;
	}

	CzVec3 flags;
	flags.x = (float)body->getCollisionCategory();
	flags.y = (float)body->getCollisionMask();
	flags.z = (float)body->getCollisionGroup();

	return CzXomlProperty(flags);
}

bool CzActor::_setOnTapped(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnTapped", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnTapped2(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnTapped2", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnTapped3(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnTapped3", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnTapped4(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnTapped4", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnTapped5(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnTapped5", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnBeginTouch(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnBeginTouch", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnBeginTouch2(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnBeginTouch2", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnBeginTouch3(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnBeginTouch3", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnBeginTouch4(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnBeginTouch4", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnBeginTouch5(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnBeginTouch5", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnEndTouch(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnEndTouch", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnEndTouch2(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnEndTouch2", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnEndTouch3(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnEndTouch3", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnEndTouch4(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnEndTouch4", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnEndTouch5(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnEndTouch5", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnCreate(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnCreate", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnDestroy(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnDestroy", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnOrientationChange(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnOrientationChange", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnCollisionStart(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnCollisionStart", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnCollisionEnd(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnCollisionEnd", (const char*)prop.p_data, true);
	return true;
}

bool CzActor::_setOnTick(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	actor->getEventsManager()->addEvent("OnTick", (const char*)prop.p_data, true);
	actor->setTickEnabled(true);
	return true;
}

bool CzActor::_setBodyAwake(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzActor* actor = (CzActor*)target;
	CzBox2dBody* body = actor->getBox2dBody();
	if (body != NULL)
	{
		if (add)
			body->SetAwake(!body->IsAwake());
		else
			body->SetAwake(prop.p_bool);
	}
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - BodyAwake, actor has no body attached - ", actor->getName().c_str(), actor->getDebugInfo().c_str());
		return false;
	}

	return true;
}
CzXomlProperty CzActor::_getBodyAwake(IzXomlResource* target)
{
	CzActor* actor = (CzActor*)target;
	CzBox2dBody* body = actor->getBox2dBody();
	if (body == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - getProperty - BodyAwake, actor has no body attached - ", actor->getName().c_str(), actor->getDebugInfo().c_str());
		return false;
	}

	return CzXomlProperty(body->IsAwake());
}

/**
 @fn	CzActor::~CzActor()

 @brief	Actor destructor.

 */

CzActor::~CzActor()
{
//	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "==== Deleting Actor - ", Name.c_str());
#if defined(CZ_ENABLE_METRICS)
	CzMetrics::TotalActorsDestroyed++;
#endif

	if (Scene != NULL)
		Scene->fixActors(this);

	SAFE_DELETE(UserPropertyList);

	// Only delete an unmanaged timeline
	setTimeline(NULL);
	setBindings(NULL);
	setSimpleBinding(NULL);

	// Delete visual if one was attached
	if (Visual != NULL)
		Visual->getManager()->removeSprite(Visual);

	// Clean up Box2D body object
	SAFE_DELETE(Box2dBody)

	// Clean-up managers
	SAFE_DELETE(UserPropertyList)
	SAFE_DELETE(ActionsManager)
	SAFE_DELETE(TimersManager)
	SAFE_DELETE(TimelinesManager)
	SAFE_DELETE(EventsManager)

	// Delete modifiers
	SAFE_DELETE(Modifiers)

	NameHash = 0;
}

/**
 @fn	void CzActor::setTimeline(CzAnimTimeline* timeline)

 @brief	Assigns an animation timeline to this actor

 Assigns an animation timeline to the actor which updates properties of the actor asynchronously from properties of the timelines animations. The animations within a timeline 
 can target specific properties of the actor.  If the actor already has a timeline assigned then it will be replaced. If the outgoing timeline is not managed then it will be 
 deleted. 

 For more information on timelines see CzAnimTimeline. 

 @param [in]	timeline	If non-null, the timeline.
 */
 
void CzActor::setTimeline(CzAnimTimeline* timeline)
{
	if (Timeline != NULL)
	{
		if (!Timeline->isManaged())
		{
			SAFE_DELETE(Timeline)
		}
	}
	Timeline = timeline;
}

/**
 @fn	void CzActor::setBindings(CzXomlBindings* bindings)

 @brief	Assigns a bindings list to this actor

 Assigns a bindings list to the actor which updates properties of the actor asynchronously from XOML variables. The bindings within a bindings list map variables to properties 
 of the actor, so that when changes to these variables are made they update the properties of the actor. If the outgoing bindings list is not managed then it will be deleted. 
 For more information on bindings see CzXomlBinding and CzXomlBindings

 @param [in]	bindings	If non-null, the bindings.
 */

void CzActor::setBindings(CzXomlBindings* bindings)
{
	if (Bindings != NULL)
	{
		if (!Bindings->isManaged())
		{
			SAFE_DELETE(Bindings)
		}
	}
	Bindings = bindings;
}

/**
 @fn	void CzActor::setSimpleBinding(CzXomlBinding* binding)

 @brief	Assigns a simple binding to this actor

 Assigns a simple binding to the actor. A simple binding can be thought of as a bindings list containing only one binding. Often, when binding actor properties  to XOML 
 variables you will only need to bind a single property (for example binding the Text property of a label to a XOML string variable). Simple bindings offer a quick and 
 easy solution that enables a binding to be made without having to create a full bindings list. Note that simple bindings do not exist in the resource manager and are 
 managed by the actor, so when a simple binding is replaced the previous simple binding will be deleted. For more information on bindings see CzXomlBinding and 
 CzXomlBindings

 @param [in,out]	binding	If non-null, the binding.
 */

void CzActor::setSimpleBinding(CzXomlBinding* binding)
{
	SAFE_DELETE(SimpleBinding)
	SimpleBinding = binding;
}

/**
 @fn	void CzActor::setCollisionRect(const CzIRect& rect)

 @brief	Sets the collision rectangle of this actor

 The Collision Rect is a rectangular area that represents the approximate area that the actor covers in the scene (usually its visible size)  WhIlst collision rects are 
 not used internally by the actor system, functionality has been provided that enables you to test for actors that overlap. CheckCollisionRect() enables you to test if 
 two actors overlay in the scene. 

 @param	rect	The collision rectangle in scene coordinates.
 */

void CzActor::setCollisionRect(const CzIRect& rect)
{
	CollisionRect = rect;
	int dx = rect.w >> 1;
	int dy = rect.h >> 1;
	CollisionSize = dx * dx + dy * dy;
}

/**
 @fn	void CzActor::setCollisionSize(int size)

 @brief	Sets this actors collision size

 Its often useful and "much" quicker to check if two actors can potentially overlap by testing their radii and distance apart. Collision Size represents the radius 
 of an actor but squared so we do not later have to use costly square roots to get the real radius. The CheckCollision() method enables you to test if two actors 
 overlap based on their radius and distance apart.

 @param	size	The radius of the actor in scene coordinates
 */

void CzActor::setCollisionSize(int size)
{
	CollisionSize = size;
//	CzIRect rc(-size >> 1, -size >> 1, size, size);
//	setCollisionRect(rc);
}

/**
 @fn	void CzActor::setLinkedTo(CzActor* actor)

 @brief	Links a child actor to a parent actor

 The actor system uses a linked child-parent hierarchy to allow actors to be assigned as children of other actors. In XOML if an actor is declared inside another 
 actors tag then it will automatically be assigned as a child of the containing actor. In code things work a little differently. Standard actors do not contain 
 a list of child actors, instead actors carry a parent link that allows them to be linked to a parent. This enables you to rework the heirarchy without moving actors 
 from one actors child list to another. Actor searches are also much faster as hierarchical searches do not need to be made to find actors within the scene. 

 @param [in,out]	actor	If non-null, the actor.
 */

void CzActor::setLinkedTo(CzActor* actor)
{
	if (LinkedTo == actor)
		return;

	// Notify old parent that we are removing ourselves
	if (LinkedTo != NULL)
	{
		LinkedTo->LinkChanged(this, true);
//		if (Visual != NULL)
//			Visual->setLinkedTo(NULL);
	}

	LinkedTo = actor;

	// Notify new actor that we are linking to it
	if (LinkedTo != NULL)
	{
		LinkedTo->LinkChanged(this, false);
		if (Visual != NULL && LinkedTo->getVisual() != NULL)
		{
			Visual->setLinkedTo(LinkedTo->getVisual());
		}
	}
	else
	{
		// Link back to sprite manager as we no longer link to another sprite
		if (!isDestroyed())
			if (Visual != NULL)
				Visual->setLinkedTo(NULL);
	}
}

/**
 @fn	void CzActor::setGridPos(int x, int y)

 @brief	Changes an actors grid cell when assigned to a grid

 When an actor is assigned to a grid, you can use this method to set which grid cell the actor is assigned to.

 @param	x	Horizontal grid cell.
 @param	y	Vertical grid cell.
 */

void CzActor::setGridPos(int x, int y)
{
	// Here we unlink the actor and then relink it back to the same actor in case it changes grid position (ensures that it gets linked into the correct cell)
	CzActor* actor = LinkedTo;
	setLinkedTo(NULL);
	GridPos = (x << 16) | y;
	setLinkedTo(actor);
}

/**
 @fn	void CzActor::setPosition(float x, float y)

 @brief	Sets actors scene position.

 Sets the actors x,y position in scene coordinates. If a physics body is attached then its physics world position will also be updated.

 @param	x	The x coordinate in scene coordinates.
 @param	y	The y coordinate in scene coordinates.
 */

void CzActor::setPosition(float x, float y)
{
	Position.x = x; Position.y = y;
	b2Body* body = (Box2dBody == NULL) ? NULL : Box2dBody->getBody();
	if (body != NULL)
	{
		CzBox2dWorld* world = Scene->getBox2dWorld();
		if (world != NULL)
			body->SetTransform(b2Vec2(world->PixelToWorldX(x), world->PixelToWorldY(y)), body->GetAngle());
	}
}

/**
 @fn	void CzActor::setDocking(CzScene::eDocking docking, int width, int height)

 @brief	Sets this actors docking position.

 Actors can be docked to the edges of the screen to enable tidy resolution independent placement of actors. Actors can be docked to 8 differrnt locations, including:
 - Top
 - Left
 - Right
 - Bottom
 - Top-left
 - Top-right
 - Bottom-left
 - Bottom-right

 Note that if an actor is added to a CzUICanvas then the canvas will take over docking and dock the actor to its own buonds. Also note that if the screen resolution / 
 orientation changes docked actors will be repositioned to fit the new resolution.

 @param	docking	The docking position.
 @param	width  	The width of the actor in scene coordinates.
 @param	height 	The height of the actor in scene coordinates.
 */

void CzActor::setDocking(CzScene::eDocking docking, int width, int height)
{
	CzIVec2 size = Scene->getScreenSize();
	CzVec2 pos = CzVec2(0, 0);
	CzSprite::eAnchor anchor = Visual->getAnchor();
	float w = width * Scale.x;
	float h = height * Scale.y;

	switch (docking)
	{
	case CzScene::Dock_Top:
		pos = Scene->ScreenToVirtual(0, 0);
		pos.x = Position.x;
		if (anchor == CzSprite::TopLeft)
			pos.y += Margin.w;
		else
			pos.y += (h + 1) / 2 + Margin.w;
		break;
	case CzScene::Dock_Bottom:
		pos = Scene->ScreenToVirtual(0, (float)size.y);
		pos.x = Position.x;
		if (anchor == CzSprite::TopLeft)
			pos.y -= h + Margin.h;
		else
			pos.y -= (h + 1) / 2 + Margin.h;
		break;
	case CzScene::Dock_Left:
		pos = Scene->ScreenToVirtual(0, 0);
		if (anchor == CzSprite::TopLeft)
			pos.x += Margin.x;
		else
			pos.x += (w + 1) / 2 + Margin.x;
		pos.y = Position.y;
		break;
	case CzScene::Dock_Right:
		pos = Scene->ScreenToVirtual((float)size.x, 0);
		if (anchor == CzSprite::TopLeft)
			pos.x -= w + Margin.y;
		else
			pos.x -= (w + 1) / 2 + Margin.y;
		pos.y = Position.y;
		break;
	case CzScene::Dock_TopLeft:
		pos = Scene->ScreenToVirtual(0, 0);
		if (anchor == CzSprite::TopLeft)
		{
			pos.x += Margin.x;
			pos.y += Margin.w;
		}
		else
		{
			pos.x += (w + 1) / 2 + Margin.x;
			pos.y += (h + 1) / 2 + Margin.w;
		}
		break;
	case CzScene::Dock_TopRight:
		pos = Scene->ScreenToVirtual((float)size.x, 0);
		if (anchor == CzSprite::TopLeft)
		{
			pos.x -= w + Margin.y;
			pos.y += Margin.w;
		}
		else
		{
			pos.x -= (w + 1) / 2 + Margin.y;
			pos.y += (h + 1) / 2 + Margin.w;
		}
		break;
	case CzScene::Dock_BottomLeft:
		pos = Scene->ScreenToVirtual(0, (float)size.y);
		if (anchor == CzSprite::TopLeft)
		{
			pos.x += Margin.x;
			pos.y -= h + Margin.h;
		}
		else
		{
			pos.x += (w + 1) / 2 + Margin.x;
			pos.y -= (h + 1) / 2 + Margin.h;
		}
		break;
	case CzScene::Dock_BottomRight:
		pos = Scene->ScreenToVirtual((float)size.x, (float)size.y);
		if (anchor == CzSprite::TopLeft)
		{
			pos.x -= w + Margin.y;
			pos.y -= h + Margin.h;
		}
		else
		{
			pos.x -= (w + 1) / 2 + Margin.y;
			pos.y -= (h + 1) / 2 + Margin.h;
		}
		break;
	}

	setPosition(pos);

	Docking = docking;
}

/**
 @fn	void CzActor::setDocking(CzScene::eDocking docking)

 @brief	Sets this actors docking position.

 A version of setDocking(CzScene::eDocking docking, int width, int height) that does not require a specific width and height. Instead these are calculated from the effective 
 size of the actors visual.

 @param	docking	The docking position.
 */

void CzActor::setDocking(CzScene::eDocking docking)
{
	if (Visual == NULL)
	{
		Docking = docking;
		return;
	}

	if (ScreenDocking)
		setDocking(docking, (int)Visual->getDestWidth(), (int)Visual->getDestHeight());
}

/**
 @fn	void CzActor::setAngle(float angle)

 @brief	Sets th angle of this actor

 Sets the actors orientation in the scene. If a physics body is attached then its physics world orientation will also be updated.

 @param	angle	The angle in degrees.
 */

void CzActor::setAngle(float angle)
{
	Angle = angle;
	Angle = -Angle;
	b2Body* body = (Box2dBody == NULL) ? NULL : Box2dBody->getBody();
	if (body != NULL)
	{
		body->SetTransform(body->GetPosition(), CzMath::DegToRad(Angle));	// Degrees to radians
	}
}

/**
 @fn	void CzActor::setVelocity(float x, float y)

 @brief	Sets the linear velocity of this actor.

 Sets the actors linear velocity. If a physics body is attached then its physics world linear velocity will also be updated.

 @param	x	The x velocity in scene coordinates.
 @param	y	The y velocity in scene coordinates.
 */

void CzActor::setVelocity(float x, float y)
{
	Velocity.x = x;  Velocity.y = y;
	b2Body* body = (Box2dBody == NULL) ? NULL : Box2dBody->getBody();
	if (body != NULL)
	{
		body->SetLinearVelocity(b2Vec2(x, y));
	}
}

/**
 @fn	void CzActor::setVelocityDamping(float x, float y)

 @brief	Sets the linear velocity damping of this actor.

 Sets the actors linear velocity damping. If a physics body is attached then its physics world linear velocity damping will also be updated.

 @param	x	The x velocity damping.
 @param	y	The y velocity damping.
 */

void CzActor::setVelocityDamping(float x, float y)
{
	VelocityDamping.x = x;  VelocityDamping.y = y;
	b2Body* body = (Box2dBody == NULL) ? NULL : Box2dBody->getBody();
	if (body != NULL)
		body->SetLinearDamping(x);
}


/**
 @fn	void CzActor::setVelocityDamping(float damping)

 @brief	Sets the velocity damping of this actor.

 Sets the actors velocity damping to the same value on both x and y axis. If a physics body is attached then its physics world linear velocity damping will also be updated.

 @param	damping	The linear velocity damping.
 */

void CzActor::setVelocityDamping(float damping)
{
	setVelocityDamping(damping, damping);
}

/**
 @fn	void CzActor::setAngularVelocity(float velocity)

 @brief	Sets the angular velocity of this actor.

 Sets the actors angular velocity. If a physics body is attached then its physics world angular velocity will also be updated.

 @param	velocity	The angular velocity.
 */

void CzActor::setAngularVelocity(float velocity)
{
	AngularVelocity = velocity;
	b2Body* body = (Box2dBody == NULL) ? NULL : Box2dBody->getBody();
	if (body != NULL)
		body->SetAngularVelocity(AngularVelocity);
}

/**
 @fn	void CzActor::setAngularVelocityDamping(float damping)

 @brief	Sets the angular velocity of this actor.

 Sets the actors angular velocity damping. If a physics body is attached then its physics world angular velocity damping will also be updated.

 @param	damping	The angular velocity damping.
 */

void CzActor::setAngularVelocityDamping(float damping)
{
	AngularVelocityDamping = damping;
	b2Body* body = (Box2dBody == NULL) ? NULL : Box2dBody->getBody();
	if (body != NULL)
		body->SetAngularDamping(AngularVelocityDamping);
}

/**
 @fn	void CzActor::setActive(bool active)

 @brief	Sets this actor as active.

 Active actors are processed / updated when they are active. An inactive actor's processing is suspended. You can set actors an inactive if you want to reeuce processing 
 or disable specific actors in a scene. Note that active actors will still be displayed if they are visible.

 @param	active	true to make active.
 */

void CzActor::setActive(bool active)
{
	if (IsActive == active)
		return;
	IsActive = active;
	b2Body* body = (Box2dBody == NULL) ? NULL : Box2dBody->getBody();
	if (body != NULL)
		body->SetActive(active);
}

/**
 @fn	void CzActor::setVisible(bool visible, bool force_set)

 @brief	Makes an actor visible.

 Actors can be made visible / invisible by setting their visibility. An invsiible actor will not be displayed but will still be processed if it is active.

 @param	visible  	true to show, false to hide.
 @param	force_set	unused at this time
 */

void CzActor::setVisible(bool visible, bool force_set)
{
	if (Visual != NULL)
	{
		if (visible && !IsVisible)
		{
			if (Timeline != NULL)
				Timeline->ForceUpdateTargetOnly(this);

			Visual->forceTransformDirty();
		}
	}
	IsVisible = visible;
}

/**
 @fn	void CzActor::setCollidable(bool collidable)

 @brief	Makes this actor collidable

 When an actor is made collidable it can collide with other actors in the scene. Note that collision between actors is only supported for actors that are under control of 
 the physics system.

 @param	collidable	true if collidable.
 */

void CzActor::setCollidable(bool collidable)
{
	IsCollidable = collidable;
	if (Box2dBody != NULL)
	{
		if (!collidable)
			Box2dBody->setCollisionFlags(0, 0, 0);
		else
		{
			int flags = Box2dBody->getCollisionFlags();
			Box2dBody->setCollisionFlags(flags >> 16, (flags >> 8) & 0xff, flags & 0xff);
		}
	}
}

/**
 @fn	void CzActor::Init()

 @brief	Initialises this actor

 Places the actor into an initialised and usable state. Most other actor classes override this method and take care of per actor initialisation.

 */

void CzActor::Init()
{
}

void CzActor::SetFromBrush(IzBrush* brush)
{
}

/**
 @fn	void CzActor::Reset()

 @brief	Resets this actor

 In a system where actor objects are "re-used", such as in some kind of actor pooling system its often handy to be able to reset the actor into a default state so 
 that the actor can be re-used. Memory pooling system pre-allocate a group of objects and re-use them instead of constantly creating and destroying them. Reset 
 offers a way to reset the actor to its default state so it can be re-used. 

 */

void CzActor::Reset()
{
	IsActive = true;
	IsVisible = true;
	IsCollidable = false;
	IsTappable = true;
	IsDraggable = false;
	IsDragging = false;
	WrapPosition = false;
	Type = 0;
	Position.x = 0;
	Position.y = 0;
	Depth = 1.0f;
	Origin.x = 0;
	Origin.y = 0;
	OriginalPosition.x = 0;
	OriginalPosition.y = 0;
	OriginalScale.x = 1.0f;
	OriginalScale.y = 1.0f;
	Scale.x = 1.0f;
	Scale.y = 1.0f;
	OriginalAngle = 0;
	Angle = 0;
	Velocity.x = 0;
	Velocity.y = 0;
	AngularVelocity = 0;
	VelocityDamping.x = 1.0f;
	VelocityDamping.y = 1.0f;
	AngularVelocityDamping = 1.0f;
	CollisionSize = 0;
	CollisionRect.x = 0;
	CollisionRect.y = 0;
	CollisionRect.w = 0;
	CollisionRect.h = 0;
	Colour.r = 0xff;
	Colour.g = 0xff;
	Colour.b = 0xff;
	Colour.a = 0xff;
	OriginalColour.r = 0xff;
	OriginalColour.g = 0xff;
	OriginalColour.b = 0xff;
	OriginalColour.a = 0xff;
	Layer = 0;
	AspectLock = AL_None;
	Destroyed = false;
	TouchIndex = -1;
	Docking = CzScene::Dock_None;
	Margin.x = 0;
	Margin.y = 0;
	Margin.w = 0;
	Margin.h = 0;
	Size.x = 0;
	Size.y = 0;
	GridPos = 0;
	Target = NULL;
	ReceiveEventFromChild = false;
	Category = AC_Generic;
	ScreenDocking = true;
	HoldFocus = false;
	UseParentOpacity = true;
	TickEnabled = false;
	DragAnchor.x = 0;
	DragAnchor.y = 0;
	OriginalMargin.x = 0;
	OriginalMargin.y = 0;
	OriginalMargin.w = 0;
	OriginalMargin.h = 0;
	OriginalSize.x = 0;
	OriginalSize.y = 0;
	PreviousPosition.x = 0;
	PreviousPosition.y = 0;
	PreviousAngle = 0;
}

/**
 @fn	int CzActor::FindChildren(CzVector<CzActor*>& children)

 @brief	Finds all actors that children of this actor

 Searches this actors scene for all actors that link to it (actors that are children) and places them into the provided children vector. 

 @param [out]	children	[out] If non-null, the children.

 @return	The number of children.
 */

int CzActor::FindChildren(CzVector<CzActor*>& children)
{
	int count = 0;
	for (CzScene::_Iterator it = Scene->begin(); it != Scene->end(); ++it)
	{
		if ((*it)->getLinkedTo() == this)
		{
			count++;
			children.push_back(*it);
		}
	}

	return count;
}

/**
 @fn	void CzActor::RemoveChildren()

 @brief	Removes all actors that are children of this actor.

 Searches the scene for actors that are children of this actor, removes them from the scene and deletes them. Note that whilst an actor is immediately removed from the scene 
 it is not deleted until the scene has finished being processed and the actor clean up stage takes place. In the meantime the actor is marked as Destroyed.

 */

void CzActor::RemoveChildren()
{
	for (CzScene::_Iterator it = Scene->begin(); it != Scene->end(); ++it)
	{
		if ((*it)->getLinkedTo() == this)
			Scene->removeActor(*it);
	}
}

/**
 @fn	void CzActor::BringActorToFront()

 @brief	Brings this actor to front of the scene.

 Removes the actor from the scene and adds it at the end of the scenes actor list. This causes the actor to be placed above any other actors that are on the same layer.

 */

void CzActor::BringActorToFront()
{
	Scene->removeActorNoDelete(this);
	Scene->addActor(this);
	if (Visual != NULL)
		Visual->BringToFront();
}

/**
 @fn	bool CzActor::Update(float dt)

 @brief	Updates this actors logic

 Updates the actors logic, including:
 - Processing event actions
 - Processing any attached modifiers
 - Updating dragging state
 - Updating physics
 - Updating the attached timeline

 This method should be overriden by derived actors to provide the actor specific logic. 

 Note that dt is used to scale actor internals that rely on time. This value enables the actor run in an environment where time (the frame rate of the device) can vary allowing 
 animation to keep up without slowing down or speeding up when the frame rate varies. 

 Note that if an actor returns false then it will automatically be removed from the scene.

 @param	dt	The number of seconds since the app was last updated

 @return	true if it succeeds, false if it fails.
 */

bool CzActor::Update(float dt)
{
	if (!IsActive)
		return true;

	if (TickEnabled)
		ProcessEventActions(CzHashes::OnTick_Hash);

	// Update any data bindings
	UpdateBindings();

	// Update modifiers
	if (Modifiers != NULL)
		Modifiers->Execute(this, dt);

	// Update timers
	TimersManager->Update(dt);

	if (!IsVisible)
		return true;

	// Update dragging
	if (IsDraggable)
	{
		if (IsDragging)
		{
			CzIVec2 raw_touch_pos  = CZ_INPUT->getTouchedPos(TouchIndex);
			CzVec2 pos = Scene->ScreenToCamera((float)raw_touch_pos.x, (float)raw_touch_pos.y);
			setPosition(pos.x + DragAnchor.x, pos.y + DragAnchor.y);
		}
	}

	b2Body* body = (Box2dBody == NULL) ? NULL : Box2dBody->getBody();
	if (body != NULL)
	{
		// Update actor position and angle to match the physics engine
		float x = body->GetPosition().x;
		float y = body->GetPosition().y;
		CzBox2dWorld* world = Scene->getBox2dWorld();
		if (world != NULL)
		{
			Position.x = world->WorldToPixelX(x);
			Position.y = world->WorldToPixelY(y);
		}
		Angle = (body->GetAngle() * 180.0f) / 3.1415927f;	// Radians to degrees
	}
	else
	{
		// Update velocity
		PreviousPosition = Position;
		Position += Velocity * dt;
		Velocity.x *= VelocityDamping.x;
		Velocity.y *= VelocityDamping.y;
		PreviousAngle = Angle;
		Angle += AngularVelocity * dt;
		AngularVelocity *= AngularVelocityDamping;
	}

	// Wrap actor position with scene extents (if enabled)
	CzIRect extents = Scene->getExtents();
	if (WrapPosition)
	{
		bool wrapped = false;
		if (Position.x < extents.x)
		{
			Position.x += extents.w;
			wrapped = true;
		}
		else
		if (Position.x >= (extents.x + extents.w))
		{
			Position.x -= extents.w;
			wrapped = true;
		}
		if (Position.y < extents.y)
		{
			Position.y += extents.h;
			wrapped = true;
		}
		else
		if (Position.y >= (extents.y + extents.h))
		{
			Position.y -= extents.h;
			wrapped = true;
		}
		if (wrapped && body != NULL)
		{
			CzBox2dWorld* world = Scene->getBox2dWorld();
			if (world != NULL)
				body->SetTransform(b2Vec2(world->PixelToWorldX(Position.x), world->PixelToWorldX(Position.y)), body->GetAngle());
		}
	}

	// Update the animation timeline
	if (Timeline != NULL)
	{
		if (!Timeline->Update(dt, this))
		{
			if (Timeline->isAutoDelete())
				Timeline = NULL;
		}
	}

	// If this actor is collidable then add it to the scenes potential collidables list
//	if (IsCollidable)
//		Scene->addCollideable(this);

	return true;
}

/**
 @fn	void CzActor::ResolveCollisions()

 @brief	Resolve collisions.

 Derived actors that are interested in collision implement this method to determine what they could have collided with. There is no base implementation for this method.

 Here is a short example that shows:

 @par Example
 <blockquote><pre>
	void ActorPlayer::ResolveCollisions()
	{
		for (CzScene::_Iterator it = Scene->begin(); it != Scene->end(); ++it) // Walk the scenes actors
		{
			if ((*it)->getType() == 1)				// Only test collision against ball type actors
			{
				if (CheckCollision(*it))			// Check for physical collision
				{
					NotifyCollision(*it);			// Notify ourselves that we collided with ball actor
					(*it)->NotifyCollision(this);	// Notify ball actor that we collided with it
				}
			}
		}
	}
 </pre></blockquote>

 */

void CzActor::ResolveCollisions()
{
}

/**
 @fn	void CzActor::NotifyCollision(CzActor* other)

 @brief	Notifies this actor that a collision has taken place.

 This method is usually implemented by actors that wnat to know when a collision nivolving them takes place. This method is usually called by the actor that hit this actor.

 @param [in,out]	other	If non-null, the other.
 */

void CzActor::NotifyCollision(CzActor* other)
{
}

/**
 @fn	bool CzActor::UpdateVisual()

 @brief	Updates this actors visual component.

 Updates the visual component of the actor moving actor data such as position, colour to its visual. 

 This method can be overriden by derived actors to provide the actor specific display logic. 


 @return	true if it succeeds, false if it fails.
 */

bool CzActor::UpdateVisual()
{
	if (Visual == NULL || Destroyed)
		return false;

	if (!IsVisible || !IsActive)
	{
		Visual->setVisible(false);
		return false;
	}
	else
	{
		// If we are linked to another actor then check that it is visible
		if (LinkedTo != NULL)
		{
			if (!LinkedTo->isVisible() || !LinkedTo->isActive())
			{
				Visual->setVisible(false);
				return false;
			}
		}
	}

	Visual->setVisible(true);

//	if (LinkedTo != NULL)
//		Visual->setLinkedTo(LinkedTo->getVisual());

	Visual->setIgnoreCamera(IgnoreCamera);
	Visual->setDepth(Depth);
	Visual->setOrigin(Origin.x, Origin.y);
	// Transfor actor properties to the visual
	Visual->setPosAngScale(	Position.x, Position.y, Angle, Scale.x, Scale.y);

	// Multiply actor colour by scene colour
	CzColour c = Colour;
	CzColour c2 = Scene->getColour();
	c.r = (uint8)(((int)c.r * c2.r) / 255);
	c.g = (uint8)(((int)c.g * c2.g) / 255);
	c.b = (uint8)(((int)c.b * c2.b) / 255);
	c.a = (uint8)(((int)c.a * c2.a) / 255);

	Visual->setColour(c, UseParentOpacity);

	// Set visible layer
	Visual->setLayer(Layer);

	return true;
}

/**
 @fn	void CzActor::removeVisual()

 @brief	Removes this actors visual.

 If the actor has a visual then it will be removed and deleted.

 */

void CzActor::removeVisual()
{
	if (Visual != NULL)
	{
		Scene->getSpriteManager()->removeSprite(Visual);
		Visual = NULL;
	}
}

/**
 @fn	CzActor* CzActor::HitTest(float x, float y)

 @brief	hecks to see if an x,y point hits / touches the actor

 Checks to see if the supplied point is within the area that the actors visual covers. Testing is done in screen coordinares 

 @param	x	The x screen coordinate to test.
 @param	y	The y screen coordinate to test.

 @return	null if it no hit, else actor that was hit.
 */

CzActor* CzActor::HitTest(float x, float y)
{
	if (Visual == NULL || !isTappable() || !Visual->isVisibleWithParents())
		return NULL;

	if (Visual->HitTest(x, y))
		return this;		// We return a CzActor so we can return a different actor if this actor hosts child actors

	return NULL;
}

/**
 @fn	bool CzActor::isOutsideFocusRange(float x, float y, float scale)

 @brief	Checks to see if the x,y point is outside the focus range

 Checks the point x, y to see if it is outside the focus range of the actor. The focus range is calculated as the radius of the visual multiplied by scale. Focus 
 range is used to decide when to release the touch focus on an actor that currently has touch focus.

 @param	x		The x screen coordinate to test.
 @param	y		The y screen coordinate to test.
 @param	scale	The scale.

 @return	true if outside focus range, false if not.
 */

bool CzActor::isOutsideFocusRange(float x, float y, float scale)
{
	if (HoldFocus)
		return false;

	if (Visual == NULL)
		return true;

	return Visual->isOutsideFocusRange(x, y, scale);
}

/**
 @fn	CzVec2 CzActor::TransformPoint(float x, float y)

 @brief	Transform point.

 Transform point x, y by actors local angle / scale transform. This method is useful if you want to figure out where a paticular point on an actor would be in the scene. 

 @param	x	The x coordinate relative to the actor in scene coordinates.
 @param	y	The y coordinate relative to the actor in scene coordinates.

 @return	The transformed point in scene coordinates
 */

CzVec2 CzActor::TransformPoint(float x, float y)
{
	if (Visual != NULL)
		return Visual->TransformPoint(x, y);

	return CzVec2(0, 0);
}

/**
 @fn	CzVec2 CzActor::TransformPointToScreen(float x, float y)

 @brief	Transform point to screen.

 Transform point x, y by actors local angle / scale transform. This method is useful if you want to figure out where a paticular point on an actor would be on screen.

 @param	x	The x coordinate relative to the actor in scene coordinates.
 @param	y	The y coordinate relative to the actor in scene coordinates.

 @return	.
 */

CzVec2 CzActor::TransformPointToScreen(float x, float y)
{
	if (Visual != NULL)
		return Visual->TransformPointToScreen(x, y);

	return CzVec2(0, 0);
}

/**
 @fn	bool CzActor::TestOverlap(CzActor* other)

 @brief	Tests for overlap between between two actors.

 Utility method that performs very basic distance check between this actor and another to see if they overlap.

 @param [in]	other	If non-null, the other actor

 @return	true if hit, false if it no hit.
 */

bool CzActor::TestOverlap(CzActor* other)
{
	// This is just about as simple as colision checks can go. We check to see if the distance between the centre of both objects is less than the sum of the radii of both
	// Note that we do not use a proper distance between two points check because that involves expensive square root calculations, but this works just the same
	// The only thing to be careful of is if your coordinate system is huge and your numbers have a large range, just ensure that the term for d = dx * dx + dy * dy does not fall 
	// out of integer range
	CzVec2 other_pos = other->getPosition();
	int		other_collision_size = other->getCollisionSize();
	int		dx = (int)(Position.x - other_pos.x);
	int		dy = (int)(Position.y - other_pos.y);
	int		d_max = (CollisionSize >> 1) + (other_collision_size >> 1);
	int		d = dx * dx + dy * dy;

	d_max *= d_max;
	if (d <= d_max)
		return true;

	return false;
}

/**
 @fn	bool CzActor::TestOverlapRect(CzActor* other)

 @brief	Test  for overlap between two actors.

 Utility method that performs rect overlap tests distance between this actor and another to see if they overlap. This method of pverlapping test is more computationally 
 expensive but also more accurate.

 @param [in]	other	If non-null, the other actor.

 @return	true if hit, false if it no hit.
 */

bool CzActor::TestOverlapRect(CzActor* other)
{
	if (Visual == NULL || other == NULL || other->getVisual() == NULL)
		return false;

	return Visual->TestOverlap(other->getVisual());
}

/**
 @fn	CzActor* CzActor::FindEventDependentParent()

 @brief	Finds the event dependent parent of this actor.

 Utility method that searches up the actor hierarchy to find the first parent that can receive events from its childen. Actors can be made to bubble events up their heirarchy to 
 parent actors so they can also receieve the same events. This method will find the first parent that can receieve bubbled events. Below is an example actor that shows event 
 bubbling:

 @par Example
 <blockquote><pre>
	<StackPanel Orientation="Vertical" Bubbling="true">
		<Label .... />
	</StackPanel>
 </pre></blockquote>

 In the above example the label and the StackPanel can both receive input events as the StackPanel parent has event bubbling enabled.

 @return	null if it fails, else the parent that can receive events from its children.
 */

CzActor* CzActor::FindEventDependentParent()
{
	// Search down the parent chain to see if any actors allow event bubbling
	CzActor* parent = LinkedTo;
	while (parent != NULL)
	{
		if (parent->canReceiveEventFromChild())
			return parent;

		parent = parent->LinkedTo;
	}

	return NULL;
}

/**
 @fn	CzActor* CzActor::FindClipper()

 @brief	Searches up the actor heirarchy for an actor that is a clipper

 Starting from this actor, searches up the actor hierarchy for an actor that is a clipper. An actor that is a clipper is an actor that clips its children against its bounds. Clippers 
 are used extensively by the UI actior system with container actors clipping their child actors against their bounds.

 @return	null if it fails, else actor that is a clipper.
 */

CzActor* CzActor::FindClipper()
{
	if (Visual != NULL)
	{
		if (Visual->affectsClip())
			return this;
	}

	if (LinkedTo != NULL)
		return LinkedTo->FindClipper();

	return NULL;
}

/**
 @fn	void CzActor::NotifyTapped(int index, int x, int y, bool allow_bubble)

 @brief	OnTapped event handler

 The OnTapped event handler is called whenever the actor is tapped on by the user. Actors can process up to 5 simultaneous touch events OnTapped to OnTapped5. if event bubbling is enabled 
 then the event will also be passed up to the parent. 

 @param	index			Index of the touch.
 @param	x				The touched x coordinate.
 @param	y				The touched y coordinate.
 @param	allow_bubble	true to allow event bubbling, false to deny event bubbling.
 */

void CzActor::NotifyTapped(int index, int x, int y, bool allow_bubble)
{
//	if (TouchIndex >= 0)
//		return;

	switch (index)
	{
	case 0:
		ProcessEventActions(CzHashes::OnTapped_Hash);
		break;
	case 1:
		ProcessEventActions(CzHashes::OnTapped2_Hash);
		break;
	case 2:
		ProcessEventActions(CzHashes::OnTapped3_Hash);
		break;
	case 3:
		ProcessEventActions(CzHashes::OnTapped4_Hash);
		break;
	case 4:
		ProcessEventActions(CzHashes::OnTapped5_Hash);
		break;
	}

	if (allow_bubble)
	{
		// Notify any dependent parents
		CzActor* parent = FindEventDependentParent();
		if (parent != NULL)
			parent->NotifyTapped(index, x, y, allow_bubble);
	}
}

/**
 @fn	void CzActor::NotifyBeginTouch(int index, int x, int y, bool allow_bubble)

 @brief	OnBeginTouch event handler

 The OnBeginTouch event handler is called whenever the user begins to touch the actor. Actors can process up to 5 simultaneous touch events OnBeginTouch to OnBeginTouch5. if event 
 bubbling is enabled then the event will also be passed up to the parent. 

 @param	index			Index of the touch.
 @param	x				The touched x coordinate.
 @param	y				The touched y coordinate.
 @param	allow_bubble	true to allow event bubbling, false to deny event bubbling.
 */

void CzActor::NotifyBeginTouch(int index, int x, int y, bool allow_bubble)
{
//	if (TouchIndex >= 0)
//		return;

	if (TouchIndex < 0)
		TouchIndex = index;
	IsDragging = true;
	if (IsDraggable)
	{
		CzVec2 pos = Scene->ScreenToCamera((float)x, (float)y);
		DragAnchor.x = (int)(Position.x - pos.x);
		DragAnchor.y = (int)(Position.y - pos.y);
	}
	switch (index)
	{
	case 0:
		ProcessEventActions(CzHashes::OnBeginTouch_Hash);
		break;
	case 1:
		ProcessEventActions(CzHashes::OnBeginTouch2_Hash);
		break;
	case 2:
		ProcessEventActions(CzHashes::OnBeginTouch3_Hash);
		break;
	case 3:
		ProcessEventActions(CzHashes::OnBeginTouch4_Hash);
		break;
	case 4:
		ProcessEventActions(CzHashes::OnBeginTouch5_Hash);
		break;
	}

	if (allow_bubble)
	{
		// Notify any dependent parents
		CzActor* parent = FindEventDependentParent();
		if (parent != NULL)
			parent->NotifyBeginTouch(index, x, y, allow_bubble);
	}
}

/**
 @fn	void CzActor::NotifyEndTouch(int index, int x, int y, bool allow_bubble)

 @brief	OnEndTouch event handler

 The OnEndTouch event handler is called whenever the user stops touching the actor. This can occur for a number fo reasons including:
 - if the user lifts their finger off the actor
 - If the user moves their finger too from the actor (loses focus)
 - if the user scrolls the container (list boc for example)
 - The scene is switched

 Actors can process up to 5 simultaneous touch events OnEndTouch to OnEndTouch5. if event bubbling is enabled then the event will also be passed up to the parent. 

 @param	index			Index of the touch.
 @param	x				The touched x coordinate.
 @param	y				The touched y coordinate.
 @param	allow_bubble	true to allow event bubbling, false to deny event bubbling.
 */

void CzActor::NotifyEndTouch(int index, int x, int y, bool allow_bubble)
{
	IsDragging = false;

	switch (index)
	{
	case 0:
		ProcessEventActions(CzHashes::OnEndTouch_Hash);
		break;
	case 1:
		ProcessEventActions(CzHashes::OnEndTouch2_Hash);
		break;
	case 2:
		ProcessEventActions(CzHashes::OnEndTouch3_Hash);
		break;
	case 3:
		ProcessEventActions(CzHashes::OnEndTouch4_Hash);
		break;
	case 4:
		ProcessEventActions(CzHashes::OnEndTouch5_Hash);
		break;
	}

	if (allow_bubble)
	{
		// Notify any dependent parents
		CzActor* parent = FindEventDependentParent();
		if (parent != NULL)
			parent->NotifyEndTouch(index, x, y, allow_bubble);
	}
	TouchIndex = -1;
}

/**
 @fn	void CzActor::NotifyCreate()

 @brief	Handles the create event

 When an actor is created it will raise the OnCreate event. This can be used to call actions or do other post actor creation huose keeping.

 */

void CzActor::NotifyCreate()
{
	ProcessEventActions(CzHashes::OnCreate_Hash);
}

/**
 @fn	void CzActor::NotifyDestroy()

 @brief	Handles the destroy event.

 When an actor is about to be destroyed it will raise the OnDestroy event. This can be used to call actions or do other pre actor destruction house keeping.

 */

void CzActor::NotifyDestroy()
{
	ProcessEventActions(CzHashes::OnDestroy_Hash);
//	removeVisual();
	setLinkedTo(NULL);
}

/**
 @fn	void CzActor::NotifyOrientationChange(CzScene::eOrientation old_orientation,  CzScene::eOrientation new_orientation)

 @brief	Handles the orientation change event.

 When a screen orientation or size change occurs all actors will receive this event enabling them to modify themselves to take advantage of the new screen orientation / size.  This 
 default implementation will update the actors position and call any attached OnOrientationChange actions.

 @param	old_orientation	The old orientation.
 @param	new_orientation	The new orientation.
 */

void CzActor::NotifyOrientationChange(CzScene::eOrientation old_orientation, CzScene::eOrientation new_orientation)
{
	if (PercentagePosition)
	{
		CzVec2 pos = PercPosToPos(OriginalPosition.x, OriginalPosition.y);
		setPosition(pos.x, pos.y);
	}

	if (Timeline != NULL)
		Timeline->ForceUpdateTargetOnly(this);

	ProcessEventActions(CzHashes::OnOrientationChange_Hash);
}

/**
 @fn	void CzActor::NotifySizeChange(CzScene::eOrientation old_orientation,  CzScene::eOrientation new_orientation)

 @brief	Handles the screen size change event.

 When a screen size change (not orientation change) occurs all actors will receive this event enabling them to modify themselves to take advantage of the new screen size.  This 
 default implementation will update the actors position and call any attached OnSizeChange actions.

 */

void CzActor::NotifySizeChange()
{
	if (PercentagePosition)
	{
		CzVec2 pos = PercPosToPos(OriginalPosition.x, OriginalPosition.y);
		setPosition(pos.x, pos.y);
	}

	if (Timeline != NULL)
		Timeline->ForceUpdateTargetOnly(this);

	ProcessEventActions(CzHashes::OnSizeChange_Hash);
}

/**
 @fn	void CzActor::ProcessEventActions(unsigned int event_name)

 @brief	Process the event actions described by event_name.

 Processes the actions list that is associated with the event named event_name (event_name is a hash of the actual event name to speed up searches)

 @param	event_name	Name of the event as a string hash
 */

void CzActor::ProcessEventActions(unsigned int event_name)
{
	// Find the event 
	CzEvent* evt = EventsManager->findEvent(event_name);
	if (evt != NULL)
	{
		// Find actions list
		CzActions* actions = CzActionsManager::FindActions(evt->ActionsName, this);
		if (actions != NULL)
			actions->Execute(this);
	}
}

/**
 @fn	void CzActor::RemoveBody()

 @brief	Removes the physics body.

 Removes the physics body that is associatd with this actor. Also sets all velocities to 0.

 */

void CzActor::RemoveBody()
{
	setCollidable(false);
	Velocity.x = 0;
	Velocity.y = 0;
	AngularVelocity = 0;
	if (Box2dBody != NULL)
	{
		Box2dBody->ReleaseBody();
		SAFE_DELETE(Box2dBody)
	}
}

/**
 @fn	CzVec2 CzActor::PercPosToPos(float x, float y)

 @brief	Converts percentage based position to scene position.

 Utility method that converts the supplied percentage based position to screen position.

 @param	x	The x coordinate as a percentage.
 @param	y	The y coordinate as a percentage.

 @return	Screen position.
 */

CzVec2 CzActor::PercPosToPos(float x, float y)
{
	CzVec2 canvas_size = CzVec2((float)Scene->getScreenSize().x, (float)Scene->getScreenSize().y);
	return CzVec2((canvas_size.x * x) / 100.0f, (canvas_size.y * y) / 100.0f);
}

/**
 @fn	float CzActor::getDistanceBetween(CzActor* actor)

 @brief	Gets distance between two actors

 Calculates the euclidean distance between this actor and another. 

 @param [in]	actor	If non-null, the actor.

 @return	The distance between the two actors.
 */

float CzActor::getDistanceBetween(CzActor* actor)
{
	CzVec2 pos = actor->getPosition();
	float dx = pos.x - Position.x;
	float dy = pos.y - Position.y;
	float d = dx * dx + dy * dy;
	if (d == 0)
		return 0;

	return sqrtf(d);
}

/**
 @fn	float CzActor::getAngleBetween(CzActor* actor)

 @brief	Gets angle between two actors

 Calculates the angle between this actor and another.

 @param [in]	actor	If non-null, the actor.

 @return	The angle between the two actors in degrees
 */

float CzActor::getAngleBetween(CzActor* actor)
{
	CzVec2 pos = actor->getPosition();
	float dx = pos.x - Position.x;
	float dy = pos.y - Position.y;
	if (dx == 0 && dy == 0)
		return 0;

	return (atan2f(dy, dx) * 180.0f) / 3.14159265f;
}

/**
 @fn	bool CzActor::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the actor.

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types.

 @param	property_name		Name of the property.
 @param [out]	prop	The property data.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActor::getProperty(const char* property_name, CzXomlProperty& prop)
{
	return getProperty(CZ_HASH(property_name), prop);
}

/**
 @fn	bool CzActor::getProperty(unsigned int property_name, CzXomlProperty& prop)

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in,out]	prop	The property.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActor::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	int err = IzXomlResource::getProperty(ActorClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzActor::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the actor.
	
 Sets the named property of this Actor. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzActor::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	int err = IzXomlResource::setProperty(ActorClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzActor::setProperty(const char* property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced.

 @param	property_name		Name of the property.
 @param [in]	data	The property data.
 @param	delta				true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActor::setProperty(const char* property_name, const CzString& data, bool delta)
{
	return setProperty(CzString::CalculateHash(property_name), data, delta);
}

/**
 @fn	bool CzActor::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.
	
 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActor::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	int err = IzXomlResource::setProperty(ActorClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzActor::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates actor property from supplied animation.

 Updates the actors property that is specified by the supplied animation. When a timeline animation targets the property of an actor this method will be called by the animation to move the 
 animations frame data into its target property. 

 @par
 <blockquote><pre>
	<Timeline Name="Anim1" AutoPlay="true">
		<Animation Anim="PosAnim" Target="Position" />
	</Timeline>
 </pre></blockquote>

 In the above example PosAnim targets the Position property. Internally the animation will call UpdateFromAnimation() passing in itself. UpdateFromAnimation() will examine the target preoperty 
 name of the animation as see that it wants to update "Position".

 @param [in]	animation	If non-null, the animation.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActor::UpdateFromAnimation(CzAnimInstance *animation)
{
	int err = IzXomlResource::setProperty(ActorClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzActor::UpdateBinding(unsigned int property_name, CzXomlVariable* var)

 @brief	Updates an actor property from the supplied XOML variable.

 Updates a single property from the value of the bound XOML variable. 

 @param	property_name  	Name of the property.
 @param [in,out]	var	If non-null, the variable.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzActor::UpdateBinding(unsigned int property_name, CzXomlVariable* var)
{
	if (!var->Modified || var->getValue().isEmpty())
		return false;

	int err = IzXomlResource::setProperty(ActorClassDef, this, property_name, var);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ActorClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	void CzActor::UpdateBindings(bool force_modified)

 @brief	Updates the actors internal bindings.

 Walks the actors bindings list updating all of its bindings and an attached simple data binding if attached. Passing to true to force_modified will force the bindings to be updated, even 
 if the system does not think the bound variables have not changed.

 @param	force_modified	true to force an update.
 */

void CzActor::UpdateBindings(bool force_modified)
{
	// Update complex bindings
	if (Bindings != NULL)
	{
		for (CzXomlBindings::_Iterator it = Bindings->begin(); it != Bindings->end(); ++it)
		{
			unsigned int property_name = (*it)->Property;
			CzXomlVariable* var = (*it)->Variable;
			int index = (*it)->Index;

			if (var != NULL)
			{
				if (var->isArray())
				{
					if (SimpleBinding->IndexVar == NULL)
					{
						// If cached index var not available then find it
						if (!SimpleBinding->VIndex.isEmpty())
							SimpleBinding->IndexVar = (CzXomlVariableInt*)CzXomlVariableManager::FindVariable(SimpleBinding->VIndex.getHash(), Scene);
					}
					if (SimpleBinding->IndexVar != NULL)
						index = SimpleBinding->IndexVar->NativeValue;

					if (index >= 0)
						var = ((CzXomlVariableArray*)var)->getElement(index);
					else
						var = NULL;
				}
			}

			if (var != NULL)
			{
				if (force_modified)
					var->Modified = true;
				UpdateBinding(property_name, var);
			}
		}
	}

	// Update simple binding
	if (SimpleBinding != NULL)
	{
		CzXomlVariable* var = SimpleBinding->Variable;
		int index = SimpleBinding->Index;

		if (var != NULL)
		{
			if (var->isArray())
			{
				if (SimpleBinding->IndexVar == NULL)
				{
					// If cached index var not available then find it
					if (!SimpleBinding->VIndex.isEmpty())
						SimpleBinding->IndexVar = (CzXomlVariableInt*)CzXomlVariableManager::FindVariable(SimpleBinding->VIndex.getHash(), Scene);
				}
				if (SimpleBinding->IndexVar != NULL)
					index = SimpleBinding->IndexVar->NativeValue;

				if (index >= 0)
					var = ((CzXomlVariableArray*)var)->getElement(index);
				else
					var = NULL;
			}
		}

		if (var != NULL)
		{
			if (force_modified)
				var->Modified = true;
			UpdateBinding(SimpleBinding->Property, var);
		}
	}
}

/**
 @fn	int CzActor::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)

 @brief	Creates an instance of this class from XOML

 LoadFromXoml is the main method used by all classes that can be instantiated from XOML mark-up. This method creates an instance of this class from the supplied XML node structure specified by node. 
 if parent is not NULL then the created instance will utilise the supplied parent to determine where it should live within the app. For example, if the parent is a scene then the actor will be placed 
 in that scene. if the parent is another actor then this actor will be placed into the scene of the supplied actor and linked as a child to the parent actor. If load_chlldren is true then any child 
 nodes contained within the actor will also be loaded.

 @param [in]	parent		If non-null, the parent.
 @param	load_children	  	true to load child nodes.
 @param [in]	node  		If non-null, the XOML markup node that defines this object

 @return	nergative value for error. if 0 then processing of further XOML will stop.
 */

int CzActor::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent == NULL || (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash))
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Actor - Needs to be declared inside a scene or an actor - ", DebugInfo.c_str());
		return 0;
	}

	// Add the actor to the scene
	if (parent->getClassTypeHash() == CzHashes::Actor_Hash)
	{
		Scene = ((CzActor*)parent)->getScene();
	}
	else
		Scene = (CzScene*)parent;

	Scene->addActor(this);

	// Check and apply style
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();
		if (name_hash == CzHashes::Style_Hash)
		{
			if (!(*it)->getValue().isEmpty())
			{
				// Find the style
				CzStyle* style = (CzStyle*)Scene->getResourceManager()->findResource((*it)->getValue().c_str(), CzHashes::Style_Hash);
				if (style == NULL)
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Could not find named style in the scene or in the global resources - ", (*it)->getValue().c_str(), DebugInfo.c_str());
				else
				{
					// Apply the style
					CzXoml::ApplyStyle(style, node);
				}
			}
		}
	}

	// Process attributes
	CzString*		box2dmaterial_name = NULL, *shape_name = NULL;
	CzString*		bindings_name = NULL, *simple_binding_name = NULL;
	CzVec2			com(0, 0);
	CzVec3			collision_flags(0, 0, 0);
	bool			sensor = false;
	bool			vd_set = false;
	bool			avd_set = false;
	bool			is_active = true;
	bool			depth_set = false;
	bool			hit_test_set = false;
	bool			layer_set = false;
	float			ang_vel = 0;
	CzVec2			vel  = CzVec2(0.0f, 0.0f);
	CzScene::eDocking docking = CzScene::Dock_None;
	bool			docking_set = false;
	CzString*		linked_to = NULL;
	CzString*		target = NULL;
	CzString*		condition = NULL;
	CzString*		anchor = NULL;

	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
			setName((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Type_Hash)
			setType((*it)->getValueAsInt());
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Position_Hash)
		{
			if (!(*it)->getValueAsPoint(Position))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Invalid value for Actor::Position, expected vec2 - ", DebugInfo.c_str());
			setOriginalPosition(Position.x, Position.y);
		}
		else
		if (name_hash == CzHashes::PercPos_Hash)
			PercentagePosition = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Origin_Hash)
		{
			if (!(*it)->getValueAsPoint(Origin))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Invalid value for Actor::Origin, expected vec2 - ", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::Velocity_Hash)
		{
			if (!(*it)->getValueAsPoint(vel))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Invalid value for Actor::Velocity, expected vec2 - ", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::VelocityDamping_Hash)
		{
			if (!(*it)->getValueAsPoint(VelocityDamping))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Invalid value for Actor::VelocityDamping, expected vec2 - ", DebugInfo.c_str());
			else
				vd_set = true;
		}
		else
		if (name_hash == CzHashes::Angle_Hash)
		{
			Angle = (*it)->getValueAsFloat();
			OriginalAngle = Angle;
		}
		else
		if (name_hash == CzHashes::AngularVelocity_Hash)
			ang_vel = (*it)->getValueAsFloat();
		else
		if (name_hash == CzHashes::AngularVelocityDamping_Hash)
		{
			AngularVelocityDamping = (*it)->getValueAsFloat();
			avd_set = true;
		}
		else
		if (name_hash == CzHashes::Scale_Hash)
		{
			Scale.x = Scale.y = (*it)->getValueAsFloat();
			setOriginalScale(Scale.x, Scale.y);
		}
		else
		if (name_hash == CzHashes::ScaleX_Hash)
		{
			Scale.x = (*it)->getValueAsFloat();
			setOriginalScale(Scale.x, 1.0f);
		}
		else
		if (name_hash == CzHashes::ScaleY_Hash)
		{
			Scale.y = (*it)->getValueAsFloat();
			setOriginalScale(1.0f, Scale.y);
		}
		else
		if (name_hash == CzHashes::Colour_Hash || name_hash == CzHashes::Color_Hash)
		{
			if (!(*it)->getValueAsColour(Colour))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Invalid value for Actor::Colour, expected colour - ", DebugInfo.c_str());
			OriginalColour = Colour;
		}
		else
		if (name_hash == CzHashes::Opacity_Hash)
		{
			Colour.a = (*it)->getValueAsInt();
			OriginalColour.a = Colour.a;
		}
		else
		if (name_hash == CzHashes::Layer_Hash)
		{
			Layer = (*it)->getValueAsInt();
			layer_set = true;
		}
		else
		if (name_hash == CzHashes::Orphan_Hash)
			Orphan = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Active_Hash)
			is_active = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Visible_Hash)
			IsVisible = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::IgnoreCamera_Hash)
			IgnoreCamera = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Docking_Hash)
		{
			docking_set = true;
			unsigned int hash = (*it)->getValue().getHash();
			if (hash == CzString::CalculateHash("top"))
				docking = CzScene::Dock_Top;
			else
			if (hash == CzString::CalculateHash("left"))
				docking = CzScene::Dock_Left;
			else
			if (hash == CzString::CalculateHash("right"))
				docking = CzScene::Dock_Right;
			else
			if (hash == CzString::CalculateHash("bottom"))
				docking = CzScene::Dock_Bottom;
			else
			if (hash == CzString::CalculateHash("topleft"))
				docking = CzScene::Dock_TopLeft;
			else
			if (hash == CzString::CalculateHash("topright"))
				docking = CzScene::Dock_TopRight;
			else
			if (hash == CzString::CalculateHash("bottomleft"))
				docking = CzScene::Dock_BottomLeft;
			else
			if (hash == CzString::CalculateHash("bottomright"))
				docking = CzScene::Dock_BottomRight;
			else
				docking = CzScene::Dock_None;
		}
		else
		if (name_hash == CzHashes::Margin_Hash)
		{
			if (!(*it)->getValueAsRect(Margin))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Invalid value for Actor::Margin, expected rect - ", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::Collidable_Hash)
			IsCollidable = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Draggable_Hash)
		{
			IsDraggable = (*it)->getValueAsBool();
			if (!hit_test_set)
				IsTappable = true;
		}
		else
		if (name_hash == CzHashes::Tappable_Hash)
			IsTappable = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::CollisionSize_Hash)
			setCollisionSize((*it)->getValueAsInt());
		else
		if (name_hash == CzHashes::CollisionRect_Hash)
		{
			CzIRect rect;
			if (!(*it)->getValueAsRect(rect))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Invalid value for Actor::CollisionRect, expected rect - ", DebugInfo.c_str());
			setCollisionRect(rect);
		}
		else
		if (name_hash == CzHashes::WrapPosition_Hash)
			WrapPosition = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Binding_Hash)
			simple_binding_name = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Bindings_Hash)
			bindings_name = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Depth_Hash)
		{
			Depth = (*it)->getValueAsFloat();
			depth_set = true;
		}
		else
		if (name_hash == CzHashes::HoldFocus_Hash)
			HoldFocus = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::OnTapped_Hash)
			EventsManager->addEvent("OnTapped", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnTapped2_Hash)
			EventsManager->addEvent("OnTapped2", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnTapped3_Hash)
			EventsManager->addEvent("OnTapped3", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnTapped4_Hash)
			EventsManager->addEvent("OnTapped4", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnTapped5_Hash)
			EventsManager->addEvent("OnTapped5", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnBeginTouch_Hash)
			EventsManager->addEvent("OnBeginTouch", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnBeginTouch2_Hash)
			EventsManager->addEvent("OnBeginTouch2", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnBeginTouch3_Hash)
			EventsManager->addEvent("OnBeginTouch3", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnBeginTouch4_Hash)
			EventsManager->addEvent("OnBeginTouch4", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnBeginTouch5_Hash)
			EventsManager->addEvent("OnBeginTouch5", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnEndTouch_Hash)
			EventsManager->addEvent("OnEndTouch", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnEndTouch2_Hash)
			EventsManager->addEvent("OnEndTouch2", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnEndTouch3_Hash)
			EventsManager->addEvent("OnEndTouch3", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnEndTouch4_Hash)
			EventsManager->addEvent("OnEndTouch4", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnEndTouch5_Hash)
			EventsManager->addEvent("OnEndTouch5", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnCreate_Hash)
			EventsManager->addEvent("OnCreate", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnDestroy_Hash)
			EventsManager->addEvent("OnDestroy", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnOrientationChange_Hash)
			EventsManager->addEvent("OnOrientationChange", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnTick_Hash)
		{
			EventsManager->addEvent("OnTick", (*it)->getValue().c_str());
			TickEnabled = true;
		}
		else
		if (name_hash == CzHashes::OnCollisionStart_Hash)
			EventsManager->addEvent("OnCollisionStart", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnCollisionEnd_Hash)
			EventsManager->addEvent("OnCollisionEnd", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::LinkedTo_Hash)
			linked_to = &(*it)->getValue();
		else
		if (name_hash == CzHashes::GridPos_Hash)
		{
			CzVec2 pos = CzVec2(0, 0);
			if (!(*it)->getValueAsPoint(pos))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Invalid value for Actor::GridPos, expected vec2 - ", DebugInfo.c_str());
			else
				GridPos = ((int)pos.x << 16) | (int)pos.y;
		}
		else
		if (name_hash == CzHashes::Target_Hash)
			target = &(*it)->getValue();
		else
		if (name_hash == CzHashes::UseParentOpacity_Hash)
			UseParentOpacity = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Bubbling_Hash)
			ReceiveEventFromChild = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Condition_Hash)
			condition = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Box2dMaterial_Hash)
			box2dmaterial_name = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Shape_Hash)
			shape_name = &(*it)->getValue();
		else
		if (name_hash == CzHashes::COM_Hash)
		{
			if (!(*it)->getValueAsPoint(com))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Invalid value for Actor::COM, expected vec2 - ", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::Sensor_Hash)
			sensor = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::CollisionFlags_Hash)
		{
			if (!(*it)->getValueAsPoint3(collision_flags))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Invalid value for Actor::CollisionFlags, expected vec3 - ", DebugInfo.c_str());
		}
	}

#if defined(_DEBUG)
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Actor Name - ", getName().c_str(), DebugInfo.c_str());
#endif

	// If layer not set and the actor is a child of another actor then set the childs layer to same as the parent
	if (!layer_set && parent->getClassTypeHash() == CzHashes::Actor_Hash)
		Layer = ((CzActor*)parent)->getLayer();

	if (condition != NULL)
	{
		// Find the condition variable
		bool condition_not = false;
		CzXomlVariable* var = NULL;
		if (*(condition->c_str()) == '!')
		{
			condition_not = true;
			CzString cond = condition->c_str() + 1;
			var = CzXomlVariable::GetVariable(cond, Scene);
		}
		else
			var = CzXomlVariable::GetVariable(*condition, Scene);
		if (var != NULL)
		{
			bool res = var->isTrue();
			if (condition_not)
				res = !res;
			if (!res)
			{
				Scene->removeActor(this, true);
				return -1;
			}
		}
#if defined (_DEBUG)
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Condition variable not found - ", condition->c_str(), DebugInfo.c_str());
#endif // _DEBUG
	}

	if (PercentagePosition)
		Position = PercPosToPos(OriginalPosition.x, OriginalPosition.y);

	// Set up LinkedTo actor
	if (linked_to != NULL && !linked_to->isEmpty())
	{
		CzActor* actor = Scene->findActor(linked_to->getHash());
		if (actor != NULL)
			setLinkedTo(actor);
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Linked actor not found - ", linked_to->c_str(), DebugInfo.c_str());
	}
	else
	if (parent->getClassTypeHash() == CzHashes::Actor_Hash)
		setLinkedTo((CzActor*)parent);	// Because we were defined inside another actor tag

	// If an actor is linked to another and no depth value has been set then it should be set to 0 as depth values for linked actors are accumulative
	if (LinkedTo != NULL && !depth_set)
		Depth = 0;

	// Search the scenes resources for the specified bindings manager
	if (bindings_name != NULL && !bindings_name->isEmpty())
	{
		CzXomlBindings* bindings = (CzXomlBindings*)Scene->getResourceManager()->findResource(bindings_name->c_str(), CzHashes::Bindings_Hash);
		if (bindings == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Actor - Could not find named bindings in the scene or in the global resources - ", bindings_name->c_str(), DebugInfo.c_str());
		setBindings(bindings);
	}

	// Set up simple binding
	if (simple_binding_name != NULL && !simple_binding_name->isEmpty())
	{
		CzString var_name, prop_name;
		CzString index_name;
		int index = -1;
		if (simple_binding_name->SplitPropVarIndex(prop_name, var_name, index, index_name))
		{
			CzXomlVariable* var = CzXomlVariable::GetVariable(var_name, Scene);
			if (var == NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Bound simple binding variable does not exist - ", var_name.c_str(), DebugInfo.c_str());
			else
			{
				SimpleBinding = new CzXomlBinding();
				SimpleBinding->Index = index;
#if defined(_DEBUG)
				SimpleBinding->_Property = prop_name;
#endif // _DEBUG
				SimpleBinding->Property = prop_name.getHash();
				SimpleBinding->Variable = var;
				if (!index_name.isEmpty())
					SimpleBinding->VIndex = index_name;
			}
		}
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Invalid format of simple binding, format should be [property]variable:array_index - ", simple_binding_name->c_str(), DebugInfo.c_str());
	}

	// Check for shape and set if present
	CzShape* shape = NULL;
	if (shape_name != NULL && !shape_name->isEmpty())
	{
		shape = (CzShape*)Scene->getResourceManager()->findResource(shape_name->c_str(), CzHashes::Shape_Hash);
		if (shape == NULL)
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Actor - Could not find named shape in the scene or in the global resources - ", shape_name->c_str(), DebugInfo.c_str());
		}
	}

	// Check for box2d material and set if present
	if (box2dmaterial_name != NULL && !box2dmaterial_name->isEmpty())
	{
		CzBox2dMaterial* material = (CzBox2dMaterial*)Scene->getResourceManager()->findResource(box2dmaterial_name->c_str(), CzHashes::Box2dMaterial_Hash);
		if (material == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Actor - Could not find named material in the scene or in the global resources - ", box2dmaterial_name->c_str(), DebugInfo.c_str());

//		if (shape == NULL)
//			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Cannot attach Box2d physics to an actor that has no shape - ", DebugInfo.c_str());
//		else
		{
			Angle = -Angle;
			Box2dBody = new CzBox2dBody();
			Box2dBody->InitBody(Scene->getBox2dWorld(), shape, material, &Position, Angle, com.x, com.y);
			if (!vd_set)
			{
				VelocityDamping.x = 0;
				VelocityDamping.y = 0;
			}
			if (!avd_set)
				AngularVelocityDamping = 0;
			setVelocityDamping(VelocityDamping.x, VelocityDamping.y);
			setAngularVelocityDamping(AngularVelocityDamping);
			Box2dBody->setAsSensor(sensor);
			Box2dBody->setCollisionFlags((int)collision_flags.x, (int)collision_flags.y, (int)collision_flags.z);
			Box2dBody->setUserData(this);
		}
	}
	setVelocity(vel.x, vel.y);
	setAngularVelocity(ang_vel);

	setActive(is_active);

	// Set docking mode
	if (docking_set)
		setDocking(docking);

	// Load children
	if (load_children)
	{
		// Process inner tags
		if (!CZ_XOML->Process(this, node))
			return 0;
	}

	// Find target
	if (target != NULL && !target->isEmpty())
	{
		Target = Scene->findActor(target->getHash());
		if (Target == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Could not find target actor - ", target->c_str(), DebugInfo.c_str());
	}

	return 1;
}

/**
 @fn	bool CzActor::PostLoadFromXoml(IzXomlResource* parent, CzXmlNode* node)

 @brief	Carries out any post XOML loading tasks

 PostLoadFromXoml adds the opportunity to carry out post load events, such as when the actor has finished loading all of its child nodes. Often timelines and actions list are specified 
 as child nodes of an actor. These will not be available in resource manager until the actors has finished loading, so the creation process cannot deal with them until after the actor has 
 been fully loaded. PostLoadFromXoml also fires the OnCreate event to notify XOML that the actor has been fully loaded.

 @param [in,out]	parent	If non-null, the parent.
 @param [in,out]	node  	If non-null, the node.

 @return	true if it succeeds, false if it fails.
 */

bool CzActor::PostLoadFromXoml(IzXomlResource* parent, CzXmlNode* node)
{
	CzString*	timeline_name = NULL;

	// We process timelines after the actor and its children have been loaded as they may be children of the actor
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Timeline_Hash)
			timeline_name = &(*it)->getValue();

	}

	// Search the timeline resources for the specified timeline
	if (timeline_name != NULL && !timeline_name->isEmpty())
	{
		CzAnimTimeline* timeline = CzAnimTimelinesManager::FindTimeline(timeline_name->getHash(), this);
		if (timeline == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Actor - Could not find named timeline in the scene or in the global resources - ", timeline_name->c_str(), DebugInfo.c_str());
		else
		{
			setTimeline(timeline);
			timeline->setTargetElement(this);
		}
	}

	// Force update any data bindings
	UpdateBindings(true);

	NotifyCreate();

	return true;
}

/**
 @fn	void CzActor::CalculateSizes(int& width, int& height)

 @brief	Calculates the size of the actor

 A utiliy method tha calculates the size of the actor based on the supplied size. If a negative value is supplied then it will be classed as a perctnage based size and will be calculated from the 
 size of the actors parent or screen size if the actor has no parent.

 @param [in,out]	width 	The width.
 @param [in,out]	height	The height.
 */

void CzActor::CalculateSizes(int& width, int& height)
{
	// Calculate size from parent or screen if size not specified
	if (LinkedTo != NULL)
	{
		// If negative size has been specified and part of a linked actor then base size on a percentage of parent
		if (width < 0)
			width = (LinkedTo->getSize().x * -width) / 100;
		if (height < 0)
			height = (LinkedTo->getSize().y * -height) / 100;;
	}
	else
	{
		// If negative size has been specified then base size on a percentage of virtual canvas
		if (width < 0)
			width = (int)((Scene->getScreenSize().x / Scene->getVirtualTransform().m[0][0] * -width) / 100);
		if (height < 0)
			height = (int)((Scene->getScreenSize().y / Scene->getVirtualTransform().m[1][1] * -height) / 100);
	}
}

/**
 @fn	void CzActor::CalculateMargins(CzIRect& margin)

 @brief	Calculates the size of the actors margins.

 A utiliy method tha calculates the size of the actors margins based on the supplied size. If a negative value is supplied then it will be classed as a perctnage based size and will be calculated from 
 the size of the actors parent or screen size if the actor has no parent.

 @param [in,out]	margin	The margin.
 */

void CzActor::CalculateMargins(CzIRect& margin)
{
	// Calculate size of margin from parent or screen if size not specified
	if (LinkedTo != NULL)
	{
		// If negative margin has been specified and part of a linked actor then base margin on a percentage of parent
		if (margin.x < 0)
			Margin.x = (LinkedTo->getSize().x * -margin.x) / 100;
		if (margin.y < 0)
			Margin.y = (LinkedTo->getSize().x * -margin.y) / 100;
		if (margin.w < 0)
			Margin.w = (LinkedTo->getSize().y * -margin.w) / 100;
		if (margin.h < 0)
			Margin.h = (LinkedTo->getSize().y * -margin.h) / 100;
	}
	else
	{
		// If negative nargin has been specified then base margin on a percentage of virtual canvas
		if (margin.x < 0)
			Margin.x = (int)((Scene->getScreenSize().x / Scene->getVirtualTransform().m[0][0] * -margin.x) / 100);
		if (margin.y < 0)
			Margin.y = (int)((Scene->getScreenSize().x / Scene->getVirtualTransform().m[0][0] * -margin.y) / 100);
		if (margin.w < 0)
			Margin.w = (int)((Scene->getScreenSize().y / Scene->getVirtualTransform().m[1][1] * -margin.w) / 100);
		if (margin.h < 0)
			Margin.h = (int)((Scene->getScreenSize().y / Scene->getVirtualTransform().m[1][1] * -margin.h) / 100);
	}
}

/**
 @fn	void CzActor::UpdateMargins()

 @brief	Updates the margins.

 During an OnOrientationChange event, this utility method is called to recalculate the size of the margins.
 */

void CzActor::UpdateMargins()
{
	if (LinkedTo != NULL)
	{
		if (OriginalMargin.x < 0)
			Margin.x = (LinkedTo->getSize().x * -OriginalMargin.x) / 100;
		if (OriginalMargin.y < 0)
			Margin.y = (LinkedTo->getSize().x * -OriginalMargin.y) / 100;
		if (OriginalMargin.w < 0)
			Margin.w = (LinkedTo->getSize().y * -OriginalMargin.w) / 100;
		if (OriginalMargin.h < 0)
			Margin.h = (LinkedTo->getSize().y * -OriginalMargin.h) / 100;
	}
	else
	{
		if (OriginalMargin.x < 0)
			Margin.x = (int)((Scene->getScreenSize().x / Scene->getVirtualTransform().m[0][0] * -OriginalMargin.x) / 100);
		if (OriginalMargin.y < 0)
			Margin.y = (int)((Scene->getScreenSize().x / Scene->getVirtualTransform().m[0][0] * -OriginalMargin.y) / 100);
		if (OriginalMargin.w < 0)
			Margin.w = (int)((Scene->getScreenSize().y / Scene->getVirtualTransform().m[1][1] * -OriginalMargin.w) / 100);
		if (OriginalMargin.h < 0)
			Margin.h = (int)((Scene->getScreenSize().y / Scene->getVirtualTransform().m[1][1] * -OriginalMargin.h) / 100);
	}
}




