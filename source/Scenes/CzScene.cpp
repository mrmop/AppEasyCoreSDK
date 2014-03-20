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

#include "IzPlatformDisplay.h"
#include "CzApp.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzCamera.h"
#include "CzString.h"
#include "CzActions.h"
#include "CzRender.h"
#include "CzScript.h"
#include "CzScriptLua.h"
#include "CzUIBase.h"
#include "CzRemoteReq.h"
#include "CzHashes.h"
#include "CzTimer.h"
#include "CzXomlResourceManager.h"

CzXomlClassDef* CzScene::ClassDef = NULL;

/**
 @fn	void CzScene::InitClass()

 @brief	Initialises the class.

 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzScene::InitClass()
{
	// Create class properties
	ClassDef = new CzXomlClassDef();
	ClassDef->addProperty(new CzXomlClassProperty("Position",				PT_Vec2,		CzScene::_setPosition,					CzScene::_getPosition));
	ClassDef->addProperty(new CzXomlClassProperty("X",						PT_Float,		CzScene::_setPositionX,					CzScene::_getPositionX));
	ClassDef->addProperty(new CzXomlClassProperty("Y",						PT_Float,		CzScene::_setPositionY,					CzScene::_getPositionY));
	ClassDef->addProperty(new CzXomlClassProperty("Angle",					PT_Float,		CzScene::_setAngle,						CzScene::_getAngle));
	ClassDef->addProperty(new CzXomlClassProperty("Scale",					PT_Float,		CzScene::_setScale,						CzScene::_getScale));
	ClassDef->addProperty(new CzXomlClassProperty("Colour",					PT_Vec4,		CzScene::_setColour,					CzScene::_getColour));
	ClassDef->addProperty(new CzXomlClassProperty("Opacity",				PT_Float,		CzScene::_setOpacity,					CzScene::_getOpacity));
	ClassDef->addProperty(new CzXomlClassProperty("Active",					PT_Bool,		CzScene::_setActive,					CzScene::_getActive));
	ClassDef->addProperty(new CzXomlClassProperty("Visible",				PT_Bool,		CzScene::_setVisible,					CzScene::_getVisible));
	ClassDef->addProperty(new CzXomlClassProperty("Layer",					PT_Int,			CzScene::_setLayer,						CzScene::_getLayer));
	ClassDef->addProperty(new CzXomlClassProperty("UserProperties",			PT_String,		NULL,									CzScene::_getUserProperties));
	ClassDef->addProperty(new CzXomlClassProperty("Name",					PT_String,		CzScene::_setName,						CzScene::_getName));
	ClassDef->addProperty(new CzXomlClassProperty("Type",					PT_Int,			CzScene::_setType,						CzScene::_getType));
	ClassDef->addProperty(new CzXomlClassProperty("CanvasSize",				PT_Vec2,		CzScene::_setCanvasSize,				CzScene::_getCanvasSize));
	ClassDef->addProperty(new CzXomlClassProperty("CanvasFit",				PT_Int,			CzScene::_setCanvasFit,					CzScene::_getCanvasFit));
	ClassDef->addProperty(new CzXomlClassProperty("CanvasOrigin",			PT_Vec2,		CzScene::_setCanvasOrigin,				CzScene::_getCanvasOrigin));
	ClassDef->addProperty(new CzXomlClassProperty("Extents",				PT_Vec4,		CzScene::_setExtents,					CzScene::_getExtents));
	ClassDef->addProperty(new CzXomlClassProperty("AllowSuspend",			PT_Bool,		CzScene::_setAllowSuspend,				CzScene::_getAllowSuspend));
	ClassDef->addProperty(new CzXomlClassProperty("Clipping",				PT_Vec4,		CzScene::_setClipping,					CzScene::_getClipping));
	ClassDef->addProperty(new CzXomlClassProperty("ClipStatic",				PT_Bool,		CzScene::_setClipStatic,				CzScene::_getClipStatic));
	ClassDef->addProperty(new CzXomlClassProperty("TargetX",				PT_Float,		CzScene::_setTargetX,					NULL));
	ClassDef->addProperty(new CzXomlClassProperty("TargetY",				PT_Float,		CzScene::_setTargetY,					NULL));
	ClassDef->addProperty(new CzXomlClassProperty("Layers",					PT_Int,			NULL,									CzScene::_getLayers));
	ClassDef->addProperty(new CzXomlClassProperty("Current",				PT_Bool,		CzScene::_setCurrent,					CzScene::_getCurrent));
	ClassDef->addProperty(new CzXomlClassProperty("Timeline",				PT_Resource,	CzScene::_setTimeline,					CzScene::_getTimeline));
	ClassDef->addProperty(new CzXomlClassProperty("Bindings",				PT_Resource,	CzScene::_setBindings,					CzScene::_getBindings));
	ClassDef->addProperty(new CzXomlClassProperty("TimeScale",				PT_Float,		CzScene::_setTimeScale,					CzScene::_getTimeScale));
	ClassDef->addProperty(new CzXomlClassProperty("Camera",					PT_Resource,	CzScene::_setCamera,					CzScene::_getCamera));
	ClassDef->addProperty(new CzXomlClassProperty("Batch",					PT_Bool,		CzScene::_setBatch,						CzScene::_getBatch));
	ClassDef->addProperty(new CzXomlClassProperty("AllowFocus",				PT_Bool,		CzScene::_setAllowFocus,				CzScene::_getAllowFocus));
	ClassDef->addProperty(new CzXomlClassProperty("OnTick",					PT_String,		CzScene::_setOnTick,					NULL));
	ClassDef->addProperty(new CzXomlClassProperty("OnPan",					PT_String,		CzScene::_setOnPan,						NULL));
	ClassDef->addProperty(new CzXomlClassProperty("OnKeyBack",				PT_String,		CzScene::_setOnKeyBack,					NULL));
	ClassDef->addProperty(new CzXomlClassProperty("OnKeyMenu",				PT_String,		CzScene::_setOnKeyMenu,					NULL));
	ClassDef->addProperty(new CzXomlClassProperty("OnOrientationChange",	PT_String,		CzScene::_setOnOrientationChange,		NULL));
	ClassDef->addProperty(new CzXomlClassProperty("OnSuspend",				PT_String,		CzScene::_setOnSuspend,					NULL));
	ClassDef->addProperty(new CzXomlClassProperty("OnResume",				PT_String,		CzScene::_setOnResume,					NULL));
	ClassDef->addProperty(new CzXomlClassProperty("OnLostFocus",			PT_String,		CzScene::_setOnLostFocus,				NULL));
	ClassDef->addProperty(new CzXomlClassProperty("OnGainedFocus",			PT_String,		CzScene::_setOnGainedFocus,				NULL));
	ClassDef->addProperty(new CzXomlClassProperty("OnCreate",				PT_String,		CzScene::_setOnCreate,					NULL));
	ClassDef->addProperty(new CzXomlClassProperty("OnDestroy",				PT_String,		CzScene::_setOnDestroy,					NULL));
	ClassDef->addProperty(new CzXomlClassProperty("OnDeviceSuspended",		PT_String,		CzScene::_setOnDeviceSuspended,			NULL));
	ClassDef->addProperty(new CzXomlClassProperty("OnDeviceResumed",		PT_String,		CzScene::_setOnDeviceResumed,			NULL));
	ClassDef->addProperty(new CzXomlClassProperty("OnDeviceVolumeUp",		PT_String,		CzScene::_setOnDeviceVolumeUp,			NULL));
	ClassDef->addProperty(new CzXomlClassProperty("OnDeviceVolumeDown",		PT_String,		CzScene::_setOnDeviceVolumeDown,		NULL));
	ClassDef->addProperty(new CzXomlClassProperty("OnDeviceNetworkOnline",	PT_String,		CzScene::_setOnDeviceNetworkOnline,		NULL));
	ClassDef->addProperty(new CzXomlClassProperty("OnDeviceNetworkOffline",	PT_String,		CzScene::_setOnDeviceNetworkOffline,	NULL));
	ClassDef->addProperty(new CzXomlClassProperty("ScriptEngine",			PT_String,		CzScene::_setScriptEngine,				CzScene::_getScriptEngine));
	ClassDef->addProperty(new CzXomlClassProperty("WorldScale",				PT_Vec2,		CzScene::_setWorldScale,				CzScene::_getWorldScale));
	ClassDef->addProperty(new CzXomlClassProperty("Physics",				PT_Bool,		CzScene::_setPhysics,					CzScene::_getPhysics));
	ClassDef->addProperty(new CzXomlClassProperty("Gravity",				PT_Vec2,		CzScene::_setGravity,					CzScene::_getGravity));
	ClassDef->addProperty(new CzXomlClassProperty("PhysicsTimeStep",		PT_Float,		CzScene::_setPhysicsTimeStep,			CzScene::_getPhysicsTimeStep));
	ClassDef->addProperty(new CzXomlClassProperty("PositionX",				PT_Float,		CzScene::_setPositionX,					CzScene::_getPositionX));
	ClassDef->addProperty(new CzXomlClassProperty("PositionY",				PT_Float,		CzScene::_setPositionY,					CzScene::_getPositionY));
}
void CzScene::ReleaseClass()
{
	if (ClassDef != NULL)
	{
		ClassDef->Release();
		delete ClassDef;
		ClassDef = NULL;
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
bool CzScene::_setName(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	if (add)
	{
		CzString name = ((CzScene*)target)->getName();
		name += (const char*)prop.p_data;
		((CzScene*)target)->setName(name.c_str());
	}
	else
		((CzScene*)target)->setName((const char*)prop.p_data);

	return true;
}

CzXomlProperty CzScene::_getName(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->getName().c_str());
}


bool CzScene::_setType(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	if (add)
		((CzScene*)target)->setType(((CzScene*)target)->getType() + prop.p_int);
	else
		((CzScene*)target)->setType(prop.p_int);

	return true;
}

CzXomlProperty CzScene::_getType(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->getType());
}

bool CzScene::_setLayer(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	if (add)
		((CzScene*)target)->setLayer(((CzScene*)target)->getLayer() + prop.p_int);
	else
		((CzScene*)target)->setLayer(prop.p_int);

	return true;
}

CzXomlProperty CzScene::_getLayer(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->getLayer());
}

bool CzScene::_setPosition(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	CzCamera* cam = scene->getCamera();
	if (cam == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - setProperty - Cannot set position as no camera is attached to the scene", scene->getDebugInfo().c_str());
#endif
		return false;
	}

	if (add)
	{
		CzVec2 pos = cam->getPosition();
		pos.x += prop.p_vec[0];
		pos.y += prop.p_vec[1];
		cam->setPosition(pos.x, pos.y);
	}
	else
		cam->setPosition(prop.p_vec[0], prop.p_vec[1]);

	return true;
}

CzXomlProperty CzScene::_getPosition(IzXomlResource* target)
{
	CzScene* scene = (CzScene*)target;
	CzCamera* cam = scene->getCamera();
	if (cam == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - getProperty - Cannot get position as no camera is attached to the scene", scene->getDebugInfo().c_str());
#endif
		return CzXomlProperty();
	}

	return CzXomlProperty(cam->getPosition());
}

bool CzScene::_setPositionX(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	CzCamera* cam = scene->getCamera();
	if (cam == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - setProperty - Cannot set X position as no camera is attached to the scene", scene->getDebugInfo().c_str());
#endif
		return false;
	}

	CzVec2 pos = cam->getPosition();
	if (add)
		cam->setPosition(pos.x + prop.p_float, pos.y);
	else
		cam->setPosition(prop.p_float, pos.y);

	return true;
}

CzXomlProperty CzScene::_getPositionX(IzXomlResource* target)
{
	CzScene* scene = (CzScene*)target;
	CzCamera* cam = scene->getCamera();
	if (cam == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - getProperty - Cannot get X position as no camera is attached to the scene", scene->getDebugInfo().c_str());
#endif
		return CzXomlProperty();
	}

	return CzXomlProperty(cam->getPosition().x);
}

bool CzScene::_setPositionY(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	CzCamera* cam = scene->getCamera();
	if (cam == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - setProperty - Cannot set Y position as no camera is attached to the scene", scene->getDebugInfo().c_str());
#endif
		return false;
	}

	CzVec2 pos = cam->getPosition();
	if (add)
		cam->setPosition(pos.x, pos.y + prop.p_float);
	else
		cam->setPosition(pos.x, prop.p_float);

	return true;
}

CzXomlProperty CzScene::_getPositionY(IzXomlResource* target)
{
	CzScene* scene = (CzScene*)target;
	CzCamera* cam = scene->getCamera();
	if (cam == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - getProperty - Cannot get Y position as no camera is attached to the scene", scene->getDebugInfo().c_str());
#endif
		return CzXomlProperty();
	}

	return CzXomlProperty(cam->getPosition().y);
}

bool CzScene::_setAngle(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	CzCamera* cam = scene->getCamera();
	if (cam == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - setProperty - Cannot set angle as no camera is attached to the scene", scene->getDebugInfo().c_str());
#endif
		return false;
	}

	if (add)
		cam->setAngle(cam->getAngle() + prop.p_float);
	else
		cam->setAngle(prop.p_float);

	return true;
}

CzXomlProperty CzScene::_getAngle(IzXomlResource* target)
{
	CzScene* scene = (CzScene*)target;
	CzCamera* cam = scene->getCamera();
	if (cam == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - getProperty - Cannot get angle as no camera is attached to the scene", scene->getDebugInfo().c_str());
#endif
		return CzXomlProperty();
	}

	return CzXomlProperty(cam->getAngle());
}

bool CzScene::_setScale(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	CzCamera* cam = scene->getCamera();
	if (cam == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - setProperty - Cannot set scale as no camera is attached to the scene", scene->getDebugInfo().c_str());
#endif
		return false;
	}

	if (add)
		cam->setScale(cam->getScale() + prop.p_float);
	else
		cam->setScale(prop.p_float);

	return true;
}

CzXomlProperty CzScene::_getScale(IzXomlResource* target)
{
	CzScene* scene = (CzScene*)target;
	CzCamera* cam = scene->getCamera();
	if (cam == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - getProperty - Cannot get scale as no camera is attached to the scene", scene->getDebugInfo().c_str());
#endif
		return CzXomlProperty();
	}

	return CzXomlProperty(cam->getScale());
}

bool CzScene::_setColour(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	if (add)
	{
		CzColour c = scene->getColour();
		scene->setColour(c.r + (uint8)prop.p_vec[0], c.g + (uint8)prop.p_vec[1], c.b + (uint8)prop.p_vec[2], c.a + (uint8)prop.p_vec[3]);
	}
	else
		scene->setColour((uint8)prop.p_vec[0], (uint8)prop.p_vec[1], (uint8)prop.p_vec[2], (uint8)prop.p_vec[3]);

	return true;
}

CzXomlProperty CzScene::_getColour(IzXomlResource* target)
{
	CzScene* scene = (CzScene*)target;
	return CzXomlProperty(scene->getColour());
}

bool CzScene::_setTargetX(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	CzCamera* cam = scene->getCamera();
	if (cam == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - setProperty - Cannot set TargetX as no camera is attached to the scene", scene->getDebugInfo().c_str());
#endif
		return false;
	}

	cam->setTargetX((const char*)prop.p_data);

	return true;
}

bool CzScene::_setTargetY(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	CzCamera* cam = scene->getCamera();
	if (cam == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - setProperty - Cannot set TargetY as no camera is attached to the scene", scene->getDebugInfo().c_str());
#endif
		return false;
	}

	cam->setTargetY((const char*)prop.p_data);

	return true;
}

bool CzScene::_setCanvasSize(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	scene->setVirtualTransform((int)prop.p_vec[0], (int)prop.p_vec[1], 0, scene->getCanvasFit(), scene->getCanvasOrigin());

	return true;
}

CzXomlProperty CzScene::_getCanvasSize(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->getVirtualSize());
}
//TODO: Canvas fit and origin need to be strings not ints
bool CzScene::_setCanvasFit(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	CzIVec2 original_size = scene->getOriginalSize();
	if (original_size.x != 0)
		scene->setVirtualTransform(original_size.x, original_size.y, 0, (eCanvasFit)prop.p_int, scene->getCanvasOrigin());
	else
		scene->setCanvasFit((eCanvasFit)prop.p_int);

	return true;
}

CzXomlProperty CzScene::_getCanvasFit(IzXomlResource* target)
{
	return CzXomlProperty((int)((CzScene*)target)->getCanvasFit());
}

bool CzScene::_setCanvasOrigin(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	CzIVec2 original_size = scene->getOriginalSize();
	if (original_size.x != 0)
		scene->setVirtualTransform(original_size.x, original_size.y, 0, scene->getCanvasFit(), (eCanvasOrigin)prop.p_int);
	else
		scene->setCanvasOrigin((eCanvasOrigin)prop.p_int);

	return true;
}

CzXomlProperty CzScene::_getCanvasOrigin(IzXomlResource* target)
{
	return CzXomlProperty((int)((CzScene*)target)->getCanvasOrigin());
}

bool CzScene::_setExtents(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	if (add)
	{
		CzIRect extents = scene->getExtents();
		scene->setExtents(extents.x + (int)prop.p_vec[0], extents.y + (int)prop.p_vec[1], extents.w + (int)prop.p_vec[2], extents.h + (int)prop.p_vec[3]);
	}
	else
		scene->setExtents((int)prop.p_vec[0], (int)prop.p_vec[1], (int)prop.p_vec[2], (int)prop.p_vec[3]);

	return true;
}

CzXomlProperty CzScene::_getExtents(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->getExtents());
}

bool CzScene::_setAllowSuspend(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	if (add)
		((CzScene*)target)->setAllowSuspend(!((CzScene*)target)->getAllowSuspend());
	else
		((CzScene*)target)->setAllowSuspend(prop.p_bool);

	return true;
}

CzXomlProperty CzScene::_getAllowSuspend(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->getAllowSuspend());
}

bool CzScene::_setClipping(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	if (add)
	{
		CzIRect clipping_area = scene->getClippingArea();
		scene->setClippingArea(clipping_area.x + (int)prop.p_vec[0], clipping_area.y + (int)prop.p_vec[1], clipping_area.w + (int)prop.p_vec[2], clipping_area.h + (int)prop.p_vec[3]);
	}
	else
		scene->setClippingArea((int)prop.p_vec[0], (int)prop.p_vec[1], (int)prop.p_vec[2], (int)prop.p_vec[3]);

	return true;
}

CzXomlProperty CzScene::_getClipping(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->getClippingArea());
}

bool CzScene::_setClipStatic(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	if (add)
		((CzScene*)target)->setClipStatic(!((CzScene*)target)->isClipStatic());
	else
		((CzScene*)target)->setClipStatic(prop.p_bool);

	return true;
}

CzXomlProperty CzScene::_getClipStatic(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->isClipStatic());
}

bool CzScene::_setActive(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	if (add)
		((CzScene*)target)->setActive(!((CzScene*)target)->isActive());
	else
		((CzScene*)target)->setActive(prop.p_bool);

	return true;
}

CzXomlProperty CzScene::_getActive(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->isActive());
}

bool CzScene::_setVisible(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	if (add)
		((CzScene*)target)->setVisible(!((CzScene*)target)->isVisible());
	else
		((CzScene*)target)->setVisible(prop.p_bool);

	return true;
}

CzXomlProperty CzScene::_getVisible(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->isVisible());
}

CzXomlProperty CzScene::_getLayers(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->getMaxLayers());
}

bool CzScene::_setCurrent(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	if (prop.p_bool)
		((CzApp*)((CzScene*)target)->getParent())->changeScene((CzScene*)target);

	return true;
}

CzXomlProperty CzScene::_getCurrent(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->getParent()->getCurrentScene() == target);
}

bool CzScene::_setTimeline(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	if (prop.p_data == NULL)
	{
		scene->setTimeline(NULL);
		return true;
	}
	CzAnimTimeline* timeline;
	if (prop.Type == PT_Resource)
		timeline = (CzAnimTimeline*)prop.p_data;
	else
		timeline = (CzAnimTimeline*)CzAnimTimelinesManager::FindTimeline(CzString::CalculateHash((const char*)prop.p_data), scene);

	if (timeline != NULL)
	{
		if (timeline != scene->getTimeline())
		{
			scene->setTimeline(timeline);
			timeline->setTargetElement(scene);
		}
	}
#if defined(_DEBUG)
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - setProperty - Could not find named timeline in the scene or in the global resources - ", (const char*)prop.p_data, scene->getDebugInfo().c_str());
		return false;
	}
#endif

	return true;
}

CzXomlProperty CzScene::_getTimeline(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->getTimeline());
}

bool CzScene::_setBindings(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	if (prop.p_data == NULL)
	{
		scene->setBindings(NULL);
		return true;
	}
	CzXomlBindings* bindings; 
	if (prop.Type == PT_Resource)
		bindings = (CzXomlBindings*)prop.p_data;
	else
		bindings = (CzXomlBindings*)scene->getResourceManager()->findResource((const char*)prop.p_data, CzHashes::Bindings_Hash);

	if (bindings == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - setProperty - Could not find named bindings in the scene or in the global resources - ", (const char*)prop.p_data, scene->getDebugInfo().c_str());
		return false;
	}
	else
		scene->setBindings(bindings);

	return true;
}

CzXomlProperty CzScene::_getBindings(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->getBindings());
}

bool CzScene::_setOpacity(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	if (add)
	{
		int o = ((CzScene*)target)->getOpacity() + (int)prop.p_float;
		if (o < 0) o = 0;
		if (o > 255) o = 255;
		((CzScene*)target)->setOpacity(o);
	}
	else
		((CzScene*)target)->setOpacity((int)prop.p_float);

	return true;
}

CzXomlProperty CzScene::_getOpacity(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->getOpacity());
}

bool CzScene::_setTimeScale(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	CzAnimTimeline* timeline = scene->getTimeline();
	if (timeline != NULL)
	{
		if (add)
			timeline->setTimeScale(timeline->getTimeScale() + prop.p_float);
		else
			timeline->setTimeScale(prop.p_float);
	}

	return true;
}

CzXomlProperty CzScene::_getTimeScale(IzXomlResource* target)
{
	CzScene* scene = (CzScene*)target;
	CzAnimTimeline* timeline = scene->getTimeline();
	if (timeline != NULL)
		return CzXomlProperty(timeline->getTimeScale());

	return CzXomlProperty();
}

bool CzScene::_setCamera(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	CzCamera* camera;
	if (prop.Type == PT_Resource)
		camera = (CzCamera*)prop.p_data;
	else
		camera = (CzCamera*)scene->getResourceManager()->findResource((const char*)prop.p_data, CzHashes::Camera_Hash);
	if (camera != NULL)
		((CzScene*)target)->setCamera(camera);
#if defined(_DEBUG)
	else
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - setProperty - Could not find named camera in the scene or in the global resources - ", (const char*)prop.p_data, scene->getDebugInfo().c_str());
#endif
	
	return true;
}

CzXomlProperty CzScene::_getCamera(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->getCamera());
}

bool CzScene::_setBatch(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	if (scene->getSpriteManager() != NULL)
	{
		if (add)
			scene->getSpriteManager()->setBatching(!scene->getSpriteManager()->getBatching());
		else
			scene->getSpriteManager()->setBatching(prop.p_bool);
	}
	return true;
}

CzXomlProperty CzScene::_getBatch(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->isBatching());
}

bool CzScene::_setAllowFocus(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	if (add)
		((CzScene*)target)->setAllowFocus(!((CzScene*)target)->getAllowFocus());
	else
		((CzScene*)target)->setAllowFocus(prop.p_bool);

	return true;
}

CzXomlProperty CzScene::_getAllowFocus(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->getAllowFocus());
}

bool CzScene::_setOnSuspend(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	scene->getEventsManager()->addEvent("OnSuspend", (const char*)prop.p_data, true);
	return true;
}

bool CzScene::_setOnResume(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	scene->getEventsManager()->addEvent("OnResume", (const char*)prop.p_data, true);
	return true;
}

bool CzScene::_setOnLostFocus(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	scene->getEventsManager()->addEvent("OnLostFocus", (const char*)prop.p_data, true);
	return true;
}

bool CzScene::_setOnGainedFocus(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	scene->getEventsManager()->addEvent("OnGainedFocus", (const char*)prop.p_data, true);
	return true;
}

bool CzScene::_setOnCreate(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	scene->getEventsManager()->addEvent("OnCreate", (const char*)prop.p_data, true);
	return true;
}

bool CzScene::_setOnDestroy(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	scene->getEventsManager()->addEvent("OnDestroy", (const char*)prop.p_data, true);
	return true;
}

bool CzScene::_setOnKeyBack(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	scene->getEventsManager()->addEvent("OnKeyBack", (const char*)prop.p_data, true);
	return true;
}

bool CzScene::_setOnKeyMenu(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	scene->getEventsManager()->addEvent("OnKeyMenu", (const char*)prop.p_data, true);
	return true;
}

bool CzScene::_setOnOrientationChange(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	scene->getEventsManager()->addEvent("OnOrientationChange", (const char*)prop.p_data, true);
	return true;
}

bool CzScene::_setOnTick(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	scene->getEventsManager()->addEvent("OnTick", (const char*)prop.p_data, true);
	return true;
}

bool CzScene::_setOnPan(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	scene->getEventsManager()->addEvent("OnPan", (const char*)prop.p_data, true);
	return true;
}

bool CzScene::_setOnDeviceSuspended(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	scene->getEventsManager()->addEvent("OnDeviceSuspended", (const char*)prop.p_data, true);
	return true;
}

bool CzScene::_setOnDeviceResumed(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	scene->getEventsManager()->addEvent("OnDeviceResumed", (const char*)prop.p_data, true);
	return true;
}

bool CzScene::_setOnDeviceVolumeUp(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	scene->getEventsManager()->addEvent("OnDeviceVolumeUp", (const char*)prop.p_data, true);
	return true;
}

bool CzScene::_setOnDeviceVolumeDown(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	scene->getEventsManager()->addEvent("OnDeviceVolumeDown", (const char*)prop.p_data, true);
	return true;
}

bool CzScene::_setOnDeviceNetworkOnline(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	scene->getEventsManager()->addEvent("OnDeviceNetworkOnline", (const char*)prop.p_data, true);
	return true;
}

bool CzScene::_setOnDeviceNetworkOffline(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	scene->getEventsManager()->addEvent("OnDeviceNetworkOffline", (const char*)prop.p_data, true);
	return true;
}

bool CzScene::_setScriptEngine(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzString se = (const char*)prop.p_data;
	((CzScene*)target)->setScriptEngine(se);
	return true;
}

CzXomlProperty CzScene::_getScriptEngine(IzXomlResource* target)
{
	return CzXomlProperty((void*)((CzScene*)target)->getScriptEngine()->getType().c_str());
}

bool CzScene::_setWorldScale(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	if (!scene->isPhysicsEnabled())
		return false;

	if (add)
	{
		CzVec2 s = scene->getBox2dWorld()->getWorldScale();
		scene->getBox2dWorld()->setWorldScale(s.x + prop.p_vec[0], s.y + prop.p_vec[1]);
	}
	else
		scene->getBox2dWorld()->setWorldScale(prop.p_vec[0], prop.p_vec[1]);

	return true;
}

CzXomlProperty CzScene::_getWorldScale(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->getBox2dWorld()->getWorldScale());
}

bool CzScene::_setPhysics(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;

	if (add)
		scene->setPhysicsEnabled(!scene->isPhysicsEnabled());
	else
		scene->setPhysicsEnabled(prop.p_bool);

	return true;
}

CzXomlProperty CzScene::_getPhysics(IzXomlResource* target)
{
	return CzXomlProperty(((CzScene*)target)->isPhysicsEnabled());
}

bool CzScene::_setGravity(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	if (!scene->isPhysicsEnabled())
		return false;

	if (add)
	{
		CzVec2 s = scene->getBox2dWorld()->getWorldScale();
		scene->getBox2dWorld()->setWorldScale(s.x + prop.p_vec[0], s.y + prop.p_vec[1]);
	}
	else
		scene->getBox2dWorld()->setWorldScale(prop.p_vec[0], prop.p_vec[1]);

	return true;
}

CzXomlProperty CzScene::_getGravity(IzXomlResource* target)
{
	CzScene* scene = (CzScene*)target;
	if (!scene->isPhysicsEnabled())
		return CzXomlProperty();

	return CzXomlProperty(((CzScene*)target)->getBox2dWorld()->getGravity());
}

bool CzScene::_setPhysicsTimeStep(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzScene* scene = (CzScene*)target;
	if (!scene->isPhysicsEnabled())
		return false;

	if (add)
		scene->setConstantTimeStep(scene->getConstantTimeStep() + prop.p_float);
	else
		scene->setConstantTimeStep(prop.p_float);

	return true;
}

CzXomlProperty CzScene::_getPhysicsTimeStep(IzXomlResource* target)
{
	CzScene* scene = (CzScene*)target;
	if (!scene->isPhysicsEnabled())
		return CzXomlProperty();

	return CzXomlProperty(((CzScene*)target)->getConstantTimeStep());
}

CzXomlProperty CzScene::_getUserProperties(IzXomlResource* target)
{
	CzScene* scene = (CzScene*)target;
	CzUserPropertyList* props = scene->getUserPropertyList();
	return CzXomlProperty(props);
}


/**
 @fn	CzScene::~CzScene()

 @brief	CzScene destructor.

 Cleans up the scene and all of iots contents. 

 */

