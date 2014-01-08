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

#include <stdio.h>
#include <stdlib.h>

#include "CzString.h" 

//
//
// TODO: Replace mem allocations with malloc, realloc and free
//
//

//
//
//
//
//
// CzString implementation
//
//
//
//
//
char* CzString::alloc(int num_chars)
{
	return new char [num_chars];
}

void CzString::free(char*& mem)
{
	if (mem != NULL)
	{
		delete [] mem;
		mem = NULL;
	}
}

/**
 @fn	CzString::CzString(const CzString &string)

 @brief	Constructs a string from another string

 @param	string	The source string.
 */

CzString::CzString(const CzString &string)
{
	FindIndex = 0;
	Data = NULL;
	Length = 0;
	Size = 0;
	AutoHash = true;
	if (string.c_str() == NULL)
		return;
	else
	{
		int len = (int)strlen(string.c_str());
		allocString(len);
		Length = len;
		memcpy(Data, string.c_str(), Length + 1);

		if (AutoHash)
			DataHash = CzString::CalculateHash(Data);
	}
}

/**
 @fn	CzString::CzString(const char *pString)

 @brief	Constructs a string from another string

 @param	string	The source string.
 */

CzString::CzString(const char *pString)
{
	FindIndex = 0;
	Data = NULL;
	Length = 0;
	Size = 0;
	AutoHash = true;
	if (pString == NULL)
		return;
	else
	{
		int len = (int)strlen(pString);
		allocString(len);
		Length = len;
		memcpy(Data, pString, Length + 1);

		if (AutoHash)
			DataHash = CzString::CalculateHash(Data);
	}
}

/**
 @fn	CzString::CzString(const char *pString, int start, int num_chars)

 @brief	Construct a string from a sub string.

 @param	pString  	The source string.
 @param	start	 	The start index.
 @param	num_chars	Number of characters.
 */

CzString::CzString(const char *pString, int start, int num_chars)
{
	FindIndex = 0;
	Data = NULL;
	Length = 0;
	Size = 0;
	AutoHash = true;
	if (pString == NULL)
		return;
	else
	{
		allocString(num_chars);
		Length = num_chars;
		memcpy(Data, pString + start, num_chars);
		Data[num_chars] = 0;

		if (AutoHash)
			DataHash = CzString::CalculateHash(Data);
	}
}

/**
 @fn	CzString::CzString(int v)

 @brief	Construct a string from an integer.

 @param	v	The integer.
 */

CzString::CzString(int v)
{
	FindIndex = 0;
	Data = NULL;
	Length = 0;
	Size = 0;
	AutoHash = true;
	set(v);
}

/**
 @fn	CzString::CzString(unsigned int v)

 @brief	Construct a string from an unsigned integer.

 @param	v	The unsigned integer.
 */

CzString::CzString(unsigned int v)
{
	FindIndex = 0;
	Data = NULL;
	Length = 0;
	Size = 0;
	AutoHash = true;
	char str[64];
	snprintf(str, 64, "%u", v);

	int len = (int)strlen(str);
	allocString(len);
	Length = len;

	memcpy(Data, str, Length + 1);

	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);
}

/**
 @fn	CzString::CzString(float v)

 @brief	Construct a string from a floating point number.

 @param	v	The floating point number.
 */

CzString::CzString(float v)
{
	FindIndex = 0;
	Data = NULL;
	Length = 0;
	Size = 0;
	AutoHash = true;
	set(v);
}

/**
 @fn	CzString::CzString(bool value)

 @brief	Construct a string from a boolean.

 @param	v	The boolean.
 */

CzString::CzString(bool v)
{
	FindIndex = 0;
	Data = NULL;
	Length = 0;
	Size = 0;
	AutoHash = true;
	set(v);
}

CzString::CzString(const CzVec2& v)
{
	FindIndex = 0;
	Data = NULL;
	Length = 0;
	Size = 0;
	AutoHash = true;
	set(v);
}

CzString::CzString(const CzVec3& v)
{
	FindIndex = 0;
	Data = NULL;
	Length = 0;
	Size = 0;
	AutoHash = true;
	set(v);
}

CzString::CzString(const CzVec4& v)
{
	FindIndex = 0;
	Data = NULL;
	Length = 0;
	Size = 0;
	AutoHash = true;
	set(v);
}

CzString::CzString(const CzColour& v)
{
	FindIndex = 0;
	Data = NULL;
	Length = 0;
	Size = 0;
	AutoHash = true;
	set(v);
}

