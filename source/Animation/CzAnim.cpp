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

#include "CzAnim.h"
#include "CzString.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzActions.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"

// Global timeline run count (used to prevent the same timeline being updated twice in one frame by different systems, which allows sharing of timeslines across objects)
int CzAnimTimeline::GlobalTimelineRunCount = 0;

//
//
//
//
// Tween easing functions
//
//
//
float CzAnim_QuadEaseIn(float d)
{
	return d * d;
}
float CzAnim_QuadEaseOut(float d)
{
	return d * (2 - d);
}
float CzAnim_CubicEaseIn(float d)
{
	return d * d * d;
}
float CzAnim_CubicEaseOut(float d)
{
	d -= 1;
	return d * d * d + 1;
}
float CzAnim_QuarticEaseIn(float d)
{
	return d * d * d * d;
}
float CzAnim_QuarticEaseOut(float d)
{
	d -= 1;
	return -(d * d * d * d - 1);
}

//
//
//
//
//
// CzAnimFrame implementation
//
//
//
//

/**
 @fn	void CzAnimFrame::setEasing(eAnimEase effect)

 @brief	Sets the animation frames easing method.

 @param	effect	The easing method.
 */

void CzAnimFrame::setEasing(eAnimEase effect)
{
	EasingFunc = NULL;
	switch (effect)
	{
	case AT_Linear:
		break;
	case AT_QuadIn:
		EasingFunc = CzAnim_QuadEaseIn;
		break;
	case AT_QuadOut:
		EasingFunc = CzAnim_QuadEaseOut;
		break;
	case AT_CubicIn:
		EasingFunc = CzAnim_CubicEaseIn;
		break;
	case AT_CubicOut:
		EasingFunc = CzAnim_CubicEaseOut;
		break;
	case AT_QuarticIn:
		EasingFunc = CzAnim_QuarticEaseIn;
		break;
	case AT_QuarticOut:
		EasingFunc = CzAnim_QuarticEaseOut;
		break;
	}
}


//
//
//
//
//
// CzAnim implementation
//
//
//
//

/**
 @fn	void CzAnim::addFrame(CzAnimFrame* frame)

 @brief	Adds an animation frame to the animation.

 @param [in,out]	frame	If non-null, the frame.
 */

void CzAnim::addFrame(CzAnimFrame* frame)
{
	Frames.push_back(frame);
}

/**
 @fn	void CzAnim::CalculateCurrentData(float current_Time, CzAnimFrame* current_data,
    bool interpolate)

 @brief	Calculates the current frame data.

 This method is called internally by the animation systen to calculate the animations current frame data from its current and next animation frames, taking into account the 
 elapsed time, interpolation and tweening effect.

 @param	current_Time				The current time.
 @param [in,out]	current_data	If non-null, current anim frame.
 @param	interpolate					true to interpolate animation frames.
 */

void CzAnim::CalculateCurrentData(float current_Time, CzAnimFrame* current_data, bool interpolate)
{
	if (current_data == NULL)
		return;

	CzAnimFrame* current = NULL;
	CzAnimFrame* next = NULL;

	if (!getCurrentFrame(&current, &next, current_Time))
		return;

	float dt2 = next->Time - current->Time;
	float dt = (current_Time - current->Time) / dt2;
	CzTweenEffect easing = current->EasingFunc;

	if (easing != NULL)
		dt = easing(dt);

	switch (Type)
	{
	case CzAnimFrame::FT_Bool:
		{
			CzAnimFrameBool* c = (CzAnimFrameBool*)current;
			((CzAnimFrameBool*)current_data)->data = c->data;
		}
		break;
	case CzAnimFrame::FT_Float:
		{
			CzAnimFrameFloat* c = (CzAnimFrameFloat*)current;
			CzAnimFrameFloat* n = (CzAnimFrameFloat*)next;
			if (c == n || !interpolate)
				((CzAnimFrameFloat*)current_data)->data = c->data;
			else
			{
				float d = n->data - c->data;
				((CzAnimFrameFloat*)current_data)->data = c->data + dt * d;
			}
		}
		break;
	case CzAnimFrame::FT_Vec2:
		{
			CzAnimFrameVec2* c = (CzAnimFrameVec2*)current;
			CzAnimFrameVec2* n = (CzAnimFrameVec2*)next;
			if (c == n || !interpolate)
				((CzAnimFrameVec2*)current_data)->data = c->data;
			else
			{
				float dx = n->data.x - c->data.x;
				float dy = n->data.y - c->data.y;
				((CzAnimFrameVec2*)current_data)->data.x = c->data.x + dt * dx;
				((CzAnimFrameVec2*)current_data)->data.y = c->data.y + dt * dy;
			}
		}
		break;
	case CzAnimFrame::FT_Vec3:
		{
			CzAnimFrameVec3* c = (CzAnimFrameVec3*)current;
			CzAnimFrameVec3* n = (CzAnimFrameVec3*)next;
			if (c == n || !interpolate)
				((CzAnimFrameVec3*)current_data)->data = c->data;
			else
			{
				float dx = n->data.x - c->data.x;
				float dy = n->data.y - c->data.y;
				float dz = n->data.z - c->data.z;
				((CzAnimFrameVec3*)current_data)->data.x = c->data.x + dt * dx;
				((CzAnimFrameVec3*)current_data)->data.y = c->data.y + dt * dy;
				((CzAnimFrameVec3*)current_data)->data.z = c->data.z + dt * dz;
			}
		}
		break;
	case CzAnimFrame::FT_Vec4:
		{
			CzAnimFrameVec4* c = (CzAnimFrameVec4*)current;
			CzAnimFrameVec4* n = (CzAnimFrameVec4*)next;
			if (c == n || !interpolate)
				((CzAnimFrameVec4*)current_data)->data = c->data;
			else
			{
				float dx = n->data.x - c->data.x;
				float dy = n->data.y - c->data.y;
				float dz = n->data.z - c->data.z;
				float dw = n->data.w - c->data.w;
				((CzAnimFrameVec4*)current_data)->data.x = (int16)(c->data.x + dt * dx);
				((CzAnimFrameVec4*)current_data)->data.y = (int16)(c->data.y + dt * dy);
				((CzAnimFrameVec4*)current_data)->data.z = (int16)(c->data.z + dt * dz);
				((CzAnimFrameVec4*)current_data)->data.w = (int16)(c->data.w + dt * dw);
			}
		}
		break;
	case CzAnimFrame::FT_Rect:
		{
			CzAnimFrameRect* c = (CzAnimFrameRect*)current;
			CzAnimFrameRect* n = (CzAnimFrameRect*)next;
			((CzAnimFrameRect*)current_data)->data = c->data;
		}
		break;
	case CzAnimFrame::FT_String:
		{
			CzAnimFrameString* c = (CzAnimFrameString*)current;
			((CzAnimFrameString*)current_data)->data = c->data;
		}
		break;
	}
}

