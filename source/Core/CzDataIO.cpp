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

#include "CzDataIO.h"

//
//
//
//
// CzDataInput implementation
//
//
//
//

/**
 @fn	bool CzDataInput::Init(int length)

 @brief	Initialises the stream object.

 Allocates a memory buffer of the specified length and sets the stream market to the start

 @param	length	The stream length.

 @return	true if it succeeds, false if it fails.
 */

bool CzDataInput::Init(int length)
{
	Data = new char[length];
	if (Data == NULL)
	{
		return false;
	}
	Position = 0;
	Length = length;
	EndOfFile = false;

	return true;
}

/**
 @fn	void CzDataInput::Release()

 @brief	Releases the stream object.

 Deletes the memory associated with the stream

 */

void CzDataInput::Release()
{
	if (Data != NULL && !Managed)
	{
		delete Data;
		Data = NULL;
	}
	EndOfFile = true;
	Length = 0;
	Position = 0;
}

/**
 @fn	void CzDataInput::setData(char* data)

 @brief	Sets the stream data.

 Sets the data stream, taking ownership of the supplied data. If the stream already has data assigned then it will be deleted.

 @param [in]	data	If non-null, the data stream.
 */

void CzDataInput::setData(char* data)
{
	Release();
	Data = data;
	Position = 0;
	EndOfFile = false;
}

/**
 @fn	void CzDataInput::setData(char* data, int length)

 @brief	Sets the stream data and its length.

 Sets the data stream, taking ownership of the supplied data. The length of the stream is also set to the supplied length. If the stream already has data assigned then it will be deleted.

 @param [in]	data	If non-null, the data.
 @param			length	The length of the data.
 */

void CzDataInput::setData(char* data, int length)
{
	Release();
	Data = data;
	Position = 0;
	Length = length;
	EndOfFile = false;
}

/**
 @fn	int CzDataInput::Skip(int num_bytes)

 @brief	Moves the stream pointer back and forth.

 Skips te supplied number of bytes in the stream, num_bytes can be negative. Attempted to skip past the start or end of teh stream will cause EOF.

 @param	num_bytes	Number of bytes.

 @return	Stream position.
 */

int CzDataInput::Skip(int num_bytes)
{
	if (EndOfFile)
		return 0;
		
	Position += num_bytes;
	if (Position < 0 || Position >= Length)
		EndOfFile = true;
		
	return Position;
}

/**
 @fn	int CzDataInput::setPos(int pos)

 @brief	Sets the stream position.

 @param	pos	The stream position.

 @return	Stream position.
 */

int CzDataInput::setPos(int pos)
{
	EndOfFile = false;
	Position = pos;
	
	if (Position < 0 || Position >= Length)
		EndOfFile = true;

	return Position;	
}

/**
 @fn	char CzDataInput::getByte()

 @brief	Gets the next byte from the stream.

 @return	The byte.
 */

char CzDataInput::getByte()
{
	if (EndOfFile)
		return 0;

	char data = (char)Data[Position];

	Position++;
	if (Position >= Length)
		EndOfFile = true;
		
	return data;
}

/**
 @fn	float CzDataInput::getByteAsFloat()

 @brief	Gets the next byte from the stream as a float.

 @return	The byte as float.
 */

float CzDataInput::getByteAsFloat()
{
	if (EndOfFile)
		return 0;

	float data = (float)Data[Position];

	Position++;
	if (Position >= Length)
		EndOfFile = true;
		
	return data;
}

/**
 @fn	char CzDataInput::getUByte()

 @brief	Gets the next byte from the stream as an unsigned byte.

 @return	The unsigned byte.
 */

unsigned char CzDataInput::getUByte()
{
	if (EndOfFile)
		return 0;

	char data = Data[Position];

	Position++;
	if (Position >= Length)
		EndOfFile = true;
		
	return data;
}

/**
 @fn	int CzDataInput::getUByteAsInt()

 @brief	Gets the next byte from the stream as an integer.

 @return	The byte as an int.
 */

int CzDataInput::getUByteAsInt()
{
	if (EndOfFile)
		return 0;

	int data = Data[Position];

	Position++;
	if (Position >= Length)
		EndOfFile = true;
		
	return data;
}

/**
 @fn	float CzDataInput::getUByteAsFloat()

 @brief	Gets the next byte from the stream as an unsigned float.

 @return	The byte as an unsigned float.
 */

float CzDataInput::getUByteAsFloat()
{
	if (EndOfFile)
		return 0;

	int data = (int)Data[Position];

	Position++;
	if (Position >= Length)
		EndOfFile = true;
		
	return (float)data;
}

/**
 @fn	int CzDataInput::getBytes(char *data, int count)

 @brief	Gets count bytes from the stream.

 @param [out]	data	If non-null, destination data.
 @param			count	Number of bytes to read.

 @return	The number of bytes read or 0 for EOF.
 */

