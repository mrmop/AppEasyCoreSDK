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
#include "lua/CzLuaDisplay.h"
#include "CzApp.h"
#include "CzXomlResourceManager.h"

//
// LUA_GetDisplayWidth - returns display width
//
static int LUA_GetDisplayWidth(lua_State *lua)
{
	int count = lua_gettop(lua);

	// Get the main game object
	CzApp* app = (CzApp*)CZ_GLOBAL_RESOURCES->getResourceManager()->getParent();

	lua_pushnumber(lua, app->getScreenSize().x);

	return 1;
}

//
// LUA_GetDisplayHeight - returns display height
//
static int LUA_GetDisplayHeight(lua_State *lua)
{
	int count = lua_gettop(lua);

	// Get the main game object
	CzApp* app = (CzApp*)CZ_GLOBAL_RESOURCES->getResourceManager()->getParent();

	lua_pushnumber(lua, app->getScreenSize().y);

	return 1;
}

//
// LUA_GetDisplayOrientation - returns display orientation
//
static int LUA_GetDisplayOrientation(lua_State *lua)
{
	int count = lua_gettop(lua);

	// Get the main game object
	CzApp* app = (CzApp*)CZ_GLOBAL_RESOURCES->getResourceManager()->getParent();

	lua_pushnumber(lua, app->getScreenOrientation());

	return 1;
}


static const luaL_Reg g_displaylib[] =
{
	{ "width",			LUA_GetDisplayWidth}, 
	{ "height",			LUA_GetDisplayHeight},
	{ "orientation",	LUA_GetDisplayOrientation},
	{NULL, NULL}
};

LUALIB_API int luaopen_display(lua_State *L)
{
	luaL_register(L, "display", g_displaylib);
	lua_pop(L, 1);

	return 1;
}


