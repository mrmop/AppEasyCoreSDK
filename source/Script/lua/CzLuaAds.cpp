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
#include "CzActor.h"
#include "IzPlatformAds.h"

//
// LUA_AdsAvailable(provider (string)) - Returns true if Ads is available
//
static int LUA_AdsAvailable(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "ads.available() not enough parameters, expected provider (string)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get ad provider
	const char* provider = NULL;
	if (lua_isstring(lua, 1))
		provider = lua_tostring(lua, 1);
	if (provider == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "ads.available() Invalid provider, expected string for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Return Ads availability
#if defined(ADS_FLURRY_APPSPOT_ENABLE)
	if (strcmp(provider, "flurry_appspot") == 0)
	{
		lua_pushboolean(lua, PLATFORM_ADS->isAvailable(Ads_FlurryAppSpot));
		return 1;
	}
#endif // ADS_FLURRY_APPSPOT_ENABLE
#if defined(ADS_REVMOB_ENABLE)
	if (strcmp(provider, "revmob") == 0)
	{
		lua_pushboolean(lua, PLATFORM_ADS->isAvailable(Ads_Revmob));
		return 1;
	}
#endif // ADS_REVMOB_ENABLE

	lua_pushboolean(lua, false);
	return 1;
}

//
// LUA_AdsInit(provider (string), id (string), data1 (string, optional) data2 (number, optional)) - Returns true if initialised
//
static int LUA_AdsInit(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "ads.init() not enough parameters, expected provider (string), id (string), data1 (string, optional) data2 (number, optional)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get ad provider
	const char* provider = NULL;
	if (lua_isstring(lua, 1))
		provider = lua_tostring(lua, 1);
	if (provider == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "ads.init() Invalid provider, expected string for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get id
	const char* id = NULL;
	if (lua_isstring(lua, 2))
		id = lua_tostring(lua, 2);
	if (id == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "ads.init() Invalid id, expected string for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get data1, data2
	const char* data1 = NULL;
	int data2 = 0;
	if (count > 2)
	{
		if (lua_isstring(lua, 3))
			data1 = lua_tostring(lua, 3);
		if (data1 == NULL)
		{
			CzScriptEngineLua::DisplayError(lua, "ads.init() Invalid data1, expected string for Param2");
			lua_pushboolean(lua, false);
			return 1;
		}
	}
	if (count > 3)
	{
		if (lua_isnumber(lua, 4))
			data2 = (int)lua_tonumber(lua, 4);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "ads.init() Invalid data2, expected number for Param3");
			lua_pushboolean(lua, false);
			return 1;
		}
	}

	// Init ads system
#if defined(ADS_FLURRY_APPSPOT_ENABLE)
	if (strcmp(provider, "flurry_appspot") == 0)
	{
		PLATFORM_ADS->Init(Ads_FlurryAppSpot, id, data1, data2);
		return 1;
	}
#endif // ADS_FLURRY_APPSPOT_ENABLE
#if defined(ADS_REVMOB_ENABLE)
	if (strcmp(provider, "revmob") == 0)
	{
		PLATFORM_ADS->Init(Ads_Revmob, id, data1, data2);
		return 1;
	}
#endif // ADS_REVMOB_ENABLE

	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_AdsSetProvider(provider (string), type (string)) - Returns true if success
//
static int LUA_AdsSetProvider(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "ads.setProvider() not enough parameters, expected provider (string), ad-type (string)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get ad provider
	const char* provider = NULL;
	if (lua_isstring(lua, 1))
		provider = lua_tostring(lua, 1);
	if (provider == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "ads.setProvider() Invalid provider, expected string for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get id
	const char* type = NULL;
	if (lua_isstring(lua, 2))
		type = lua_tostring(lua, 2);
	if (type == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "ads.setProvider() Invalid type expected string for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Init ads system
#if defined(ADS_FLURRY_APPSPOT_ENABLE)
	if (strcmp(provider, "flurry_appspot") == 0)
	{
		if (strcmp(type, "banner") == 0)
			PLATFORM_ADS->setProvider(AdType_Banner, Ads_FlurryAppSpot);
		else
		if (strcmp(type, "inter") == 0)
			PLATFORM_ADS->setProvider(AdType_Interstitial, Ads_FlurryAppSpot);
		return 1;
	}
#endif // ADS_FLURRY_APPSPOT_ENABLE
#if defined(ADS_REVMOB_ENABLE)
	if (strcmp(provider, "revmob") == 0)
	{
		if (strcmp(type, "banner") == 0)
			PLATFORM_ADS->setProvider(AdType_Banner, Ads_Revmob);
		else
		if (strcmp(type, "inter") == 0)
			PLATFORM_ADS->setProvider(AdType_Interstitial, Ads_Revmob);
		else
		if (strcmp(type, "popup") == 0)
			PLATFORM_ADS->setProvider(AdType_Popup, Ads_Revmob);
		return 1;
	}
#endif // ADS_REVMOB_ENABLE

	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_AdsSetCallback(callback (function)) - Returns true if succeeds
//
static int LUA_AdsSetCallback(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "ads.setCallback() not enough parameters, expected callback (function)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the callback
	void* callback = NULL;
	if (lua_isfunction(lua, 1))
		callback = (void*)lua_topointer(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "ads.setCallback() Invalid callback, expected function for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// If previous callback ref set then free it
	CzScriptCallback& cb = PLATFORM_ADS->getScriptCallback();
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
// LUA_AdsLoad(type (string), data1 (string, optional) data2 (number, optional)) - Returns true if succeeds
//
static int LUA_AdsLoad(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "ads.load() not enough parameters, expected type (string), data1 (string, optional) data2 (number, optional)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get ad type
	const char* type = NULL;
	if (lua_isstring(lua, 1))
		type = lua_tostring(lua, 1);
	if (type == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "ads.load() Invalid type, expected string for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get data1, data2
	const char* data1 = NULL;
	int data2 = 0;
	if (count > 1)
	{
		if (lua_isstring(lua, 2))
			data1 = lua_tostring(lua, 2);
		if (data1 == NULL)
		{
			CzScriptEngineLua::DisplayError(lua, "ads.load() Invalid data1, expected string for Param1");
			lua_pushboolean(lua, false);
			return 1;
		}
	}
	if (count > 2)
	{
		if (lua_isnumber(lua, 3))
			data2 = (int)lua_tonumber(lua, 3);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "ads.load() Invalid data2, expected number for Param2");
			lua_pushboolean(lua, false);
			return 1;
		}
	}

	// Init ads system
	if (strcmp(type, "banner") == 0)
	{
		PLATFORM_ADS->Load(AdType_Banner, data1, data2);
	}
	else
	if (strcmp(type, "inter") == 0)
	{
		PLATFORM_ADS->Load(AdType_Interstitial, data1, data2);
	}
	else
	{
		CzScriptEngineLua::DisplayError(lua, "ads.load() Invalid ad type for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_AdsLoadShow(type (string), data1 (string, optional) data2 (number, optional)) - Returns true if succeeds
//
static int LUA_AdsLoadShow(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "ads.loadShow() not enough parameters, expected type (string), data1 (string, optional) data2 (number, optional)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get ad type
	const char* type = NULL;
	if (lua_isstring(lua, 1))
		type = lua_tostring(lua, 1);
	if (type == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "ads.loadShow() Invalid type, expected string for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get data1, data2
	const char* data1 = NULL;
	int data2 = 0;
	if (count > 1)
	{
		if (lua_isstring(lua, 2))
			data1 = lua_tostring(lua, 2);
		if (data1 == NULL)
		{
			CzScriptEngineLua::DisplayError(lua, "ads.loadShow() Invalid data1, expected string for Param1");
			lua_pushboolean(lua, false);
			return 1;
		}
	}
	if (count > 2)
	{
		if (lua_isnumber(lua, 3))
			data2 = (int)lua_tonumber(lua, 3);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "ads.loadShow() Invalid data2, expected number for Param2");
			lua_pushboolean(lua, false);
			return 1;
		}
	}

	// Init ads system
	if (strcmp(type, "banner") == 0)
	{
		PLATFORM_ADS->LoadShow(AdType_Banner, data1, data2);
	}
	else
	if (strcmp(type, "inter") == 0)
	{
		PLATFORM_ADS->LoadShow(AdType_Interstitial, data1, data2);
	}
	else
	{
		CzScriptEngineLua::DisplayError(lua, "ads.load() Invalid ad type for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, true);
	return 1;
}


//
// LUA_AdsShow(type (string)) - Returns true if succeeds
//
static int LUA_AdsShow(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "ads.show() not enough parameters, expected type (string))");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get ad type
	const char* type = NULL;
	if (lua_isstring(lua, 1))
		type = lua_tostring(lua, 1);
	if (type == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "ads.show() Invalid type, expected string for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Hide ads
	if (strcmp(type, "banner") == 0)
	{
		PLATFORM_ADS->Show(AdType_Banner);
	}
	else
	if (strcmp(type, "inter") == 0)
	{
		PLATFORM_ADS->Show(AdType_Interstitial);
	}
	else
	{
		CzScriptEngineLua::DisplayError(lua, "ads.show() Invalid ad type for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}
	lua_pushboolean(lua, true);
	return 1;
}

//
// LUA_AdsHide(type (string)) - Returns true if succeeds
//
static int LUA_AdsHide(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "ads.hide() not enough parameters, expected type (string))");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get ad type
	const char* type = NULL;
	if (lua_isstring(lua, 1))
		type = lua_tostring(lua, 1);
	if (type == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "ads.hide() Invalid type, expected string for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Hide ads
	if (strcmp(type, "banner") == 0)
	{
		PLATFORM_ADS->Hide(AdType_Banner);
	}
	else
	if (strcmp(type, "inter") == 0)
	{
		PLATFORM_ADS->Hide(AdType_Interstitial);
	}
	else
	{
		CzScriptEngineLua::DisplayError(lua, "ads.hide() Invalid ad type for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}
	lua_pushboolean(lua, true);
	return 1;
}



//
// Lua Ads library
//
static const luaL_Reg g_adslib[] =
{
	{ "available",		LUA_AdsAvailable }, 
	{ "init",			LUA_AdsInit }, 
	{ "setProvider",	LUA_AdsSetProvider }, 
	{ "setCallback",	LUA_AdsSetCallback }, 
	{ "load",			LUA_AdsLoad }, 
	{ "loadShow",		LUA_AdsLoadShow }, 
	{ "show",			LUA_AdsShow }, 
	{ "hide",			LUA_AdsHide }, 
	{NULL, NULL}
};

LUALIB_API int luaopen_ads(lua_State *L)
{
	luaL_register(L, "ads", g_adslib);
	lua_pop(L, 1);

	return 1;
}
