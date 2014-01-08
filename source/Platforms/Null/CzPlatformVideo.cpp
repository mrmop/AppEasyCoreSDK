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

#include "CzPlatformVideo.h"
#include "CzString.h"

int32 CzPlatformVideo_Stopped(void* systemData, void* userData)
{
	PLATFORM_VIDEO->NotifyStopped();
	return 1;
}

int CzPlatformVideo::Init()
{
	VideoStoppedCallback = NULL;
	VideoStoppedCallbackData = NULL;
	Initialised = true;

	if (g_CzShowTracing > 1)
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Registering video callbacks");

	return 1;
}

void CzPlatformVideo::Release()
{
	Stop();
	Initialised = false;
}

bool CzPlatformVideo::isPlaying() const
{
	return false;
}

void CzPlatformVideo::setVolume(float volume)
{
}

float CzPlatformVideo::getVolume() const
{
	return 0;
}

bool CzPlatformVideo::Play(const char* filename, const CzIRect& rect, int repeat_count, eCzVideoCodec codec)
{
	return false;
}

bool CzPlatformVideo::PlayFromMemory(char* mem, uint32 len, const CzIRect& rect, int repeat_count, eCzVideoCodec codec)
{
	return false;
}

void CzPlatformVideo::Stop()
{
}

void CzPlatformVideo::Pause()
{
}

void CzPlatformVideo::Resume()
{
}

float CzPlatformVideo::getPlayPosition() const
{
	return 0;
}

bool CzPlatformVideo::setPlayPosition(float pos)
{
	return false;
}

bool CzPlatformVideo::isCodecSupported(eCzVideoCodec codec)
{
	return false;
}

bool CzPlatformVideo::isCodecSupported(unsigned int codec_hash, bool &found)
{
	return false;
}

void CzPlatformVideo::NotifyStopped()
{
	if (VideoStoppedCallback != NULL)
		VideoStoppedCallback(this, VideoStoppedCallbackData);
}


