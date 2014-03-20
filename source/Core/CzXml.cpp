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

#include "CzXml.h"
#include "CzString.h" 
#include "CzDataIO.h"
#include "CzFile.h"
#include "CzDecrypt.h"

#define SHOW_ERRORS	1

const char* CzXmlParserErrors[] = 
{
	"",
	"No XML tags found", 
	"XML file could not be read", 
	"Missing end tag", 
	"Missing close comment '-->'", 
	"Mismatched end tag", 
	"Missing '=' sign in attribute definition", 
	"Missing value in attribute definition", 
	"Missing closing quote in attribute value definition", 
	"Invalid tag", 
	"Invalid comment", 
	"Pre-parser error, ran out of space, increase size of parser pools", 
	"Invalid parser pools", 
};

// Pooled lists to reduce memory fragmentation and loads of allocations / frees
int			CzXmlTools::IntListPool[CZ_INT_LIST_POOL_SIZE];
float		CzXmlTools::FloatListPool[CZ_FLOAT_LIST_POOL_SIZE];
bool		CzXmlTools::BoolListPool[CZ_BOOL_LIST_POOL_SIZE];

//
//
//
//
// CzXmlAttribute implementation
//
//
//
//
int CzXmlAttribute::getValueAsInt() const
{
	return Value.getAsInt();
}

float CzXmlAttribute::getValueAsFloat() const
{
	return Value.getAsFloat();
}

bool CzXmlAttribute::getValueAsBool() const
{
	return Value.getAsBool();
}

bool CzXmlAttribute::getValueAsPoint(CzVec2& point)
{
	if (Value.getAsListOfFloat(CzXmlTools::FloatListPool) < 2)
		return false;

	point.x = CzXmlTools::FloatListPool[0];
	point.y = CzXmlTools::FloatListPool[1];

	return true;
}

bool CzXmlAttribute::getValueAsPoint3(CzVec3& point, float default_y, float default_z)
	{
	int count = Value.getAsListOfFloat(CzXmlTools::FloatListPool);
	if (count < 1)
		return false;

	point.x = CzXmlTools::FloatListPool[0];
	if (count > 1)
	point.y = CzXmlTools::FloatListPool[1];
	else
		point.y = default_y;

	if (count > 2)
	point.z = CzXmlTools::FloatListPool[2];
	else
		point.z = default_z;

	return true;
}

bool CzXmlAttribute::getValueAsPoint3Copy(CzVec3& point)
	{
	int count = Value.getAsListOfFloat(CzXmlTools::FloatListPool);
	if (count < 1)
		return false;

	point.x = CzXmlTools::FloatListPool[0];
	if (count > 1)
		point.y = CzXmlTools::FloatListPool[1];
	else
		point.y = point.x;

	if (count > 2)
		point.z = CzXmlTools::FloatListPool[2];
	else
		point.z = point.x;

	return true;
}

bool CzXmlAttribute::getValueAsPoint4(CzVec4& point)
{
	if (Value.getAsListOfFloat(CzXmlTools::FloatListPool) < 4)
		return false;

	point.x = CzXmlTools::FloatListPool[0];
	point.y = CzXmlTools::FloatListPool[1];
	point.z = CzXmlTools::FloatListPool[2];
	point.w = CzXmlTools::FloatListPool[3];

	return true;
}

bool CzXmlAttribute::getValueAsColour(CzColour& colour)
{
	if (Value.getAsListOfInt(CzXmlTools::IntListPool) < 4)
		return false;

	colour.r = CzXmlTools::IntListPool[0];
	colour.g = CzXmlTools::IntListPool[1];
	colour.b = CzXmlTools::IntListPool[2];
	colour.a = CzXmlTools::IntListPool[3];

	return true;
}

bool CzXmlAttribute::getValueAsRect(CzIRect& rect)
{
	if (Value.getAsListOfInt(CzXmlTools::IntListPool) < 4)
		return false;

	rect.x = CzXmlTools::IntListPool[0];
	rect.y = CzXmlTools::IntListPool[1];
	rect.w = CzXmlTools::IntListPool[2];
	rect.h = CzXmlTools::IntListPool[3];

	return true;
}

