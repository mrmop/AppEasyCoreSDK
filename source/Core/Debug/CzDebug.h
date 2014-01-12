// 
//
// AppEasy SDK - Cross Platform Multi-purpose Game and App Engine
//
// Developed by Matthew Hopwood of Pocketeers Limited - www.pocketeers.co.uk
//
// For updates, tutorials and more details check out www.appeasymobile.com
//
// This code is provided free of charge and without any warranty whatsoever. You must ensure that this whole notice is present in all files and derivatives, so the reader knows its origin.
// If you use this SDK in your product then please ensure that you credit AppEasy's usage appropriately. Please see http://www.appeasymobile.com for licensing details and support
//
//

#if !defined(_CCZ_DEBUG_H_)
#define _CCZ_DEBUG_H_

//#include "CzSlotArray.h"

/**
 @addtogroup Debug
 @{
 */
 
#define	CZ_DEBUG_TRACE_NONE		0
#define	CZ_DEBUG_TRACE_MIN		1
#define	CZ_DEBUG_TRACE_MAX		2
extern int g_CzShowTracing;

#define CZ_DEBUG_CHANNEL_NONE		0
#define CZ_DEBUG_CHANNEL_INFO		1
#define CZ_DEBUG_CHANNEL_WARNING	2
#define CZ_DEBUG_CHANNEL_ERROR		3

/**
 @class	CzDebug

 @brief	Error logging class.

 Error logging uses the concept of channels. A channel dtermines how the printed information will be prefixed. Currently available channels include:

 - CZ_DEBUG_CHANNEL_NONE - No specific channel, no prefix
 - CZ_DEBUG_CHANNEL_INFO - Informational message, prefixed with Info:
 - CZ_DEBUG_CHANNEL_WARNING - A warning, prefixed with Warning:
 - CZ_DEBUG_CHANNEL_ERROR - An error, prefixed with Error:

 You can add your own prefixes by updating g_CzChannelName

 */

class CzDebug
{
public:
	static void Init();
	static void Log(int channel, const char* message, const char* data = NULL);
	static void	Log(int channel, const char* message, const char* data1, const char* data2);
	static void Log(int channel, const char* message, const char* file, int line);
	static void Log(int channel, const char* message, const char* data, const char* file, int line);
};

/// @}


#endif	// _CCZ_DEBUG_H_
