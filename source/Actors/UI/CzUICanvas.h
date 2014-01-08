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

#if !defined(_CCZ_UI_CANVAS_H_)
#define _CCZ_UI_CANVAS_H_

#include "CzUIPanel.h"

//
//
//
//
// CzUICanvas - A panel that allows placement of any UI element in any place, no specific layout rules are enforced
//
//
//
//
class CzUICanvas : public CzUIPanel
{
	// Properties
protected:
public:
	// Properties end
protected:
	void						LinkChanged(CzActor* child, bool remove);
	bool						UpdateLayout();			// Updates the layout of the child items
public:
	CzUICanvas() : CzUIPanel() { setActualClassType("canvas"); }

	bool				Update(float dt);

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);
};

//
//  CzUICanvasCreator - Creates an instance of a canvas actor object
//
class CzUICanvasCreator : public IzXomlClassCreator
{
public:
	CzUICanvasCreator()
	{
		setClassName("canvas");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzUICanvas(); }
};


#endif	// _CCZ_UI_CANVAS_H_