CzStringList* CzXmlAttribute::getValueAsList()
{
	CzStringList* pList = new CzStringList;
	
	// Separate Value by commas
	int			len = Value.getLength();
	const char* text = Value.c_str();
	char		c;
	
	while ((c = *text) != 0)
	{
		// Find a none white space
		if (c != ' ' && c != '\t' && c != '\n' && c != ',')
		{
			int count = 0;
			const char* found = text;
			
			// Find end of string
			while (count++ < 63)
			{
				c = *text;
				if (c == '\n' || c == ',' || c == 0)
					break;
				text++;
			}
			int len = text - found;
			if (len > 0)
			{
				CzString *pString = new CzString();
				pString->Copy((char*)found, 0, len);
			
				pList->push_back(pString);
			}
			if (c == 0) break;
		}
		text++;
	}

	return pList;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
// CzXmlNode implementation
//
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char*	CzXmlNode::getParserFileName() const
{
	if (Parser == NULL)
		return NULL;

	return Parser->getFilename().c_str();
}



CzXmlNode::~CzXmlNode()
{
//	if (!Managed)
	{
		for (CzXmlAttributeList::iterator it = Attributes.begin(); it != Attributes.end(); ++it)
		{
			if (!(*it)->Managed)
				delete *it;
		}
		for (CzXmlNodeList::iterator it = Children.begin(); it != Children.end(); ++it)
		{
			if (!(*it)->Managed)
				delete *it;
		}
	}
	Attributes.clear();
	Children.clear();
}

void CzXmlNode::AddAttribute(CzXmlAttribute *attribute)
{
	CzXmlAttribute* old_attribute = getAttribute(attribute->getName().c_str());

	if (old_attribute == NULL)
	{
		// Attribute was not present so add
		Attributes.push_back(attribute);
	}
/*	else
	{
		// Attribute was present so update it
		old_attribute->setName((char*)attribute->getName().c_str(), attribute->getName().getLength());
		old_attribute->setValue((char*)attribute->getValue().c_str(), attribute->getValue().getLength());
	}*/
}

void CzXmlNode::AddAttribute(const CzString& name, const CzString& value, CzXmlParser* parser)
{
	CzXmlAttribute* old_attribute = getAttribute(name.c_str());

	if (old_attribute == NULL)
	{
		// Attribute was not present so add
		CzXmlAttribute* attribute;
		if (parser != NULL)
			attribute = parser->AllocAttribute();
		else
			attribute = new CzXmlAttribute();
		attribute->setName((char*)name.c_str(), name.getLength());
		attribute->setValue((char*)value.c_str(), value.getLength());
		Attributes.push_back(attribute);
	}
/*	else
	{
		// Attribute was present so update it
		old_attribute->setName((char*)name.c_str(), name.getLength());
		old_attribute->setValue((char*)value.c_str(), value.getLength());
	}*/
}

void CzXmlNode::AddAttribute(const CzString& name, const char* value, CzXmlParser* parser)
{
	CzXmlAttribute* old_attribute = getAttribute(name.c_str());

	if (old_attribute == NULL)
	{
		// Attribute was not present so add
		CzXmlAttribute* attribute;
		if (parser != NULL)
			attribute = parser->AllocAttribute();
		else
			attribute = new CzXmlAttribute();
		attribute->setName((char*)name.c_str(), name.getLength());
		attribute->setValue((char*)value, strlen(value));
		Attributes.push_back(attribute);
	}
/*	else
	{
		// Attribute was present so update it
		old_attribute->setName((char*)name.c_str(), name.getLength());
		old_attribute->setValue((char*)value, strlen(value));
	}*/
}

void CzXmlNode::UpdateAttribute(const CzString& name, const char* value, CzXmlParser* parser)
{
	CzXmlAttribute* old_attribute = getAttribute(name.c_str());

	if (old_attribute == NULL)
	{
		// Attribute was not present so add
		CzXmlAttribute* attribute;
		if (parser != NULL)
			attribute = parser->AllocAttribute();
		else
			attribute = new CzXmlAttribute();
		attribute->setName((char*)name.c_str(), name.getLength());
		attribute->setValue((char*)value, strlen(value));
		Attributes.push_back(attribute);
	}
	else
	{
		// Attribute was present so update it
		old_attribute->setName((char*)name.c_str(), name.getLength());
		old_attribute->setValue((char*)value, strlen(value));
	}
}

int CzXmlNode::getValueAsInt() const
{
	return Value.getAsInt();
}

float CzXmlNode::getValueAsFloat() const
{
	return Value.getAsFloat();
}

bool CzXmlNode::getValueAsBool() const
{
	return Value.getAsBool();
}

void CzXmlNode::PrintAttributes(CzString& out)
{
	for (CzXmlAttributeList::iterator i = Attributes.begin(); i != Attributes.end(); ++i)
	{
		CzXmlAttribute* attribute = *i;
		out += attribute->Name.c_str();
		out += "=\"";
		out += attribute->Value.c_str();
		out += "\" ";
	}

}

void CzXmlNode::PrintTree(CzString& out, int level)
{
	for (int t = 0; t < level; t++)
	{
		out += "\t";
	}
	out += "<";
	out += Name.c_str();
	PrintAttributes(out);
	if (HasValue)
	{
		if (Value.getLength() == 0)
			out += " />\n";
		else
		{
			out += ">";
			out += Value.c_str();
		}
	}
	else
	{
		out += ">\n";
		CzXmlNode* node;
		for (CzXmlNodeList::iterator it = Children.begin(); it != Children.end(); it++)
		{
			node = *it;
			node->PrintTree(out, level + 1);
		}
		for (int t = 0; t < level; t++)
			out += "\t";
	}
//	if (Value.getLength() > 0 || Value.c_str() == NULL)
	if (Value.getLength() > 0 || !HasValue)
	{
		out += "</";
		out += Name.c_str();
		out += ">\n";
	}
}

void CzXmlNode::ToLower(bool recursive)
{
	Name.ToLower();
	for (CzXmlAttributeList::iterator i = Attributes.begin(); i != Attributes.end(); ++i)
	{
		(*i)->getName().ToLower();
	}
	if (recursive)
	{
		for (CzXmlNodeList::iterator i2 = Children.begin(); i2 != Children.end(); ++i2)
		{
			(*i2)->ToLower(recursive);
		}
	}
}

void CzXmlNode::ToUpper(bool recursive)
{
	Name.ToUpper();
	for (CzXmlAttributeList::iterator i = Attributes.begin(); i != Attributes.end(); ++i)
	{
		(*i)->getName().ToUpper();
	}
	if (recursive)
	{
		for (CzXmlNodeList::iterator i2 = Children.begin(); i2 != Children.end(); ++i2)
		{
			(*i2)->ToUpper(recursive);
		}
	}
}

int CzXmlNode::SaveAttributes(CzFile* file)
{
	for (CzXmlAttributeList::iterator i = Attributes.begin(); i != Attributes.end(); ++i)
	{
		CzString out;
		out.allocString(512);
		out = " ";
		out += (*i)->Name.c_str();
		out += "=\"";
		out += (*i)->Value.c_str();
		out += "\"";
		if (!file->Write((void*)out.c_str(), out.getLength()))
			return -1;
	}

	return 0;
}

int CzXmlNode::SaveTree(CzFile* file, int level)
{
	int err = 0;

	CzString out;
	out.allocString(256);

	for (int t = 0; t < level; t++)
		out += "\t";
	out += "<";
	out += Name;
	if (!file->Write((void*)out.c_str(), out.getLength()))
		return -1;

	err = SaveAttributes(file);
	if (err < 0)
		return err;
	if (HasValue)
	{
		if (Value.getLength() == 0)
			out = " />\n";
		else
		{
			out = ">";
			out += Value.c_str();
		}
		out += "</";
		out += Name.c_str();
		out += ">\n";
		if (!file->Write((void*)out.c_str(), out.getLength()))
			return -1;
	}
	else
	{
		if (Children.size() == 0)
		{
			out = " />\n";
			if (!file->Write((void*)out.c_str(), out.getLength()))
				return -1;
		}
		else
		{
			out = ">\n";
			if (!file->Write((void*)out.c_str(), out.getLength()))
				return -1;
			for (CzXmlNodeList::iterator i = Children.begin(); i != Children.end(); ++i)
			{
				err = (*i)->SaveTree(file, level + 1);
				if (err < 0)
					return err;
			}
			for (int t = 0; t < level; t++)
			{
				out = "\t";
				if (!file->Write((void*)out.c_str(), out.getLength()))
					return -1;
			}
			out = "</";
			out += Name.c_str();
			out += ">\n";
			if (!file->Write((void*)out.c_str(), out.getLength()))
				return -1;
		}
	}

	return 0;
}

CzXmlNode* CzXmlNode::Clone(CzXmlNode* parent)
{
	// Clone node
	CzXmlNode* node = new CzXmlNode();
	node->Managed = false;
	node->Name = Name;
	node->Value = Value;
	node->HasValue = HasValue;
	node->Parent = parent;
	node->Parser = NULL;
	node->Line = Line;

	// Clone nodes attributes
	for (CzXmlAttributeList::iterator it = Attributes.begin(); it != Attributes.end(); ++it)
	{
		CzXmlAttribute* attrib = new CzXmlAttribute();
		attrib->Managed = false;
		attrib->setName((*it)->getName().c_str());
		attrib->setValue((*it)->getValue().c_str());
		node->AddAttribute(attrib);
	}

	return node;
}

CzXmlNode* CzXmlNode::CloneTree(CzXmlNode* parent)
{
	// Clone node
	CzXmlNode* node = Clone(parent);

	// Clone children
	for (CzXmlNodeList::iterator it = Children.begin(); it != Children.end(); ++it)
	{
		node->AddChild((*it)->CloneTree(this));
	}

	return node;
}

int CzXmlNode::Save(const char* filename)
{
	// Dont save root node
	if (Name == ".")
	{
		CzXmlNode* node = getFirstNode();
		if (node == NULL)
			return -1;
		return node->Save(filename);
	}

	CzFile file;
	if (file.Open(filename, "wb", true))
	{
		const char* xml_head = "<?xml version=\"1.0\"?>\n";
		if (!file.Write((void*)xml_head, strlen(xml_head)))
			return -1;
		int err = SaveTree(&file);
		if (err < 0)
			return err;
	}
	else
		return -1;

	return 0;
}

CzXmlNode* CzXmlNode::getFirstNode()
{
	if (Children.size() == 0)
		return NULL;
	
	return *Children.begin();
}

CzXmlNode* CzXmlNode::getFirstNamedNode(unsigned int name_hash)
{
	// Check this mode
	if (Name.getHash() == name_hash)
		return this;
	
	// Check child nodes
	for (CzXmlNodeList::iterator i = Children.begin(); i != Children.end(); ++i)
	{
		CzXmlNode* pNode = *i;
		CzXmlNode* pFound =  pNode->getFirstNamedNode(name_hash);
		if (pFound != NULL)
			return pFound;
	}
	
	return NULL;
}

void CzXmlNode::getNamedNodes(unsigned int name_hash, CzXmlNodeList *nodes)
{
	// Check this mode
	if (Name.getHash() == name_hash)
		nodes->push_back(this);
		
	// Check child nodes
	for (CzXmlNodeList::iterator i = Children.begin(); i != Children.end(); ++i)
		(*i)->getNamedNodes(name_hash, nodes);
}

CzXmlAttribute*	CzXmlNode::getAttribute(const char* name)
{
	unsigned int hash = CzString::CalculateHash(name);
	
	for (CzXmlAttributeList::iterator i = Attributes.begin(); i != Attributes.end(); ++i)
	{
		if ((*i)->Name.getHash() == hash)
			return *i;
	}
	
	return NULL;
}

CzXmlAttribute*	CzXmlNode::getAttribute(unsigned int hash)
{
	for (CzXmlAttributeList::iterator i = Attributes.begin(); i != Attributes.end(); ++i)
	{
		if ((*i)->Name.getHash() == hash)
			return *i;
	}
	
	return NULL;
}

CzXmlAttribute*	CzXmlNode::getAttributeAtInex(int index)
{
	if (index < 0 || index >= (int)Attributes.size())
		return NULL;
	
	CzXmlAttributeList::iterator i = Attributes.begin();
	while (index-- > 0)
		++i;
		
	return *i;
}

//
//
//
//
// CXMLParser implementation
//
//
//
//
const char* CzXmlParser::getErrorString(eCzXmlParseError error) const
{
	return CzXmlParserErrors[error];
}

const char* CzXmlParser::getCloseTagName(CzXmlNode *node) const
{
	int len = node->Name.getLength();
	char* pCloseTag = new char [len + 3];
	pCloseTag[0] = '<';
	pCloseTag[1] = '/';
	pCloseTag[len + 3] = '>';
	
	for (int t = 0; t < len; t++)
		pCloseTag[t + 2] = node->Name[t];
		
	return pCloseTag;
}

void CzXmlParser::ShowError(eCzXmlParseError error, int pos) const
{
#ifdef SHOW_ERRORS
	CzString out;
	out += getErrorString(error);
	out += " around line ";
//	out += CzString(m_pDataInput->GetLineNumber(pos));
	out += CzString(pos + 1);
	CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, out.c_str());
#endif
}