/**
 @fn	bool CzAnim::getCurrentFrame(CzAnimFrame** current, CzAnimFrame** next,
    float current_Time)

 @brief	Gets current frame.

 @author	Mat
 @date	14/01/2013

 @param [in,out]	current	If non-null, the current.
 @param [in,out]	next   	If non-null, the next.
 @param	current_Time	   	The current time.

 @return	true if it succeeds, false if it fails.
 */

bool CzAnim::getCurrentFrame(CzAnimFrame** current, CzAnimFrame** next, float current_Time)
{
	int size = Frames.size();
	if (size == 0)
		return false;				// Return false if no frames exist
	if (size < 2)
	{
		*current = *(Frames.begin());	// Return first frame if only one frame exists
		*next = *current;
		return true;
	}

	_Iterator prev = Frames.begin();
	for (_Iterator it = Frames.begin(); it != Frames.end(); it++)
	{
		if (current_Time < (*it)->Time)
		{
			*current = *prev;
			*next = *it;
			return true;
		}
		prev = it;
	}

	*current = *prev;
	*next = *prev;

	return true;	// return last frame
}

/**
 @fn	void CzAnim::GenerateAtlasFrames(int count, int frame_w, int frame_h, int start_x,
    int start_y, int pitch_x, int pitch_y, int image_width, float frame_duration)

 @brief	Generates a group of rect image atlas frames.

 @param	count		  	Number of frames to generate.
 @param	frame_w		  	The frame width.
 @param	frame_h		  	The frame height.
 @param	start_x		  	The start x coordinate.
 @param	start_y		  	The start y coordinate.
 @param	pitch_x		  	The x pitch.
 @param	pitch_y		  	The y pitch.
 @param	image_width   	Width of the image.
 @param	frame_duration	Duration of each frame.
 */

void CzAnim::GenerateAtlasFrames(int count, int frame_w, int frame_h, int start_x, int start_y, int pitch_x, int pitch_y, int image_width, float frame_duration)
{
	setType(CzAnimFrame::FT_Rect);

	int			x = start_x; 
	int			y = start_y;
	float		time = 0;

	while (count != 0)
	{
		CzAnimFrameRect* frame = new CzAnimFrameRect();

		// Set frames data
		frame->data.x = x;
		frame->data.y = y;
		frame->data.w = frame_w;
		frame->data.h = frame_h;
		frame->Time = time;

		// Set to next frame horizontally
		x += pitch_x;
		if (x >= image_width)
		{
			// Step down to next line of frames
			y += pitch_y;
			x = start_x;
		}

		addFrame(frame);

		count--;
		time += frame_duration;
	}
}

/**
 @fn	int CzAnim::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)

 @brief	Creates an instance of this class from XOML.

 LoadFromXoml is the main method used by all classes that can be instantiated from XOML mark-up. This method creates an instance of this class from the supplied XML node structure specified by node. 
 
 @param [in]	parent		If non-null, the parent.
 @param	load_children	  	true to load child nodes.
 @param [in]	node  		If non-null, the XOML markup node that defines this object

 @return	nergative value for error. if 0 then processing of further XOML will stop.
 */

