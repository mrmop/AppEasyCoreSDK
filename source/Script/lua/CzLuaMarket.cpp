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
#include "CzMarket.h"
#include "CzTemplates.h"

//
// LUA_MarketAvailable()
//
static int LUA_MarketAvailable(lua_State *lua)
{
	// Return market availability
	lua_pushboolean(lua, PLATFORM_MARKET->isAvailable());

	return 1;
}

//
// LUA_MarketFindProduct(product-name (string))
//
static int LUA_MarketFindProduct(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "market.find() not enough parameters, expected product-name (string)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the product name
	const char* product = NULL;
	if (lua_isstring(lua, 1))
		product = lua_tostring(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "market.find() Invalid parameter, expected string for product-name for Param0");
		lua_pushnil(lua);
		return 1;
	}

	if (PLATFORM_MARKET->getActiveMarket() == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "market.find() Market has not been created");
		lua_pushnil(lua);
		return 1;
	}

	CzMarketProduct* p = PLATFORM_MARKET->getActiveMarket()->findProductByName(CZ_HASH(product));
	if (p == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "market.find() Product not found - ", product);
		lua_pushnil(lua);
		return 1;
	}

	// Return the product
	lua_pushlightuserdata(lua, p);

	return 1;
}

//
// LUA_MarketGetProducts()
//
static int LUA_MarketGetProducts(lua_State *lua)
{
	int count = lua_gettop(lua);

	CzMarket* market = PLATFORM_MARKET->getActiveMarket();
	if (market == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "market.getProducts(), market has not been created");
		lua_pushnil(lua);
		return 1;
	}

	// Put all product ID's into table
	lua_createtable(lua, market->getProductCount(), 0);
	int new_table = lua_gettop(lua);
	int index = 1;
	for (CzMarket::_ProductIterator it = market->begin(); it != market->end(); ++it)
	{
		lua_pushstring(lua, (*it)->Name.c_str());
		lua_rawseti(lua, new_table, index++);
	}

	return 1;
}

//
// LUA_MarketProductPurchase(product (object))
//
static int LUA_MarketProductPurchase(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "market.purchase() not enough parameters, expected product (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the product object
	CzMarketProduct* product = NULL;
	if (lua_islightuserdata(lua, 1))
		product = (CzMarketProduct*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "market.purchase() Invalid target object, expected product");
		lua_pushboolean(lua, false);
		return 1;
	}

	if (PLATFORM_MARKET->getActiveMarket() == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "market.purchase() Market has not been created");
		lua_pushboolean(lua, false);
		return 1;
	}

	if (PLATFORM_MARKET->getActiveMarket()->isBusy())
	{
		CzScriptEngineLua::DisplayError(lua, "market.purchase() Market is busy");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Purchase the product
	lua_pushboolean(lua, PLATFORM_MARKET->getActiveMarket()->PurchaseProduct(product->ProductID.c_str()));

	return 1;
}

//
// LUA_MarketSetCallback(callback (function))
//
static int LUA_MarketSetCallback(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "market.setCallback() not enough parameters, expected callback (function)");
		lua_pushboolean(lua, false);
		return 1;
	}

	if (PLATFORM_MARKET->getActiveMarket() == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "market.purchase() Market has not been created");
		lua_pushboolean(lua, false);
		return 1;
	}

	if (PLATFORM_MARKET->getActiveMarket()->isBusy())
	{
		CzScriptEngineLua::DisplayError(lua, "market.purchase() Market is busy");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the callback
	void* callback = NULL;
	if (lua_isfunction(lua, 1))
		callback = (void*)lua_topointer(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "market.purchase() Invalid callback, expected function for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// If previous callback ref set then free it
	CzScriptCallback& cb = PLATFORM_MARKET->getActiveMarket()->getScriptCallback();
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
// LUA_MarketIsProductPurchased(product (object))
//
static int LUA_MarketIsProductPurchased(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "market.purchased() not enough parameters, expected product (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the product object
	CzMarketProduct* product = NULL;
	if (lua_islightuserdata(lua, 1))
		product = (CzMarketProduct*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "market.purchased() Invalid target object, expected product");
		lua_pushboolean(lua, false);
		return 1;
	}

	if (PLATFORM_MARKET->getActiveMarket() == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "market.purchased() Market has not been created");
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, product->Purchased);

	return 1;
}