CzScene::~CzScene()
{
	NotifyDestroy();
	SAFE_DELETE(ScriptEngine)
	Destroyed = true;

	SAFE_DELETE(UserPropertyList);

	// Only delete an unmanaged timeline
	if (Timeline != NULL)
	{
		if (!Timeline->isManaged())
		{
			SAFE_DELETE(Timeline)
		}
	}

	if (Camera != NULL)
	{
		if (!Camera->isManaged())
		{
			SAFE_DELETE(Camera)
		}
	}
	SAFE_DELETE(EventsManager)
	SAFE_DELETE(Modifiers)
	SAFE_DELETE(Collidables)
	clearActors(true);

	SAFE_DELETE(Box2dWorld)
	SAFE_DELETE(SpriteManager)
	SAFE_DELETE(VariableManager)
	SAFE_DELETE(ActionsManager)
	SAFE_DELETE(TimersManager)
	SAFE_DELETE(TimelinesManager)
	SAFE_DELETE(ProgramManager)
	SAFE_DELETE(ResourceManager)

}

/**
 @fn	void CzScene::setVirtualTransform(int required_width, int required_height, float angle,
	eCanvasFit fit, eCanvasOrigin canvas_origin)

 @brief	Sets the scenes virtual transform.

 Targeting a large selection of different phones, tablets and other devices with a variety of screen sizes and aspect ratios is a bit of a nightmare when it comes to game development. 
 Luckily the CzScene class takes care of this for us. A scene is quite a clever object in that it can render itself to any sized / configuration display using the virtual canvas concept. 
 A virtual canvas is basically our own ideal screen size that we want to render to. The scene will scale and translate int visuals to fit our canvas onto the devices display allowing us 
 to get on with developing our app or game using a static resolution. Lets take a look at the prototype for setting a scenes virtual canvas:

 @param	required_width 	Width of the required canvas.
 @param	required_height	Height of the required canvas.
 @param	angle		   	The angle of the canvas in degrees.
 @param	fit			   	Defines how canvas fits to the screen.
 @param	canvas_origin  	The canvas origin.
 */

