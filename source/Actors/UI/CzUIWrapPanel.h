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

#if !defined(_CCZ_UI_WRAPPANEL_H_)
#define _CCZ_UI_WRAPPANEL_H_

#include "CzUIPanel.h"

//
//
//
//
// CzUIWrapPanel - A panel that stacks content horizontally until no space left then moves down to the next line
//
//
//
//
class CzUIWrapPanel : public CzUIPanel
{
	// Properties
protected:
	eCzOrientation			Orientation;			///< Stacking orientation
public:
	void					setOrientation(eCzOrientation o)		{ Orientation = o; setLayoutDirty(true); }
	eCzOrientation			getOrientation() const					{ return Orientation; }
	bool					setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool					setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool					getProperty(unsigned int property_name, CzXomlProperty& prop);
	// Properties end
protected:
	bool					UpdateLayout();			// Updates the layout of the child items
	bool					UpdateBinding(unsigned int property_name, CzXomlVariable* var);
public:
	CzUIWrapPanel() : CzUIPanel(), Orientation(Orientation_Horizontal) 	{ setActualClassType("wrappanel"); }
	virtual ~CzUIWrapPanel();

	bool					Update(float dt);

	// Implementation of IzXomlClass interface
	int						LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Implementation of IzAnimTarget interface
	bool					UpdateFromAnimation(CzAnimInstance *animation);

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	UIWrapPanelClassDef;								// XOML class definition
public:
	static void				InitClass();
	static void				ReleaseClass();

	static bool				_setOrientation(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getOrientation(IzXomlResource* target);
};

//
//  CzUIWrapPanelCreator - Creates an instance of a wrap panel actor object
//
class CzUIWrapPanelCreator : public IzXomlClassCreator
{
public:
	CzUIWrapPanelCreator()
	{
		setClassName("wrappanel");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzUIWrapPanel(); }
};



#endif	// _CCZ_UI_WRAPPANEL_H_
