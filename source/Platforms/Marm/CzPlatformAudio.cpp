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

#include "s3e.h"
#include "IwSound.h"
#include "Marm/CzPlatformAudio.h"
#include "CzXoml.h"
#include "CzHashes.h"
#include "CzFile.h"
#include "CzScene.h"
#include "CzAudio.h"
#include "CzXoml.h"
#include "CzXomlResourceManager.h"

s3eAudioCodec CzPlatformAudio::getCodec(eCzAudioCodec codec)
{
	switch (codec)
	{
	case Audio_Codec_Midi:
		return S3E_AUDIO_CODEC_MIDI;
	case Audio_Codec_Mp3:
		return S3E_AUDIO_CODEC_MP3;
	case Audio_Codec_Aac:
		return S3E_AUDIO_CODEC_AAC;
	case Audio_Codec_Aacplus:
		return S3E_AUDIO_CODEC_AACPLUS;
	case Audio_Codec_Qcp:
		return S3E_AUDIO_CODEC_QCP;
	case Audio_Codec_Pcm:
		return S3E_AUDIO_CODEC_PCM;
	case Audio_Codec_Spf:
		return S3E_AUDIO_CODEC_SPF;
	case Audio_Codec_Amr:
		return S3E_AUDIO_CODEC_AMR;
	case Audio_Codec_Mp4:
		return S3E_AUDIO_CODEC_MP4;
	}

	CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Audio - Marmalade does not support this audio codec - ", CzString(codec).c_str());

	return S3E_AUDIO_CODEC_MP3;
}

void	CzPlatformAudio::setSoundOn(bool on)
{
	SoundOn = on;
	if (!on)
		StopAllSounds();
}

void	CzPlatformAudio::setMusicOn(bool on)
{
	MusicOn = on;
	if (!on)
		StopMusic();
}

int		CzPlatformAudio::Init()
{
	if (g_CzShowTracing > 1)
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising IwSound");

	// initialise IwSound
	IwSoundInit();

	// Turn music and sound on
	SoundOn = true;
	MusicOn = true;

	// Used to convert our WAV's located in the resource group files
#ifdef IW_BUILD_RESOURCES
	IwGetResManager()->AddHandler(new CIwResHandlerWAV);
#endif

	Initialised = true;

	return 1;
}

void	CzPlatformAudio::Release()
{
	StopAllSounds();
	StopMusic();
	IwSoundTerminate();
}

void	CzPlatformAudio::Update()
{
	IwGetSoundManager()->Update();
}

bool CzPlatformAudio::InitSound(void* memory_file, int memory_file_size, CzSoundData& sound_data, CzSoundSpec& sound_spec)
{
	CzFile file;

	sound_data = (CzSoundData)CIwSoundWAV::Create("in_memory", memory_file, memory_file_size);
	if (sound_data == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "CzPlatformAudio::InitSound() - Could not create sound!");
		return false;
	}
	CIwSoundSpec* ss = new CIwSoundSpec();
	ss->SetData(static_cast<CIwSoundData*>(sound_data));
	sound_spec = static_cast<CzSoundSpec>(ss);

	return true;
}

CzSoundInst CzPlatformAudio::PlaySound(unsigned int name_hash, CzScene* scene, float volume, float pitch, float pan, bool looped)
{
	// find the sound effect
	CzSound* sound = (CzSound*)CzXomlResourceManager::FindResource(name_hash, CzHashes::Sound_Hash, scene);
	if (sound == NULL)
		return NULL;
	
	return sound->Play(volume, pitch, pan, looped);
}

CzSoundInst CzPlatformAudio::PlaySound(CzSoundSpec sound_spec, float volume, float pitch, float pan, bool looped)
{
	if (!isSoundOn())
		return NULL;

	CIwSoundParams params;
	params.m_Vol = (iwfixed)(volume * IW_GEOM_ONE);
	params.m_Pan = (iwfixed)(pan * IW_GEOM_ONE);
	params.m_Pitch = (iwfixed)(pitch * IW_GEOM_ONE);

	CIwSoundSpec* ss = static_cast<CIwSoundSpec*>(sound_spec);
	ss->setLooped(looped);
	CIwSoundInst* i = ss->Play(&params);

	setSoundVolume(i, volume);
	setSoundPitch(i, pitch);
	setSoundPan(i, pan);

	return (CzSoundInst)i;
}