void CzScene::setVirtualTransform(int required_width, int required_height, float angle, eCanvasFit fit, eCanvasOrigin canvas_origin)
{
	float scale_x = 1.0f, scale_y = 1.0f;
	int screen_width = PLATFORM_DISPLAY->getCurrentWidth();
	int screen_height = PLATFORM_DISPLAY->getCurrentHeight();

	if (screen_width > screen_height)
		Orientation = Orientation_Landscape;
	else
		Orientation = Orientation_Portrait;

	OriginalSize.x = required_width;
	OriginalSize.y = required_height;
	CanvasFit = fit;
	CanvasOrigin = canvas_origin;

	// If negative size has been specified then base size on a percentage of screen
	if (OriginalSize.x < 0)
		required_width = (screen_width * -OriginalSize.x) / 100;
	if (OriginalSize.y < 0)
		required_height = (screen_height * -OriginalSize.y) / 100;
	
	ScreenSize.x = screen_width;
	ScreenSize.y = screen_height;
	VirtualSize.x = required_width;
	VirtualSize.y = required_height;
	if (fit == Fit_None)
	{
		scale_x = scale_y = 1.0f;
	}
	else
	if (fit == Fit_Both)
	{
		// Simply scale to fit native screen resolution
		scale_x = (float)screen_width / required_width;
		scale_y = (float)screen_height / required_height;
	}
	else
	{
		// A fixed aspect ensures that the aspect ratio of the scene fits the native screen aspect ratio
		if (fit == Fit_Width)
			scale_x = scale_y = (float)screen_width / required_width;
		else
		if (fit == Fit_Height)
			scale_x = scale_y = (float)screen_height / required_height;
		else
		if (fit == Fit_Best)
		{
			float sx = (float)screen_width / required_width;
			float sy = (float)screen_height / required_height;

			if (sx < sy )
				scale_x = scale_y = sx;
			else
				scale_x = scale_y = sy;
		}
	}

	// Build rotation / scale tranform
	CzMatrix3 scale;
	scale.Scale(scale_x, scale_y);
	VirtualTransform.Rotate(angle);
	VirtualTransform.MultiplyPost(&scale);

	switch (canvas_origin)
	{
	case Origin_Centre:
		{
			// Position scene in middle of screen so that origin is screens centre
			VirtualTransform.TranslateSet((float)(screen_width / 2), (float)(screen_height / 2));
		}
		break;
	case Origin_TopLeft:
		{
			VirtualTransform.TranslateSet(0, 0);
		}
		break;
	case Origin_Top:
		{
			VirtualTransform.TranslateSet((float)(screen_width / 2), 0);
		}
		break;
	case Origin_Left:
		{
			VirtualTransform.TranslateSet(0, (float)(screen_height / 2));
		}
		break;
	}

	// If no camera attached then create and attach one
	if (Camera == NULL)
		setCamera(new CzCamera());

	Camera->setTransformDirty(true);
	Camera->Update(0);
	Transform = Camera->getTransform();
	Transform.MultiplyPost(&VirtualTransform);

	// Set sprite manager screen centre and centre of projection
	if (SpriteManager != NULL)
	{
		// Transfer scene transform to sprite manager
		SpriteManager->setTransformNoCamera(VirtualTransform);
		SpriteManager->setTransform(Transform);

		SpriteManager->setScreenCentre((float)(screen_width / 2), (float)(screen_height / 2));
		SpriteManager->setCOP((float)(screen_width / 2), (float)(screen_height / 2));
	}
}

