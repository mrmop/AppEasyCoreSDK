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

#if !defined(_CIZ_PLATFORM_DISPLAY_H_)
#define _CIZ_PLATFORM_DISPLAY_H_

#include "CzPlatform.h"

#define	PLATFORM_DISPLAY	PLATFORM->getDisplay()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IzPlatformDisplay
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IzPlatformDisplay
{
	// Properties
protected:
	bool		Initialised;
public:
	bool		isInitialised() const	{ return Initialised; }
	// Properties end
public:
	// Init
	virtual int		Init(bool use_gl) = 0;
	virtual void	Release() = 0;

	// Parameter query
	virtual int		getCurrentWidth() = 0;
	virtual int		getCurrentHeight() = 0;
	virtual int		getOrientation() = 0;

	// Display management
	virtual void	setColour(int r, int g, int b, int a) = 0;
	virtual void	Clear() = 0;
	virtual void	Swap() = 0;

	// Graphics mode query
};


#endif	// _CIZ_PLATFORM_DISPLAY_H_
