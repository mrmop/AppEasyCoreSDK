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

#include "CzPlatformDisplay.h"


//
// Marmalade Display system
//
int		CzPlatformDisplay::Init(bool use_gl)
{
	// Initialise graphics system

	UseGL = use_gl;

	Initialised = true;

	return 1;
}

void	CzPlatformDisplay::Release()
{
	// Shutdown graphics system
}

int		CzPlatformDisplay::getCurrentWidth()
{
	return 0;
}

int		CzPlatformDisplay::getCurrentHeight()
{
	return 0;
}

void	CzPlatformDisplay::setColour(int r, int g, int b, int a)
{
}

void	CzPlatformDisplay::Clear()
{
	// Clear the screen
}

void	CzPlatformDisplay::Swap()
{
}

int		CzPlatformDisplay::getOrientation()
{
	return 0;
}
