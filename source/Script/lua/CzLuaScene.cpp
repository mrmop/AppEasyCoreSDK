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
// LUA_SetPropertyScene scene (object), property (string), value (string)
//
static int LUA_SetPropertyScene(lua_State *lua)
{
	if (lua_gettop(lua) < 3)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.set() not enough parameters, expected scene (object), property (string), value (string, number, boolean or vec)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::Scene_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.set() Invalid target object, expected scene");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzString prop_name;
	if (lua_isstring(lua, 2))
		prop_name = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "scene.set() property must be a property name - object - ", object->getName().c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	CzXomlProperty prop;
	if (!LUA_ValueToProperty(lua, 3, prop))
	{
		CzScriptEngineLua::DisplayError(lua, "scene.set() value must be a string, number, boolean or vec - object - ", object->getName().c_str());
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "- property - ", prop_name.c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, ((CzScene*)object)->setProperty(prop_name.getHash(), prop, false));
 
	return 1;
}

//
// LUA_AddPropertyScene scene (object), property (string), value (string)
//
static int LUA_AddPropertyScene(lua_State *lua)
{
	if (lua_gettop(lua) < 3)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.add() not enough parameters, expected scene (object), property (string), value (string, number, boolean or vec)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::Scene_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.add() Invalid target object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzString prop_name, value;
	if (lua_isstring(lua, 2))
		prop_name = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "scene.add() property must be a property name (Param1) - object - ", object->getName().c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	CzXomlProperty prop;
	if (!LUA_ValueToProperty(lua, 3, prop))
	{
		CzScriptEngineLua::DisplayError(lua, "scene.add() value must be a string, number, boolean or vec - object - ", object->getName().c_str());
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "- property - ", prop_name.c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, ((CzScene*)object)->setProperty(prop_name.getHash(), prop, true));
 
	return 1;
}

//
// LUA_GetPropertyScene scene (object), property (string)
//
static int LUA_GetPropertyScene(lua_State *lua)
{
	if (lua_gettop(lua) < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.get() not enough parameters, expected scene (object), property (string)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the target object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::Scene_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.get() Invalid target object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	CzString prop_name, value;
	if (lua_isstring(lua, 2))
		prop_name = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "scene.get() property must be a property name (Param1) - object - ", object->getName().c_str());
		lua_pushnil(lua);
		return 1;
	}

	CzXomlProperty prop;
	if (!((CzScene*)object)->getProperty(prop_name.getHash(), prop))
	{
		CzScriptEngineLua::DisplayError(lua, "scene.get() scene property does not exist - ", prop_name.c_str());
		lua_pushnil(lua);
		return 1;
	}

	LUA_ReturnProperty(lua, prop);

	return 1;
}

//
// LUA_FindScene scene-name (string)
//
static int LUA_FindScene(lua_State *lua)
{
	if (lua_gettop(lua) < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.find() not enough parameters, expected scene-name (string)");
		lua_pushnil(lua);
		return 1;
	}

	const char* name = NULL;

	// Get the scene name
	if (lua_isstring(lua, 1))
		name = lua_tostring(lua, 1);
	if (name == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.find() Invalid name for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Locate scene
	CzScene* scene = NULL;
	IzXomlResource* parent = CZ_GLOBAL_RESOURCES->getResourceManager()->getParent();
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Game_Hash)
		scene = ((CzApp*)parent)->findScene(name);
	else
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = ((CzScene*)parent)->getParent()->findScene(name);

	if (scene == NULL)
	{
//		CzScriptEngineLua::DisplayError(lua, "scene.find() scene not found - ", name);
		lua_pushnil(lua);
		return 1;
	}

	// Return the scene
	lua_pushlightuserdata(lua, scene);

	return 1;
}

