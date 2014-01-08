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

#if !defined(_CCZ_FONT_H_)
#define _CCZ_FONT_H_

#include "CzUtil.h"
#include "CzXoml.h"
#include "IzPlatformFont.h"

class CzXomlResourceManager;

//
//
//
//
// CzFont - Represents a text rendering font
//
//
//
//
class CzFont : public IzXomlResource
{
public:
	enum eState
	{
		State_Invalid, 
		State_Loading, 
		State_Loaded, 
	};

	// Properties
protected:
	eState					State;				// State of image
	CzFontHandle			Font;				// Font handle
	float					PointSize;			// Point size of font
	CzString				Filename;			// Name of font file
	CzFont*					UsingFont;			// Points to a font that this font is re-using
	uint8*					FontBuffer;			// Holds loaded TTF font data
	uint					FontBufferSize;		// Size of TTF font data
public:
	eState					getState() const						{ return State; }
	CzFontHandle			getFontHandle();
	void					setPointSize(float size)				{ PointSize = size; }
	float					getPointSize() const					{ return PointSize; }
	CzFont*					getUsingFont()							{ return UsingFont; }
	void					setFilename(const char* filename)		{ Filename = filename; }
	CzString&				getFilename()							{ return Filename; }
	bool					isFontLoaded() const					{ return Font != NULL; }
	uint8*					getFontBuffer()							{ return FontBuffer; }
	uint					getFontBufferSize()						{ return FontBufferSize; }
	// Properties End
protected:
	CzFile*					File;				// File object (if font is file based)
	bool					Delete;

public:
	CzFont() : IzXomlResource(), Font(NULL), UsingFont(NULL), File(NULL), State(State_Invalid), PointSize(8), Delete(false), FontBuffer(NULL), FontBufferSize(0)		{ setClassType("font"); }
	virtual ~CzFont();

	bool			Load(bool blocking = true);						// Force load the image

	// Implementation of IzXomlClass interface
	int				LoadFromXoml(IzXomlResource* parebt, bool load_children, CzXmlNode* node);

	// Prepeared text (optimised for render)
	CzFontPreparedText	CreatePreparedText();

	// Internal
	void			FinishLoad();									// Called back when aysnc loading is completed
};

//
//  CzFontCreator - Creates an instance of a font object
//
class CzFontCreator : public IzXomlClassCreator
{
public:
	CzFontCreator()
	{
		setClassName("font");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzFont(); }
};

//
//
//
//
//  Helper macros
//
//
//
//

#define CZ_NEW_FONT(font, name, location, point_size)		\
	CzFont* font = new CzFont();							\
	font->setName("serif");									\
	font->setFilename(location);							\
	font->setPointSize(point_size);							\
	font->Load();




#endif	// _CCZ_FONT_H_
