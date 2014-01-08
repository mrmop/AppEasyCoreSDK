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


//
// LUA_XmlCreateParser() - Creates a new xml parser
//
static int LUA_XmlCreateParser(lua_State *lua)
{
	CzXmlParser*	xml = new CzXmlParser();

	lua_pushlightuserdata(lua, xml);

	return 1;
}

//
// LUA_XmlDestroyParser(parser (object)) - Creates a new xml parser
//
static int LUA_XmlDestroyParser(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "xml.destroyParser() not enough parameters, expected parser (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the parser
	CzXmlParser* parser = NULL;
	if (lua_isuserdata(lua, 1))
		parser = (CzXmlParser*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.destroyParser() Invalid parameter, expected xml parser object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	SAFE_DELETE(parser)

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_XmlCreateNode(node-name (string)) - Creates a new xml node
//
static int LUA_XmlCreateNode(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "xml.createNode() not enough parameters, expected node-name (string)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the parser
	const char* node_name = NULL;
	if (lua_isstring(lua, 1))
		node_name = lua_tostring(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.createNode() Invalid parameter, expected xml node-name for Param0");
		lua_pushnil(lua);
		return 1;
	}

	CzXmlNode* node = new CzXmlNode();
	node->SetName(node_name, strlen(node_name));
	node->Managed = false;
	lua_pushlightuserdata(lua, node);

	return 1;
}

//
// LUA_XmlDestroyNode(node (object)) - Destroys an xml node
//
static int LUA_XmlDestroyNode(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "xml.destroyNode() not enough parameters, expected node (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the node
	CzXmlNode* node = NULL;
	if (lua_isuserdata(lua, 1))
		node = (CzXmlNode*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.destroyNode() Invalid parameter, expected xml node object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzXmlNode* parent = node->Parent;
	if (parent != NULL)
		parent->RemoveChild(node);
	SAFE_DELETE(node)

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_XmlAddNode(parent-node (object), child-node (object)) - Adds a child-node to the parent-node's node list
//
static int LUA_XmlAddNode(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "xml.addNode() not enough parameters, expected parent-node (object), child-node (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the parent-node
	CzXmlNode* parent = NULL;
	if (lua_isuserdata(lua, 1))
		parent = (CzXmlNode*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.addNode() Invalid parameter, expected xml node object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the child-node
	CzXmlNode* child = NULL;
	if (lua_isuserdata(lua, 2))
		child = (CzXmlNode*)lua_touserdata(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.addNode() Invalid parameter, expected xml node object for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	parent->AddChild(child);

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_XmlRemoveNode(node (object)) - Removes an xml node
//
static int LUA_XmlRemoveNode(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "xml.removeNode() not enough parameters, expected node (object)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the node
	CzXmlNode* node = NULL;
	if (lua_isuserdata(lua, 1))
		node = (CzXmlNode*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.removeNode() Invalid parameter, expected xml node object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	CzXmlNode* parent = node->Parent;
	if (parent != NULL)
		parent->RemoveChild(node);

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_XmlSetNodeName(node (object), node-name (string)) - Sets the nodes name
//
static int LUA_XmlSetNodeName(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "xml.setNodeName() not enough parameters, expected node (object), node-name (string)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the node
	CzXmlNode* node = NULL;
	if (lua_isuserdata(lua, 1))
		node = (CzXmlNode*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.setNodeName() Invalid parameter, expected xml node object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the name
	const char* node_name = NULL;
	if (lua_isstring(lua, 2))
		node_name = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.setNodeName() Invalid parameter, expected xml node-name for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	node->SetName(node_name, strlen(node_name));
	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_XmlGetNodeName(node (object)) - Gets the nodes name
//
static int LUA_XmlGetNodeName(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "xml.getNodeName() not enough parameters, expected node (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the node
	CzXmlNode* node = NULL;
	if (lua_isuserdata(lua, 1))
		node = (CzXmlNode*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.getNodeName() Invalid parameter, expected xml node object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	lua_pushstring(lua, node->GetName().c_str());

	return 1;
}

//
// LUA_XmlSetNodeValue(node (object), node-value (string)) - Sets the nodes value
//
static int LUA_XmlSetNodeValue(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "xml.seNodeValue() not enough parameters, expected node (object), node-value (string)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the node
	CzXmlNode* node = NULL;
	if (lua_isuserdata(lua, 1))
		node = (CzXmlNode*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.seNodeValue() Invalid parameter, expected xml node object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the value
	const char* node_value = NULL;
	if (lua_isstring(lua, 2))
		node_value = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.seNodeValue() Invalid parameter, expected xml node-value for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	node->SetValue(node_value, strlen(node_value));
	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_XmlGetNodeValue(node (object)) - Gets the nodes value
//
static int LUA_XmlGetNodeValue(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "xml.getNodeValue() not enough parameters, expected node (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the node
	CzXmlNode* node = NULL;
	if (lua_isuserdata(lua, 1))
		node = (CzXmlNode*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.getNodeValue() Invalid parameter, expected xml node object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	lua_pushstring(lua, node->getValue().c_str());

	return 1;
}

//
// LUA_XmlSetAttribs(node (object), attributes (table)) - Sets the attributes of a node
//
static int LUA_XmlSetAttribs(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "xml.setAttribs() not enough parameters, expected node (object), attributes (table)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the node
	CzXmlNode* node = NULL;
	if (lua_isuserdata(lua, 1))
		node = (CzXmlNode*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.setAttribs() Invalid parameter, expected xml node object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	if (lua_istable(lua, 2))
	{
		// Table is in the stack at index 't'
		lua_pushnil(lua);  // First key
		while (lua_next(lua, 2) != 0)
		{
			// Get param name and value
			const char* name = lua_tostring(lua, -2);
			const char* value = lua_tostring(lua, -1);

			if (name != NULL && value != NULL)
			{
				// Set base template paramater
				CzXmlAttribute* index_attrib = new CzXmlAttribute();
				index_attrib->Managed = false;
				index_attrib->setName(name);
				index_attrib->setValue(value);
				node->AddAttribute(index_attrib);
			}

			// Removes value but keeps key for the next iteration
			lua_pop(lua, 1);
		}
	}
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "xml.setAttribs() param1 should be of type table");
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, true);

	return 1;
}

//
// LUA_XmlGetAttribs(node (object)) - Gets the attributes of a node
//
static int LUA_XmlGetAttribs(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "xml.getAttribs() not enough parameters, expected node (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the node
	CzXmlNode* node = NULL;
	if (lua_isuserdata(lua, 1))
		node = (CzXmlNode*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.getAttribs() Invalid parameter, expected xml node object for Param0");
		lua_pushnil(lua);
		return 1;
	}
	count = node->getAttributeCount();
	if (count == 0)
	{
		lua_pushnil(lua);
		return 1;
	}

	// Add attributes to table
	lua_newtable(lua);
	int new_table = lua_gettop(lua);
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); ++it)
	{
		lua_pushstring(lua, (*it)->getName().c_str());	// Push key (attribute name)
		lua_pushstring(lua, (*it)->getValue().c_str());	// Push value (attribute value)
		lua_settable(lua, new_table);
	}

	return 1;
}

//
// LUA_XmlGetRoot(parser (object)) - Gets the root node
//
static int LUA_XmlGetRoot(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "xml.root() not enough parameters, expected parser (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the parser
	CzXmlParser* parser = NULL;
	if (lua_isuserdata(lua, 1))
		parser = (CzXmlParser*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.root() Invalid parameter, expected xml parser object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	lua_pushlightuserdata(lua, parser->getRoot());

	return 1;
}

//
// LUA_XmlFindFirstNamedNode(parent-node (object), node-name (string)) - Gets the first node with the specified name
//
static int LUA_XmlFindFirstNamedNode(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "xml.findFirstNamedNode() not enough parameters, expected parent-node (object), node-name (string)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the node
	CzXmlNode* node = NULL;
	if (lua_isuserdata(lua, 1))
		node = (CzXmlNode*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.findFirstNamedNode() Invalid parameter, expected xml node object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Get the name
	const char* node_name = NULL;
	if (lua_isstring(lua, 2))
		node_name = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.findFirstNamedNode() Invalid parameter, expected xml node-name for Param1");
		lua_pushnil(lua);
		return 1;
	}

	lua_pushlightuserdata(lua, node->getFirstNamedNode(CzString::CalculateHash(node_name)));

	return 1;
}

//
// LUA_XmlGetChildren(parent-node (object)) - Gets all child nodes as a table
//
static int LUA_XmlGetChildren(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "xml.children() not enough parameters, expected parent-node (object)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the node
	CzXmlNode* node = NULL;
	if (lua_isuserdata(lua, 1))
		node = (CzXmlNode*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.children() Invalid parameter, expected xml node object for Param0");
		lua_pushnil(lua);
		return 1;
	}

	// Add nodes to table
	count = node->Children.size();
	lua_createtable(lua, count, 0);
	int new_table = lua_gettop(lua);
	int index = 1;
	for (CzXmlNode::_Iterator it = node->begin(); it != node->end(); ++it)
	{
		lua_pushlightuserdata(lua, *it);
		lua_rawseti(lua, new_table, index++);
	}

	return 1;
}

//
// LUA_XmlSave(node (object), filename (string)) - Saves the xml as a file
//
static int LUA_XmlSave(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 2)
	{
		CzScriptEngineLua::DisplayError(lua, "xml.save() not enough parameters, expected node (object), filename (string)");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the node
	CzXmlNode* node = NULL;
	if (lua_isuserdata(lua, 1))
		node = (CzXmlNode*)lua_touserdata(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.save() Invalid parameter, expected xml node object for Param0");
		lua_pushboolean(lua, false);
		return 1;
	}

	// Get the filename
	const char* filename = NULL;
	if (lua_isstring(lua, 2))
		filename = lua_tostring(lua, 2);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.save() Invalid parameter, expected string for Param1");
		lua_pushboolean(lua, false);
		return 1;
	}

	lua_pushboolean(lua, node->Save(filename) >= 0);

	return 1;
}

//
// LUA_XmlLoad(filename (string)) - Loads the specified xml file returning a parser object
//
static int LUA_XmlLoad(lua_State *lua)
{
	int count = lua_gettop(lua);
	if (count < 1)
	{
		CzScriptEngineLua::DisplayError(lua, "xml.load() not enough parameters, expected filename (string)");
		lua_pushnil(lua);
		return 1;
	}

	// Get the filename
	const char* filename = NULL;
	if (lua_isstring(lua, 1))
		filename = lua_tostring(lua, 1);
	else
	{
		CzScriptEngineLua::DisplayError(lua, "xml.load() Invalid parameter, expected string for Param0");
		lua_pushnil(lua);
		return 1;
	}

	CzXmlParser* parser = new CzXmlParser();
	if (parser->Parse(filename) != XmlErrorNone)
	{
		lua_pushnil(lua);
		delete parser;
	}
	else
		lua_pushlightuserdata(lua,  parser);

	return 1;
}



//
// Lua AppEasy Xml library
//
static const luaL_Reg g_xmllib[] = {
	{"createParser",		LUA_XmlCreateParser}, 
	{"destroyParser",		LUA_XmlDestroyParser}, 
	{"createNode",			LUA_XmlCreateNode}, 
	{"destroyNode",			LUA_XmlDestroyNode}, 
	{"addNode",				LUA_XmlAddNode}, 
	{"setNodeName",			LUA_XmlSetNodeName}, 
	{"getNodeName",			LUA_XmlGetNodeName}, 
	{"setNodeValue",		LUA_XmlSetNodeValue}, 
	{"getNodeValue",		LUA_XmlGetNodeValue}, 
	{"removeNode",			LUA_XmlRemoveNode}, 
	{"setAttribs",			LUA_XmlSetAttribs}, 
	{"getAttribs",			LUA_XmlGetAttribs}, 
	{"root",				LUA_XmlGetRoot}, 
	{"findFirstNamedNode",	LUA_XmlFindFirstNamedNode}, 
	{"children",			LUA_XmlGetChildren}, 
	{"save",				LUA_XmlSave}, 
	{"load",				LUA_XmlLoad}, 
	{NULL, NULL}
};

LUALIB_API int luaopen_xml(lua_State *L)
{
	luaL_register(L, "xml", g_xmllib);
	lua_pop(L, 1);

	return 1;
}


