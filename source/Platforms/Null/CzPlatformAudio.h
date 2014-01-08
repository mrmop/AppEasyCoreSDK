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

#if !defined(_CCZ_PLATFORM_AUDIO_H_)
#define _CCZ_PLATFORM_AUDIO_H_

#include "IzPlatformAudio.h"

class CzScene;

class CzPlatformAudio : public IzPlatformAudio
{
	// Properties
protected:
	void			setSoundOn(bool on);
	void			setMusicOn(bool on);
public:
	// Properties end
public:
	int				Init();
	void			Release();
	void			Update();

	bool			InitSound(void* memory_file, int memory_file_size, CzSoundData& sound_data, CzSoundSpec& sound_spec);
	CzSoundInst		PlaySound(unsigned int name_hash, CzScene* scene, float volume, float pitch, float pan, bool looped);
	CzSoundInst		PlaySound(CzSoundSpec sound_spec, float volume, float pitch, float pan, bool looped);
	void			StopSound(CzSoundInst sound_inst);
	void			PauseSound(CzSoundInst sound_inst);
	void			ResumeSound(CzSoundInst sound_inst);
	void			setSoundVolume(CzSoundInst sound_inst, float volume);
	void			setSoundPitch(CzSoundInst sound_inst, float pitch);
	void			setSoundPan(CzSoundInst sound_inst, float pan);
	bool			isSoundPlaying(CzSoundInst sound_inst) const;
	void			StopAllSounds();
	void			PauseAllSounds();
	void			ResumeAllSounds();

	bool			isMusicCodecSupported(eCzAudioCodec codec);
	void			setGlobalSoundVolume(float vol);
	void			setMusicVolume(float vol);
	bool			PlayMusic(const char* name, int repeat_count = 0);
	bool			PlayMusic(void* buffer, uint32 buffer_length, uint32 repeat_count);
	void			StopMusic();
	void			PauseMusic();
	void			ResumeMusic();
	bool			isMusicPlaying();

	void			DeleteSoundData(CzSoundData data);
	void			DeleteSoundSpec(CzSoundSpec spec);
};


#endif	// _CCZ_PLATFORM_AUDIO_H_