eCzXmlParseError	CzXmlParser::ParseAttributes(CzXmlNode* node)
{
	while (1)
	{
		char* pBuffer = (char *)m_pDataInput->getData();
		
		// Skip tag name
		if (m_pDataInput->SkipToWhitespaceInTag() < 0)
			return XmlErrorNone;
			
		// Skip to first attribute name
		int name_pos = m_pDataInput->SkipToNoneWhitespaceInTag();
		if (name_pos < 0)
			return XmlErrorNone;
		
		// Find length of name
		int name_end_pos = m_pDataInput->SkipToWhitespaceInTag();
		if (name_end_pos < 0)
			return XmlErrorNone;
		
		// Set attribute name
		CzXmlAttribute* attrib = AllocAttribute();
		node->AddAttribute(attrib);
		attrib->setName(pBuffer + name_pos, name_end_pos - name_pos);
		
		// Find equal sign
		int equal_pos = m_pDataInput->SkipToCharInTag('=');
		if (equal_pos < 0)
		{
			ShowError(XmlErrorMissingEquals, m_pDataInput->getPos());
			return XmlErrorMissingEquals;
		}
			
		// Find open quote position
		int quote_start = m_pDataInput->SkipToCharInTag('"');
		if (quote_start < 0)
		{
			ShowError(XmlErrorMissingValue, m_pDataInput->getPos());
			return XmlErrorMissingValue;
		}
		m_pDataInput->Skip(1);
		
		// Get closing quote position
		int quote_end = m_pDataInput->SkipToCharInTag('"');
		if (quote_end < 0)
		{
			ShowError(XmlErrorMissingClosingQuote, m_pDataInput->getPos());
			return XmlErrorMissingClosingQuote;
		}
			
		attrib->setValue(pBuffer + quote_start + 1, quote_end - quote_start - 1);
	}
	
	return XmlErrorNone;
}

