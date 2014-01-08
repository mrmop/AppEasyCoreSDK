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

#if !defined(_CCZ_COLOUR_H_)
#define _CCZ_COLOUR_H_

#include "CzUtil.h"

/**
 @addtogroup Core
 @{
 */
 
/**
 @struct	CzColour

 @brief	Structure used to represent colour.

 */

struct CzColour
{
	// Properties
public:
	uint8   r;  ///< Red component
	uint8   g;  ///< Green component
	uint8   b;  ///< Blue component
	uint8   a;  ///< Alpha component
	// Properties End
public:
	CzColour()
	{
		r = g = b = a = 0;
	}
	CzColour(uint8 r, uint8 g, uint8 b, uint8 a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	CzColour(uint32 rgba)
	{
#ifdef CZ_LITTLE_ENDIAN
		r = rgba >> 24;
		g = (rgba >> 16) & 0xff;
		b = (rgba >> 8) & 0xff;
		a = rgba & 0xff;
#else
		r = rgba & 0xff;
		g = (rgba >> 8) & 0xff;
		b = (rgba >> 16) & 0xff;
		a = (rgba >> 24) & 0xff;
#endif
	}
	
	void set(const CzColour& c)
	{
		r = c.r;
		g = c.g;
		b = c.b;
		a = c.a;
	}
	void set(uint8 r, uint8 g, uint8 b, uint8 a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	void set(uint32 rgba)
	{
#ifdef CZ_LITTLE_ENDIAN
		r = rgba >> 24;
		g = (rgba >> 16) & 0xff;
		b = (rgba >> 8) & 0xff;
		a = rgba & 0xff;
#else
		r = rgba & 0xff;
		g = (rgba >> 8) & 0xff;
		b = (rgba >> 16) & 0xff;
		a = (rgba >> 24) & 0xff;
#endif
	}

	uint32 get() const
	{
#ifdef CZ_LITTLE_ENDIAN
		return  (uint32)a | ((uint32)b << 8) | ((uint32)g << 16) | ((uint32)r << 24);
#else
		return  (uint32)r | ((uint32)g << 8) | ((uint32)b << 16) | ((uint32)a << 24);
#endif
	}


};

/// @}

#endif	// _CCZ_COLOUR_H_