int CzDataInput::getBytes(char *data, int count)
{
	if (EndOfFile)
		return 0;

	int rem = Remaining();

	if (count > rem)
	{
		count = rem;
		EndOfFile = true;
	}

	memcpy(data, (char*)&Data[Position], count);

	Position += count;
	if (Position >= Length)
		EndOfFile = true;

	return count;
}

/**
 @fn	int CzDataInput::getBytes(short* data, int count)

 @brief	Gets count bytes from the stream and writes them into a short buffer.

 @param [out]	data	If non-null, destination data.
 @param			count	Number of bytes to read.

 @return	The number of bytes read or 0 for EOF.
 */

int CzDataInput::getBytes(short* data, int count)
{
	if (EndOfFile)
		return 0;

	while (count != 0)
	{
		*data++ = Data[Position];

		Position++;
		if (Position >= Length)
		{
			EndOfFile = true;
			break;
		}
		
		count--;
	}
		
	return count;
}

/**
 @fn	int CzDataInput::getBytes(int* data, int count)

 @brief	Gets count bytes from the stream and writes them into an int buffer.

 @param [out]	data	If non-null, destination data.
 @param			count	Number of bytes to read.

 @return	The number of bytes read or 0 for EOF.
 */

int CzDataInput::getBytes(int* data, int count)
{
	if (EndOfFile)
		return 0;

	while (count != 0)
	{
		*data++ = Data[Position];

		Position++;
		if (Position >= Length)
		{
			EndOfFile = true;
			break;
		}
		
		count--;
	}
		
	return count;
}

/**
 @fn	int CzDataInput::getBytes(float* data, int count)

 @brief	Gets count bytes from the stream and writes them into a float buffer.

 @param [out]	data	If non-null, destination data.
 @param			count	Number of bytes to read.

 @return	The number of bytes read or 0 for EOF.
 */

int CzDataInput::getBytes(float* data, int count)
{
	if (EndOfFile)
		return 0;

	while (count != 0)
	{
		*data++ = (float)Data[Position];

		Position++;
		if (Position >= Length)
		{
			EndOfFile = true;
			break;
		}
		
		count--;
	}
		
	return count;
}

/**
 @fn	int CzDataInput::getUBytes(char *data, int count)

 @brief	Gets count bytes from the stream and writes them into a short buffer.

 @param [out]	data	If non-null, destination data.
 @param			count	Number of bytes to read.

 @return	The number of bytes read or 0 for EOF.
 */

int CzDataInput::getUBytes(unsigned char *data, int count)
{
	if (EndOfFile)
		return 0;

	int rem = Remaining();

	if (count > rem)
	{
		count = rem;
		EndOfFile = true;
	}

	memcpy(data, &Data[Position], count);

	Position += count;
	if (Position >= Length)
		EndOfFile = true;

	return count;
}

/**
 @fn	int CzDataInput::getUBytes(int* data, int count)

 @brief	Gets count unsigned bytes from the stream and writes them into an int buffer.

 @param [out]	data	If non-null, destination data.
 @param			count	Number of bytes to read.

 @return	The number of bytes read or 0 for EOF.
 */

int CzDataInput::getUBytes(int* data, int count)
{
	if (EndOfFile)
		return 0;

	while (count != 0)
	{
		*data++ = (int)Data[Position];

		Position++;
		if (Position >= Length)
		{
			EndOfFile = true;
			break;
		}
		
		count--;
	}
		
	return count;
}

/**
 @fn	int CzDataInput::getUBytes(float* data, int count)

 @brief	Gets count unsigned bytes from the stream and writes them into a float buffer.

 @param [out]	data	If non-null, destination data.
 @param			count	Number of bytes to read.

 @return	The number of bytes read or 0 for EOF.
 */

int CzDataInput::getUBytes(float* data, int count)
{
	if (EndOfFile)
		return 0;

	while (count != 0)
	{
		*data++ = (float)Data[Position];

		Position++;
		if (Position >= Length)
		{
			EndOfFile = true;
			break;
		}
		
		count--;
	}
		
	return count;
}

/**
 @fn	int CzDataInput::getChars(char* data, int count)

 @brief	Gets count chars from the stream and writes them into a char buffer.

 @param [out]	data	If non-null, destination data.
 @param			count	Number of bytes to read.

 @return	The number chars read or 0 for EOF.
 */

int CzDataInput::getChars(char* data, int count)
{
	if (EndOfFile)
		return 0;

	int i = 0;
	while (count != 0)
	{
		*data++ = (char)Data[Position];

		Position++;
		if (Position >= Length)
		{
			EndOfFile = true;
			break;
		}
		
		count--;
	}
		
	return count;
}

/**
 @fn	short CzDataInput::getShort()

 @brief	Gets the next short from the stream.

 @return	The short.
 */