int CzAnim::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Process animation attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
			setName((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Type_Hash)
		{
			unsigned int type_hash = CzString::CalculateHash((*it)->getValue().c_str());
			if (type_hash == CzHashes::bool_Hash)
				setType(CzAnimFrame::FT_Bool);
			else
			if (type_hash == CzHashes::float_Hash)
				setType(CzAnimFrame::FT_Float);
			else
			if (type_hash == CzHashes::vec2_Hash)
				setType(CzAnimFrame::FT_Vec2);
			else
			if (type_hash == CzHashes::vec3_Hash)
				setType(CzAnimFrame::FT_Vec3);
			else
			if (type_hash == CzHashes::vec4_Hash)
				setType(CzAnimFrame::FT_Vec4);
			else
			if (type_hash == CzHashes::rect_Hash)
				setType(CzAnimFrame::FT_Rect);
			else
			if (type_hash == CzHashes::string_Hash)
				setType(CzAnimFrame::FT_String);
		}
		else
		if (name_hash == CzHashes::Duration_Hash)
		{
			setDuration((*it)->getValueAsFloat());
		}
		else
		if (name_hash == CzHashes::AutoDelete_Hash)
			AutoDelete = (*it)->getValueAsBool();
	}

	if (Type == CzAnimFrame::FT_Invalid)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Anim - Invalid animation type for animation", node->getParserFileName(), node->Line);
		return 0;
	}

	// Prrocess anim frames
	CzAnimFrame* current_frame = NULL;
	for (CzXmlNode::_Iterator it2 = node->begin(); it2 != node->end(); ++it2)
	{
		unsigned int name_hash = (*it2)->GetName().getHash();
		if (name_hash == CzHashes::Atlas_Hash)
		{
			int			count = 0;
			CzVec2		size = CzVec2(0, 0);
			CzVec2		pos = CzVec2(0, 0);
			CzVec2		pitch = CzVec2(0, 0);
			int			width = 0;
			float		duration = 0.1f;

			for (CzXmlNode::_AttribIterator it = (*it2)->attribs_begin(); it != (*it2)->attribs_end(); ++it)
			{
				unsigned int attrib_hash = (*it)->getName().getHash();

				if (attrib_hash == CzHashes::Count_Hash)
					count = (*it)->getValueAsInt();
				else
				if (attrib_hash == CzHashes::Size_Hash)
				{
					if (!(*it)->getValueAsPoint(size))
						CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Anim - Invalid value for Atlas::Size, expected vec2", (*it2)->getParserFileName(), (*it2)->Line);
				}
				else
				if (attrib_hash == CzHashes::Position_Hash)
				{
					if (!(*it)->getValueAsPoint(pos))
						CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Anim - Invalid value for Atlas::Position, expected vec2", (*it2)->getParserFileName(), (*it2)->Line);
				}
				else
				if (attrib_hash == CzHashes::Pitch_Hash)
				{
					if (!(*it)->getValueAsPoint(pitch))
						CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Anim - Invalid value for Atlas::Pitch, expected vec2", (*it2)->getParserFileName(), (*it2)->Line);
				}
				else
				if (attrib_hash == CzHashes::Width_Hash)
					width = (*it)->getValueAsInt();
				else
				if (attrib_hash == CzHashes::Duration_Hash)
					duration = (*it)->getValueAsFloat();
			}

			if (count != 0)
				GenerateAtlasFrames(count, (int)size.x, (int)size.y, (int)pos.x, (int)pos.y, (int)pitch.x, (int)pitch.y, width, duration);
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Anim - Atlas frame count cannot be 0", (*it2)->getParserFileName(), (*it2)->Line);

		}
		else
		if (name_hash == CzHashes::Frame_Hash)
		{
			// Get frame values
			int index = 0;
			CzXmlAttribute* value = NULL;
			CzXmlAttribute* time = NULL;
			CzXmlAttribute* easing = NULL;

			for (CzXmlNode::_AttribIterator it = (*it2)->attribs_begin(); it != (*it2)->attribs_end(); ++it)
			{
				unsigned int attrib_hash = (*it)->getName().getHash();

				if (attrib_hash == CzHashes::Value_Hash)
					value = *it;
				else
				if (attrib_hash == CzHashes::Time_Hash)
					time = *it;
				else
				if (attrib_hash == CzHashes::Easing_Hash)
					easing = *it;
			}
			if (value != NULL && time != NULL)
			{
				// Create animation frame
				switch (Type)
				{
				case CzAnimFrame::FT_Bool:
					{
						CzAnimFrameBool* frame = new CzAnimFrameBool();
						current_frame = frame;
						addFrame(frame);
						frame->data = value->getValueAsBool();
						break;
					}
				case CzAnimFrame::FT_Float:
					{
						CzAnimFrameFloat* frame = new CzAnimFrameFloat();
						current_frame = frame;
						addFrame(frame);
						frame->data = value->getValueAsFloat();
						break;
					}
					break;
				case CzAnimFrame::FT_Vec2:
					{
						CzAnimFrameVec2* frame = new CzAnimFrameVec2();
						current_frame = frame;
						addFrame(frame);
						if (!value->getValueAsPoint(frame->data))
						{
							CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Anim - Invalid value for Frame::Values, expected vec2", (*it2)->getParserFileName(), (*it2)->Line);
							return 0;
						}
					}
					break;
				case CzAnimFrame::FT_Vec3:
					{
						CzAnimFrameVec3* frame = new CzAnimFrameVec3();
						current_frame = frame;
						addFrame(frame);
						if (!value->getValueAsPoint3(frame->data))
						{
							CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Anim - Invalid value for Frame::Values, expected vec3", (*it2)->getParserFileName(), (*it2)->Line);
							return 0;
						}
					}
					break;
				case CzAnimFrame::FT_Vec4:
					{
						CzAnimFrameVec4* frame = new CzAnimFrameVec4();
						current_frame = frame;
						addFrame(frame);
						if (!value->getValueAsPoint4(frame->data))
						{
							CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Anim - Invalid value for Frame::Values, expected vec4", (*it2)->getParserFileName(), (*it2)->Line);
							return 0;
						}
					}
					break;
				case CzAnimFrame::FT_Rect:
					{
						CzAnimFrameRect* frame = new CzAnimFrameRect();
						current_frame = frame;
						addFrame(frame);
						if (!value->getValueAsRect(frame->data))
						{
							CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Anim - Invalid value for Frame::Values, expected rect", (*it2)->getParserFileName(), (*it2)->Line);
							return 0;
						}
					}
					break;
				case CzAnimFrame::FT_String:
					{
						CzAnimFrameString* frame = new CzAnimFrameString();
						current_frame = frame;
						addFrame(frame);
						frame->data = value->getValue();
						break;
					}
					break;
				default:
					CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Anim - Frame type not supported", (*it2)->getParserFileName(), (*it2)->Line);
					return 0;
				}

				// Set current time
				current_frame->Time = time->getValueAsFloat();

				// Set easing
				if (easing != NULL)
				{
					unsigned int ease_hash = easing->getValue().getHash();
					if (ease_hash == CzHashes::Linear_Hash)
						current_frame->setEasing(CzAnimFrame::AT_Linear);
					else
					if (ease_hash == CzHashes::QuadIn_Hash)
						current_frame->setEasing(CzAnimFrame::AT_QuadIn);
					else
					if (ease_hash == CzHashes::QuadOut_Hash)
						current_frame->setEasing(CzAnimFrame::AT_QuadOut);
					else
					if (ease_hash == CzHashes::CubicIn_Hash)
						current_frame->setEasing(CzAnimFrame::AT_CubicIn);
					else
					if (ease_hash == CzHashes::CubicOut_Hash)
						current_frame->setEasing(CzAnimFrame::AT_CubicOut);
					else
					if (ease_hash == CzHashes::QuarticIn_Hash)
						current_frame->setEasing(CzAnimFrame::AT_QuarticIn);
					else
					if (ease_hash == CzHashes::QuarticOut_Hash)
						current_frame->setEasing(CzAnimFrame::AT_QuarticOut);
					else
					{
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Anim - Easing type not found, defaulting to linear - ", easing->getValue().c_str(), (*it2)->getParserFileName(), (*it2)->Line);
					}
				}
			}
		}
	}

	// If we are declared inside a scene then animation is local to the scene
	CzScene* scene = NULL;
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Actor_Hash)
		scene = ((CzActor*)parent)->getScene();
	else
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

	if (scene != NULL)
		return scene->getResourceManager()->addResource(this) ? 1 : 0;
	else
		return CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this) ? 1 : 0;

	return 1;
}

