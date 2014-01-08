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
#include "CzScene.h"
#include "CzApp.h"
#include "CzFacebook.h"
#include "CzSettings.h"

//
// LUA_FacebookAvailable() - Returns true if Facebook is available
//
static int LUA_FacebookAvailable(lua_State *lua)
{
	// Return Facebook availability
	lua_pushboolean(lua, PLATFORM_FACEBOOK->isAvailable());

	return 1;
}

//
// LUA_FacebookLogin(permissions (table), app-id (string, optional) ) - Returns true if login process has started successfully
//
static int LUA_FacebookLogin(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "facebook.login() not enough parameters, expected permissions (table), app-id (string, optional)");
		lua_pushboolean(lua, false);
		return 1;
	}

	char* permissions[128];
	int num_permissions = 0;
	if (lua_istable(lua, 1))
	{
		// Table is in the stack at index 't'
		lua_pushnil(lua);  // First key
		while (lua_next(lua, 1) != 0)
		{
			// Get param name and value
			const char* value = lua_tostring(lua, -1);
			if (value != NULL)
			{
				permissions[num_permissions] = new char [strlen(value + 1)];
				strcpy(permissions[num_permissions], value);
				num_permissions++;
				if (num_permissions >= 128)
				{
					lua_pop(lua, 1);
					break;
				}
			}

			// Removes value but keeps key for the next iteration
			lua_pop(lua, 1);
		}
	}
	else
	{
		CzScriptEngineLua::DisplayError(lua, "facebook.login() Invalid parameter, expected permissions (table) for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get App ID if supplied
	if (count > 1)
	{
		// Get the App ID
		const char* app_id = NULL;
		if (lua_isstring(lua, 2))
			app_id = lua_tostring(lua, 2);
		if (app_id == NULL)
		{
			CzScriptEngineLua::DisplayError(lua, "facebook.login() Invalid App ID, expected string for Param1");
			lua_pushboolean(lua, false);
			return 1;
		}
		PLATFORM_FACEBOOK->setAppID(app_id);
	}
	else
	{
		PLATFORM_FACEBOOK->setAppID(CZ_SETTINGS->getFacebookAppID().c_str());
	}

	// Begin Facebook login process
	bool success = PLATFORM_FACEBOOK->Login((const char**)permissions, num_permissions);

	// Clean up permission strings
	for (int t = 0; t < num_permissions; t++)
		delete permissions[t];

	lua_pushboolean(lua, success);

	return 1;
}

//
// LUA_FacebookLogout() - Returns true if success
//
static int LUA_FacebookLogout(lua_State *lua)
{
	// Log out of Facebook
	lua_pushboolean(lua, PLATFORM_FACEBOOK->Logout());

	return 1;
}

//
// LUA_FacebookReauthorise(permissions (table)) - Returns true if successfull
//
static int LUA_FacebookReauthorise(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "facebook.reauthor() not enough parameters, expected permissions (table)");
		lua_pushboolean(lua, false);
		return 1;
	}

	char* permissions[128];
	int num_permissions = 0;
	if (lua_istable(lua, 1))
	{
		// Table is in the stack at index 't'
		lua_pushnil(lua);  // First key
		while (lua_next(lua, 1) != 0)
		{
			// Get param name and value
			const char* value = lua_tostring(lua, -1);
			if (value != NULL)
			{
				permissions[num_permissions] = new char [strlen(value + 1)];
				strcpy(permissions[num_permissions], value);
				num_permissions++;
				if (num_permissions >= 128)
				{
					lua_pop(lua, 1);
					break;
				}
			}

			// Removes value but keeps key for the next iteration
			lua_pop(lua, 1);
		}
	}
	else
	{
		CzScriptEngineLua::DisplayError(lua, "facebook.reauthor() Invalid parameter, expected permissions (table) for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Begin Facebook reauthorisation process
	bool success = PLATFORM_FACEBOOK->Reauthorise((const char**)permissions, num_permissions);

	// Clean up permission strings
	for (int t = 0; t < num_permissions; t++)
		delete permissions[t];

	lua_pushboolean(lua, success);

	return 1;
}

//
// LUA_FacebookLoggedIn() - Returns true if logged in
//
static int LUA_FacebookLoggedIn(lua_State *lua)
{
	// Return Facebook logged in status
	lua_pushboolean(lua, PLATFORM_FACEBOOK->IsLoggedIn());

	return 1;
}

//
// LUA_FacebookRequest(method-name (string), http-method (string), parameters (table, optional)) - Returns the created request
//
static int LUA_FacebookRequest(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "facebook.request() not enough parameters, expected method-name (string), http-method (string). parameters (table, optional)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the method name
	const char* method_name = NULL;
	if (lua_isstring(lua, 1))
		method_name = lua_tostring(lua, 1);
	if (method_name == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "facebook.request() Invalid method-name, expected string for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Get the http method
	const char* http_method = NULL;
	if (lua_isstring(lua, 2))
		http_method = lua_tostring(lua, 2);
	if (http_method == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "facebook.request() Invalid http-method, expected string for Param1");
		lua_pushnil(lua);
		return 1;
	}

	// Create facebook request
	CzFacebookRequest request = PLATFORM_FACEBOOK->InitRequest(method_name, http_method);
	if (request == NULL)
		lua_pushnil(lua);

	// Set parameters
	if (count > 2)
	{
		if (lua_istable(lua, 3))
		{
			// Table is in the stack at index 't'
			lua_pushnil(lua);  // First key
			while (lua_next(lua, 3) != 0)
			{
				// Get param name and value
				const char* name = lua_tostring(lua, -2);
				if (name != NULL)
				{
					if (lua_isnumber(lua, -1))
						PLATFORM_FACEBOOK->AddParamNumber(request, name, (int64)lua_tonumber(lua, -1));
					else
					if (lua_isstring(lua, -1))
						PLATFORM_FACEBOOK->AddParamString(request, name, lua_tostring(lua, -1));
					else
						CzScriptEngineLua::DisplayError(lua, "facebook.request() Invalid parameter type in parameters table (Param2), expected string or number");
				}

				lua_pop(lua, 1);
			}
		}
		else
		{
			CzScriptEngineLua::DisplayError(lua, "facebook.request() Invalid parameter, expected parameters (table) for Param2");
			lua_pushnil(lua);
			return 1;
		}
	}

	// Send facebook request
	PLATFORM_FACEBOOK->SendRequest(request);
	lua_pushlightuserdata(lua, request);

	return 1;
}

//
// LUA_FacebookGraph(graph-path (string), http-method (string), parameters (table, optional)) - Returns the created request
//
static int LUA_FacebookGraph(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "facebook.graph() not enough parameters, expected graph-path (string), http-method (string). parameters (table, optional)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the graph-path
	const char* graph_path = NULL;
	if (lua_isstring(lua, 1))
		graph_path = lua_tostring(lua, 1);
	if (graph_path == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "facebook.graph() Invalid graph-path, expected string for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Get the http method
	const char* http_method = NULL;
	if (lua_isstring(lua, 2))
		http_method = lua_tostring(lua, 2);
	if (http_method == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "facebook.graph() Invalid http-method, expected string for Param1");
		lua_pushnil(lua);
		return 1;
	}

	// Create facebook request
	CzFacebookRequest request = PLATFORM_FACEBOOK->InitGraphRequest(graph_path, http_method);
	if (request == NULL)
		lua_pushnil(lua);

	// Set parameters
	if (count > 2)
	{
		if (lua_istable(lua, 3))
		{
			// Table is in the stack at index 't'
			lua_pushnil(lua);  // First key
			while (lua_next(lua, 3) != 0)
			{
				// Get param name and value
				const char* name = lua_tostring(lua, -2);
				if (name != NULL)
				{
					if (lua_isnumber(lua, -1))
						PLATFORM_FACEBOOK->AddParamNumber(request, name, (int64)lua_tonumber(lua, -1));
					else
					if (lua_isstring(lua, -1))
						PLATFORM_FACEBOOK->AddParamString(request, name, lua_tostring(lua, -1));
					else
						CzScriptEngineLua::DisplayError(lua, "facebook.request() Invalid parameter type in parameters table (Param2), expected string or number");
				}

				lua_pop(lua, 1);
			}
		}
		else
		{
			CzScriptEngineLua::DisplayError(lua, "facebook.request() Invalid parameter, expected parameters (table) for Param2");
			lua_pushnil(lua);
			return 1;
		}
	}

	// Send facebook request
	PLATFORM_FACEBOOK->SendRequest(request);
	lua_pushlightuserdata(lua, request);

	return 1;
}

//
// LUA_FacebookSetCallback(callback (function)) - Returns true if success
//
static int LUA_FacebookSetCallback(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "facebook.setCallback() not enough parameters, expected callback (function)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the callback
	void* callback = NULL;
	if (lua_isfunction(lua, 1))
		callback = (void*)lua_topointer(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "facebook.setCallback() Invalid callback, expected function for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// If previous callback ref set then free it
	CzScriptCallback& cb = PLATFORM_FACEBOOK->getScriptCallback();
	if (cb.Valid)
		lua_unref(((CzScriptEngineLua*)cb.ScriptEngine)->getState(), cb.FunctionRef);

	// Generate a new callback reference to callback function
	int callback_index = luaL_ref(lua, LUA_REGISTRYINDEX);
	cb.FunctionRef = callback_index;
	CzScene* container = (CzScene*)lua->user_data;
	if (container == NULL)
		cb.ScriptEngine = CZ_GLOBAL_RESOURCES->getScriptEngine();
	else
		cb.ScriptEngine = container->getScriptEngine();
	cb.Valid = true;

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_FacebookErrorCode(request (object)) - Returns error code
//
static int LUA_FacebookErrorCode(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "facebook.errorCode() not enough parameters, expected request (object)");
		lua_pushnil(lua);
		return 1;
	}

	if (!lua_islightuserdata(lua, 1))
	{
		CzScriptEngineLua::DisplayError(lua, "facebook.errorCode() Invalid parameter, expected request (object) for Param0");
		lua_pushnil(lua);
		return 1;
	}

	lua_pushnumber(lua, PLATFORM_FACEBOOK->GetErrorCode((CzFacebookRequest)lua_touserdata(lua, 1)));

	return 1;
}

//
// LUA_FacebookError(request (object)) - Returns error string
//
static int LUA_FacebookError(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "facebook.error() not enough parameters, expected request (object)");
		lua_pushnil(lua);
		return 1;
	}

	if (!lua_islightuserdata(lua, 1))
	{
		CzScriptEngineLua::DisplayError(lua, "facebook.error() Invalid parameter, expected request (object) for Param0");
		lua_pushnil(lua);
		return 1;
	}

	lua_pushstring(lua, PLATFORM_FACEBOOK->GetError((CzFacebookRequest)lua_touserdata(lua, 1)));

	return 1;
}