eCzXmlParseError CzXmlParser::Parse(CzFile *file, bool decrypt)
{
	if (file->Open(NULL, "rb", true))
	{
		Filename = file->getFilename();
		int size = file->getFileSize();
		m_pDataInput = new CzDataInput();
		m_pDataInput->Init(size);
		if (!file->Read(m_pDataInput->getData(), size))
			return XmlErrorFileError;

		// Decrypt encrypted code
		if (decrypt)
			CzDecrypt::Decrypt((unsigned char*)m_pDataInput->getData(), size, (unsigned char*)CzDecrypt::DecryptKey, 128);
	}
	else
		return XmlErrorFileError;

	m_pDataInput->StripXMLComments();

	return Parse(m_pDataInput);
}

eCzXmlParseError CzXmlParser::Parse(const char* filename, bool decrypt)
{
	CzFile file(filename);

	eCzXmlParseError err = Parse(&file, decrypt);

	return err;
}

int CzXmlParser::Save(const char* filename)
{
	if (m_pRoot != NULL)
		return m_pRoot->Save(filename);

	return 0;
}


int CzXmlParser::getNextWord(const char* data, int &offset)
{
	char c;
	int index = 0;
	offset = -1;

	while ((c = *data++) != 0)
	{
		if (offset < 0)
		{
			// Find none whitespace
			if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
				offset = index;
		}
		else
		{
			// Find whitespace or xml tag brace
			if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '>' || c == '=')
				return index - offset;
		}

		index++;
	}

	return -1;
}

