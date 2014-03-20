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

#include "IzPlatformSys.h"
#include "CzScriptLua.h"
#include "IzPlatformOther.h"
#include "CzPlatform.h"
#include "lua/CzLuaMatrix.h"
#include "CzApp.h"
#include "CzPlatform.h"
#include "CzSettings.h"
#include "CzMatrix4.h"

//
// LUA_MatrixCreate - returns matrix (object)
//
static int LUA_MatrixCreate(lua_State *lua)
{
	CzMatrix4* mat = new CzMatrix4();

	lua_pushlightuserdata(lua, mat);

	return 1;
}

//
// LUA_MatrixDestroy matrix (object)
//
static int LUA_MatrixDestroy(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.destroy() not enough parameters, expected matrix (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzMatrix4* mat = NULL;
	if (lua_isuserdata(lua, 1))
		mat = (CzMatrix4*)lua_touserdata(lua, 1);
	if (mat == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.destroy() expected Matrix for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	delete mat;

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_MatrixSet matrix (object), values (number)
//
static int LUA_MatrixSet(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.set() not enough parameters, expected matrix (object), values (up to 16 numbers)");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzMatrix4* mat = NULL;
	if (lua_isuserdata(lua, 1))
		mat = (CzMatrix4*)lua_touserdata(lua, 1);
	if (mat == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.set() expected Matrix for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	int index = 0;
	for (int t = 0; t < count - 1; t++)
	{
		float value = 0;
		if (lua_isnumber(lua, t + 2))
			value = (float)lua_tonumber(lua, t + 2);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "matrix.set() expected value for Param");
			lua_pushboolean(lua, false);
			return 1;
		}

		*((float *)(mat->m) + index) = value;
		index++;
	}

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_MatrixSetAt matrix (object), index (number), value (number)
//
static int LUA_MatrixSetAt(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 3)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.setAt() not enough parameters, expected matrix (object), index (number), value (number)");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzMatrix4* mat = NULL;
	if (lua_isuserdata(lua, 1))
		mat = (CzMatrix4*)lua_touserdata(lua, 1);
	if (mat == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.setAt() expected Matrix for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	int index = 0;
	if (lua_isnumber(lua, 2))
		index = (int)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.setAt() expected number for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}
	if (index < 0 || index > 15)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.setAt() index out of range for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	float value = 0;
	if (lua_isnumber(lua, 3))
		value = (float)lua_tonumber(lua, 3);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.setAt() expected value for Param2");
		lua_pushboolean(lua, false);
		return 1;
	}

	*((float *)(mat->m) + index) = value;

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_MatrixGetAt matrix (object), index (number) -- returns value
//
static int LUA_MatrixGetAt(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.getAt() not enough parameters, expected matrix (object), index (number)");
		lua_pushnil(lua);
		return 1;
	}

	CzMatrix4* mat = NULL;
	if (lua_isuserdata(lua, 1))
		mat = (CzMatrix4*)lua_touserdata(lua, 1);
	if (mat == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.getAt() expected Matrix for Param0");
		lua_pushnil(lua);
		return 1;
	}

	int index = 0;
	if (lua_isnumber(lua, 2))
		index = (int)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.getAt() expected number for Param1");
		lua_pushnil(lua);
		return 1;
	}
	if (index < 0 || index > 15)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.getAt() index out of range for Param1");
		lua_pushnil(lua);
		return 1;
	}

	lua_pushnumber(lua, *((float *)(mat->m) + index));

	return 1;
}

