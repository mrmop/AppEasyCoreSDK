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

#include "Marm/CzPlatformDisplay.h"
#include "IwGx.h"
#include "IwGL.h"
#include "Iw2D.h"


//
// Marmalade Display system
//
int		CzPlatformDisplay::Init(bool use_gl)
{
	if (g_CzShowTracing > 1)
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising IwGx");

	IwGxInit();

	if (g_CzShowTracing > 1)
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising Iw2D");

	Iw2DInit();

	if (g_CzShowTracing > 1)
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Disabling mipmaps");

	Iw2DSetUseMipMapping(false);
//	IwGxMipMappingOff();

	if (use_gl)
	{
		if (g_CzShowTracing > 1)
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising IwGL");

		IwGLInit();
	}
	UseGL = use_gl;

	Initialised = true;

	return 1;
}

void	CzPlatformDisplay::Release()
{
	if (UseGL)
		IwGLTerminate();
	Iw2DTerminate();
	IwGxTerminate();
}

int		CzPlatformDisplay::getCurrentWidth()
{
	return IwGxGetScreenWidth();
}

int		CzPlatformDisplay::getCurrentHeight()
{
	return IwGxGetScreenHeight();
}

void	CzPlatformDisplay::setColour(int r, int g, int b, int a)
{
	IwGxSetColClear(r, g, b, a);
}

void	CzPlatformDisplay::Clear()
{
	// Clear the screen
	if (UseGL)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	else
		IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
}

void	CzPlatformDisplay::Swap()
{
	// Flush IwGx
	IwGxFlush();

	// Display the rendered frame
	if (UseGL)
		IwGLSwapBuffers();
	else
		IwGxSwapBuffers();
}

int		CzPlatformDisplay::getOrientation()
{
	IwGxScreenOrient orientation = IwGxGetScreenOrient();

	switch (orientation)
	{
	case IW_GX_ORIENT_90:
		return 90;
	case IW_GX_ORIENT_180:
		return 180;
	case IW_GX_ORIENT_270:
		return 270;
	}
	
	return 0;
}
