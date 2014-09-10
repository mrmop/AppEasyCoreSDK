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

#include "IzPlatformSys.h"
#include "CzScriptLua.h"
#include "IzPlatformOther.h"
#include "CzPlatform.h"
#include "lua/CzLuaSys.h"
#include "CzApp.h"
#include "CzPlatform.h"
#include "CzSettings.h"
#include "s3eFile.h"

//
// LUA_isType(type-name)
//
static int LUA_isType(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (lua_gettop(lua) < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "sys.isType() not enough parameters, expected type-name");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the resource
	IzXomlResource* resource = NULL;
	if (lua_isuserdata(lua, 1))
		resource = (CzScene*)lua_touserdata(lua, 1);
	else
	{
        CzScriptEngineLua::DisplayError(lua, "sys.isTypeOf() Invalid parameter, expected object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the type of actor to create
	const char* type = NULL;
	if (lua_isstring(lua, 2))
		type = lua_tostring(lua, 2);
	else
	{
        CzScriptEngineLua::DisplayError(lua, "sys.isTypeOf() Invalid parameter, expected type-name for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}
	unsigned int type_hash = CZ_HASH(type);

	if (resource->getActualClassTypeHash() == type_hash)
		lua_pushboolean(lua, true);
	else
		lua_pushboolean(lua, false);


	return 1;
}

//
// LUA_isTypeOf(type-name)
//
static int LUA_isTypeOf(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (lua_gettop(lua) < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "sys.isTypeOf() not enough parameters, expected type-name");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the resource
	IzXomlResource* resource = NULL;
	if (lua_isuserdata(lua, 1))
		resource = (CzScene*)lua_touserdata(lua, 1);
	else
	{
        CzScriptEngineLua::DisplayError(lua, "sys.isTypeOf() Invalid parameter, expected object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the type of actor to create
	const char* type = NULL;
	if (lua_isstring(lua, 2))
		type = lua_tostring(lua, 2);
	else
	{
        CzScriptEngineLua::DisplayError(lua, "sys.isTypeOf() Invalid parameter, expected type-name for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}
	unsigned int type_hash = CZ_HASH(type);


	if (resource->getClassTypeHash() == type_hash)
		lua_pushboolean(lua, true);
	else
		lua_pushboolean(lua, false);


	return 1;
}

//
// LUA_LaunchURL - Launches a URL
//
static int LUA_LaunchURL(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "sys.launchURL() not enough parameters, expected url (string) ");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the url
	const char* url = NULL;
	if (lua_isstring(lua, 1))
		url = lua_tostring(lua, 1);
	if (url == NULL)
	{
        CzScriptEngineLua::DisplayError(lua, "sys.launchURL(),  invalid url for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	PLATFORM_SYS->ExecuteViaOS(url, false);

	lua_pushboolean(lua, true);
    return 1;
}

//
// LUA_Exit - Exit the program
//
static int LUA_Exit(lua_State *lua)
{
	int count = lua_gettop(lua);
	PLATFORM_SYS->RequestQuit();

	lua_pushboolean(lua, true);
    return 1;
}

//
// LUA_getTimeMs - Returns current time in millseconds
//
static int LUA_getTimeMs(lua_State *lua)
{
	int count = lua_gettop(lua);
	lua_pushnumber(lua, (double)PLATFORM_SYS->getTimeInMs());
    return 1;
}

//
// LUA_getTimeUTC - Returns coordinated universal time in millseconds
//
static int LUA_getTimeUTC(lua_State *lua)
{
	int count = lua_gettop(lua);
	lua_pushnumber(lua, (double)PLATFORM_SYS->getTimeUTC());
    return 1;
}

//
// LUA_getTotalFrames - Returns total frames that have been processed since the app started
//
static int LUA_getTotalFrames(lua_State *lua)
{
	lua_pushnumber(lua, (double)CzGlobals::App->getTotalFrames());
    return 1;
}

//
// LUA_getFrameRate - Returns the current estimated frame rate
//
static int LUA_getFrameRate(lua_State *lua)
{
	lua_pushnumber(lua, (double)CzGlobals::App->getFrameRate());
    return 1;
}

//
// LUA_PauseTime - Pauses time in the app
//
static int LUA_PauseTime(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "sys.pauseTime() not enough parameters, expected paused (boolean) ");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the state
	bool paused = false;
	if (lua_isboolean(lua, 1))
		paused = lua_toboolean(lua, 1) != 0;
	else
	{
        CzScriptEngineLua::DisplayError(lua, "sys.pauseTime(),  invalid paused state for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzGlobals::App->setTimePaused(paused);

	lua_pushboolean(lua, true);
    return 1;
}

//
// LUA_isTimePaused - Returns the current time paused state of the App
//
static int LUA_isTimePaused(lua_State *lua)
{
	lua_pushboolean(lua, CzGlobals::App->isTimePaused());
    return 1;
}

//
// LUA_getDeviceType - Returns device type
//
static int LUA_getDeviceType(lua_State *lua)
{
	int count = lua_gettop(lua);

	eCzDeviceType type = PLATFORM_SYS->getDeviceType();
	switch (type)
	{
	case CzDeviceType_iPhone:
		lua_pushstring(lua, "iphone");
		break;
	case CzDeviceType_iPad:
		lua_pushstring(lua, "ipad");
		break;
	case CzDeviceType_Android:
		lua_pushstring(lua, "android");
		break;
	case CzDeviceType_QNX:
		lua_pushstring(lua, "blackberry");
		break;
	case CzDeviceType_Windows:
		lua_pushstring(lua, "windows");
		break;
	case CzDeviceType_OSX:
		lua_pushstring(lua, "osx");
		break;
	case CzDeviceType_WP8:
		lua_pushstring(lua, "wp8");
		break;
	case CzDeviceType_WP81:
		lua_pushstring(lua, "wp81");
		break;
	case CzDeviceType_WS8:
		lua_pushstring(lua, "ws8");
		break;
	case CzDeviceType_WS81:
		lua_pushstring(lua, "ws81");
		break;
	case CzDeviceType_TIZEN:
		lua_pushstring(lua, "tizen");
		break;
	case CzDeviceType_ROKU:
		lua_pushstring(lua, "roku");
		break;
	default:
		lua_pushstring(lua, "unsupported");
	}

    return 1;
}

//
// LUA_getOSVersion - Returns OS version (return the devices OS version)
//
static int LUA_getOSVersion(lua_State *lua)
{
	int count = lua_gettop(lua);

	const char* version = PLATFORM_SYS->getDeviceOSVersion();
	lua_pushstring(lua, version);

    return 1;
}

//
// LUA_getOSVersionInt - Returns OS version (return the devices OS version as integer)
//
static int LUA_getOSVersionInt(lua_State *lua)
{
	int count = lua_gettop(lua);

	int version = PLATFORM_SYS->getDeviceOSVersionInt();
	lua_pushnumber(lua, version);

    return 1;
}

//
// LUA_getLocale - Returns devices language (ISO 639 and ISO 3166 formats, e.g. en_GB)
//
static int LUA_getLocale(lua_State *lua)
{
	int count = lua_gettop(lua);

	const char* locale = PLATFORM_SYS->getDeviceLocale();
	lua_pushstring(lua, locale);

    return 1;
}

//
// LUA_getFreeMem - Returns current free memory
//
static int LUA_getFreeMem(lua_State *lua)
{
	int count = lua_gettop(lua);
	lua_pushnumber(lua, (double)PLATFORM_SYS->getFreeMemory());
    return 1;
}

//
// LUA_StartVibrate - Starts vibration - duration (number), priority (number, optional)
//
static int LUA_StartVibrate(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "sys.vibrate() not enough parameters, expected duration (number), priority (number 0 to 255, optional)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get duration
	float duration = 0.5;
	if (lua_isnumber(lua, 1))
		duration = (float)lua_tonumber(lua, 1);
	else
	{
        CzScriptEngineLua::DisplayError(lua, "sys.vibrate(), expected number for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get priority
	float priority = 255.0f;
	if (count > 1)
	{
		if (lua_isnumber(lua, 2))
			priority = (float)lua_tonumber(lua, 2);
		else
		{
	        CzScriptEngineLua::DisplayError(lua, "sys.vibrate(), expected number for Param1");
			lua_pushboolean(lua, false);
			return 1;
		}
	}

	PLATFORM->getOther()->VibrateStart(priority, duration);

	lua_pushboolean(lua, true);
    return 1;
}

//
// LUA_StopVibrate - Returns current free memory
//
static int LUA_StopVibrate(lua_State *lua)
{
	int count = lua_gettop(lua);

	PLATFORM->getOther()->VibrateStop();

	lua_pushboolean(lua, true);
    return 1;
}

//
// LUA_ChangePowerSaving - command (string (start, stop))
//
static int LUA_ChangePowerSaving(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "sys.powerSaving() not enough parameters, expected enable (boolean)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get command
	bool enabled = false;
	if (lua_isboolean(lua, 1))
		enabled = lua_toboolean(lua, 1) != 0;
	else
	{
		CzScriptEngineLua::DisplayError(lua, "sys.powerSaving() invalid enable (Param0), expected boolean");
		lua_pushboolean(lua, false);
		return 1;
	}

	CZ_SETTINGS->setPreventPowerSaving(!enabled);

	lua_pushboolean(lua, true);
 
    return 1;
}

//
// LUA_YieldToOS - Yields to OS (time in millseonds (number))
//
static int LUA_YieldToOS(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "sys.yield() not enough parameters, expected time (number)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get command
	float time = 0;
	if (lua_isnumber(lua, 1))
		time = lua_tonumber(lua, 1) != 0;
	else
	{
		CzScriptEngineLua::DisplayError(lua, "sys.yield() invalid time (Param0), expected number");
		lua_pushboolean(lua, false);
		return 1;
	}

	PLATFORM_SYS->YieldToOS((int)time);

	lua_pushboolean(lua, true);
 
    return 1;
}

//
// LUA_LoadXoml - Loads XOML (filename (string), scene (object, optional))
//
static int LUA_LoadXoml(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "sys.loadXoml() not enough parameters, expected filename (string), scene (object, optional)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get filename
	const char* filename = NULL;
	if (lua_isstring(lua, 1))
		filename = lua_tostring(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "sys.loadXoml() invalid filename (Param0), expected string");
		lua_pushboolean(lua, false);
		return 1;
	}

	IzXomlResource* container = NULL;
	if (count > 1)
	{
		// Get scene
		if (lua_isuserdata(lua, 2))
			container = (IzXomlResource*)lua_touserdata(lua, 2);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "sys.loadXoml() invalid scene (Param1), expected scene object");
			lua_pushboolean(lua, false);
			return 1;
		}
	}
	if (container == NULL)
		container = CzGlobals::App;

	lua_pushboolean(lua, CZ_XOML->Process(container, filename));
 
    return 1;
}

//
// LUA_setBackgroundColour - Sets background clear colour (red, green, blue)
//
static int LUA_setBackgroundColour(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 3)
	{
        CzScriptEngineLua::DisplayError(lua, "sys.setBackgroundColour() not enough parameters, expected red (number), green (number), blue (number)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get colour cponents
	int red = 0;
	if (lua_isnumber(lua, 1))
		red = (int)lua_tonumber(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "sys.setBackgroundColour() invalid value (Param0), expected number");
		lua_pushboolean(lua, false);
		return 1;
	}
	int green = 0;
	if (lua_isnumber(lua, 2))
		green = (int)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "sys.setBackgroundColour() invalid value (Param1), expected number");
		lua_pushboolean(lua, false);
		return 1;
	}
	int blue = 0;
	if (lua_isnumber(lua, 3))
		blue = (int)lua_tonumber(lua, 3);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "sys.setBackgroundColour() invalid value (Param2), expected number");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzGlobals::App->SetBackgroundColour(red, green, blue, 255);

	lua_pushboolean(lua, true);
 
    return 1;
}

//
// LUA_listDirectory - lists directory contents. if passed path is nil then lists drives instead
//
static int LUA_listDirectory(lua_State *lua)
{	
	const char* dir = NULL;

	int count = lua_gettop(lua);
	if (count > 0)
	{
		// Get directory name
		if (lua_isstring(lua, 1))
			dir = lua_tostring(lua, 1);
	}

	//get file list
	s3eFileList* list = s3eFileListDirectory(dir);
	if (list==NULL)
	{
		switch (s3eFileGetError())
		{
		case S3E_FILE_ERR_NOT_FOUND:
			CzScriptEngineLua::DisplayError(lua, "sys.listDirectory() directory doesn't exist or doesn't contain any files.");
		case S3E_FILE_ERR_DEVICE:
			CzScriptEngineLua::DisplayError(lua, "sys.listDirectory() file device failed.");
		case S3E_FILE_ERR_TOO_MANY:
			CzScriptEngineLua::DisplayError(lua, "sys.listDirectory() too many file handles are open.");
		case S3E_FILE_ERR_MEM:
			CzScriptEngineLua::DisplayError(lua, "sys.listDirectory() couldn't allocate enough memory for the list.");
		}
		lua_pushnil(lua);
		return 1;
	}	

	//count items
	int file_count = 0;
	char buf[512];
	while (s3eFileListNext(list,&buf[0],512)==S3E_RESULT_SUCCESS)
		file_count+=1;
	s3eFileListClose(list);

	//get list again
	list = s3eFileListDirectory(dir);
	if (list==NULL)
	{
		switch (s3eFileGetError())
		{
		case S3E_FILE_ERR_NOT_FOUND:
			CzScriptEngineLua::DisplayError(lua, "sys.listDirectory() directory doesn't exist or doesn't contain any files.");
		case S3E_FILE_ERR_DEVICE:
			CzScriptEngineLua::DisplayError(lua, "sys.listDirectory() file device failed.");
		case S3E_FILE_ERR_TOO_MANY:
			CzScriptEngineLua::DisplayError(lua, "sys.listDirectory() too many file handles are open.");
		case S3E_FILE_ERR_MEM:
			CzScriptEngineLua::DisplayError(lua, "sys.listDirectory() couldn't allocate enough memory for the list.");
		}
		lua_pushnil(lua);
		return 1;
	}

	lua_createtable(lua, file_count, 0);
	int new_table = lua_gettop(lua);
	int index = 1;

	// push file names
	for (int i=0;i<file_count;i++)
	{
		s3eFileListNext(list,&buf[0],512);		
		lua_pushstring(lua, &buf[0]);
		lua_rawseti(lua, new_table, index++);	
	}
	s3eFileListClose(list);

	return 1;
}

//
// Lua AppEasy System library
//
static const luaL_Reg g_syslib[] = {
	{"isType",				LUA_isType}, 
	{"isTypeOf",			LUA_isTypeOf}, 
	{"launchURL",			LUA_LaunchURL}, 
	{"exit",				LUA_Exit}, 
	{"getTimeMs",			LUA_getTimeMs}, 
	{"getTimeUTC",			LUA_getTimeUTC}, 
	{"getTotalFrames",		LUA_getTotalFrames}, 
	{"getFrameRate",		LUA_getFrameRate}, 
	{"getDeviceType",		LUA_getDeviceType}, 
	{"getOSVersion",		LUA_getOSVersion}, 
	{"getOSVersionInt",		LUA_getOSVersionInt}, 
	{"getLocale",			LUA_getLocale}, 
	{"getFreeMem",			LUA_getFreeMem}, 
	{"vibrate",				LUA_StartVibrate}, 
	{"stopVibrate",			LUA_StopVibrate}, 
	{"changePowerSaving",	LUA_ChangePowerSaving}, 
	{"yield",				LUA_YieldToOS}, 
	{"loadXoml",			LUA_LoadXoml}, 
	{"pauseTime",			LUA_PauseTime}, 
	{"isTimePaused",		LUA_isTimePaused}, 
	{"setBackgroundColour",	LUA_setBackgroundColour}, 
	{"listDirectory",	LUA_listDirectory}, 	
	{NULL, NULL}
};

LUALIB_API int luaopen_sys(lua_State *L)
{
	luaL_register(L, "sys", g_syslib);
	lua_pop(L, 1);

	return 1;
}


