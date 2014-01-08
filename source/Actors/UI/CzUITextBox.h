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

#if !defined(_CCZ_UI_TEXTBOX_H_)
#define _CCZ_UI_TEXTBOX_H_

#include "CzUIBase.h"
#include "CzUILabel.h"

//
//
//
//
// CzUITextBox - An image based actor that acts as a text box
//
//
//
//
class CzUITextBox : public CzUILabel
{
public:

	// Properties
protected:
	CzString				Prompt;								// Prompt to show to the user when asked for text input
	CzString				TargetVariable;						// Name of the variable that will be updated with the value entered
	CzInput::eInputTypeHint	TypeHint;							// Hint to which type of data user should enter
public:
	void					setPrompt(const char* prompt)				{ Prompt = prompt; }
	CzString&				getPrompt()									{ return Prompt; }
	void					setTargetVariable(const char* name)			{ TargetVariable = name; }
	CzString&				getTargetVariable()							{ return TargetVariable; }
	void					setTypeHint(CzInput::eInputTypeHint hint)	{ TypeHint = hint; }
	CzInput::eInputTypeHint	getTypeHint() const							{ return TypeHint; }
	bool					setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool					setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool					getProperty(unsigned int property_name, CzXomlProperty& prop);
	// Properties end
protected:
	bool					UpdateBinding(unsigned int property_name, CzXomlVariable* var);
public:
	CzUITextBox();

	// Event handlers
	virtual void			NotifyTapped(int index, int x, int y, bool allow_bubble);
	virtual void			NotifyTextChanged();

	// Implementation of IzXomlClass interface
	int						LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Implementation of IzAnimTarget interface
	bool					UpdateFromAnimation(CzAnimInstance *animation);

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	UITextBoxClassDef;								// XOML class definition
public:
	static void				InitClass();
	static void				ReleaseClass();
	static bool				_setPrompt(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getPrompt(IzXomlResource* target);
	static bool				_setVariable(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getVariable(IzXomlResource* target);
	static bool				_setTypeHint(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getTypeHint(IzXomlResource* target);
	static bool				_setOnTextChanged(IzXomlResource* target, const CzXomlProperty& prop, bool add);
};

//
//  CzUITextBoxCreator - Creates an instance of text box actor object
//
class CzUITextBoxCreator : public IzXomlClassCreator
{
public:
	CzUITextBoxCreator()
	{
		setClassName("textbox");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzUITextBox(); }
};







#endif	// _CCZ_UI_TEXTBOX_H_
