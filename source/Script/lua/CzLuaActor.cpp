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
#include "CzActorImage.h"
#include "CzActorText.h"
#include "CzActorParticles.h"
#include "CzScene.h"
#include "CzProgram.h"
#include "CzUIBase.h"
#include "CzBrush.h"
#include "CzApp.h"
#include "CzTemplates.h"
#include "CzHashes.h"
#include "CzUIIcon.h"
#include "CzUILabel.h"
#include "CzXomlResourceManager.h"

//
// LUA_SetVertexActor actor (object), index (number), (x (number), y (number)) or array of vecs
//
static int LUA_SetVertexActor(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 3)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.setVertex() not enough parameters, expected actor (object), index (number), (x (number), y (number)) or array of vec)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.setVertex() Invalid target object for Param0, expected actor");
		lua_pushboolean(lua, false);
		return 1;
	}

	int index = 0;
	if (lua_isnumber(lua, 2))
		index = (int)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "actor.setVertex() index must be a number (Param1) - object - ", object->getName().c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	CzSprite* visual = ((CzActor*)object)->getVisual();
	if (visual == NULL)
	{
		lua_pushboolean(lua, false);
		return 1;
	}
	CzGeometry* prim = visual->getGemoetry();
	if (prim == NULL)
	{
		lua_pushboolean(lua, false);
		return 1;
	}

	if (lua_istable(lua, 3))
	{
		lua_pushnil(lua);  // First key
		while (lua_next(lua, 3) != 0)
		{
			const float* v = lua_tovec(lua, -1);
			(prim->Verts + index)->x = v[0];
			(prim->Verts + index)->y = v[1];
			index++;
			lua_pop(lua, 1);
		}

/*		int size = luaL_getn(lua, 3);
		for (int t = 1; t < size + 1; t++)
		{
			lua_rawgeti(lua, 3, t);
			const float* v = lua_tovec(lua, -1);
			(prim->Verts + index)->x = v[0];
			(prim->Verts + index)->y = v[1];
			index++;
		}*/

	}
	else
	{
		float x;
		if (lua_isnumber(lua, 3))
			x = (float)lua_tonumber(lua, 3);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.setVertex() x must be a number (Param2) - object - ", object->getName().c_str());
			lua_pushboolean(lua, false);
			return 1;
		}

		float y;
		if (lua_isnumber(lua, 4))
			y = (float)lua_tonumber(lua, 4);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.setVertex() y must be a number (Param3) - object - ", object->getName().c_str());
			lua_pushboolean(lua, false);
			return 1;
		}

		if (index < 0 || index > prim->VertCount)
		{
			lua_pushboolean(lua, false);
			return 1;
		}
		(prim->Verts + index)->x = x;
		(prim->Verts + index)->y = y;
	}
	visual->forceTransformDirty();

	lua_pushboolean(lua, true);
 
	return 1;
}

//
// LUA_SetVertexIndexActor actor (object), index (number), vert_index (number) or array of number
//
static int LUA_SetVertexIndexActor(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 3)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.setVertexIndex() not enough parameters, expected actor (object), index (number), (vert_index (number) or array of number)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.setVertexIndex() Invalid target object for Param0, expected actor");
		lua_pushboolean(lua, false);
		return 1;
	}

	int index = 0;
	if (lua_isnumber(lua, 2))
		index = (int)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "actor.setVertexIndex() index must be a number (Param1) - object - ", object->getName().c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	CzSprite* visual = ((CzActor*)object)->getVisual();
	if (visual == NULL)
	{
		lua_pushboolean(lua, false);
		return 1;
	}
	CzGeometry* prim = visual->getGemoetry();
	if (prim == NULL)
	{
		lua_pushboolean(lua, false);
		return 1;
	}

	if (lua_istable(lua, 3))
	{
		lua_pushnil(lua);  // First key
		while (lua_next(lua, 3) != 0)
		{
			int idx = (int)lua_tonumber(lua, -1);
			*(prim->Indices + index) = idx;
			index++;
			lua_pop(lua, 1);
		}
/*		int size = luaL_getn(lua, 3);
		for (int t = 1; t < size + 1; t++)
		{
			lua_rawgeti(lua, 3, t);
			int idx = (int)lua_tonumber(lua, -1);
			*(prim->Indices + index) = idx;
			index++;
		}*/
	}
	else
	{
		int idx;
		if (lua_isnumber(lua, 3))
			idx = (int)lua_tonumber(lua, 3);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.setVertexIndex() vert_index must be a number (Param2) - object - ", object->getName().c_str());
			lua_pushboolean(lua, false);
			return 1;
		}

		if (index < 0 || index > prim->VertCount)
		{
			lua_pushboolean(lua, false);
			return 1;
		}
		*(prim->Indices + index) = idx;
	}
	visual->forceTransformDirty();

	lua_pushboolean(lua, true);
 
	return 1;
}

