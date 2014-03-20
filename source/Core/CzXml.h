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

// Warnings
// * Before parsing ensure that the XKL data pools have been set up by calling CzXmlParser::PoolsInit()
// * CzXmlParser::PoolsReset() to reset pool data and prevent running out of pool memory (do not call whilst parsing an XML file)


#if !defined(_CZ_XML_H_)
#define _CZ_XML_H_


#include "CzUtil.h"
#include "CzDataIO.h"

class CzXmlParser;
class CzFile;
struct CzXmlAttribute;
struct CzXmlNode;

#define CZ_INT_LIST_POOL_SIZE		256
#define CZ_FLOAT_LIST_POOL_SIZE		8192
#define CZ_BOOL_LIST_POOL_SIZE		256

/**
 @addtogroup Core
 @{
 */
 
//
//
//
// Type definitions provided for ease of code reading
//
//
//

/**
 @typedef	CzList<CzXmlAttribute*> CzXmlAttributeList

 @brief	A list of CzXmlAttribute's
 */
typedef CzList<CzXmlAttribute*> CzXmlAttributeList;

/**
 @typedef	CzList<CzXmlNode*> CzXmlNodeList

 @brief	A list of CzXmlNode's
 */
typedef CzList<CzXmlNode*> CzXmlNodeList;

/**
 @class	CzXmlTools

 @brief	General tools used by all CzXml classes.

 */
class CzXmlTools
{
public:
	static int			GetFirstWhitespaceIndex(char* pMem, int len);	//< Returns index of the first white space
	static int			IntListPool[CZ_INT_LIST_POOL_SIZE];				//< Int pool used by XML system
	static float		FloatListPool[CZ_FLOAT_LIST_POOL_SIZE];			//< Float pool used by XML system
	static bool			BoolListPool[CZ_BOOL_LIST_POOL_SIZE];			//< Bool pool used by XML system
};

/**
 @struct	CzXmlAttribute

 @brief	An Xml attribute, e.g. <Something attribute="some value" />

 */

struct CzXmlAttribute
{
public:
	bool				Managed;			///< Managec attributes are managed by the pools system and should not be deleted
	CzString			Name;				///< Attributes name
	CzString			Value;				///< Attributes value

public:
	CzXmlAttribute() : Managed(false) {}
	virtual ~CzXmlAttribute() {}

	void			Clear()					///< Clears the attributes name and value
	{
		Name = "";
		Value = "";
//		Name.reset();
//		Value.reset();
	}
	
	void	setName(const char* pName)		///< Sets the name of the attribute
	{
		Name.Copy(pName, 0, strlen(pName));
	}
	void	setName(char* pName, int len)	///< Sets the name of the attribute
	{
		Name.Copy(pName, 0, len);
	}
	CzString& getName() { return Name; }	///< Gets the name of the attribute
	void	setValue(const char* pValue)	///< Sets the value of the attribute
	{
		Value.Copy(pValue, 0, strlen(pValue));
		Value.ReplaceHTMLCodes();
	}
	void	setValue(char* pValue, int len)	///< Sets the value of the attribute
	{
		Value.Copy(pValue, 0, len);
		Value.ReplaceHTMLCodes();
	}
	
	// Single attribute element getters
	CzString&			getValue() { return Value; }		///< Gets the value of the attribute
	int					getValueAsInt() const;				///< Gets the value of the attribute as an integer
	float				getValueAsFloat() const;			///< Gets the value of the attribute as a floating point number
	bool				getValueAsBool() const;				///< Gets the value of the attribute as a boolean
	bool				getValueAsPoint(CzVec2 &point);		///< Gets the value of the attribute as a vec2
	bool				getValueAsPoint3(CzVec3 &point, float default_y = 0, float default_z = 0);	///< Gets the value of the attribute as a vec3 (if less than 3 values are found then rest will be assigned 0)
	bool				getValueAsPoint3Copy(CzVec3 &point);								///< Gets the value of the attribute as a vec3 (if less than 3 values are found then rest will be assigned the first value)
	bool				getValueAsPoint4(CzVec4 &point);	///< Gets the value of the attribute as a vec4
	bool				getValueAsColour(CzColour &colour);	///< Gets the value of the attribute as a colour
	bool				getValueAsRect(CzIRect &rect);		///< Gets the value of the attribute as a rect
	
