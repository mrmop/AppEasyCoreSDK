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
#include "CzActor.h"
#include "CzScene.h"
#include "CzProgram.h"
#include "CzUIBase.h"
#include "CzBrush.h"
#include "CzApp.h"
#include "CzTemplates.h"
#include "CzHashes.h"

//
// LUA_FindTimeline timeline-name (string), scene or actor (object, optional)
//
static int LUA_FindTimeline(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "timeline.find() not enough parameters, expected timeline-name (string), scene or actor (object, optional)");
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
        CzScriptEngineLua::DisplayError(lua, "timeline.find() invalid name for Param0");
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
			CzScriptEngineLua::DisplayError(lua, "timeline.find() expected scene or actor for Param1");
			lua_pushnil(lua);
			return 1;
		}
	}

	// Locate timeline
	CzAnimTimeline* timeline = CzAnimTimelinesManager::FindTimeline(name, container);
	if (timeline == NULL)
	{
        CzScriptEngineLua::DisplayError(lua, "timeline.find() timeline not found - ", name);
		lua_pushnil(lua);
		return 1;
	}

	// Return the timeline
	lua_pushlightuserdata(lua, timeline);

    return 1;
}

//
// LUA_StopTimeline object (timeline)
//
static int LUA_StopTimeline(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "timeline.stop() not enough parameters, expected timeline (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the program
	CzAnimTimeline* timeline = NULL;
	if (lua_isuserdata(lua, 1))
		timeline = (CzAnimTimeline*)lua_touserdata(lua, 1);
	if (timeline == NULL)
	{
        CzScriptEngineLua::DisplayError(lua, "timeline.stop() Invalid timeline object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	timeline->stop();
 
	lua_pushboolean(lua, true);
    return 1;
}

//
// LUA_PlayTimeline object (timeline)
//
static int LUA_PlayTimeline(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "timeline.play() not enough parameters, expected timeline (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the program
	CzAnimTimeline* timeline = NULL;
	if (lua_isuserdata(lua, 1))
		timeline = (CzAnimTimeline*)lua_touserdata(lua, 1);
	if (timeline == NULL)
	{
        CzScriptEngineLua::DisplayError(lua, "timeline.play() Invalid timeline object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	timeline->play();
 
	lua_pushboolean(lua, true);
    return 1;
}

//
// LUA_RestartTimeline object (timeline)
//
static int LUA_RestartTimeline(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "timeline.restart() not enough parameters, expected timeline (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the program
	CzAnimTimeline* timeline = NULL;
	if (lua_isuserdata(lua, 1))
		timeline = (CzAnimTimeline*)lua_touserdata(lua, 1);
	if (timeline == NULL)
	{
        CzScriptEngineLua::DisplayError(lua, "timeline.restart() Invalid timeline object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	timeline->restart();
 
	lua_pushboolean(lua, true);
    return 1;
}

//
// LUA_PauseTimeline object (timeline)
//
static int LUA_PauseTimeline(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "timeline.pause() not enough parameters, expected timeline (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the program
	CzAnimTimeline* timeline = NULL;
	if (lua_isuserdata(lua, 1))
		timeline = (CzAnimTimeline*)lua_touserdata(lua, 1);
	if (timeline == NULL)
	{
        CzScriptEngineLua::DisplayError(lua, "timeline.pause() Invalid timeline object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	timeline->pause();
 
	lua_pushboolean(lua, true);
    return 1;
}

//
// LUA_isTimelinePlaying object (timeline)
//
static int LUA_isTimelinePlaying(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "timeline.playing() not enough parameters, expected timeline (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	CzAnimTimeline* timeline = NULL;
	if (lua_isuserdata(lua, 1))
		timeline = (CzAnimTimeline*)lua_touserdata(lua, 1);
	if (timeline == NULL)
	{
        CzScriptEngineLua::DisplayError(lua, "timeline.playing() Invalid timeline object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, timeline->isPlaying());
    return 1;
}

//
// LUA_isTimelineAddAnim timeline-name (string), scene or actor (object, optional)
//
/*static int LUA_isTimelineAddAnim(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
        CzScriptEngineLua::DisplayError(lua, "timeline.add() not enough parameters, expected timeline (object), animation (object");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	CzAnimTimeline* timeline = NULL;
	if (lua_isuserdata(lua, 1))
		timeline = (CzAnimTimeline*)lua_touserdata(lua, 1);
	if (timeline == NULL)
	{
        CzScriptEngineLua::DisplayError(lua, "timeline.add() Invalid timeline object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	CzAnim* anim = NULL;
	if (lua_isuserdata(lua, 1))
		anim = (CzAnim*)lua_touserdata(lua, 1);
	if (anim == NULL)
	{
        CzScriptEngineLua::DisplayError(lua, "timeline.add() Invalid anim object for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}



    return 1;

}*/

static const luaL_Reg g_timelinelib[] =
{
	{ "find",				LUA_FindTimeline}, 
	{ "stop",				LUA_StopTimeline}, 
	{ "play",				LUA_PlayTimeline}, 
	{ "pause",				LUA_PauseTimeline}, 
	{ "restart",			LUA_RestartTimeline}, 
	{ "playing",			LUA_isTimelinePlaying}, 
//	{ "add",				LUA_isTimelineAddAnim}, 
	{NULL, NULL}
};

LUALIB_API int luaopen_timeline(lua_State *L)
{
	luaL_register(L, "timeline", g_timelinelib);
	lua_pop(L, 1);

	return 1;
}
