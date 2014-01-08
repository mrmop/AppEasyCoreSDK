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

#include "Marm/CzPlatformVideo.h"
#include "CzString.h"

s3eVideoCodec CzPlatformVideo::getCodec(eCzVideoCodec codec)
{
	switch (codec)
	{
	case Video_Codec_Mpeg4:
		return S3E_VIDEO_CODEC_MPEG4;
	case Video_Codec_3gpp:
		return S3E_VIDEO_CODEC_3GPP;
	}

	CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Marmalade does not support this video codec - ", CzString(codec).c_str());

	return S3E_VIDEO_CODEC_MPEG4;
}

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

	s3eVideoRegister(S3E_VIDEO_STOP, CzPlatformVideo_Stopped, NULL);

	return 1;
}

void CzPlatformVideo::Release()
{
	s3eVideoUnRegister(S3E_VIDEO_STOP, CzPlatformVideo_Stopped);
	Stop();
	Initialised = false;
}

bool CzPlatformVideo::isPlaying() const
{
	return s3eVideoIsPlaying() == S3E_TRUE;
}

void CzPlatformVideo::setVolume(float volume)
{
	s3eVideoSetInt(S3E_VIDEO_VOLUME, (int)(volume * S3E_VIDEO_MAX_VOLUME));
}

float CzPlatformVideo::getVolume() const
{
	return (float)s3eVideoGetInt(S3E_VIDEO_VOLUME) / S3E_VIDEO_MAX_VOLUME;
}

bool CzPlatformVideo::Play(const char* filename, const CzIRect& rect, int repeat_count, eCzVideoCodec codec)
{
	s3eVideoCodec mcodec = getCodec(codec);
	if (s3eVideoIsCodecSupported(mcodec))
	{
		s3eResult res = s3eVideoPlay(filename, repeat_count, rect.x, rect.y, rect.w, rect.h);
		if (res != S3E_RESULT_SUCCESS)
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Could not play the video file - ", filename);

		return res == S3E_RESULT_SUCCESS;
	}
	else
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Codec not supported - ", filename);

	return false;
}

bool CzPlatformVideo::PlayFromMemory(char* mem, uint32 len, const CzIRect& rect, int repeat_count, eCzVideoCodec codec)
{
	s3eVideoCodec mcodec = getCodec(codec);
	if (s3eVideoIsCodecSupported(mcodec))
		return s3eVideoPlayFromBuffer(mem, len, repeat_count, rect.x, rect.y, rect.w, rect.h) == S3E_RESULT_SUCCESS;

	return false;
}

void CzPlatformVideo::Stop()
{
	if (isPlaying())
		s3eVideoStop();
}

void CzPlatformVideo::Pause()
{
	s3eVideoPause();
}

void CzPlatformVideo::Resume()
{
	s3eVideoResume();
}

float CzPlatformVideo::getPlayPosition() const
{
	return (float)s3eVideoGetInt(S3E_VIDEO_POSITION);
}

bool CzPlatformVideo::setPlayPosition(float pos)
{
	return false;
}

bool CzPlatformVideo::isCodecSupported(eCzVideoCodec codec)
{
	s3eVideoCodec mcodec = getCodec(codec);
	return s3eVideoIsCodecSupported(mcodec) == S3E_TRUE;
}

bool CzPlatformVideo::isCodecSupported(unsigned int codec_hash, bool &found)
{
	s3eVideoCodec codec;

	if (codec_hash == CZ_HASH("MPEG4"))
		codec = S3E_VIDEO_CODEC_MPEG4;
	else
	if (codec_hash == CZ_HASH("3GPP"))
		codec = S3E_VIDEO_CODEC_3GPP;
	else
	{
		found = false;
		return false;
	}

	found = true;

	return s3eVideoIsCodecSupported(codec) == S3E_TRUE;
}

void CzPlatformVideo::NotifyStopped()
{
	if (VideoStoppedCallback != NULL)
		VideoStoppedCallback(this, VideoStoppedCallbackData);
}