//
// LUA_FacebookResponse(request (object)) - Returns response string
//
static int LUA_FacebookResponse(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "facebook.response() not enough parameters, expected request (object)");
		lua_pushnil(lua);
		return 1;
	}

	if (!lua_islightuserdata(lua, 1))
	{
		CzScriptEngineLua::DisplayError(lua, "facebook.response() Invalid parameter, expected request (object) for Param0");
		lua_pushnil(lua);
		return 1;
	}

	lua_pushstring(lua, PLATFORM_FACEBOOK->GetResponse((CzFacebookRequest)lua_touserdata(lua, 1)));

	return 1;
}

//
// LUA_FacebookAccessToken() - Returns the Facebook access token as a string
//
static int LUA_FacebookAccessToken(lua_State *lua)
{
	// Return Facebook access token
	lua_pushstring(lua, PLATFORM_FACEBOOK->GetAccessToken());

	return 1;
}

//
// LUA_FacebookWaitForCallback() - Waits for the rspone callback to be called before continuing
//
static int LUA_FacebookWaitForCallback(lua_State *lua)
{
	PLATFORM_FACEBOOK->WaitForCallback();

	return 1;
}

static const luaL_Reg g_facebooklib[] =
{
	{ "available",		LUA_FacebookAvailable}, 
	{ "login",			LUA_FacebookLogin}, 
	{ "logout",			LUA_FacebookLogout},
	{ "token",			LUA_FacebookAccessToken},
	{ "reauthor",		LUA_FacebookReauthorise},
	{ "loggedIn",		LUA_FacebookLoggedIn},
	{ "request",		LUA_FacebookRequest},
	{ "graph",			LUA_FacebookGraph},
	{ "setCallback",	LUA_FacebookSetCallback},
	{ "errorCode",		LUA_FacebookErrorCode},
	{ "error",			LUA_FacebookError},
	{ "response",		LUA_FacebookResponse},
	{ "waitCallback",	LUA_FacebookWaitForCallback},
	{NULL, NULL}
};

LUALIB_API int luaopen_facebook(lua_State *L)
{
	luaL_register(L, "facebook", g_facebooklib);
	lua_pop(L, 1);
  
	return 1;
}