//
//
//
//
// CzAnimInstance implementation
//
//
//
//

CzAnimInstance::~CzAnimInstance()
{
	// Delete events manager
	SAFE_DELETE(EventsManager)
	// Delete data
	SAFE_DELETE(CurrentData)
}


/**
 @fn	void CzAnimInstance::setAnimation(CzAnim* anim)

 @brief	Assigns a CzAnim animation to this animation instance

 @param [in]	anim	If non-null, the animation.
 */

void CzAnimInstance::setAnimation(CzAnim* anim)
{
	if (anim != NULL)
	{
		switch (anim->getType())
		{
		case CzAnimFrame::FT_Bool:
			CurrentData = new CzAnimFrameBool();
			break;
		case CzAnimFrame::FT_Float:
			CurrentData = new CzAnimFrameFloat();
			break;
		case CzAnimFrame::FT_Vec2:
			CurrentData = new CzAnimFrameVec2();
			break;
		case CzAnimFrame::FT_Vec3:
			CurrentData = new CzAnimFrameVec3();
			break;
		case CzAnimFrame::FT_Vec4:
			CurrentData = new CzAnimFrameVec4();
			break;
		case CzAnimFrame::FT_Rect:
			CurrentData = new CzAnimFrameRect();
			break;
		case CzAnimFrame::FT_String:
			CurrentData = new CzAnimFrameString();
			break;
		}
	}
	Animation = anim;
}

void CzAnimInstance::addEventsManager()
{
	if (EventsManager == NULL)
		EventsManager = new CzEventManager();
}

/**
 @fn	void CzAnimInstance::Update(float dt, IzAnimTarget* target)

 @brief	Updates the animation instance.

 This method updates this animation instance, including:
 - Calculating new animation frame data
 - Updating target object properties from the new frame data
 - Starting, repeating or stopping the instance

 @param	dt	The number of seconds since the app was last updated
 @param [in,out]	target	If non-null, the target.
 @return	true if aimation is still playing, false if finished playing
 */

bool CzAnimInstance::Update(float dt, IzAnimTarget* target)
{
	if (Started && Status == CzAnim::AS_Stopped)
		return false;

	if (Status != CzAnim::AS_Playing || Animation == NULL)
		return true;

	IzAnimTarget* current_target = (target == NULL) ? Target : target;

//	dt = (dt * FRAME_SPEED_LOCK_MS) / 1000.0f;	// Convert to seconds

	if (RepeatCount != 0)
	{
		if (RepeatedCount >= RepeatCount)
		{
			Status = CzAnim::AS_Stopped;

			// Notify user that the animation has stopped
			NotifyEnd();
			return false;
		}
	}

	if (!DelayExpired && Delay >= 0)
	{
		CurrentTime += dt;
		if (CurrentTime >= Delay)
		{
			DelayExpired = true;
			CurrentTime = 0;

			// Notify the user know that the animation has started
			NotifyStart();
		}
		else
			return true;
	}

	float duration = Animation->getDuration();
	CurrentTime += dt;
	if (CurrentTime >= duration)
	{
		if (RepeatCount != 0)
		{
			RepeatedCount++;
			if (RepeatedCount >= RepeatCount)
			{
				Animation->CalculateCurrentData(CurrentTime, CurrentData, Interpolate);
				CurrentDataValid = true;
				if (current_target != NULL)
					current_target->UpdateFromAnimation(this);

				return true;
			}
		}

		// Notify the user that the animation has looped
		NotifyRepeat();

		CurrentTime -= duration;
	}

	Animation->CalculateCurrentData(CurrentTime, CurrentData, Interpolate);
	CurrentDataValid = true;

	if (current_target != NULL)
		current_target->UpdateFromAnimation(this);

	return true;
}

/**
 @fn	void CzAnimInstance::UpdateTargetOnly(IzAnimTarget* target)

 @brief	Updates the instances target only.

 This method updates this animation instances target only.

 @param [in,out]	target	If non-null, the target.
 */

void CzAnimInstance::UpdateTargetOnly(IzAnimTarget* target)
{
	if (Status != CzAnim::AS_Playing)
		return;

	if (target == NULL)
		target = Target;

	if (target != NULL && CurrentData != NULL)
	{
		if (!CurrentDataValid)
		{
			Animation->CalculateCurrentData(CurrentTime, CurrentData, Interpolate);
			CurrentDataValid = true;
		}
		target->UpdateFromAnimation(this);
	}
}

/**
 @fn	void CzAnimInstance::ForceUpdateTargetOnly(IzAnimTarget* target)

 @brief	Updates the instances target only.

 This method force updates this animation instances target only, even if th animation is not playing.

 @param [in,out]	target	If non-null, the target.
 */

void CzAnimInstance::ForceUpdateTargetOnly(IzAnimTarget* target)
{
	if (target == NULL)
		target = Target;

	if (target != NULL && CurrentData != NULL)
	{
		if (!CurrentDataValid)
		{
			Animation->CalculateCurrentData(CurrentTime, CurrentData, Interpolate);
			CurrentDataValid = true;
		}
		target->UpdateFromAnimation(this);
	}
}

/**
 @fn	void CzAnimInstance::ProcessEventActions(unsigned int event_name)

 @brief	Process the event actions described by event_name.

 Processes the actions list that is associated with the event named event_name (event_name is a hash of the actual event name to speed up searches)

 @param	event_name	Name of the event as a string hash
 */

