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

#if !defined(_CCZ_UI_LABEL_H_)
#define _CCZ_UI_LABEL_H_

#include "CzUIBase.h"
#include "CzActorText.h"


//
//
//
//
// CzUILabel - A text based actor that acts as a label
//
//
//
//
class CzUILabel : public CzUIBase
{
	// Properties
protected:
	CzActorText*	TextActor;
	CzColour		TextColour;
	CzString		Text;
	CzColour		SelectedTextColour;
	CzString		SelectedText;
	CzColour		DisabledTextColour;
	CzString		DisabledText;
public:
	CzActorText*	getTextActor()									{ return TextActor; }
	void			setTextColour(const CzColour& colour);
	CzColour		getTextColour()	const							{ return TextColour; }
	void			setText(const char* text);
	CzString&		getText()										{ return Text; }
	void			setSelectedTextColour(const CzColour& colour);
	CzColour		getSelectedTextColour() const					{ return SelectedTextColour; }
	void			setSelectedText(const char* text);
	CzString&		getSelectedText()								{ return SelectedText; }
	void			setDisabledTextColour(const CzColour& colour);
	CzColour		getDisabledTextColour() const					{ return DisabledTextColour; }
	void			setDisabledText(const char* text);
	CzString&		getDisabledText()								{ return DisabledText; }
	void			setEnabled(bool enabled);
	bool			setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool			setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool			getProperty(unsigned int property_name, CzXomlProperty& prop);
	void			setIgnoreCamera(bool enable);
	// Properties end
protected:
	void			UpdateSizeFromText();
	bool			UpdateBinding(unsigned int property_name, CzXomlVariable* var);
public:
	CzUILabel();

	virtual bool	InitTextActor(CzFont* font, const CzIRect& rect, const CzString& text, bool auto_height = false, bool text_use_parent_opacity = false, bool filter = true);
	virtual bool	InitTextActor(CzFont* font, const CzIRect& rect, const CzString& text, int flags, eCzAlignH align_h, eCzAlignV align_v, const CzVec4& skew, const CzIRect& text_margin, 
					bool auto_height, bool text_use_parent_opacity = false, bool filter = true);
	void			SetFromBrush(IzBrush* brush, bool resize = false);

	bool			Update(float dt);

	// Event handlers

	// Implementation of IzXomlClass interface
	int				LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Implementation of IzAnimTarget interface
	bool			UpdateFromAnimation(CzAnimInstance *animation);

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	LabelClassDef;								// XOML class definition
public:
	static void				InitClass();
	static void				ReleaseClass();

	static bool				_setText(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getText(IzXomlResource* target);
	static bool				_setSelectedText(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSelectedText(IzXomlResource* target);
	static bool				_setDisabledText(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getDisabledText(IzXomlResource* target);
	static bool				_setTextColour(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getTextColour(IzXomlResource* target);
	static bool				_setSelectedTextColour(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSelectedTextColour(IzXomlResource* target);
	static bool				_setDisabledTextColour(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getDisabledTextColour(IzXomlResource* target);
	static bool				_setAlignH(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setAlignV(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setWrap(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setTextSkew(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getTextSkew(IzXomlResource* target);
	static bool				_setTextMargin(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getTextMargin(IzXomlResource* target);
	static bool				_setRect(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getRect(IzXomlResource* target);
	static bool				_setAutoHeight(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getAutoHeight(IzXomlResource* target);
	static bool				_setTextFilter(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getTextFilter(IzXomlResource* target);
	static CzXomlProperty	_getActorText(IzXomlResource* target);
	static CzXomlProperty	_getTextSize(IzXomlResource* target);
};

//
//  CzUILabelCreator - Creates an instance of a label actor object
//
class CzUILabelCreator : public IzXomlClassCreator
{
public:
	CzUILabelCreator()
	{
		setClassName("label");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzUILabel(); }
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
#define CZ_NEW_LABEL(scene, label, name, text, background, font, size, auto_height)	\
	CzUILabel* label = new CzUILabel();												\
	label->setName(name);															\
	scene->addActor(label);															\
	label->Init(background, size.x, size.y);										\
	{																				\
		CzIRect rc = CzIRect(0, 0, -100, -100);										\
		label->InitTextActor(font, rc, text, auto_height);							\
	}

#endif	// _CCZ_UI_LABEL_H_
