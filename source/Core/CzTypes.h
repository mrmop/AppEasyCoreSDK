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

#if !defined(_CCZ_TYPES_H_)
#define _CCZ_TYPES_H_

#include <list>
#include <vector>

#include "CzRect.h"
#include "CzVec2.h"
#include "CzIVec2.h"
#include "CzSVec2.h"
#include "CzVec3.h"
#include "CzVec4.h"
#include "CzMatrix3.h"
#include "CzMatrix4.h"
#include "CzColour.h"

#define	CzList		std::list
#define	CzVector	std::vector

typedef CzList<char> CzCharList;
typedef CzList<int> CzIntList;
typedef CzList<float> CzFloatList;
typedef CzList<bool> CzBoolList;
typedef CzVector<char> CzCharVector;
typedef CzVector<int> CzIntVector;
typedef CzVector<float> CzFloatVector;
typedef CzVector<bool> CzBoolVector;

#if !defined(NULL)
#define NULL	0
#endif

/*typedef char			int8;
typedef unsigned char	uint8;
typedef short			int16;
typedef unsigned short	uint16;
typedef int				int32;
typedef unsigned int	uint32;
#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef __int64			int64;
typedef unsigned __int64 uint64;
#else
typedef long long int	int64;
typedef unsigned long long int uint64;
#endif*/

#if defined(_MSC_VER) || defined(__BORLANDC__)
   #define CZ_LITTLE_ENDIAN
#else
   #define CZ_BIG_ENDIAN
#endif

/**
 @enum	eCzOrientation

 @brief	Values that represent orientation.
 */
enum eCzOrientation
{
	Orientation_Horizontal, 
	Orientation_Vertical, 
};

/**
 @enum	eCzAlignH

 @brief	Values that represent horizontal alignment.
 */
enum eCzAlignH
{
	AlignH_Left, 
	AlignH_Centre, 
	AlignH_Right, 
};

/**
 @enum	eCzAlignV

 @brief	Values that represent vertical alignment.
 */
enum eCzAlignV
{
	AlignV_Top, 
	AlignV_Middle, 
	AlignV_Bottom, 
};

/**
 @enum	eCzAspectLock

 @brief	Values that represent aspect lock.
 */
enum eCzAspectLock
{
	AL_None,  
	AL_X,  
	AL_Y, 
};

typedef void*		CzFileHandle;
typedef void*		CzTexture;
typedef void*		CzFontHandle;
typedef void*		CzFontPreparedText;
typedef void*		CzSoundInst;
typedef void*		CzSoundSpec;
typedef void*		CzSoundData;
typedef void*		CzWebView;
typedef void*		CzFacebookRequest;



#endif	// _CCZ_TYPES_H_