void CzAnimInstance::ProcessEventActions(unsigned int event_name)
{
	if (EventsManager == NULL)
		return;

	// Find the event
	CzEvent* evt = EventsManager->findEvent(event_name);
	if (evt != NULL)
	{
		// Find actions list
		IzXomlResource* parent = Parent->getParent();
//		if (parent->getClassTypeHash() != CzHashes::Actor_Hash && parent->getClassTypeHash() != CzHashes::Scene_Hash)
//		{
//		}
		CzActions* actions = CzActionsManager::FindActions(evt->ActionsName, parent);
		if (actions != NULL)
			actions->Execute(parent);
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Anim - Action not found - ", evt->_ActionsName.c_str());
	}
}

void CzAnimInstance::NotifyStart()
{
	ProcessEventActions(CzHashes::OnStart_Hash);
	if (StartedCallback != NULL)
		StartedCallback((void*)this, NULL);
}

void CzAnimInstance::NotifyEnd()
{
	ProcessEventActions(CzHashes::OnEnd_Hash);
	if (StoppedCallback != NULL)
		StoppedCallback((void*)this, NULL);
}

void CzAnimInstance::NotifyRepeat()
{
	ProcessEventActions(CzHashes::OnRepeat_Hash);
	if (LoopedCallback != NULL)
		LoopedCallback((void*)this, NULL);
}

//
//
//
//
// CzAnimTimeline implementation
//
//
//
//

CzAnimTimeline::~CzAnimTimeline()
{
	for (_Iterator it = begin(); it != end(); it++)
	{
		delete (*it);
	}
	Animations.clear();
}


/**
 @fn	CzAnimInstance* CzAnimTimeline::findAnimation(const char* name)

 @brief	Searches for the named animation.

 @param	name	The animation name.

 @return	null if it fails, else the found animation.
 */

CzAnimInstance* CzAnimTimeline::findAnimation(const char* name)
{
	return findAnimation(CzString::CalculateHash(name));
}

/**
 @fn	CzAnimInstance* CzAnimTimeline::findAnimation(unsigned int name_hash)

 @brief	Searches for the named animation.

 @param	name	The animation name as a string hash.


 @return	null if it fails, else the found animation.
 */

CzAnimInstance* CzAnimTimeline::findAnimation(unsigned int name_hash)
{
	for (_Iterator it = begin(); it != end(); it++)
	{
		if ((*it)->getAnimation()->getNameHash() == name_hash)
			return *it;
	}

	return NULL;
}

/**
 @fn	void CzAnimTimeline::play()

 @brief	Plays all animations in the animation instance
 */

void CzAnimTimeline::play()
{
	for (_Iterator it = begin(); it != end(); it++)
	{
		(*it)->play();
	}
}

/**
 @fn	void CzAnimTimeline::stop()

 @brief	Stops all animations in the animation instance
 */

void CzAnimTimeline::stop()
{
	for (_Iterator it = begin(); it != end(); it++)
	{
		(*it)->stop();
	}
}

/**
 @fn	void CzAnimTimeline::pause()

 @brief	Pausesthe playback of all animations in the animation instance
 */

void CzAnimTimeline::pause()
{
	for (_Iterator it = begin(); it != end(); it++)
	{
		(*it)->pause();
	}
}

/**
 @fn	void CzAnimTimeline::restart()

 @brief	Restarts all animations in the animation instance
 */

void CzAnimTimeline::restart()
{
	for (_Iterator it = begin(); it != end(); it++)
	{
		(*it)->restart();
	}
//	UpdateTargetOnly();
}

/**
 @fn	bool CzAnimTimeline::isStopped()

 @brief	Query if all animations within the instance have stopped playing.

 @return	true if stopped, false if not.
 */

bool CzAnimTimeline::isStopped()
{
	for (_Iterator it = begin(); it != end(); it++)
	{
		if (!(*it)->isStopped())
			return false;
	}

	return true;
}

/**
 @fn	bool CzAnimTimeline::isPlaying()

 @brief	Query if any animations within the instance are still playing.

 @return	true if playing, false if not.
 */

bool CzAnimTimeline::isPlaying()
{
	for (_Iterator it = begin(); it != end(); it++)
	{
		if ((*it)->isPlaying())
			return true;
	}

	return false;
}

/**
 @fn	void CzAnimTimeline::ForceUpdateTargetOnly(IzAnimTarget* target)

 @brief	Force update all animations in the timeline (targets only).

 This method force updates all animation instance targets in this timeline, even if the animation is not playing.

 @param [in,out]	target	If non-null, the target.
 */

void CzAnimTimeline::ForceUpdateTargetOnly(IzAnimTarget* target)
{
	for (_Iterator it = begin(); it != end(); it++)
		(*it)->ForceUpdateTargetOnly(target);
}

/**
 @fn	void CzAnimTimeline::UpdateTargetOnly(IzAnimTarget* target)

 @brief	Updates all animations in the timeline (targets only).

 This method updates all animation instance targets only.

 @param [in,out]	target	If non-null, the target.
 */

void CzAnimTimeline::UpdateTargetOnly(IzAnimTarget* target)
{
	for (_Iterator it = begin(); it != end(); it++)
		(*it)->UpdateTargetOnly(target);
}

/**
 @fn	void CzAnimTimeline::Update(float time_delta, IzAnimTarget* target)

 @brief	Updates all animations in this timeline.

 @param	time_delta		  	The time delta.
 @param [in,out]	target	If non-null, the target.
 @return	true if any animation instances are still playing, false if all have finished playing
 */

bool CzAnimTimeline::Update(float time_delta, IzAnimTarget* target)
{
	// Prevents timeline from being more than once per game loop
	if (TimelineRunCount == GlobalTimelineRunCount)
	{
		UpdateTargetOnly(target);
		return true;
	}

	bool not_done = false;
	for (_Iterator it = begin(); it != end(); it++)
		not_done |= (*it)->Update(time_delta * TimeScale, target);

	TimelineRunCount = GlobalTimelineRunCount;

	// If auto delete set then remove destroy the timeline
	if (AutoDelete && !not_done)
		Remove();

	return not_done;
}

