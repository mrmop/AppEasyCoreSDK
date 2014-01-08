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
#include "CzActions.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzHashes.h"

//
// LUA_FindActions actions-name (string), scene or actor (object, optional)
//
static int LUA_FindActions(lua_State *lua)
{
	if (lua_gettop(lua) < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "actions.find() not enough parameters, expected actions-name (string), scene or actor (object, optional)");
		lua_pushnil(lua);
		return 1;
	}

	IzXomlResource* container = NULL;
	const char* name = NULL;

	// Get the actions name
	if (lua_isstring(lua, 1))
		name = lua_tostring(lua, 1);
	if (name == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "actions.find() Invalid name for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Get the container object
	if (lua_isuserdata(lua, 2))
		container = (IzXomlResource*)lua_touserdata(lua, 2);
	if (container != NULL && (container->getClassTypeHash() != CzHashes::Scene_Hash && container->getClassTypeHash() != CzHashes::Actor_Hash))
	{
		CzScriptEngineLua::DisplayError(lua, "actions.find() expected scene or actor for Param1");
		lua_pushnil(lua);
		return 1;
	}

	// Locate actions
	CzActions* actions = CzActionsManager::FindActions(name, container);
	if (actions == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "actions.find() actions not found - ", name);
		lua_pushnil(lua);
		return 1;
	}

	// Return the actions
	lua_pushlightuserdata(lua, actions);

	return 1;
}

//
// LUA_CallActions object (actions), target (scene or actor)
//
static int LUA_CallActions(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "actions.call() not enough parameters, expected actions (object), target-scene-actor (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	CzActions* actions = NULL;
	if (lua_isuserdata(lua, 1))
		actions = (CzActions*)lua_touserdata(lua, 1);
	if (actions == NULL || actions->getClassTypeHash() != CzHashes::Actions_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actions.call() Invalid actions object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target
	IzXomlResource* object = NULL;
	if (count > 1)
	{
		if (lua_isuserdata(lua, 2))
			object = (IzXomlResource*)lua_touserdata(lua, 2);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actions.call() Invalid target, expected scene or actor in Param1");
			lua_pushboolean(lua, false);
			return 1;
		}

		if (object->getClassTypeHash() != CzHashes::Actor_Hash && object->getClassTypeHash() != CzHashes::Scene_Hash)
		{
			CzScriptEngineLua::DisplayError(lua, "actions.call() Invalid target, expected scene or actor in Param1");
			lua_pushboolean(lua, false);
			return 1;
		}
	}

	actions->Execute(object);
 
	lua_pushboolean(lua, true);
	return 1;
}

//
// Lua Actions library
//
static const luaL_Reg g_actionslib[] =
{
	{ "find",		LUA_FindActions }, 
	{ "call",		LUA_CallActions }, 
	{NULL, NULL}
};

LUALIB_API int luaopen_actions(lua_State *L)
{
	luaL_register(L, "actions", g_actionslib);
	lua_pop(L, 1);

	return 1;
}