//
// LUA_SetUVActor actor (object), index (number), (u (number), v (number)) or array of vecs
//
static int LUA_SetUVActor(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 3)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.setUV() not enough parameters, expected actor (object), index (number), (u (number), v (number)) or array of vecs");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.setUV() Invalid target object for Param0, expected actor");
		lua_pushboolean(lua, false);
		return 1;
	}

	int index = 0;
	if (lua_isnumber(lua, 2))
		index = (int)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "actor.setUV() index must be a number (Param1) - object - ", object->getName().c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActor*)object)->getVisual();
	if (visual == NULL)
	{
		lua_pushboolean(lua, false);
		return 1;
	}
	CzRenderPrim* prim = visual->getPrim();
	if (prim == NULL || prim->UVs == NULL)
	{
		lua_pushboolean(lua, false);
		return 1;
	}

	if (lua_istable(lua, 3))
	{
		lua_pushnil(lua);  // First key
		while (lua_next(lua, 3) != 0)
		{
			const float* v = lua_tovec(lua, -1);
			(prim->UVs + index)->x = v[0];
			(prim->UVs + index)->y = v[1];
			index++;
			lua_pop(lua, 1);
		}

/*		int size = luaL_getn(lua, 3);
		for (int t = 1; t < size + 1; t++)
		{
			lua_rawgeti(lua, 3, t);
			const float* v = lua_tovec(lua, -1);
			(prim->UVs + index)->x = v[0];
			(prim->UVs + index)->y = v[1];
			index++;
		}*/
	}
	else
	{
		float x;
		if (lua_isnumber(lua, 3))
			x = (float)lua_tonumber(lua, 3);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.setUV() u must be a number (Param2) - object - ", object->getName().c_str());
			lua_pushboolean(lua, false);
			return 1;
		}

		float y;
		if (lua_isnumber(lua, 4))
			y = (float)lua_tonumber(lua, 4);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.setUV() v must be a number (Param3) - object - ", object->getName().c_str());
			lua_pushboolean(lua, false);
			return 1;
		}

		if (index < 0 || index > prim->VertCount)
		{
			lua_pushboolean(lua, false);
			return 1;
		}
		(prim->UVs + index)->x = x;
		(prim->UVs + index)->y = y;
	}
	visual->setUVsDirty(true);

	lua_pushboolean(lua, true);
 
	return 1;
}

//
// LUA_SetRGBAActor actor (object), index (number), (r (number), g (number), b (number), a (number)) or array of vecs
//
static int LUA_SetRGBAActor(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 3)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.setRGBA() not enough parameters, expected actor (object), index (number), (r (number), g (number), b (number), a (number)) or array of vecs");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.setRGBA() Invalid target object for Param0, expected actor");
		lua_pushboolean(lua, false);
		return 1;
	}

	int index = 0;
	if (lua_isnumber(lua, 2))
		index = (int)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "actor.setRGBA() index must be a number (Param1) - object - ", object->getName().c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	CzBitmapSprite* visual = (CzBitmapSprite*)((CzActor*)object)->getVisual();
	if (visual == NULL)
	{
		lua_pushboolean(lua, false);
		return 1;
	}
	CzGeometry* geom = visual->getGemoetry();
	if (geom == NULL || geom->Colours == NULL)
	{
		lua_pushboolean(lua, false);
		return 1;
	}
	CzRenderPrim* prim = visual->getPrim();

	if (lua_istable(lua, 3))
	{
		lua_pushnil(lua);  // First key
		while (lua_next(lua, 3) != 0)
		{
			const float* v = lua_tovec(lua, -1);
			(geom->Colours + index)->r = (uint8)v[0];
			(geom->Colours + index)->g = (uint8)v[1];
			(geom->Colours + index)->b = (uint8)v[2];
			(prim->Colours + index)->a = (uint8)v[3];
			index++;
			lua_pop(lua, 1);
		}
/*		int size = luaL_getn(lua, 3);
		for (int t = 1; t < size + 1; t++)
		{
			lua_rawgeti(lua, 3, t);
			const float* v = lua_tovec(lua, -1);
			(geom->Colours + index)->r = (uint8)v[0];
			(geom->Colours + index)->g = (uint8)v[1];
			(geom->Colours + index)->b = (uint8)v[2];
			(prim->Colours + index)->a = (uint8)v[3];
			index++;
		}*/
	}
	else
	{
		float r;
		if (lua_isnumber(lua, 3))
			r = (float)lua_tonumber(lua, 3);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.setRGBA() r must be a number (Param2) - object - ", object->getName().c_str());
			lua_pushboolean(lua, false);
			return 1;
		}

		float g;
		if (lua_isnumber(lua, 4))
			g = (float)lua_tonumber(lua, 4);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.setRGBA() g must be a number (Param3) - object - ", object->getName().c_str());
			lua_pushboolean(lua, false);
			return 1;
		}

		float b;
		if (lua_isnumber(lua, 5))
			b = (float)lua_tonumber(lua, 5);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.setRGBA() b must be a number (Param4) - object - ", object->getName().c_str());
			lua_pushboolean(lua, false);
			return 1;
		}

		float a;
		if (lua_isnumber(lua, 6))
			a = (float)lua_tonumber(lua, 6);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.setRGBA() alpha must be a number (Param5) - object - ", object->getName().c_str());
			lua_pushboolean(lua, false);
			return 1;
		}

		if (index < 0 || index > prim->VertCount)
		{
			lua_pushboolean(lua, false);
			return 1;
		}
		(geom->Colours + index)->r = (uint8)r;
		(geom->Colours + index)->g = (uint8)g;
		(geom->Colours + index)->b = (uint8)b;
		(prim->Colours + index)->a = (uint8)a;
	}

	lua_pushboolean(lua, true);
 
	return 1;
}



