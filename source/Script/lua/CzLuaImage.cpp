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
// LUA_ImageGetSize image (object) - (returns width, height)
//
static int LUA_ImageGetSize(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "image.getSize() not enough parameters, expected image (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the image object
	CzImage* image = NULL;
	if (lua_isuserdata(lua, 1))
		image = (CzImage*)lua_touserdata(lua, 1);
	if (image == NULL || image->getClassTypeHash() != CzHashes::Image_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "image.getSize() Invalid image object, expected image (param0)");
		lua_pushnil(lua);
		return 1;
	}

	lua_pushvec(lua, (float)image->getWidth(), (float)image->getHeight(), 0, 0);

	return 1;
}

//
// LUA_ImageReload image (object), filename (string), blocking (boolean, optional)
//
static int LUA_ImageReload(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "image.reload() not enough parameters, expected image (object), filename (string), blocking (boolean, optional)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the image object
	CzImage* image = NULL;
	if (lua_isuserdata(lua, 1))
		image = (CzImage*)lua_touserdata(lua, 1);
	if (image == NULL || image->getClassTypeHash() != CzHashes::Image_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "image.reload() Invalid image object, expected image (param0)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the filename
	const char* filename = NULL;
	if (lua_isstring(lua, 2))
		filename = lua_tostring(lua, 2);
	if (filename == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "image.reload() Invalid filename, expected string (param1)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the filename
	bool blocking = true;
	if (count > 2)
	{
		if (lua_isboolean(lua, 3))
			blocking = lua_toboolean(lua, 3) != 0;
		else
		{
			CzScriptEngineLua::DisplayError(lua, "image.reload() Invalid value for blocking, expected boolean (param2)");
			lua_pushboolean(lua, false);
			return 1;
		}
	}

	lua_pushboolean(lua, image->Reload(filename, blocking));

	return 1;
}


//
// Lua Image library
//
static const luaL_Reg g_imagelib[] =
{
	{"getSize",			LUA_ImageGetSize},
	{"reload",			LUA_ImageReload},
	{NULL, NULL}
};

LUALIB_API int luaopen_image(lua_State *L)
{
	luaL_register(L, "image", g_imagelib);
	lua_pop(L, 1);
  
	return 1;
}

