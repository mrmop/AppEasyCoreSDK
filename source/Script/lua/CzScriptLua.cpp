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
#include "lua/CzLuaSys.h"
#include "lua/CzLuaAction.h"
#include "lua/CzLuaActor.h"
#include "lua/CzLuaBrush.h"
#include "lua/CzLuaCamera.h"
#include "lua/CzLuaDisplay.h"
#include "lua/CzLuaFacebook.h"
#include "lua/CzLuaFont.h"
#include "lua/CzLuaHttp.h"
#include "lua/CzLuaImage.h"
#include "lua/CzLuaInput.h"
#include "lua/CzLuaMarket.h"
#include "lua/CzLuaMatrix.h"
#include "lua/CzLuaMedia.h"
#include "lua/CzLuaParticles.h"
#include "lua/CzLuaPhysics.h"
#include "lua/CzLuaProgram.h"
#include "lua/CzLuaResource.h"
#include "lua/CzLuaScene.h"
#include "lua/CzLuaShape.h"
#include "lua/CzLuaTemplate.h"
#include "lua/CzLuaTimeline.h"
#include "lua/CzLuaTimer.h"
#include "lua/CzLuaVariable.h"
#include "lua/CzLuaUserProps.h"
#include "lua/CzLuaXml.h"
#include "lua/CzLuaHttp.h"
#include "lua/CzLuaAds.h"
#include "CzActor.h"
#include "CzScene.h"
#include "lsqlite3.h"



//
//
//
//
// CzScriptEngineLua implementation
//
//
//
//

int CzScriptEngineLua::Init(CzScene* scene)
{
	// Open LUA
	Lua = lua_open();
	if (Lua == NULL)
		return -1;

	// Add LUA libraries
	luaL_openlibs(Lua);

	// Add functions
	luaopen_sys(Lua);
	luaopen_actions(Lua);
	luaopen_actor(Lua);
	luaopen_brush(Lua);
	luaopen_camera(Lua);
	luaopen_display(Lua);
	luaopen_facebook(Lua);
	luaopen_font(Lua);
	luaopen_http(Lua);
	luaopen_image(Lua);
	luaopen_input(Lua);
	luaopen_market(Lua);
	luaopen_matrix(Lua);
	luaopen_media(Lua);
	luaopen_particles(Lua);
	luaopen_physics(Lua);
	luaopen_program(Lua);
	luaopen_resource(Lua);
	luaopen_scene(Lua);
	luaopen_shape(Lua);
	luaopen_template(Lua);
	luaopen_timeline(Lua);
	luaopen_timer(Lua);
	luaopen_userprops(Lua);
	luaopen_variable(Lua);
	luaopen_xml(Lua);
	luaopen_lsqlite3(Lua);
	luaopen_http(Lua);
	luaopen_ads(Lua);

	// Set the scene
	Scene = scene;
	Lua->user_data = (void*)scene;

	return 1;
}

void CzScriptEngineLua::Release()
{
	// Shut down the lua state
	if (Lua != NULL)
	{
//		lua_gc(Lua, LUA_GCCOLLECT, 0);
		lua_close(Lua);
		Lua = NULL;
	}
}

int CzScriptEngineLua::LoadScript(const char* script, int script_len, const char* name)
{
	if (Lua == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Script - No Lua script engine attached to the scene, please using ScriptEngine=\"Lua\" scene tag to attach one - ", name);
		return -1;
	}

	// Pass the string to lua to execute
	if (luaL_loadbuffer(Lua, script, script_len, name) == 0)
	{
		if (lua_pcall(Lua, 0, 0, 0) != 0)
		{
			// Output the error
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "LUA ERROR: ", lua_tostring(Lua, -1));

			// Pop error message off the stack
			lua_pop(Lua, 1);
//			lua_close(Lua);
//			Lua = NULL;

			return -1;
		}
	}
	else
	{
		// Output the error
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "LUA ERROR: ", lua_tostring(Lua, -1));

		// Pop error message off the stack
		lua_pop(Lua, 1);
//		lua_close(Lua);
//		Lua = NULL;

		return -1;
	}

	return 1;
}