/**
 @fn	int CzAnimTimeline::LoadFromXoml(IzXomlResource* parent, bool load_children,
    CzXmlNode* node)

 @brief	Creates an instance of this class from XOML.

 LoadFromXoml is the main method used by all classes that can be instantiated from XOML mark-up. This method creates an instance of this class from the supplied XML node structure specified by node. 
 
 @param [in]	parent		If non-null, the parent.
 @param	load_children	  	true to load child nodes.
 @param [in]	node  		If non-null, the XOML markup node that defines this object

 @return	nergative value for error. if 0 then processing of further XOML will stop.
 */

int CzAnimTimeline::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Process animation attributes
	bool auto_play = false;
	bool local = true;

	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
			setName((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::AutoPlay_Hash)
			auto_play = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::TimeScale_Hash)
			setTimeScale((*it)->getValueAsFloat());
		else
		if (name_hash == CzHashes::AutoDelete_Hash)
			AutoDelete = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Local_Hash)
			local = (*it)->getValueAsBool();
	}

	CzScene* scene = NULL;
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Actor_Hash)
		scene = ((CzActor*)parent)->getScene();
	else
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

	// Calculate number of animations
	int num_animations = node->Children.size();

	// Prrocess animations
	for (CzXmlNode::_Iterator it2 = node->begin(); it2 != node->end(); ++it2)
	{
		bool valid_anim = false;
		CzAnimInstance* anim_instance = new CzAnimInstance();
		addAnimation(anim_instance);

		unsigned int name_hash = (*it2)->GetName().getHash();
		if (name_hash == CzHashes::Animation_Hash || name_hash == CzHashes::Anim_Hash)
		{
			for (CzXmlNode::_AttribIterator it = (*it2)->attribs_begin(); it != (*it2)->attribs_end(); ++it)
			{
				unsigned int attrib_hash = (*it)->getName().getHash();

				if (attrib_hash == CzHashes::Anim_Hash)
				{
					// Search for the animation
					CzAnim* anim = NULL;
					if (scene != NULL)
						anim = (CzAnim*)scene->getResourceManager()->findResource((*it)->getValue().c_str(), CzHashes::Animation_Hash);
					else
						anim = (CzAnim*)CZ_GLOBAL_RESOURCES->getResourceManager()->findResource((*it)->getValue().c_str(), CzHashes::Animation_Hash);
					if (anim != NULL)
						valid_anim = true;
					else
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Timeline - Could not find named animation in the scene or in the global resources - ", (*it)->getValue().c_str(), (*it2)->getParserFileName(), (*it2)->Line);

					anim_instance->setAnimation(anim);
				}
				else
				if (attrib_hash == CzHashes::Target_Hash)
					anim_instance->setTarget(NULL, (*it)->getValue().c_str());
				else
				if (attrib_hash == CzHashes::Repeat_Hash)
					anim_instance->setRepeatCount((*it)->getValueAsInt());
				else
				if (attrib_hash == CzHashes::StartAtTime_Hash)
					anim_instance->setDelay((*it)->getValueAsFloat());
				else
				if (attrib_hash == CzHashes::Delta_Hash)
					anim_instance->setDelta((*it)->getValueAsBool());
				else
				if (attrib_hash == CzHashes::Interpolate_Hash)
					anim_instance->setInterpolate((*it)->getValueAsBool());
				else
				if (attrib_hash == CzHashes::OnStart_Hash)
				{
					anim_instance->addEventsManager();
					anim_instance->getEventsManager()->addEvent("OnStart", (*it)->getValue().c_str(), true);
				}
				else
				if (attrib_hash == CzHashes::OnEnd_Hash)
				{
					anim_instance->addEventsManager();
					anim_instance->getEventsManager()->addEvent("OnEnd", (*it)->getValue().c_str(), true);
				}
				else
				if (attrib_hash == CzHashes::OnRepeat_Hash)
				{
					anim_instance->addEventsManager();
					anim_instance->getEventsManager()->addEvent("OnRepeat", (*it)->getValue().c_str(), true);
				}
			}
		}
		if (!valid_anim)
			removeAnimation(anim_instance);
	}

	// If we are declared inside a scene or actor then timer is local to it
	setParent(parent);
	if (parent != NULL && (parent->getClassTypeHash() == CzHashes::Scene_Hash || parent->getClassTypeHash() == CzHashes::Actor_Hash))
	{
		if (parent->getClassTypeHash() == CzHashes::Scene_Hash)
			((CzScene*)parent)->getTimelinesManager()->addTimeline(this);
		else
		{
			if (local)
				((CzActor*)parent)->getTimelinesManager()->addTimeline(this);
			else
				((CzActor*)parent)->getScene()->getTimelinesManager()->addTimeline(this);
		}
	}
	else
	{
		CZ_GLOBAL_RESOURCES->getTimelinesManager()->addTimeline(this);
	}


	if (auto_play)
		play();

	return 1;
}

void CzAnimTimeline::Remove()
{
	// Delete animations if they are set to auto delete
	for (_Iterator it = begin(); it != end(); it++)
	{
		CzAnim* anim = (*it)->getAnimation();
		if (anim != NULL)
		{
			if (anim->isAutoDelete())
				anim->Remove();
		}
	}

	// Delete timeline
	if (Manager != NULL)
		Manager->removeTimeline(this);
	else
		delete this;
}


