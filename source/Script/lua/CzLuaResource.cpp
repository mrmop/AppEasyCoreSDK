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
// LUA_FindResource resource-name (string), resource_type (string), container scene (object, optional)
//
static int LUA_FindResource(lua_State *lua)
{
	int count = lua_gettop(lua);

	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "resource.find() not enough parameters, expected resource-name (string), resource_type (string), container scene (object, optional)");
		lua_pushnil(lua);
		return 1;
	}

	IzXomlResource* scene = NULL;
	const char* name = NULL;
	const char* type = NULL;

	// Get the resource name
	if (lua_isstring(lua, 1))
		name = lua_tostring(lua, 1);
	if (name == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "resource.find() Invalid name for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Get the resource type
	if (lua_isstring(lua, 2))
		type = lua_tostring(lua, 2);
	if (type == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "resource.find() Invalid type for Param1");
		lua_pushnil(lua);
		return 1;
	}

	// Get the container scene
	if (count > 2)
	{
		if (lua_isuserdata(lua, 3))
			scene = (IzXomlResource*)lua_touserdata(lua, 3);
		if (scene != NULL && scene->getClassTypeHash() != CzHashes::Scene_Hash)
		{
			CzScriptEngineLua::DisplayError(lua, "resource.find() expected scene for Param2");
			lua_pushnil(lua);
			return 1;
		}
	}
	else
		scene = (CzScene*)lua->user_data;

	// Locate resource
	IzXomlResource* resource = CzXomlResourceManager::FindResource(name, CZ_HASH(type), scene);
	if (resource == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "resource.find() resource not found - ", name);
		lua_pushnil(lua);
		return 1;
	}

	// Return the resource
	lua_pushlightuserdata(lua, resource);

	return 1;
}

//
// LUA_FindResourceOfType resource_type (string), container scene (object, optional) - returns table of found resources
//
static int LUA_FindResourceOfType(lua_State *lua)
{
	int count = lua_gettop(lua);

	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "resource.findOfType() not enough parameters, expected resource_type (string), container scene (object, optional)");
		lua_pushnil(lua);
		return 1;
	}

	IzXomlResource* scene = NULL;
	const char* type = NULL;

	// Get the resource type
	if (lua_isstring(lua, 1))
		type = lua_tostring(lua, 1);
	if (type == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "resource.findOfType() Invalid type for Param0");
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
			CzScriptEngineLua::DisplayError(lua, "resource.findOfType() expected scene for Param1");
			lua_pushnil(lua);
			return 1;
		}
	}
	else
		scene = (CzScene*)lua->user_data;

	CzVector<IzXomlResource*> resources;
	count = CzXomlResourceManager::FindResourcesOfType(CzString::CalculateHash(type), scene, resources);
	if (count == 0)
	{
		lua_pushnil(lua);
		return 1;
	}

	lua_createtable(lua, count, 0);
	int new_table = lua_gettop(lua);
	int index = 1;

	// Add actors to table
	for (CzVector<IzXomlResource*>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		lua_pushlightuserdata(lua, *it);
		lua_rawseti(lua, new_table, index++);
	}

	return 1;
}


//
// LUA_DestroyResource(resource (object))
//
static int LUA_DestroyResource(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "resource.destroy() not enough parameters, expected object");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the main game object
	CzApp* game = (CzApp*)CZ_GLOBAL_RESOURCES->getResourceManager()->getParent();

	// Get the object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "resource.destroy() Invalid object, expected an object in Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	object->getParent()->removeResource(object);

	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_DestroyTagged(tag-name)