int CzScriptEngineLua::CallFunction(IzXomlResource* object, CzString* function_name, CzString* param1, CzString* param2, CzString* param3)
{
	if (Lua == NULL)
	{
		// Check global script engine
		IzScriptEngine* se = CZ_GLOBAL_RESOURCES->getScriptEngine();
		if (se != this && se != NULL)
			return se->CallFunction(object, function_name, param1, param2, param3);

		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Script - No Lua script engine attached to the scene, trying to call script function - ", function_name->c_str());
		return -1;
	}

	if (function_name == NULL || function_name->c_str() == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Script - Invalid Lua script function name");
		return -1;
	}

	int count = lua_gettop(Lua);
	int ret = 0;

	int pcount = 1;
	lua_getglobal(Lua, function_name->c_str());				// Push function name that we want to call onto the stack
	if (lua_isnil(Lua, -1))
	{
		lua_pop(Lua, 1);

		// Check global script engine
		IzScriptEngine* se = CZ_GLOBAL_RESOURCES->getScriptEngine();
		if (se != this && se != NULL)
			return se->CallFunction(object, function_name, param1, param2, param3);

		CzString error = "Script - function ";
		error += function_name->c_str();
		error += " does not exist";
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, error.c_str());
		return ret;
	}

	lua_pushlightuserdata(Lua, (void*)object);				// Push the reference object onto the stack
	if (param1 != NULL)
	{
		lua_pushstring(Lua, param1->c_str());
		pcount++;
	}
	if (param2 != NULL)
	{
		lua_pushstring(Lua, param2->c_str());
		pcount++;
	}
	if (param3 != NULL)
	{
		lua_pushstring(Lua, param3->c_str());
		pcount++;
	}
 
	if (lua_pcall(Lua, pcount, 1, 0) != 0)
	{
		CzString error = "Script - Calling script - ";
		error += function_name->c_str();
		error += "(";
		if (param1 != NULL)
		{
			error += param1->c_str();
			error += ", ";
		}
		if (param2 != NULL)
		{
			error += param2->c_str();
			error += ", ";
		}
		if (param3 != NULL)
			error += param3->c_str();
		error += ") - ";
		error += lua_tostring(Lua, -1);
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, error.c_str());
	}
	if (lua_isnumber(Lua, -1))
	{
		// Get the result returned from the LUA function
		ret = (int)lua_tonumber(Lua, -1);
	}
	else
	{
		CzString error = "Script - Function ";
		error += function_name->c_str();
		error += " should return a number";
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, error.c_str());
		ret = 0;
	}

	lua_pop(Lua, 1);

	return ret;
}

int CzScriptEngineLua::CallFunctionRef(int function_ref, CzString* param1, CzString* param2, CzString* param3, CzString* param4)
{
	if (Lua == NULL)
	{
		// Check global script engine
		IzScriptEngine* se = CZ_GLOBAL_RESOURCES->getScriptEngine();
		if (se != this && se != NULL)
			return se->CallFunctionRef(function_ref, param1, param2, param3, param4);

		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Script - No Lua script engine attached to the scene, trying to call script function ref");
		return -1;
	}

	if (function_ref < 0)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Script - Invalid Lua script function ref");
		return -1;
	}

	int pcount = 0;
	lua_rawgeti(Lua, LUA_REGISTRYINDEX, function_ref);
	if (param1 != NULL)
	{
		lua_pushstring(Lua, param1->c_str());
		pcount++;
	}
	if (param2 != NULL)
	{
		lua_pushstring(Lua, param2->c_str());
		pcount++;
	}
	if (param3 != NULL)
	{
		lua_pushstring(Lua, param3->c_str());
		pcount++;
	}
	if (param4 != NULL)
	{
		lua_pushstring(Lua, param4->c_str());
		pcount++;
	}
 
	if (lua_pcall(Lua, pcount, 0, 0) != 0)
	{
		CzString error = "Script - Calling script function ref - ";
		error += lua_tostring(Lua, -1);
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, error.c_str());
		lua_pop(Lua, 1);
	}

	return 0;
}

