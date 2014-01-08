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

#include "Marm/CzPlatformFont.h"
#include "IwGxFont.h"

//
// Marmalade font system
//
int		CzPlatformFont::Init()
{
	if (g_CzShowTracing > 1)
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Initialising IwGxFont");

	IwGxFontInit();

	Initialised = true;

	return 1;
}

void	CzPlatformFont::Release()
{
	IwGxFontTerminate();
}

CzFontHandle CzPlatformFont::CreateTTFontFromBuffer(void* memory_buffer, int memory_buffer_size, float point_size)
{
	CIwGxFont* font = IwGxFontCreateTTFontFromBuffer((uint8*)memory_buffer, memory_buffer_size, (int)point_size);

	return (CzFontHandle)font;
}

void CzPlatformFont::DestroyFont(CzFontHandle font)
{
	CIwGxFont* f = static_cast<CIwGxFont*>(font);

	delete f;
}

void CzPlatformFont::SetCurrentFont(CzFontHandle font)
{
	CIwGxFont* f = static_cast<CIwGxFont*>(font);

	IwGxFontSetFont(f);
}

void CzPlatformFont::SetCurrentFontFlags(eCzFontFlags flags)
{
	uint32 f = IW_GX_FONT_DEFAULT_F;

	if (flags & CzFontFlags_NoWrap)
		f |= IW_GX_FONT_NOWRAP_F;

	IwGxFontClearFlags(0xffffffff - IW_GX_FONT_DEFAULT_F);
	IwGxFontSetFlags(f);
}

void CzPlatformFont::SetCurrentFontRect(const CzIRect& rc)
{
	CIwRect r;
	r.x = (short)rc.x;
	r.y = (short)rc.y;
	r.w = (short)rc.w;
	r.h = (short)rc.h;
	IwGxFontSetRect(r);
}

void CzPlatformFont::SetCurrentFontAlignment(eCzAlignH horiz, eCzAlignV vert)
{
	switch (horiz)
	{
	case AlignH_Left:
		IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_LEFT);
		break;
	case AlignH_Centre:
		IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_CENTRE);
		break;
	case AlignH_Right:
		IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_RIGHT);
		break;
	}
	switch (vert)
	{
	case AlignH_Left:
		IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_TOP);
		break;
	case AlignH_Centre:
		IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_MIDDLE);
		break;
	case AlignH_Right:
		IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_BOTTOM);
		break;
	}
}

CzFontPreparedText CzPlatformFont::CreatePreparedText()
{
	CIwGxFontPreparedData* data = new CIwGxFontPreparedData();

	return (CzFontPreparedText)data;
}

void CzPlatformFont::DestroyPreparedText(CzFontPreparedText data)
{
	CIwGxFontPreparedData* d = static_cast<CIwGxFontPreparedData*>(data);
	if (d != NULL)
		delete d;
}

bool CzPlatformFont::PrepareText(CzFontPreparedText prep, const char* text)
{
	if (text == NULL)
		return false;
	CIwGxFontPreparedData* d = static_cast<CIwGxFontPreparedData*>(prep);
	if (d != NULL)
	{
		IwGxFontPrepareText(*d, text, strlen(text));
		return true;
	}

	return false;
}

int CzPlatformFont::GetTextHeight(CzFontPreparedText prep)
{
	CIwGxFontPreparedData* d = static_cast<CIwGxFontPreparedData*>(prep);
	if (d != NULL)
	{
		return d->GetHeight();
	}

	return 0;
}

int CzPlatformFont::GetTextWidth(CzFontPreparedText prep)
{
	CIwGxFontPreparedData* d = static_cast<CIwGxFontPreparedData*>(prep);
	if (d != NULL)
	{
		return d->GetWidth();
	}

	return 0;
}