	// List attribute element getters (generates a list from comma separated attribute data
	CzStringList*		getValueAsList();					///< Gets the value of the attribute as a list of strings
};

/**
 @struct	CzXmlNode

 @brief	An Xml node, e.g. <XmlNode></XmlNode>

 */

struct CzXmlNode
{
public:
	typedef CzXmlNodeList::iterator				_Iterator;
	typedef CzXmlAttributeList::iterator		_AttribIterator;
	_Iterator				begin()				{ return Children.begin(); }
	_Iterator				end()				{ return Children.end(); }
	_AttribIterator			attribs_begin()		{ return Attributes.begin(); }
	_AttribIterator			attribs_end()		{ return Attributes.end(); }

public:
	bool					Managed;			///< Managec nodes are managed by the pools system and should not be deleted
	bool					HasValue;			///< Determines if value is valid
	uint16					Line;				///< File line
	CzString				Name;				///< Node name
	CzString				Value;				///< Node value
	CzXmlAttributeList		Attributes;			///< List of attributes associated with this node
	CzXmlNode*				Parent;				///< Parent node
	CzXmlNodeList			Children;			///< List of children nodes
	CzXmlParser*			Parser;				///< Parser that created this node
	
public:
	CzXmlNode() : Parent(NULL), HasValue(false), Managed(false), Parser(NULL), Line(0) {}
	virtual ~CzXmlNode();
	void Clear()																				///< Clears the node
	{
		Name = "";
		Value = "";
		Attributes.clear();
		Parent = NULL;
		Children.clear();
		HasValue = false;
	}
	
	void AddChild(CzXmlNode* node)																///< Adds a child node to the tree
	{
		node->Parent = this;
		Children.push_back(node);
	}
	
	void RemoveChild(CzXmlNode* node)															///< Remove a child node from the tree (does not destroy it)
	{
		Children.remove(node);
	}
	
	void AddAttribute(CzXmlAttribute *attribute);												///< Adds an attribute to this node
	void AddAttribute(const CzString& name, const CzString& value, CzXmlParser* parser);		///< Adds an attribute to this node
	void AddAttribute(const CzString& name, const char* value, CzXmlParser* parser);			///< Adds an attribute to this node
	void UpdateAttribute(const CzString& name, const char* value, CzXmlParser* parser);			///< Updates an existing named attribute in this node

	void SetName(const char* name)																///< Sets the nodes name
	{
		Name.Copy((char*)name, 0, strlen(name));
	}
	void SetName(const char* name, int len)														///< Sets the nodes name
	{
		Name.Copy((char*)name, 0, len);
	}
	void SetValue(const char* value)															///< Sets the nodes value
	{
		Value.Copy((char*)value, 0, strlen(value));
		Value.ReplaceHTMLCodes();
		HasValue = true;
	}
	void SetValue(const char* value, int len)													///< Sets the nodes value
	{
		Value.Copy((char*)value, 0, len);
		Value.ReplaceHTMLCodes();
		HasValue = true;
	}
	const CzString&	GetName() const { return Name; }											///< Gets the nodes name
	
	// Single value getters (data between tags)
	const CzString&	getValue() const { return Value; }											///< Gets the nodes value
	int						getValueAsInt() const;												///< Gets the nodes value as an integer
	float					getValueAsFloat() const;											///< Gets the nodes value as a floating point number
	bool					getValueAsBool() const;												///< Gets the nodes value as a boolean

	CzXmlParser*			getParser()						{ return Parser; }					///< Returns the parser that created this node
	void					setParser(CzXmlParser* parser)	{ Parser = parser; }				///< Sets the parser that created this node
	
	// Print attributes / tree (DEBUG)
	void					PrintAttributes(CzString& out);										///< Print node attributes to a string
	void					PrintTree(CzString& out, int level = 0);							///< Print entire tree to a string

	CzXmlNode*				getFirstNode();														///< Get first child node
	CzXmlNode*				getFirstNamedNode(unsigned int name_hash);							///< Get first occurrence of a node in this node and all children
	void					getNamedNodes(unsigned int name_hash, CzXmlNodeList* nodes);		///< Get all occurrences of a node in this node and all children