/**
 @fn	void CzScene::setVirtualTransform(int required_width, int required_height, float angle,
	bool fix_aspect, bool lock_width, eCanvasOrigin canvas_origin)

 @brief	Sets virtual transform.

 Calls through to setVirtualTransform(int required_width, int required_height, float angle, eCanvasFit fit, eCanvasOrigin canvas_origin)

 @param	required_width 	Width of the required canvas.
 @param	required_height	Height of the required canvas.
 @param	angle		   	The angle of the canvas in degrees.
 @param	fix_aspect	   	true to fix aspect ratio.
 @param	lock_width	   	true to lock scaling to screen width.
 @param	canvas_origin  	The canvas origin.
 */

void CzScene::setVirtualTransform(int required_width, int required_height, float angle, bool fix_aspect, bool lock_width, eCanvasOrigin canvas_origin)
{
	if (!fix_aspect)
		setVirtualTransform(required_width, required_height, angle, Fit_Both, canvas_origin);
	else
	{
		if (lock_width)
			setVirtualTransform(required_width, required_height, angle, Fit_Width, canvas_origin);
		else
			setVirtualTransform(required_width, required_height, angle, Fit_Height, canvas_origin);
	}
}

/**
 @fn	int CzScene::Init(int max_collidables, int max_layers, bool doSleep)

 @brief	Initialises this scene.

 @param	max_collidables	The maximum number of collidables.
 @param	max_layers	   	The maximum number of layers.
 @param	doSleep		   	true to allow physics object sleeping.

 @return	1 for success.
 */

int	 CzScene::Init(int max_collidables, int max_layers, bool doSleep)
{
	// Allocate collidables list
	Timeline = NULL;
	Collidables = new CzActor*[max_collidables];
	MaxCollidables = max_collidables;
	MaxLayers = max_layers;

	// Allocate managers
	SpriteManager = new CzSpriteManager();
	SpriteManager->Init(max_layers);
	ResourceManager = new CzXomlResourceManager();
	ResourceManager->setParent(this);
	VariableManager = new CzXomlVariableManager();
	VariableManager->setParent(this);
	ActionsManager = new CzActionsManager();
	ActionsManager->setParent(this);
	TimelinesManager = new CzAnimTimelinesManager();
	TimelinesManager->setParent(this);
	TimersManager = new CzTimersManager();
	TimersManager->setParent(this);
	ProgramManager = new CzProgramManager();
	ProgramManager->setParent(this);

	Colour.r = 0xff;
	Colour.g = 0xff;
	Colour.b = 0xff;
	Colour.a = 0xff;
	PrevTouchVel.x = 0;
	PrevTouchVel.y = 0;

	if (PhysicsEnabled)
	{
		Box2dWorld = new CzBox2dWorld();
		Box2dWorld->InitWorld(doSleep);
	}

	return 1;
}

/**
 @fn	void CzScene::setScriptEngine(CzString& type)

 @brief	Sets the scenes script engine type.

 When set creates and assigns a script engine too the scene. Currently supported script engine types include:
 - Lua

 @param [in]	type	Script engine type.
 */

void CzScene::setScriptEngine(const CzString& type)
{
	if (ScriptEngine != NULL)
		delete ScriptEngine;
	
	unsigned int engine_hash = type.getHash();
	if (engine_hash == CZ_HASH("lua"))
	{
		CzScriptEngineLua* engine = new CzScriptEngineLua();
		engine->Init(this);
		ScriptEngine = engine;
	}
}

/**
 @fn	void CzScene::addActor(CzActor *actor)

 @brief	Adds an actor to the scene.

 @param [in,out]	actor	If non-null, the actor.
 */

void CzScene::addActor(CzActor *actor)
{
	// Add an actor to the scene
	Actors.push_back(actor);
	actor->setScene(this);
}

/**
 @fn	bool CzScene::findActorInRemovals(CzActor* actor)

 @brief	Searches for the actor in the actor removals list.

 When an actor is removed during scene processing, it is added to a removals list which gets cleaned up at the end of scene. This method will search the rmovals list to check if the 
 actor is already in the removals list.

 @param [in]	actor	If non-null, the actor.

 @return	true if it succeeds, false if it fails.
 */

bool CzScene::findActorInRemovals(CzActor* actor)
{
	for (_Iterator it = Removals.begin(); it != Removals.end(); ++it)
		if (*it == actor)
			return true;

	return false;
}

/**
 @fn	void CzScene::removeActorNoDelete(CzActor* actor)

 @brief	Removes the actor from the scene without cleaning it up.

 Sometimes you may want to remove an actor from a scene without destroying, for example if you want to take an actor frop one scene and place it in another scene.

 @param [in]	actor	If non-null, the actor.
 */

void CzScene::removeActorNoDelete(CzActor* actor)
{
	Actors.remove(actor);
}

/**
 @fn	void CzScene::removeActor(CzActor* actor, bool instant_delete)

 @brief	Removes the actor from the scene.

 If instant_delete is false (as by default) then the specified actor will be marked as destroyed and added to the scenes actor removal list, which is cleaned 
 up at the end of the scene. If instant_delete is true then the actor will instead of removed from the scene and instantly deleted. 

 Note that when an actor is removed from the scene, all actors that link to the actor will also be removed.

 @param [in,out]	actor	If non-null, the actor.
 @param	instant_delete   	true to instant delete the actor.
 */

void CzScene::removeActor(CzActor* actor, bool instant_delete)
{
	if (findActorInRemovals(actor))
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Scene - Actor already present in removal list, cannot remove the same actor twice - ", actor->getName().c_str(), DebugInfo.c_str());
		return;
	}

	if (instant_delete)
	{
		for (_Iterator it = Actors.begin(); it != Actors.end(); ++it)
		{
			if ((*it) == actor)
			{
				removeLinkedActors(actor, instant_delete);
				if (!actor->isManaged())
					delete actor;
				else
					actor->setDestroyed(true);
				Actors.erase(it);
				return;
			}
		}
	}

	removeLinkedActors(actor, instant_delete);
	Removals.push_back(actor);
	actor->setDestroyed(true);
}

/**
 @fn	void CzScene::removeActor(unsigned int name_hash, bool instant_delete)

 @brief	Removes the actor from the scene.
	
 If instant_delete is false (as by default) then the specified actor will be marked as destroyed and added to the scenes actor removal list, which is cleaned 
 up at the end of the scene. If instant_delete is true then the actor will instead of removed from the scene and instantly deleted.
	
 Note that when an actor is removed from the scene, all actors that link to the actor will also be removed.

 @param	name_hash	  	Actor name as a string hash.
 @param	instant_delete	true to instant delete the actor.
 */

void CzScene::removeActor(unsigned int name_hash, bool instant_delete)
{
	// Renove named actor from the scene
	for (_Iterator it = Actors.begin(); it != Actors.end(); ++it)
	{
		if (name_hash == (*it)->getNameHash())
		{
			removeLinkedActors(*it, instant_delete);
			if (instant_delete)
			{
				if (!(*it)->isManaged())
					delete *it;
				else
					(*it)->setDestroyed(true);
				Actors.erase(it);
			}
			else
			{
				Removals.push_back(*it);
				(*it)->setDestroyed(true);
			}
			return;
		}
	}
}

/**
 @fn	void CzScene::removeActors(unsigned int tag_hash)

 @brief	Removes all actors that belong to the spcified group.

 @param	tag_hash	The group tag as a string hash hash.
 */

void CzScene::removeActors(unsigned int tag_hash)
{
	// Renove named actor from the scene
	for (_Iterator it = Actors.begin(); it != Actors.end(); ++it)
	{
		if (tag_hash == (*it)->getTagHash())
		{
			removeLinkedActors(*it);
			Removals.push_back(*it);
			(*it)->setDestroyed(true);
		}
	}
}


void CzScene::fixActors(CzActor* parent)
{
/*	// When an actor is deleted it also deletes its visual. In the sprite manager any linked actors are added as children of the parent actors visual
	// When the parent actors visual is deleted, it also deletes all child sprites, so we have to find all actors in the scene that are linked to 
	// the parent and remove their visuals
	for (_Iterator it = Actors.begin(); it != Actors.end(); ++it)
	{
		if ((*it)->getLinkedTo() == parent)
		{
			(*it)->setVisual(NULL);
		}
	}
	*/
}

/**
 @fn	void CzScene::removeLinkedActors(CzActor* actor, bool instant_delete)

 @brief	Removes all actors that link to the specified actor.

 @param [in,out]	actor	If non-null, the actor.
 @param	instant_delete   	true to instant delete.
 */

void CzScene::removeLinkedActors(CzActor* actor, bool instant_delete)
{
	for (_Iterator it = Actors.begin(); it != Actors.end(); ++it)
	{
		CzActor* a = (*it);
		if (a->getLinkedTo() == actor)
			removeActor(a, instant_delete);
	}
}

/**
 @fn	int CzScene::checkActorState(CzActor* actor)

 @brief	Checks the actors current state.

 @param [in]	actor	If non-null, the actor.

 @return	0 - actor not found
			1 - actor is destroyed
			2 - actor is not destroyed.
 */

