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

#if !defined(_CIZ_PLATFORM_VIDEO_H_)
#define _CIZ_PLATFORM_VIDEO_H_

#include "CzPlatform.h"

#define	PLATFORM_VIDEO		PLATFORM->getVideo()

enum eCzVideoCodec
{
	Video_Codec_None, 
	Video_Codec_Mpeg4, 
	Video_Codec_3gpp, 
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IzPlatformVideo
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IzPlatformVideo
{
	// Properties
protected:
	bool				Initialised;
	CzCallback			VideoStoppedCallback;			//< Callback to be called when video stops playing
	void*				VideoStoppedCallbackData;		///< Callback data to be passed back with callback
public:
	bool				isInitialised() const	{ return Initialised; }
	virtual float		getPlayPosition() const = 0;
	virtual bool		setPlayPosition(float pos) = 0;
	virtual bool		isPlaying() const = 0;
	virtual void		setVolume(float volume) = 0;
	virtual float		getVolume() const = 0;
	void				setVideoStoppedCallback(CzCallback callback, void* data)	{ VideoStoppedCallback = callback; VideoStoppedCallbackData = data; }
	CzCallback			getVideoStoppedCallback()									{ return VideoStoppedCallback; }
	// Properties end
public:
	virtual int			Init() = 0;
	virtual void		Release() = 0;
	virtual bool		Play(const char* filename, const CzIRect& rect, int repeat_count, eCzVideoCodec codec) = 0;
	virtual bool		PlayFromMemory(char* mem, uint32 len, const CzIRect& rect, int repeat_count, eCzVideoCodec codec) = 0;
	virtual void		Stop() = 0;
	virtual void		Pause() = 0;
	virtual void		Resume() = 0;
	virtual bool		isCodecSupported(eCzVideoCodec codec) = 0;
	virtual bool		isCodecSupported(unsigned int codec_hash, bool &found) = 0;

	// Event notification
	virtual void		NotifyStopped() = 0;
	
};


#endif	// _CIZ_PLATFORM_VIDEO_H_