int CzXmlParser::getNextQuotedWord(const char* data, int &offset)
{
	char c;
	char prev_c = 0;
	int index = 0;
	offset = -1;

	while ((c = *data++) != 0)
	{
		if (offset < 0)
		{
			// Find none whitespace
			if (c == '"')
				offset = index + 1;
		}
		else
		{
			// Find whitespace
			if (c == '>' || c == '<')
				return -1;
			if (c == '"' && prev_c != '\\')
				return index - offset;
		}

		index++;
		prev_c = c;
	}

	return -1;
}

eCzXmlParseError	CzXmlParser::ParseAttributes(const char* data, int count, CzXmlNode* node)
{
	while (count > 0)
	{
		// Get attribute name
		int offset;
		int len = getNextWord(data, offset);
		char c = *(data + offset);
		if (len <= 0 || c == '/' || c == '>')
			return XmlErrorNone;

		CzXmlAttribute* attrib = AllocAttribute();
//memcpy(str, data + offset, len);
//str[len] = 0;
//sprintf(str2, "Attrib name %s, ", str);
//CzDebug::Log(str2);
		attrib->setName((char*)data + offset, len);
		node->AddAttribute(attrib);
		data += len + 1 + offset;
		count -= len + 1 + offset;
		if (count < 0)
			return XmlErrorInvalidTag;

		// Get attribute value
		len = getNextQuotedWord(data, offset);
		if (len < 0)
			return XmlErrorInvalidTag;
//memcpy(str, data + offset, len);
//str[len] = 0;
//sprintf(str2, "Attrib value %s\n", str);
//CzDebug::Log(str2);
		attrib->setValue((char*)data + offset, len);
		data += len + 2 + offset;
		count -= len + 2 + offset;
		if (count < 0)
			return XmlErrorMissingClosingQuote;
	}

	return XmlErrorNone;
}