//
//
//
//
// CzAnimUtil implementation
//
//
// 
//
CzAnim* CzAnimUtil::CreateVec2Anim(const char* name, const CzVec2& source, const CzVec2& target, float duration, CzAnimFrame::eAnimEase easing, int category)
{
	CzAnim* anim = new CzAnim();
	anim->setName(name);
	anim->setCategory(category);
	anim->setDuration(duration);
	anim->setManaged(true);
	anim->setType(CzAnimFrame::FT_Vec2);

	CzAnimFrameVec2* frame = new CzAnimFrameVec2();
	frame->data.x = source.x;
	frame->data.y = source.y;
	frame->Time = 0;
	frame->setEasing(easing);
	anim->addFrame(frame);

	frame = new CzAnimFrameVec2();
	frame->data.x = target.x;
	frame->data.y = target.y;
	frame->Time = duration;
	anim->addFrame(frame);

	return anim;
}
CzAnim* CzAnimUtil::CreateVec3Anim(const char* name, const CzVec3& source, const CzVec3& target, float duration, CzAnimFrame::eAnimEase easing, int category)
{
	CzAnim* anim = new CzAnim();
	anim->setName(name);
	anim->setCategory(category);
	anim->setDuration(duration);
	anim->setManaged(true);
	anim->setType(CzAnimFrame::FT_Vec3);

	CzAnimFrameVec3* frame = new CzAnimFrameVec3();
	frame->data.x = source.x;
	frame->data.y = source.y;
	frame->data.z = source.z;
	frame->Time = 0;
	frame->setEasing(easing);
	anim->addFrame(frame);

	frame = new CzAnimFrameVec3();
	frame->data.x = target.x;
	frame->data.y = target.y;
	frame->data.z = target.z;
	frame->Time = duration;
	anim->addFrame(frame);

	return anim;
}
CzAnim* CzAnimUtil::CreateVec4Anim(const char* name, const CzVec4& source, const CzVec4& target, float duration, CzAnimFrame::eAnimEase easing, int category)
{
	CzAnim* anim = new CzAnim();
	anim->setName(name);
	anim->setCategory(category);
	anim->setDuration(duration);
	anim->setManaged(true);
	anim->setType(CzAnimFrame::FT_Vec4);

	CzAnimFrameVec4* frame = new CzAnimFrameVec4();
	frame->data.x = source.x;
	frame->data.y = source.y;
	frame->data.z = source.z;
	frame->data.w = source.w;
	frame->Time = 0;
	frame->setEasing(easing);
	anim->addFrame(frame);

	frame = new CzAnimFrameVec4();
	frame->data.x = target.x;
	frame->data.y = target.y;
	frame->data.z = target.z;
	frame->data.w = target.w;
	frame->Time = duration;
	anim->addFrame(frame);

	return anim;
}
CzAnim* CzAnimUtil::CreateFloatAnim(const char* name, float source, float target, float duration, CzAnimFrame::eAnimEase easing, int category)
{
	CzAnim* anim = new CzAnim();
	anim->setName(name);
	anim->setCategory(category);
	anim->setDuration(duration);
	anim->setManaged(true);
	anim->setType(CzAnimFrame::FT_Float);

	CzAnimFrameFloat* frame = new CzAnimFrameFloat();
	frame->data = source;
	frame->Time = 0;
	frame->setEasing(easing);
	anim->addFrame(frame);

	frame = new CzAnimFrameFloat();
	frame->data = target;
	frame->Time = duration;
	anim->addFrame(frame);

	return anim;
}
CzAnim* CzAnimUtil::CreateRectAnim(const char* name, const CzIRect& source, const CzIRect& target, float duration, CzAnimFrame::eAnimEase easing, int category)
{
	CzAnim* anim = new CzAnim();
	anim->setName(name);
	anim->setCategory(category);
	anim->setDuration(duration);
	anim->setManaged(true);
	anim->setType(CzAnimFrame::FT_Vec4);

	CzAnimFrameRect* frame = new CzAnimFrameRect();
	frame->data.x = source.x;
	frame->data.y = source.y;
	frame->data.w = source.w;
	frame->data.h = source.h;
	frame->Time = 0;
	frame->setEasing(easing);
	anim->addFrame(frame);

	frame = new CzAnimFrameRect();
	frame->data.x = target.x;
	frame->data.y = target.y;
	frame->data.w = target.w;
	frame->data.h = target.h;
	frame->Time = duration;
	anim->addFrame(frame);

	return anim;
}

CzAnimTimeline* CzAnimUtil::CreateTimeline(const char* name, CzAnim* anim, IzAnimTarget* target, const char* target_property, int repeat, bool delta, float start_at_time, bool interpolate)
{
	CzAnimTimeline* timeline = new CzAnimTimeline();
	timeline->setName(name);
	timeline->setManaged(false);

	CzAnimInstance* inst = new CzAnimInstance();
	inst->setAnimation(anim);
	inst->setDelay(start_at_time);
	inst->setDelta(delta);
	inst->setInterpolate(interpolate);
	inst->setParent(timeline);
	inst->setTarget(target, target_property);
	inst->setRepeatCount(repeat);
	inst->setManaged(true);

	timeline->addAnimation(inst);

	return timeline;
}

// 
// 
//
//
// CzAnimTimelinesManager implementation
//
//
//
//

/**
 @fn	void CzAnimTimelinesManager::addTimeline(CzAnimTimeline* timeline)

 @brief	Adds a timeline to the timelines manager.

 @param [in,out]	timelines	If non-null, the timeline.
 */

void CzAnimTimelinesManager::addTimeline(CzAnimTimeline* timeline)
{
#if defined(CZ_REMOVE_DUPLICATES)
	CzAnimTimeline* tl = findTimeline(timeline->getNameHash());
	if (tl != NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Timeline - Already exists, timeline was replaced - ", timeline->getName().c_str());
		removeTimeline(tl);
	}
#endif
	Timelines.push_back(timeline);
	timeline->setManaged(true);
	timeline->setManager(this);
}

/**
 @fn	void CzAnimTimelinesManager::removeTimeline(CzAnimTimeline* timeline)

 @brief	Removes the timeline from the timelines manager.

 @param [in,out]	timeline	If non-null, the timeline.
 */

void CzAnimTimelinesManager::removeTimeline(CzAnimTimeline* timeline)
{
	removeTimeline(timeline->getNameHash());
}

/**
 @fn	void CzAnimTimelinesManager::removeTimeline(unsigned int name_hash)

 @brief	Removes the named timeline from the timelines manager.

 @param	name_hash	The timeline name as a string hash.
 */

