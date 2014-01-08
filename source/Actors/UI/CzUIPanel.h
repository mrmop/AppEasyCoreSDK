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

#if !defined(_CCZ_UI_PANEL_H_)
#define _CCZ_UI_PANEL_H_

#include "CzActor.h"
#include "CzScene.h"
#include "CzUIBase.h"

class CzUIPanel : public CzUIBase
{
public:
	// Public access to actor iteration
	typedef CzList<CzActor*>::iterator _Iterator;
	_Iterator					begin() { return Children.begin(); }
	_Iterator					end() { return Children.end(); }

	// Properties
protected:
public:
	// Properties end
protected:
	CzList<CzActor*>			Children;				///< Collection of actors that are linked to this panel
	virtual void				LinkChanged(CzActor* child, bool remove);
	void						RemoveActor(CzActor* actor);
public:
	CzUIPanel() : CzUIBase() { }
	virtual ~CzUIPanel();
};



#endif	// _CCZ_UI_PANEL_H_