//
// LUA_FindScenesOfType scene-type (number)
//
static int LUA_FindScenesOfType(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.findOfType() not enough parameters, expected scene-type (number)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the type
	int type = 0;
	if (lua_isnumber(lua, 1))
		type = (int)lua_tonumber(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "scene.findOfType() expected number for Param0");
		lua_pushnil(lua);
		return 1;
	}

	CzVector<CzScene*> found_scenes;
	count = CzGlobals::App->findScenesOfType(type, found_scenes);
	if (count == 0)
	{
		lua_pushnil(lua);
		return 1;
	}

	lua_createtable(lua, count, 0);
	int new_table = lua_gettop(lua);
	int index = 1;

	// Add actors to table
	for (CzVector<CzScene*>::iterator it = found_scenes.begin(); it != found_scenes.end(); ++it)
	{
		lua_pushlightuserdata(lua, *it);
		lua_rawseti(lua, new_table, index++);
	}

	return 1;
}

//
// LUA_CreateScene scene-name (string), width (number), height (number), canvas_fit (string), origin (string), physics (boolean), batching (boolean), script-engine (string)
//
static int LUA_CreateScene(lua_State *lua)
{
	if (lua_gettop(lua) < 8)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.create() not enough parameters, scene-name (string), width (number), height (number), canvas_fit (string), origin (string), physics (boolean), batching (boolean), script-engine (string)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the scene name
	const char* name = NULL;
	if (lua_isstring(lua, 1))
		name = lua_tostring(lua, 1);
	if (name == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.create() Invalid name for Param0, expected string");
		lua_pushnil(lua);
		return 1;
	}

	// Get the scene size
	int width, height;
	if (lua_isnumber(lua, 2))
		width = (int)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "scene.create() Invalid width for Param1, expected number");
		lua_pushnil(lua);
		return 1;
	}
	if (lua_isnumber(lua, 3))
		height = (int)lua_tonumber(lua, 3);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "scene.create() Invalid height for Param2, expected number");
		lua_pushnil(lua);
		return 1;
	}

	// Get the scene canvas fit
	const char* canvas_fit = NULL;
	if (lua_isstring(lua, 4))
		canvas_fit = lua_tostring(lua, 4);
	if (canvas_fit == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.create() Invalid canvas_fit for Param3, expected string");
		lua_pushnil(lua);
		return 1;
	}

	// Get the scene canvas origin
	const char* canvas_org = NULL;
	if (lua_isstring(lua, 5))
		canvas_org = lua_tostring(lua, 5);
	if (canvas_org == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.create() Invalid canvas_fit for Param4, expected string");
		lua_pushnil(lua);
		return 1;
	}

	unsigned int canvas_fit_hash = CZ_HASH(canvas_fit);
	unsigned int canvas_org_hash = CZ_HASH(canvas_org);
	CzScene::eCanvasFit fit = CzScene::Fit_None;
	if (canvas_fit_hash == CZ_HASH("width"))
		fit = CzScene::Fit_Width;
	else
	if (canvas_fit_hash == CZ_HASH("height"))
		fit = CzScene::Fit_Height;
	else
	if (canvas_fit_hash == CZ_HASH("both"))
		fit = CzScene::Fit_Both;
	else
	if (canvas_fit_hash == CZ_HASH("best"))
		fit = CzScene::Fit_Best;
	CzScene::eCanvasOrigin org = CzScene::Origin_Centre;
	if (canvas_org_hash == CZ_HASH("top"))
		org = CzScene::Origin_Top;
	else
	if (canvas_org_hash == CZ_HASH("left"))
		org = CzScene::Origin_Left;
	else
	if (canvas_org_hash == CZ_HASH("topleft"))
		org = CzScene::Origin_TopLeft;

	// Get the scene physics / batching
	bool physics = false, batching = false;
	if (lua_isboolean(lua, 6))
		physics = lua_toboolean(lua, 6) != 0;
	else
	{
		CzScriptEngineLua::DisplayError(lua, "scene.create() Invalid physics for Param5, expected boolean");
		lua_pushnil(lua);
		return 1;
	}
	if (lua_isboolean(lua, 7))
		batching = lua_toboolean(lua, 7) != 0;
	else
	{
		CzScriptEngineLua::DisplayError(lua, "scene.create() Invalid batching for Param6, expected boolean");
		lua_pushnil(lua);
		return 1;
	}

	// Get the scene script engine
	const char* script_engine = NULL;
	if (lua_isstring(lua, 8))
		script_engine = lua_tostring(lua, 8);
	if (script_engine == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.create() Invalid script-engine for Param7, expected string");
		lua_pushnil(lua);
		return 1;
	}

	CzScene* scene = new CzScene();
	scene->setName(name);
	scene->Init();
	scene->setBatching(batching);
	scene->setVirtualTransform(width, height, 0, fit, org);

	// Create script engine
	CzString se = script_engine;
	if (script_engine != NULL)
		scene->setScriptEngine(se);

	// Add scene to game
	CzApp* game = (CzApp*)CZ_GLOBAL_RESOURCES->getResourceManager()->getParent();
	game->addScene(scene, false);

	// Return the scene
	lua_pushlightuserdata(lua, scene);

	return 1;
}

