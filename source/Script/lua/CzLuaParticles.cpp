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
#include "CzActorParticles.h"
#include "CzScene.h"
#include "CzApp.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"

//
// LUA_CreateParticle actor (object), type (number)
//
static int LUA_CreateParticle(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 4)
	{
		CzScriptEngineLua::DisplayError(lua, "particles.create() not enough parameters, expected particle-actor (object), count (number), x (number), y (number), srcrect (vec4, optional)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the actor
	CzActorParticles* actor = NULL;
	if (lua_isuserdata(lua, 1))
		actor = (CzActorParticles*)lua_touserdata(lua, 1);
	if (actor == NULL || actor->getActualClassTypeHash() != CzHashes::ActorParticles_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "particles.create() Invalid ActorParticles object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Get count
	int num_particles = 0;
	if (lua_isnumber(lua, 2))
		num_particles = (int)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "particles.create() Invalid count for Param1, expected number");
		lua_pushnil(lua);
		return 1;
	}
	if (num_particles <= 0)
	{
		CzScriptEngineLua::DisplayError(lua, "particles.create() Invalid count for Param1, expected number > 0");
		lua_pushnil(lua);
		return 1;
	}

	// Get x position
	float x = 0;
	if (lua_isnumber(lua, 3))
		x = (float)lua_tonumber(lua, 3);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "particles.create() Invalid x position for Param2, expected number");
		lua_pushnil(lua);
		return 1;
	}

	// Get y position
	float y = 0;
	if (lua_isnumber(lua, 4))
		y = (float)lua_tonumber(lua, 4);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "particles.create() Invalid y position for Param3, expected number");
		lua_pushnil(lua);
		return 1;
	}

	// Get srcrect
	const float* srcrect = NULL;
	if (count > 4)
	{
		if (lua_isvec(lua, 5))
			srcrect = lua_tovec(lua, 5);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "particles.create() Invalid srcrect for Param4, expected number");
			lua_pushnil(lua);
			return 1;
		}
	}

	CzIRect rc;
	CzActorParticle* p0 = actor->getParticle(0);
	CzActorParticle* p;
	if (p0 != NULL)
		p = p0->Clone();
	else
		p = new CzActorParticle();

	if (srcrect == NULL)
	{
		if (p0 != NULL)
			rc = p0->Visual->getSrcRect();
	}
	else
	{
		rc.x = (int)*srcrect++;
		rc.y = (int)*srcrect++;
		rc.w = (int)*srcrect++;
		rc.h = (int)*srcrect++;
	}
	p->Position.x = x;
	p->Position.y = y;
	if (actor->addParticle(p, rc, num_particles))
	{
		p->Visual->setDestSize(rc.w, rc.h);
		lua_pushlightuserdata(lua, p);
	}
	else
		lua_pushnil(lua);

	return 1;
}

//
// LUA_FindParticle actor (object), index (number)
//
static int LUA_FindParticle(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "particles.find() not enough parameters, expected particle-actor (object), index (number)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the actor
	CzActorParticles* actor = NULL;
	if (lua_isuserdata(lua, 1))
		actor = (CzActorParticles*)lua_touserdata(lua, 1);
	if (actor == NULL || actor->getActualClassTypeHash() != CzHashes::ActorParticles_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "particles.find() Invalid ActorParticles object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Get index
	int index = 0;
	if (lua_isnumber(lua, 2))
		index = (int)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "particles.find() Invalid index for Param1, expected number");
		lua_pushnil(lua);
		return 1;
	}

	CzActorParticle* p = actor->getParticle(index);
	if (p == NULL)
		lua_pushnil(lua);
	else
		lua_pushlightuserdata(lua, p);

	return 1;
}