void CzString::reallocString(int len)
{
	FindIndex = 0;
	if (Size > len + 1)
		return;
	int old_size = Size;
	Size = len + 1;
//	if (Size < 16) Size = 16;
	char* data = alloc(Size);
	if (Data != NULL)
	{
		memcpy(data, Data, old_size);
		free(Data);
	}
	Data = data;
//	Data[len] = 0;
}
void CzString::allocString(int len)
{
	FindIndex = 0;
	Length = 0;
	if (Size > len + 1)
	{
		Data[0] = 0;
		return;
	}
	Size = len + 1;
//	if (Size < 16) Size = 16;
	free(Data);
	Data = alloc(Size);
	Data[0] = 0;
}
void CzString::reset()
{
	Length = 0;
	if (Size > 0 && Data != NULL)
		Data[0] = 0;
}
void CzString::setString(const char *str)
{
	FindIndex = 0;
	if (str == NULL)
	{
		free(Data);
		Size = 0;
		Length = 0;
		return;
	}
	int len = strlen(str);
	allocString(len);
	memcpy(Data, str, len + 1);
	Length = len;
	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);
}
void CzString::setString(const char *str, int len)
{
	FindIndex = 0;
	if (str == NULL)
	{
		free(Data);
		Size = 0;
		Length = 0;
		return;
	}
	allocString(len);
	memcpy(Data, str, len);
	Data[len] = 0;
	Length = len;
	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);
}

CzString CzString::getSubString(int start, int max_chars)
{
	if ((start + max_chars) > Length)
		max_chars -= (start + max_chars) - Length;
	return CzString(Data, start, max_chars);
}

CzString CzString::getSubString(int start)
{
	if (start > Length - 1)
		return CzString("");

	return CzString(Data, start, Length - start);
}

/*void CzString::setName(char *name)
{
	if (name == NULL)
		return;
	NameHash = CzString::CalculateHash(name);
}*/

void CzString::setAutoHash(bool enable)
{
	AutoHash = enable;
	if (enable)
	{
		if (Data != NULL)
			DataHash = CzString::CalculateHash(Data);
	}
	else
	{
		DataHash = 0;
	}
}

int CzString::getAsInt() const
{
	if (Data == NULL)
		return 0;
	return atoi(Data);
}

float CzString::getAsFloat() const
{
	if (Data == NULL)
		return 0;
	return (float)atof(Data);
}

bool CzString::getAsBool() const
{
	if (Data == NULL)
		return false;
	return (Compare("1", 1) || Compare("true", 4));
}

int CzString::getAsListOfInt(int *int_pool) const
{
	if (Data == NULL)
		return 0;

	int index = 0;

	// Separate Value by commas
	int			len = Length;
	const char* text = Data;
	char		c;
	char		num[64];

	while ((c = *text++) != 0)
	{
		// Find a number
		int count = 0;
		if (c >= '0' && c <= '9' || c == '-')
		{
			num[count] = c;
			// Read number to end
			while (count++ < 63)
			{
				c = *text;
				if (c < '0' || c > '9')
					break;
				text++;
				num[count] = c;
			}
			// Convert
			num[count] = 0;
			int n = atoi(num);
			int_pool[index++] = n;
		}
	}

	return index;
}

int CzString::getAsListOfFloat(float* float_pool) const
{
	if (Data == NULL)
		return 0;

	int index = 0;

	// Separate Value by commas
	int			len = Length;
	const char* text = Data;
	char		c;
	char		num[64];

	while ((c = *text++) != 0)
	{
		// Find a number
		int count = 0;
		if (c >= '0' && c <= '9' || c == '-')
		{
			num[count] = c;
			// Read number to end
			while (count++ < 63)
			{
				c = *text;
				if ((c < '0' || c > '9') && c != '.')
					break;
				text++;
				num[count] = c;
			}
			// Convert
			num[count] = 0;
			float n = (float)atof((const char*)num);
			float_pool[index++] = n;
		}
	}

	return index;
}

void CzString::set(int v)
{
	FindIndex = 0;
	char str[64];
	snprintf(str, 64, "%d", v);

	int len = (int)strlen(str);

	allocString(len);
	Length = len;

	memcpy(Data, str, Length + 1);

	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);
}

void CzString::set(float v)
{
	FindIndex = 0;
	char str[64];
	snprintf(str, 64, "%f", v);

	int len = (int)strlen(str);

	allocString(len);
	Length = len;

	memcpy(Data, str, Length + 1);

	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);
}

void CzString::set(bool v)
{
	FindIndex = 0;
	int len;
	
	if (v)
		len = 4;
	else
		len = 5;

	allocString(len);
	Length = len;

	if (v)
		strcpy(Data, "true");
	else
		strcpy(Data, "false");

	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);
}

void CzString::set(const CzVec2& v)
{
	FindIndex = 0;
	CzString s = v.x;
	s += ",";
	s += v.y;
	setString(s.c_str());
}