	// Gets attribute count
	int						getAttributeCount() const { return Attributes.size(); }		///< Returns number of attributes that the node contains
	// Gets the named attribute
	CzXmlAttribute*			getAttribute(const char* name);								///< Returns named attribute
	CzXmlAttribute*			getAttribute(unsigned int hash);							///< Returns named attribute
	// Gets the Nth attribute
	CzXmlAttribute*			getAttributeAtInex(int index);								///< Returns attribute at the specified index

	// Save XML file
	int						SaveAttributes(CzFile* file);								///< Saves the attributes to a file
	int						SaveTree(CzFile* file, int level = 0);						///< Saves entire node tree to a file
	int						Save(const char* filename);									///< Saves entire node tree to a file with the specified filename
	const char*				getParserFileName() const;									///< Returns the name of the file that is being parsed

	// Tag / attribute case chnage (does not change the case of attribute values)
	void					ToLower(bool recursive);									///< Changes tags and attributes names to lower case, does not affect values
	void					ToUpper(bool recursive);									///< Changes tags and attributes names to upper case, does not affect values

	// Tree cloning
	CzXmlNode*				Clone(CzXmlNode* parent);									///< Clones this node
	CzXmlNode*				CloneTree(CzXmlNode* parent);								///< Clones the entire tree

};

/**
 @enum	eCzXmlParseError

 @brief	XML parser errors.
 */

enum eCzXmlParseError
{
	XmlErrorNone,
	XmlErrorNoTagFound,
	XmlErrorFileError,
	XmlErrorMissingEndTag,
	XmlErrorMissingEndComment,
	XmlErrorMismatchedEndTag, 
	XmlErrorMissingEquals, 
	XmlErrorMissingValue, 
	XmlErrorMissingClosingQuote, 
	XmlErrorInvalidTag, 
	XmlErrorInvalidComment, 
	XmlErrorPreParseError, 
	XmlErrorInvalidPools, 
};

/**
 @struct	CzXmlTagMarker

 @brief	XML tag marker.

 Tag markers are used to speed up XML parsing

 */

struct CzXmlTagMarker
{
	int		Start;
	int		Length;
	int		Line;
	
	void	Clear()
	{
		Start = 0;
		Length = 0;
		Line = 0;
	}
};

