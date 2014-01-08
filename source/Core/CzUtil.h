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

#if !defined(_CCZ_UTIL_H_)
#define _CCZ_UTIL_H_

#include "CzTypes.h"
#include "CzDebug.h"

class CzApp;

/**
 @addtogroup Core
 @{
 */
 
#ifndef NULL
	#define NULL 0
#endif

/**
 @def	SAFE_DELETE(x) if (x != NULL)

 @brief	A macro that safely deletes an object.

 @param	x	the object to delete.
 */

#define SAFE_DELETE(x) if (x != NULL) { delete x; x = NULL; }

/**
 @def	SAFE_DELETE_ARRAY(x) if (x != NULL)

 @brief	A macro that safely deletes an array of objects.

 @param	x	The rray of objects to delete.
 */

#define SAFE_DELETE_ARRAY(x) if (x != NULL) { delete [] x; x = NULL; }

/**
 @def	CZ_HASH(x) (CzString::CalculateHash(x));

 @brief	A macro that calculates the hash of a string.

 @param	x	The string.
 */

#define	CZ_HASH(x)				(CzString::CalculateHash(x))

/**
 @def	FRAME_SPEED_LOCK_MS();

 @brief	A macro that defines the frame speed lock in milliseconds.

 This value is calculates from 1000 / target_frame-rate 

 */

#define	FRAME_SPEED_LOCK_MS		16.67f

//
//
// SINGLETONS
//
//
//
// Define a class as a singleton (Add to class definition in header file)
//
#define CDEFINE_SINGLETON(the_class)				\
private:										\
	static the_class* _instance;				\
	the_class() {}								\
	~the_class() {}								\
	the_class(const the_class &);				\
	the_class& operator=(const the_class &);	\
public:											\
	static void Create();						\
	static void Destroy();						\
	static the_class* getInstance();

//
// Declare singleton methods (Add to source file)
//
#define CDECLARE_SINGLETON(the_class)		\
the_class* the_class::_instance = NULL;		\
void the_class::Create()					\
{											\
	if (_instance == NULL)					\
		_instance = new the_class;			\
}											\
void the_class::Destroy()					\
{											\
	if (_instance != NULL)					\
	{										\
		delete _instance;					\
		_instance = NULL;					\
	}										\
}											\
the_class* the_class::getInstance()			\
{											\
  return _instance;							\
}


//
//
//
//
// CzCallback - Game callback type
//
//
//
//
typedef int (*CzCallback)(void* caller, void* data);
typedef void* CzCallbackData;

/**
 @class	CzUtils

 @brief	CzUtils - Pure static utility class.

 */

class CzUtils
{
public:

public:
	static const char*	GetGraphicModeName(int width, int height);
	static int			GetGraphicModeIndex(int width, int height);
	static int			CountDigits(int number);


};

/**
 @class	CzMetrics

 @brief	CzMetrics - Metrics class.

 Used to gather certain metrics

 */

class CzMetrics
{
public:
	static int		TotalSpritesProcessed;			///< Total sprites that were processed
	static int		TotalSpritesCreated;			///< Total sprites that were created
	static int		TotalSpritesDestroyed;			///< Total sprites that were destroyed
	static int		TotalActorsProcessed;			///< Total actors that were processed
	static int		TotalActorsCreated;				///< Total actors that were created
	static int		TotalActorsDestroyed;			///< Total actors that were destroyed

};

/**
 @class	CzGlobals

 @brief	CzGlobals - Global data.

 */

class CzGlobals
{
public:
	static CzApp*		App;		///< The app
};

//#define CZ_ENABLE_METRICS

/// @}

#endif	// _CCZ_UTIL_H_
