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
#include "CzPlatform.h"
#include "lua/CzLuaTimer.h"
#include "CzApp.h"
#include "CzPlatform.h"
#include "CzSettings.h"
#include "CzTimer.h"
#include "CzHashes.h"


//
// LUA_FindTimer timer-name (string), scene or actor (object, optional)
//
static int LUA_FindTimer(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "timer.find() not enough parameters, expected timer-name (string), scene or actor (object, optional)");
		lua_pushnil(lua);
		return 1;
	}

	IzXomlResource* container = NULL;
	const char* name = NULL;

	// Get the variable name
	if (lua_isstring(lua, 1))
		name = lua_tostring(lua, 1);
	if (name == NULL)
	{
        CzScriptEngineLua::DisplayError(lua, "timer.find() invalid name for Param0");
		lua_pushnil(lua);
		return 1;
	}

	if (count > 1)
	{
		// Get the container object
		if (lua_isuserdata(lua, 2))
			container = (IzXomlResource*)lua_touserdata(lua, 2);
		if (container != NULL && (container->getClassTypeHash() != CzHashes::Scene_Hash && container->getClassTypeHash() != CzHashes::Actor_Hash))
		{
			CzScriptEngineLua::DisplayError(lua, "timer.find() expected scene or actor for Param1");
			lua_pushnil(lua);
			return 1;
		}
	}

	// Locate timer
	CzXomlTimer* timer = CzTimersManager::FindTimer(name, container);
	if (timer == NULL)
	{
        CzScriptEngineLua::DisplayError(lua, "timer.find() timer not found - ", name);
		lua_pushnil(lua);
		return 1;
	}

	// Return the timer
	lua_pushlightuserdata(lua, timer);

    return 1;
}

//
// LUA_StopTimer object (timer)
//
static int LUA_StopTimer(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "timer.stop() not enough parameters, expected timer (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the program
	CzXomlTimer* timer = NULL;
	if (lua_isuserdata(lua, 1))
		timer = (CzXomlTimer*)lua_touserdata(lua, 1);
	if (timer == NULL)
	{
        CzScriptEngineLua::DisplayError(lua, "timer.stop() Invalid timer object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	timer->stop();
 
	lua_pushboolean(lua, true);
    return 1;
}

//
// LUA_StartTimer object (timer)
//
static int LUA_StartTimer(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "timer.start() not enough parameters, expected timer (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the program
	CzXomlTimer* timer = NULL;
	if (lua_isuserdata(lua, 1))
		timer = (CzXomlTimer*)lua_touserdata(lua, 1);
	if (timer == NULL)
	{
        CzScriptEngineLua::DisplayError(lua, "timer.start() Invalid timer object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	timer->start();
 
	lua_pushboolean(lua, true);
    return 1;
}

//
// LUA_RestartTimer object (timer)
//
static int LUA_RestartTimer(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "timer.restart() not enough parameters, expected timer (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the program
	CzXomlTimer* timer = NULL;
	if (lua_isuserdata(lua, 1))
		timer = (CzXomlTimer*)lua_touserdata(lua, 1);
	if (timer == NULL)
	{
        CzScriptEngineLua::DisplayError(lua, "timer.restart() Invalid timer object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	timer->restart();
 
	lua_pushboolean(lua, true);
    return 1;
}

//
// LUA_isTimerRunningobject (timer)
//
static int LUA_isTimerRunning(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "timer.running() not enough parameters, expected timer (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	CzXomlTimer* timer = NULL;
	if (lua_isuserdata(lua, 1))
		timer = (CzXomlTimer*)lua_touserdata(lua, 1);
	if (timer == NULL)
	{
        CzScriptEngineLua::DisplayError(lua, "timer.running() Invalid timer object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, timer->isRunning());
    return 1;
}

//
// Lua AppEasy System library
//
static const luaL_Reg g_timerlib[] = {
	{ "find",				LUA_FindTimer}, 
	{ "stop",				LUA_StopTimer}, 
	{ "start",				LUA_StartTimer}, 
	{ "restart",			LUA_RestartTimer}, 
	{ "running",			LUA_isTimerRunning}, 
	{NULL, NULL}
};

LUALIB_API int luaopen_timer(lua_State *L)
{
	luaL_register(L, "timer", g_timerlib);
	lua_pop(L, 1);

	return 1;
}


