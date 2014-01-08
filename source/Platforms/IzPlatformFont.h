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

#if !defined(_CIZ_PLATFORM_FONT_H_)
#define _CIZ_PLATFORM_FONT_H_

#include "CzPlatform.h"

#define	PLATFORM_FONT		PLATFORM->getFont()	

enum eCzFontFlags
{
	CzFontFlags_None = 0, 
	CzFontFlags_NoWrap = 1, 
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IzPlatformFont
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IzPlatformFont
{
	// Properties
protected:
	bool		Initialised;
public:
	bool		isInitialised() const	{ return Initialised; }
	// Properties end
public:
	virtual int Init() = 0;
	virtual void Release() = 0;

	// Font creation / loading
	virtual CzFontHandle	CreateTTFontFromBuffer(void* memory_buffer, int memory_buffer_size, float point_size) = 0;
	virtual void			DestroyFont(CzFontHandle font) = 0;

	// Prepared text
	virtual CzFontPreparedText	CreatePreparedText() = 0;								// Creates buffer to be used to prepare text
	virtual void			DestroyPreparedText(CzFontPreparedText data) = 0;
	virtual bool			PrepareText(CzFontPreparedText prep, const char* text) = 0;	// Prepares rendering optimised text data using the current font
	virtual int				GetTextHeight(CzFontPreparedText data) = 0;
	virtual int				GetTextWidth(CzFontPreparedText data) = 0;

	// General font methods
	virtual void			SetCurrentFont(CzFontHandle font) = 0;
	virtual void			SetCurrentFontFlags(eCzFontFlags flags) = 0;
	virtual void			SetCurrentFontRect(const CzIRect& rc) = 0;
	virtual void			SetCurrentFontAlignment(eCzAlignH horiz, eCzAlignV vert) = 0;
	
};


#endif	// _CIZ_PLATFORM_FONT_H_