int CzScene::checkActorState(CzActor* actor)
{
	// Find and return named actors state.
	for (_Iterator it = Actors.begin(); it != Actors.end(); ++it)
	{
		if (*it == actor)
		{
			if (actor->isDestroyed())
				return 1;
			else
				return 2;
			break;
		}
	}

	return 0;
}

/**
 @fn	CzActor* CzScene::findActor(const char* name)

 @brief	Searches for the named actor.

 @param	name	The actors name.

 @return	null if not found, else the found actor.
 */

CzActor* CzScene::findActor(const char* name)
{
	return findActor(CzString::CalculateHash(name));
}

/**
 @fn	CzActor* CzScene::findActor(unsigned int name_hash)

 @brief	Searches for the named actor.

 @param	name_hash	The actors name as a strnig hash.

 @return	null if not found, else the found actor.
 */

CzActor* CzScene::findActor(unsigned int name_hash)
{
	// Find and eturn named actor
	for (_Iterator it = Actors.begin(); it != Actors.end(); ++it)
	{
		if (name_hash == (*it)->getNameHash() && !(*it)->isDestroyed())
			return *it;
	}

	return NULL;
}

/**
 @fn	CzActor* CzScene::findActor(int type)

 @brief	Searches for the first actor of the specified type.

 @param	type	The actor type.

 @return	null if not found, else the found actor.
 */

CzActor* CzScene::findActor(int type)
{
	// Find and return first actor of specified type
	for (_Iterator it = Actors.begin(); it != Actors.end(); ++it)
	{
		if (type == (*it)->getType() && !(*it)->isDestroyed())
			return *it;
	}
	return NULL;
}

/**
 @fn	int CzScene::findActors(unsigned int tag_hash, CzVector<CzActor*>& actors)

 @brief	Searches for a group of actors that have a specific group tag name.

 @param	tag_hash		  	The group tag as a string hash.
 @param [out]	actors	[out] The actors that were found.

 @return	The number of found actors.
 */

int CzScene::findActors(unsigned int tag_hash, CzVector<CzActor*>& actors)
{
	// Find all actors with named tag
	int count = 0;
	for (_Iterator it = Actors.begin(); it != Actors.end(); ++it)
	{
		if (tag_hash == (*it)->getTagHash())
		{
			actors.push_back(*it);
			count++;
		}
	}

	return count;
}

/**
 @fn	int CzScene::findActorsOfType(int type, CzVector<CzActor*>& actors)

 @brief	Searches for a group of actors that have a specific group tag name.

 @param	type		  	The type of actor
 @param [out]	actors	[out] The actors that were found.

 @return	The number of found actors.
 */

int CzScene::findActorsOfType(int type, CzVector<CzActor*>& actors)
{
	// Find all actors with named tag
	int count = 0;
	for (_Iterator it = Actors.begin(); it != Actors.end(); ++it)
	{
		if (type == (*it)->getType())
		{
			actors.push_back(*it);
			count++;
		}
	}

	return count;
}

/**
 @fn	CzActor* CzScene::findClosestActor(int x, int y, int type)

 @brief	Searches for the actor of the specific type that is nearest to the specified point.

 @param	x   	The x coordinate of the point in scene coordinates.
 @param	y   	The y coordinate of the point in scene coordinates.
 @param	type	The actor type.

 @return	null if not found, else the found actor.
 */

CzActor* CzScene::findClosestActor(int x, int y, int type)
{
	// Find player frog that is closest to point x, y
	CzActor* closest = NULL;
	float closest_d = 99999999.9f;

	for (_Iterator it = begin(); it != end(); ++it)
	{
		if ((*it)->getType() == type && !(*it)->isDestroyed())
		{
			CzVec2 pos = (*it)->getPosition();
			float dx = x - pos.x;
			float dy = y - pos.y;
			float d = dx * dx + dy * dy;

			if (d < closest_d)
			{
				closest = *it;
				closest_d = d;
			}
		}
	}

	return closest;
}

/**
 @fn	CzActor* CzScene::findFurthestActor(int x, int y, int type)

 @brief	Searches for the actor of the specific type that is furthest from the specified point.

 @param	x   	The x coordinate of the point in scene coordinates.
 @param	y   	The y coordinate of the point in scene coordinates.
 @param	type	The actor type.

 @return	null if not found, else the found actor.
 */

CzActor* CzScene::findFurthestActor(int x, int y, int type)
{
	// Find player frog that is closest to point x, y
	CzActor* furthest = NULL;
	float furthest_d = -99999999.9f;

	for (_Iterator it = begin(); it != end(); ++it)
	{
		if ((*it)->getType() == type && !(*it)->isDestroyed())
		{
			CzVec2 pos = (*it)->getPosition();
			float dx = x - pos.x;
			float dy = y - pos.y;
			float d = dx * dx + dy * dy;

			if (d > furthest_d)
			{
				furthest = *it;
				furthest_d = d;
			}
		}
	}

	return furthest;
}

/**
 @fn	void CzScene::clearActors(bool ignore_visuals)

 @brief	Clears and destroys all actors in the scene.

 ignore_visuals - When a scene closes down we dont want actors to delete their own visuals as the sprite manager will auto clean them up when it shuts down

 @param	ignore_visuals	true to ignore visuals.
 */

void CzScene::clearActors(bool ignore_visuals)
{
	// Remove and delete all actors from the scsne (only delete if actors are not managed)
	while (Actors.begin() != Actors.end())
	{
		CzActor* actor = *Actors.begin();
		if (ignore_visuals)		// When a scene closes down we dont want actors to delete their own visuals as the sprite manager will auto clean them up when it shuts down
			actor->setVisual(NULL);
		if (!actor->isManaged())
			delete actor;
		Actors.erase(Actors.begin());
	}

	if (Actors.size() != 0)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Scene - Still contains actors", DebugInfo.c_str());
	}

	Actors.clear();
}

/**
 @fn	bool CzScene::addCollideable(CzActor* actor)

 @brief	Adds n actor to the a collideables list.

 @param [in]	actor	If non-null, the actor.

 @return	true if it succeeds, false if it fails.
 */

bool CzScene::addCollideable(CzActor* actor)
{
	if (NextFreeCollidable < MaxCollidables)
		Collidables[NextFreeCollidable++] = actor;
	else
		return false;

	return true;
}

/**
 @fn	void CzScene::setCamera(CzCamera* camera)

 @brief	Sets the scenes current camera.

 @param [in,out]	camera	If non-null, the camera.
 */

void CzScene::setCamera(CzCamera* camera)
{
	if (Camera == camera)
		return;

	if (Camera != NULL)
	{
		if (!Camera->isManaged())
		{
			SAFE_DELETE(Camera)
		}
	}
	Camera = camera;
	if (Camera != NULL)
		Camera->setTransformDirty(true);
}

/**
 @fn	void CzScene::UpdateActorResizing()

 @brief	Causes UI based actors to resize themselves.

 Checks all UI based actors in the scene to see if they need their size updating. If so then the UI actor will call its ResizeToContent() method to 
 update its size.

 */

void CzScene::UpdateActorResizing()
{
	_Iterator it = begin();
	if (it == end())
		return;

	// Step to last actor
	int size = Actors.size();
	for (int t = 0; t < size - 1; t++, it++);

	// Check all actors to see if any of them have resized
	for (int t = 0; t < size; t++)
	{
		if ((*it)->getCategory() == CzActor::AC_UI)
		{
			CzUIBase* ui = (CzUIBase*)*it;
			if (ui->getNeedsResize())
				ui->ResizeToContent();
		}
		it--;
	}
}

/**
 @fn	void CzScene::UpdatePhysics(float dt)

 @brief	Updates the physics world.

 @param	dt	The number of seconds since physics was last updated
 */

void CzScene::UpdatePhysics(float dt)
{
	if (PhysicsEnabled && Box2dWorld != NULL)
	{
		if (ConstantTimeStep == 0)
			Box2dWorld->UpdateWorld(dt);
		else
			Box2dWorld->UpdateWorld(ConstantTimeStep);
	}
}

/**
 @fn	void CzScene::Update(float dt)

 @brief	Updates the scene.

 Updates the scenes logic, including:
 - Processing scene OnTick event
 - Updating scene bindings
 - Updating scene modifiers
 - Updating scene programs
 - Updating physics world
 - Updating camera
 - Updating actors
 - Removing old destroyed actors
 - Updating scene timeline
 - Clearing collision lists

 This method should be overriden by derived scenes to provide the scene specific logic. 

 Note that dt is used to scale scene internals that rely on time. This value enables the actor run in an environment where time (the frame rate of the device) can 
 vary allowing animation to keep up without slowing down or speeding up when the frame rate varies. 

 @param	dt	The number of seconds since the app was last updated
 */

