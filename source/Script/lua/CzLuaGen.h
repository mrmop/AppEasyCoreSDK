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

#if !defined(_CZ_LUA_GEN_H_)
#define _CZ_LUA_GEN_H_

#include "CzScript.h"

// LUA headers
extern "C"
{
   #include "lua.h"
   #include "lauxlib.h"
   #include "lualib.h"
}

bool LUA_ValueToString(lua_State *lua, int stack_index, CzString &string);
bool LUA_ValueToProperty(lua_State *lua, int stack_index, CzXomlProperty &prop);
void LUA_ReturnProperty(lua_State *lua, CzXomlProperty &prop);


#endif	// _CZ_LUA_GEN_H_