//
// LUA_MatrixIdentity matrix (object)
//
static int LUA_MatrixIdentity(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.identity() not enough parameters, expected matrix (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzMatrix4* mat = NULL;
	if (lua_isuserdata(lua, 1))
		mat = (CzMatrix4*)lua_touserdata(lua, 1);
	if (mat == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.identity() expected Matrix for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	mat->Identity();

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_MatrixTranspose matrix (object)
//
static int LUA_MatrixTranspose(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.transpose() not enough parameters, expected matrix (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzMatrix4* mat = NULL;
	if (lua_isuserdata(lua, 1))
		mat = (CzMatrix4*)lua_touserdata(lua, 1);
	if (mat == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.transpose() expected Matrix for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	mat->Transpose();

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_MatrixQuat matrix (object), x (number), y (number), z (number), w (number)
//
static int LUA_MatrixQuat(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 5)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.quat() not enough parameters, expected matrix (object), x (number), y (number), z (number), w (number)");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzMatrix4* mat = NULL;
	if (lua_isuserdata(lua, 1))
		mat = (CzMatrix4*)lua_touserdata(lua, 1);
	if (mat == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.quat() expected Matrix for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	float x = 0;
	if (lua_isnumber(lua, 2))
		x = (float)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.quat() expected number for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	float y = 0;
	if (lua_isnumber(lua, 3))
		y = (float)lua_tonumber(lua, 3);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.quat() expected number for Param2");
		lua_pushboolean(lua, false);
		return 1;
	}

	float z = 0;
	if (lua_isnumber(lua, 4))
		z = (float)lua_tonumber(lua, 4);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.quat() expected number for Param3");
		lua_pushboolean(lua, false);
		return 1;
	}

	float w = 0;
	if (lua_isnumber(lua, 5))
		w = (float)lua_tonumber(lua, 5);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.quat() expected number for Param4");
		lua_pushboolean(lua, false);
		return 1;
	}

	mat->Quat(x, y, z, w);

	lua_pushboolean(lua, true);

	return 1;
}
//
// LUA_MatrixRotate matrix (object), x-angle (number), y-angle (number), z-angle (number)
//
static int LUA_MatrixRotate(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 4)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.rotate() not enough parameters, expected matrix (object), x-angle (number), y-angle (number), z-angle (number)");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzMatrix4* mat = NULL;
	if (lua_isuserdata(lua, 1))
		mat = (CzMatrix4*)lua_touserdata(lua, 1);
	if (mat == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.rotate() expected Matrix for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	float x_angle = 0;
	if (lua_isnumber(lua, 2))
		x_angle = (float)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.rotate() expected number for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	float y_angle = 0;
	if (lua_isnumber(lua, 3))
		y_angle = (float)lua_tonumber(lua, 3);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.rotate() expected number for Param2");
		lua_pushboolean(lua, false);
		return 1;
	}

	float z_angle = 0;
	if (lua_isnumber(lua, 4))
		z_angle = (float)lua_tonumber(lua, 4);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.rotate() expected number for Param3");
		lua_pushboolean(lua, false);
		return 1;
	}

	mat->Rotate(x_angle, y_angle, z_angle);

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_MatrixScale matrix (object), x-scale (number), y-scale (number), z-scale (number)
//
static int LUA_MatrixScale(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 4)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.scale() not enough parameters, expected matrix (object), x-scale (number), y-scale (number), z-scale (number)");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzMatrix4* mat = NULL;
	if (lua_isuserdata(lua, 1))
		mat = (CzMatrix4*)lua_touserdata(lua, 1);
	if (mat == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.scale() expected Matrix for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	float x_scale = 0;
	if (lua_isnumber(lua, 2))
		x_scale = (float)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.scale() expected number for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	float y_scale = 0;
	if (lua_isnumber(lua, 3))
		y_scale = (float)lua_tonumber(lua, 3);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.scale() expected number for Param2");
		lua_pushboolean(lua, false);
		return 1;
	}

	float z_scale = 0;
	if (lua_isnumber(lua, 4))
		z_scale = (float)lua_tonumber(lua, 4);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.scale() expected number for Param3");
		lua_pushboolean(lua, false);
		return 1;
	}

	mat->Scale(x_scale, y_scale, z_scale);

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_MatrixTranslate matrix (object), x-translate (number), y-translate (number), z-translate (number)
//
static int LUA_MatrixTranslate(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 4)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.translate() not enough parameters, expected matrix (object), x-translate (number), y-translate (number), z-translate (number)");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzMatrix4* mat = NULL;
	if (lua_isuserdata(lua, 1))
		mat = (CzMatrix4*)lua_touserdata(lua, 1);
	if (mat == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.translate() expected Matrix for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	float x_translate = 0;
	if (lua_isnumber(lua, 2))
		x_translate = (float)lua_tonumber(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.translate() expected number for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	float y_translate = 0;
	if (lua_isnumber(lua, 3))
		y_translate = (float)lua_tonumber(lua, 3);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.translate() expected number for Param2");
		lua_pushboolean(lua, false);
		return 1;
	}

	float z_translate = 0;
	if (lua_isnumber(lua, 4))
		z_translate = (float)lua_tonumber(lua, 4);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.translate() expected number for Param3");
		lua_pushboolean(lua, false);
		return 1;
	}

	mat->Translate(x_translate, y_translate, z_translate);

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_MatrixTransform matrix (object), vector (vec) - return vector (vec)
//
static int LUA_MatrixTransform(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.transform() not enough parameters, expected matrix (object), vector (vec)");
		lua_pushnil(lua);
		return 1;
	}

	CzMatrix4* mat = NULL;
	if (lua_isuserdata(lua, 1))
		mat = (CzMatrix4*)lua_touserdata(lua, 1);
	if (mat == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.transform() expected Matrix for Param0");
		lua_pushnil(lua);
		return 1;
	}

	if (lua_istable(lua, 2))
	{
		int size = luaL_getn(lua, 2);

		CzVec3* verts = new CzVec3[size]();
		CzVec3* overts = verts;

		lua_pushnil(lua);  // First key
		while (lua_next(lua, 2) != 0)
		{
			const float* v = lua_tovec(lua, -1);
			verts->x = v[0];
			verts->y = v[1];
			verts->z = v[2];
			verts++;
			lua_pop(lua, 1);
		}

/*		for (int t = 1; t < size + 1; t++)
		{
			lua_rawgeti(lua, 2, t);
			const float* v = lua_tovec(lua, -1);
			verts->x = v[0];
			verts->y = v[1];
			verts->z = v[2];
			verts++;
		}*/

		mat->TransformN(overts, overts, size);

		lua_createtable(lua, size, 0);
		int new_table = lua_gettop(lua);
		verts = overts;
		for (int t = 1; t < size + 1; t++)
		{
			lua_pushvec(lua, verts->x, verts->y, verts->z, 0);
			lua_rawseti(lua, new_table, t);
			verts++;
		}

		delete [] overts;
	}
	else
	{
		const float* v;
		if (lua_isvec(lua, 2))
			v = lua_tovec(lua, 2);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "matrix.transform() expected vec for Param1");
			lua_pushnil(lua);
			return 1;
		}

		CzVec3 ov = mat->Transform(v[0], v[1], v[2]);
		lua_pushvec(lua, ov.x, ov.y, ov.z, 0);
	}

	return 1;
}