void CzScene::Update(float dt)
{
	if (!IsActive)
		return;

//	int screen_width = PLATFORM_DISPLAY->getCurrentWidth();
//	int screen_height = PLATFORM_DISPLAY->getCurrentHeight();
//	if (screen_width != ScreenSize.x || screen_height != ScreenSize.y)
//		NotifyOrientationChanged();

	if (TickEnabled)
		ProcessEventActions(CzHashes::OnTick_Hash);

	// Update any data bindings
	if (Bindings != NULL)
		UpdateBindings();

	// Update modifiers
	if (Modifiers != NULL)
		Modifiers->Execute(this, dt);

	// Run any scene programs
	ProgramManager->Execute(dt);

	if (!Parent->isTimePaused())
		UpdatePhysics(dt);

	// Update timers
	TimersManager->Update(dt);

	IsDragging = false;

	if (Camera != NULL)
	{
		bool touching = CZ_INPUT->getTouch(0)->active;
		CzVec2 cam_pos = Camera->getPosition();
		CzVec2 cam_vel = Camera->getVelocity();
		bool changed_x = false;
		bool changed_y = false;

		// Handle camera panning with touch
		if (Parent->isTouchPanEnabled())
		{
			if (Parent->getTouchFocus(0) == NULL || (Parent->getTouchFocus(0) != NULL && Camera->getIgnoreActors()))
			{
				bool pan_x = Camera->isTouchPanX();
				bool pan_y = Camera->isTouchPanY();
				if (pan_x || pan_y)
				{
					// Calculate drag in virtual canvas coordinates
					CzIVec2 raw_drag = CZ_INPUT->getDragDelta(0);
					CzVec2 drag = CzVec2((float)raw_drag.x, (float)raw_drag.y);
//					drag.x = (drag.x / Transform.m[0][0]);
//					drag.y = (drag.y / Transform.m[1][1])ex;
					if (touching)
					{
						//if (drag.x >= -1 && drag.x <= 1)
//							drag.x = 0;
						if (pan_x)
						{
							if (drag.x != 0)
							{
								cam_pos.x -= drag.x;
								IsDragging = true;
							}
							changed_x = true;
							PrevTouchVel.x = -drag.x / 2.0f;
						}
//						if (drag.y >= -1 && drag.y <= 1)
//							drag.y = 0;
						if (pan_y)
						{
							if (drag.y != 0)
							{
								cam_pos.y -= drag.y;
								IsDragging = true;
							}
							changed_y = true;
							PrevTouchVel.y = -drag.y / 2.0f;
						}
						if (changed_x || changed_y)
						{
							Parent->releaseTouchFocus(0, 999999, 999999);	// Use stupidly large values so they are invalid
							NotifyPan();
						}
					}
					else
					if (PrevTouching)
					{
						if (pan_x)
						{
							if (PrevTouchVel.x >= -1 && PrevTouchVel.x <= 1)
								PrevTouchVel.x = 0;
							cam_vel.x = PrevTouchVel.x * 60.0f;
							changed_x = true;
						}
						if (pan_y)
						{
							if (PrevTouchVel.y >= -1 && PrevTouchVel.y <= 1)
								PrevTouchVel.y = 0;
							cam_vel.y = PrevTouchVel.y * 60.0f;
							changed_y = true;
						}
						PrevTouchVel.x = 0;
						PrevTouchVel.y = 0;
					}
				}
			}
		}

		if (Extents.w != 0)
		{
//						CzVec2 offs = ScreenToVirtual(ScreenSize.x / 2.0f, ScreenSize.y / 2.0f);
			CzMatrix3 trans = Camera->getTransform();
			float min_x = (float)Extents.x;
			float min_y = (float)Extents.y;
			float max_x = (float)(Extents.x + Extents.w);
			float max_y = (float)(Extents.y + Extents.h);
			float sx = trans.m[0][0];
			float sy = trans.m[1][1];
			min_x *= sx;
			max_x *= sx;
			min_y *= sy;
			max_y *= sy;
			min_x += ScreenSize.x / 2;
			max_x -= ScreenSize.x / 2;
			min_y += ScreenSize.y / 2;
			max_y -= ScreenSize.y / 2;
			if (max_x < min_x)
				max_x = min_x = 0;
			if (max_y < min_y)
				max_y = min_y = 0;

			// Prevent cam from going outside scenes extents
			if (cam_pos.x < min_x)
			{
				cam_pos.x = min_x;
				cam_vel.x = 0;
				PrevTouchVel.x = 0;
				changed_x = true;
			}
			else
			if (cam_pos.x > max_x)
			{
				cam_pos.x = max_x;
				cam_vel.x = 0;
				PrevTouchVel.x = 0;
				changed_x = true;
			}
			if (cam_pos.y < min_y)
			{
				cam_pos.y = min_y;
				cam_vel.y = 0;
				PrevTouchVel.y = 0;
				changed_y = true;
			}
			else
			if (cam_pos.y > max_y)
			{
				cam_pos.y = max_y;
				cam_vel.y = 0;
				PrevTouchVel.y = 0;
				changed_y = true;
			}
		}

		// Update cam position and velocity
		if (changed_x)
		{
			Camera->setPosition(cam_pos.x, Camera->getPosition().y);
			Camera->setVelocity(cam_vel.x, Camera->getVelocity().y);
		}
		if (changed_y)
		{
			Camera->setPosition(Camera->getPosition().x, cam_pos.y);
			Camera->setVelocity(Camera->getVelocity().x, cam_vel.y);
		}

		PrevTouching = touching;

		// I need to point out here that we use the concept of dirty transforms to cut down processing when  the camera is not moving
		// If the cameras transform is not dirty (the camera has not moved) then there is no need to force the sprite manager to make 
		// all of its child sprites to rebuild their transforms
		Camera->Update(dt);
		if (Camera->isTransformDirty())
		{
			// Combine the virtual canvas and camera transforms
			Transform = Camera->getTransform();
			Transform.MultiplyPost(&VirtualTransform);

			// Transfer scene transform to sprite manager
			if (SpriteManager != NULL)
			{
				SpriteManager->setTransformNoCamera(VirtualTransform);
				SpriteManager->setTransform(Transform);
			}
			Camera->setTransformDirty(false);
		}
	}

	if (ClippingArea.w != 0)
		SpriteManager->setClipRect((float)ClippingArea.x, (float)ClippingArea.y, (float)ClippingArea.w, (float)ClippingArea.h);
	else
		SpriteManager->setClipRect((float)(-VirtualSize.x / 2), (float)(-VirtualSize.y / 2), (float)VirtualSize.x, (float)VirtualSize.y);

	// Process the actors
	for (_Iterator it = begin(); it != end(); ++it)
	{
//CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Scene - Processing actor: : ", (*it)->getName().c_str());

#if defined(CZ_ENABLE_METRICS)
		CzMetrics::TotalActorsProcessed++;
#endif
		CzActor* actor = *it;
		if (!actor->Update(dt))
			removeActor(actor);
	}

	UpdateActorResizing();

	// Resolve actor collisions
	for (_Iterator it = begin(); it != end(); ++it)
	{
		if ((*it)->isCollidable())
			(*it)->ResolveCollisions();
	}

	// Remove any deleted actors
	CleanupRemovedActors();

	// Update the animation timeline
	if (Timeline != NULL)
	{
		if (!Timeline->Update(dt, this))
		{
			if (Timeline->isAutoDelete())
				Timeline = NULL;
		}
	}

	// Reset collidables list
	NextFreeCollidable = 0;

	// Reset collision lists
	for (_Iterator it = begin(); it != end(); it++)
	{
		CzBox2dBody* body = (*it)->getBox2dBody();
		if (body != NULL)
			body->clearAllCollisions();
	}

	// Update run count
	RunCount++;
}

/**
 @fn	void CzScene::CleanupRemovedActors()

 @brief	Cleans up removed actors.

 Cleans up any actors that were destroyed during the last frame. Also removes any desdtrpyed actors from the touch and key focus lists.

 */
 
void CzScene::CleanupRemovedActors()
{
	for (_Iterator it = Removals.begin(); it != Removals.end(); ++it)
	{
		CzActor* focus;
		CzActor* actor = *it;

#if defined(_DEBUG)
		if (actor->getClassTypeHash() != CzHashes::Actor_Hash)
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Scene - CleanupRemovedActors() - Trying to cleanup invalid actor", DebugInfo.c_str());
#endif
		// if actor has touch focus then release it
		for (int t = 0; t < CZ_MAX_TOUCHES; t++)
		{
			focus = Parent->getTouchFocus(t);
			if (focus != NULL && focus == actor)
				Parent->setTouchFocus(t, NULL);
		}
		focus = Parent->getKeyFocus();
		if (focus != NULL && focus == actor)
			Parent->setKeyFocus(NULL);

		// Do not delete managed actors as the pooled object manager will be handling this
		if (!actor->isManaged())
		{
			delete actor;
		}
		else
		{
			// Instead we disable them and mark them as unused so they can be reallocated
			actor->setUsed(false);
			actor->setActive(false);
		}

		Actors.remove(actor);
	}
	Removals.clear();
}

/**
 @fn	void CzScene::CheckforInvalidRemovals(const char* check_point)

 @brief	Checks the actor removals list for invalid removals.

 @param	check_point	The check point name.
 */

void CzScene::CheckforInvalidRemovals(const char* check_point)
{
	for (_Iterator it = Removals.begin(); it != Removals.end(); ++it)
	{
#if defined(_DEBUG)
		if ((*it)->getClassTypeHash() != CzHashes::Actor_Hash)
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Scene - CheckforInvalidRemovals() - Trying to cleanup invalid actor - ", check_point, DebugInfo.c_str());
#endif
	}
}

/**
 @fn	void CzScene::Draw()

 @brief	Draws this scene.

 Renders the scene and all of its contents.  This includes:
 - Updating actor visuals
 - Setting the scenes transformed clipping rectangle
 - Drwing the scenes sprite manager

 */

void CzScene::Draw()
{
	if (!IsVisible)
		return;

	// Transfer actor visual data to sprite manager sprites
	for (_Iterator it = begin(); it != end(); ++it)
	{
		CzActor* s = *it;
		s->UpdateVisual();
	}

	if (ClippingArea.w != 0)
	{
		// Set clipping rectangle (clipping coords are specified in virtual canvas coordinates so we need to transform them to the screen)
		TransformedClipRect[0].x = (float)ClippingArea.x;
		TransformedClipRect[1].x = (float)(ClippingArea.x + ClippingArea.w);
		TransformedClipRect[0].y = (float)ClippingArea.y;
		TransformedClipRect[1].y = (float)(ClippingArea.y + ClippingArea.h);
		if (ClipStatic)
			VirtualTransform.TransformN(TransformedClipRect, TransformedClipRect, 2);
		else
			Transform.TransformN(TransformedClipRect, TransformedClipRect, 2);
	}
	else
	{
		TransformedClipRect[0].x = 0;
		TransformedClipRect[0].y = 0;
		TransformedClipRect[1].x = (float)ScreenSize.x;
		TransformedClipRect[1].y = (float)ScreenSize.y;
	}
	SpriteManager->setScreenClipRect(TransformedClipRect[0].x, TransformedClipRect[0].y, (TransformedClipRect[1].x) - (TransformedClipRect[0].x), (TransformedClipRect[1].y) - (TransformedClipRect[0].y));

	// Draw the sprite manager sprites
	SpriteManager->Draw();

	if (ClippingArea.w != 0)
		PLATFORM_RENDER->ResetClipRect();

}

/**
 @fn	void CzScene::RestoreClipping()

 @brief	Restore the scenes clipping rectangle.

 */

void CzScene::RestoreClipping()
{
	if (ClippingArea.w != 0)
	{
		PLATFORM_RENDER->SetClipRect((int)TransformedClipRect[0].x, (int)TransformedClipRect[0].y, (int)(TransformedClipRect[1].x - TransformedClipRect[0].x), (int)(TransformedClipRect[1].y - TransformedClipRect[0].y));
	}
}

/**
 @fn	CzVec2 CzScene::ScreenToVirtual(float screen_x, float screen_y, bool ignore_translation)

 @brief	Converts a device screen coordinates to scene virtual canvas coordinates.

 @param	screen_x		  	The screen x coordinate.
 @param	screen_y		  	The screen y coordinate.
 @param	ignore_translation	true to ignore translation.

 @return	Scene virtual canvas coordinate.
 */

CzVec2 CzScene::ScreenToVirtual(float screen_x, float screen_y, bool ignore_translation)
{
	if (!ignore_translation)
	{
		screen_x -= VirtualTransform.getX();
		screen_y -= VirtualTransform.getY();
	}

	return CzVec2(screen_x / VirtualTransform.m[0][0], screen_y / VirtualTransform.m[1][1]);
}

/**
 @fn	CzVec2 CzScene::VirtualToScreen(float virtual_x, float virtual_y, bool ignore_translation)

 @brief	Convetrs scene virtual canvas coordinates to device screen coordinates.

 @param	virtual_x		  	The virtual x coordinate.
 @param	virtual_y		  	The virtual y coordinate.
 @param	ignore_translation	true to ignore translation.

 @return	Screen coordinate.
 */

CzVec2 CzScene::VirtualToScreen(float virtual_x, float virtual_y, bool ignore_translation)
{
	if (!ignore_translation)
		return CzVec2(virtual_x * VirtualTransform.m[0][0] + VirtualTransform.getX(), virtual_y * VirtualTransform.m[1][1] + VirtualTransform.getY());
	else
		return CzVec2(virtual_x * VirtualTransform.m[0][0], virtual_y * VirtualTransform.m[1][1]);
}