void CzAnimTimelinesManager::removeTimeline(unsigned int name_hash)
{
	// Renove named Timeline from the manager
	for (_Iterator it = Timelines.begin(); it != Timelines.end(); ++it)
	{
		if (name_hash == (*it)->getNameHash())
		{
			delete *it;
			Timelines.erase(it);
			break;
		}
	}
}

/**
 @fn	CzAnimTimeline* CzAnimTimelinesManager::findTimeline(unsigned int name_hash)

 @brief	Searches for the manager for the named timeline.

 @param	name_hash	The timeline name as a string hash.

 @return	null if it not found, else the found timeline.
 */

CzAnimTimeline* CzAnimTimelinesManager::findTimeline(unsigned int name_hash)
{
	for (_Iterator it = Timelines.begin(); it != Timelines.end(); ++it)
	{
		if ((*it)->getNameHash() == name_hash)
			return *it;
	}

	return NULL;
}

/**
 @fn	CzAnimTimeline* CzAnimTimelinesManager::findTimeline(const char* name)

 @brief	Searches for the manager for the named timeline.

 @param	name	The timeline name.

 @return	null if it not found, else the found timeline.
 */

CzAnimTimeline* CzAnimTimelinesManager::findTimeline(const char* name)
{
	return findTimeline(CzString::CalculateHash(name));
}

/**
 @fn	void CzAnimTimelinesManager::clearTimelines()

 @brief	Clears and destroys all the timelines in the manager.

 */

void CzAnimTimelinesManager::clearTimelines()
{
	// Remove and delete all timelines from the manager
	for (_Iterator it = Timelines.begin(); it != Timelines.end(); ++it)
		delete *it;
	Timelines.clear();
}

/**
 @fn	CzAnimTimeline* CzAnimTimelinesManager::FindTimeline(const char* name,
    IzXomlResource* container)

 @brief	Static method that searches for the named timeline.

 If the timeline is not found in the supplied conntainer then the global timelines manager will be searched.

 @param	name				 	The timeline name.
 @param [in]	container	If non-null, the container.

 @return	null if not found, else the found timeline.
 */

CzAnimTimeline* CzAnimTimelinesManager::FindTimeline(const char* name, IzXomlResource* container)
{
	return FindTimeline(CZ_HASH(name), container);
}

/**
 @fn	CzAnimTimeline* CzAnimTimelinesManager::FindTimeline(unsigned int name_hash,
    IzXomlResource* container)

 @brief	Static method that searches for the named timeline.

 If the timeline is not found in the supplied conntainer then the global timelines manager will be searched.

 @param	name				 	The timeline name as a string hash.
 @param [in]	container	If non-null, the container.

 @return	null if not found, else the found timeline.
 */

CzAnimTimeline* CzAnimTimelinesManager::FindTimeline(unsigned int name_hash, IzXomlResource* container)
{
	CzAnimTimeline* timeline = NULL;
	
	if (container != NULL)
	{
		// If container class is an actor then check the actors scenes timelines manager
		if (container->getClassTypeHash() == CzHashes::Actor_Hash)
		{
			timeline = ((CzActor*)container)->getTimelinesManager()->findTimeline(name_hash);

			// If not found in the actor then check the actors scene
			if (timeline == NULL)
				container = ((CzActor*)container)->getScene();
		}

		// If container class is a scene then check the scenes timelines manager
		if (timeline == NULL && container->getClassTypeHash() == CzHashes::Scene_Hash)
			timeline = ((CzScene*)container)->getTimelinesManager()->findTimeline((name_hash));
	}

	// If the timelines has no container or the timelines was not found in the supplied container then check the global timelines manager
	if (timeline == NULL)
		timeline = (CzAnimTimeline*)CZ_GLOBAL_RESOURCES->getTimelinesManager()->findTimeline((name_hash));

	return timeline;
}

/**
 @fn	CzAnimTimeline* CzAnimTimelinesManager::FindTimeline(const char* name, CzScene* container,
    const char* actor_name)

 @brief	Static method that searches for the named timeline.

 Searches the supplied actors timelines manager for the named timeline, if supplied actor not valid then the supplied scene will be searched instead. If the timeline is 
 not found in the scenes timelines manager then the global timelines manager will be searched. 

 @param	name				 	The timeline name.
 @param [in]	container	If non-null, the container.
 @param	actor_name			 	Name of the actor.

 @return	null if not found, else the found timeline.
 */

CzAnimTimeline*	CzAnimTimelinesManager::FindTimeline(const char* name, CzScene* container, const char* actor_name)
{
	return FindTimeline(CZ_HASH(name), container, CZ_HASH(actor_name));
}

/**
 @fn	CzAnimTimeline* CzAnimTimelinesManager::FindTimeline(unsigned int name_hash,
    CzScene* scene, unsigned int actor_hash)

 @brief	Static method that searches for the named timeline.

 Searches the supplied actors timelines manager for the named timeline, if supplied actor not valid then the supplied scene will be searched instead. If the timeline is 
 not found in the scenes timelines manager then the global timelines manager will be searched. 

 @param	name_hash			 	The timeline name as a string hash.
 @param [in]	container	If non-null, the container.
 @param	actor_name			 	Name of the actor.

 @return	null if not found, else the found timeline.
 */

CzAnimTimeline* CzAnimTimelinesManager::FindTimeline(unsigned int name_hash, CzScene* scene, unsigned int actor_hash)
{
	CzAnimTimeline* timeline = NULL;
	
	if (scene != NULL)
	{
		CzActor* actor = scene->findActor(actor_hash);
		if (actor != NULL)
			timeline = actor->getTimelinesManager()->findTimeline((name_hash));
		else
			timeline = scene->getTimelinesManager()->findTimeline((name_hash));
	}

	if (timeline == NULL)
		timeline = (CzAnimTimeline*)CZ_GLOBAL_RESOURCES->getTimelinesManager()->findTimeline((name_hash));

	return timeline;
}












