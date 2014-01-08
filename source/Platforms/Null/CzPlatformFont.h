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

#if !defined(_CCZ_PLATFORM_FONT_H_)
#define _CCZ_PLATFORM_FONT_H_

#include "IzPlatformFont.h"

class CzPlatformFont : public IzPlatformFont
{
	// Properties
protected:
public:
	// Properties end
public:
	int		Init();
	void	Release();

	// Font creation / loading
	CzFontHandle	CreateTTFontFromBuffer(void* memory_buffer, int memory_buffer_size, float point_size);
	void			DestroyFont(CzFontHandle font);

	// Prepared text
	CzFontPreparedText	CreatePreparedText();								// Creates buffer to be used prepare text
	void			DestroyPreparedText(CzFontPreparedText data);
	bool			PrepareText(CzFontPreparedText prep, const char* text);	// Prepares rendering optimised text data using the current font
	int				GetTextHeight(CzFontPreparedText data);

	// General font methods
	void			SetCurrentFont(CzFontHandle font);
	void			SetCurrentFontFlags(eCzFontFlags flags);
	void			SetCurrentFontRect(const CzIRect& rc);
	void			SetCurrentFontAlignment(eCzAlignH horiz, eCzAlignV vert);
	
};


#endif	// _CCZ_PLATFORM_FONT_H_
