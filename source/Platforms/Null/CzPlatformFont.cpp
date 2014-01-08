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

#include "CzPlatformFont.h"

//
// Marmalade font system
//
int		CzPlatformFont::Init()
{
	// Init font system

	Initialised = true;

	return 1;
}

void	CzPlatformFont::Release()
{
	// Terminate font system
}

CzFontHandle CzPlatformFont::CreateTTFontFromBuffer(void* memory_buffer, int memory_buffer_size, float point_size)
{
	return (CzFontHandle)NULL;
}

void CzPlatformFont::DestroyFont(CzFontHandle font)
{
}

void CzPlatformFont::SetCurrentFont(CzFontHandle font)
{
}

void CzPlatformFont::SetCurrentFontFlags(eCzFontFlags flags)
{
}

void CzPlatformFont::SetCurrentFontRect(const CzIRect& rc)
{
}

void CzPlatformFont::SetCurrentFontAlignment(eCzAlignH horiz, eCzAlignV vert)
{
	switch (horiz)
	{
	case AlignH_Left:
		break;
	case AlignH_Centre:
		break;
	case AlignH_Right:
		break;
	}
	switch (vert)
	{
	case AlignH_Left:
		break;
	case AlignH_Centre:
		break;
	case AlignH_Right:
		break;
	}
}

CzFontPreparedText CzPlatformFont::CreatePreparedText()
{
	return (CzFontPreparedText)NULL;
}

void CzPlatformFont::DestroyPreparedText(CzFontPreparedText data)
{
}

bool CzPlatformFont::PrepareText(CzFontPreparedText prep, const char* text)
{
	return false;
}

int CzPlatformFont::GetTextHeight(CzFontPreparedText prep)
{
	return 0;
}



