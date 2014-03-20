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
#include "CzXomlResourceManager.h"

//
// LUA_CreateFromTemplate template (object), container (object), templates parameters (table, optional)
//
static int LUA_CreateFromTemplate(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (lua_gettop(lua) < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "template.from() not enough parameters, expected template (object), scene (object), templates parameters (table, optional)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the template
	CzTemplate* temp = NULL;
	if (lua_isuserdata(lua, 1))
		temp = (CzTemplate*)lua_touserdata(lua, 1);
	if (temp == NULL || (temp != NULL && temp->getClassTypeHash() != CzHashes::Template_Hash))
	{
		CzScriptEngineLua::DisplayError(lua, "template.from() Invalid template for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the scene
	IzXomlResource* container = NULL;
	if (lua_isuserdata(lua, 2))
		container = (CzScene*)lua_touserdata(lua, 2);
/*	if (container)
	{
		CzScriptEngineLua::DisplayError(lua, "template.from() Invalid container for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}*/

	// Create a set of XML attributes that will replace the template parameters
	CzXmlNode* replacements = NULL;

	if (count > 2)
	{
		if (lua_istable(lua, 3))
		{
			replacements = new CzXmlNode();
			replacements->Managed = false;

			// Table is in the stack at index 't'
			lua_pushnil(lua);  // First key
			while (lua_next(lua, 3) != 0)
			{
				// Get param name and value
				const char* name = lua_tostring(lua, -2);
				const char* value = lua_tostring(lua, -1);

				if (name != NULL && value != NULL)
				{
					// Set base template paramater
					CzXmlAttribute* index_attrib = new CzXmlAttribute();
					index_attrib->Managed = false;
					index_attrib->setName(name);
					index_attrib->setValue(value);
					replacements->AddAttribute(index_attrib);
				}

				// Removes value but keeps key for the next iteration
				lua_pop(lua, 1);
			}
		}
		else
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "template.from() param2 should be of type table - ", temp->getName().c_str());
			lua_pushboolean(lua, false);
			return 1;
		}
	}

	if (!temp->Instantiate(container, replacements))
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "template.from() could not instantiate from items template - ", temp->getName().c_str());
#endif	// _DEBUG
		SAFE_DELETE(replacements)
		lua_pushboolean(lua, false);
		return 1;
	}

	SAFE_DELETE(replacements)
	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_DestroyTemplate(template (object))
//
static int LUA_DestroyTemplate(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (lua_gettop(lua) < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "template.destroy() not enough parameters, expected template (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::Template_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "template.destroy() Invalid object, expected a template in Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	object->getParent()->removeResource(object);

	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_FindTemplate template-name (string), container scene (object, optional)
//
static int LUA_FindTemplate(lua_State *lua)
{
	int count = lua_gettop(lua);

	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "template.find() not enough parameters, expected template-name (string), container scene (object, optional)");
		lua_pushnil(lua);
		return 1;
	}

	IzXomlResource* scene = NULL;
	const char* name = NULL;

	// Get the resource name
	if (lua_isstring(lua, 1))
		name = lua_tostring(lua, 1);
	if (name == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "template.find() Invalid name for Param0");
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
			CzScriptEngineLua::DisplayError(lua, "template.find() expected scene for Param1");
			lua_pushnil(lua);
			return 1;
		}
	}
	else
		scene = (CzScene*)lua->user_data;

	// Locate resource
	IzXomlResource* resource = CzXomlResourceManager::FindResource(name, CzHashes::Template_Hash, scene);
	if (resource == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "template.find() template not found - ", name);
		lua_pushnil(lua);
		return 1;
	}

	// Return the resource
	lua_pushlightuserdata(lua, resource);

	return 1;
}


static const luaL_Reg g_templatelib[] =
{
	{ "find",		LUA_FindTemplate}, 
	{ "destroy",	LUA_DestroyTemplate}, 
	{ "from",		LUA_CreateFromTemplate}, 
	{NULL, NULL}
};

LUALIB_API int luaopen_template(lua_State *L)
{
	luaL_register(L, "template", g_templatelib);
	lua_pop(L, 1);

	return 1;
}