void CzString::set(const CzVec3& v)
{
	FindIndex = 0;
	CzString s = v.x;
	s += ",";
	s += v.y;
	s += ",";
	s += v.z;
	setString(s.c_str());
}

void CzString::set(const CzVec4& v)
{
	CzString s = v.x;
	s += ",";
	s += v.y;
	s += ",";
	s += v.z;
	s += ",";
	s += v.w;
	setString(s.c_str());
}

void CzString::set(const CzColour& v)
{
	CzString s = v.r;
	s += ",";
	s += v.g;
	s += ",";
	s += v.b;
	s += ",";
	s += v.a;
	setString(s.c_str());
}


/**
 @fn	void CzString::Copy(const char* pString, int start, int count)

 @brief	Copies a portion of the supplied string to this string.

 @param	pString	The source string.
 @param	start  	The start index.
 @param	count  	Number of chars to copy.
 */

void CzString::Copy(const char* pString, int start, int count)
{
	allocString(count);
	memcpy(Data, pString + start, count);
	Length = count;
	Data[count] = 0;
	FindIndex = 0;
	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);
}

/**
 @fn	void CzString::Copy(const char* pString)

 @brief	Copies the entire string to this string.

 @param	pString	The source string.
 */

void CzString::Copy(const char* pString)
{
	Copy(pString, 0, (int)strlen(pString));
}

/**
 @fn	void CzString::Copy(CzString& string)

 @brief	Copies the entire string to this string.

 @param [in]	string	The source string.
 */

void CzString::Copy(CzString& string)
{
	Copy((char *)string.c_str(), 0, string.getLength());
}

char CzString::operator[]	(int nIndex)
{
	if (nIndex < 0 || nIndex > getLength())
		return 0;

	return *(Data + nIndex);
}

CzString& CzString::operator= (const CzString &op)
{
	if (op.c_str() == NULL)
	{
		free(Data);
		Size = 0;
		Length = 0;
		return *this;
	}

	int length = op.getLength();
	FindIndex = 0;
	allocString(length);
	Length = length;
	memcpy(Data, op.c_str(), Length + 1);

	if (AutoHash)
		DataHash = op.getHash();

	return *this;
}

CzString& CzString::operator=	(const char *op)
{
	if (op == Data)
		return *this;
	if (op == NULL)
	{
		free(Data);
		Size = 0;
		Length = 0;
		return *this;
	}

	FindIndex = 0;
	int length = (int)strlen(op);

	allocString(length);
	Length = length;
	memcpy(Data, op, Length + 1);

	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);

	return *this;
}

CzString& CzString::operator+= (const CzString &op)
{
	if (op.c_str() == NULL)
		return *this;

	int len1 = getLength();
	int	len2 = op.getLength();

	if (Data == NULL)
	{
		allocString(len1 + len2);
		memcpy(Data, op.c_str(), len2 + 1);
	}
	else
	{
		if ((len1 + len2) >= Size)
			reallocString(len1 + len2);

		memcpy(Data + len1, op.c_str(), len2);
		Data[len1 + len2] = 0;
	}
	Length = len1 + len2;

	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);

	return *this;
}

CzString& CzString::operator+= (const char *op)
{
	if (op == NULL)
		return *this;

	int len1 = getLength();
	int	len2 = (int)strlen(op);

	if (Data == NULL)
	{
		allocString(len1 + len2);
		memcpy(Data, op, len2 + 1);
	}
	else
	{
		if ((len1 + len2) >= Size)
			reallocString(len1 + len2);

		memcpy(Data + len1, op, len2);
		Data[len1 + len2] = 0;
	}
	Length = len1 + len2;

	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);
	
	return *this;
}

CzString& CzString::operator=	(int v)
{
	set(v);

	return *this;
}

CzString& CzString::operator=	(float v)
{
	set(v);

	return *this;
}

CzString& CzString::operator=	(char chr)
{
	FindIndex = 0;

	allocString(2);

	*Data = chr;
	*(Data + 1) = 0;
	Length = 1;

	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);
	
	return *this;
}

CzString& CzString::operator+=	(int v)
{
	char str[64];
	snprintf(str, 64, "%d", v);

	int len1 = getLength();
	int	len2 = (int)strlen(str);

	if (Data == NULL)
	{
		allocString(len1 + len2);
		memcpy(Data, str, len2 + 1);
	}
	else
	{
		if ((len1 + len2) >= Size)
			reallocString(len1 + len2);

		memcpy(Data + len1, str, len2);
		Data[len1 + len2] = 0;
	}

	Length = len1 + len2;

	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);
	
	return *this;
}