//
// LUA_DestroyScene(scene (object))
//
static int LUA_DestroyScene(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (lua_gettop(lua) < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.destroy() not enough parameters, expected scene (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the main game object
	CzApp* game = (CzApp*)CZ_GLOBAL_RESOURCES->getResourceManager()->getParent();

	// Get the object
	IzXomlResource* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (IzXomlResource*)lua_touserdata(lua, 1);
	if (object == NULL || object->getClassTypeHash() != CzHashes::Scene_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.destroy() Invalid object, expected a scene object in Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	game->removeScene((CzScene*)object);

	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_SetCurrentScene(scene-name (string)) - returns previous scene
//
static int LUA_SetCurrentScene(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (lua_gettop(lua) < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.setCurrent() not enough parameters, expected scene (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the main game object
	CzApp* game = (CzApp*)CZ_GLOBAL_RESOURCES->getResourceManager()->getParent();

	// Get the scene
	CzScene* scene = NULL;
	if (lua_isuserdata(lua, 1))
		scene = (CzScene*)lua_touserdata(lua, 1);
	if (scene == NULL || scene->getClassTypeHash() != CzHashes::Scene_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.setCurrent() Invalid scene for Param0");
		lua_pushnil(lua);
		return 1;
	}

	CzScene* prev_scene = game->getCurrentScene();
	game->changeScene(scene);

	lua_pushlightuserdata(lua, prev_scene);

	return 1;
}

//
// LUA_GetCurrentScene - returns current scene
//
static int LUA_GetCurrentScene(lua_State *lua)
{
	int count = lua_gettop(lua);

	// Get the main game object
	CzApp* game = (CzApp*)CZ_GLOBAL_RESOURCES->getResourceManager()->getParent();

	lua_pushlightuserdata(lua, game->getCurrentScene());

	return 1;
}

//
// LUA_SceneToScreen scene (object) x (number) y (number) - Returns vector
//
static int LUA_SceneToScreen(lua_State *lua)
{
	if (lua_gettop(lua) < 3)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.toScreen() not enough parameters, expected scene (object) x (number) y (number)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the scene object
	CzScene* scene = NULL;
	if (lua_isuserdata(lua, 1))
		scene = (CzScene*)lua_touserdata(lua, 1);
	if (scene == NULL || scene->getClassTypeHash() != CzHashes::Scene_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.toScreen() Invalid scene object");
		lua_pushnil(lua);
		return 1;
	}

	// Get x coord
	float x = 0;
	if (lua_isnumber(lua, 2))
		x = (float)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "scene.toScreen() expected a number for Param1");
		lua_pushnil(lua);
		return 1;
	}

	// Get y coord
	float y = 0;
	if (lua_isnumber(lua, 3))
		y = (float)lua_tonumber(lua, 3);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "scene.toScreen() expected a number for Param2");
		lua_pushnil(lua);
		return 1;
	}

	CzVec2 screen = scene->VirtualToScreen(x, y);
	lua_pushvec(lua, screen.x, screen.y, 0, 0);
 
	return 1;
}

//
// LUA_ScreenToScene scene (object) x (number) y (number) include_camera (boolean) - Returns vector
//
static int LUA_ScreenToScene(lua_State *lua)
{
	if (lua_gettop(lua) < 3)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.toScene() not enough parameters, expected scene (object) x (number) y (number) include_camera (boolean)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the scene object
	CzScene* scene = NULL;
	if (lua_isuserdata(lua, 1))
		scene = (CzScene*)lua_touserdata(lua, 1);
	if (scene == NULL || scene->getClassTypeHash() != CzHashes::Scene_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.toScene() Invalid scene object");
		lua_pushnil(lua);
		return 1;
	}

	// Get x coord
	float x = 0;
	if (lua_isnumber(lua, 2))
		x = (float)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "scene.toScene() expected a number for Param1");
		lua_pushnil(lua);
		return 1;
	}

	// Get y coord
	float y = 0;
	if (lua_isnumber(lua, 3))
		y = (float)lua_tonumber(lua, 3);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "scene.toScene() expected a number for Param2");
		lua_pushnil(lua);
		return 1;
	}

	// Get include camera parameter
	bool camera = false;
	if (lua_gettop(lua) > 3)
	{
		if (lua_isboolean(lua, 4))
			camera = lua_toboolean(lua, 4) != 0;
		else
		{
			CzScriptEngineLua::DisplayError(lua, "scene.toScene() expected a boolean for Param3");
			lua_pushnil(lua);
			return 1;
		}
	}

	CzVec2 s;
	if (camera)
		s = scene->ScreenToCamera(x, y);
	else
		s = scene->ScreenToVirtual(x, y);
	lua_pushvec(lua, s.x, s.y, 0, 0);
 
	return 1;
}

