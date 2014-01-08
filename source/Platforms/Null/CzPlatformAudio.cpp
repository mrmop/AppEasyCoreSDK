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

#include "CzPlatformAudio.h"
#include "CzXoml.h"
#include "CzHashes.h"
#include "CzFile.h"
#include "CzScene.h"
#include "CzAudio.h"
#include "CzXoml.h"
#include "CzXomlResourceManager.h"

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
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising Sound");

	// initialise IwSound

	// Turn music and sound on
	SoundOn = true;
	MusicOn = true;

	Initialised = true;

	return 1;
}

void	CzPlatformAudio::Release()
{
	StopAllSounds();
	StopMusic();

	// Terminate suond engine
}

void	CzPlatformAudio::Update()
{
	// Update suond engine
}

bool CzPlatformAudio::InitSound(void* memory_file, int memory_file_size, CzSoundData& sound_data, CzSoundSpec& sound_spec)
{
	CzFile file;

	// Create sound effect from in-memory file

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

	// Play the sound effect

	// Set default sound parameters

	return (CzSoundInst)NULL;
}

void CzPlatformAudio::StopSound(CzSoundInst sound_inst)
{
	if (sound_inst == NULL)
		return;
}

void CzPlatformAudio::PauseSound(CzSoundInst sound_inst)
{
	if (sound_inst == NULL)
		return;
}

void CzPlatformAudio::ResumeSound(CzSoundInst sound_inst)
{
	if (sound_inst == NULL)
		return;
}

void CzPlatformAudio::setSoundVolume(CzSoundInst sound_inst, float volume)
{
	if (sound_inst == NULL)
		return;
}

void CzPlatformAudio::setSoundPitch(CzSoundInst sound_inst, float pitch)
{
	if (sound_inst == NULL)
		return;
}

void CzPlatformAudio::setSoundPan(CzSoundInst sound_inst, float pan)
{
	if (sound_inst == NULL)
		return;
}

bool CzPlatformAudio::isSoundPlaying(CzSoundInst sound_inst) const
{
	if (sound_inst == NULL)
		return false;

	return false;
}

void CzPlatformAudio::StopAllSounds()
{
	// Stop all audio
}

void CzPlatformAudio::PauseAllSounds()
{
	// Pause all sound effects
}

void CzPlatformAudio::ResumeAllSounds()
{
	// Resume all sound effects
}

bool CzPlatformAudio::isMusicCodecSupported(eCzAudioCodec codec)
{
	return false;
}

bool CzPlatformAudio::PlayMusic(const char* name, int repeat_count)
{
	if (!isMusicOn())
		return false;

	// Play music file

	return true;
}

bool CzPlatformAudio::PlayMusic(void* buffer, uint32 buffer_length, uint32 repeat_count)
{
	if (!isMusicOn())
		return false;

	// Play music file from in-memory file

	return true;
}

void CzPlatformAudio::StopMusic()
{
}

void CzPlatformAudio::PauseMusic()
{
}

void CzPlatformAudio::ResumeMusic()
{
}

bool CzPlatformAudio::isMusicPlaying()
{
	return false;
}

void CzPlatformAudio::setGlobalSoundVolume(float vol)
{
}

void CzPlatformAudio::setMusicVolume(float vol)
{
}

void CzPlatformAudio::DeleteSoundData(CzSoundData data)
{
}

void CzPlatformAudio::DeleteSoundSpec(CzSoundSpec spec)
{
}

