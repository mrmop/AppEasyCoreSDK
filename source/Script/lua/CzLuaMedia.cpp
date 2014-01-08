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

#include "CzScriptLua.h"
#include "lua/CzLuaGen.h"
#include "IzPlatformAudio.h"
#include "IzPlatformVideo.h"
#include "CzPlatform.h"
#include "CzActor.h"
#include "CzScene.h"
#include "CzProgram.h"
#include "CzUIBase.h"
#include "CzBrush.h"
#include "CzApp.h"
#include "CzTemplates.h"
#include "CzCam.h"
#include "CzHashes.h"
#include "CzUIVideoOverlay.h"

//
// LUA_PlaySound sound (object), looped (boolean), volume (number), pitch (number), pan (number)
//
static int LUA_PlaySound(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "media.playSound() not enough parameters, expected sound (object), looped (boolean, optional), (volume, pitch, pan optional number)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the target object
	CzSound* sound = NULL;
	if (lua_isuserdata(lua, 1))
		sound = (CzSound*)lua_touserdata(lua, 1);
	if (sound == NULL || sound->getClassTypeHash() != CzHashes::Sound_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "media.playSound() Invalid sound object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	bool looped = false;
	float volume = 1.0f;
	float pitch = 1.0f;
	float pan = 1.0f;

	if (count > 1)
	{
		if (lua_isboolean(lua, 2))
			looped = lua_toboolean(lua, 2) != 0;
		else
		{
			CzScriptEngineLua::DisplayError(lua, "media.playSound() looped must be a boolean (Param1) - ", sound->getName().c_str());
			lua_pushnil(lua);
			return 1;
		}
	}
	if (count > 2)
	{
		if (lua_isnumber(lua, 3))
			volume = (float)lua_tonumber(lua, 3);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "media.playSound() volume must be a number (Param2) - ", sound->getName().c_str());
			lua_pushnil(lua);
			return 1;
		}
	}
	if (count > 3)
	{
		if (lua_isnumber(lua, 4))
			pitch = (float)lua_tonumber(lua, 4);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "media.playSound() pitch must be a number (Param3) - ", sound->getName().c_str());
			lua_pushnil(lua);
			return 1;
		}
	}
	if (count > 4)
	{
		if (lua_isnumber(lua, 5))
			pan = (float)lua_tonumber(lua, 5);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "media.playSound() pan must be a number (Param4) - ", sound->getName().c_str());
			lua_pushnil(lua);
			return 1;
		}
	}

	CzSoundInst inst = sound->Play(volume, pitch, pan, looped);

	lua_pushlightuserdata(lua, inst);
 
	// Return the number of result arguments that were passed back to lua
	return 1;
}

