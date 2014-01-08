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

#ifndef __CZ_STRING_H__
#define __CZ_STRING_H__

#include <string>
#include "CzUtil.h"
#include "CzSlotArray.h"

/**
 @addtogroup Core
 @{
 */
 
/**
 @class	CzString

 @brief	A simple named string builder style class

 <h1>Introduction</h1>

 Strings are used extensively throughout app / game development, making it an incredibly important subject. We use strings for everything from naming objects to presenting interactive text to the player. 

 String building can be a nightmare for memory managers as constantly rebuilding strings causes many memory allocations and deallocations fragmenting the available memory into small hard to use units. 

 A string builder is a class that allows you to build a string using a predefined sized buffer or at the very least a buffer than can be resized. CzString supports the following features:
 - String builder functionality
 - String concetanation and resizing
 - String building from basic types such as integers, floats and boolean types
 - String comparison
 - String splitting
 - Stream style string searching
 - Find strings between markers
 - Character replacement
 - HTML decoding
 - Hex encoding / decoding
 - URL encoding / decoding
 - Change of case
 - Extraction of numbers / items and lists of numbers / items

 <h1>Basic String Building</h1>

 Strings can be created from raw text, integers, floats, boolean and even vector variables as shown below:

 @code
CzString string("Hello");		// Creation from raw text
CzString int_string(1234);		// Creation from an integer
CzString int_string(100.234f);	// Creation from a float
CzString int_string(true);		// Creation from a boolean
 @endcode

 Strings can also be concatenated:


 @eode
CzString string("Hello");
string += ". How you doing";
 @endcode


 If you are creating a string and you know that it will require quite a number of concatenations then you should set its initial size to prevent memory resizing, here's an example:

 @code
CzString string;
string.allocString(1024);	// here we preallocate 1024 bytes of memory for the string
string += "Hello!";
string += " How you doing.";
string += " I'm great thanks, how are you?";
string += " Fantastico!";
 @endcode

 <h1>Comparing Strings</h1>

 When two strings are compared using the CzString class their hash values are compared for equality. By default each time you create or modify a string a hash value for the string is recalculated. 
 When building a long string it may be a good idea to turn auto string hashing off until the string has been built then turn itback on just before your last concatenation:

 @code
CzString string;
string.allocString(1024);
string.setAutoHash(false);
string += "Hello!";
string += " How you doing.";
string += " I'm great thanks, how are you?";
string.setAutoHash(true);
string += " Fantastico!";
 @endcode

 Note that if string auto hashing is disabled then a full string compare will be carried out.

 <h1>Stream Style Searching</h1>

 CzString is set up to allow stream like searching whereby your last searched position will be saved, allowing you to carry out additional searches from where the last search left off. This 
 type of string searching is incredibly useful when it comes to parsing areas of memory. The following methods can be used:

 <h1>Getting Strings Values</h1>

 CzString provides some useful methods for converting from strings to certain other types:

 @code
int				getAsInt() const;								// Returns the integer value of the string
bool			getAsBool() const;								// Returns the boolean value of the string. Valid values include true and 1, all other values are classed as false
float			getAsFloat() const;								// Returns the floating point value of the string
int				getAsListOfInt(int *int_pool);					// Returns a list of integers (string should contain comma separated values)
int				getAsListOfFloat(float* float_pool);			// Returns a list of floats (string should contain comma separated values)
int				getAsVectorOfFloat(CzVector<float> *int_pool);	// Returns a vector of ints (string should contain comma separated values)
int		        getAsVectorOfInt(CzVector<int> *int_pool);		// Returns a vector of floats (string should contain comma separated values)
 @endcode

 <h1>Other Useful String Tools</h1>

 CzString contains many other useful utility methods to help make various tasks easier:
 - Replace() - Replaces all occurrences of char / string with the replacement in a string
 - Contains() - Returns true if a string contains the specified character
 - ReplaceHTMLCodes() - Replaces HTML style codes such as &amp; with their ACII equivalents
 - URLEncode() - Encodes a string as URL encoded
 - URLDecode() - Decodes a URL encoded string
 - HexEncode() - Encodes the text as hexadecimal
 - HexDecode() - Decodes a string of Hex to text
 - ToLower() - Converts a string to all lower case
 - ToUpper() - Converts a string to all upper case
 - SplitFileName() - Splits a string into file name and extension strings
 - GetFilenameExt() - Extracts a file names extension as a string
 - Split() - Splits a string using a specified separator into an array of strings 
 - SplitVarIndex() - Splits an XOML variable name / index (var_name:index)
 - SplitPropVarIndex() - Splits an XOML property variable name / index ([prop_name]var_name:index)

 */

