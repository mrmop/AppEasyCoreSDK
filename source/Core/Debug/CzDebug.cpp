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
#include "CzDebug.h"
#include "CzString.h"
#include "CzFile.h"

/**
 @def	_DEBUG_FILENAME_();

 @brief	A macro that defines the debug output filename.
 */
//#define _DEBUG_FILENAME_	"raw:///sdcard/debug.txt"
//#define _DEBUG_FILENAME_	"rst:///AppEasy/shared/documents/debug.txt"
#if defined(_MSC_VER)
#define _DEBUG_FILENAME_	"raw://debug.txt"
#endif

/**
 @brief	The level of debug tracing output to display.

 The followin lecvels are available:
 - 0 - No debug tracing at all
 - 1 - Minimal debug trace output
 - 2 - Maximum debug trace output (default)

 */
int g_CzShowTracing = 2;

/**
 @brief	The debug channel prefix names table.
 */
const char* g_CzChannelName[] = 
{
	"", 
	"Info: ", 
	"Warning: ", 
	"Error: ", 
};

/**
 @fn	void CzDebug::Init()

 @brief	Initialise the debug output system

 */

void CzDebug::Init()
{
	if (g_CzShowTracing == 0)
		return;
#if defined(_DEBUG_FILENAME_)
	CzString s = "AppEasy Debug Output";
	CzFileHandle file = PLATFORM_FILE->Open(_DEBUG_FILENAME_, "wb");
	if (file != NULL)
	{
		PLATFORM_FILE->Write(file, (void*)s.c_str(), s.getLength());
		PLATFORM_FILE->Close(file);
	}
#endif	// _DEBUG_FILENAME_
}

/**
 @fn	void CzDebug::Log(int channel, const char* message, const char* file, int line)

 @brief	Logs debug errors

 @param	channel	The channel.
 @param	message	The message.
 @param	file   	The file name where the info, error or warning took place.
 @param	line   	The line number wher ethe info, error or warning took place.
 */
void CzDebug::Log(int channel, const char* message, const char* file, int line)
{
	if (g_CzShowTracing == 0)
		return;

	if (message == NULL)
		return;

	CzString str;
	str.allocString(256);
	str = g_CzChannelName[channel];
	str += message;
	str += " - File: "; 
	str += file;
	str += " line "; 
	str += line;

	PLATFORM_SYS->ConsoleWrite(str.c_str());

#if defined(_DEBUG_FILENAME_)
	if (PLATFORM_FILE != NULL && str.getLength() > 0)
	{
		CzFileHandle file = PLATFORM_FILE->Open(_DEBUG_FILENAME_, "ab");
		if (file != NULL)
		{
			PLATFORM_FILE->Write(file, (void*)str.c_str(), str.getLength());
			PLATFORM_FILE->Write(file, (void*)"\r\n", 2);
			PLATFORM_FILE->Close(file);
		}
	}
#endif	// _DEBUG_FILENAME_
}

/**
 @fn	void CzDebug::Log(int channel, const char* message, const char* data)

 @brief	Logs debug errors

 @param	channel	The channel.
 @param	message	The message.
 @param	data   	The extra data to output.
 */
void CzDebug::Log(int channel, const char* message, const char* data)
{
	if (g_CzShowTracing == 0)
		return;

//	if (message == NULL || data == NULL)
//		return;

	CzString str;
	str.allocString(256);
	str = g_CzChannelName[channel];
	str += message;
	if (data != NULL)
		str += (char*)data;

	PLATFORM_SYS->ConsoleWrite(str.c_str());

#if defined(_DEBUG_FILENAME_)
	if (PLATFORM_FILE != NULL && str.getLength() > 0)
	{
		CzFileHandle file = PLATFORM_FILE->Open(_DEBUG_FILENAME_, "ab");
		if (file != NULL)
		{
			PLATFORM_FILE->Write(file, (void*)str.c_str(), str.getLength());
			PLATFORM_FILE->Write(file, (void*)"\r\n", 2);
			PLATFORM_FILE->Close(file);
		}
	}
#endif	// _DEBUG_FILENAME_
}

/**
 @fn	void CzDebug::Log(int channel, const char* message, const char* data1, const char* data2)

 @brief	Logs debug errors

 @param	channel	The channel.
 @param	message	The message.
 @param	data1   	The extra data to output.
 @param	data2   	The extra data to output.
 */
void CzDebug::Log(int channel, const char* message, const char* data1, const char* data2)
{
	if (g_CzShowTracing == 0)
		return;

//	if (message == NULL || data == NULL)
//		return;

	CzString str;
	str.allocString(256);
	str = g_CzChannelName[channel];
	str += message;
	if (data1 != NULL)
		str += (char*)data1;
	if (data2 != NULL)
		str += (char*)data2;

	PLATFORM_SYS->ConsoleWrite(str.c_str());

#if defined(_DEBUG_FILENAME_)
	if (PLATFORM_FILE != NULL && str.getLength() > 0)
	{
		CzFileHandle file = PLATFORM_FILE->Open(_DEBUG_FILENAME_, "ab");
		if (file != NULL)
		{
			PLATFORM_FILE->Write(file, (void*)str.c_str(), str.getLength());
			PLATFORM_FILE->Write(file, (void*)"\r\n", 2);
			PLATFORM_FILE->Close(file);
		}
	}
#endif	// _DEBUG_FILENAME_
}

/**
 @fn	void CzDebug::Log(int channel, const char* message, const char* data, const char* file, int line)

 @brief	Logs debug errors

 @param	channel	The channel.
 @param	message	The message.
 @param	data   	The extra data to output.
 @param	file   	The file name where the info, error or warning took place.
 @param	line   	The line number where the info, error or warning took place.
 */
void CzDebug::Log(int channel, const char* message, const char* data, const char* file, int line)
{
	if (g_CzShowTracing == 0)
		return;

//	if (message == NULL || data == NULL)
//		return;

	CzString str;
	str.allocString(256);
	str = g_CzChannelName[channel];
	str += message;
	if (data != NULL)
		str += (char*)data;
	str += " - File: "; 
	str += file;
	str += " line "; 
	str += line;

	PLATFORM_SYS->ConsoleWrite(str.c_str());

#if defined(_DEBUG_FILENAME_)
	if (PLATFORM_FILE != NULL && str.getLength() > 0)
	{
		CzFileHandle file = PLATFORM_FILE->Open(_DEBUG_FILENAME_, "ab");
		if (file != NULL)
		{
			PLATFORM_FILE->Write(file, (void*)str.c_str(), str.getLength());
			PLATFORM_FILE->Write(file, (void*)"\r\n", 2);
			PLATFORM_FILE->Close(file);
		}
	}
#endif	// _DEBUG_FILENAME_
}