//
// LUA_MarketIsProducConsumable(product (object))
//
static int LUA_MarketIsProducConsumable(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "market.consumable() not enough parameters, expected product (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the product object
	CzMarketProduct* product = NULL;
	if (lua_islightuserdata(lua, 1))
		product = (CzMarketProduct*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "market.consumable() Invalid target object, expected product");
		lua_pushboolean(lua, false);
		return 1;
	}

	if (PLATFORM_MARKET->getActiveMarket() == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "market.consumable(), market has not been created");
		lua_pushboolean(lua, false);
		return 1;
	}


	lua_pushboolean(lua, product->Consumable);

	return 1;
}

//
// LUA_MarketRestore()
//
static int LUA_MarketRestore(lua_State *lua)
{
	int count = lua_gettop(lua);

	if (PLATFORM_MARKET->getActiveMarket() == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "market.restore(), market has not been created");
		lua_pushnumber(lua, -1);
		return 1;
	}

	lua_pushboolean(lua, PLATFORM_MARKET->getActiveMarket()->RestoreProducts());

	return 1;
}

//
// LUA_MarketCurrentProduct()
//
static int LUA_MarketCurrentProduct(lua_State *lua)
{
	int count = lua_gettop(lua);

	if (PLATFORM_MARKET->getActiveMarket() == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "market.currentProduct(), market has not been created");
		lua_pushnil(lua);
		return 1;
	}
	const char* product_id = PLATFORM_MARKET->getActiveMarket()->getCurrentProductID();

	lua_pushlightuserdata(lua, PLATFORM_MARKET->getActiveMarket()->findProduct(product_id));

	return 1;
}

//
// LUA_MarketIsBusy()
//
static int LUA_MarketIsBusy(lua_State *lua)
{
	int count = lua_gettop(lua);

	if (PLATFORM_MARKET->getActiveMarket() == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "market.busy() Market has not been created");
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, PLATFORM_MARKET->getActiveMarket()->isBusy());

	return 1;
}

//
// LUA_MarketGetPrice(product (object))
//
static int LUA_MarketGetPrice(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "market.price() not enough parameters, expected product (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the product object
	CzMarketProduct* product = NULL;
	if (lua_islightuserdata(lua, 1))
		product = (CzMarketProduct*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "market.price(), invalid product for Param0");
		lua_pushnil(lua);
		return 1;
	}

	lua_pushnumber(lua, product->Price);

	return 1;
}

//
// LUA_MarketGetName(product (object))
//
static int LUA_MarketGetName(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "market.name() not enough parameters, expected product (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the product object
	CzMarketProduct* product = NULL;
	if (lua_islightuserdata(lua, 1))
		product = (CzMarketProduct*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "market.name(), invalid product for Param0");
		lua_pushnil(lua);
		return 1;
	}

	lua_pushstring(lua, product->Name.c_str());

	return 1;
}

//
// LUA_MarketGetStatus(product (object))
//
static int LUA_MarketGetStatus(lua_State *lua)
{
	int count = lua_gettop(lua);

	lua_pushnumber(lua, (double)PLATFORM_MARKET->getStatus());

	return 1;
}
static const luaL_Reg g_marketlib[] =
{
	{"available",		LUA_MarketAvailable}, 
	{"find",			LUA_MarketFindProduct}, 
	{"products",		LUA_MarketGetProducts}, 
	{"purchased",		LUA_MarketIsProductPurchased}, 
	{"consumable",		LUA_MarketIsProducConsumable}, 
	{"setCallback",		LUA_MarketSetCallback}, 
	{"purchase",		LUA_MarketProductPurchase}, 
	{"currentProduct",  LUA_MarketCurrentProduct}, 
	{"restore",			LUA_MarketRestore}, 
	{"busy",			LUA_MarketIsBusy}, 
	{"price",			LUA_MarketGetPrice}, 
	{"name",			LUA_MarketGetName}, 
	{"status",			LUA_MarketGetStatus}, 
	{NULL, NULL}
};

LUALIB_API int luaopen_market(lua_State *L)
{
	luaL_register(L, "market", g_marketlib);
	lua_pop(L, 1);
  
	return 1;
}

