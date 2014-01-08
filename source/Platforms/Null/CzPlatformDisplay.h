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

#if !defined(_CCZ_PLATFORM_DISPLAY_H_)
#define _CCZ_PLATFORM_DISPLAY_H_

#include "IzPlatformDisplay.h"

class CzPlatformDisplay : public IzPlatformDisplay
{
	// Properties
protected:
	bool			UseGL;
public:
	// Properties end
public:
	// Init
	int				Init(bool use_gl);
	void			Release();

	// Parameter query
	int				getCurrentWidth();
	int				getCurrentHeight();
	int				getOrientation();

	// Display management
	void			setColour(int r, int g, int b, int a);
	void			Clear();
	void			Swap();

};


#endif	// _CCZ_PLATFORM_DISPLAY_H_