short CzDataInput::getShort()
{
	if (EndOfFile)
		return 0;
		
	short *data = (short *)&Data[Position];
	
	Position += 2;
	if (Position >= Length)
		EndOfFile = true;

	return *data;
}

/**
 @fn	short CzDataInput::getShort()

 @brief	Gets the next short from the stream, swapping the byte order.

 @return	The short.
 */
 
short CzDataInput::getShortSwab()
{
	if (EndOfFile)
		return 0;
		
	int nData = (int)Data[Position + 1] << 8;
	nData |= (int)Data[Position + 0];

	Position += 2;
	if (Position >= Length)
		EndOfFile = true;

	return (short)nData;
}

/**
 @fn	float CzDataInput::getShortAsFloat()

 @brief	Gets the next short from the buffer as a float.

 @return	The short as a float.
 */

float CzDataInput::getShortAsFloat()
{
	if (EndOfFile)
		return 0;
		
	short *data = (short *)&Data[Position];
	
	Position += 2;
	if (Position >= Length)
		EndOfFile = true;

	return (float)*data;
}

/**
 @fn	int CzDataInput::getUShort()

 @brief	Gets the next unsigned short from the stream.

 @return	The unsigned short.
 */

int CzDataInput::getUShort()
{
	if (EndOfFile)
		return 0;
		
	short *data = (short *)&Data[Position];
	
	Position += 2;
	if (Position >= Length)
		EndOfFile = true;

	return (int)*data;
}

/**
 @fn	float CzDataInput::getUShortAsFloat()

 @brief	Gets the next unsigned short as a float.

 @return	The unsigned short as a float.
 */

float CzDataInput::getUShortAsFloat()
{
	if (EndOfFile)
		return 0;
		
	short *data = (short *)&Data[Position];
	
	Position += 2;
	if (Position >= Length)
		EndOfFile = true;

	return (float)*data;
}

/**
 @fn	int CzDataInput::getShorts(short* data, int count)

 @brief	Gets count shorts from the stream and writes them into a short buffer.

 @param [out]	data	If non-null, destination data.
 @param			count	Number of shorts to read.

 @return	The number of shorts read or 0 for EOF.
 */

int CzDataInput::getShorts(short* data, int count)
{
	if (EndOfFile)
		return 0;

	short *pSData = (short *)&Data[Position];
		
	while (count != 0)
	{
		*data++ = *pSData++;
		
		Position += 2;
		if (Position >= Length)
		{
			EndOfFile = true;
			break;
		}
		
		count--;
	}

	return count;
}

/**
 @fn	int CzDataInput::getShorts(int* data, int count)

 @brief	Gets count shorts from the stream and writes them into an int buffer.

 @param [out]	data	If non-null, destination data.
 @param			count	Number of shorts to read.

 @return	The number of shorts read or 0 for EOF.
 */

int CzDataInput::getShorts(int* data, int count)
{
	if (EndOfFile)
		return 0;

	int *pIData = (int *)&Data[Position];
		
	while (count != 0)
	{
		*data++ = *pIData++;
		
		Position += 4;
		if (Position >= Length)
		{
			EndOfFile = true;
			break;
		}
		
		count--;
	}

	return count;
}

/**
 @fn	int CzDataInput::getShorts(float* data, int count)

 @brief	Gets count shorts from the stream and writes them into a float buffer.

 @param [out]	data	If non-null, destination data.
 @param			count	Number of shorts to read.

 @return	The number of shorts read or 0 for EOF.
 */

int CzDataInput::getShorts(float* data, int count)
{
	if (EndOfFile)
		return 0;

	float *pFData = (float *)&Data[Position];
		
	while (count != 0)
	{
		*data++ = *pFData++;
		
		Position += 4;
		if (Position >= Length)
		{
			EndOfFile = true;
			break;
		}
		
		count--;
	}

	return count;
}

/**
 @fn	int CzDataInput::getUShorts(unsigned short* data, int count)

 @brief	Gets count unsigned shorts from the stream and writes them into an unsigned short buffer.

 @param [out]	data	If non-null, destination data.
 @param			count	Number of shorts to read.

 @return	The number of shorts read or 0 for EOF.
 */

int CzDataInput::getUShorts(unsigned short* data, int count)
{
	if (EndOfFile)
		return 0;

	short *pSData = (short *)&Data[Position];
		
	while (count != 0)
	{
		*data++ = *pSData++;
		
		Position += 2;
		if (Position >= Length)
		{
			EndOfFile = true;
			break;
		}
		
		count--;
	}

	return count;
}

/**
 @fn	int CzDataInput::getUShorts(int* data, int count)

 @brief	Gets count unsigned shorts from the stream and writes them into an int buffer.

 @param [out]	data	If non-null, destination data.
 @param			count	Number of shorts to read.

 @return	The number of shorts read or 0 for EOF.
 */