//
// LUA_SetProperty actor (object), property (string), value (any), use-user-property (bool, optional)
//
static int LUA_SetPropertyActor(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 3)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.set() not enough parameters, expected actor (object), property (string), value (string, number, boolean or vec), use-user-property (bool, optional)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.set() Invalid target object for Param0, expected actor");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzString prop_name;
	if (lua_isstring(lua, 2))
		prop_name = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "actor.set() property must be a property name (Param1) - object - ", object->getName().c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	CzXomlProperty prop;
	if (!LUA_ValueToProperty(lua, 3, prop))
	{
		CzScriptEngineLua::DisplayError(lua, "actor.set() value must be a string, number, boolean or vec (Param2) - object - ", object->getName().c_str());
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "- property - ", prop_name.c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	bool user = false;
	if (count > 3)
	{
		if (lua_isboolean(lua, 4))
			user = lua_toboolean(lua, 4);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.set()  use-user-property must be a boolean (Param3) - object - ", object->getName().c_str());
			lua_pushboolean(lua, false);
			return 1;
		}
	}

	if (user)
	{
		CzUserPropertyList* list = ((CzActor*)object)->getUserPropertyList();
		if (list != NULL)
		{
			CzXomlNamedProperty* prop2 = list->findProperty(prop_name.getHash());
			if (prop2 != NULL)
				lua_pushboolean(lua, prop2->Property->setValue(prop, false));
			else
			{
				CzScriptEngineLua::DisplayError(lua, "actor.set() actor's user property list does not contain specified property - object - ", object->getName().c_str());
				CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "- property - ", prop_name.c_str());
				lua_pushboolean(lua, false);
				return 1;
			}
		}
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.set() actor does not contain a user property list - object - ", object->getName().c_str());
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "- property - ", prop_name.c_str());
			lua_pushboolean(lua, false);
			return 1;
		}
	}
	else
		lua_pushboolean(lua, ((CzActor*)object)->setProperty(prop_name.getHash(), prop, false));
 
	return 1;
}

//
// LUA_AddProperty actor (object), property (string), value (any), use-user-property (bool, optional)
//
static int LUA_AddPropertyActor(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 3)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.add() not enough parameters, expected actor (object), property (string), value (string, number, boolean or vec), use-user-property (bool, optional)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.add() Invalid target object for Param0, expected actor");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzString prop_name, value;
	if (lua_isstring(lua, 2))
		prop_name = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "actor.add() property must be a property name (Param1) - object - ", object->getName().c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	CzXomlProperty prop;
	if (!LUA_ValueToProperty(lua, 3, prop))
	{
		CzScriptEngineLua::DisplayError(lua, "actor.add() value must be a string, number, boolean or vec (Param2) - object - ", object->getName().c_str());
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "- property - ", prop_name.c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	bool user = false;
	if (count > 3)
	{
		if (lua_isboolean(lua, 4))
			user = lua_toboolean(lua, 4);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.set()  use-user-property must be a boolean (Param3) - object - ", object->getName().c_str());
			lua_pushboolean(lua, false);
			return 1;
		}
	}

	if (user)
	{
		CzUserPropertyList* list = ((CzActor*)object)->getUserPropertyList();
		if (list != NULL)
		{
			CzXomlNamedProperty* prop2 = list->findProperty(prop_name.getHash());
			if (prop2 != NULL)
				lua_pushboolean(lua, prop2->Property->setValue(prop, true));
			else
			{
				CzScriptEngineLua::DisplayError(lua, "actor.add() actor's user property list does not contain specified property - object - ", object->getName().c_str());
				CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "- property - ", prop_name.c_str());
				lua_pushboolean(lua, false);
				return 1;
			}
		}
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.add() actor does not contain a user property list - object - ", object->getName().c_str());
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "- property - ", prop_name.c_str());
			lua_pushboolean(lua, false);
			return 1;
		}
	}
	else
		lua_pushboolean(lua, ((CzActor*)object)->setProperty(prop_name.getHash(), prop, true));
	
	return 1;
}

//
// LUA_GetProperty actor (object), property (string), use-user-property (bool, optional)
//
static int LUA_GetPropertyActor(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.get() not enough parameters, expected actor (object), property (string), use-user-property (bool, optional)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the target object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.get() Invalid target object for Param0, expected actor");
		lua_pushnil(lua);
		return 1;
	}

	CzString prop_name;
	if (lua_isstring(lua, 2))
		prop_name = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "actor.get() property must be a property name (Param1) - object - ", object->getName().c_str());
		lua_pushnil(lua);
		return 1;
	}

	bool user = false;
	if (count > 2)
	{
		if (lua_isboolean(lua, 3))
			user = lua_toboolean(lua, 3);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.get()  use-user-property must be a boolean (Param2) - object - ", object->getName().c_str());
			lua_pushnil(lua);
			return 1;
		}
	}

	if (user)
	{
		CzUserPropertyList* list = ((CzActor*)object)->getUserPropertyList();
		if (list != NULL)
		{
			CzXomlNamedProperty* prop2 = list->findProperty(prop_name.getHash());
			if (prop2 != NULL)
				LUA_ReturnProperty(lua, *prop2->Property);
			else
			{
				CzScriptEngineLua::DisplayError(lua, "actor.get() actor's user property list does not contain specified property - object - ", object->getName().c_str());
				lua_pushnil(lua);
				return 1;
			}
		}
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.get() actor does not contain a user property list - object - ", object->getName().c_str());
			lua_pushnil(lua);
			return 1;
		}
	}
	else
	{
		CzXomlProperty prop;
		if (!((CzActor*)object)->getProperty(prop_name.getHash(), prop))
		{
			CzScriptEngineLua::DisplayError(lua, "actor.get() actor property does not exist - ", prop_name.c_str());
			lua_pushnil(lua);
			return 1;
		}

		LUA_ReturnProperty(lua, prop);
	}

	return 1;
}