CzString& CzString::operator+=	(float v)
{
	char str[64];
	snprintf(str, 64, "%f", v);

	int len1 = getLength();
	int	len2 = (int)strlen(str);

	if (Data == NULL)
	{
		allocString(len1 + len2);
		memcpy(Data, str, len2 + 1);
	}
	else
	{
		if ((len1 + len2) >= Size)
			reallocString(len1 + len2);

		memcpy(Data + len1, str, len2);
		Data[len1 + len2] = 0;
	}

	Length = len1 + len2;

	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);
	
	return *this;
}

CzString& CzString::operator+=	(char chr)
{
	int len1 = getLength();
	int	len2 = 1;

	if (Data == NULL)
	{
		allocString(len1 + len2);
		*Data = chr;
	}
	else
	{
		if ((len1 + len2) >= Size)
			reallocString(len1 + len2);

		*(Data + len1) = chr;
		Data[len1 + len2] = 0;
	}

	Length = len1 + len2;

	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);
	
	return *this;
}

bool CzString::operator==	(const CzString &op)
{
	if (Data == NULL)
		return false;

	if (AutoHash && op.isAutohash())
	{
		if (DataHash == op.getHash())
			return true;
	}
	else
	{
		if (strcmp(op.c_str(), Data) == 0)
			return true;
	}
	return false;
}

bool CzString::operator==	(unsigned int hash)
{
	if (Data == NULL)
		return false;

	if (!AutoHash)
		DataHash = CalculateHash(Data);

	if (DataHash == hash)
		return true;

	return false;
}

bool CzString::operator==	(const char *op)
{
	if (Data == NULL)
		return false;

	if (strcmp(op, Data) == 0)
		return true;

	return false;
}

bool CzString::Compare(const char* pString, int len) const
{
	return memcmp(Data, pString, len) == 0;
}

bool CzString::Compare(int start, const char* pString, int len) const
{
	return memcmp(Data + start, pString, len) == 0;
}

unsigned int	CzString::CalculateHash(const char* pString, int hash)
{
	if (pString == NULL)
		return 0;

	int c;
	while ((c = *pString++))
	{
		c = (c < 'A' || c > 'Z') ? c : (c - 'A' + 'a');
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}

/**
 @fn	int CzString::Replace(const char* string, const char* with)

 @brief	Replaces all occurances of "string" with new string "with".

 @param	string	The string to replace.
 @param	with  	The replacement string.

 @return	.
 */

int CzString::Replace(const char* string, const char* with)
{
	int len = strlen(string);
	int wlen = strlen(with);

	// Find the string
	FindReset();

	// Count how many times the string occurs
	int occcurances = 0;
	int pos = 0;
	do
	{
		pos = FindNext(string, len);
		FindIndex = pos + len;
		if (pos >= 0)
			occcurances++;
	}
	while (pos >= 0);
	if (occcurances == 0)
		return -1;

	// Calculate new string size
	int new_size = Length - len * occcurances + wlen * occcurances;
	char* new_buffer = alloc(new_size + 1);

	// Build new string with replacements
	FindReset();
	int start_pos = 0, buff_pos = 0;
	pos = 0;
	do
	{
		pos = FindNext(string, len);
		FindIndex = pos + len;
		if (pos < 0)
		{
			// Copy any remaining string
			memcpy(new_buffer + buff_pos, Data + start_pos, Length - start_pos);
			buff_pos += Length - start_pos;
			break;
		}

		// Copy string that doesnt match
		int plen = pos - start_pos;
		memcpy(new_buffer + buff_pos, Data + start_pos, plen);
		buff_pos += plen;

		// Copy replacement
		memcpy(new_buffer + buff_pos, with, wlen);
		buff_pos += wlen;

		start_pos = FindIndex;
	}
	while (pos >= 0);

	new_buffer[buff_pos] = 0;
	free(Data);
	Data = new_buffer;
	Length = new_size;
	Size = new_size + 1;

	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);

	return pos;
}

/**
 @fn	void CzString::Replace(char chr, char with)

 @brief	Replaces all occurances of character "chr" with new character "with".

 @param	chr 	The character to replace.
 @param	with	The replacement character.
 */

void CzString::Replace(char chr, char with)
{
	char*	str = Data;

	for (int t = 0; t < Length; t++)
	{
		if (*str == chr)
		{
			*str = with;
		}
		str++;
	}
}

/**
 @fn	bool CzString::ContainsHTMLCodes() const

 @brief	Query if the string potentially contains special HTML characters.

 @return	true if it succeeds, false if it fails.
 */