int CzDataInput::getUShorts(int* data, int count)
{
	if (EndOfFile)
		return 0;

	int *pIData = (int *)&Data[Position];
		
	while (count != 0)
	{
		*data++ = *pIData++;
		
		Position += 4;
		if (Position >= Length)
		{
			EndOfFile = true;
			break;
		}
		
		count--;
	}

	return count;
}

/**
 @fn	int CzDataInput::getUShorts(float* data, int count)

 @brief	Gets count unsigned shorts from the stream and writes them into a float buffer.

 @param [out]	data	If non-null, destination data.
 @param			count	Number of shorts to read.

 @return	The number of shorts read or 0 for EOF.
 */

int CzDataInput::getUShorts(float* data, int count)
{
	if (EndOfFile)
		return 0;

	float *pFData = (float *)&Data[Position];
		
	while (count != 0)
	{
		*data++ = *pFData++;
		
		Position += 4;
		if (Position >= Length)
		{
			EndOfFile = true;
			break;
		}
		
		count--;
	}

	return count;
}

/**
 @fn	int CzDataInput::getInt()

 @brief	Gets the next int from the stream.

 @return	The int.
 */

int CzDataInput::getInt()
{
	if (EndOfFile)
		return 0;
		
	int *data = (int *)&Data[Position];

	Position += 4;
	if (Position >= Length)
		EndOfFile = true;

	return *data;
}

/**
 @fn	int CzDataInput::getIntSwab()

 @brief	Gets the next int from the stream, swapping the byte order.

 @return	The int.
 */

int CzDataInput::getIntSwab()
{
	if (EndOfFile)
		return 0;
		
	int nData = (int)Data[Position + 3] << 24;
	nData |= (int)Data[Position + 2] << 16;
	nData |= (int)Data[Position + 1] << 8;
	nData |= (int)Data[Position + 0];

	Position += 4;
	if (Position >= Length)
		EndOfFile = true;

	return nData;
}

/**
 @fn	float CzDataInput::getFloat()

 @brief	Gets the next float from the stream.

 @return	The float.
 */

float CzDataInput::getFloat()
{
	float data;

	getUBytes((unsigned char*)&data, sizeof(data));

	return data;
}

/**
 @fn	int CzDataInput::getInts(int* data, int count)

 @brief	Gets count ints from the stream and writes them into an int buffer.

 @param [out]	data	If non-null, destination data.
 @param			count	Number of ints to read.

 @return	The number of ints read or 0 for EOF.
 */

int CzDataInput::getInts(int* data, int count)
{
	if (EndOfFile)
		return 0;

	int *pIData = (int *)&Data[Position];
		
	while (count != 0)
	{
		*data++ = *pIData++;
		
		Position += 4;
		if (Position >= Length)
		{
			EndOfFile = true;
			break;
		}
		
		count--;
	}

	return count;
}

/**
 @fn	int CzDataInput::getInts(float* data, int count)

 @brief	Gets count ints from the stream and writes them into a float buffer.

 @param [out]	data	If non-null, destination data.
 @param			count	Number of ints to read.

 @return	The number of ints read or 0 for EOF.
 */

int CzDataInput::getInts(float* data, int count)
{
	if (EndOfFile)
		return 0;

	int *pFData = (int *)&Data[Position];
		
	while (count != 0)
	{
		*data++ = (float)*pFData++;
		
		Position += 4;
		if (Position >= Length)
		{
			EndOfFile = true;
			break;
		}
		
		count--;
	}

	return count;
}

/**
 @fn	int CzDataInput::getNextQuotedStringAsint(int *pNum)

 @brief	Gets the the next quoted string as an integer.

 @param [out]	pNum	If non-null, address of integer variable.

 @return	true if it succeeds, false if it fails.
  */

bool CzDataInput::getNextQuotedStringAsint(int *pNum)
{
	CzString num;

	if (getNextQuotedString(&num))
	{
		*pNum = num.getAsInt();
		return true;
	}

	return false;
}

/**
 @fn	bool CzDataInput::getNextQuotedString(CzString *pString)

 @brief	Gets the next quoted string.

 @param [in,out]	pString	If non-null, the string.

 @return	true if it succeeds, false if it fails.
 */

bool CzDataInput::getNextQuotedString(CzString *pString)
{
	char buffer[2048];
	if (getNextQuotedString(buffer, 2048))
	{
		pString->Copy(buffer);
		return true;
	}

	return false;
}

/**
 @fn	bool CzDataInput::getNextQuotedString(char *pString, int max_len)

 @brief	Gets the next quoted string, searching max_len chars forward in the stream.

 @param [out]	pString	If non-null, the string.
 @param	max_len			   	Maximum chars to search forwards.

 @return	true if it succeeds, false if it fails.
 */