//
// LUA_FindActor actor-name (string), scene (object, optional)
//
static int LUA_FindActor(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.find() not enough parameters, expected actor-name (string), scene (object, optional)");
		lua_pushnil(lua);
		return 1;
	}

	IzXomlResource* container = NULL;
	const char* name = NULL;

	// Get the actor name
	if (lua_isstring(lua, 1))
		name = lua_tostring(lua, 1);
	if (name == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.find() Invalid name for Param0");
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
			CzScriptEngineLua::DisplayError(lua, "actor.find() expected scene for Param1");
			lua_pushnil(lua);
			return 1;
		}
	}
	else
		container = (CzScene*)lua->user_data;

	if (container == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.find() invalid container scene");
		lua_pushnil(lua);
		return 1;
	}

	// Locate actor
	CzActor* actor = ((CzScene*)container)->findActor(name);
	if (actor == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.find() actor not found - ", name);
		lua_pushnil(lua);
		return 1;
	}

	// Return the actor
	lua_pushlightuserdata(lua, actor);

	return 1;
}

//
// LUA_FindActorsOfType actor-type (number), scene (object, optional)
//
static int LUA_FindActorsOfType(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.findOfType() not enough parameters, expected actor-type (number), scene (object, optional)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the typw
	int type = 0;
	if (lua_isnumber(lua, 1))
		type = (int)lua_tonumber(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "actor.findOfType() expected number for Param0");
		lua_pushnil(lua);
		return 1;
	}

	CzScene* scene = NULL;
	// Get the container scene
	if (count > 1)
	{
		if (lua_isuserdata(lua, 2))
			scene = (CzScene*)lua_touserdata(lua, 2);
		if (scene != NULL && scene->getClassTypeHash() != CzHashes::Scene_Hash)
		{
			CzScriptEngineLua::DisplayError(lua, "actor.findOfType() expected scene for Param1");
			lua_pushnil(lua);
			return 1;
		}
	}
	else
		scene = (CzScene*)lua->user_data;

	if (scene == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.findOfType() invalid container scene");
		lua_pushnil(lua);
		return 1;
	}

	CzVector<CzActor*> found_actors;
	count = scene->findActorsOfType(type, found_actors);
	if (count == 0)
	{
		lua_pushnil(lua);
		return 1;
	}

	lua_createtable(lua, count, 0);
	int new_table = lua_gettop(lua);
	int index = 1;

	// Add actors to table
	for (CzVector<CzActor*>::iterator it = found_actors.begin(); it != found_actors.end(); ++it)
	{
		lua_pushlightuserdata(lua, *it);
		lua_rawseti(lua, new_table, index++);
	}

	return 1;
}

//
// LUA_FindActorsTagged tag (string), scene (object, optional)
//
static int LUA_FindActorsTagged(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.findTagged() not enough parameters, expected tag (string), scene (object, optional)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the tag
	const char* tag = NULL;
	if (lua_isstring(lua, 1))
		tag = lua_tostring(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "actor.findTagged() expected string for Param0");
		lua_pushnil(lua);
		return 1;
	}

	CzScene* scene = NULL;
	// Get the container scene
	if (count > 1)
	{
		if (lua_isuserdata(lua, 2))
			scene = (CzScene*)lua_touserdata(lua, 2);
		if (scene != NULL && scene->getClassTypeHash() != CzHashes::Scene_Hash)
		{
			CzScriptEngineLua::DisplayError(lua, "actor.findTagged() expected scene for Param1");
			lua_pushnil(lua);
			return 1;
		}
	}
	else
		scene = (CzScene*)lua->user_data;

	if (scene == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.findTagged() invalid container scene");
		lua_pushnil(lua);
		return 1;
	}

	CzVector<CzActor*> found_actors;
	count = scene->findActors(CzString::CalculateHash(tag), found_actors);
	if (count == 0)
	{
		lua_pushnil(lua);
		return 1;
	}

	lua_createtable(lua, count, 0);
	int new_table = lua_gettop(lua);
	int index = 1;

	// Add actors to table
	for (CzVector<CzActor*>::iterator it = found_actors.begin(); it != found_actors.end(); ++it)
	{
		lua_pushlightuserdata(lua, *it);
		lua_rawseti(lua, new_table, index++);
	}

	return 1;
}
//
// LUA_FindAllActors scene (object, optional)
//
static int LUA_FindAllActors(lua_State *lua)
{
	int count = lua_gettop(lua);

	CzScene* scene = NULL;
	// Get the container scene
	if (count > 0)
	{
		if (lua_isuserdata(lua, 1))
			scene = (CzScene*)lua_touserdata(lua, 1);
		if (scene != NULL && scene->getClassTypeHash() != CzHashes::Scene_Hash)
		{
			CzScriptEngineLua::DisplayError(lua, "actor.findAll() expected scene for Param1");
			lua_pushnil(lua);
			return 1;
		}
	}
	else
		scene = (CzScene*)lua->user_data;

	if (scene == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.findOfType() invalid container scene");
		lua_pushnil(lua);
		return 1;
	}

	CzList<CzActor*>& found_actors = scene->getActors();
	count = found_actors.size();
	if (count == 0)
	{
		lua_pushnil(lua);
		return 1;
	}

	lua_createtable(lua, count, 0);
	int new_table = lua_gettop(lua);
	int index = 1;

	// Add actors to table
	for (CzScene::_Iterator it = found_actors.begin(); it != found_actors.end(); ++it)
	{
		lua_pushlightuserdata(lua, *it);
		lua_rawseti(lua, new_table, index++);
	}

	return 1;
}

