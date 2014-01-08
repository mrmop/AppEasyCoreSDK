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
#include "CzHashes.h"

//
// LUA_FindCzmera camera-name (string), scene (object, optional)
//
static int LUA_FindCamera(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
        CzScriptEngineLua::DisplayError(lua, "camera.find() not enough parameters, expected camera-name (string), scene (object, optional)");
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
        CzScriptEngineLua::DisplayError(lua, "camera.find() Invalid name for Param0");
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
			CzScriptEngineLua::DisplayError(lua, "camera.find() expected scene for Param1");
			lua_pushnil(lua);
			return 1;
		}
	}
	else
		container = (CzScene*)lua->user_data;

	// Locate camera
	IzXomlResource* cam = CzXomlResourceManager::FindResource(name, CzHashes::Camera_Hash, container);
	if (cam == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "camera.find() camera not found - ", name);
		lua_pushnil(lua);
		return 1;
	}

	// Return the program
	lua_pushlightuserdata(lua, cam);

    return 1;
}

//
// LUA_SetProperty camera (object), property (string), value (string)
//
static int LUA_SetPropertyCamera(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 3)
	{
		CzScriptEngineLua::DisplayError(lua, "camera.set() not enough parameters, expected camera (object), property (string), value (string, number, boolean or vec)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::Camera_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "camera.set() Invalid target object, expected camera");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzString prop_name, value;
	if (lua_isstring(lua, 2))
		prop_name = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "camera.set() property must be a property name - object - ", object->getName().c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	CzXomlProperty prop;
	if (!LUA_ValueToProperty(lua, 3, prop))
	{
		CzScriptEngineLua::DisplayError(lua, "camera.set() value must be a string, number, boolean or vec - object - ", object->getName().c_str());
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "- property - ", prop_name.c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, ((CzCamera*)object)->setProperty(prop_name.getHash(), prop, false));
 
	return 1;
}

//
// LUA_AddProperty camera (object), property (string), value (string)
//
static int LUA_AddPropertyCamera(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 3)
	{
		CzScriptEngineLua::DisplayError(lua, "camera.add() not enough parameters, expected camera (object), property (string), value (string, number, boolean or vec)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::Camera_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "camera.add() Invalid target object for Param0, expected camera");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzString prop_name, value;
	if (lua_isstring(lua, 2))
		prop_name = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "camera.add() property must be a property name (Param1) - object - ", object->getName().c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	CzXomlProperty prop;
	if (!LUA_ValueToProperty(lua, 3, prop))
	{
		CzScriptEngineLua::DisplayError(lua, "camera.add() value must be a string, number, boolean or vec - object - ", object->getName().c_str());
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "- property - ", prop_name.c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, ((CzCamera*)object)->setProperty(prop_name.getHash(), prop, true));
	
	return 1;
}

//
// LUA_GetProperty camera (object), property (string)
//
static int LUA_GetPropertyCamera(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "camera.get() not enough parameters, expected camera (object), property (string)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the target object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::Camera_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "camera.get() Invalid target object for Param0, expected camera");
		lua_pushnil(lua);
		return 1;
	}

	CzString prop_name, value;
	if (lua_isstring(lua, 2))
		prop_name = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "camera.get() property must be a property name (Param1) - object - ", object->getName().c_str());
		lua_pushnil(lua);
		return 1;
	}

	CzXomlProperty prop;
	if (!((CzCamera*)object)->getProperty(prop_name.getHash(), prop))
	{
		CzScriptEngineLua::DisplayError(lua, "camera.get() camera property does not exist - ", prop_name.c_str());
		lua_pushnil(lua);
		return 1;
	}
	LUA_ReturnProperty(lua, prop);

	return 1;
}

static const luaL_Reg g_cameralib[] =
{
	{"find",			LUA_FindCamera}, 
	{"set",				LUA_SetPropertyCamera}, 
	{"add",				LUA_AddPropertyCamera}, 
	{"get",				LUA_GetPropertyCamera}, 
	{NULL, NULL}
};


LUALIB_API int luaopen_camera(lua_State *L)
{
	luaL_register(L, "camera", g_cameralib);
	lua_pop(L, 1);

	return 1;
}