/**
 @fn	CzVec2 CzScene::ScreenToCamera(float pos_x, float pos_y, bool ignore_translation)

 @brief	Converts a device screen coordinates to the scenes camera coordinates.

 @param	pos_x			  	The position x coordinate in screen coordinates.
 @param	pos_y			  	The position y coordinate in screen coordinates.
 @param	ignore_translation	true to ignore translation.

  @return	Scene camera coordinate.
*/

CzVec2 CzScene::ScreenToCamera(float pos_x, float pos_y, bool ignore_translation)
{
	CzVec2 cam_pos = CzVec2(0, 0);
	if (Camera != NULL)
		cam_pos = Camera->getPosition();
	CzMatrix3 cam_transform;
	cam_transform.Identity();

	if (Camera != NULL)
		cam_transform = Camera->getTransform();

	cam_pos.x = cam_pos.x / cam_transform.m[0][0];
	cam_pos.y = cam_pos.y / cam_transform.m[1][1];

	if (!ignore_translation)
	{
		pos_x -= VirtualTransform.getX();
		pos_y -= VirtualTransform.getY();
	}

	CzVec2 virtual_pos = CzVec2(pos_x / Transform.m[0][0], pos_y / Transform.m[1][1]);

	return CzVec2(cam_pos.x + virtual_pos.x, cam_pos.y + virtual_pos.y);
}

/**
 @fn	void CzScene::ProcessEventActions(unsigned int event_name)

 @brief	Process the event actions described by event_name.

 Processes the actions list that is associated with the event named event_name (event_name is a hash of the actual event name to speed up searches)

 @param	event_name	Name of the event as a string hash
 */

void CzScene::ProcessEventActions(unsigned int event_name)
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

void CzScene::NotifyPan()
{
	ProcessEventActions(CzHashes::OnPan_Hash);
}

void CzScene::NotifyCreate()
{
	ProcessEventActions(CzHashes::OnCreate_Hash);
}

void CzScene::NotifyDestroy()
{
	ProcessEventActions(CzHashes::OnDestroy_Hash);
}

void CzScene::NotifySuspending(CzScene* new_scene)
{
	ProcessEventActions(CzHashes::OnSuspend_Hash);
}

void CzScene::NotifyResuming(CzScene* old_scene)
{
	ProcessEventActions(CzHashes::OnResume_Hash);
}

void CzScene::NotifyLostFocus(CzScene* new_scene)
{
	ProcessEventActions(CzHashes::OnLostFocus_Hash);
}

void CzScene::NotifyGainedFocus(CzScene* old_scene)
{
	ProcessEventActions(CzHashes::OnGainedFocus_Hash);
}

void	CzScene::NotifyDeviceSuspended()
{
	ProcessEventActions(CzHashes::OnDeviceSuspended_Hash);
}

void	CzScene::NotifyDeviceResumed()
{
	ProcessEventActions(CzHashes::OnDeviceResumed_Hash);
}

void	CzScene::NotifyDeviceVolumeUp()
{
	ProcessEventActions(CzHashes::OnDeviceVolumeUp_Hash);
}

void	CzScene::NotifyDeviceVolumeDown()
{
	ProcessEventActions(CzHashes::OnDeviceVolumeDown_Hash);
}

void	CzScene::NotifyDeviceNetworkOnline()
{
	ProcessEventActions(CzHashes::OnDeviceNetworkOnline_Hash);
}

void	CzScene::NotifyDeviceNetworkOffline()
{
	ProcessEventActions(CzHashes::OnDeviceNetworkOffline_Hash);
}

/**
 @fn	void CzScene::Suspend(CzScene* new_scene)

 @brief	Suspends this scene

 @param [in,out]	new_scene	If non-null, the new scene.
 */

void CzScene::Suspend(CzScene* new_scene)
{
	NotifySuspending(this);
	setActive(false);
}

/**
 @fn	void CzScene::Resume(CzScene* old_scene)

 @brief	Resumes this scene.

 @param [in,out]	old_scene	If non-null, the old scene.
 */

void CzScene::Resume(CzScene* old_scene)
{
	NotifyResuming(this);
	setActive(true);
}

void CzScene::NotifyKeyBack()
{
	ProcessEventActions(CzHashes::OnKeyBack_Hash);
}
void CzScene::NotifyKeyMenu()
{
	ProcessEventActions(CzHashes::OnKeyMenu_Hash);
}

void CzScene::NotifyOrientationChanged()
{
	CzScene::eOrientation old = Orientation;

	ProcessEventActions(CzHashes::OnOrientationChange_Hash);

	// Resize scene
	int min = (OriginalSize.x < OriginalSize.y) ? OriginalSize.x : OriginalSize.y;
	int max = (OriginalSize.x > OriginalSize.y) ? OriginalSize.x : OriginalSize.y;
	if (old == Orientation_Landscape)
		setVirtualTransform(min, max, 0, CanvasFit, CanvasOrigin);
	else
	if (old == Orientation_Portrait)
		setVirtualTransform(max, min, 0, CanvasFit, CanvasOrigin);

	// Inform actors of change
	for (_Iterator it = begin(); it != end(); ++it)
	{
		(*it)->NotifyOrientationChange(old, Orientation);
	}
}

void CzScene::NotifySizeChanged()
{
	CzScene::eOrientation old = Orientation;

	ProcessEventActions(CzHashes::OnSizeChange_Hash);

	// Resize scene
	int min = (OriginalSize.x < OriginalSize.y) ? OriginalSize.x : OriginalSize.y;
	int max = (OriginalSize.x > OriginalSize.y) ? OriginalSize.x : OriginalSize.y;
	if (old == Orientation_Landscape)
		setVirtualTransform(min, max, 0, CanvasFit, CanvasOrigin);
	else
	if (old == Orientation_Portrait)
		setVirtualTransform(max, min, 0, CanvasFit, CanvasOrigin);

	// Inform actors of change
	for (_Iterator it = begin(); it != end(); ++it)
	{
		(*it)->NotifySizeChange();
	}
}

/**
 @fn	CzActor* CzScene::FindTappedActor(int x, int y)

 @brief	Searches for the first tapped actor.

 Searches this scene to see if the supplied point lies within the boundaries of an actor. If one is found then it is returned.  Actors on higher actor layers 
 will be chosen over actors on lower layers.

 @param	x	The x screen coordinate to test.
 @param	y	The y screen coordinate to test.

 @return	null if none found, else the found tapped actor.
 */

CzActor* CzScene::FindTappedActor(int x, int y)
{
	CzActor* hit = NULL;
	for (_Iterator it = begin(); it != end(); ++it)
	{
		if ((*it)->isTappable())
		{
			if (hit == NULL || (hit->getLayer() <= (*it)->getLayer()))
			{
				CzActor* new_hit = (*it)->HitTest((float)x, (float)y);
				if (new_hit != NULL)
					hit = new_hit;
			}
		}
	}

	return hit;
}

/**
 @fn	int CzScene::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)

 @brief	Creates an instance of this class from XOML.

 LoadFromXoml is the main method used by all classes that can be instantiated from XOML mark-up. This method creates an instance of this class from the supplied XML node structure specified by node. 
 Note that a scene can only be loaded into the main app, so parent must be of type CzApp.
 
 @param [in]	parent		If non-null, the parent.
 @param	load_children	  	true to load child nodes.
 @param [in]	node  		If non-null, the XOML markup node that defines this object

 @return	nergative value for error. if 0 then processing of further XOML will stop.
  */