//
// LUA_CreateActor(container (object), parameters)
//
static int LUA_CreateActor(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.create() not enough parameters, expected type (string) parent (scene or actor), ......");
		lua_pushnil(lua);
		return 1;
	}

	// Get the type of actor to create
	const char* type = NULL;
	if (lua_isstring(lua, 1))
		type = lua_tostring(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "actor.create() Invalid actor type for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Get the parent container
	IzXomlResource* container = NULL;
	if (lua_isuserdata(lua, 2))
		container = (CzScene*)lua_touserdata(lua, 2);
	if (container == NULL || (container->getClassTypeHash() != CzHashes::Actor_Hash && container->getClassTypeHash() != CzHashes::Scene_Hash))
	{
		CzScriptEngineLua::DisplayError(lua, "actor.create() Invalid container for Param1");
		lua_pushnil(lua);
		return 1;
	}

	// Get actor / scene
	CzActor* actor = NULL;
	CzScene* scene = NULL;
	if (container->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)container;
	if (container->getClassTypeHash() == CzHashes::Actor_Hash)
	{
		actor = (CzActor*)container;
		scene = actor->getScene();
	}

	unsigned int type_hash = CZ_HASH(type);
	if (type_hash == CzHashes::Icon_Hash)
	{
		CzUIIcon* new_actor = new CzUIIcon();
		scene->addActor(new_actor);

		// Get the brush
		IzBrush *brush = NULL;
		if (lua_isuserdata(lua, 3))
			brush = (IzBrush*)lua_touserdata(lua, 3);
		if (brush == NULL || brush->getClassTypeHash() != CzHashes::Brush_Hash)
		{
			CzScriptEngineLua::DisplayError(lua, "actor.create() Invalid brush for Param2");
			lua_pushnil(lua);
			return 1;
		}

		// Get the width and height
		int width = -100, height = -100;
		if (lua_isnumber(lua, 4))
			width = (int)lua_tonumber(lua, 4);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.create() Width missing for Param3");
			lua_pushnil(lua);
			return 1;
		}
		if (lua_isnumber(lua, 5))
			height = (int)lua_tonumber(lua, 5);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.create() Height missing for Param4");
			lua_pushnil(lua);
			return 1;
		}
		new_actor->Init(brush, width, height);
		if (actor != NULL)
		{
			new_actor->setLinkedTo(actor);
			new_actor->setDepth(0);
		}

		lua_pushlightuserdata(lua, new_actor);
	}
	else
	if (type_hash == CzHashes::ActorText_Hash)
	{
		CzActorText* new_actor = new CzActorText();
		scene->addActor(new_actor);

		// Get the font
		CzFont *font = NULL;
		if (lua_isuserdata(lua, 3))
			font = (CzFont*)lua_touserdata(lua, 3);
		if (font == NULL || font->getClassTypeHash() != CzHashes::Font_Hash)
		{
			CzScriptEngineLua::DisplayError(lua, "actor.create() Invalid font for Param2");
			lua_pushnil(lua);
			return 1;
		}

		// Get the text
		const char* text = NULL;
		if (lua_isstring(lua, 4))
			text = lua_tostring(lua, 4);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.create() Text missing for Param3");
			lua_pushnil(lua);
			return 1;
		}

		CzIRect			rect = CzIRect(0, 0, -100, -100);
		CzString t = text;
		new_actor->Init(font, rect, t, 0);
		if (actor != NULL)
		{
			new_actor->setLinkedTo(actor);
			new_actor->setDepth(0);
		}

		lua_pushlightuserdata(lua, new_actor);
	}
	else
	if (type_hash == CzHashes::Label_Hash)
	{
		CzUILabel* new_actor = new CzUILabel();
		scene->addActor(new_actor);

		// Get the font
		CzFont *font = NULL;
		if (lua_isuserdata(lua, 3))
			font = (CzFont*)lua_touserdata(lua, 3);
		if (font == NULL || font->getClassTypeHash() != CzHashes::Font_Hash)
		{
			CzScriptEngineLua::DisplayError(lua, "actor.create() Invalid font for Param2");
			lua_pushnil(lua);
			return 1;
		}

		// Get the text
		const char* text = NULL;
		if (lua_isstring(lua, 4))
			text = lua_tostring(lua, 4);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.create() Text missing for Param3");
			lua_pushnil(lua);
			return 1;
		}

		// Get the brush
		IzBrush *brush = NULL;
		if (lua_isuserdata(lua, 5))
			brush = (IzBrush*)lua_touserdata(lua, 5);
		if (brush == NULL || brush->getClassTypeHash() != CzHashes::Brush_Hash)
		{
			CzScriptEngineLua::DisplayError(lua, "actor.create() Invalid brush for Param4");
			lua_pushnil(lua);
			return 1;
		}

		// Get the width and height
		int width = -100, height = -100;
		if (lua_isnumber(lua, 6))
			width = (int)lua_tonumber(lua, 6);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.create() Width missing for Param5");
			lua_pushnil(lua);
			return 1;
		}
		if (lua_isnumber(lua, 7))
			height = (int)lua_tonumber(lua, 7);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.create() Height missing for Param6");
			lua_pushnil(lua);
			return 1;
		}

		CzIRect		rect = CzIRect(0, 0, -100, -100);
		CzIRect		margin = CzIRect(0, 0, 0, 0);
		CzVec4		skew = CzVec4(0, 0, 0, 0);
		new_actor->Init(brush, width, height);
		if (actor != NULL)
		{
			new_actor->setLinkedTo(actor);
			new_actor->setDepth(0);
		}
		CzString t = text;
		new_actor->InitTextActor(font, rect, t, 0, AlignH_Centre, AlignV_Middle, skew, margin, false);

		lua_pushlightuserdata(lua, new_actor);
	}
	else
	{
		CzScriptEngineLua::DisplayError(lua, "actor.create() Cannot create actor of type - ", type);
		lua_pushnil(lua);
	}

	return 1;
}