bool CzDataInput::getNextQuotedString(char *pString, int max_len)
{
	if (EndOfFile)
		return false;

	// Search for quote
	char*	data = (char *)&Data[Position];
	int		quote_count = 0;
	int		index = 0;

	do
	{
		char c = *data++;

//		if (c == 0 || c == '\n')
		if (c == 0)
			return false;	// Error if we ecounter null or newline

		if (c == '"')
		{
			quote_count++;
		}
		else
		{
			if (quote_count == 1)
				pString[index++] = c;

		}
		Position++;
		if (Position >= Length)
		{
			EndOfFile = true;
			return false;
		}
	}
	while (quote_count != 2 && index < max_len);

	if (quote_count != 2)
		return false;
	pString[index] = 0;

	return true;
}

/**
 @fn	bool CzDataInput::getNextString(CzString *pString)

 @brief	Gets the next string.

 @param [out]	pString	If non-null, the string.

 @return	true if it succeeds, false if it fails.
 */

bool CzDataInput::getNextString(CzString *pString)
{
	char buffer[2048];
	if (getNextString(buffer, 2048))
	{
		pString->Copy(buffer);
		return true;
	}
	
	return false;
}

/**
 @fn	bool CzDataInput::getNextString(char *pString, int max_len)

 @brief	Gets the next string, searching max_len chars forward in the stream.

 @param [out]	pString	If non-null, the string.
 @param	max_len			   	Maximum chars to search forwards.

 @return	true if it succeeds, false if it fails.
 */

bool CzDataInput::getNextString(char *pString, int max_len)
{
	if (EndOfFile)
		return false;

	char*	data = (char *)&Data[Position];
	int		index = 0;

	do
	{
		char c = *data++;

		Position++;
		if (Position >= Length)
			EndOfFile = true;
		
		if (c == 0 || c == '\n' || c == ' ' || c == '\t')
			break;
			
		pString[index++] = c;

		if (EndOfFile)
			return false;
	}
	while (index < max_len);
	pString[index] = 0;

	return true;
}

/**
 @fn	bool CzDataInput::getNextOccuranceOf(char chr)

 @brief	Skips teh stream to the next occurance of the specified character.

 @param	chr	The character to find.

 @return	true if it succeeds, false if it fails.
 */

bool CzDataInput::getNextOccuranceOf(char chr)
{
	if (EndOfFile)
		return false;

	char*	data = (char *)&Data[Position];

	do
	{
		char c = *data++;
		
		if (c == chr)
			return true;

		if (c == 0)
			return false;

		Position++;
		if (Position >= Length)
		{
			EndOfFile = true;
			return false;
		}
	}
	while (1);

	return false;
}

/**
 @fn	int CzDataInput::FindString(const char* pString, int str_len) const

 @brief	Searches forward for a string.

 @param	pString	The string to find.
 @param	str_len	The length of the string.

 @return	The length of the found string or < 0 for not found.
 */

int CzDataInput::FindString(const char* pString, int str_len) const
{
	char*	data = (char *)&Data[Position];
	int		char_index = 0;
	int		pos = Position;

	char c;
	do
	{
		c = *data++;

		if (c == *(pString + char_index))
		{
			char_index++;
			if (char_index >= str_len)
				return pos - str_len + 1;
		}
		else
			char_index = 0;

		pos++;
	}
	while (pos < Length && c != 0);

	return -1;
}

/**
 @fn	int CzDataInput::getNextTag(char tag_start_char, char tag_end_char, int range,
    int &start_pos)

 @brief	Gets the next tag marked by tag_start_char and tag_end_char tag chars.

 For example, if tag_start_char = "<" and tag_end_char = ">" then the next XML tag will be found.

 @param	tag_start_char		 	The tag start character.
 @param	tag_end_char		 	The tag end character.
 @param	range				 	Number of characters to search forwards.
 @param [in,out]	start_pos	Stream position where tag starts.

 @return	Length of tag or < 0 for not found.
 */

int CzDataInput::getNextTag(char tag_start_char, char tag_end_char, int range, int &start_pos)
{
	if (Position >= Length)
	{
		EndOfFile = true;
		return -1;
	}
	if (EndOfFile)
		return -1;

	char*	data = (char *)&Data[Position];
	int		index = 0;
	bool	found_first = false;
	int		num_open = 0;
	bool	tag_found = false;

	while (range-- > 0)
	{
		char c = *data++;

		if (c == 0)
		{
			if (!found_first)
				return -2;
			else
				return -1;
		}

		if (c == tag_start_char)
		{
			if (!found_first)
			{
				found_first = true;
				start_pos = Position;
				if (c == tag_start_char)
					tag_found = true;
			}
			else
				return -1;	// We came across a 2nd opening brace before a closing one
		}
		else
		if (c == tag_end_char)
		{
			if (tag_found)
			{
				Position++;
				return index + 1;
			}
			else
				return -1;	// We came across a closing brace before an opening one
		}
		if (found_first)
			index++;

		Position++;
		if (Position >= Length)
		{
			EndOfFile = true;
			if (!found_first)
				return -2;
			else
				return -1;
		}
	}

	if (!found_first)
		return -2;
	else
		return -1;
}