/**
 @class	CzXmlParser

 @brief	The Xml parser.

 <h1>Introduction</h1>

 AppEasy comes with a basic XML parser that has the following features:
 - Load and save XML files
 - Very small code footprint
 - Very quick parser
 - Uses memory pooling for tags, attributes and values, reducing memory fragmentation
 - Error output, including line numbers

 CzXmlParser does however have limitations such as no support for XML schemas and will allow you to do some things that normal XML parsers will not allow you to do.

 The XML engine is split into the following classes:
 - CzXmlAttribute - A nodes named attributes and values
 - CzXmlNode - A named node containing a collection of attributes
 - CzXmlParser - The main parser object that loads and parsers the XML file

 <h1>Loading an XML file</h1>

 To load an XML file, create an instance of CzXmlParser and call Parse() to parse the data, like shown below:

 @code
// Load the xml file
CzXmlParser*	xml = new CzXmlParser();
if (xml->Parse("./Scene1.xml") == eXMLParserError_None)
{
	// Save the xml file back out
	xml->getRoot()->Save("test1.xml");
}
 @endcode

 In this example we create an instance of the XML parser object then call Parse() passing in the name of the XML file. If there was no parsing errors then we save the file back out to 
 check that it worked.

 Three versions of Parse() are available:
 - Parse(const char* filename);
 - Parse(CzFile *file);
 - Parse(CzDataInput* data);

 These allowing parsing of a named file, an AppEasy file and a data input stream.

 <h1>Working with Nodes</h1>

 The parser provides a number of useful methods that you can use to get nodes from the parsed data:
 - getRoot() - Returns the root node of the loaded XML data
 - getFirstNamedNode() - Searches the complete XML node structure from the specified parent node and returns the first node who's name matches the supplied node name
 - gettNamedNodes() - Searches the complete XML node structure from the specified node and returns all nodes who's names match the supplied node name

 Once you have a node you can begin querying the nodes value and attributes. To get the nodes value you can use the following methods:
 - getValue() - Returns the nodes value as a string
 - getValueAsInt() - Returns the nodes value as an integer
 - getValueAsFloat() - Returns the nodes value as a floating point number
 - getValueAsBool() - Returns the nodes value as a boolean

 For debug purposes you can also print the nodes attributes and complete hierarchy using the following methods:
 - PrintAttributes() - Prints the nodes attributes to a string
 - PrintTree() - Prints the entier XML node structure to a string

 You can search a nodes child nodes using the following methods:
 - getFirstNode() - Returns the first node
 - getFirstNamedNode(unsigned int name_hash) - Returns the first named mode
 - getNamedNodes(unsigned int name_hash, CzXmlNodeList *nodes) - Returns all nodes that match the supplied name

 Note that these methods take a hashed string value as node names instead of a string for faster searching.

 CzXmlNode also provides methods for saving its structure to a file:
 - SaveAttributes(CzFile* file) - Saves a group of mode attributes to a file
 - SaveTree(CzFile* file) - Saves a complete mode tree to a file
 - Save(const char* filename) -  Saves a complete mode tree to a file with specified filename

 Querying attributes can be done using the following methods:
 - GetAttributeCount() - Returns number of attributes that the node contains
 - GetAttribute(const char* name) - Gets the named attribute
 - GetAttribute(int index) - Gets the attribute at the specified index

 And finally methods have been provided for building nodes:
 - SetName(const char* name, int len) - Sets the nodes name
 - SetValue(const char* value, int len) - Sets the nodes value
 - AddChild(CzXmlNode* node) - Adds a new node as a child
 - AddAttribute(CzXmlAttribute *attribute) - Adds a new attribute to the node
 - AddAttribute(CzString& name, CzString& value) - Adds a new attribute to the node
 - AddAttribute(CzString& name, const char* value) - Adds a new attribute to the node
 - UpdateAttribute(const CzString& name, const char* value, CzXmlParser* parser) - Updates an existing named attribute in the node

 <h1>Node and Attribute Iteration</h1>
 
 CzXmlNode provides iterator based access to both child nodes and attributes:

 @code
typedef CIwList<CzXmlNode*>::iterator		_Iterator;
typedef CIwList<CzXmlAttribute*>::iterator		_AttribIterator;
_Iterator				begin()
_Iterator				end()
_AttribIterator			attribs_begin()
_AttribIterator			attribs_end()	
 @endcode

 These types and methods allow you to easily walk the nodes child nodes and attributes. Below is an example showing how to walk a nodes child nodes and each nodes attributes:

 @code
// Walk the child nodes
for (CzXmlNode::_Iterator nodes = node->begin(); nodes != node->end(); ++nodes)
{
	// Walk the nodes attrobutes
	for (CzXmlNode::_AttribIterator attribs = (*nodes)->attribs_begin(); attribs != (*nodes)->attribs_end(); ++attribs)
	{
	}
}
 @endcode

 <h1>Attribute Query</h1>

 CzXmlAttribute provides an extensive set of methods for querying attribute values and converting the data to different formats. Below is a list of all methods:

 @code
CzString&		getValue() { return Value; }
int				getValueAsInt() const;
float			getValueAsFloat() const;
bool			getValueAsBool() const;
bool			getValueAsPoint(CIwFVec2 &point);
bool			getValueAsPoint3(CIwFVec3 &point);
bool			getValueAsPoint4(CIwFVec4 &point);
bool			getValueAsColour(CIwColour &colour);
bool			getValueAsRect(CIwRect &rect);

CzXmlStringList*	getValueAsList();
int				getValueAsListOfInt();
int				getValueAsListOfFloat();
int				getValueAsListOfBool();
@endcode

 The list value retrieval methods uses a pooled memory system to reduce constant allocation and deallocation of memory, so please ensure that you store off the values 
 retrieved from the pool buffers before calling the list methods again or data will be written over. To see how the list pool buffers are used lets take a quick look 
 at at GetValueAsRect():

 @code
bool CzXmlAttribute::getValueAsRect(CzIRect& rect)
{
	if (Value.getAsListOfInt(CzXmlTools::IntListPool) != 4)
	{
		return false;
	}
	rect.x = CzXmlTools::IntListPool[0];
	rect.y = CzXmlTools::IntListPool[1];
	rect.w = CzXmlTools::IntListPool[2];
	rect.h = CzXmlTools::IntListPool[3];

	return true;
}
 @endcode

 As you can see, when we call GetAsListOfInt() a global buffer called CzXmlTools::IntListPool is filled with the values that are returned.

 <h1>Creating an XML file</h1>

 XML is very useful when it comes to representing data in a platform independent manner. It's also very useful when it comes to serialising game state and other data to 
 storage as it can be saved in a good structured format.

 To create an XML file you need to create a root XML node then add further named child nodes that contain values and attributes that contain your data. Below shows a 
 quick example:

 @code
// Create root XML node
CzXmlNode* root = new CzXmlNode();
root->SetName("xml");

// Create and add a settings child node to the root
CzXmlNode* settings_node = new CzXmlNode();
settings_node->SetName("Settings");
root->AddChild(settings_node);

// Create and add a FullVersion node to the settings node
CzXmlNode*	value_node = new CzXmlNode();
value_node->SetName("FullVersion");
value_node->SetValue("true");
settings_node->AddChild(value_node);

// Save the XML file
settings_node->Save("./Settings.xml");

// Cleanup xml data
delete root;
 @endcode

The above code will generate the following XML file:

 @code
<?xml version="1.0"?>
<Settings>
	<FullVersion>true</FullVersion>
</Settings>
 @endcode

 */