void CzPlatformAudio::StopSound(CzSoundInst sound_inst)
{
	if (sound_inst == NULL)
		return;
	static_cast<CIwSoundInst*>(sound_inst)->Stop();
}

void CzPlatformAudio::PauseSound(CzSoundInst sound_inst)
{
	if (sound_inst == NULL)
		return;
	static_cast<CIwSoundInst*>(sound_inst)->Pause();
}

void CzPlatformAudio::ResumeSound(CzSoundInst sound_inst)
{
	if (sound_inst == NULL)
		return;
	static_cast<CIwSoundInst*>(sound_inst)->Resume();
}

void CzPlatformAudio::setSoundVolume(CzSoundInst sound_inst, float volume)
{
	if (sound_inst == NULL)
		return;
	static_cast<CIwSoundInst*>(sound_inst)->SetVol((iwfixed)(volume * IW_GEOM_ONE));
}

void CzPlatformAudio::setSoundPitch(CzSoundInst sound_inst, float pitch)
{
	if (sound_inst == NULL)
		return;
	static_cast<CIwSoundInst*>(sound_inst)->SetPitch((iwfixed)(pitch * IW_GEOM_ONE));
}

void CzPlatformAudio::setSoundPan(CzSoundInst sound_inst, float pan)
{
	if (sound_inst == NULL)
		return;
	static_cast<CIwSoundInst*>(sound_inst)->SetPan((iwfixed)(pan * IW_GEOM_ONE));
}

bool CzPlatformAudio::isSoundPlaying(CzSoundInst sound_inst) const
{
	if (sound_inst == NULL)
		return false;

	return static_cast<CIwSoundInst*>(sound_inst)->IsPlaying();
}

void CzPlatformAudio::StopAllSounds()
{
	// Stop all audio
	IwGetSoundManager()->PauseAll();
	IwGetSoundManager()->StopAll();
}

void CzPlatformAudio::PauseAllSounds()
{
	// Pause all sound effects
	IwGetSoundManager()->PauseAll();
}

void CzPlatformAudio::ResumeAllSounds()
{
	// Resume all sound effects
	IwGetSoundManager()->ResumeAll();
}

bool CzPlatformAudio::isMusicCodecSupported(eCzAudioCodec codec)
{
	return s3eAudioIsCodecSupported(getCodec(codec)) ? true : false;
}

bool CzPlatformAudio::PlayMusic(const char* name, int repeat_count)
{
	if (!isMusicOn())
		return false;

	if (s3eAudioPlay(name, repeat_count) != S3E_RESULT_SUCCESS)
		return false;

	return true;
}

bool CzPlatformAudio::PlayMusic(void* buffer, uint32 buffer_length, uint32 repeat_count)
{
	if (!isMusicOn())
		return false;

	if (s3eAudioPlayFromBuffer(buffer, buffer_length, repeat_count) != S3E_RESULT_SUCCESS)
		return false;

	return true;
}

void CzPlatformAudio::StopMusic()
{
	// Use s3eAudio to stop the devices music player from playing
	if (s3eAudioIsPlaying())
		s3eAudioStop();
}

void CzPlatformAudio::PauseMusic()
{
	// Use s3eAudio to pause the devices music player
	s3eAudioPause();
}

void CzPlatformAudio::ResumeMusic()
{
	// Use s3eAudio to resume the devices music player
	s3eAudioResume();
}

bool CzPlatformAudio::isMusicPlaying()
{
	return s3eAudioIsPlaying() ? true : false;
}

void CzPlatformAudio::setGlobalSoundVolume(float vol)
{
	s3eSoundSetInt(S3E_SOUND_VOLUME, (int)(vol * S3E_SOUND_MAX_VOLUME));
}

void CzPlatformAudio::setMusicVolume(float vol)
{
	s3eAudioSetInt(S3E_AUDIO_VOLUME, (int)(vol * S3E_AUDIO_MAX_VOLUME));
}

void CzPlatformAudio::DeleteSoundData(CzSoundData data)
{
	if (data != NULL)
		delete static_cast<CIwSoundData*>(data);
}
void CzPlatformAudio::DeleteSoundSpec(CzSoundSpec spec)
{
	if (spec != NULL)
		delete static_cast<CIwSoundData*>(spec);
}

