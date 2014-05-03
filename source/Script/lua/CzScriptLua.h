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

#if !defined(_CZ_SCRIPT_LUA_H_)
#define _CZ_SCRIPT_LUA_H_

#include "CzScript.h"

// LUA headers
extern "C"
{
   #include "lua.h"
   #include "lstate.h"
   #include "lauxlib.h"
   #include "lualib.h"
}

/**
 @class	CzScriptEngineLua

 @brief	LUA script engine.

 */

class CzScriptEngineLua : public IzScriptEngine
{
public:
	// Proprties
protected:
	lua_State*		Lua;			///< The lua engines state
public:
	lua_State*		getState()		{ return Lua; }
	// Properties end

protected:
public:

	CzScriptEngineLua() : IzScriptEngine(), Lua(NULL) {}
	virtual ~CzScriptEngineLua()
	{
		Release();
	}

	int				Init(CzScene* scene);
	void			Release();
	int				LoadScript(const char* script, int script_len, const char* name);
	int				CallFunction(IzXomlResource* object, CzString* function_name, CzString* param1, CzString* param2, CzString* param3);
	CzXomlProperty getVariable(CzString* var_name);
	int				CallFunctionRef(int function_ref, CzString* param1, CzString* param2, CzString* param3, CzString* param4);
	int				CallFunctionRefWithTable(int function_ref, CzList<const char*>* properties, CzList<const char*>* values);
	int				CallFunctionRefWithObject(void* object, int function_ref, CzString* param1, CzString* param2, CzString* param3, CzString* param4);
	CzString		getType() { return CzString("lua"); }
	

	static void		DisplayError(lua_State *lua, const char* message);
	static void		DisplayError(lua_State *lua, const char* message, const char* data);

	// Internal
};




#endif	// _CZ_SCRIPT_LUA_H_