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

//
// LUA_isMultiTouch - Determines if multi-touch is supported
//
static int LUA_isMultiTouch(lua_State *lua)
{
	int count = lua_gettop(lua);
	lua_pushboolean(lua, CZ_INPUT->isMultiTouch());
	return 1;
}

//
// LUA_hasCompass - Determines if compass is supported
//
static int LUA_hasCompass(lua_State *lua)
{
	int count = lua_gettop(lua);
	lua_pushboolean(lua, CZ_INPUT->isCompassAvailable());
	return 1;
}

//
// LUA_hasAccelerometer - Determines if accelerometer is supported
//
static int LUA_hasAccelerometer(lua_State *lua)
{
	int count = lua_gettop(lua);
	lua_pushboolean(lua, CZ_INPUT->isAccelerometerAvailable());
	return 1;
}

//
// LUA_hasKeyboard - Determines if keyboard is supported
//
static int LUA_hasKeyboard(lua_State *lua)
{
	int count = lua_gettop(lua);
	lua_pushboolean(lua, CZ_INPUT->isKeysAvailable());
	return 1;
}

//
// LUA_StartCompass - Start the compass
//
static int LUA_StartCompass(lua_State *lua)
{
	int count = lua_gettop(lua);
	lua_pushboolean(lua, CZ_INPUT->startCompass());
	return 1;
}

//
// LUA_StopCompass - Start the compass
//
static int LUA_StopCompass(lua_State *lua)
{
	int count = lua_gettop(lua);
	CZ_INPUT->stopCompass();
	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_getCompassInfo - (returns heading_x, heading_y, heading_z, direction)
//
static int LUA_getCompassInfo(lua_State *lua)
{
	int count = lua_gettop(lua);
	CzVec3 compass = CZ_INPUT->getCompassHeading();
	lua_pushvec(lua, compass.x, compass.y, compass.z, CZ_INPUT->getCompassDirection());

	return 1;
}

//
// LUA_StartAccelerometer - Start the Accelerometer
//
static int LUA_StartAccelerometer(lua_State *lua)
{
	int count = lua_gettop(lua);
	lua_pushboolean(lua, CZ_INPUT->startAccelerometer());
	return 1;
}

//
// LUA_StopAccelerometer - Stop the keyboard
//
static int LUA_StopAccelerometer(lua_State *lua)
{
	int count = lua_gettop(lua);
	CZ_INPUT->startAccelerometer();
	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_getAccelerometerInfo - (returns heading_x, heading_y, heading_z, direction)
//
static int LUA_getAccelerometerInfo(lua_State *lua)
{
	int count = lua_gettop(lua);
	CzVec3 accelerometer = CZ_INPUT->getAccelerometerPosition();
	lua_pushvec(lua, accelerometer.x, accelerometer.y, accelerometer.z, 0);
	return 1;
}

//
// LUA_getAccelerometerOffset - (returns heading_x, heading_y, heading_z, direction)
//
static int LUA_getAccelerometerOffset(lua_State *lua)
{
	int count = lua_gettop(lua);
	CzVec3 accelerometer = CZ_INPUT->getAccelerometerOffset();
	lua_pushvec(lua, accelerometer.x, accelerometer.y, accelerometer.z, 0);
	return 1;
}

//
// LUA_setAccelerometerRef
//
static int LUA_setAccelerometerRef(lua_State *lua)
{
	int count = lua_gettop(lua);
	CZ_INPUT->setAccelerometerReference();
	return 1;
}

//
// LUA_isBackPressed - Returns back button pressed status
//
static int LUA_isBackPressed(lua_State *lua)
{
	int count = lua_gettop(lua);
	lua_pushboolean(lua, CZ_INPUT->isBackPressed());
	return 1;
}

//
// LUA_isMenuPressed - Returns menu button pressed status
//
static int LUA_isMenuPressed(lua_State *lua)
{
	int count = lua_gettop(lua);
	lua_pushboolean(lua, CZ_INPUT->isMenuPressed());
	return 1;
}

//
// LUA_getTextInput - Opens on screen keyboard to get text input from user
//
static int LUA_getTextInput(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "input.textInput() not enough parameters, expected message (string) ");
		lua_pushnil(lua);
		return 1;
	}

	// Get the message to display to user
	const char* message = NULL;
	const char* default_text = NULL;
	if (lua_isstring(lua, 1))
		message = lua_tostring(lua, 1);
	if (message == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "input.textInput() invalid message for Param0");
		lua_pushnil(lua);
		return 1;
	}

	if (count > 1)
	{
		if (lua_isstring(lua, 2))
			default_text = lua_tostring(lua, 2);
		if (default_text == NULL)
		{
			CzScriptEngineLua::DisplayError(lua, "input.textInput() invalid default_text for Param1");
			lua_pushnil(lua);
			return 1;
		}
	}

	const char* data = CZ_INPUT->showOnScreenKeyboard(message, 0, default_text);
	if (data == NULL)
		lua_pushnil(lua);
	else
		lua_pushstring(lua, data);

	return 1;
}

