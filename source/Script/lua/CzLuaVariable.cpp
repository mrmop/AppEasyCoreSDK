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
// LUA_FindVariable variable-name (string), container scene (object, optional)
//
static int LUA_FindVariable(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (lua_gettop(lua) < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.find() not enough parameters, expected variable-name (string), container scene (object, optional)");
		lua_pushnil(lua);
		return 1;
	}

	IzXomlResource* scene = NULL;
	const char* name = NULL;

	// Get the variable name
	if (lua_isstring(lua, 1))
		name = lua_tostring(lua, 1);
	if (name == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.find() Invalid name for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Get the container scene
	if (count > 1)
	{
		if (lua_isuserdata(lua, 2))
			scene = (IzXomlResource*)lua_touserdata(lua, 2);
		if (scene != NULL && scene->getClassTypeHash() != CzHashes::Scene_Hash)
		{
			CzScriptEngineLua::DisplayError(lua, "variable.find() expected scene for Param1");
			lua_pushnil(lua);
			return 1;
		}
	}
	else
		scene = (CzScene*)lua->user_data;

	// Locate variable
	CzXomlVariable* var = CzXomlVariableManager::FindVariable(name, scene);
	if (var == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.find() variable not found - ", name);
		lua_pushnil(lua);
		return 1;
	}

	// Return the variable
	lua_pushlightuserdata(lua, var);

	return 1;
}

//
// LUA_GetVariableSize variable (object)
//
static int LUA_GetVariableSize(lua_State *lua)
{
	if (lua_gettop(lua) < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.size() not enough parameters, expected variable (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get variable object
	CzXomlVariable* var = NULL;
	if (lua_isuserdata(lua, 1))
		var = (CzXomlVariable*)lua_touserdata(lua, 1);
	if (var == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.size() Invalid variable for Param0");
		lua_pushnil(lua);
		return 1;
	}

	if (!var->isArray())
		lua_pushinteger(lua, 1);
	else
		lua_pushinteger(lua, ((CzXomlVariableArray*)var)->getSize());

	return 1;
}

//
// LUA_GetVariableCount variable (object)
//
static int LUA_GetVariableCount(lua_State *lua)
{
	if (lua_gettop(lua) < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.count() not enough parameters, expected variable (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get variable object
	CzXomlVariable* var = NULL;
	if (lua_isuserdata(lua, 1))
		var = (CzXomlVariable*)lua_touserdata(lua, 1);
	if (var == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.count() Invalid variable for Param0");
		lua_pushnil(lua);
		return 1;
	}

	if (!var->isArray())
		lua_pushinteger(lua, 1);
	else
		lua_pushinteger(lua, ((CzXomlVariableArray*)var)->getCount());

	return 1;
}

//
// LUA_VariableIsArray variable (object)
//
static int LUA_VariableIsArray(lua_State *lua)
{
	if (lua_gettop(lua) < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.array() not enough parameters, expected variable (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get variable object
	CzXomlVariable* var = NULL;
	if (lua_isuserdata(lua, 1))
		var = (CzXomlVariable*)lua_touserdata(lua, 1);
	if (var == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.array() Invalid variable for Param0");
		lua_pushnil(lua);
		return 1;
	}

	lua_pushboolean(lua, var->isArray());

	return 1;
}

//
// LUA_GetValue variable (object), index (number, optional)
//
static int LUA_GetValue(lua_State *lua)
{
	if (lua_gettop(lua) < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.get() not enough parameters, expected variable (object), index (number, optional)");
		lua_pushnil(lua);
		return 1;
	}

	// Get variable object
	CzXomlVariable* var = NULL;
	if (lua_isuserdata(lua, 1))
		var = (CzXomlVariable*)lua_touserdata(lua, 1);
	if (var == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.get() Invalid variable for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Get variable index
	int index = 0;
	if (lua_gettop(lua) > 1)
	{
		if (lua_isnumber(lua, 2))
			index = (int)lua_tonumber(lua, 2);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "variable.get() Invalid value for Param1, expected number");
			lua_pushnil(lua);
			return 1;
		}
	}

	if (var->isArray())
	{
		if (index < 0 || index >= ((CzXomlVariableArray*)var)->getSize())
		{
			CzScriptEngineLua::DisplayError(lua, "variable.get() index is out of range");
			lua_pushnil(lua);
			return 1;
		}
		var = ((CzXomlVariableArray*)var)->getElement(index);
	}

	switch (var->Type)
	{
	case VT_String:
	case VT_XML:
		lua_pushstring(lua, (const char*)var->getValue().c_str());
		break;
	case VT_Bool:
		lua_pushboolean(lua, ((CzXomlVariableBool*)var)->NativeValue);
		break;
	case VT_Float:
		lua_pushnumber(lua, (double)((CzXomlVariableFloat*)var)->NativeValue);
		break;
	case VT_Int:
		lua_pushnumber(lua, (double)((CzXomlVariableInt*)var)->NativeValue);
		break;
	case VT_Vec2:
		{
			CzXomlVariableVec2* v = (CzXomlVariableVec2*)var;
			lua_pushvec(lua, v->NativeValue.x, v->NativeValue.y, 0, 0);
		}
		break;
	case VT_Vec3:
		{
			CzXomlVariableVec3* v = (CzXomlVariableVec3*)var;
			lua_pushvec(lua, v->NativeValue.x, v->NativeValue.y, v->NativeValue.y, 0);
		}
		break;
	case VT_Vec4:
		{
			CzXomlVariableVec4* v = (CzXomlVariableVec4*)var;
			lua_pushvec(lua, v->NativeValue.x, v->NativeValue.y, v->NativeValue.y, v->NativeValue.w);
		}
		break;
	case VT_Condition:
		lua_pushboolean(lua, ((CzXomlVariableCondition*)var)->isTrue());
		break;
	default:
		lua_pushnil(lua);
		break;
	}

	return 1;
}

//
// LUA_SetVariable variable (variable), value (any), index (number)
//
static int LUA_SetVariable(lua_State *lua)
{
	if (lua_gettop(lua) < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.set() not enough parameters, expected variable (variable), value (any), index (number, optional)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the variable
	CzXomlVariable* var = NULL;
	if (lua_isuserdata(lua, 1))
		var = (CzXomlVariable*)lua_touserdata(lua, 1);
	if (var == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.set() Invalid variable for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the value
	CzString value;
	if (!LUA_ValueToString(lua, 2, value))
	{
		CzScriptEngineLua::DisplayError(lua, "variable.set() value (param1) must be a string, number, boolean or vec - ", var->Name.c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the array (index if supplied)
	int index = -1;
	if (lua_gettop(lua) > 2)
	{
		if (lua_isnumber(lua, 3))
			index = (int)lua_tonumber(lua, 3);
	}

	// Set the value
	if (var->isArray())
	{
		if (index < 0 || index >= ((CzXomlVariableArray*)var)->getSize())
		{
			CzScriptEngineLua::DisplayError(lua, "variable.set() index is out of range");
			lua_pushboolean(lua, false);
			return 1;
		}
		var = ((CzXomlVariableArray*)var)->getElement(index);
	}

	var->setValue(value.c_str());

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_AddVariable variable (variable), value (any), index (number)
//
static int LUA_AddVariable(lua_State *lua)
{
	if (lua_gettop(lua) < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.add() not enough parameters, expected variable (variable), value (any), index (number)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the variable
	CzXomlVariable* var = NULL;
	if (lua_isuserdata(lua, 1))
		var = (CzXomlVariable*)lua_touserdata(lua, 1);
	if (var == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.add() Invalid variable for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the value
	CzString value;
	if (!LUA_ValueToString(lua, 2, value))
	{
		CzScriptEngineLua::DisplayError(lua, "variable.add() value (param1) must be a string, number, boolean or vec - ", var->Name.c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the array (index if supplied)
	int index = -1;
	if (lua_gettop(lua) > 2)
	{
		if (lua_isnumber(lua, 3))
			index = (int)lua_tonumber(lua, 3);
	}

	// Set the value
	if (var->isArray())
	{
		if (index < 0 || index >= ((CzXomlVariableArray*)var)->getSize())
		{
			CzScriptEngineLua::DisplayError(lua, "variable.set() index is out of range");
			lua_pushboolean(lua, false);
			return 1;
		}
		var = ((CzXomlVariableArray*)var)->getElement(index);
	}

	var->addValue(value.c_str());

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_AppendArray variable (variable), value (any), count (number)
//
static int LUA_AppendArray(lua_State *lua)
{
	if (lua_gettop(lua) < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.append() not enough parameters, expected variable (variable), value (any), count (number, optional)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the variable
	CzXomlVariable* var = NULL;
	if (lua_isuserdata(lua, 1))
		var = (CzXomlVariable*)lua_touserdata(lua, 1);
	if (var == NULL || !var->isArray())
	{
		CzScriptEngineLua::DisplayError(lua, "variable.append() Invalid variable for Param0, requires a XOML array variable");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the value
	CzString value;
	if (!LUA_ValueToString(lua, 2, value))
	{
		CzScriptEngineLua::DisplayError(lua, "variable.append() value (param1) must be a string, number, boolean or vec - ", var->Name.c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the array (index if supplied)
	int count = 1;
	if (lua_gettop(lua) > 2)
	{
		if (lua_isnumber(lua, 3))
			count = (int)lua_tonumber(lua, 3);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "variable.append() value (param2) must be a number - ", var->Name.c_str());
			lua_pushboolean(lua, false);
			return 1;
		}
	}

	for (int t = 0; t < count; t++)
	{
		// Set the value
		CzXomlVariable* var2 = NULL;
		switch (var->Type)
		{
		case VT_ArrayString:
			var2 = new CzXomlVariable();
			break;
		case VT_ArrayBool:
			var2 = new CzXomlVariableBool();
			break;
		case VT_ArrayFloat:
			var2 = new CzXomlVariableFloat();
			break;
		case VT_ArrayInt:
			var2 = new CzXomlVariableInt();
			break;
		}
		var2->setValue(value.c_str());

		// Append the variable
		((CzXomlVariableArray*)var)->addElement(var2);
	}

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_SaveVariable variable (object), index (number, optional)
//
static int LUA_SaveVariable(lua_State *lua)
{
	if (lua_gettop(lua) < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.save() not enough parameters, expected variable (object), index (number, optional)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get variable object
	CzXomlVariable* var = NULL;
	if (lua_isuserdata(lua, 1))
		var = (CzXomlVariable*)lua_touserdata(lua, 1);
	if (var == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.save() Invalid variable for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get variable index
	int index = 0;
	if (lua_gettop(lua) > 1)
	{
		if (lua_isnumber(lua, 2))
			index = (int)lua_tonumber(lua, 2);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "variable.save() Invalid value for Param1, expected number");
			lua_pushboolean(lua, false);
			return 1;
		}
	}

	if (var->isArray())
	{
		if (index < 0 || index >= ((CzXomlVariableArray*)var)->getSize())
		{
			CzScriptEngineLua::DisplayError(lua, "variable.save() index is out of range");
			lua_pushboolean(lua, false);
			return 1;
		}
		var = ((CzXomlVariableArray*)var)->getElement(index);
	}

	if (!var->isPersistent())
	{
		CzScriptEngineLua::DisplayError(lua, "variable.save() variable is not persistent, make persistent to enable save");
		lua_pushboolean(lua, false);
		return 1;
	}

	var->Save();

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_GetAsTable variable (object)
//
static int LUA_GetAsTable(lua_State *lua)
{
	if (lua_gettop(lua) < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.asTable() not enough parameters, expected variable (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get variable object
	CzXomlVariable* var = NULL;
	if (lua_isuserdata(lua, 1))
		var = (CzXomlVariable*)lua_touserdata(lua, 1);
	if (var == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "variable.asTable() Invalid variable for Param0");
		lua_pushnil(lua);
		return 1;
	}

	if (!var->isArray())
	{
		CzScriptEngineLua::DisplayError(lua, "variable.asTable() variable must be an array for Param0");
		lua_pushnil(lua);
	}

	CzXomlVariableArray* arr = (CzXomlVariableArray*)var;
	int count = arr->getSize();
	if (count == 0)
	{
		lua_pushnil(lua);
		return 1;
	}

	lua_createtable(lua, count, 0);
	int new_table = lua_gettop(lua);
	// Add to table
	for (int t = 0 ; t < count; t++)
	{
		CzXomlVariable* v = arr->getElement(t);
		if (v == NULL)
			lua_pushnil(lua);
		else
		{
			eCzXomlVariableType type = v->Type;
			if (type == VT_String)
				lua_pushstring(lua, v->getValue().c_str());
			else
			if (type == VT_Int)
				lua_pushnumber(lua, (lua_Number)((CzXomlVariableInt*)v)->NativeValue);
			else
			if (type == VT_Float)
				lua_pushnumber(lua, ((CzXomlVariableFloat*)v)->NativeValue);
			else
			if (type == VT_Bool)
				lua_pushboolean(lua, ((CzXomlVariableBool*)v)->NativeValue ? 1 : 0);
		}
		lua_rawseti(lua, new_table, t + 1);
	}

	return 1;
}

static const luaL_Reg g_variablelib[] =
{
	{ "find",			LUA_FindVariable}, 
	{ "size",			LUA_GetVariableSize}, 
	{ "count",			LUA_GetVariableCount}, 
	{ "array",			LUA_VariableIsArray}, 
	{ "get",			LUA_GetValue}, 
	{ "set",			LUA_SetVariable}, 
	{ "add",			LUA_AddVariable}, 
	{ "append",			LUA_AppendArray}, 
	{ "save",			LUA_SaveVariable}, 
	{ "asTable",		LUA_GetAsTable}, 
	{NULL, NULL}
};

LUALIB_API int luaopen_variable(lua_State *L)
{
	luaL_register(L, "variable", g_variablelib);
	lua_pop(L, 1);

	return 1;
}