bool CzString::ContainsHTMLCodes() const
{
	// Check for chance of special char
	bool	found = false;
	char*	str = Data;

	for (int t = 0; t < Length; t++)
	{
		char c = *str++;
		if (c == '&')
		{
			return true;
		}
	}

	return false;
}

/**
 @fn	int CzString::Contains(char c) const

 @brief	Determines if the string contains the sepcified character

 @param	c	The character to test.

 @return	Position at which character was found or -1 for not found.
 */

int CzString::Contains(char c) const
{
	if (Data == NULL)
		return -1;
	if (*Data == c)
		return 0;
	char*	str = Data + 1;

	for (int t = 1; t < Length; t++)
	{
		if (*str++ == c)
			return t;
	}

	return -1;
}

/**
 @fn	int CzString::Occurrences(char c) const

 @brief	Counts the number of occurrences of a specific character.

 @param	c	The character to test.

 @return	Number of occurances, -1 for error.
 */

int CzString::Occurrences(char c) const
{
	if (Data == NULL)
		return -1;
	char*	str = Data;

	int count = 0;
	for (int t = 0; t < Length; t++)
	{
		if (*str++ == c)
			count++;
	}

	return count;
}

/**
 @fn	void CzString::ReplaceHTMLCodes()

 @brief	Replace any specical HTML codes with single character versions. 
 
 Supports the following codes:
 - &lt;
 - &gt;
 - &amp;
 - &quot;
 - &apos;

 */

void CzString::ReplaceHTMLCodes()
{
	if (!ContainsHTMLCodes())
		return;

	char* str = Data;
	char* nstr = str;

	// Replace all instances of special tags with ascii codes
	int len = Length;
	for (int t = 0; t < len; t++)
	{
		char c = *str++;
		if (c == '&')
		{
			if (t < len - 3)
			{
				if (*str == 'l' && *(str + 1) == 't' && *(str + 2) == ';')
				{
					*nstr++ = '<';
					str += 3;  t += 3;
				}
				else
				if (*str == 'g' && *(str + 1) == 't' && *(str + 2) == ';')
				{
					*nstr++ = '>';
					str += 3;  t += 3;
				}
				else
				if (t < len - 4)
				{
					if (*str == 'a' && *(str + 1) == 'm' && *(str + 2) == 'p' && *(str + 3) == ';')
					{
						*nstr++ = '&';
						str += 4;  t += 4;
					}
					else
					if (t < len - 4)
					{
						if (*str == 'q' && *(str + 1) == 'u' && *(str + 2) == 'o' && *(str + 3) == 't' && *(str + 4) == ';')
						{
							*nstr++ = '"';
							str += 5;  t += 5;
						}
						else
						if (*str == 'a' && *(str + 1) == 'p' && *(str + 2) == 'o' && *(str + 3) == 's' && *(str + 4) == ';')
						{
							*nstr++ = '\'';
							str += 5;  t += 5;
						}
						else
							*nstr++ = c;
					}
					else
						*nstr++ = c;
				}
				else
					*nstr++ = c;
			}
			else
				*nstr++ = c;
		}
		else
			*nstr++ = c;
	}
	*nstr = 0;
	
	FindIndex = 0;
	Length = (int)nstr - (int)Data;
	
	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);
}

/**
 @fn	void CzString::URLEncode(const char* str)

 @brief	URL encodes the supplied string.

 @param	str	The string to encode.
 */

void CzString::URLEncode(const char* str)
{
	int slen = strlen(str);
	if (slen == 0)
		return;
	char* new_data = alloc(slen * 3 + 1);

	char hexstr[4];
	int len = 0;
	for (int t = 0; t < slen; t++)
	{
		unsigned int c = (unsigned int)*str++;
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c < '9') || c == '$' || c == '-' || c == '_' || c == '.' || c == '!' || c == '*' || c == '\'' || c == '(' || c == ')')
		{
			*(new_data + len) = c; len++;
		}
		else
		{
			snprintf(hexstr, 4, "%x", c);
			*(new_data + len) = '%'; len++;
			if (c < 16)
			{
				*(new_data + len) = '0'; len++;
				*(new_data + len) = hexstr[0]; len++;
			}
			else
			{
				*(new_data + len) = hexstr[0]; len++;
				*(new_data + len) = hexstr[1]; len++;
			}
		}
	}
	*(new_data + len) = 0;

	free(Data);
	FindIndex = 0;
	Data = new_data;
	Size = Length * 3 + 1;
	Length = len;

	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);
}

/**
 @fn	void CzString::URLEncode()

 @brief	URL encodes this string.

 */

void CzString::URLEncode()
{
	URLEncode(Data);
}

/**
 @fn	void CzString::URLDecode()

 @brief	URL decodes this string.

 */

