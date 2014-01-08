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

#include "CzUIVideoOverlay.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"

CzXomlClassDef* CzUIVideoOverlay::VideoOverlayClassDef = NULL;

/**
 @fn	void CzUIVideoOverlay::InitClass()

 @brief	Initialises the class.
    
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzUIVideoOverlay::InitClass()
{
	// Create class properties
	VideoOverlayClassDef = new CzXomlClassDef();
	VideoOverlayClassDef->addProperty(new CzXomlClassProperty("Video",		PT_Resource,	CzUIVideoOverlay::_setVideo,		CzUIVideoOverlay::_getVideo));
	VideoOverlayClassDef->addProperty(new CzXomlClassProperty("AutoPlay",	PT_Bool,		CzUIVideoOverlay::_setAutoPlay,		CzUIVideoOverlay::_getAutoPlay));
	VideoOverlayClassDef->addProperty(new CzXomlClassProperty("Repeat",		PT_Int,			CzUIVideoOverlay::_setRepeat,		CzUIVideoOverlay::_getRepeat));
	VideoOverlayClassDef->addProperty(new CzXomlClassProperty("Volume",		PT_Float,		CzUIVideoOverlay::_setVolume,		CzUIVideoOverlay::_getVolume));
	VideoOverlayClassDef->addProperty(new CzXomlClassProperty("Command",	PT_String,		CzUIVideoOverlay::_setCommand,		NULL));
	VideoOverlayClassDef->addProperty(new CzXomlClassProperty("IsPlaying",	PT_Bool,		NULL,								CzUIVideoOverlay::_getIsPlaying));
	VideoOverlayClassDef->addProperty(new CzXomlClassProperty("Started",	PT_Bool,		NULL,								CzUIVideoOverlay::_getStarted));
	VideoOverlayClassDef->addProperty(new CzXomlClassProperty("OnStart",	PT_String,		CzUIVideoOverlay::_setOnStart,		NULL));
	VideoOverlayClassDef->addProperty(new CzXomlClassProperty("OnEnd",		PT_String,		CzUIVideoOverlay::_setOnEnd,		NULL));
	VideoOverlayClassDef->addProperty(new CzXomlClassProperty("OnPause",	PT_String,		CzUIVideoOverlay::_setOnPause,		NULL));
	VideoOverlayClassDef->addProperty(new CzXomlClassProperty("OnResume",	PT_String,		CzUIVideoOverlay::_setOnResume,		NULL));
	VideoOverlayClassDef->addProperty(new CzXomlClassProperty("OnError",	PT_String,		CzUIVideoOverlay::_setOnError,		NULL));

}
void CzUIVideoOverlay::ReleaseClass()
{
	if (VideoOverlayClassDef != NULL)
	{
		VideoOverlayClassDef->Release();
		delete VideoOverlayClassDef;
		VideoOverlayClassDef = NULL;
	};
}

bool CzUIVideoOverlay::_setVideo(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIVideoOverlay* actor = (CzUIVideoOverlay*)target;
	CzVideo* video;
	if (prop.Type == PT_Resource)
		video = (CzVideo*)prop.p_data;
	else
		video = (CzVideo*)actor->getScene()->getResourceManager()->findResource((const char*)prop.p_data, CzHashes::Video_Hash);
	if (video != NULL)
		actor->setVideo(video);
	else
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VideoOverlay - setProperty - could not find Video in the scene or in the global resources - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());

	return true;
}

CzXomlProperty CzUIVideoOverlay::_getVideo(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIVideoOverlay*)target)->getVideo());
}

bool CzUIVideoOverlay::_setAutoPlay(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIVideoOverlay* actor = (CzUIVideoOverlay*)target;

	if (add)
		actor->setAutoPlay(!actor->isAutoPlay());
	else
		actor->setAutoPlay(prop.p_bool);

	return true;
}

CzXomlProperty CzUIVideoOverlay::_getAutoPlay(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIVideoOverlay*)target)->isAutoPlay());
}

bool CzUIVideoOverlay::_setRepeat(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIVideoOverlay* actor = (CzUIVideoOverlay*)target;

	CzVideo* video = actor->getVideo();
	if (video == NULL)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VideoOverlay - setProperty - Cannot set repeat, no video attached - ", actor->getName().c_str(), actor->getDebugInfo().c_str());
	else
		video->setRepeatCount(prop.p_int);

	return true;
}

CzXomlProperty CzUIVideoOverlay::_getRepeat(IzXomlResource* target)
{
	CzUIVideoOverlay* actor = (CzUIVideoOverlay*)target;
	CzVideo* video = actor->getVideo();
	if (video == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VideoOverlay - getProperty - Cannot get repeat no, video attached - ", actor->getName().c_str(), actor->getDebugInfo().c_str());
		return CzXomlProperty();
	}

	return CzXomlProperty(video->getRepeatCount());
}

bool CzUIVideoOverlay::_setVolume(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIVideoOverlay* actor = (CzUIVideoOverlay*)target;

	CzVideo* video = actor->getVideo();
	if (video == NULL)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VideoOverlay - setProperty - Cannot set volume no, video attached - ", actor->getName().c_str(), actor->getDebugInfo().c_str());
	else
	{
		if (add)
			video->setVolume(video->getVolume() + prop.p_float);
		else
			video->setVolume(prop.p_float);
	}

	return true;
}

CzXomlProperty CzUIVideoOverlay::_getVolume(IzXomlResource* target)
{
	CzUIVideoOverlay* actor = (CzUIVideoOverlay*)target;
	CzVideo* video = actor->getVideo();
	if (video == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VideoOverlay - getProperty - Cannot get volume, no video attached - ", actor->getName().c_str(), actor->getDebugInfo().c_str());
		return CzXomlProperty();
	}

	return CzXomlProperty(video->getVolume());
}

bool CzUIVideoOverlay::_setCommand(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIVideoOverlay* actor = (CzUIVideoOverlay*)target;

	unsigned int command_hash = CzString::CalculateHash((const char*)prop.p_data);
	if (command_hash == CZ_HASH("stop"))
		actor->Stop();
	else
	if (command_hash == CZ_HASH("play"))
		actor->Play();
	else
	if (command_hash == CZ_HASH("pause"))
		actor->Pause();
	else
	if (command_hash == CZ_HASH("resume"))
		actor->Resume();
	else
		return false;
		
	return true;
}

CzXomlProperty CzUIVideoOverlay::_getIsPlaying(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIVideoOverlay*)target)->isPlaying());
}

CzXomlProperty CzUIVideoOverlay::_getStarted(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIVideoOverlay*)target)->isStarted());
}

bool CzUIVideoOverlay::_setOnStart(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIVideoOverlay* actor = (CzUIVideoOverlay*)target;
	actor->getEventsManager()->addEvent("OnStart", (const char*)prop.p_data, true);
	return true;
}

bool CzUIVideoOverlay::_setOnEnd(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIVideoOverlay* actor = (CzUIVideoOverlay*)target;
	actor->getEventsManager()->addEvent("OnEnd", (const char*)prop.p_data, true);
	return true;
}

bool CzUIVideoOverlay::_setOnPause(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIVideoOverlay* actor = (CzUIVideoOverlay*)target;
	actor->getEventsManager()->addEvent("OnPause", (const char*)prop.p_data, true);
	return true;
}

bool CzUIVideoOverlay::_setOnResume(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIVideoOverlay* actor = (CzUIVideoOverlay*)target;
	actor->getEventsManager()->addEvent("OnResume", (const char*)prop.p_data, true);
	return true;
}

bool CzUIVideoOverlay::_setOnError(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIVideoOverlay* actor = (CzUIVideoOverlay*)target;
	actor->getEventsManager()->addEvent("OnError", (const char*)prop.p_data, true);
	return true;
}

//
//
//
//
//
// CzUIVideoOverlay implementation
//
//
//
//
//

bool CzUIVideoOverlay::Play()
{
	if (Video != NULL)
	{
		// Only allow video to begin play after UI element has been rendered at least once to allow us to pick up screen coordinates
		CzVec2* v = Visual->getScreenV();
		float dx = (v + 1)->x - v->x;
		if (dx != 0)
		{
			if (BackgroundBrush != NULL && BackgroundBrush->getBrushType() == IzBrush::BT_9Patch)
				Video->setRect((int)v->x, (int)v->y, (int)((v + 3)->x - v->x), (int)((v + 12)->y - v->y));
			else
				Video->setRect((int)v->x, (int)v->y, (int)((v + 1)->x - v->x), (int)((v + 3)->y - v->y));
			Started = true;
			if (!Video->Play())
			{
				ProcessEventActions(CzHashes::OnError_Hash);
				return false;
			}
			Playing = true;
			ProcessEventActions(CzHashes::OnStart_Hash);

			return true;
		}
	}

	ProcessEventActions(CzHashes::OnError_Hash);

	return false;
}

void CzUIVideoOverlay::Stop()
{
	PLATFORM_VIDEO->Stop();

	if (Playing)
		ProcessEventActions(CzHashes::OnEnd_Hash);

	Playing = false;
}

void CzUIVideoOverlay::Pause()
{
	PLATFORM_VIDEO->Pause();
	if (Playing)
		ProcessEventActions(CzHashes::OnPause_Hash);
}

void CzUIVideoOverlay::Resume()
{
	PLATFORM_VIDEO->Resume();
	if (Playing)
		ProcessEventActions(CzHashes::OnResume_Hash);
}

bool CzUIVideoOverlay::Update(float dt)
{
	if (!CzUIBase::Update(dt))
		return false;

	if (!Started && AutoPlay)
	{
		Play();
	}

	if (Playing)
	{
		if (!PLATFORM_VIDEO->isPlaying())
		{
			Playing = false;
			ProcessEventActions(CzHashes::OnEnd_Hash);
		}
	}

	return true;
}

int CzUIVideoOverlay::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "VideoOverlay - Needs to be declared inside a scene or an actor - ", DebugInfo.c_str());
		return 0;
	}

	// Load main actor attributes
	int ret = CzUIBase::LoadFromXoml(parent, false, node);
	if (ret <= 0)
		return ret;

	// Process VideoOverlay specific attributes
	CzString*	video_name = NULL;
	float		volume = 1.0f;
	int			repeat = 1;

	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Video_Hash)
			video_name = &(*it)->getValue();
		else
		if (name_hash == CzHashes::AutoPlay_Hash)
			AutoPlay = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Repeat_Hash)
			repeat = (*it)->getValueAsInt();
		else
		if (name_hash == CzHashes::Volume_Hash)
			volume = (*it)->getValueAsFloat();
		else
		if (name_hash == CzHashes::OnStart_Hash)
			EventsManager->addEvent("OnStart", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnEnd_Hash)
			EventsManager->addEvent("OnEnd", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnPause_Hash)
			EventsManager->addEvent("OnPause", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnResume_Hash)
			EventsManager->addEvent("OnResume", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::OnError_Hash)
			EventsManager->addEvent("OnError", (*it)->getValue().c_str());
	}

	if (LinkedTo != NULL)
	{
		if (Size.x == 0)
			Size.x = LinkedTo->getSize().x;
		if (Size.y == 0)
			Size.y = LinkedTo->getSize().y;
	}

	// Search the scenes resources for the specified video
	if (video_name != NULL)
	{
		Video = (CzVideo*)Scene->getResourceManager()->findResource(video_name->c_str(), CzHashes::Video_Hash);
		if (Video == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VideoOverlay - Could not find Video in the scene or in the global resources - ", video_name->c_str(), DebugInfo.c_str());
		else
		{
			Video->setVolume(volume);
			Video->setRepeatCount(repeat);
		}
	}

	if (load_children)
	{
		// Process inner tags
		if (!CZ_XOML->Process(this, node))
			return 0;
	}
	return 1;
}

/**
 @fn	bool CzUIVideoOverlay::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the actor.

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types. Note that as 
 CzUIVideoOverlay is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param	property_name		Name of the property.
 @param [out]	prop	The property data that.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIVideoOverlay::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	if (CzUIBase::getProperty(property_name, prop))
		return true;

	int err = IzXomlResource::getProperty(VideoOverlayClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = VideoOverlayClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VideoOverlay - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIVideoOverlay::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the UIVideoOverlay.
    
 Sets the named property of this UIVideoOverlay. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzUIVideoOverlay::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	if (CzUIBase::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(VideoOverlayClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = VideoOverlayClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VideoOverlay - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIVideoOverlay::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced. Note that as CzUIVideoOverlay is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in]	data	The property data.
 @param	delta				true to property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIVideoOverlay::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	if (CzUIBase::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(VideoOverlayClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = VideoOverlayClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VideoOverlay - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIVideoOverlay::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates actor property from supplied animation.

 Updates the actors property that is specified by the supplied animation. When a timeline animation targets the property of an actor this method will be called by the animation to move the 
 animations frame data into its target property. Note that as CzUIVideoOverlay is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param [in]	animation	If non-null, the animation.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIVideoOverlay::UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzUIBase::UpdateFromAnimation(animation))
		return true;

	int err = IzXomlResource::setProperty(VideoOverlayClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = VideoOverlayClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VideoOverlay - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIVideoOverlay::UpdateBinding(unsigned int property_name, CzXomlVariable* var)

 @brief	Updates an actor property from the supplied XOML variable.

 Updates a single property from the value of the bound XOML variable. Note that as CzUIVideoOverlay is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param	property_name  	Name of the property.
 @param [in,out]	var	If non-null, the variable.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIVideoOverlay::UpdateBinding(unsigned int property_name, CzXomlVariable* var)
{
	if (!var->Modified || var->getValue().isEmpty())
		return false;

	if (CzUIBase::UpdateBinding(property_name, var))
		return true;

	int err = IzXomlResource::setProperty(VideoOverlayClassDef, this, property_name, var);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = VideoOverlayClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VideoOverlay - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

