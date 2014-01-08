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

#include "CzUITextView.h"
#include "CzHashes.h"
#include "CzMath.h"
#include "CzBitmapSprite.h"

CzXomlClassDef* CzUITextView::TextViewClassDef = NULL;

/**
 @fn	void CzUITextView::InitClass()

 @brief	Initialises the class.
    
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzUITextView::InitClass()
{
	// Create class properties
	TextViewClassDef = new CzXomlClassDef();
	TextViewClassDef->addProperty(new CzXomlClassProperty("MinZoom",			PT_Float,		CzUITextView::_setMinZoom,			CzUITextView::_getMinZoom));
	TextViewClassDef->addProperty(new CzXomlClassProperty("MaxZoom",			PT_Float,		CzUITextView::_setMaxZoom,			CzUITextView::_getMaxZoom));
	TextViewClassDef->addProperty(new CzXomlClassProperty("Zoom",				PT_Float,		CzUITextView::_setZoom,				CzUITextView::_getZoom));
	TextViewClassDef->addProperty(new CzXomlClassProperty("Area",				PT_Vec2,		CzUITextView::_setArea,				CzUITextView::_getArea));

}
void CzUITextView::ReleaseClass()
{
	if (TextViewClassDef != NULL)
	{
		TextViewClassDef->Release();
		delete TextViewClassDef;
		TextViewClassDef = NULL;
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

bool CzUITextView::_setMinZoom(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUITextView* actor = (CzUITextView*)target;

	if (add)
		actor->setMinZoom(actor->getMinZoom() + prop.p_float);
	else
		actor->setMinZoom(prop.p_float);

	return true;
}

CzXomlProperty CzUITextView::_getMinZoom(IzXomlResource* target)
{
	return CzXomlProperty(((CzUITextView*)target)->getMinZoom());
}

bool CzUITextView::_setMaxZoom(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUITextView* actor = (CzUITextView*)target;

	if (add)
		actor->setMaxZoom(actor->getMaxZoom() + prop.p_float);
	else
		actor->setMaxZoom(prop.p_float);

	return true;
}

CzXomlProperty CzUITextView::_getMaxZoom(IzXomlResource* target)
{
	return CzXomlProperty(((CzUITextView*)target)->getMaxZoom());
}

bool CzUITextView::_setZoom(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUITextView* actor = (CzUITextView*)target;

	if (add)
		actor->setZoom(actor->getZoom() + prop.p_float);
	else
		actor->setZoom(prop.p_float);

	return true;
}

CzXomlProperty CzUITextView::_getZoom(IzXomlResource* target)
{
	return CzXomlProperty(((CzUITextView*)target)->getZoom());
}

bool CzUITextView::_setArea(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUITextView* actor = (CzUITextView*)target;

	if (add)
	{
		CzIVec2 area = actor->getArea();
		area.x += (int)prop.p_vec[0];
		area.y += (int)prop.p_vec[1];
		actor->setArea(area);
	}
	else
	{
		CzIVec2 area;
		area.x = (int)prop.p_vec[0];
		area.y = (int)prop.p_vec[1];
		actor->setArea(area);
	}

	return true;
}

CzXomlProperty CzUITextView::_getArea(IzXomlResource* target)
{
	return CzXomlProperty(((CzUITextView*)target)->getArea());
}




//
//
//
//
//
// CzUITextView implementation
//
//
//
//
//

CzUITextView::CzUITextView() : CzUILabel(), Zoom(1.0f), MinZoom(1.f), MaxZoom(1.0f), PrevZoom(0)
{
	setActualClassType("textview");
	PreviousDist = 1.0f;
	ZoomValid = false;
	CompoundScale = 1.0f;
	CompoundPosition.x = 0;
	CompoundPosition.y = 0;
	PanVelocity.x = 0;
	PanVelocity.y = 0;
	NumTouches = 0;
	TouchIndices[0] = -1;
	TouchIndices[1] = -1;
}

void CzUITextView::setArea(CzIVec2& size)
{
	Area = size;
}

void CzUITextView::setZoom(float scale)
{
	if (scale < MinZoom)
		scale = MinZoom;
	if (scale > MaxZoom)
		scale = MaxZoom;

	float scale2 = scale / CompoundScale;
	float tx = Midpoint.x - Midpoint.x * scale2;
	float ty = Midpoint.y - Midpoint.y * scale2;
	CompoundPosition.x = (scale2 * CompoundPosition.x) + tx;
	CompoundPosition.y = (scale2 * CompoundPosition.y) + ty;


	Zoom = scale;
	CompoundScale = scale;
	TextActor->setScale(Zoom);
}

bool CzUITextView::InitView(bool native_res, float min_scale, float max_scale)
{
	TextActor->setTappable(false);
#if defined(_DEBUG)
	CzString name = Name;
	name += "text";
	TextActor->setName(name.c_str());
#endif	// _DEBUG

	Area = TextActor->getSize();

	MinZoom = min_scale;
	MaxZoom = max_scale;

	return true;
}

bool CzUITextView::Update(float dt)
{
	if (!IsVisible)
		return CzUILabel::Update(dt);

	// Set clip rect for the list box
	CzVec4 rc;
	rc.x = (float)((-Size.x >> 1) + ClippingMargin.x);
	rc.y = (float)((-Size.y >> 1) + ClippingMargin.w);
	rc.z = (float)(Size.x - (ClippingMargin.x + ClippingMargin.y));
	rc.w = (float)(Size.y - (ClippingMargin.w + ClippingMargin.h));
	Visual->setClipRect(rc);

	// Get touches
	CzTouch* touch1 = CZ_INPUT->getTouch(TouchIndices[0]);
	CzTouch* touch2 = CZ_INPUT->getTouch(TouchIndices[1]);

	// Prevent dragging of control whilst manipulating the contained image
	if (touch1 != NULL || touch2 != NULL)
	{
		bool draggable = IsDraggable;
		IsDraggable = false;
		if (!CzUILabel::Update(dt))
			return false;
		IsDraggable = draggable;
	}
	else
	{
		if (!CzUILabel::Update(dt))
			return false;
	}

	PrevZoom = Zoom;

	// Convert touches to camera coords
	CzVec2 p1 = touch1 == NULL ? CzVec2(0, 0) : Scene->ScreenToCamera((float)touch1->x, (float)touch1->y);
	CzVec2 p2 = touch2 == NULL ? CzVec2(0, 0) : Scene->ScreenToCamera((float)touch2->x, (float)touch2->y);
	p1.x -= (Position.x);
	p1.y -= (Position.y);
	p2.x -= (Position.x);
	p2.y -= (Position.y);

	CzMatrix3& transform = Scene->getVirtualTransform();
	if ((touch1 != NULL && touch1->active) && (touch2 == NULL || !touch2->active))
	{
		// Handle panning
		if (InitialScrollPos.x < -10000)
			InitialScrollPos = ScrollPos;
		CzIVec2 drag_delta = CZ_INPUT->getDragDelta(0);
		if (drag_delta.x != 0 || drag_delta.y != 0)
		{
			CzMatrix3& transform2 = Visual->getFinalTransform();
			CzVec2 drag = CzVec2(((float)drag_delta.x) / transform2.m[0][0], ((float)drag_delta.y) / transform2.m[1][1]);
			if (drag.x >= -1 && drag.x <= 1)
				drag.x = 0;
			if (drag.y >= -1 && drag.y <= 1)
				drag.y = 0;
			PanVelocity.x = drag.x;
			PanVelocity.y = drag.y;
			if (drag.x != 0)
			{
				CompoundPosition.x += (float)drag.x;
				PanVelocity.x = (float)-drag.x / dt;
			}
			if (drag.y != 0)
			{
				CompoundPosition.y += (float)drag.y;
				PanVelocity.y = (float)-drag.y / dt;
			}
		}
	}
	else
	{
		InitialScrollPos.x = -999999;
		if (PanVelocity.x != 0 || PanVelocity.y != 0)
			CompoundPosition -= PanVelocity * dt;

		PanVelocity *= 0.8f;
		if (PanVelocity.x > -0.5f && PanVelocity.x < 0.5f)
			PanVelocity.x = 0;
		if (PanVelocity.y > -0.5f && PanVelocity.y < 0.5f)
			PanVelocity.y = 0;
	}
	if ((touch1 != NULL && touch1->active) && (touch2 != NULL && touch2->active))
	{
		// Handle pinch zoom
		float x1 = p1.x;
		float y1 = p1.y;
		float x2 = p2.x;
		float y2 = p2.y;
		float d = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
		float dist = 1.0f;
		if (d != 0)
			dist = CzMath::sqrt(d);

		if (dist > 0)
		{
			if (ZoomValid)
			{
				// Update scale and position
				CompoundScale = SavedCompoundScale;
				CompoundPosition = SavedCompoundPosition;

				float scale = dist / PreviousDist;
				CompoundScale = CompoundScale * scale;
				if (CompoundScale > MaxZoom)
				{
					scale = MaxZoom / SavedCompoundScale;
					CompoundScale = MaxZoom;
				}
				else
				if (CompoundScale < MinZoom)
				{
					scale = MinZoom / SavedCompoundScale;
					CompoundScale = MinZoom;
				}
				float tx = Midpoint.x - Midpoint.x * scale;
				float ty = Midpoint.y - Midpoint.y * scale;
				CompoundPosition.x = (scale * CompoundPosition.x) + tx;
				CompoundPosition.y = (scale * CompoundPosition.y) + ty;
			}
			else
			{
				// Latch start scale and position
				SavedCompoundScale = CompoundScale;
				SavedCompoundPosition = CompoundPosition;
				PreviousDist = dist;
				ZoomValid = true;
				Midpoint.x = (((x1 + x2) / 2)) / Visual->getTransform().m[0][0];
				Midpoint.y = (((y1 + y2) / 2)) / Visual->getTransform().m[1][1];
			}
		}
	}
	else
	{
		ZoomValid = false;
	}

	// Set image scale
	Zoom = CompoundScale;
	if (Zoom < MinZoom)
		Zoom = MinZoom;
	if (Zoom > MaxZoom)
		Zoom = MaxZoom;
	TextActor->setScale(Zoom);

//	setZoom(Zoom);

	// Move image to bounds
	int cx = ClippingMargin.x + ClippingMargin.y;
	int cy = ClippingMargin.w + ClippingMargin.h;
	int dx = (int)(Area.x * Zoom - Size.x + cx);
	int dy = (int)(Area.y * Zoom - Size.y + cy);
	if (dx < 0)
		dx = 0;
	if (dy < 0)
		dy = 0;
	int x1 = -dx / 2;
	int y1 = -dy / 2;
	int x2 = dx / 2;
	int y2 = dy / 2;

	if (EnableSpring)
	{
		if ((touch1 == NULL || !touch1->active) && (touch2 == NULL || !touch2->active))
		{
			if (CompoundPosition.x < x1)
				CompoundPosition.x += (x1 - CompoundPosition.x) * 0.5f;
			else
			if (CompoundPosition.x > x2)
				CompoundPosition.x += (x2 - CompoundPosition.x) * 0.5f;
			if (CompoundPosition.y < y1)
				CompoundPosition.y += (y1 - CompoundPosition.y) * 0.5f;
			else
			if (CompoundPosition.y > y2)
				CompoundPosition.y += (y2 - CompoundPosition.y) * 0.5f;
		}
	}
	else
	{
		if (CompoundPosition.x < x1)
			CompoundPosition.x = (float)x1;
		else
		if (CompoundPosition.x > x2)
			CompoundPosition.x = (float)x2;
		if (CompoundPosition.y < y1)
			CompoundPosition.y = (float)y1;
		else
		if (CompoundPosition.y > y2)
			CompoundPosition.y = (float)y2;
	}

	// Set image position
	TextActor->setPosition(CompoundPosition.x, CompoundPosition.y);

	return true;
}

bool CzUITextView::UpdateVisual()
{
	if (!CzActorImage::UpdateVisual())
		return false;

	return true;
}

void CzUITextView::NotifyBeginTouch(int index, int x, int y, bool allow_bubble)
{
	if (!Enabled)
		return;

	if (!TextActor->getVisual()->HitTest((float)x, (float)y))
	{
		CzActor::NotifyBeginTouch(index, x, y, allow_bubble);
		return;
	}

	if (NumTouches > 2)
		return;

	if (TouchIndices[0] < 0)
	{
		TouchIndices[0] = index;
		NumTouches++;
	}
	else
	if (TouchIndices[1] < 0)
	{
		TouchIndices[1] = index;
		NumTouches++;
	}

	CzActor::NotifyBeginTouch(index, x, y, allow_bubble);

	if (SelectType == SelectType_Normal)
	{
		setSelected(true);
	}
}

void CzUITextView::NotifyEndTouch(int index, int x, int y, bool allow_bubble)
{
	if (!Enabled)
		return;

	if (TouchIndices[0] == index)
	{
		TouchIndices[0] = -1;
		NumTouches--;
	}
	else
	if (TouchIndices[1] == index)
	{
		TouchIndices[1] = -1;
		NumTouches--;
	}

	CzActor::NotifyEndTouch(index, x, y, allow_bubble);

	if (SelectType == SelectType_Normal)
	{
		setSelected(false);
	}
}

/**
 @fn	int CzUITextView::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)

 @brief	Creates an instance of this class from XML

 LoadFromXoml is the main method used by all classes that can be instantiated from XOML mark-up. This method creates an instance of this class from the supplied XML node structure specified by node. 
 if parent is not NULL then the created instance will utilise the supplied parent to determine where it should live within the app. For example, if the parent is a scene then the actor will be placed 
 in that scene. if the parent is another actor then this actor will be placed into the scene of the supplied actor and linked as a child to the parent actor. If load_chlldren is true then any child 
 nodes contained within the actor will also be loaded. Note that as CzUITextView is derived from CzUILabel, CzUILabel::LoadFromXoml() will also be called to load CzUILabel specific properties. 

 @param [in]	parent		If non-null, the parent.
 @param	load_children	  	true to load child nodes.
 @param [in]	node  		If non-null, the XOML markup node that defines this object

 @return	negative value for error. if 0 then processing of further XOML will stop.
 */

