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
#include "lua/CzLuaGen.h"
#include "CzApp.h"
#include "CzPlatform.h"
#include "CzSettings.h"

//
//
// Common functions
//
//
bool LUA_ValueToString(lua_State *lua, int stack_index, CzString &string)
{
    if (lua_isnumber(lua, stack_index))
	{
        float n = (float)lua_tonumber(lua, stack_index);
		int in = (int)n;
		if (n == in)
			string = in;
		else
			string = n;
	}
	else
    if (lua_isboolean(lua, stack_index))
        string = lua_toboolean(lua, stack_index) != 0;
	else
    if (lua_isvec(lua, stack_index))
	{
        const float* v = lua_tovec(lua, stack_index);
		string = *v++;
		string += ",";
		string += *v++;
		string += ",";
		string += *v++;
		string += ",";
		string += *v;
	}
	else
    if (lua_isstring(lua, stack_index))
        string = lua_tostring(lua, stack_index);
	else
		return false;

	if (string.c_str() == NULL)
		return false;

	return true;
}

bool LUA_ValueToProperty(lua_State *lua, int stack_index, CzXomlProperty &prop)
{
    if (lua_isstring(lua, stack_index))
	{
		prop.Type = PT_String;
        prop.p_data = (void*)lua_tostring(lua, stack_index);
	}
	else
    if (lua_isnumber(lua, stack_index))
	{
		prop.Type = PT_Float;
		prop.p_float = (float)lua_tonumber(lua, stack_index);
	}
	else
    if (lua_isboolean(lua, stack_index))
	{
		prop.Type = PT_Bool;
		prop.p_bool = lua_toboolean(lua, stack_index) != 0;
	}
	else
    if (lua_isuserdata(lua, stack_index))
	{
		prop.Type = PT_Resource;
		prop.p_data = lua_touserdata(lua, stack_index);
	}
	else
    if (lua_isvec(lua, stack_index))
	{
		prop.Type = PT_Vec4;
        const float* v = lua_tovec(lua, stack_index);
		prop.p_vec[0] = *v++;
		prop.p_vec[1] = *v++;
		prop.p_vec[2] = *v++;
		prop.p_vec[3] = *v;
	}
	else
    if (lua_isnil(lua, stack_index))
	{
		prop.Type = PT_String;
        prop.p_data = NULL;
	}
	else
		return false;

	return true;
}

void LUA_ReturnProperty(lua_State *lua, CzXomlProperty &prop)
{
	switch (prop.Type)
	{
	case PT_String:
		lua_pushstring(lua, (const char*)prop.p_data);
		break;
	case PT_Bool:
		lua_pushboolean(lua, prop.p_bool);
		break;
	case PT_Float:
		lua_pushnumber(lua, (double)prop.p_float);
		break;
	case PT_Int:
		lua_pushnumber(lua, (double)prop.p_int);
		break;
	case PT_Vec2:
	case PT_Vec3:
	case PT_Vec4:
		lua_pushvec(lua, prop.p_vec[0], prop.p_vec[1], prop.p_vec[2], prop.p_vec[3]);
		break;
	case PT_Resource:
		lua_pushlightuserdata(lua, prop.p_data);
		break;
	default:
		lua_pushnil(lua);
		break;
	}
}