void CzString::URLDecode()
{
	if (Data == NULL)
		return;

	int len = 0;
	char* new_data = alloc(Length + 1);
	char* str = Data;

	for (int t = 0; t < Length; t++)
	{
		unsigned int c = (unsigned int)*str++;
		if (c == '%')
		{
			int h1 = GetValueFromHexDigit(*str++);
			int h2 = GetValueFromHexDigit(*str++);
			*(new_data + len) = (h1 << 4) | h2;
		}
		else
		{
			*(new_data + len) = c;
		}
		len++;
	}
	*(new_data + len) = 0;

	free(Data);
	FindIndex = 0;
	Data = new_data;
	Size = Length + 1;
	Length = len;

	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);
}

/**
 @fn	void CzString::HexEncode(const char* str, int num_bytes)

 @brief	Hexadecimal encode the supplied string.

 @param	str		 	The source string.
 @param	num_bytes	Number of bytes.
 */

void CzString::HexEncode(const char* str, int num_bytes)
{
	if (num_bytes == 0)
		return;

	char* new_data = alloc(num_bytes * 2 + 1);
	char hexstr[4];

	int len = 0;
	for (int t = 0; t < num_bytes; t++)
	{
		unsigned int c = (unsigned int)*str++;
		snprintf(hexstr, 4, "%x", c);
		if (c < 16)
		{
			*(new_data + len) = '0'; len++;
			*(new_data + len) = hexstr[0]; len++;
		}
		else
		{
			*(new_data + len) = hexstr[0]; len++;
			*(new_data + len) = hexstr[1]; len++;
		}
	}
	*(new_data + len) = 0;

	free(Data);
	Data = new_data;
	FindIndex = 0;
	Length = len;
	Size = Length + 1;

	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);
}

/**
 @fn	void CzString::HexEncode()

 @brief	Hexadecimal encode this string.

 */

void CzString::HexEncode()
{
	HexEncode(Data, Length);
}

/**
 @fn	void CzString::HexDecode()

 @brief	Hexadecimal decode this string.

 */

void CzString::HexDecode()
{
	if (Data == NULL)
		return;

	int len = 0;
	char* new_data = Data;
	char* str = Data;

	for (int t = 0; t < Length / 2; t++)
	{
		int h1 = GetValueFromHexDigit(*str++);
		int h2 = GetValueFromHexDigit(*str++);
		*(new_data + len) = (h1 << 4) | h2;
		len++;
	}
	*(new_data + len) = 0;
	Length = len;
	FindIndex = 0;

	if (AutoHash)
		DataHash = CzString::CalculateHash(Data);
}

/**
 @fn	bool CzString::SplitFilename(CzString& filename, CzString& ext)

 @brief	Splits this string into filename and extension.

 @param [out]	filename	Filename component.
 @param [out]	ext			The extension component.

 @return	true if it succeeds, false if it fails.
 */

bool CzString::SplitFilename(CzString& filename, CzString& ext)
{
	int index = 0;
	
	// Find the dot
	for (int t = getLength() - 1; t != 0; t--)
	{
		if (*(Data + t) == '.')
		{
			index = t;
			break;
		}
	}
	if (index == 0) return false;
	
	filename.Copy(Data, 0, index);
	ext.Copy(Data, index + 1, getLength() - index - 1);
	
	return true;
}

/**
 @fn	bool CzString::GetFilenameExt(CzString& ext)

 @brief	Gets the filename extent portion of the filename.

 @param [out]	ext	The extension.

 @return	true if it succeeds, false if it fails.
 */

bool CzString::GetFilenameExt(CzString& ext)
{
	int index = 0;
	
	// Find the dot
	for (int t = getLength() - 1; t != 0; t--)
	{
		if (*(Data + t) == '.')
		{
			index = t;
			break;
		}
	}
	if (index == 0) return false;
	
	ext.Copy(Data, index + 1, getLength() - index - 1);
	
	return true;
}

/**
 @fn	bool CzString::SplitVarIndex(CzString& var, int& index)

 @brief	Splits an XOML variable name / index (var_name:index)

 @param [out]	var  	The variable name component.
 @param [out]	index	The index component.
 @param [out]	vindex	The index component as text.

 @return	true if it succeeds, false if it fails.
 */

bool CzString::SplitVarIndex(CzString& var, int& index, CzString& vindex)
{
	char* pData = Data;
	index = -1;

	FindReset();

	// Split string at colon char and return value after colon
	for (int t = 0; t < Length; t++)
	{
		if (*pData++ == ':')
		{
			var.setString(Data, t);
			vindex.Copy(pData, 0, Length - t - 1);
			if (CzString::IsNumber(*pData))
				index = vindex.getAsInt();
			return true;
		}
	}

	// No split found so just copy this string to output
	var = *this;

	return true;
}

