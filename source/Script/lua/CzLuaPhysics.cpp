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

//Add support for weld and possibly mouse joints
//Add car demo using wheel joint

//
// LUA_FindJoint actor (object), joint-name (string) - returns a joint object or nil if not found
// - Searches an actor for a named joint
//
static int LUA_FindJoint(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "physics.findJoint() not enough parameters, expected actor (object), joint-name (string)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the actor that contains the joint
	CzActor* actor = NULL;
	if (lua_isuserdata(lua, 1))
		actor = (CzActor*)lua_touserdata(lua, 1);
	if (actor == NULL || actor->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "physics.findJoint() Invalid actor object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Get joint name
	const char* joint_name = NULL;
	if (lua_isstring(lua, 2))
		joint_name = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "physics.findJoint() invalid joint name (Param1), expected string");
		lua_pushnil(lua);
		return 1;
	}

	IzBox2dJoint* joint = actor->getBox2dBody()->findJoint(CZ_HASH(joint_name));
	if (joint == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "physics.findJoint() joint not found - ", joint_name);
		lua_pushnil(lua);
		return 1;
	}

	lua_pushlightuserdata(lua, joint);

	return 1;
}

//
// LUA_DestroyJoint actor (object), joint (object)
// - Removes joint from actor
//
static int LUA_DestroyJoint(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "physics.destroyJoint() not enough parameters, expected actor (object), joint (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the actor that contains the joint
	CzActor* actor = NULL;
	if (lua_isuserdata(lua, 1))
		actor = (CzActor*)lua_touserdata(lua, 1);
	if (actor == NULL || actor->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "physics.destroyJoint() Invalid actor object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get joint
	IzBox2dJoint* joint = NULL;
	if (lua_isuserdata(lua, 2))
		joint = (IzBox2dJoint*)lua_touserdata(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "physics.destroyJoint() invalid joint (Param1), expected joint, actor is ", actor->getName().c_str());
		lua_pushboolean(lua, false);
		return 1;
	}

	actor->getBox2dBody()->removeJoint(joint);

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_SetJointBodyA joint (object), actor (object)
//
static int LUA_SetJointBodyA(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "physics.setJointBodyA() not enough parameters, expected joint (object), actor (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get joint
	IzBox2dJoint* joint = NULL;
	if (lua_isuserdata(lua, 1))
		joint = (IzBox2dJoint*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "physics.setJointBodyA() invalid joint (Param0), expected joint");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the actor that contains the joint
	CzActor* actor = NULL;
	if (lua_isuserdata(lua, 2))
		actor = (CzActor*)lua_touserdata(lua, 2);
	if (actor == NULL || actor->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "physics.setJointBodyA() Invalid actor object for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	joint->BodyA = actor->getBox2dBody();

	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_SetJointBodyB joint (object), actor (object)
//
static int LUA_SetJointBodyB(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "physics.setJointBodyB() not enough parameters, expected joint (object), actor (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get joint
	IzBox2dJoint* joint = NULL;
	if (lua_isuserdata(lua, 1))
		joint = (IzBox2dJoint*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "physics.setJointBodyB() invalid joint (Param0), expected joint");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the actor that contains the joint
	CzActor* actor = NULL;
	if (lua_isuserdata(lua, 2))
		actor = (CzActor*)lua_touserdata(lua, 2);
	if (actor == NULL || actor->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzScriptEngineLua::DisplayError(lua, "physics.setJointBodyB() Invalid actor object for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	joint->BodyB = actor->getBox2dBody();

	lua_pushboolean(lua, true);
	return 1;
}



static const luaL_Reg g_physicslib[] =
{
	{"findJoint",		LUA_FindJoint},
	{"destroyJoint",	LUA_DestroyJoint},
	{"setJointBodyA",	LUA_SetJointBodyA},
	{"setJointBodyB",	LUA_SetJointBodyB},
//	{"createJoint",		LUA_CreateJoint},
//	{"changeMaterial",	LUA_ChangeMaterial},
	{NULL, NULL}
};


LUALIB_API int luaopen_physics(lua_State *L)
{
	luaL_register(L, "physics", g_physicslib);
	lua_pop(L, 1);

	return 1;
}