int CzScriptEngineLua::CallFunctionRefWithObject(void* object, int function_ref, CzString* param1, CzString* param2, CzString* param3, CzString* param4)
{
	if (Lua == NULL)
	{
		// Check global script engine
		IzScriptEngine* se = CZ_GLOBAL_RESOURCES->getScriptEngine();
		if (se != this && se != NULL)
			return se->CallFunctionRefWithObject(object, function_ref, param1, param2, param3, param4);

		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Script - No Lua script engine attached to the scene, trying to call script function ref");
		return -1;
	}

	if (function_ref < 0)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Script - Invalid Lua script function ref");
		return -1;
	}

	int pcount = 1;
	lua_rawgeti(Lua, LUA_REGISTRYINDEX, function_ref);
	lua_pushlightuserdata(Lua, (void*)object);				// Push the reference object onto the stack
	if (param1 != NULL)
	{
		lua_pushstring(Lua, param1->c_str());
		pcount++;
	}
	if (param2 != NULL)
	{
		lua_pushstring(Lua, param2->c_str());
		pcount++;
	}
	if (param3 != NULL)
	{
		lua_pushstring(Lua, param3->c_str());
		pcount++;
	}
	if (param4 != NULL)
	{
		lua_pushstring(Lua, param4->c_str());
		pcount++;
	}
 
	if (lua_pcall(Lua, pcount, 0, 0) != 0)
	{
		CzString error = "Script - Calling script function ref - ";
		error += lua_tostring(Lua, -1);
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, error.c_str());
		lua_pop(Lua, 1);
	}

	return 0;
}

CzXomlProperty CzScriptEngineLua::getVariable(CzString* var_name)
{
	CzXomlProperty p;

	lua_getglobal(Lua, var_name->c_str());				// Push variable name onto stack
	if (lua_isnil(Lua, -1))
	{
		// Check global script engine
		IzScriptEngine* se = CZ_GLOBAL_RESOURCES->getScriptEngine();
		if (se != this && se != NULL)
		{
			lua_pop(Lua, 1);
			return se->getVariable(var_name);
		}

		// Output the error
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "LUA ERROR: ", lua_tostring(Lua, -1));

		lua_pop(Lua, 1);
	}
	else
	{
		int type = lua_type(Lua, -1);

		switch (type)
		{
		case LUA_TBOOLEAN:
			p.Type = PT_Bool;
			p.p_bool = lua_toboolean(Lua, -1) != 0;
			break;
		case LUA_TNUMBER:
			p.Type = PT_Float;
			p.p_float = (float)lua_tonumber(Lua, -1);
			break;
		case LUA_TVEC:
			{
				p.Type = PT_Vec4;
				const float* vec = lua_tovec(Lua, -1);
				p.p_vec[0] = *vec;
				p.p_vec[1] = *(vec + 1);
				p.p_vec[2] = *(vec + 2);
				p.p_vec[3] = *(vec + 3);
			}
			break;
		case LUA_TSTRING:
			p.Type = PT_String;
			p.p_data = (void*)lua_tostring(Lua, -1);
			break;
/*		case LUA_TLIGHTUSERDATA:
			p.Type = PT_Resource;
			p.p_data = (void*)lua_touserdata(Lua, -1);
			break;
		case LUA_TFUNCTION:
			p.Type = PT_Resource;
			p.p_data = (void*)lua_topointer(Lua, -1);
			break;
		case LUA_TTABLE:
			p.Type = PT_Resource;
			p.p_data = (void*)lua_topointer(Lua, -1);
			break;*/
		default:
			CzScriptEngineLua::DisplayError(Lua, "Script - Lua variable type not supported");
		}
	}

	lua_pop(Lua, 1);

	return p;
}

void CzScriptEngineLua::DisplayError(lua_State *lua, const char* message)
{
	if (g_CzShowTracing == 0)
		return;

	lua_Debug ar;
	lua_getstack(lua, 1, &ar);
	lua_getinfo(lua, "nSl", &ar);
	int line = ar.currentline;	

	CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, message, ar.source, ar.currentline);
}

void CzScriptEngineLua::DisplayError(lua_State *lua, const char* message, const char* data)
{
	if (g_CzShowTracing == 0)
		return;

	lua_Debug ar;
	lua_getstack(lua, 1, &ar);
	lua_getinfo(lua, "nSl", &ar);
	int line = ar.currentline;

	CzString error = message;
	error += data;

	CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, error.c_str(), ar.source, ar.currentline);
}