/**
 @fn	bool CzString::SplitPropVarIndex(CzString& prop, CzString& var, int& index)

 @brief	Splits an XOML property variable name / index ([prop_name]var_name:index)

 @param [out]	prop 	The property name component.
 @param [out]	var  	The variable name component.
 @param [out]	index	The index component.
 @param [out]	vindex	The index component as text.
 
 @return	true if it succeeds, false if it fails.
 */

bool CzString::SplitPropVarIndex(CzString& prop, CzString& var, int& index, CzString& vindex)
{
	char* pData = Data;
	index = -1;

	FindReset();

	// Get property
	int pos = StepFindIndexNoneWhiteSpace();
	if (pos < 0)
		return false;
	int start_pos = 0;
	int len = GetNextMarkedString('[', ']', start_pos);
	if (len >= 0)
		prop.setString(Data + start_pos, len);
	if (StepFindIndex(1) == Length)
		return false;

	// Split string at colon char and return value after colon
	pData += FindIndex;
	for (int t = 0; t < Length - FindIndex; t++)
	{
		if (*pData++ == ':')
		{
			var.setString(Data + FindIndex, t);
			vindex.Copy(pData, 0, Length - t - 1);
			if (CzString::IsNumber(*pData))
				index = vindex.getAsInt();

			return true;
		}
	}

	// No split found so just copy out the var
	var.setString(Data + FindIndex, Length - FindIndex);

	return true;
}

/**
 @fn	CzSlotArray<CzString*>* CzString::Split(char split_char)

 @brief	Slpits the string at the specified characer

 @param	split_char	The split character.

 @return	null if it fails, else an array of sub strings.
 */

CzSlotArray<CzString*>* CzString::Split(char split_char)
{
	CzSlotArray<CzString*>* strings = new CzSlotArray<CzString*>();

	Split(split_char, strings);

	return strings;
}

/**
 @fn	void CzString::Split(char split_char, CzSlotArray<CzString*>* strings)

 @brief	Slpits the string at the specified characer

 @param	split_char		   	The split character.
 @param [out]	strings	If non-null, the split strings.
 */

void CzString::Split(char split_char, CzSlotArray<CzString*>* strings)
{
	char* pData = Data;

	FindReset();
	strings->clear();
	int start = 0, len = 0;
	for (int t = 0; t < Length + 1; t++)
	{
		char c = *pData++;
		if (c == split_char || c == 0)
		{
			CzString* s = new CzString();
			s->setString(Data + start, t - start);
			start = t + 1;
			strings->add(s);
		}
	}
}

/**
 @fn	void CzString::ToUpper()

 @brief	Converts this string upper case.

 */

void CzString::ToUpper()
{
	char* pData = Data;
	for (int t = 0; t < Length; t++)
	{
		char c = *pData;
		if (IsLower(c))
		{
			c += 'A' - 'a';
			*pData++ = c;
		}
		else
			pData++;
	}
}

/**
 @fn	void CzString::ToLower()

 @brief	Converts this string to lower case

 */

void CzString::ToLower()
{
	char* pData = Data;
	for (int t = 0; t < Length; t++)
	{
		char c = *pData;
		if (IsUpper(c))
		{
			c += 'a' - 'A';
			*pData++ = c;
		}
		else
			pData++;
	}
}

int	CzString::FindNext(const char* string, int len)
{
	int		count = Length - FindIndex;
	char*	str = Data + FindIndex;
	if (count <= 0)
		return -1;
	
	// Find first character
	char fc = *string;
	while (count != 0)
	{
		char c = *str++;
		if (c == fc)
		{
			// if number of chars left in this string are less than the length of the test string then it cant match
			if (count < len)
				return -1;

			// Check the rest of the string
			char*	str1 = str;
			char*	str2 = (char*)string + 1;
			int		count2 = len - 1;
			bool	mismatch = false;

			while (count2-- != 0)
			{
				if (*str1++ != *str2++)
				{
					mismatch = true;
					break;
				}
			}
			
			// A match was found
			if (!mismatch)
				return FindIndex;
		}
		FindIndex++;
		count--;
	}

	return -1;
}

int	CzString::FindNext(char chr)
{
	int		count = Length - FindIndex;
	char*	str = Data + FindIndex;
	if (count <= 0)
		return -1;
	
	while (count != 0)
	{
		char c = *str++;
		if (c == chr)
			return FindIndex;
		FindIndex++;
		count--;
	}

	return -1;
}

int	CzString::Find(const char* string)
{
	FindIndex = 0;
	int index = FindNext(string);
	FindIndex = 0;
	return index;
}

