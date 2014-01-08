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

#if !defined(_CCZ_UI_STACKPANEL_H_)
#define _CCZ_UI_STACKPANEL_H_

#include "CzUIPanel.h"

//
//
//
//
// CzUIStackPanel - An image based game actor that acts as a stack panel, allowing other actors to be stacked inside it
//
//
//
//
class CzUIStackPanel : public CzUIPanel
{
	// Properties
protected:
	eCzOrientation			Orientation;			///< Stacking orientation
	eCzAlignH				AlignH;					///< Horizontal alignment of contained actors
	eCzAlignV				AlignV;					///< Vertical alignment of contained actors
public:
	void					setOrientation(eCzOrientation o)	{ Orientation = o; ResizeToContent(); }
	eCzOrientation			getOrientation() const				{ return Orientation; }
	void					setAlignH(eCzAlignH align)			{ AlignH = align; setLayoutDirty(true); }
	eCzAlignH				getAlignH() const					{ return AlignH; }
	void					setAlignV(eCzAlignV align)			{ AlignV = align; setLayoutDirty(true); }
	eCzAlignV				getAlignV() const					{ return AlignV; }
	bool					setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool					setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool					getProperty(unsigned int property_name, CzXomlProperty& prop);
	// Properties end
protected:
	bool					UpdateLayout();			// Updates the layout of the child items
	bool					UpdateBinding(unsigned int property_name, CzXomlVariable* var);
public:
	CzUIStackPanel() : CzUIPanel(), Orientation(Orientation_Horizontal), AlignH(AlignH_Centre), AlignV(AlignV_Middle)	{ setActualClassType("stackpanel"); }
	virtual ~CzUIStackPanel();

	// Implementation of IzXomlClass interface
	int						LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	void					ResizeToContent();

	// Implementation of IzAnimTarget interface
	bool					UpdateFromAnimation(CzAnimInstance *animation);

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	UIStackPanelClassDef;								// XOML class definition
public:
	static void				InitClass();
	static void				ReleaseClass();

	static bool				_setOrientation(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getOrientation(IzXomlResource* target);
	static bool				_setAlignH(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setAlignV(IzXomlResource* target, const CzXomlProperty& prop, bool add);
};

//
//  CzUIStackPanelCreator - Creates an instance of a stack panel actor object
//
class CzUIStackPanelCreator : public IzXomlClassCreator
{
public:
	CzUIStackPanelCreator()
	{
		setClassName("stackpanel");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzUIStackPanel(); }
};


#endif	// _CCZ_UI_STACKPANEL_H_