//
// LUA_getTouchInfo touch-index (number 0 to 4) - (returns x, y, touch_state)
//
static int LUA_getTouchInfo(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "input.touchInfo() not enough parameters, expected touch-index (number)");
		lua_pushnil(lua);
		return 1;
	}

	// Get index
	int index = 0;
	if (count > 0)
	{
		if (lua_isnumber(lua, 1))
			index = (int)lua_tonumber(lua, 1);
		if (index < 0 || index > 4)
		{
			CzScriptEngineLua::DisplayError(lua, "input.touchInfo() touch-index out of range, expected 0 to 4 for Param0");
			lua_pushnil(lua);
			return 1;
		}
	}

	CzTouch* touch = CZ_INPUT->getTouch(index);
	float touched = touch->touched ? 1.0f : 0.0f;
	lua_pushvec(lua, (float)touch->x, (float)touch->y, touched, (float)touch->id);

	return 1;
}

//
// LUA_getLastKeyPressed - (returns key code of last key pressed)
//
static int LUA_getLastKeyPressed(lua_State *lua)
{
	lua_pushinteger(lua, CZ_INPUT->getLastKey());
	return 1;
}

//
// LUA_isKeyUp key-code - (returns true or false)
//
static int LUA_isKeyUp(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "input.isKeyUp() not enough parameters, expected key-code (number)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get index
	int key_code = 0;
	if (lua_isnumber(lua, 1))
		key_code = (int)lua_tonumber(lua, 1);

	lua_pushboolean(lua, CZ_INPUT->isKeyUp(key_code));

	return 1;
}

//
// LUA_isKeyDown key-code - (returns true or false)
//
static int LUA_isKeyDown(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "input.isKeyDown() not enough parameters, expected key-code (number)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get index
	int key_code = 0;
	if (lua_isnumber(lua, 1))
		key_code = (int)lua_tonumber(lua, 1);

	lua_pushboolean(lua, CZ_INPUT->isKeyDown(key_code));

	return 1;
}


//
// Lua Input library
//
static const luaL_Reg g_inputlib[] =
{
	{"multitouch",			LUA_isMultiTouch},
	{"compass",				LUA_hasCompass},
	{"accelerometer",		LUA_hasAccelerometer},
	{"keyboard",			LUA_hasKeyboard},
	{"startCompass",		LUA_StartCompass},
	{"stopCompass",			LUA_StopCompass},
	{"getCompass",			LUA_getCompassInfo},
	{"startAccelerometer",	LUA_StartAccelerometer},
	{"stopAccelerometer",	LUA_StopAccelerometer},
	{"getAccelerometer",	LUA_getAccelerometerInfo},
	{"getAccelerometerOffset",	LUA_getAccelerometerOffset},
	{"setAccelerometerRef",	LUA_setAccelerometerRef},
	{"backPressed",			LUA_isBackPressed},
	{"menuPressed",			LUA_isMenuPressed},
	{"textInput",			LUA_getTextInput},
	{"touchInfo",			LUA_getTouchInfo},
	{"getKeyPressed",		LUA_getLastKeyPressed},
	{"isKeyUp",				LUA_isKeyUp},
	{"isKeyDown",			LUA_isKeyDown},
	{NULL, NULL}
};

LUALIB_API int luaopen_input(lua_State *L)
{
	luaL_register(L, "input", g_inputlib);
	lua_pop(L, 1);
  
	return 1;
}