int	CzString::FindNext(const char* string)
{
	return FindNext(string, strlen(string));
}

void CzString::FindReset()
{
	FindIndex = 0;
}

int CzString::StepFindIndex(int amount)
{
	FindIndex += amount;
	if (FindIndex < 0)
		FindIndex = 0;
	else
	if (FindIndex > Length)
		FindIndex = Length;

	return FindIndex;
}

int CzString::StepFindIndexNoneWhiteSpace()
{
	int		count = Length - FindIndex;
	char*	str = Data + FindIndex;
	if (count <= 0)
		return -1;
	
	while (count != 0)
	{
		char c = *str++;
		if (c != ' ' && c != '\t')
			return FindIndex;
		FindIndex++;
		count--;
	}

	return -1;
}

int CzString::StepFindIndexWhiteSpace()
{
	int		count = Length - FindIndex;
	char*	str = Data + FindIndex;
	if (count <= 0)
		return -1;
	
	while (count != 0)
	{
		char c = *str++;
		if (c == ' ' && c == '\t')
			return FindIndex;
		FindIndex++;
		count--;
	}

	return -1;
}

int CzString::GetNextMarkerOffset(char marker)
{
	int		count = Length - FindIndex;
	char*	str = Data + FindIndex;
	if (count <= 0)
		return -1;
	
	int start = FindIndex;

	while (count-- != 0)
	{
		char c = *str++;
		if (c == marker || c == 0)
		{
			FindIndex++;					// Skip the marker for next search
			return FindIndex - start - 1;	// return length from start
		}

		FindIndex++;
	}

	return -1;
}

int CzString::GetNextString()
{
	int		count = Length - FindIndex;
	char*	str = Data + FindIndex;
	if (count <= 0)
		return -1;
	int start = FindIndex;
	
	while (count-- != 0)
	{
		char c = *str++;
		if (c == ' ' || c == 0)
			return FindIndex - start;
			
		FindIndex++;
	}

	return FindIndex - start;
}

int CzString::GetNextMarkedString(char start_mark, char end_mark, int &offset)
{
	int		count = Length - FindIndex;
	char*	str = Data + FindIndex;
	if (count <= 0)
		return -1;
	
	offset = -1;

	while (count-- != 0)
	{
		char c = *str++;
		if (offset < 0)
		{
			// Find start marker
			if (c == start_mark)
				offset = FindIndex + 1;
		}
		else
		{
			// Find end marker
			if (c == end_mark)
				return FindIndex - offset;
		}

		FindIndex++;
	}

	return -1;
}

int CzString::GetNextMarkedStringAfterString(const char* search_string, char start_mark, char end_mark, CzString& out_string)
{
	int pos = FindNext(search_string);
	if (pos >= 0)
	{
		// Get the text label
		int offset;
		StepFindIndex(strlen(search_string));
		int len = GetNextMarkedString(start_mark, end_mark, offset);
		if (len > 0)
		{
			out_string.setString(getString() + offset, len);
			return pos;
		}
		
		return -1;
	}

	return -1;
}
int CzString::getAsVectorOfInt(CzVector<int>* int_pool)
{
	if (Data == NULL)
		return 0;

	int index = 0;

	// Separate Value by commas
	int			len = Length;
	const char* text = Data;
	char		c;
	char		num[64];

	while ((c = *text++) != 0)
	{
		// Find a number
		int count = 0;
		if (c >= '0' && c <= '9' || c == '-')
		{
			num[count] = c;
			// Read number to end
			while (count++ < 63)
			{
				c = *text;
				if (c < '0' || c > '9')
					break;
				text++;
				num[count] = c;
			}
			// Convert
			num[count] = 0;
			int n = atoi(num);
			//int_pool[index++] = n;
			int_pool->push_back(n);
		}
	}

	return index;
}

int CzString::getAsVectorOfFloat(CzVector<float>* float_pool)
{
	if (Data == NULL)
		return 0;

	int index = 0;

	// Separate Value by commas
	int			len = Length;
	const char* text = Data;
	char		c;
	char		num[64];
	static char * e = "e";
	
	
	
	while ((c = *text++) != 0)
	{
		// Find a number
		int count = 0;
		if (c >= '0' && c <= '9' || c == '-')
		{
			num[count] = c;
			// Read number to end
			while (count++ < 63)
			{
				c = *text;
				if ((c < '0' || c > '9') && c != '.' && c != *e)
					break;
				text++;
				num[count] = c;
			}
			// Convert
			num[count] = 0;
			float n = (float)atof((const char*)num);
			//float_pool->at(index++) = n;
			float_pool->push_back(n);
		}
	}

	return index;
}