//
// LUA_ParticleSet particle (object), property-name (string), value (any)
//
static int LUA_ParticleSet(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 3)
	{
		CzScriptEngineLua::DisplayError(lua, "particles.set() not enough parameters, expected particle (object), property-name (string), value (string, number, boolean or vec)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	CzActorParticle* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (CzActorParticle*)lua_touserdata(lua, 1);
	if (object == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "particles.set() Invalid target object for Param0, expected particle");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzString prop_name;
	if (lua_isstring(lua, 2))
		prop_name = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "particles.set() must be a property name (Param1)");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzXomlProperty prop;
	if (!LUA_ValueToProperty(lua, 3, prop))
	{
		CzScriptEngineLua::DisplayError(lua, "particles.set() value must be a string, number, boolean or vec (Param2)");
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "- property - ", prop_name.c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, object->setProperty(prop_name.getHash(), prop, false));
 
	return 1;
}

//
// LUA_ParticleAdd particle (object), property-name (string), value (any)
//
static int LUA_ParticleAdd(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 3)
	{
		CzScriptEngineLua::DisplayError(lua, "particles.add() not enough parameters, expected particle (object), property-name (string), value (string, number, boolean or vec)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the target object
	CzActorParticle* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (CzActorParticle*)lua_touserdata(lua, 1);
	if (object == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "particles.add() Invalid target object for Param0, expected particle");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzString prop_name, value;
	if (lua_isstring(lua, 2))
		prop_name = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "particles.add() must be a property name (Param1)");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzXomlProperty prop;
	if (!LUA_ValueToProperty(lua, 3, prop))
	{
		CzScriptEngineLua::DisplayError(lua, "particles.add() value must be a string, number, boolean or vec (Param3)");
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "- property - ", prop_name.c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, object->setProperty(prop_name.getHash(), prop, true));
	
	return 1;
}

//
// LUA_ParticleGet particle (object), property-name (string)
//
static int LUA_ParticleGet(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "particles.get() not enough parameters, expected particle (object), property-name (string)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the target object
	CzActorParticle* object = NULL;
	if (lua_isuserdata(lua, 1))
		object = (CzActorParticle*)lua_touserdata(lua, 1);
	if (object == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "particles.get() Invalid target object for Param0, expected particle");
		lua_pushnil(lua);
		return 1;
	}

	CzString prop_name;
	if (lua_isstring(lua, 2))
		prop_name = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "particles.get() must be a property name (Param1)");
		lua_pushnil(lua);
		return 1;
	}

	CzXomlProperty prop;
	if (!object->getProperty(prop_name.getHash(), prop))
	{
		CzScriptEngineLua::DisplayError(lua, "particles.get() actor property does not exist - ", prop_name.c_str());
		lua_pushnil(lua);
		return 1;
	}
	
	LUA_ReturnProperty(lua, prop);

	return 1;
}

//
// LUA_ParticleRemoveAll particle-actor (object)
//
static int LUA_ParticleRemoveAll(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "particles.removeAll() not enough parameters, expected particle-actor (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the actor
	CzActorParticles* actor = NULL;
	if (lua_isuserdata(lua, 1))
		actor = (CzActorParticles*)lua_touserdata(lua, 1);
	if (actor == NULL || actor->getActualClassTypeHash() != CzHashes::ActorParticles_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "particles.removeAll() Invalid ActorParticles object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	actor->removeAllParticles();

	lua_pushboolean(lua, true);


	return 1;
}

//
// Lua library
//
static const luaL_Reg g_particleslib[] =
{
	{"create",			LUA_CreateParticle}, 
	{"find",			LUA_FindParticle}, 
	{"get",				LUA_ParticleGet}, 
	{"set",				LUA_ParticleSet}, 
	{"add",				LUA_ParticleAdd}, 
	{"removeAll",		LUA_ParticleRemoveAll}, 
	{NULL, NULL}
};

LUALIB_API int luaopen_particles(lua_State *L)
{
	luaL_register(L, "particles", g_particleslib);
	lua_pop(L, 1);

	return 1;
}
