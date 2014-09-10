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

#if !defined(_CIZ_PLATFORM_AUDIO_H_)
#define _CIZ_PLATFORM_AUDIO_H_

#include "CzPlatform.h"

class CzScene;

enum eCzAudioCodec
{
	Audio_Codec_None, 
	Audio_Codec_Aac, 
	Audio_Codec_Aacplus, 
	Audio_Codec_Amr, 
	Audio_Codec_Midi, 
	Audio_Codec_Mp3, 
	Audio_Codec_Mp4, 
	Audio_Codec_Pcm, 
	Audio_Codec_Qcp, 
	Audio_Codec_Spf, 
};

#define	PLATFORM_AUDIO		PLATFORM->getAudio()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IzPlatformAudio
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IzPlatformAudio
{
	// Properties
protected:
	bool					Initialised;				///< Platform system initialised state
	bool					SoundOn;					///< Sound on flag (will not play sound effects if off)
	bool					MusicOn;					///< Music flag (will not play music if off(
public:
	bool					isInitialised() const				{ return Initialised; }
	bool					isSoundOn() const					{ return SoundOn; }
	virtual void			setSoundOn(bool on) = 0;
	bool					isMusicOn() const					{ return MusicOn; }
	virtual void			setMusicOn(bool on) = 0;
	// Properties end
public:
	virtual ~IzPlatformAudio() {}
	virtual int				Init() = 0;
	virtual void			Release() = 0;
	virtual void			Update() = 0;

	virtual bool			InitSound(void* memory_file, int memory_file_size, CzSoundData& sound_data, CzSoundSpec& sound_spec) = 0;
	virtual CzSoundInst		PlaySound(unsigned int name_hash, CzScene* scene, float volume, float pitch, float pan, bool looped) = 0;
	virtual CzSoundInst		PlaySound(CzSoundSpec sound_spec, float volume, float pitch, float pan, bool looped) = 0;
	virtual void			StopSound(CzSoundInst sound_inst) = 0;
	virtual void			PauseSound(CzSoundInst sound_inst) = 0;
	virtual void			ResumeSound(CzSoundInst sound_inst) = 0;
	virtual void			setSoundVolume(CzSoundInst sound_inst, float volume) = 0;
	virtual void			setSoundPitch(CzSoundInst sound_inst, float pitch) = 0;
	virtual void			setSoundPan(CzSoundInst sound_inst, float pan) = 0;
	virtual void			StopAllSounds() = 0;
	virtual void			PauseAllSounds() = 0;
	virtual void			ResumeAllSounds() = 0;
	virtual bool			isSoundPlaying(CzSoundInst sound_inst) const = 0;

	virtual bool			isMusicCodecSupported(eCzAudioCodec codec) = 0;
	virtual void			setGlobalSoundVolume(float vol) = 0;
	virtual void			setMusicVolume(float vol) = 0;
	virtual bool			PlayMusic(const char* name, int repeat_count = 0) = 0;
	virtual bool			PlayMusic(void* buffer, uint32 buffer_length, uint32 repeat_count) = 0;
	virtual void			StopMusic() = 0;
	virtual void			PauseMusic() = 0;
	virtual void			ResumeMusic() = 0;
	virtual bool			isMusicPlaying() = 0;

	virtual void			DeleteSoundData(CzSoundData data) = 0;
	virtual void			DeleteSoundSpec(CzSoundSpec spec) = 0;
};

#endif	// _CIZ_PLATFORM_AUDIO_H_