/**
 @fn	int CzDataInput::getNextTagOrValue(char tag_start_char, char tag_end_char, int range,
    int &start_pos)

 @brief	Gets the next tag marked by tag_start_char and tag_end_char tag chars or value.

 For example, if tag_start_char = "<" and tag_end_char = ">" then the next XML tag will be found.

 @param	tag_start_char		 	The tag start character.
 @param	tag_end_char		 	The tag end character.
 @param	range				 	Number of characters to search forwards.
 @param [in,out]	start_pos	Stream position where tag starts.

 @return	Length of tag or < 0 for not found.
 */

int CzDataInput::getNextTagOrValue(char tag_start_char, char tag_end_char, int range, int &start_pos)
{
	if (Position >= Length)
	{
		EndOfFile = true;
		return -1;
	}
	if (EndOfFile)
		return -1;

	char*	data = (char *)&Data[Position];
	int		index = 0;
	bool	found_first = false;
	int		num_open = 0;
	bool	tag_found = false;

	while (range-- > 0)
	{
		char c = *data++;

		if (c == 0)
		{
			if (!found_first)
				return -2;
			else
				return -1;
		}

		if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
		{
			if (!found_first)
			{
				found_first = true;
				start_pos = Position;
				if (c == tag_start_char)
					tag_found = true;
			}
			else
			{
				if (c == tag_end_char)
				{
					if (tag_found)
					{
						Position++;
						return index + 1;
					}
				}
				else
				if (c == tag_start_char)
				{
					if (tag_found)
						return -1;
					else
					{
//						Position++;
						return index;
					}

				}
			}
/*			if (c == tag_start_char)
			{
				num_open++;
				if (num_open > 1)
					return -1;
			}*/
		}
		if (found_first)
			index++;


		Position++;
		if (Position >= Length)
		{
			EndOfFile = true;
			if (!found_first)
				return -2;
			else
				return -1;
		}
	}

	if (!found_first)
		return -2;
	else
		return -1;
}

/**
 @fn	int CzDataInput::SkipToWhitespaceInTag()

 @brief	Skips stream to next available whitespace character in an XML tag.

 @return	Stream position of whitespace or < 0 not found.
 */

int CzDataInput::SkipToWhitespaceInTag()
{
	if (EndOfFile)
		return -1;

	char*	data = (char *)&Data[Position];

	do
	{
		char c = *data++;

		if (c == ' ' || c == '\t' || c == '=' || c == '\n')
			return Position;
//		if (c == 0 || c == '/' || c == '>')
		if (c == 0 || c == '>')
			return -1;

		Position++;
		if (Position >= Length)
		{
			EndOfFile = true;
			return -1;
		}

	}
	while (1);

	return -1;
}

/**
 @fn	int CzDataInput::SkipToNoneWhitespaceInTag()

 @brief	Skip stream to next none whitespace character in an XML tag.

 @return	Stream position of none whitespace or < 0 not found.
 */

int CzDataInput::SkipToNoneWhitespaceInTag()
{
	if (EndOfFile)
		return -1;

	char*	data = (char *)&Data[Position];

	do
	{
		char c = *data++;

		if (c == 0)
			return -1;

		if (c != ' ' && c != '\t' && c != '\n')
			return Position;

		Position++;
		if (Position >= Length)
		{
			EndOfFile = true;
			return -1;
		}

	}
	while (1);

	return -1;
}

/**
 @fn	int CzDataInput::SkipToCharInTag(char chr)

 @brief	Skip to character in tag.

 @param	chr	The character.

 @return	Stream position of character or < 0 not found.
 */

int CzDataInput::SkipToCharInTag(char chr)
{
	if (EndOfFile)
		return -1;

	char*	data = (char *)&Data[Position];

	do
	{
		char c = *data++;

//		if (c == 0 || c == '/' || c == '>')
		if (c == 0 || c == '>')
			return -1;

		if (c == chr)
			return Position;

		Position++;
		if (Position >= Length)
		{
			EndOfFile = true;
			return -1;
		}

	}
	while (1);

	return -1;
}

/**
 @fn	int CzDataInput::CalculateCheckSum()

 @brief	Calculates the check sum of teh strean.

 @return	The calculated check sum.
 */

int	CzDataInput::CalculateCheckSum()
{
	int checksum = 0;
	for (int t = 0; t < Position; t++)
	{
		checksum += Data[t];
	}
	
	return checksum;
}

/**
 @fn	int CzDataInput::getLineNumber(int pos) const

 @brief	Gets line number of the specified stream position (assumes the stream is text).

 @param	pos	The position.

 @return	The line number.
 */

int	CzDataInput::getLineNumber(int pos) const
{
	char* data = Data;
	int line = 0;
	
	while (pos-- != 0)
	{
		if (*data++ == '\n')
			line++;
	}
	
	return line + 1;
}