//
// LUA_MatrixTransformProject matrix (object), (vector (vec) or array of vec), d (number, optional) - return vector (vec)
//
static int LUA_MatrixTransformProject(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.transformProject() not enough parameters, expected matrix (object), (vector (vec) or array of vec), d");
		lua_pushnil(lua);
		return 1;
	}

	CzMatrix4* mat = NULL;
	if (lua_isuserdata(lua, 1))
		mat = (CzMatrix4*)lua_touserdata(lua, 1);
	if (mat == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.transformProject() expected Matrix for Param0");
		lua_pushnil(lua);
		return 1;
	}

	float d = 256;
	if (count > 2)
	{
		if (lua_isnumber(lua, 3))
			d = (float)lua_tonumber(lua, 3);
	}

	if (lua_istable(lua, 2))
	{
		int size = luaL_getn(lua, 2);

		CzVec3* verts = new CzVec3[size]();
		CzVec3* overts = verts;

		lua_pushnil(lua);  // First key
		while (lua_next(lua, 2) != 0)
		{
			const float* v = lua_tovec(lua, -1);
			verts->x = v[0];
			verts->y = v[1];
			verts->z = v[2];
			verts++;
			lua_pop(lua, 1);
		}
/*		for (int t = 1; t < size + 1; t++)
		{
			lua_rawgeti(lua, 2, t);
			const float* v = lua_tovec(lua, -1);
			verts->x = v[0];
			verts->y = v[1];
			verts->z = v[2];
			verts++;
		}*/

		mat->TransformProjectN(overts, overts, size, d);

		lua_createtable(lua, size, 0);
		int new_table = lua_gettop(lua);
		verts = overts;
		for (int t = 1; t < size + 1; t++)
		{
			lua_pushvec(lua, verts->x, verts->y, verts->z, 0);
			lua_rawseti(lua, new_table, t);
			verts++;
		}

		delete [] overts;
	}
	else
	{
		const float* v;
		if (lua_isvec(lua, 2))
			v = lua_tovec(lua, 2);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "matrix.transformProject() expected vec for Param1");
			lua_pushnil(lua);
			return 1;
		}

		CzVec3 ov = mat->TransformProject(v[0], v[1], v[2], d);
		lua_pushvec(lua, ov.x, ov.y, ov.z, 0);
	}

	return 1;
}