//
// LUA_UpdateScenePhysics scene (object), time (number)
//
static int LUA_UpdateScenePhysics(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.updatePhysics() not enough parameters, expected scene (object), time (number)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get scene
	CzScene* scene = NULL;
	if (lua_isuserdata(lua, 1))
		scene = (CzScene*)lua_touserdata(lua, 1);
	if (scene == NULL || scene->getClassTypeHash() != CzHashes::Scene_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.updatePhysics() invalid scene (Param0), expected scene");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get time
	float time;
	if (lua_isnumber(lua, 2))
		time = (float)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "scene.updatePhysics() Invalid time expected number for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	scene->UpdatePhysics(time);

	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_CleanupScene scene (object)
//
static int LUA_CleanupScene(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.cleanup() not enough parameters, expected scene (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get scene
	CzScene* scene = NULL;
	if (lua_isuserdata(lua, 1))
		scene = (CzScene*)lua_touserdata(lua, 1);
	if (scene == NULL || scene->getClassTypeHash() != CzHashes::Scene_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "scene.cleanup() invalid scene (Param0), expected scene");
		lua_pushboolean(lua, false);
		return 1;
	}

	scene->CleanupRemovedActors();

	lua_pushboolean(lua, true);
	return 1;
}

static const luaL_Reg g_scenelib[] =
{
	{ "set",			LUA_SetPropertyScene}, 
	{ "add",			LUA_AddPropertyScene},
	{ "get",			LUA_GetPropertyScene},
	{ "find",			LUA_FindScene},
	{ "findOfType",		LUA_FindScenesOfType},
	{ "create",			LUA_CreateScene},
	{ "destroy",		LUA_DestroyScene},
	{ "setCurrent",		LUA_SetCurrentScene},
	{ "getCurrent",		LUA_GetCurrentScene},
	{ "toScreen",		LUA_SceneToScreen},
	{ "toScene",		LUA_ScreenToScene},
	{ "updatePhysics",	LUA_UpdateScenePhysics},
	{ "cleanup",		LUA_CleanupScene},
	{NULL, NULL}
};

LUALIB_API int luaopen_scene(lua_State *L)
{
	luaL_register(L, "scene", g_scenelib);
	lua_pop(L, 1);

	return 1;
}


