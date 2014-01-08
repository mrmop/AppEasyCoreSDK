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

#include "CzVideo.h"
#include "CzActor.h"
#include "CzHashes.h"
#include "IzPlatformDisplay.h"
#include "CzXomlResourceManager.h"

//
//
//
//
// CzVideo implementation
//
//
//
//
CzVideo::~CzVideo()
{
	PLATFORM_VIDEO->setVideoStoppedCallback(NULL, NULL);
	SAFE_DELETE(EventsManager);
	SAFE_DELETE(File)
	SAFE_DELETE(VideoData)
}

bool CzVideo::Init(const char* filename, bool preload, bool blocking, eCzVideoCodec codec)
{
	if (!PLATFORM_VIDEO->isCodecSupported(codec))
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Video - Playback is not supported on this OS", DebugInfo.c_str());
		return false;
	}

	Filename = filename;
	Codec = codec;
	File = new CzFile(filename);

	if (preload)
		return Load(blocking);
	if (Rect.w == 0)
	{
		Rect.x = 0;
		Rect.y = 0;
		Rect.w = PLATFORM_DISPLAY->getCurrentWidth();
		Rect.h = PLATFORM_DISPLAY->getCurrentHeight();
	}

	return true;
}

bool CzVideo::Init(char* video_data, uint32 video_data_len, eCzVideoCodec codec)
{
	if (!PLATFORM_VIDEO->isCodecSupported(codec))
		return false;

	VideoDataLen = video_data_len;
	VideoData = video_data;
	Codec = codec;
	State = State_Loaded;
	if (Rect.w == 0)
	{
		Rect.x = 0;
		Rect.y = 0;
		Rect.w = PLATFORM_DISPLAY->getCurrentWidth();
		Rect.h = PLATFORM_DISPLAY->getCurrentHeight();
	}

	return true;
}

int32 CzVideo_FileRetrievedAsyncCallback(void* caller, void* data)
{
	CzVideo* video = (CzVideo*)data;
	video->FinishLoad();

	return 0;
}

bool CzVideo::Load(bool blocking)
{
	if (!PLATFORM_VIDEO->isCodecSupported(Codec))
		return false;

	// Local video files are played from file and not pre-loaded, so only continue if remote based
	if (!CzFile::isHttp(Filename.c_str(), Filename.getLength()))
	{
		State = State_Loaded;
		return true;
	}

	// Video is file based
	if (File != NULL)
	{
		File->setFileAvailableCallback(CzVideo_FileRetrievedAsyncCallback, this);
		if (File->Open(NULL, "rb", blocking))
			return true;
		else
			return false;
	}

	return false;
}

void CzVideo::FinishLoad()
{
	if (File != NULL)
	{
		if (CzFile::isHttp(File->getFilename().c_str(), File->getFilename().getLength()))
		{
			if (File->isFileAvailable() && File->getError() == CzFileErrorNone)
			{
				VideoDataLen = File->getContentLength();
				VideoData = new char [VideoDataLen];
				memcpy(VideoData, File->getContent(), VideoDataLen);
			}
			else
			{
				State = State_Invalid;
				return;
			}
		}
		else
		{
			VideoDataLen = File->getFileSize();
			VideoData = new char [VideoDataLen];
			File->Read(VideoData, VideoDataLen);
		}
		State = State_Loaded;
		SAFE_DELETE(File)
	}
}

int32 CzVideo_Stopped(void* caller, void* data)
{
	CzVideo* video = (CzVideo*)data;
	if (video != NULL)
		video->NotifyStopped();

	return 1;
}

bool CzVideo::Play()
{
	if (State != State_Loaded)
		Load(true);
	if (State != State_Loaded)
		return false;

	bool success = false;

	PLATFORM_VIDEO->setVideoStoppedCallback(CzVideo_Stopped, this);

	if (VideoData != NULL)
		success = PLATFORM_VIDEO->PlayFromMemory(VideoData, VideoDataLen, Rect, RepeatCount, Codec);
	else
	if (!Filename.isEmpty())
		success = PLATFORM_VIDEO->Play(Filename.c_str(), Rect, RepeatCount, Codec);

	if (success)
		PLATFORM_VIDEO->setVolume(Volume);

	return success;
}

int CzVideo::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Process image attributes
	CzString*	name = NULL;
	CzString*	location = NULL;
	bool		preload = false;
	bool		blocking = false;
	CzString*	condition = NULL;
	eCzVideoCodec codec = Video_Codec_Mpeg4;
	int			repeat = 1;

	CzScene* scene = NULL;
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Actor_Hash)
		scene = ((CzActor*)parent)->getScene();
	else
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
		{
			name = &(*it)->getValue();
			setName(name->c_str());
		}
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Location_Hash)
			location = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Preload_Hash)
			preload = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Blocking_Hash)
			blocking = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Condition_Hash)
			condition = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Repeat_Hash)
			repeat = (*it)->getValueAsInt();
		else
		if (name_hash == CzHashes::Codec_Hash)
		{
			unsigned int format_hash = (*it)->getValue().getHash();
			if (format_hash == CZ_HASH("MPEG4"))
				codec = Video_Codec_Mpeg4;
			else
			if (format_hash == CZ_HASH("3GPP"))
				codec = Video_Codec_3gpp;
		}
	}

	if (location == NULL || name == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Video - Requires a location and a name to be specified", DebugInfo.c_str());
		return 0;
	}

	if (condition != NULL)
	{
		// Find the condition variable
		bool condition_not = false;
		CzXomlVariable* var = NULL;
		if (*(condition->c_str()) == '!')
		{
			condition_not = true;
			CzString cond = condition->c_str() + 1;
			var = CzXomlVariable::GetVariable(cond, scene);
		}
		else
			var = CzXomlVariable::GetVariable(*condition, scene);
		if (var != NULL)
		{
			bool res = var->isTrue();
			if (condition_not)
				res = !res;
			if (!res)
				return -1;
		}
#if defined (_DEBUG)
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Video - Condition variable not found - ", condition->c_str(), DebugInfo.c_str());
#endif // _DEBUG 
	}

	// If codec is not supported or a video of the same name already exists then delete
	CzVideo* video = (CzVideo*)CzXomlResourceManager::FindResource(Name.c_str(), CzHashes::Video_Hash, scene);
	if (!PLATFORM_VIDEO->isCodecSupported(codec) || video != NULL)
		return -1;

	// Initialise the video
	Init(location->c_str(), preload, blocking, codec);
	setRepeatCount(repeat);

	// If we are declared inside a scene then image is local to the scene
	if (scene != NULL)
		return scene->getResourceManager()->addResource(this) ? 1 : 0;
	else
		return CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this) ? 1 : 0;

	return 1;
}

/**
 @fn	void CzVideo::ProcessEventActions(unsigned int event_name)

 @brief	Process the event actions described by event_name.

 Processes the actions list that is associated with the event named event_name (event_name is a hash of the actual event name to speed up searches)

 @param	event_name	Name of the event as a string hash
 */

void CzVideo::ProcessEventActions(unsigned int event_name)
{
	// Find the event 
	CzEvent* evt = EventsManager->findEvent(event_name);
	if (evt != NULL)
	{
		// Find actions list
		IzXomlResource* parent = Parent->getParent();
		CzActions* actions = CzActionsManager::FindActions(evt->ActionsName, parent);
		if (actions != NULL)
			actions->Execute(parent);
	}
}

/**
 @fn	void CzVideo::NotifyLoaded()

 @brief	Handles the video stopped event.

 */

void CzVideo::NotifyStopped()
{
	PLATFORM_VIDEO->setVideoStoppedCallback(NULL, NULL);
}