class CzString
{
	// Properties
protected:
	char*			Data;			///< The string data
	int				Length;			///< Length of the string
	int				Size;			///< Size of memory reserved for the string
	unsigned int	DataHash;		///< Hash value of the data (for fast comparison)
//	unsigned int	NameHash;		///< Hash value of the name (for fast searching)
	bool			AutoHash;		///< true to calculate hash value every time string is updated
	int				FindIndex;		///< Used to track multiple searches

public:
	void			reallocString(int len);											///< Reallocate the amount of space available to the string
	void			allocString(int len);											///< Allocates a new string, destroying the existing string
	void			reset();														///< Destroys the string
	void			setString(const char *str);										///< Sets the string to the new string
	void			setString(const char *str, int len);							///< Sets the string to the new string of specified length
	char*			getString()									{ return Data; }	///< Returns the string
	CzString		getSubString(int start, int max_chars);							///< Returns a sub string of this string
	CzString		getSubString(int start);										///< Returns all chars from start to the enmd of the string
//	void			setName(char *name);
//	unsigned int	getNameHash() const							{ return NameHash; }
	unsigned int	getHash() const								{ return DataHash; }	///< Gets ths strings hash value
	void			setAutoHash(bool enable);											///< Sets string auo hashing on / off
	void			setLength(int length)						{ Length = length; }	///< Sets ths strings length (does not affect the amount of memory allocated to the string)
	bool			isAutohash() const							{ return AutoHash; }	///< Returns true if auto hashing is enabled
	// Properties end

private:
	char*			alloc(int num_chars);
	void			free(char*& mem);

public:
	CzString() : Data(NULL), Length(0), Size(0), AutoHash(true), FindIndex(0), DataHash(0) {}
	CzString(const CzString &string);
	CzString(const char *pString, int start, int num_chars);
	CzString(const char *pString);
	CzString(int v);
	CzString(unsigned int v);
	CzString(float v);
	CzString(bool v);
	CzString(const CzVec2& v);
	CzString(const CzVec3& v);
	CzString(const CzVec4& v);
	CzString(const CzColour& v);
	virtual ~CzString()
	{
		if (Data != NULL)
			delete [] Data;
	}

	// Query
	int				getSize() const { return Size; }							///< Returns the size of the string. Size is the amount of memory available to the string
	int				getLength() const { return Length; }						///< Returns the length of the string
	int				getAsInt() const;											///< Returns the integer value of the string
	bool			getAsBool() const;											///< Returns the boolean value of the string. Valid values include true and 1, all other values are classed as false
	float			getAsFloat() const;											///< Returns the floating point value of the string
	int				getAsListOfInt(int *int_pool) const;						///< Returns a list of integers (string should contain comma separated values)
	int				getAsListOfFloat(float* float_pool) const;					///< Returns a list of floats (string should contain comma separated values)
	int				getAsVectorOfFloat(CzVector<float> *int_pool);				///< Returns a vector of ints (string should contain comma separated values)
	int		        getAsVectorOfInt(CzVector<int> *int_pool);					///< Returns a vector of floats (string should contain comma separated values)
	bool			isEmpty() const { return Data == NULL || Length == 0; }		///< Returns true if the string is empty

	void			set(int v);
	void			set(float v);
	void			set(bool v);
	void			set(const CzVec2& v);
	void			set(const CzVec3& v);
	void			set(const CzVec4& v);
	void			set(const CzColour& v);
	
	// Copy
	void			Copy(const char* pString);
	void			Copy(CzString& pString);
	void			Copy(const char *pString, int start, int count);