//
// LUA_MatrixProject (vector (vec) or array of vec), d (number, optional) - return vector (vec)
//
static int LUA_MatrixProject(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.project() not enough parameters, expected (vector (vec) or array of vec), d");
		lua_pushnil(lua);
		return 1;
	}

	float d = 256;
	if (count > 1)
	{
		if (lua_isnumber(lua, 2))
			d = (float)lua_tonumber(lua, 2);
	}

	if (lua_istable(lua, 1))
	{
		int size = luaL_getn(lua, 1);

		CzVec3* verts = new CzVec3[size]();
		CzVec3* overts = verts;

		lua_pushnil(lua);  // First key
		while (lua_next(lua, 1) != 0)
		{
			const float* v = lua_tovec(lua, -1);
			verts->x = v[0];
			verts->y = v[1];
			verts->z = v[2];
			verts++;
			lua_pop(lua, 1);
		}
/*		for (int t = 1; t < size + 1; t++)
		{
			lua_rawgeti(lua, 1, t);
			const float* v = lua_tovec(lua, -1);
			verts->x = v[0];
			verts->y = v[1];
			verts->z = v[2];
			verts++;
		}*/

		CzMatrix4::ProjectN(overts, overts, size, d);

		lua_createtable(lua, size, 0);
		int new_table = lua_gettop(lua);
		verts = overts;
		for (int t = 1; t < size + 1; t++)
		{
			lua_pushvec(lua, verts->x, verts->y, verts->z, 0);
			lua_rawseti(lua, new_table, t);
			verts++;
		}

		delete [] overts;
	}
	else
	{
		const float* v;
		if (lua_isvec(lua, 1))
			v = lua_tovec(lua, 1);
		else
		{
			CzScriptEngineLua::DisplayError(lua, "matrix.project() expected vec for Param1");
			lua_pushnil(lua);
			return 1;
		}

		CzVec3 ov = CzMatrix4::Project(v[0], v[1], v[2], d);
		lua_pushvec(lua, ov.x, ov.y, ov.z, 0);
	}

	return 1;
}


//
// LUA_MatrixMultiply matrix (object), matrix (object)
//
static int LUA_MatrixMultiply(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.multiply() not enough parameters, expected matrix (object), matrix (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzMatrix4* mat1 = NULL;
	if (lua_isuserdata(lua, 1))
		mat1 = (CzMatrix4*)lua_touserdata(lua, 1);
	if (mat1 == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.multiply() expected Matrix for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzMatrix4* mat2 = NULL;
	if (lua_isuserdata(lua, 2))
		mat2 = (CzMatrix4*)lua_touserdata(lua, 2);
	if (mat2 == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.multiply() expected Matrix for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	mat1->Multiply(mat2);
	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_MatrixPostMultiply matrix (object), matrix (object)
//
static int LUA_MatrixPostMultiply(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.postMultiply() not enough parameters, expected matrix (object), matrix (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzMatrix4* mat1 = NULL;
	if (lua_isuserdata(lua, 1))
		mat1 = (CzMatrix4*)lua_touserdata(lua, 1);
	if (mat1 == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.postMultiply() expected Matrix for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzMatrix4* mat2 = NULL;
	if (lua_isuserdata(lua, 2))
		mat2 = (CzMatrix4*)lua_touserdata(lua, 2);
	if (mat2 == NULL)
	{
		CzScriptEngineLua::DisplayError(lua, "matrix.postMultiply() expected Matrix for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	mat1->MultiplyPost(mat2);
	lua_pushboolean(lua, true);

	return 1;
}
//
// Lua Geometry library
//
static const luaL_Reg g_Matrixlib[] =
{
	{"create",			LUA_MatrixCreate},
	{"destroy",			LUA_MatrixDestroy},
	{"set",				LUA_MatrixSet},
	{"setAt",			LUA_MatrixSetAt},
	{"getAt",			LUA_MatrixGetAt},
	{"identity",		LUA_MatrixIdentity},
	{"transpose",		LUA_MatrixTranspose},
	{"quat",			LUA_MatrixQuat},
	{"rotate",			LUA_MatrixRotate},
	{"scale",			LUA_MatrixScale},
	{"translate",		LUA_MatrixTranslate},
	{"transform",		LUA_MatrixTransform},
	{"transformProject", LUA_MatrixTransformProject},
	{"multiply",		LUA_MatrixMultiply},
	{"postMultiply",	LUA_MatrixPostMultiply},
	{"project",			LUA_MatrixProject},
	{NULL, NULL}
};

LUALIB_API int luaopen_matrix(lua_State *L)
{
	luaL_register(L, "matrix", g_Matrixlib);
	lua_pop(L, 1);

	return 1;
}