class CzXmlParser
{
public:

private:
	CzString			Filename;					///< Name of file being parsed
	CzString			m_Encoding;					///< Encoding type
	CzString			m_Version;					///< XML version
	CzDataInput*		m_pDataInput;				///< Input stream
	CzXmlNode*			m_pRoot;					///< Root node
	CzXmlTagMarker*		Marker;						///< Tag markers buffer
	int					MarkerCount;				///< Total tag markers in the buffer

	CzXmlNodeList::iterator		RemoveNodeFromList(CzXmlNodeList& list, const CzXmlNodeList::iterator& it);
public:
	CzXmlParser() : m_pDataInput(NULL), m_pRoot(NULL)
	{
		TagPool = NULL;
		NextFreePoolTagIndex = 0;
		MaxPoolTags = 0;
		NodePool = NULL;
		NextFreePoolNodeIndex = 0;
		MaxPoolNodes = 0;
		AttributePool = NULL;
		NextFreePoolAttributeIndex = 0;
		MaxPoolAttributes = 0;
//		m_Encoding.setString("utf-8");
//		m_Version.setString("1.0");
	}
	virtual ~CzXmlParser()
	{
//		if (m_pRoot != NULL)
//			delete m_pRoot;
		SAFE_DELETE_ARRAY(TagPool);
		SAFE_DELETE_ARRAY(NodePool);
		SAFE_DELETE_ARRAY(AttributePool);
		SAFE_DELETE(m_pDataInput);
	}

	void			setEncoding(const char* enocding) { m_Encoding.setString(enocding); }
	const CzString&	getEncoding() const { return m_Encoding; }
	void			setVersion(const char* version) { m_Version.setString(version); }
	const CzString&	getVersion() const { return m_Version; }
	const CzString&	getFilename() const { return Filename; }

	CzXmlNode*		AllocNode()			///< Allocates space for a new node
	{
		if (NodePool == NULL)
			return new CzXmlNode();
		if (NextFreePoolNodeIndex >= MaxPoolNodes)
		{
			return new CzXmlNode();
//			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Ran out of nodes, allocate more");
//			return NULL;
		}
		CzXmlNode* node = NodePool + NextFreePoolNodeIndex++;
		node->Clear();
		node->setParser(this);
		node->Managed = true;

		return node;
	}
	CzXmlTagMarker*	AllocTag()			///< Allocates space for a new tag marker
	{
		if (TagPool == NULL)
			return new CzXmlTagMarker();
		if (NextFreePoolTagIndex >= MaxPoolTags)
		{
			return new CzXmlTagMarker();
//			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Ran out of tag markers, allocate more");
//			return NULL;
		}
		CzXmlTagMarker* tag = TagPool + NextFreePoolTagIndex++;
		tag->Clear();

		return tag;
	}
	
