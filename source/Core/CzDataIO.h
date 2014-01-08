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

#if !defined(_CZ_DATAIO_H_)
#define _CZ_DATAIO_H_

#include "CzUtil.h" 
#include "CzString.h" 

/**
 @addtogroup Core
 @{
 */

/**
 @class	CzDataInput

 @brief	Provides stream like read access to a memory buffer.

 From the perspective of AppEasy a stream is data that can be read from or written to. When data is read from a stream, the position at which we read the next lot of data moves to the end of 
 the previously read data. Streams have a stream pointer / position which marks the place where new data will be read or written.

 AppEasy provides two simple classes that allow you to treat memory as though it was a stream. This can come in very useful when serialising binary data or parsing data located in a memory 
 buffer.

 Two classes are provided which provide support for input and output:
 - CzDataInput - Input stream access to a a memory buffer
 - CzDataOutput - Output stream access to a a memory buffer

 Lets take a look at an example showing how to use :

 @code
// Create and initialise an input stream of 1024 bytes
CzDataInput *stream  = new CzDataInput();
stream->Init(1024);

// Read some data into the stream
if (!file->Read(stream->getData(), size))
	return -1;

// Do something with the data
while (!in->isEOF())
{
	char b = getByte();
}
 @endcode

 */

class CzDataInput
{
	// Properties
protected:
	char* 			Data;					///< Streams data
	int				Position;				///< Current position in stream
	int				Length;					///< Length of stream (actual allocated memory sie is aligned to 32 bytes)
	bool			EndOfFile;				///< true if end of stream reached
	bool			Managed;				///< If true data buffer will not be released when object deleted
public:
	// Properties End
	
private:
public:
	CzDataInput(bool managed) : Data(NULL), Position(0), Length(0), EndOfFile(false), Managed(managed) {}
	CzDataInput() : Data(NULL), Position(0), Length(0), EndOfFile(false), Managed(false) {}
	virtual ~CzDataInput() { Release(); }

	bool	Init(int length);
	virtual void	Release();

	char*	getData() { return Data; }
	void	setData(char* data);
	void	setData(char* data, int length);
	void	setManaged(bool managed)	{ Managed = managed; }
	bool	isManaged() const			{ return Managed; }
	int		Skip(int num_bytes);
	int		setPos(int pos);
	int		getPos() const			{ return Position; }
	int		getLength() const		{ return Length; }
	bool	isEOF() const			{ return EndOfFile; }
	int		Remaining() const		{ return Length - Position;	}
	char	getByte();
	float	getByteAsFloat();
	unsigned char	getUByte();
	int		getUByteAsInt();
	float	getUByteAsFloat();
	int		getBytes(char* data, int count);
	int		getBytes(short* data, int count);
	int		getBytes(int* data, int count);
	int		getBytes(float* data, int count);
	int		getUBytes(unsigned char* data, int count);
	int		getUBytes(int* data, int count);
	int		getUBytes(float* data, int count);
	int		getChars(char* data, int count);
	short	getShort();
	short	getShortSwab();
	float	getShortAsFloat();
	int		getUShort();
	float	getUShortAsFloat();
	int		getShorts(short* data, int count);
	int		getShorts(int* data, int count);
	int		getShorts(float* data, int count);
	int		getUShorts(unsigned short* data, int count);
	int		getUShorts(int* data, int count);
	int		getUShorts(float* data, int count);
	int		getInt();
	int		getIntSwab();
	int		getInts(int* data, int count);
	int		getInts(float* data, int count);
	float	getFloat();
	bool	getNextString(CzString *pString);
	bool	getNextString(char *pString, int max_len);
	bool	getNextQuotedStringAsint(int *pNum);
	bool	getNextQuotedString(CzString *pString);
	bool	getNextQuotedString(char *pString, int max_len);
	bool	getNextOccuranceOf(char chr);
	int		getNextTag(char tag_start_char, char tag_end_char, int range, int &start_pos);
	int		getNextTagOrValue(char tag_start_char, char tag_end_char, int range, int &start_pos);
	int		FindString(const char* pString, int str_len) const;

	int		SkipToWhitespaceInTag();
	int		SkipToNoneWhitespaceInTag();
	int		SkipToCharInTag(char chr);
	
	int		getLineNumber(int pos) const;
	
	int		CalculateCheckSum();
	bool	StripXMLComments();
	bool	CountXmlEntities(int& tags, int& attributes);
};

/**
 @class	CzDataOutput

 @brief	Provides stream write like access to a memory buffer.

 Lets take a look at an example showing how to use this class:

 @code
// Create and initialise an input stream of 1024 bytes
CzDataOutput *stream  = new CzDataOutput();
stream->Init(1024);

// Write some data into the stream
stream->set('a');			// Write a character
stream->set(10);			// Write an integer
stream->set(1.2f);			// Write a floating point value
stream->set("Hello", 5);	// Write some text
 @endcode

 */

class CzDataOutput
{
protected:
	// Properties
	char* 			Data;					///< Streams data
	int				Position;				///< Current position in stream
	int				Length;					///< Length of stream (actual allocated memory sie is aligned to 32 bytes)
	bool			EndOfFile;				///< true if end of stream reached
	bool			Managed;				///< If true data buffer will not be released when object deleted
public:
	// Properties End
	
private:
public:
	CzDataOutput(bool managed) : Data(NULL), Position(0), Length(0), EndOfFile(false), Managed(managed) {}
	CzDataOutput() :	Data(NULL), Position(0), Length(0), EndOfFile(false), Managed(false) {}
	virtual ~CzDataOutput() { Release(); }

	bool	Init(int length);
	virtual void	Release();

	char*	getData() { return Data; }
	void	setData(char* data);
	void	setData(char* data, int length);
	void	setManaged(bool managed)	{ Managed = managed; }
	bool	isManaged() const			{ return Managed; }
	int		Skip(int num_bytes);
	int		setPos(int pos);
	int		getPos() const { return Position; }
	int		getLength() const { return Length; }
	bool	isEOF() const { return EndOfFile; }
	int		Remaining() const { return Length - Position; }
	void	set(char data);
	void	set(short data);
	void	set(int data);
	void	set(float data);
	void	set(char* data, int count);
	void	set(short* data, int count);
	void	set(int* data, int count);
	void	set(float* data, int count);
	void	set(CzString* pString);
	
	int		CalculateCheckSum();
};

/// @}

#endif	//_CZ_DATAIO_H_
