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

#include "CzUIPanel.h"

void CzUIPanel::LinkChanged(CzActor* child, bool remove)
{
	if (remove)
		RemoveActor(child);
	else
		Children.push_back(child);
	setLayoutDirty(true);
	NotifyParentsResize();
}

void CzUIPanel::RemoveActor(CzActor* actor)
{
	// Renove named actor from the panel
	for (_Iterator it = Children.begin(); it != Children.end(); ++it)
	{
		if (*it == actor)
		{
			Children.erase(it);
			break;
		}
	}
}

CzUIPanel::~CzUIPanel()
{
	Children.clear();
}