	CzXmlAttribute*	AllocAttribute()		///< Allocates space for a new attribute
	{
		if (AttributePool == NULL)
			return new CzXmlAttribute();
		if (NextFreePoolAttributeIndex >= MaxPoolAttributes)
		{
			return new CzXmlAttribute();
//			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Ran out of attributes, allocate more");
//			return NULL;
		}
		CzXmlAttribute* attribute = AttributePool + NextFreePoolAttributeIndex++;
		attribute->Clear();
		attribute->Managed = true;

		return attribute;
	}

	// Parse the supplied file
	eCzXmlParseError		Parse(const char* pFilename, bool decrypt = false);	///< Parses the supplied file
	eCzXmlParseError		Parse(CzFile *file, bool decrypt = false);			///< Parses the supplied file
	eCzXmlParseError		Parse(CzDataInput* pData);							///< Parses the supplied memory buffer
	int						Save(const char* filename);				///< Saves entire node tree to a file with the specified filename
	const char*				getErrorString(eCzXmlParseError error) const;	///< Returns the string repesentation of the supplied error
	CzXmlNode*				getRoot()								///< Get root node (note that root node is '.' and not the first XML tag)
	{
		return m_pRoot;
	}
	CzXmlNode*				getFirstNamedNode(CzXmlNode *parent, const char* node_name)		///< Get first occurrence of a node in this node and all children
	{
		if (parent == NULL)
			parent = getRoot();

		int hash = CzString::CalculateHash(node_name);
		if (parent != NULL)
			return parent->getFirstNamedNode(hash);
		if (m_pRoot != NULL)
			return m_pRoot->getFirstNamedNode(hash);
			
		return NULL;
	}
	CzXmlNodeList*			getNamedNodes(CzXmlNode *parent, const char* node_name)///</ Get all occurrences of a node in this node and all children (caller is responsible for cleaning up list). Pass NULL as bode_name to return all nodes
	{
		if (parent == NULL)
			parent = getRoot();

		int				hash = CzString::CalculateHash(node_name);
		CzXmlNodeList*	pNodes = new CzXmlNodeList;
		if (parent != NULL)
			parent->getNamedNodes(hash, pNodes);
		else
		if (m_pRoot != NULL)
			m_pRoot->getNamedNodes(hash, pNodes);
			
		return pNodes;
	}

private:
	eCzXmlParseError	ParseXMLHeader();
	eCzXmlParseError	ParseAttributes(const char* data, int count, CzXmlNode* node);
	bool				PreParse();
	eCzXmlParseError	Parse(CzXmlNode* parent);

	eCzXmlParseError	ParseAttributes(CzXmlNode* node);
	eCzXmlParseError	Parse(CzXmlNode* pParent, int range, int &num_tags_found);
	const char*			getCloseTagName(CzXmlNode *node) const;
	void				ShowError(eCzXmlParseError error, int pos) const;

	int					getNextWord(const char* data, int &offset);
	int					getNextQuotedWord(const char* data, int &offset);

//	friend CzXmlNode;
//	friend CzXmlAttribute;

	// Tag and node pooling
	CzXmlTagMarker*		TagPool;					///< Used to separate tags from the raw XML file
	int					NextFreePoolTagIndex;		///< Next free pool tag index
	int					MaxPoolTags;				///< Maxmimum available pooled tags
	CzXmlNode*			NodePool;					///< Xml nodes pool
	int					NextFreePoolNodeIndex;		///< Bext free node index in nodes pool
	int					MaxPoolNodes;				///< Maximum available nodes in the nodes pool
	CzXmlAttribute*		AttributePool;				///< Xml attribute pool
	int					NextFreePoolAttributeIndex;	///< Bext free attribute index in nodes pool
	int					MaxPoolAttributes;			///< Maximum available attributes in the attributes pool
public:

};

/// @}

#endif	// _CZ_XML_H_