/**
 @fn	bool CzDataInput::StripXMLComments()

 @brief	Strip XML comments from the stream.

 @return	true if it succeeds, false if it fails.

 */

bool CzDataInput::StripXMLComments()
{
	if (Data == NULL)
		return false;

	char*	new_data = new char[Length + 1];
	char*	data2 = new_data;
	bool	changed = false;

	// Copy all text except commeents to new buffer
	char* data = Data;
	int len = Length;

	while (len-- > 0)
	{
		char c = *data++;
		bool end_commend = false;

		if (len > 6 && c == '<' && *data == '!' && *(data + 1) == '-' && *(data + 2) == '-')
		{
			data += 3;
			len -= 3; 
			if (len < 0)
				return false;
			// Found a comment, now search ahead to the end of the comment
			while (len-- > 0)
			{
				c = *data++;
				if (c == '-')
				{
					len--; if (len < 0) return false;
					c = *data++;
					if (c == '-')
					{
						len--; if (len < 0) return false;
						c = *data++;
						if (c == '>')
						{
							end_commend = true;
							changed = true;
							break;
						}
					}
				}
			}
			if (len <= 0)
				break;
		}
		if (!end_commend)
		{
			// Replace special chars
			if (c == '\\')
			{
				if (*data == 'n')
				{
					*data2++ = '\n';
					data++;
					len--;
				}
				else
					*data2++ = c;
			}
			else
				*data2++ = c;
		}
	}
	*data2 = 0;

	if (changed)
	{
		delete [] Data;
		Length = strlen(new_data) + 1;
		Data = new_data;
	}
	else
	{
		delete [] new_data;
	}

	return true;
}

/**
 @fn	int CzDataInput::CountXmlEntities(int& tags, int& attributes)

 @brief	Count the number of XML tags an attributes entities present in the stream.

 @param [out]	tags	  	The number of tags found.
 @param [out]	attributes	The number of attributes found.

 @return	The total number of XML entities.
 */

bool CzDataInput::CountXmlEntities(int& tags, int& attributes)
{
	if (Data == NULL)
		return false;

	tags = 0;
	attributes = 0;

	char* data = Data;
	int len = Length;

	int num_open = 0;
	int num_quotes = 0;

	while (len-- > 0)
	{
		char c = *data++;
		if (c == '<')
			tags++;
		else
		if (c == '"')
			attributes++;
	}
	attributes /= 2;

	return true;
}



//
//
//
//
// CzDataOutput implementation
//
//
//
//

/**
 @fn	bool CzDataOutput::Init(int length)

 @brief	Initialises the stream object.

 Allocates a memory buffer of the specified length and sets the stream market to the start

 @param	length	The stream length.

 @return	true if it succeeds, false if it fails.
 */

bool CzDataOutput::Init(int length)
{
	Data = new char[length];
	if (Data == NULL)
		return false;
	memset(Data, 0, length);
	Position = 0;
	Length = length;
	EndOfFile = false;

	return true;
}

/**
 @fn	void CzDataOutput::Release()

 @brief	Releases the stream object.

 Deletes the memory associated with the stream
*/

void CzDataOutput::Release()
{
	if (Data != NULL && !Managed)
	{
		delete Data;
		Data = NULL;
	}
	EndOfFile = true;
	Length = 0;
	Position = 0;
}

/**
 @fn	void CzDataOutput::setData(char* data)

 @brief	Sets the stream data.

 Sets the data stream, taking ownership of the supplied data. If the stream already has data assigned then it will be deleted.

 @param [in]	data	If non-null, the data stream.
 */

void CzDataOutput::setData(char* data)
{
	Release();
	Data = data;
	Position = 0;
	EndOfFile = false;
}

/**
 @fn	void CzDataOutput::setData(char* data, int length)

 @brief	Sets the stream data and its length.

 Sets the data stream, taking ownership of the supplied data. The length of the stream is also set to the supplied length. If the stream already has data assigned then it will be deleted.

 @param [in]	data	If non-null, the data.
 @param			length	The length of the data.

 */

void CzDataOutput::setData(char* data, int length)
{
	Release();
	Data = data;
	Position = 0;
	Length = length;
	EndOfFile = false;
}

/**
 @fn	int CzDataOutput::Skip(int num_bytes)

 @brief	Moves the stream pointer back and forth.

 Skips te supplied number of bytes in the stream, num_bytes can be negative. Attempted to skip past the start or end of teh stream will cause EOF.

 @param	num_bytes	Number of bytes.

 @return	Stream position.

 */

int CzDataOutput::Skip(int num_bytes)
{
	if (EndOfFile)
		return 0;
		
	Position += num_bytes;
	if (Position < 0 || Position >= Length)
		EndOfFile = true;
		
	return Position;
}

/**
 @fn	int CzDataOutput::setPos(int pos)

 @brief	Sets the stream position.

 @param	pos	Position to set stream position to.

 @return	Stream position.
 */

