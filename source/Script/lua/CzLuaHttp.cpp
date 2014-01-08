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
#include "IzPlatformComms.h"
#include "CzScene.h"
#include "CzHashes.h"
#include "CzApp.h"
#include "CzRemoteReq.h"

//
// LUA_HttpFindRequest request-name (string) - (returns the remote request)
//
static int LUA_HttpFindRequest(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "http.find() not enough parameters, expected remote-request-name (string), scene (object, optional)");
		lua_pushnil(lua);
		return 1;
	}

	IzXomlResource* container = NULL;
	const char* name = NULL;

	// Get the programs name
	if (lua_isstring(lua, 1))
		name = lua_tostring(lua, 1);
	if (name == NULL)
	{
        CzScriptEngineLua::DisplayError(lua, "http.find() Invalid name, expected string for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Get the container scene
	if (count > 1)
	{
		if (lua_isuserdata(lua, 2))
			container = (IzXomlResource*)lua_touserdata(lua, 2);
		if (container != NULL && container->getClassTypeHash() != CzHashes::Scene_Hash)
		{
			CzScriptEngineLua::DisplayError(lua, "http.find() expected scene for Param1");
			lua_pushnil(lua);
			return 1;
		}
	}
	else
		container = (CzScene*)lua->user_data;

	// Locate request
	IzXomlResource* rr = CzXomlResourceManager::FindResource(name, CzHashes::RemoteReq_Hash, container);
	if (rr == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "http.find() remote request not found - ", name);
		lua_pushnil(lua);
		return 1;
	}

	// Return the program
	lua_pushlightuserdata(lua, rr);

    return 1;}

//
// LUA_HttpSetProperty request (object), property (string), value (any)
//
static int LUA_HttpSetProperty(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 3)
	{
		CzScriptEngineLua::DisplayError(lua, "http.set() not enough parameters, expected remote-request (object), property (string), value (any)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::RemoteReq_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "http.set() Invalid target object for Param0, expected remote-request");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzString prop_name;
	if (lua_isstring(lua, 2))
		prop_name = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "http.set() property must be a property name (Param1) - object - ", object->getName().c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	CzXomlProperty prop;
	if (!LUA_ValueToProperty(lua, 3, prop))
	{
		CzScriptEngineLua::DisplayError(lua, "http.set() value must be a string, number, boolean or vec (Param2) - object - ", object->getName().c_str());
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "- property - ", prop_name.c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, ((CzRemoteReq*)object)->setProperty(prop_name.getHash(), prop, false));
 
	return 1;
}

//
// LUA_HttpGetProperty request (object), property (string) - (returns the requested property)
//
static int LUA_HttpGetProperty(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "http.get() not enough parameters, expected remote-request (object), property (string)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the target object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::RemoteReq_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "http.get() Invalid target object for Param0, expected remote-request");
		lua_pushnil(lua);
		return 1;
	}

	CzString prop_name;
	if (lua_isstring(lua, 2))
		prop_name = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "http.get() property must be a property name (Param1) - object - ", object->getName().c_str());
		lua_pushnil(lua);
		return 1;
	}

	CzXomlProperty prop;
	if (!((CzRemoteReq*)object)->getProperty(prop_name.getHash(), prop))
	{
		CzScriptEngineLua::DisplayError(lua, "http.get() remote-request property does not exist - ", prop_name.c_str());
		lua_pushnil(lua);
		return 1;
	}
	LUA_ReturnProperty(lua, prop);

	return 1;
}

//
// LUA_HttpSend request (object) - (returns true if success)
//
static int LUA_HttpSend(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "http.send() not enough parameters, expected remote-request (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::RemoteReq_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "http.send() Invalid target object for Param0, expected remote-request");
		lua_pushboolean(lua, false);
		return 1;
	}

	((CzRemoteReq*)object)->MakeRequest();

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_HttpResponseCode - (returns last request response code)
//
static int LUA_HttpResponseCode(lua_State *lua)
{
	int count = lua_gettop(lua);
	lua_pushnumber(lua, PLATFORM_COMMS->HttpGetResponseCode(CZ_HTTP_MANAGER->getHttpObject()));

	return 1;
}

//
// LUA_HttpResponseHeader header-name (string) - (returns the requested header)
//
static int LUA_HttpResponseHeader(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "http.responseHeader() not enough parameters, expected header-name (string)");
		lua_pushnil(lua);
		return 1;
	}

	CzString header_name;
	if (lua_isstring(lua, 1))
		header_name = lua_tostring(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "http.responseHeader() header must be a string (Param1)");
		lua_pushnil(lua);
		return 1;
	}

	CzString header_data;
	if (PLATFORM_COMMS->HttpGetHeader(CZ_HTTP_MANAGER->getHttpObject(), header_name.c_str(), header_data))
		lua_pushnil(lua);

	lua_pushstring(lua, header_data.c_str());

	return 1;
}


//
// Lua http library
//
static const luaL_Reg g_httplib[] =
{
	{"find",			LUA_HttpFindRequest},
	{"set",				LUA_HttpSetProperty},
	{"get",				LUA_HttpGetProperty},
	{"send",			LUA_HttpSend},
	{"responseCode",	LUA_HttpResponseCode},
	{"responseHeader",	LUA_HttpResponseHeader},
	{NULL, NULL}
};

LUALIB_API int luaopen_http(lua_State *L)
{
	luaL_register(L, "http", g_httplib);
	lua_pop(L, 1);

	return 1;
}

