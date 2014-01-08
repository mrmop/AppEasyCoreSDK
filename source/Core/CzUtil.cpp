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

#include "IzPlatformDisplay.h"
#include "CzUtil.h"
#include "CzString.h"
#include "CzFile.h"
#include "CzApp.h"

/**
 @struct	CzGraphicsMode

 @brief	Represents a graphics mode.

 */
struct CzGraphicsMode
{
	const char* name;
	int			width;
	int			height;
};

/**
 @brief	List of graphics modes.
 */
static CzGraphicsMode g_CzUtils_GraphicsModeNames[] = 
{
	{ "QQVGA", 160, 120 },		// 0
	{ "HQVGA", 240, 160 },		// 1
	{ "QVGA", 320, 240 },		// 2
	{ "WQVGA", 400, 240 },		// 3
	{ "HVGA", 480, 320 },		// 4
	{ "VGA", 640, 480 },		// 5
	{ "WVGA", 800, 480 }, 		// 6
	{ "FWVGA", 854, 480 }, 		// 7
	{ "SVGA", 800, 600 }, 		// 8
	{ "DVGA", 960, 640 }, 		// 9
	{ "WSVGA", 1024, 576 }, 	// 10
	{ "WSVGA", 1024, 600 }, 	// 11
	{ "XVGA", 1024, 768 }, 		// 12
	{ "WXVGA", 1280, 768 }, 	// 13
	{ "XVGA+", 1152, 864 }, 	// 14
	{ "WXVGA+", 1140, 900 }, 	// 15
	{ "SXVGA", 1280, 1024 }, 	// 16
	{ "SXVGA+", 1400, 1050 }, 	// 17
	{ "WSXVGA+", 1680, 1050 }, 	// 18
	{ "UXVGA", 1600, 1200 }, 	// 19
	{ "WUXVGA", 1920 , 1200 }, 	// 20
	{ "QWXGA", 2048, 1152 }, 	// 21
	{ "QXGA", 2048, 1536 }, 	// 22
	{ "WQXGA", 2560, 1600 }, 	// 23
	{ "QSXGA", 2560, 2048 }, 	// 24
	{ "WQSXGA", 3200, 2048 }, 	// 25
	{ "QUXGA", 3200, 2400 }, 	// 26
	{ "WQUXGA", 3840, 2400 }, 	// 27
	{ "HXGA", 4096, 3072 }, 	// 28
	{ "WHXGA", 5120, 3200 }, 	// 29
	{ "HSXGA", 5120, 4096 }, 	// 30
	{ "WHSXGA", 6400, 4096 }, 	// 31
	{ "HUXGA", 6400, 4800 }, 	// 32
	{ "WHUXGA", 7680, 4800 }, 	// 33
	{ "nHD", 640, 360 }, 		// 34
	{ "qHD", 960, 540 }, 		// 35
	{ "WQHD", 2560, 1440 }, 	// 36
	{ "QFHD", 3840, 2160 }, 	// 37
};

/**
 @fn	const char* CzUtils::GetGraphicModeName(int width, int height)

 @brief	Gets graphic mode name from the supplied width and height.

 See g_CzUtils_GraphicsModeNames for list of available modes.

 @param	width 	The width.
 @param	height	The height.

 @return	null if it fails, else the graphic mode name.
 */

const char* CzUtils::GetGraphicModeName(int width, int height)
{
	for (int t = 0; t < 37; t++)
	{
		int w = g_CzUtils_GraphicsModeNames[t].width;
		int h = g_CzUtils_GraphicsModeNames[t].height;
		if ((width == w && height == h) || (width == h && height == w))
			return g_CzUtils_GraphicsModeNames[t].name;
	}

	return NULL;
}

/**
 @fn	int CzUtils::GetGraphicModeIndex(int width, int height)

 @brief	Gets graphic mode index from the supplied width and height.

 See g_CzUtils_GraphicsModeNames for list of available modes.

 @param	width 	The width.
 @param	height	The height.

 @return	The graphic mode index, -1 for none matched.
 */

int CzUtils::GetGraphicModeIndex(int width, int height)
{
	for (int t = 0; t < 37; t++)
	{
		int w = g_CzUtils_GraphicsModeNames[t].width;
		int h = g_CzUtils_GraphicsModeNames[t].height;
		if ((width == w && height == h) || (width == h && height == w))
			return t;
	}

	return -1;
}

/**
 @fn	int CzUtils::CountDigits(int number)

 @brief	Count how many didgits an integer has.

 @param	number	The number.

 @return	The total number of digits.
 */

int CzUtils::CountDigits(int number)
{
	int count = 0;
	if (number > 0)
	{
		if (number >= 1000000000)
			return 10;
		else
		if (number >= 100000000)
			return 9;
		else
		if (number >= 10000000)
			return 8;
		else
		if (number >= 1000000)
			return 7;
		else
		if (number >= 100000)
			return 6;
		else
		if (number >= 10000)
			return 5;
		else
		if (number >= 1000)
			return 4;
		else
		if (number >= 100)
			return 3;
		else
		if (number >= 10)
			return 2;
		else
			return 1;
	}
	else
	{
		if (number <= -1000000000)
			return 10;
		else
		if (number <= -100000000)
			return 9;
		else
		if (number <= -10000000)
			return 8;
		else
		if (number <= -1000000)
			return 7;
		else
		if (number <= -100000)
			return 6;
		else
		if (number <= -10000)
			return 5;
		else
		if (number <= -1000)
			return 4;
		else
		if (number <= -100)
			return 3;
		else
		if (number <= -10)
			return 2;
		else
			return 1;
	}
}


//
//
// CzMetrics implementation
//
//
int	CzMetrics::TotalSpritesProcessed = 0;
int	CzMetrics::TotalSpritesCreated = 0;
int	CzMetrics::TotalSpritesDestroyed = 0;
int	CzMetrics::TotalActorsProcessed = 0;
int	CzMetrics::TotalActorsCreated = 0;
int	CzMetrics::TotalActorsDestroyed = 0;

//
//
//
// CzGlobals implementation
//
//
//
CzApp*	CzGlobals::App = NULL;