int CzUITextView::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "TextView - Needs to be declared inside a scene or an actor - ", DebugInfo.c_str());
		return 0;
	}

	// Load main actor attributes
	int ret = CzUILabel::LoadFromXoml(parent, false, node);
	if (ret <= 0)
		return ret;

	// Process Imageview specific attributes
	CzString	brush_name;
	CzVec2		area = CzVec2(-100, -100);
	float		scale = 1.0f;

	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Zoom_Hash)
			scale = (*it)->getValueAsFloat();
		else
		if (name_hash == CzHashes::MinZoom_Hash)
			MinZoom = (*it)->getValueAsFloat();
		else
		if (name_hash == CzHashes::MaxZoom_Hash)
			MaxZoom = (*it)->getValueAsFloat();
	}

	InitView(true, MinZoom, MaxZoom);
	setZoom(scale);

	if (load_children)
	{
		// Process inner tags
		if (!CZ_XOML->Process(TextActor, node))
			return 0;
	}
	return 1;
}

/**
 @fn	bool CzUITextView::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the actor.

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types. Note that as 
 CzUITextView is derived from CzUILabel, all CzUILabel properties will also be checked against.

 @param	property_name		Name of the property.
 @param [out]	prop	The property data that.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUITextView::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	if (CzUILabel::getProperty(property_name, prop))
		return true;

	int err = IzXomlResource::getProperty(TextViewClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = TextViewClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "TextView - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUITextView::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the UITextView.
    
 Sets the named property of this UITextView. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzUITextView::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	if (CzUILabel::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(TextViewClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = TextViewClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "TextView - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUITextView::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced. Note that as CzUITextView is derived from CzUILabel, all CzUILabel properties will also be checked against.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in]	data	The property data.
 @param	delta				true to property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUITextView::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	if (CzUILabel::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(TextViewClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = TextViewClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "TextView - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUITextView::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates actor property from supplied animation.

 Updates the actors property that is specified by the supplied animation. When a timeline animation targets the property of an actor this method will be called by the animation to move the 
 animations frame data into its target property. Note that as CzUITextView is derived from CzUILabel, all CzUILabel properties will also be checked against.

 @param [in]	animation	If non-null, the animation.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUITextView::UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzUILabel::UpdateFromAnimation(animation))
		return true;

	int err = IzXomlResource::setProperty(TextViewClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = TextViewClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "TextView - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUITextView::UpdateBinding(unsigned int property_name, CzXomlVariable* var)

 @brief	Updates an actor property from the supplied XOML variable.

 Updates a single property from the value of the bound XOML variable. Note that as CzUITextView is derived from CzUILabel, all CzUILabel properties will also be checked against.

 @param	property_name  	Name of the property.
 @param [in,out]	var	If non-null, the variable.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUITextView::UpdateBinding(unsigned int property_name, CzXomlVariable* var)
{
	// Two way binding for Zoom (passes the value back to the variable)
	if (PrevZoom != Zoom)
	{
		// Only write to buond variable if it hasnt been changed
		if (property_name == CzHashes::Zoom_Hash && !var->Modified)
		{
			if (var->Type == VT_Float)
				var->setValue(CzString(Zoom).c_str());		// Write Value back to bound variable
			else
			if (var->Type == VT_Int)
				var->setValue(CzString((int)Zoom).c_str());	// Write Value back to bound variable
			return true;
		}
	}

	if (!var->Modified || var->getValue().isEmpty())
		return false;

	if (CzUILabel::UpdateBinding(property_name, var))
		return true;

	int err = IzXomlResource::setProperty(TextViewClassDef, this, property_name, var);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = TextViewClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "TextView - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

