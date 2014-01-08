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

#if !defined(_CZ_ACTOR_TEXT_H_)
#define _CZ_ACTOR_TEXT_H_

#include "CzActor.h"


/**
 @addtogroup Actors
 @{
 */

/**
 @class	CzActorText

 @brief	A text based actor that can display text.

 Text actors can be created directly but are usually used in conjunction with some other actor. For example user interface controls usually host a text actor on an image actor that 
 represents the background to the user interface control. Lets take a look at text actor properties:
 - Font (font) - Name of font to use to draw the text
 - Rect (x, y, width, height) - The area that the text should be drawn inside of. If not provided then the area will be calculated  based on the screen size or the parent actor
 - Text (string) - String to display
 - AlignH (centre, left, right) - Horizontal alignment, default is centre
 - AlignV (middle, top, bottom) - Vertical alignment, default is middle
 - Wrap (boolean) - If true then text is wrapped onto next line if too long to fit on one line, if not then text will overhand its container
 - Skew (top, bottom, left, right) - Four parameter skewing, which allows the actor to be skewed in four different directions
 - BeforeChildren (boolean) - When set to true this actor will be rendered before its children, otherwise it will be rendered afterwards (default is true)
 - Filter (boolean) - When set to true this actor will rendered using filtering (default is true)
 - AlphaMode (alpha_mode) - Sets the mode to use when mixing transparency (alpha) from the actor. AlphaMode can be one of none, half, add, sub and blend (default mode is blend)
 - AutoHeight (boolean) - When set to true the height of the text actor will be recalculated to make it large enough to fit its text content. For example, if you set the original 
 Rect to only hold a single line but the text takes 3 lines then the actor will be resized to fit all 3 lines of text (default is false).
 - Anchor (topleft or centre) - Sets the draw anchor (topleft causes the actor to be displayed relative to its top-left cornr whilst centre will use the actors centre)
 
 Notes:
 - A font must be specified
 - Because a text actor inherits from a basic actor, it inherits all of the basic actors properties as well as those properties shown above. 
 
 Now lets take a quick look at a text actor  definition in XOML:

 @pat XOML Example
  @code
<ActorText Font="serif" Text="Hello World" Position="10, 20" />
  @endcode

*/

class CzActorText : public CzActor
{
public:
	// Properties
protected:
	bool				AutoHeight;					///< When true the contained text will decide the height of the text display rect
	bool				MarginIsPadding;			///< if true then the margin parameter will be used as padding instead
public:
	void				setText(const char* text);
	const char*			getText() const;
	void				setRect(const CzIRect& rect);
	CzIRect				getRect() const;
	void				setFlags(eCzFontFlags flags);
	void				setFont(CzFont* font);
	CzFont*				getFont();
	void				setAlignH(eCzAlignH align);
	void				setAlignV(eCzAlignV align);
	void				setMarginIsPadding(bool enable)				{ MarginIsPadding = true; }
	bool				setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool				setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool				getProperty(unsigned int property_name, CzXomlProperty& prop);
	void				setAutoHeight(bool enable)					{ AutoHeight = enable; recalculateHeight(); }
	bool				isAutoHeight() const						{ return AutoHeight; }
	bool				isFontLoaded() const						{ return FontLoaded; }
	// Properties end
protected:
	CzIRect				OriginallRect;
	bool				FontLoaded;
	bool				UpdateBinding(unsigned int property_name, CzXomlVariable* var);
	void				recalculateHeight();

public:
	CzActorText() : CzActor(), AutoHeight(false), FontLoaded(false), MarginIsPadding(false)			{ setActualClassType("actortext"); IsTappable = false; }
	virtual ~CzActorText()			{}

	virtual bool		Init(CzFont* font, const CzIRect& rect, const CzString& text, int flags);
	virtual bool		UpdateVisual();

	// Event handlers
	virtual void		NotifyOrientationChange(CzScene::eOrientation old_orientation, CzScene::eOrientation new_orientation);

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Implementation of IzAnimTarget interface
	bool				UpdateFromAnimation(CzAnimInstance *animation);

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	ActorTextClassDef;								// XOML class definition

public:
	static void				InitClass();
	static void				ReleaseClass();

	static bool					_setText(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getText(IzXomlResource* target);
	static bool					_setFont(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getFont(IzXomlResource* target);
	static bool					_setRect(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getRect(IzXomlResource* target);
	static bool					_setAlignH(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setAlignV(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setWrap(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool					_setSkew(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getSkew(IzXomlResource* target);
	static bool					_setBeforeChildren(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getBeforeChildren(IzXomlResource* target);
	static bool					_setFilter(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getFilter(IzXomlResource* target);
	static bool					_setAutoHeight(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getAutoHeight(IzXomlResource* target);
	static bool					_setAlphaMode(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty		_getAlphaMode(IzXomlResource* target);
	static CzXomlProperty		_getTextSize(IzXomlResource* target);

};


/// @}

/**
 @class	CzActorTextCreator

 @brief	Creates an instance of a text based text actor object.

 Used by the XOML system to instantiate a CzActorText object.

 */

class CzActorTextCreator : public IzXomlClassCreator
{
public:
	CzActorTextCreator()
	{
		setClassName("actortext");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzActorText(); }
};

#endif // _CZ_ACTOR_TEXT_H_