//
static int LUA_DestroyResourceTagged(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "resource.destroyTagged() not enough parameters, expected tag-name (string), container scene (object, optional)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the tag
	const char* tag = NULL;
	if (lua_isstring(lua, 1))
		tag = lua_tostring(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "resource.destroyTagged() Invalid parameter, expected tag-name for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}
	unsigned int tag_hash = CZ_HASH(tag);

	// Get the container scene
	IzXomlResource* scene = NULL;
	if (count > 1)
	{
		if (lua_isuserdata(lua, 2))
			scene = (IzXomlResource*)lua_touserdata(lua, 2);
		if (scene != NULL && scene->getClassTypeHash() != CzHashes::Scene_Hash)
		{
			CzScriptEngineLua::DisplayError(lua, "resource.find() expected scene for Param2");
			lua_pushboolean(lua, false);
			return 1;
		}
	}
	else
		scene = (IzXomlResource*)lua->user_data;

	if (scene != NULL)
		((CzScene*)scene)->getResourceManager()->removeTaggedResources(tag_hash);
	else
		CZ_GLOBAL_RESOURCES->getResourceManager()->removeTaggedResources(tag_hash);

	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_CreateResource(type-name(string), parameters(table) or xoml-string (string), parent(object, optional))
//
static int LUA_CreateResource(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "resource.create() not enough parameters, expected type-name (string), parameters (table) or xoml-string (string), parent (object, optional)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the resource type name
	const char* type = NULL;
	if (lua_isstring(lua, 1))
		type = lua_tostring(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "resource.create() Invalid parameter, expected type-name (string) for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Get the resource parent
	IzXomlResource* parent = CzGlobals::App;
	if (count > 2)
	{
		if (lua_isuserdata(lua, 3))
			parent = (IzXomlResource*)lua_touserdata(lua, 3);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "resource.create() Invalid parameter, expected parent (object) for Param2");
			lua_pushnil(lua);
			return 1;
		}
	}

	if (lua_istable(lua, 2))
	{
		// Get the parameters and create a node to hold them
		CzXmlNode* node_root = new CzXmlNode();
		node_root->Managed = false;
		node_root->SetName(type);

		// Table is in the stack at index 't'
		lua_pushnil(lua);  // First key
		while (lua_next(lua, 2) != 0)
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
				node_root->AddAttribute(index_attrib);
			}

			// Removes value but keeps key for the next iteration
			lua_pop(lua, 1);
		}

		// Instantiate object
		IzXomlResource* object = CZ_XOML->ProcessNode(parent, node_root);
		if (object == NULL)
			lua_pushnil(lua);
		else
			lua_pushlightuserdata(lua, object);
		delete node_root;
	}
	else
	{
		CzScriptEngineLua::DisplayError(lua, "resource.create() Invalid parameter, expected parameters table for Param1");
		lua_pushnil(lua);
		return 1;
	}

	return 1;
}

//
// LUA_CreateResourceFromString(xoml-string (string), parent(object, optional))
//
static int LUA_CreateResourceFromString(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "resource.createFromString() not enough parameters, xoml-string (string), parent (object, optional)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the resource type name
	IzXomlResource* parent = CzGlobals::App;
	if (count > 1)
	{
		if (lua_isuserdata(lua, 2))
			parent = (IzXomlResource*)lua_touserdata(lua, 2);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "resource.createFromString() Invalid parameter, expected parent (object) for Param1");
			lua_pushboolean(lua, false);
			return 1;
		}
	}

	if (lua_isstring(lua, 1))
	{
		// Get the xoml string
		const char* xoml = NULL;
		xoml = lua_tostring(lua, 1);
		CzDataInput* data = new CzDataInput();
		data->setManaged(true);
		data->setData((char*)xoml, strlen(xoml));
		CzXmlParser* parser = new CzXmlParser();
		if (parser->Parse(data) == XmlErrorNone)
		{
			if (!CZ_XOML->Process(parent, parser->getRoot()))
				lua_pushboolean(lua, false);
			else
				lua_pushboolean(lua, true);
		}
		else
			lua_pushboolean(lua, false);
		delete parser;
	}
	else
	{
		CzScriptEngineLua::DisplayError(lua, "resource.createFromString() Invalid parameter, expected xoml string for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	return 1;
}
static const luaL_Reg g_resourcelib[] =
{
	{"find",			LUA_FindResource}, 
	{"findOfType",		LUA_FindResourceOfType}, 
	{"destroy",			LUA_DestroyResource}, 
	{"destroyTagged",	LUA_DestroyResourceTagged}, 
	{"create",			LUA_CreateResource}, 
	{"createFromString", LUA_CreateResourceFromString}, 
	{NULL, NULL}
};

LUALIB_API int luaopen_resource(lua_State *L)
{
	luaL_register(L, "resource", g_resourcelib);
	lua_pop(L, 1);

	return 1;
}

