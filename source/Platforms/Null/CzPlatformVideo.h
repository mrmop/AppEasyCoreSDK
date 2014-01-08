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

#if !defined(_CCZ_PLATFORM_VIOEO_H_)
#define _CCZ_PLATFORM_VIOEO_H_

#include "IzPlatformVideo.h"

class CzPlatformVideo: public IzPlatformVideo
{
	// Properties
protected:
	float					getPlayPosition() const;
	bool					setPlayPosition(float pos);
	bool					isPlaying() const;
	void					setVolume(float volume);
	float					getVolume() const;
public:
	// Properties end

protected:
public:
	int						Init();
	void					Release();

	bool					Play(const char* filename, const CzIRect& rect, int repeat_count, eCzVideoCodec codec);
	bool					PlayFromMemory(char* mem, uint32 len, const CzIRect& rect, int repeat_count, eCzVideoCodec codec);
	void					Stop();
	void					Pause();
	void					Resume();
	bool					isCodecSupported(eCzVideoCodec codec);
	bool					isCodecSupported(unsigned int codec_hash, bool &found);

	// Event notification
	void					NotifyStopped();
};


#endif	// _CCZ_PLATFORM_VIOEO_H_