//
// LUA_StopSound sound (object)
//
static int LUA_StopSound(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "media.stopSound() not enough parameters, expected sound-instance (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	CzSoundInst sound_instance = NULL;
	if (lua_isuserdata(lua, 1))
		sound_instance = (CzSoundInst)lua_touserdata(lua, 1);
	if (sound_instance == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "media.stopSound()  invalid sound-instance for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	PLATFORM_AUDIO->StopSound(sound_instance);
 
	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_PauseSound sound (object)
//
static int LUA_PauseSound(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "media.pauseSound() not enough parameters, expected sound-instance (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	CzSoundInst sound_instance = NULL;
	if (lua_isuserdata(lua, 1))
		sound_instance = (CzSoundInst)lua_touserdata(lua, 1);
	if (sound_instance == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "media.pauseSound()  invalid sound-instance for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	PLATFORM_AUDIO->PauseSound(sound_instance);
 
	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_ResumeSound sound (object)
//
static int LUA_ResumeSound(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "media.resumeSound() not enough parameters, expected sound-instance (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	CzSoundInst sound_instance = NULL;
	if (lua_isuserdata(lua, 1))
		sound_instance = (CzSoundInst)lua_touserdata(lua, 1);
	if (sound_instance == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "media.resumeSound()  invalid sound-instance for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	PLATFORM_AUDIO->ResumeSound(sound_instance);
 
	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_ChangeSound sound-instance (object), paramater-name (string), value (number)
//
static int LUA_ChangeSound(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 3)
	{
		CzScriptEngineLua::DisplayError(lua, "media.changeSound() not enough parameters, expected sound-instance (object) property (volume, pitch, pan), value (number)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	CzSoundInst sound_instance = NULL;
	if (lua_isuserdata(lua, 1))
		sound_instance = (CzSoundInst)lua_touserdata(lua, 1);
	if (sound_instance == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "media.changeSound()  invalid sound-instance for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get parameter to change
	const char* param = NULL;
	if (lua_isstring(lua, 2))
		param = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "media.changeSound() invalid sound parameter (Param1), expected property (string)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get value
	float value;
	if (lua_isnumber(lua, 3))
		value = (float)lua_tonumber(lua, 3);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "media.changeSound() invalid sound parameter value (Param2), expected value (number)");
		lua_pushboolean(lua, false);
		return 1;
	}

	unsigned int param_hash = CZ_HASH(param);
	if (param_hash == CzHashes::Volume_Hash)
		PLATFORM_AUDIO->setSoundVolume(sound_instance, value);
	else
	if (param_hash == CzHashes::Pitch_Hash)
		PLATFORM_AUDIO->setSoundPitch(sound_instance, value);
	else
	if (param_hash == CzHashes::Pan_Hash)
		PLATFORM_AUDIO->setSoundPan(sound_instance, value);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "media.changeSound() invalid sound parameter instance (Param1) - ", param);
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, true);
 
	// Return the number of result arguments that were passed back to lua
	return 1;
}

//
// LUA_isSoundPlaying sound-instance (object)
//
static int LUA_isSoundPlaying(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "media.soundPlaying() not enough parameters, expected sound-instance (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	CzSoundInst sound_instance = NULL;
	if (lua_isuserdata(lua, 1))
		sound_instance = (CzSoundInst)lua_touserdata(lua, 1);
	if (sound_instance == NULL)
	{
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, PLATFORM_AUDIO->isSoundPlaying(sound_instance));
 
	// Return the number of result arguments that were passed back to lua
	return 1;
}

//
// LUA_PlayMusic file-name (string)
//
static int LUA_PlayMusic(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "media.playMusic() not enough parameters, expected file-name (string), repeat-count (number, optional) ");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	const char* filename = NULL;
	if (lua_isstring(lua, 1))
		filename = lua_tostring(lua, 1);
	if (filename == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "media.playMusic() invalid music file-name for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	int repeat_count = 0;
	if (count > 1)
	{
		// Get the target object
		const char* filename = NULL;
		if (lua_isnumber(lua, 2))
			repeat_count = (int)lua_tonumber(lua, 2);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "media.playMusic() invalid value for repeat-count in Param1");
			lua_pushboolean(lua, false);
			return 1;
		}
	}

	lua_pushboolean(lua, PLATFORM_AUDIO->PlayMusic(filename, repeat_count));
 
	// Return the number of result arguments that were passed back to lua
	return 1;
}

//
// LUA_StopMusic - Stops music from playing
//
static int LUA_StopMusic(lua_State *lua)
{
	int count = lua_gettop(lua);
	PLATFORM_AUDIO->StopMusic();

	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_PauseMusic - Pauses music
//
static int LUA_PauseMusic(lua_State *lua)
{
	int count = lua_gettop(lua);
	PLATFORM_AUDIO->PauseMusic();

	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_ResumeMusic - resume music
//
static int LUA_ResumeMusic(lua_State *lua)
{
	int count = lua_gettop(lua);
	PLATFORM_AUDIO->ResumeMusic();

	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_isMusicPlaying - Checks if music is currently playing
//
static int LUA_isMusicPlaying(lua_State *lua)
{
	int count = lua_gettop(lua);
	lua_pushboolean(lua, PLATFORM_AUDIO->isMusicPlaying());
	return 1;
}

//
// LUA_SetMusicVolume - Changes the musics volume
//
static int LUA_SetMusicVolume(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "media.setMusicVolume() not enough parameters, expected volume (number) ");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get volume
	float volume;
	if (lua_isnumber(lua, 1))
		volume = (float)lua_tonumber(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "media.setMusicVolume() invalid volume parameter (Param0), expected (number)");
		lua_pushboolean(lua, false);
		return 1;
	}

	PLATFORM_AUDIO->setMusicVolume(volume);

	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_isAudioCodecSupported - Checks if music codec is supported
//
static int LUA_isAudioCodecSupported(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "media.audioCodecSupported() not enough parameters, expected codec (string) ");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	const char* codec = NULL;
	if (lua_isstring(lua, 1))
		codec = lua_tostring(lua, 1);
	if (codec == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "media.audioCodecSupported() invalid codec for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	unsigned int codec_hash = CZ_HASH(codec);
	eCzAudioCodec cod;

	if (codec_hash == CZ_HASH("midi"))
		cod = Audio_Codec_Midi;
	else
	if (codec_hash == CZ_HASH("mp3"))
		cod = Audio_Codec_Mp3;
	else
	if (codec_hash == CZ_HASH("aac"))
		cod = Audio_Codec_Aac;
	else
	if (codec_hash == CZ_HASH("aacplus"))
		cod = Audio_Codec_Aacplus;
	else
	if (codec_hash == CZ_HASH("qcp"))
		cod = Audio_Codec_Qcp;
	else
	if (codec_hash == CZ_HASH("pcm"))
		cod = Audio_Codec_Pcm;
	else
	if (codec_hash == CZ_HASH("spf"))
		cod = Audio_Codec_Spf;
	else
	if (codec_hash == CZ_HASH("amr"))
		cod = Audio_Codec_Amr;
	else
	if (codec_hash == CZ_HASH("mp4"))
		cod = Audio_Codec_Mp4;
	else
	{
		CzScriptEngineLua::DisplayError(lua, "media.audioCodecSupported() invalid codec for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, PLATFORM_AUDIO->isMusicCodecSupported(cod));
	return 1;
}

//
// LUA_isVideoCodecSupported - Checks if video codec is supported
//
static int LUA_isVideoCodecSupported(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "media.videoCodecSupported() not enough parameters, expected codec (string) ");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	const char* codec = NULL;
	if (lua_isstring(lua, 1))
		codec = lua_tostring(lua, 1);
	if (codec == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "media.videoCodecSupported() invalid codec for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	unsigned int codec_hash = CZ_HASH(codec);
	bool found = false;
	if (PLATFORM_VIDEO->isCodecSupported(codec_hash, found))
		lua_pushboolean(lua, true);
	else
	{
		if (!found)
			CzScriptEngineLua::DisplayError(lua, "media.videoCodecSupported() invalid codec for Param0");

		lua_pushboolean(lua, false);
	}

	return 1;
}

//
// LUA_ChangeVideoCam videocam-instance (object), command (string (start, stop))
//
static int LUA_ChangeVideoCam(lua_State *lua)
{
	if (!PLATFORM_CAM->isAvailable())
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "media.changeVideoCam(), video cam not available on this platform");
		lua_pushboolean(lua, false);
		return 1;
	}

	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "media.changeVideoCam() not enough parameters, expected video-cam (object), command (string (start, stop))");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	CzVideoCam* cam = NULL;
	if (lua_isuserdata(lua, 1))
		cam = (CzVideoCam*)lua_touserdata(lua, 1);
	if (cam == NULL || cam->getClassTypeHash() != CzHashes::VideoCam_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "media.changeVideoCam()  invalid video-cam object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get command
	const char* param = NULL;
	if (lua_isstring(lua, 2))
		param = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "media.changeVideoCam() invalid command (Param1)");
		lua_pushboolean(lua, false);
		return 1;
	}

	unsigned int param_hash = CZ_HASH(param);
	if (param_hash == CzHashes::Start_Hash)
		cam->Start();
	else
	if (param_hash == CzHashes::Stop_Hash)
		cam->Stop();
	else
	{
		CzScriptEngineLua::DisplayError(lua, "media.changeVideoCam() invalid command (Param1) - ", param);
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, true);
 
	// Return the number of result arguments that were passed back to lua
	return 1;
}

//
// LUA_SnapshotVideoCam filename (string with no extension), quality (number, optional))
//
static int LUA_SnapshotVideoCam(lua_State *lua)
{
	if (!PLATFORM_CAM->isAvailable())
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "media.snapshotVideoCam(), video cam not available on this platform");
		lua_pushboolean(lua, false);
		return 1;
	}

	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "media.snapshotVideoCam() not enough parameters, expected filename (string with no extension))");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get filename
	const char* filename = NULL;
	if (lua_isstring(lua, 1))
		filename = lua_tostring(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "media.snapshotVideoCam(), expected string for filename (Param0)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get quality
	float quality = 90.0f;
	if (lua_isnumber(lua, 2))
		quality = (float)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "media.snapshotVideoCam(), expected number for quality (Param1)");
		lua_pushboolean(lua, false);
		return 1;
	}

	if (PLATFORM_CAM->getImageBrush() != NULL)
	{
		CzString fn = filename;
		fn += ".jpg";
		PLATFORM_CAM->getImageBrush()->SaveJpg(fn.c_str(), (int)quality);
	}

	lua_pushboolean(lua, true);
 
	// Return the number of result arguments that were passed back to lua
	return 1;
}