bool CzXmlParser::PreParse()
{
	// Count number of tags and attriibutes
	int num_tags, num_attribs;
	m_pDataInput->CountXmlEntities(num_tags, num_attribs);
	num_tags++;	// Add one on for root

	// Allocate buffers
	TagPool = new CzXmlTagMarker[num_tags]();
	NodePool = new CzXmlNode[num_tags]();
	AttributePool = new CzXmlAttribute[num_attribs]();
	MaxPoolTags = num_tags;
	MaxPoolNodes = num_tags;
	MaxPoolAttributes = num_attribs;
	NextFreePoolNodeIndex = 0;
	NextFreePoolTagIndex = 0;
	NextFreePoolAttributeIndex = 0;

	Marker = TagPool + NextFreePoolTagIndex;
	int line_num = 0;
	while (1)
	{
		if (m_pDataInput->isEOF())
			break;

		int tag_pos = 0;
		int pos = m_pDataInput->getPos();
		if (pos >= m_pDataInput->getLength())
			return true;

		int tag_len = m_pDataInput->getNextTagOrValue('<', '>', m_pDataInput->Remaining(), tag_pos);
		if (tag_len < 0)
		{
			if (tag_len == -1)
			{
				ShowError(XmlErrorMissingEndTag, line_num);
				return false;
			}
			else
				return true;
		}
		int pos2 = m_pDataInput->getPos() - 1;

		// Calculate line number
		if (pos2 > pos)
		{
			int d = pos2 - pos;
			char* data = m_pDataInput->getData() + pos;
			for (int t = 0; t < pos2 - pos; t++)
			{
				if (*data++ == '\n')
					line_num++;
			}
		}

		if (tag_len > 0)
		{
			CzXmlTagMarker* tag_marker = AllocTag();
			if (tag_marker == NULL)
			{
				return false;
			}
			tag_marker->Start = tag_pos;
			tag_marker->Length = tag_len;
			tag_marker->Line = line_num;
			MarkerCount++;
			
//			CzString* tag = new CzString(m_pDataInput->getData(), tag_pos, tag_len);
//			TempElements.push_back(tag);

//CzDebug::Log(tag->c_str());
		}
	}

	return true;
}