//
// LUA_DestroyActor(container (object)
//
static int LUA_DestroyActor(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.destroy() not enough parameters, expected actor (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the main game object
	CzApp* game = (CzApp*)CZ_GLOBAL_RESOURCES->getResourceManager()->getParent();

	// Get the object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.destroy() Invalid object, expected an actor for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	((CzActor*)object)->getScene()->removeActor((CzActor*)object);

	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_DestroyTaggedActors(container (object)
//
static int LUA_DestroyTaggedActors(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.destroyTagged() not enough parameters, expected actor tag (string)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the main game object
	CzApp* game = (CzApp*)CZ_GLOBAL_RESOURCES->getResourceManager()->getParent();

	// Get the tag
	const char* tag = NULL;
	if (lua_isstring(lua, 1))
		tag = lua_tostring(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "actor.destroyTagged() Invalid parameter, expected tag-name for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the container scene
	IzXomlResource* container = NULL;
	if (count > 1)
	{
		if (lua_isuserdata(lua, 2))
			container = (IzXomlResource*)lua_touserdata(lua, 2);
		if (container == NULL || (container != NULL && container->getClassTypeHash() != CzHashes::Scene_Hash))
		{
			CzScriptEngineLua::DisplayError(lua, "actor.destroyTagged() expected scene for Param1");
			lua_pushnil(lua);
			return 1;
		}
	}
	else
		container = (CzScene*)lua->user_data;

	((CzScene*)container)->removeActors(CZ_HASH(tag));

	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_isActorClipped actor (object)
//
static int LUA_isActorClipped(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.clipped() not enough parameters, expected actor (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the target object
	CzActor* actor = NULL;
	if (lua_isuserdata(lua, 1))
		actor = (CzActor*)lua_touserdata(lua, 1);
	if (actor == NULL || actor->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.clipped() Invalid actor object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	lua_pushboolean(lua, actor->getVisual()->isClippedByManager());
 
	
	return 1;
}

//
// LUA_ActorsOverlaps actor1 (object), actor2 (object)
//
static int LUA_ActorsOverlaps(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 3)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.overlaps() not enough parameters, expected actor1 (object), actor2 (object), quick (boolean)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the actor1 object
	CzActor* actor1 = NULL;
	if (lua_isuserdata(lua, 1))
		actor1 = (CzActor*)lua_touserdata(lua, 1);
	if (actor1 == NULL || actor1->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.overlaps() Invalid actor1 object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Get the actor2 object
	CzActor* actor2 = NULL;
	if (lua_isuserdata(lua, 2))
		actor2 = (CzActor*)lua_touserdata(lua, 2);
	if (actor2 == NULL || actor2->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.overlaps() Invalid actor2 object for Param1");
		lua_pushnil(lua);
		return 1;
	}

	// Get quick test
	bool quick = true;
	if (lua_isboolean(lua, 3))
		quick = lua_toboolean(lua, 3) != 0;

	if (quick)
		lua_pushboolean(lua, actor1->TestOverlap(actor2));
	else
		lua_pushboolean(lua, actor1->TestOverlapRect(actor2));
 
	
	return 1;
}

//
// LUA_getDistanceBetweenActors actor1 (object), actor2 (object)
//
static int LUA_getDistanceBetweenActors(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.distance() not enough parameters, expected actor1 (object), actor2 (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the actor1 object
	CzActor* actor1 = NULL;
	if (lua_isuserdata(lua, 1))
		actor1 = (CzActor*)lua_touserdata(lua, 1);
	if (actor1 == NULL || actor1->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.distance() Invalid actor1 object");
		lua_pushnil(lua);
		return 1;
	}

	// Get the actor2 object
	CzActor* actor2 = NULL;
	if (lua_isuserdata(lua, 2))
		actor2 = (CzActor*)lua_touserdata(lua, 2);
	if (actor2 == NULL || actor2->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.distance() Invalid actor2 object");
		lua_pushnil(lua);
		return 1;
	}

	lua_pushnumber(lua, actor1->getDistanceBetween(actor2));
 
	
	return 1;
}

//
// LUA_getAngleBetweenActors actor1 (object), actor2 (object)
//
static int LUA_getAngleBetweenActors(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.anglediff() not enough parameters, expected actor1 (object), actor2 (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the actor1 object
	CzActor* actor1 = NULL;
	if (lua_isuserdata(lua, 1))
		actor1 = (CzActor*)lua_touserdata(lua, 1);
	if (actor1 == NULL || actor1->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.anglediff() Invalid actor1 object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Get the actor2 object
	CzActor* actor2 = NULL;
	if (lua_isuserdata(lua, 2))
		actor2 = (CzActor*)lua_touserdata(lua, 2);
	if (actor2 == NULL || actor2->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.anglediff() Invalid actor2 object for Param1");
		lua_pushnil(lua);
		return 1;
	}

	lua_pushnumber(lua, actor1->getAngleBetween(actor2));
 
	
	return 1;
}

//
// LUA_getStartContacts actor (object) collision-mask (number)
//
static int LUA_getStartContacts(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.getStartContacts() not enough parameters, expected actor (object), collision-mask (number, optional)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the actor object
	CzActor* actor = NULL;
	if (lua_isuserdata(lua, 1))
		actor = (CzActor*)lua_touserdata(lua, 1);
	if (actor == NULL || actor->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.getStartContacts() Invalid actor object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	unsigned int collision_mask = 0;
	if (count > 1)
	{
		if (lua_isnumber(lua, 2))
			collision_mask = (unsigned int)lua_tonumber(lua, 2);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.getStartContacts() Invalid collision-mask for Param1, expected number");
			lua_pushnil(lua);
			return 1;
		}
	}

	CzBox2dBody* body = actor->getBox2dBody();
	if (body == NULL)
	{
		lua_pushnil(lua);
		return 1;
	}

	CzSlotArray<CzBox2dCollidable*>& started = body->getCollisionsStarted();

	int col_count = 0;
	for (int t = 0; t < started.getSize(); t++)
	{
		CzBox2dCollidable* collision = started.element_at(t);
		if (collision != NULL)
		{
			CzActor* actor2 = (CzActor*)collision->getUserData();
			if (actor2 != NULL && (actor2->getType() & collision_mask) == collision_mask)
				col_count++;
		}
	}
	if (col_count == 0)
	{
		lua_pushnil(lua);
		return 1;
	}
	
	lua_createtable(lua, col_count, 0);
	int new_table = lua_gettop(lua);
	int index = 1;

	// Check begin contacts
	for (int t = 0; t < started.getSize(); t++)
	{
		CzBox2dCollidable* collision = started.element_at(t);
		if (collision != NULL)
		{
			CzActor* actor2 = (CzActor*)collision->getUserData();
			if (actor2 != NULL && (actor2->getType() & collision_mask) == collision_mask)
			{
				lua_pushlightuserdata(lua, actor2);
				lua_rawseti(lua, new_table, index++);
			}
		}
	}

	
	return 1;
}

//
// LUA_getEndContacts actor (object) collision-mask (number)
//
static int LUA_getEndContacts(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.getEndContacts() not enough parameters, expected actor (object), collision-mask (number, optional)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the actor object
	CzActor* actor = NULL;
	if (lua_isuserdata(lua, 1))
		actor = (CzActor*)lua_touserdata(lua, 1);
	if (actor == NULL || actor->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.getEndContacts() Invalid actor object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	unsigned int collision_mask = 0;
	if (count > 1)
	{
		if (lua_isnumber(lua, 2))
			collision_mask = (unsigned int)lua_tonumber(lua, 2);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "actor.getEndContacts() Invalid collision-mask for Param1, expected number");
			lua_pushnil(lua);
			return 1;
		}
	}

	CzBox2dBody* body = actor->getBox2dBody();
	if (body == NULL)
	{
		lua_pushnil(lua);
		return 1;
	}

	CzSlotArray<CzBox2dCollidable*>& ended = body->getCollisionsEnded();

	int col_count = 0;
	for (int t = 0; t < ended.getSize(); t++)
	{
		CzBox2dCollidable* collision = ended.element_at(t);
		if (collision != NULL)
		{
			CzActor* actor2 = (CzActor*)collision->getUserData();
			if (actor2 != NULL && (actor2->getType() & collision_mask) == collision_mask)
				col_count++;
		}
	}
	if (col_count == 0)
	{
		lua_pushnil(lua);
		return 1;
	}
	
	lua_createtable(lua, col_count, 0);
	int new_table = lua_gettop(lua);
	int index = 1;

	// Check end contacts
	for (int t = 0; t < ended.getSize(); t++)
	{
		CzBox2dCollidable* collision = ended.element_at(t);
		if (collision != NULL)
		{
			CzActor* actor2 = (CzActor*)collision->getUserData();
			if (actor2 != NULL && (actor2->getType() & collision_mask) == collision_mask)
			{
				lua_pushlightuserdata(lua, actor2);
				lua_rawseti(lua, new_table, index++);
			}
		}
	}

	return 1;
}

//
// LUA_getChildrenActor actor (object)
//
static int LUA_getChildrenActor(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.children() not enough parameters, expected actor (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the actor object
	CzActor* actor = NULL;
	if (lua_isuserdata(lua, 1))
		actor = (CzActor*)lua_touserdata(lua, 1);
	if (actor == NULL || actor->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.children() Invalid actor object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Count number of children
	int child_count = 0;
	CzScene* scene = actor->getScene();
	for (CzScene::_Iterator it = scene->begin(); it != scene->end(); ++it)
		if ((*it)->getLinkedTo() == actor)
			child_count++;
	if (child_count == 0)
	{
		lua_pushnil(lua);
		return 1;
	}

	lua_createtable(lua, child_count, 0);
	int new_table = lua_gettop(lua);
	int index = 1;

	// Add any actors that link to this actor
	for (CzScene::_Iterator it = scene->begin(); it != scene->end(); ++it)
	{
		if ((*it)->getLinkedTo() == actor)
		{
			lua_pushlightuserdata(lua, *it);
			lua_rawseti(lua, new_table, index++);
		}
	}

	return 1;
}

//
// LUA_changeTimelineActor actor (object), command (play, stop. pause, restart) - Returns true if success
//
static int LUA_changeTimelineActor(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.changeTimeline() not enough parameters, expected actor (object), command (play, stop. pause, restart)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the actor object
	CzActor* actor = NULL;
	if (lua_isuserdata(lua, 1))
		actor = (CzActor*)lua_touserdata(lua, 1);
	if (actor == NULL || actor->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.changeTimeline() Invalid actor object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	const char* command = NULL;
	if (lua_isstring(lua, 2))
		actor = (CzActor*)lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "actor.changeTimeline() Invalid command for Param1, expected string");
		lua_pushboolean(lua, false);
		return 1;
	}

	if (actor->getTimeline() == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.changeTimeline() actor has no timeline assigned");
		lua_pushboolean(lua, false);
		return 1;
	}

	unsigned int hash = CZ_HASH(command);
	if (hash == CzHashes::Play_Hash)
		actor->getTimeline()->play();
	else
	if (hash == CzHashes::Stop_Hash)
		actor->getTimeline()->stop();
	else
	if (hash == CzHashes::Pause_Hash)
		actor->getTimeline()->pause();
	else
	if (hash == CzHashes::Restart_Hash)
		actor->getTimeline()->restart();
	else
	{
		CzScriptEngineLua::DisplayError(lua, "actor.changeTimeline() invalid command - ", command);
		lua_pushnil(lua);
		return 1;
	}

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_CheckActorState actor (object)
//
static int LUA_CheckActorState(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.checkState() not enough parameters, expected actor (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the actor to check
	CzActor* actor = NULL;
	if (lua_isuserdata(lua, 1))
		actor = (CzActor*)lua_touserdata(lua, 1);
	if (actor == NULL || actor->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.checkState() Invalid actor object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	lua_pushinteger(lua, actor->getScene()->checkActorState(actor));
	
	return 1;
}

//
// LUA_BringActorToFront actor (object)
//
static int LUA_BringActorToFront(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.bringToFront() not enough parameters, expected actor (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the actor to check
	CzActor* actor = NULL;
	if (lua_isuserdata(lua, 1))
		actor = (CzActor*)lua_touserdata(lua, 1);
	if (actor == NULL || actor->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.bringToFront() Invalid actor object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	actor->BringActorToFront();
 
	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_FindClosestActor actor (object), type (number)
//
static int LUA_FindClosestActor(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.closest() not enough parameters, expected actor (object), type (number)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the actor to check
	CzActor* actor = NULL;
	if (lua_isuserdata(lua, 1))
		actor = (CzActor*)lua_touserdata(lua, 1);
	if (actor == NULL || actor->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.closest() Invalid actor object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Get the actor type
	int type = 0;
	if (lua_isnumber(lua, 2))
		type = (int)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "actor.closest() Invalid type for Param1, expected number");
		lua_pushnil(lua);
		return 1;
	}

	CzVec2 pos = actor->getPosition();
	CzActor* closest = actor->getScene()->findClosestActor((int)pos.x, (int)pos.y, type);
	if (closest == NULL)
		lua_pushnil(lua);
	else
		lua_pushlightuserdata(lua, closest);

	return 1;
}

//
// LUA_FindFurthestActor actor (object), type (number)
//
static int LUA_FindFurthestActor(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.furthest() not enough parameters, expected actor (object), type (number)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the actor to check
	CzActor* actor = NULL;
	if (lua_isuserdata(lua, 1))
		actor = (CzActor*)lua_touserdata(lua, 1);
	if (actor == NULL || actor->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "actor.furthest() Invalid actor object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Get the actor type
	int type = 0;
	if (lua_isnumber(lua, 2))
		type = (int)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "actor.furthest() Invalid type for Param1, expected number");
		lua_pushnil(lua);
		return 1;
	}

	CzVec2 pos = actor->getPosition();
	CzActor* furthest = actor->getScene()->findFurthestActor((int)pos.x, (int)pos.y, type);
	if (furthest == NULL)
		lua_pushnil(lua);
	else
		lua_pushlightuserdata(lua, furthest);

	return 1;
}


//
// Lua library
//
static const luaL_Reg g_actorlib[] =
{
	{"setVertex",			LUA_SetVertexActor}, 
	{"setVertexIndex",		LUA_SetVertexIndexActor}, 
	{"setUV",				LUA_SetUVActor}, 
	{"setRGBA",				LUA_SetRGBAActor}, 
	{"set",					LUA_SetPropertyActor}, 
	{"add",					LUA_AddPropertyActor}, 
	{"get",					LUA_GetPropertyActor}, 
	{"find",				LUA_FindActor}, 
	{"findOfType",			LUA_FindActorsOfType}, 
	{"findTagged",			LUA_FindActorsTagged}, 
	{"findAll",				LUA_FindAllActors}, 
	{"create",				LUA_CreateActor}, 
	{"destroy",				LUA_DestroyActor}, 
	{"destroyTagged",		LUA_DestroyTaggedActors}, 
	{"clipped",				LUA_isActorClipped}, 
	{"overlaps",			LUA_ActorsOverlaps}, 
	{"distance",			LUA_getDistanceBetweenActors}, 
	{"anglediff",			LUA_getAngleBetweenActors}, 
	{"getStartContacts",	LUA_getStartContacts}, 
	{"getEndContacts",		LUA_getEndContacts}, 
	{"children",			LUA_getChildrenActor}, 
	{"changeTimeline",		LUA_changeTimelineActor}, 
	{"checkState",			LUA_CheckActorState}, 
	{"bringToFront",		LUA_BringActorToFront}, 
	{"closest",				LUA_FindClosestActor}, 
	{"furthest",			LUA_FindFurthestActor}, 
	{NULL, NULL}
};

LUALIB_API int luaopen_actor(lua_State *L)
{
	luaL_register(L, "actor", g_actorlib);
	lua_pop(L, 1);

	return 1;
}