//
// LUA_EnableMusic - Enables or disables music
//
static int LUA_EnableMusic(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "media.enableMusic() not enough parameters, expected enable (boolean) ");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	bool enable = true;
	if (lua_isboolean(lua, 1))
		enable = lua_toboolean(lua, 1) != 0;
	else
	{
		CzScriptEngineLua::DisplayError(lua, "media.enableMusic() expected boolean for enable (Param0)");
		lua_pushboolean(lua, false);
		return 1;
	}

	PLATFORM_AUDIO->setMusicOn(enable);

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_EnableSound - Enables or disables sound
//
static int LUA_EnableSound(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "media.enableSound() not enough parameters, expected enable (boolean) ");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	bool enable = true;
	if (lua_isboolean(lua, 1))
		enable = lua_toboolean(lua, 1) != 0;
	else
	{
		CzScriptEngineLua::DisplayError(lua, "media.enableSound() expected boolean for enable (Param0)");
		lua_pushboolean(lua, false);
		return 1;
	}

	PLATFORM_AUDIO->setSoundOn(enable);

	lua_pushboolean(lua, true);

	return 1;
}


static const luaL_Reg g_medialib[] =
{
	{"playSound",			LUA_PlaySound},
	{"stopSound",			LUA_StopSound},
	{"pauseSound",			LUA_PauseSound},
	{"resumeSound",			LUA_ResumeSound},
	{"changeSound",			LUA_ChangeSound},
	{"soundPlaying",		LUA_isSoundPlaying},
	{"playMusic",			LUA_PlayMusic},
	{"stopMusic",			LUA_StopMusic},
	{"pauseMusic",			LUA_PauseMusic},
	{"resumeMusic",			LUA_ResumeMusic},
	{"musicPlaying",		LUA_isMusicPlaying},
	{"setMusicVolume",		LUA_SetMusicVolume},
	{"audioCodecSupported", LUA_isAudioCodecSupported},
	{"videoCodecSupported", LUA_isVideoCodecSupported},
	{"changeVideoCam",		LUA_ChangeVideoCam},
	{"snapshotVideoCam",	LUA_SnapshotVideoCam},
	{"enableMusic",			LUA_EnableMusic},
	{"enableSound",			LUA_EnableSound},
	{NULL, NULL}
};

LUALIB_API int luaopen_media(lua_State *L)
{
	luaL_register(L, "media", g_medialib);
	lua_pop(L, 1);

	return 1;
}