int CzScene::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Check that we were created from a CzApp derived class
	if (parent->getClassTypeHash() != CzHashes::Game_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Scene - Needs to be declared inside a CzApp derived class", DebugInfo.c_str());
		return 0;
	}

	// Process scene attributes
	int				max_colliders = 128;
	int				max_layers = 10;
	CzVec2			canvas_size((float)PLATFORM_DISPLAY->getCurrentWidth(), (float)PLATFORM_DISPLAY->getCurrentHeight());
	bool			fix_aspect = true;
	bool			lock_width = false;
	eCanvasFit canvas_fit = Fit_Best;
	eCanvasOrigin canvas_origin = Origin_Centre;
	bool			current = false;
	CzString		timeline_name;
	CzString		bindings_name;
	CzString		camera_name;
	CzString*		scriptengine = NULL;
	CzColour		colour;
	bool			batch = false;
	bool			allow_focus = false;
	bool			do_sleep = true;
	bool			canvas_fit_set = false;
	CzVec2			gravity = CzVec2(0, 0);
	bool			gravity_set = false;
	bool			physics = true;
	CzVec2			world_scale = CzVec2(10, 10);
	bool			world_scale_set = false;

	colour.set(0xffffffff);

	// Check and apply style
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();
		if (name_hash == CzHashes::Style_Hash)
		{
			if (!(*it)->getValue().isEmpty())
			{
				// Find the style
				CzStyle* style = (CzStyle*)CZ_GLOBAL_RESOURCES->getResourceManager()->findResource((*it)->getValue().c_str(), CzHashes::Style_Hash);
				if (style == NULL)
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - Could not find named style in the global resources - ", (*it)->getValue().c_str(), DebugInfo.c_str());
				else
				{
					// Apply the style
					CzXoml::ApplyStyle(style, node);
				}
				break;
			}
		}
	}

	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
			setName((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Type_Hash)
			setType((*it)->getValueAsInt());
		else
		if (name_hash == CzHashes::Layer_Hash)
			setLayer((*it)->getValueAsInt());
		else
		if (name_hash == CzHashes::CanvasSize_Hash)
		{
			if (!(*it)->getValueAsPoint(canvas_size))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - Invalid value for CanvasSize, expected vec2", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::CanvasFit_Hash)
		{
			canvas_fit_set = true;
			unsigned int hash = (*it)->getValue().getHash();
			if (hash == CzString::CalculateHash("width"))
				canvas_fit = Fit_Width;
			else
			if (hash == CzString::CalculateHash("height"))
				canvas_fit = Fit_Height;
			else
			if (hash == CzString::CalculateHash("both"))
				canvas_fit = Fit_Both;
			else
			if (hash == CzString::CalculateHash("best"))
				canvas_fit = Fit_Best;
			else
				canvas_fit = Fit_None;
		}
		else
		if (name_hash == CzHashes::CanvasOrigin_Hash)
		{
			unsigned int hash = (*it)->getValue().getHash();
			if (hash == CzString::CalculateHash("centre"))
				canvas_origin = Origin_Centre;
			else
			if (hash == CzString::CalculateHash("topleft"))
				canvas_origin = Origin_TopLeft;
			else
			if (hash == CzString::CalculateHash("top"))
				canvas_origin = Origin_Top;
			else
			if (hash == CzString::CalculateHash("left"))
				canvas_origin = Origin_Left;
		}
		else
		if (name_hash == CzHashes::FixAspect_Hash)
			fix_aspect = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::LockWidth_Hash)
			lock_width = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Extents_Hash)
		{
			CzIRect extents;
			if (!(*it)->getValueAsRect(extents))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - Invalid value for Extents, expected rect", DebugInfo.c_str());
			else
				setExtents(extents.x, extents.y, extents.w, extents.h);
		}
		else
		if (name_hash == CzHashes::AllowSuspend_Hash)
			setAllowSuspend((*it)->getValueAsBool());
		else
		if (name_hash == CzHashes::Clipping_Hash)
		{
			CzIRect clipping;
			if (!(*it)->getValueAsRect(clipping))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - Invalid value for Clipping, expected rect", DebugInfo.c_str());
			else
				setClippingArea(clipping.x, clipping.y, clipping.w, clipping.h);
		}
		else
		if (name_hash == CzHashes::ClipStatic_Hash)
			ClipStatic = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Active_Hash)
			setActive((*it)->getValueAsBool());
		else
		if (name_hash == CzHashes::Visible_Hash)
			setVisible((*it)->getValueAsBool());
		else
		if (name_hash == CzHashes::Layers_Hash)
			max_layers = ((*it)->getValueAsInt());
		else
		if (name_hash == CzHashes::Colliders_Hash)
			max_colliders = ((*it)->getValueAsInt());
		else
		if (name_hash == CzHashes::Current_Hash)
			current = ((*it)->getValueAsBool());
		else
		if (name_hash == CzHashes::Timeline_Hash)
			timeline_name = (*it)->getValue();
		else
		if (name_hash == CzHashes::Bindings_Hash)
			bindings_name = (*it)->getValue();
		else
		if (name_hash == CzHashes::Colour_Hash || name_hash == CzHashes::Color_Hash)
		{
			if (!(*it)->getValueAsColour(colour))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - Invalid value for Colour, expected colour", DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::Opacity_Hash)
			colour.a = (*it)->getValueAsInt();
		else
		if (name_hash == CzHashes::Camera_Hash)
			camera_name = (*it)->getValue();
		else
		if (name_hash == CzHashes::Batch_Hash)
			batch = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::AllowFocus_Hash)
			allow_focus = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::OnSuspend_Hash)
			EventsManager->addEvent("OnSuspend", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnResume_Hash)
			EventsManager->addEvent("OnResume", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnLostFocus_Hash)
			EventsManager->addEvent("OnLostFocus", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnGainedFocus_Hash)
			EventsManager->addEvent("OnGainedFocus", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnCreate_Hash)
			EventsManager->addEvent("OnCreate", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnDestroy_Hash)
			EventsManager->addEvent("OnDestroy", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnKeyBack_Hash)
			EventsManager->addEvent("OnKeyBack", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnKeyMenu_Hash)
			EventsManager->addEvent("OnKeyMenu", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnOrientationChange_Hash)
			EventsManager->addEvent("OnOrientationChange", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnPan_Hash)
			EventsManager->addEvent("OnPan", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnTick_Hash)
		{
			EventsManager->addEvent("OnTick", (*it)->getValue().c_str());
			TickEnabled = true;
		}
		else
		if (name_hash == CzHashes::OnDeviceSuspended_Hash)
			EventsManager->addEvent("OnDeviceSuspend", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnDeviceResumed_Hash)
			EventsManager->addEvent("OnDeviceResume", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnDeviceVolumeUp_Hash)
			EventsManager->addEvent("OnDeviceVolumeUp", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnDeviceVolumeDown_Hash)
			EventsManager->addEvent("OnDeviceVolumeDown", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnDeviceNetworkOnline_Hash)
			EventsManager->addEvent("OnDeviceNetworkOnline", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnDeviceNetworkOffline_Hash)
			EventsManager->addEvent("OnDeviceNetworkOffline", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::ScriptEngine_Hash)
			scriptengine = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Gravity_Hash)
		{
			if (!(*it)->getValueAsPoint(gravity))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - Invalid value for Gravity, expected vec2", DebugInfo.c_str());
			else
				gravity_set = true;
		}
		else
		if (name_hash == CzHashes::DoSleep_Hash)
			do_sleep = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::WorldScale_Hash)
		{
			if (!(*it)->getValueAsPoint(world_scale))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - Invalid value for WorldScale, expected vec2", DebugInfo.c_str());
			else
				world_scale_set = true;
		}
		else
		if (name_hash == CzHashes::Physics_Hash)
			physics = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::PhysicsTimestep_Hash)
			ConstantTimeStep = (*it)->getValueAsFloat();
	}

	// If scene already exists then load children into the original scene and discard this scene
	CzScene* old_scene = ((CzApp*)parent)->findScene(NameHash);
	if (old_scene != NULL && !old_scene->isDestroyed())
	{
		if (load_children)
		{
			// Process inner tags
			if (!CZ_XOML->Process(old_scene, node))
				return 0;
		}
		return -1;
	}

	PhysicsEnabled = physics;

	// Initialise the scene
	Parent = (CzApp*)parent;
	if (load_children)
	{
		Init(max_colliders, max_layers, do_sleep);
		SpriteManager->setBatching(batch);

		// Set the virtual tansform (called her first as the virtual transform needs to be set up so that actors inside the scene that use docking can have access to the virtual transform)
		if (canvas_fit_set)
			setVirtualTransform((int)canvas_size.x, (int)canvas_size.y, 0, canvas_fit, canvas_origin);
		else
			setVirtualTransform((int)canvas_size.x, (int)canvas_size.y, 0, fix_aspect, lock_width, canvas_origin);

		if (physics)
		{
			if (gravity_set)
				Box2dWorld->setGravity(gravity.x, gravity.y);
			if (world_scale_set)
				Box2dWorld->setWorldScale(world_scale.x, world_scale.y);
		}
	}

	// Set the colour
	Colour = colour;

	// Create script engine
	if (scriptengine != NULL)
		setScriptEngine(*scriptengine);

	if (load_children)
	{
		// Process inner tags
		if (!CZ_XOML->Process(this, node))
			return 0;
	}

	// Search the scenes resources for the specified timeline
	if (!timeline_name.isEmpty())
	{
		CzAnimTimeline* timeline = CzAnimTimelinesManager::FindTimeline(timeline_name.getHash(), this);
		if (timeline == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - Could not find named timeline in the scene or in the global resources - ", timeline_name.c_str(), DebugInfo.c_str());
		else
		{
			setTimeline(timeline);
			timeline->setTargetElement(this);
		}
	}

	// Search the scenes resources for the specified bindings manager
	if (!bindings_name.isEmpty())
	{
		CzXomlBindings* bindings = (CzXomlBindings*)getResourceManager()->findResource(bindings_name.c_str(), CzHashes::Bindings_Hash);
		if (bindings == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - Could not find named bindings in the scene or in the global resources - ", bindings_name.c_str(), DebugInfo.c_str());
		else
			setBindings(bindings);
	}

	// Add the scene to the game
	((CzApp*)parent)->addScene(this, current);

	// Add camera
	if (!camera_name.isEmpty())
	{
		// Find camera resource
		CzCamera* camera = (CzCamera*)getResourceManager()->findResource(camera_name.c_str(), CzHashes::Camera_Hash);
		if (camera == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - Could not find named camera in the scene or in the global resources - ", camera_name.c_str(), DebugInfo.c_str());
		setCamera(camera);

		if (Camera != NULL)
		{
			// Note that we set the virtual transform in this 2nd place because a camera has been attached which could change the transform
			if (canvas_fit_set)
				setVirtualTransform((int)canvas_size.x, (int)canvas_size.y, 0, canvas_fit, canvas_origin);
			else
				setVirtualTransform((int)canvas_size.x, (int)canvas_size.y, 0, fix_aspect, lock_width, canvas_origin);
		}
	}

	// If scene is marked as current then switcn to it
	if (current)
		((CzApp*)parent)->changeScene(this);

	// Set allow focus
	AllowFocus = allow_focus;

	return 1;
}

/**
 @fn	bool CzScene::PostLoadFromXoml(IzXomlResource* parent, CzXmlNode* node)

 @brief	Carries out any post XOML loading tasks

 PostLoadFromXoml adds the opportunity to carry out post load events, such as when the scene has finished loading all of its content. 
 In this case we update the scenes bindings for the first time to ensure that the scenes bound properties have the same values as the 
 variables that they are bound to. We also first off ht OnCreate event.

 @param [in]	parent		If non-null, the parent.
 @param [in]	node  		If non-null, the XOML markup node that defines this object

 @return	true if it succeeds, false if it fails.
 */

bool CzScene::PostLoadFromXoml(IzXomlResource* parent, CzXmlNode* node)
{
	// Force update any data bindings
	if (Bindings != NULL)
		UpdateBindings(true);

	NotifyCreate();

	return true;
}

/**
 @fn	bool CzScene::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the scene.

 Returns the named property of the scene. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types.

 @param	property_name		Name of the property.
 @param [out]	prop	The property data.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzScene::getProperty(const char* property_name, CzXomlProperty& prop)
{
	return getProperty(CZ_HASH(property_name), prop);
}

/**
 @fn	bool CzScene::getProperty(unsigned int property_name, CzXomlProperty& prop)

 Returns the named property of the scene. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in,out]	prop	The property.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzScene::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	int err = IzXomlResource::getProperty(ClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzScene::setProperty(const char* property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the scene.

 Sets the named property of this scene. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced.

 @param	property_name		Name of the property.
 @param [in]	data		The property data.
 @param	delta				true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzScene::setProperty(const char* property_name, const CzString& data, bool delta)
{
	return setProperty(CzString::CalculateHash(property_name), data, delta);
}

/**
 @fn	bool CzScene::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the scene.
	
 Sets the named property of this scene. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzScene::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	int err = IzXomlResource::setProperty(ClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - setProperty - Cannot set this property type from string - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzScene::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the scene.
	
 Sets the named property of this scene to the supplied property data. If delta is set to true then the existing value of the property will be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzScene::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	int err = IzXomlResource::setProperty(ClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzScene::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates scene  property from supplied animation.

 Updates the scenes property that is specified by the supplied animation. When a timeline animation targets the property of an scene this method will be called by the animation to move the 
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

bool CzScene::UpdateFromAnimation(CzAnimInstance *animation)
{
	int err = IzXomlResource::setProperty(ClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = ClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	void CzScene::UpdateBindings(bool force_modified)

 @brief	Updates the scenes internal bindings.

 Walks the scenes bindings list updating all of its bindings and an attached simple data binding if attached. Passing to true to force_modified will force the bindings to be updated, even 
 if the system does not think the bound variables have not changed.

 @param	force_modified	true to force an update.
 */

void CzScene::UpdateBindings(bool force_modified)
{
	for (CzXomlBindings::_Iterator it = Bindings->begin(); it != Bindings->end(); ++it)
	{
		unsigned int property_name = (*it)->Property;
		CzXomlVariable* var = (*it)->Variable;

		if (var->Modified || force_modified)
		{
			int err = IzXomlResource::setProperty(ClassDef, this, property_name, var);
			if (err < 0)
			{
				CzXomlClassProperty* cprop = ClassDef->findProperty(property_name);
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Scene - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
			}
		}
	}
}

