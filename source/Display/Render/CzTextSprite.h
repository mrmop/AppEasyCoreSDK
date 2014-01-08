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

#if !defined(_CCZ_TEXT_SPRITE_H_)
#define _CCZ_TEXT_SPRITE_H_

#include "CzSprite.h"

//
//
//
// CzTextSprite - A CzTextSprite is a text based visual representation of an on screen game object
//
//
//
class CzTextSprite : public CzSprite
{
	// Properties
protected:
	CzString			Text;					// Text to display
	CzIRect				Rect;					// Area where to draw the text
	CzFont*				Font;					// Font
	bool				TextDirty;				// Dirty when text has changed
	eCzFontFlags		Flags;					// Font flags
	eCzAlignH			AlignH;					// Horiznotal alignment
	eCzAlignV			AlignV;					// Vertical alignment
public:
	void		setText(const char* text)
	{
		if (Text.getHash() != CZ_HASH(text))
		{
			Text = text;
			TextDirty = true;
		}
	}
	CzString&		getText()										{ return Text; }
	void			setRect(const CzIRect& rect)					{ Rect = rect; TransformDirty = true; TextDirty = true; }
	CzIRect&		getRect() 										{ return Rect; }
	void			setFont(CzFont* font)
	{
		Font = font;
		TextDirty = true;
	}
	CzFont*			getFont()										{ return Font; }
	void			setFlags(eCzFontFlags flags)					{ Flags = flags; }
	void			setAlignH(eCzAlignH align)						{ AlignH = align; }
	void			setAlignV(eCzAlignV align)						{ AlignV = align; }
	CzFontPreparedText getPreparedData()							{ return PreparedText; }
	// Properties End

protected:
	CzFontPreparedText PreparedText;
	void			RebuildTransform();
	void			UpdateBindings();
	void			TransformVertices();

public:

	CzTextSprite() : CzSprite(), Font(NULL), Flags(CzFontFlags_None), AlignH(AlignH_Centre), AlignV(AlignV_Middle), TextDirty(true), Rect(-128, -128, 256, 256), PreparedText(NULL)   { setSpriteType(ST_Text); }
	virtual ~CzTextSprite();
	
	void			Init(int vertex_count = 4);
	void			Draw();

	void			RebuildText();
};





#endif // _CCZ_TEXT_SPRITE_H_