eCzXmlParseError CzXmlParser::Parse(CzXmlNode* parent)
{
	CzXmlNodeList* parent_stack = new CzXmlNodeList();
	
	parent_stack->push_back(parent);
	int		count = NextFreePoolTagIndex;
	int		tag_index = 0;
	char*	c_data = m_pDataInput->getData();

	parent = parent_stack->back();

	while (tag_index < MarkerCount)
	{
		if (parent == NULL)
			break;
		CzXmlTagMarker* marker = Marker + tag_index;
		const char* data = c_data + marker->Start;
		int len = marker->Length;

		if (*data == '<' && *(data + len - 1) == '>')
		{
			if (*(data + 1) == '/')
			{
				// Found an end tag, check that name matches parents
				int et_offset = 0;
				int et_len = getNextWord(data + 2, et_offset);
				if (et_len <= 0)
				{
/*					CzString error = "CzXmlParser::Parse - Invalid close tag - ";
					error += parent->Name.c_str();
					error += " - at line ";
					error += CzString(marker->Line);
					CzDebug::Log(error.c_str());*/
					ShowError(XmlErrorInvalidTag, marker->Line);
					return XmlErrorInvalidTag;
				}
				if (!parent->Name.Compare(data + et_offset + 2, et_len))
				{
/*					CzString error = "CzXmlParser::Parse - Mismatched end tag - ";
					error += parent->Name.c_str();
					error += " - at line ";
					error += CzString(marker->Line);
					CzDebug::Log(error.c_str());
					CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "CzXmlParser::Parse - Instead found - ", CzString(data, et_offset + 2, et_len).c_str());*/
					ShowError(XmlErrorMismatchedEndTag, marker->Line);
					return XmlErrorMismatchedEndTag; 
				}
				for (CzXmlNodeList::iterator it = parent_stack->begin(); it != parent_stack->end(); it++)
				{
					CzXmlNode* node = *it;
					if (node == parent)
					{
						parent_stack->erase(it);
						parent = parent_stack->back();
						break;
					}
				}
				tag_index++;
			}
			else
			{
				// Create a node
				CzXmlNode* node = AllocNode();
				node->Line = marker->Line + 1;
				int offset;
				int nlen = getNextWord(data + 1, offset);
				if (nlen <= 0)
				{
//					CzString error = "CzXmlParser::Parse - Invalid tag at line ";
//					error += CzString(marker->Line);
//					CzDebug::Log(error.c_str());
					ShowError(XmlErrorInvalidTag, marker->Line);
					return XmlErrorInvalidTag;
				}
				node->Name.setString(data + offset + 1, nlen);
				if ((len - 2) > nlen)
				{
					eCzXmlParseError error = ParseAttributes(data + offset + nlen + 1, len - nlen - offset - 1, node);
					if (error != XmlErrorNone)
					{
						ShowError(error, marker->Line);
						return error;
					}
				}
				parent->AddChild(node);

				if (*(data + len - 2) != '/')		// Self closing tags dont get added as parents
				{
					parent_stack->push_back(node);	// Found an inner tag so push parent
					parent = parent_stack->back();
				}

				// Check next line to see if it is a tag or a value
				tag_index++;
				if (tag_index != (NextFreePoolTagIndex - 1))
				{
					CzXmlTagMarker* marker2 = Marker + tag_index;
					const char* data2 = c_data + marker2->Start;
					int len2 = marker2->Length;

					if (*data2 != '<')
					{
						// Found a value
						node->Value.setString(data2, len2);
						node->HasValue = true;
					}
				}
			}
		}
		else
			tag_index++;
	}

	parent_stack->clear();
	delete parent_stack;

	return XmlErrorNone;
}

eCzXmlParseError CzXmlParser::Parse(CzDataInput* data)
{
	SAFE_DELETE_ARRAY(TagPool);
	SAFE_DELETE_ARRAY(NodePool);
	SAFE_DELETE_ARRAY(AttributePool);
	m_pRoot = NULL;
	if (m_pDataInput != NULL && data != m_pDataInput)
	{
		delete m_pDataInput;
		m_pDataInput = NULL;
	}

	m_pDataInput = data;

	// Parse header
	ParseXMLHeader();
	
	MarkerCount = 0;
	if (!PreParse())
		return XmlErrorPreParseError;

	// Create root
	m_pRoot = AllocNode();
	m_pRoot->SetName(".", 1);

	eCzXmlParseError error = Parse(m_pRoot);

	SAFE_DELETE(TagPool);

	//	eCzXmlParseError error = Parse(m_pRoot, m_pDataInput->Remaining(), num_tags_found);
	return error;
}

eCzXmlParseError CzXmlParser::ParseXMLHeader()
{
	int pos = m_pDataInput->FindString("<?xml", 5);
	if (pos >= 0)
	{
		pos = m_pDataInput->FindString("?>", 2);
		if (pos >= 0)
		{
			// For now we ignore the xml header info
			// TODO: Pull version and encoding
			m_pDataInput->setPos(pos + 2);
		}
	}

	return XmlErrorNone;
}

//
// CzXmlTools implementation
//
int	CzXmlTools::GetFirstWhitespaceIndex(char* mem, int len)
{
	for (int t = 0; t < len; t++)
	{
		char c = *mem++;
		if (c == ' ' || c == '\t' || c == '\n')
			return t;
	}

	return -1;
}



