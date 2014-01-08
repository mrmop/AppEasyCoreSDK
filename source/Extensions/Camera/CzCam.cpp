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
#include <memory.h>

#include "CzCam.h"
#include "CzActor.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"


//
//
//
//
// CzVideoCam implementation
//
//
//
//

int CzVideoCam::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	CzString* quality = NULL;
	CzString* resolution = NULL;
	CzString* target = NULL;
	CzString* start = NULL;
	CzString* direction = NULL;

	// Parse attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
			setName((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Target_Hash)
			target = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Quality_Hash)
			quality = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Resolution_Hash)
			resolution = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Start_Hash)
			start = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Direction_Hash)
			direction = &(*it)->getValue();
		else
		if (name_hash == CzHashes::OnError_Hash)
			EventsManager->addEvent("OnError", (*it)->getValue().c_str(), true);
	}

	// If we are declared inside a scene then we are local to the scene
	CzScene* scene = NULL;
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Actor_Hash)
		scene = ((CzActor*)parent)->getScene();
	else
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

	// Find the target image
	if (target != NULL)
	{
		CzImage* image = (CzImage*)CzXomlResourceManager::FindResource(target->getHash(), CzHashes::Image_Hash, scene);
		if (image == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VideoCam - Could not find target image - ", target->c_str(), DebugInfo.c_str());
		IzPlatformCam* cam = PLATFORM_CAM;
		PLATFORM_CAM->setImageBrush(image);
	}
	else
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VideoCam - No target image specified", DebugInfo.c_str());

	Quality = CCQ_Medium;
	Resolution = CCS_Medium;

	if (quality != NULL)
	{
		unsigned int hash = quality->getHash();
		if (hash == CZ_HASH("low"))
			Quality = CCQ_Low;
		else
		if (hash == CZ_HASH("high"))
			Quality = CCQ_High;
	}

	if (resolution != NULL)
	{
		unsigned int hash = resolution->getHash();
		if (hash == CZ_HASH("low"))
			Resolution = CCS_Small;
		else
		if (hash == CZ_HASH("high"))
			Resolution = CCS_Large;
	}

	if (direction != NULL)
	{
		if (direction->getHash() == CZ_HASH("front"))
			setFrontFacing();
		else
			setRearFacing();
	}

	if (scene != NULL)
	{
		if (!scene->getResourceManager()->addResource(this))
			return 0;
	}
	else
	{
		if (!CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this))
			return 0;
	}

	if (start != NULL && start->getAsBool())
		Start();

	return 1;
}

void CzVideoCam::Stop()
{
	if (PLATFORM_CAM != NULL)
		PLATFORM_CAM->Stop();
}

bool CzVideoCam::Start()
{
	if (!PLATFORM_CAM->isAvailable())
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VideoCam - Start - This platform does not support VideoCam", DebugInfo.c_str());
		ProcessEventActions(CZ_HASH("OnError"), Parent->getParent());
		return false;
	}
	else
	{
		if (CameraType == CCT_Front)
		{
			if (!PLATFORM_CAM->setFrontFacing())
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VideoCam - Start - Front facing camera not available", DebugInfo.c_str());
				ProcessEventActions(CZ_HASH("OnError"), Parent->getParent());
				return false;
			}
		}
		else
		{
			if (!PLATFORM_CAM->setRearFacing())
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "VideoCam - Start - Rear facing camera not available", DebugInfo.c_str());
				ProcessEventActions(CZ_HASH("OnError"), Parent->getParent());
				return false;
			}
		}
		if (!PLATFORM_CAM->Start(Resolution, Quality))
		{
			ProcessEventActions(CZ_HASH("OnError"), Parent->getParent());
			return false;
		}
	}

	return true;
}

void CzVideoCam::ProcessEventActions(unsigned int event_name, IzXomlResource* parent)
{
	// Find the event
	CzEvent* evt = EventsManager->findEvent(event_name);
	if (evt != NULL)
	{
		// Find actions list
		CzActions* actions = CzActionsManager::FindActions(evt->ActionsName, parent);
		if (actions != NULL)
			actions->Execute(parent);
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "VideoCam - Action not found - ", evt->_ActionsName.c_str(), DebugInfo.c_str());
	}
}