	// Operators
	CzString&		operator=	(const CzString& op);
	CzString&		operator=	(const char *op);
	char			operator[]	(int nIndex);
	CzString&		operator+=	(const CzString& op);
	CzString&		operator+=	(const char* op);
	CzString&		operator=	(int v);
	CzString&		operator=	(float v);
	CzString&		operator=	(char chr);
	CzString&		operator+=	(int v);
	CzString&		operator+=	(float v);
	CzString&		operator+=	(char chr);
	bool			operator==	(const CzString& op);
	bool			operator==	(const char* op);
	bool			operator==	(unsigned int hash);
	char*			str() const { return Data; }
	const char*		c_str() const { return (const char *)Data; }

	bool			Compare(const char* pString, int len) const;						///< Compares two strings, returns true if same
	bool			Compare(int start, const char* pString, int len) const;				///< Compares a string and a sub string, returns true if same

	// Searching
	int				Find(const char* string);											///< Simple string search
	int				FindNext(const char* string, int len);								///< Searches from last find position for text string
	int				FindNext(const char* string);										///< Searches from last find position for text string
	int				FindNext(char chr);													///< Searches from last find position for the specified character
	void			FindReset();														///< Resets the find position to start of string
	int				StepFindIndex(int amount);											///< Adjust the find position by the specified
	int				StepFindIndexNoneWhiteSpace();										///< Adjust the find position to the next none white space
	int				StepFindIndexWhiteSpace();											///< Adjust the find position to the next white space
	void			setFindIndex(int index) { FindIndex = index; }						///< Sets the current find index
	int				getFindIndex() const { return FindIndex; }							///< Gets the current find index
	int				GetNextMarkerOffset(char marker);									///< Returns length to the end of paramneter marker
	int				GetNextString();													///< Returns the next string
	int				GetNextMarkedString(char start_mark, char end_mark, int &offset);	///< Returns a string marked by start and end marker characters
	int				GetNextMarkedStringAfterString(const char* search_string, char start_mark, char end_mark, CzString& out_string);	///< Returns the next marked string after finding a certain string
	// Utility
	int				Replace(const char* string, const char* with);
	void			Replace(char chr, char with);
	bool			ContainsHTMLCodes() const;
	int				Contains(char c) const;
	int				Occurrences(char c) const;
	void			ReplaceHTMLCodes();
	void			URLEncode(const char* str);
	void			URLEncode();
	void			URLDecode();
	void			HexEncode(const char* str, int num_bytes);
	void			HexEncode();
	void			HexDecode();
	bool			SplitFilename(CzString& filename, CzString& ext);
	bool			GetFilenameExt(CzString& ext);
	bool			SplitVarIndex(CzString &var, int &index, CzString& vindex);
	bool			SplitPropVarIndex(CzString &prop, CzString &var, int &index, CzString& vindex);
	CzSlotArray<CzString*>* Split(char split_char);
	void			Split(char split_char, CzSlotArray<CzString*>* strings);

	static unsigned int	CalculateHash(const char* pString, int hash = 5381);

	inline static bool IsNumber(char c)
	{
		if (c >= '0' && c <= '9')
			return true;
		return false;
	}

	inline static bool IsAlpha(char c)
	{
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
			return true;
		return false;
	}

	inline static bool IsLower(char c)
	{
		if (c >= 'a' && c <= 'z')
			return true;
		return false;
	}

	inline static bool IsUpper(char c)
	{
		if (c >= 'A' && c <= 'Z')
			return true;
		return false;
	}

	inline static int GetValueFromHexDigit(char c)
	{
		if (c >= '0' && c <= '9')
			return c - '0';
		if (c >= 'a' && c <= 'f')
			return c - 'a' + 10;
		if (c >= 'A' && c <= 'F')
			return c - 'A' + 10;

		return 0;
	}
	
	void		ToUpper();
	void		ToLower();
};

/**
 @typedef	CzList<CzString*> CzXmlStringList

 @brief	A list of CzString's
 */
typedef CzList<CzString*> CzStringList;

/// @}

#endif	// __CZ_STRING_H__
