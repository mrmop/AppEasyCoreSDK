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

#include "CzUICanvas.h"
#include "CzUtil.h"
#include "CzString.h"
#include "CzTemplates.h"
#include "CzHashes.h"

void CzUICanvas::LinkChanged(CzActor* child, bool remove)
{
	CzUIPanel::LinkChanged(child, remove);

	if (!remove)
		child->setScreenDocking(false);
}

bool CzUICanvas::UpdateLayout()
{
	float width = (float)Size.x / 2;
	float height = (float)Size.y / 2;

	float dx = PreviousScrollPos.x - ScrollPos.x;
	float dy = PreviousScrollPos.y - ScrollPos.y;

	// Dock any actors that are set as dockable and scroll rest
	for (_Iterator it = begin(); it != end(); ++it)
	{
		CzActor* actor = *it;
		CzScene::eDocking docking = actor->getDocking();
		if (docking != CzScene::Dock_None)
		{
			CzVec2 pos = CzVec2(0, 0);
			CzIRect margin = actor->getMargin();

			switch (docking)
			{
			case CzScene::Dock_Top:
				pos.x = 0;
				pos.y = 0 - height + (actor->getSize().y + 1) / 2 + margin.w;
				break;
			case CzScene::Dock_Bottom:
				pos.x = 0;
				pos.y = 0 + height - ((actor->getSize().y + 1) / 2 + margin.h);
				break;
			case CzScene::Dock_Left:
				pos.x = 0 - width + (actor->getSize().x + 1) / 2 + margin.x;
				pos.y = 0;
				break;
			case CzScene::Dock_Right:
				pos.x = 0 + width - ((actor->getSize().x + 1) / 2 + margin.y);
				pos.y = 0;
				break;
			case CzScene::Dock_TopLeft:
				pos.x = 0 - width + (actor->getSize().x + 1) / 2 + margin.x;
				pos.y = 0 - height + (actor->getSize().y + 1) / 2 + margin.w;
				break;
			case CzScene::Dock_TopRight:
				pos.x = 0 + width - ((actor->getSize().x + 1) / 2 + margin.y);
				pos.y = 0 - height + (actor->getSize().y + 1) / 2 + margin.w;
				break;
			case CzScene::Dock_BottomLeft:
				pos.x = 0 - width + (actor->getSize().x + 1) / 2 + margin.x;
				pos.y = 0 + height - ((actor->getSize().y + 1) / 2 + margin.h);
				break;
			case CzScene::Dock_BottomRight:
				pos.x = 0 + width - ((actor->getSize().x + 1) / 2 + margin.y);
				pos.y = 0 + height - ((actor->getSize().y + 1) / 2 + margin.h);
				break;
			}

			actor->setPosition(pos);
		}
		else
		{
			CzVec2 pos = actor->getPosition();
			actor->setPosition(pos.x + dx, pos.y + dy);
		}
	}

	return CzUIBase::UpdateLayout();
}

bool CzUICanvas::Update(float dt)
{
	if (!CzUIBase::Update(dt))
		return false;

	// Set clip rect for the list box
	CzVec4 rc;
	rc.x = (float)((-Size.x >> 1) + ClippingMargin.x);
	rc.y = (float)((-Size.y >> 1) + ClippingMargin.w);
	rc.z = (float)(Size.x - (ClippingMargin.x +  + ClippingMargin.y));
	rc.w = (float)(Size.y - (ClippingMargin.w +  + ClippingMargin.h));
	Visual->setClipRect(rc);

	return true;
}

int CzUICanvas::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Canvas - Needs to be declared inside a scene or an actor - ", DebugInfo.c_str());
		return 0;
	}

	// Load main actor attributes
	int ret = CzUIBase::LoadFromXoml(parent, false, node);
	if (ret <= 0)
		return ret;

	// Process StackPanel specific attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

	}

	// Process inner tags
	if (load_children)
	{
		if (!CZ_XOML->Process(this, node))
			return 0;
	}

	UpdateLayout();
	ResizeToContent();

	return 1;
}