int CzDataOutput::setPos(int pos)
{
	EndOfFile = false;
	Position = pos;
	
	if (Position < 0 || Position >= Length)
		EndOfFile = true;

	return Position;	
}

/**
 @fn	void CzDataOutput::set(char data)

 @brief	Writes a char to the stream.

 @param	data	The data to write.

 */

void CzDataOutput::set(char data)
{
	Position++;
	if (Position >= Length)
		EndOfFile = true;
		
	if (EndOfFile)
	{
		Position--;
		return;
	}

	Data[Position - 1] = data;
}

/**
 @fn	void CzDataOutput::set(short data)

 @brief	Writes a short to the stream.

 @param	data	The data to write.

 */

void CzDataOutput::set(short data)
{
	Position += 2;
	if (Position >= Length)
		EndOfFile = true;
		
	if (EndOfFile)
	{
		Position -= 2;
		return;
	}

	short *buff = (short *)&Data[Position - 2];
	*buff = data;
}

/**
 @fn	void CzDataOutput::set(int data)

 @brief	Writes an int to the stream.

 @param	data	The data to write.

 */

void CzDataOutput::set(int data)
{
	Position += 4;
	if (Position >= Length)
		EndOfFile = true;
		
	if (EndOfFile)
	{
		Position -= 4;
		return;
	}

	int *buff = (int *)&Data[Position - 4];
	*buff = data;
}

/**
 @fn	void CzDataOutput::set(float data)

 @brief	Writes a float to the stream.

 @param	data	The data to write.

 */

void CzDataOutput::set(float data)
{
	Position += 4;
	if (Position >= Length)
		EndOfFile = true;
		
	if (EndOfFile)
	{
		Position -= 4;
		return;
	}

	float *buff = (float *)&Data[Position - 4];
	*buff = data;
}

/**
 @fn	void CzDataOutput::set(CzString *data)

 @brief	Writes a string to the stream.

 @param	data	The string to write.
 */

void CzDataOutput::set(CzString *data)
{
	int len = data->getLength();

	Position += len;
	if (Position >= Length)
		EndOfFile = true;
		
	if (EndOfFile)
	{
		Position -= len;
		return;
	}

	memcpy((char *)&Data[Position - len], (char *)data->c_str(), len);
}

/**
 @fn	void CzDataOutput::set(char* data, int count)

 @brief	Writes a char buffer to the stream.

 @param	data	The data to write.
 @param	count	Number of bytes to write.
 */

void CzDataOutput::set(char* data, int count)
{
	Position += count;
	if (Position >= Length)
		EndOfFile = true;
		
	if (EndOfFile)
	{
		Position -= count;
		return;
	}

	char *pDData = (char *)&Data[Position - count];
	for (int t = 0; t < count; t++)
	{
		*pDData++ = *data++;
	}
}

/**
 @fn	void CzDataOutput::set(short* data, int count)

 @brief	Writes a short buffer to the stream.

 @param	data	The data to write.
 @param	count	Number of shorts to write.
*/

void CzDataOutput::set(short* data, int count)
{
	Position += count << 1;
	if (Position >= Length)
		EndOfFile = true;
		
	if (EndOfFile)
	{
		Position -= count << 1;
		return;
	}

	short *pDData = (short *)&Data[Position - (count << 1)];
	for (int t = 0; t < count; t++)
	{
		*pDData++ = *data++;
	}
}

/**
 @fn	void CzDataOutput::set(int* data, int count)

 @brief	Writes an int buffer to the stream.

 @param	data	The data to write.
 @param	count	Number of ints to write.

 */

void CzDataOutput::set(int* data, int count)
{
	Position += count << 2;
	if (Position >= Length)
		EndOfFile = true;
		
	if (EndOfFile)
	{
		Position -= count << 2;
		return;
	}

	int *pDData = (int *)&Data[Position - (count << 2)];
	for (int t = 0; t < count; t++)
	{
		*pDData++ = *data++;
	}
}

/**
 @fn	void CzDataOutput::set(float* data, int count)

  @brief	Writes a float buffer to the stream.

 @param	data	The data to write.
 @param	count	Number of floats to write.

*/

void CzDataOutput::set(float* data, int count)
{
	Position += count << 2;
	if (Position >= Length)
		EndOfFile = true;
		
	if (EndOfFile)
	{
		Position -= count << 2;
		return;
	}

	float *pDData = (float *)&Data[Position - (count << 2)];
	for (int t = 0; t < count; t++)
	{
		*pDData++ = *data++;
	}
}

/**
 @fn	int CzDataOutput::CalculateCheckSum()

 @brief	Calculates the check sum of teh strean.

 @return	The calculated check sum.

 */

int	CzDataOutput::CalculateCheckSum()
{
	int checksum = 0;
	for (int t = 0; t < Position; t++)
	{
		checksum += Data[t];
	}
	
	return checksum;
}

