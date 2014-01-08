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
// LUA_SetUserProperty user-property (object), value (string)
//
static int LUA_SetUserProperty(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "userprop.set() not enough parameters, user-property (object), value (string, number, boolean or vec)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	CzXomlNamedProperty* prop = NULL;
	if (lua_isuserdata(lua, 1))
		prop = (CzXomlNamedProperty*)lua_touserdata(lua, 1);
	if (prop == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "userprop.set() Invalid target object, expected user property for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzXomlProperty prop2;
	if (!LUA_ValueToProperty(lua, 2, prop2))
	{
		CzScriptEngineLua::DisplayError(lua, "userprop.set() value must be a string, number, boolean or vec for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, prop->Property->setValue(prop2, false));
 
	return 1;
}

//
// LUA_AddUserProperty user-property (object), value (string)
//
static int LUA_AddUserProperty(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "userprop.add() not enough parameters, user-property (object), value (string)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	CzXomlNamedProperty* prop = NULL;
	if (lua_isuserdata(lua, 1))
		prop = (CzXomlNamedProperty*)lua_touserdata(lua, 1);
	if (prop == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "userprop.add() Invalid target object, expected user property for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzXomlProperty prop2;
	if (!LUA_ValueToProperty(lua, 2, prop2))
	{
		CzScriptEngineLua::DisplayError(lua, "userprop.add() value must be a string, number, boolean or vec for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, prop->Property->setValue(prop2, true));
 
	return 1;

}

//
// LUA_GetUserProperty user-property (object)
//
static int LUA_GetUserProperty(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "userprop.get() not enough parameters, user-property (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the target object
	CzXomlNamedProperty* nprop = NULL;
	if (lua_isuserdata(lua, 1))
		nprop = (CzXomlNamedProperty*)lua_touserdata(lua, 1);
	if (nprop == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "userprop.get() Invalid target object, expected user property for Param0");
		lua_pushnil(lua);
		return 1;
	}
	CzXomlProperty* prop = nprop->Property;
	LUA_ReturnProperty(lua, *prop);

	return 1;
}


//
// LUA_FindUserProperty user-property-list (object), property-name (string)
//
static int LUA_FindUserProperty(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "userprop.find() not enough parameters, user-property-list (object), property-name (string)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the target object
	CzUserPropertyList* props = NULL;
	if (lua_isuserdata(lua, 1))
		props = (CzUserPropertyList*)lua_touserdata(lua, 1);
	if (props == NULL || props->getClassTypeHash() != CzHashes::UserProperties_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "userprop.find() Invalid target object, expected user property list for Param0");
		lua_pushnil(lua);
		return 1;
	}

	CzString name;
	if (!LUA_ValueToString(lua, 2, name))
	{
		CzScriptEngineLua::DisplayError(lua, "userprop.find() name must be a string for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushlightuserdata(lua, props->findProperty(name.getHash()));
 
	return 1;
}

//
// Lua User Properties library
//
static const luaL_Reg g_userpropslib[] =
{
	{ "set",		LUA_SetUserProperty }, 
	{ "add",		LUA_AddUserProperty }, 
	{ "get",		LUA_GetUserProperty }, 
	{ "find",		LUA_FindUserProperty }, 
	{NULL, NULL}
};

LUALIB_API int luaopen_userprops(lua_State *L)
{
	luaL_register(L, "userprops", g_userpropslib);
	lua_pop(L, 1);
  
	return 1;
}
