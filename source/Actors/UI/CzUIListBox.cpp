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

#include "CzUIListBox.h"
#include "CzUtil.h"
#include "CzString.h"
#include "CzTemplates.h"
#include "CzApp.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"

CzXomlClassDef* CzUIListBox::UIListBoxClassDef = NULL;

/**
 @fn	void CzUIListBox::InitClass()

 @brief	Initialises the class.
    
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzUIListBox::InitClass()
{
	// Create class properties
	UIListBoxClassDef = new CzXomlClassDef();
	UIListBoxClassDef->addProperty(new CzXomlClassProperty("ItemCount",			PT_Int,			NULL,									CzUIListBox::_getItemCount));
	UIListBoxClassDef->addProperty(new CzXomlClassProperty("Orientation",		PT_String,		CzUIListBox::_setOrientation,			CzUIListBox::_getOrientation));
	UIListBoxClassDef->addProperty(new CzXomlClassProperty("AlignH",			PT_String,		CzUIListBox::_setAlignH,				NULL));
	UIListBoxClassDef->addProperty(new CzXomlClassProperty("AlignV",			PT_String,		CzUIListBox::_setAlignV,				NULL));
	UIListBoxClassDef->addProperty(new CzXomlClassProperty("ItemsTargetType",	PT_String,		CzUIListBox::_setItemsTargetType,		NULL));
	UIListBoxClassDef->addProperty(new CzXomlClassProperty("ItemsData",			PT_Resource,	CzUIListBox::_setItemsData,				CzUIListBox::_getItemsData));
	UIListBoxClassDef->addProperty(new CzXomlClassProperty("ItemsTemplate",		PT_Resource,	CzUIListBox::_setItemsTemplate,			CzUIListBox::_getItemsTemplate));
	UIListBoxClassDef->addProperty(new CzXomlClassProperty("MultiSelect",		PT_Bool,		CzUIListBox::_setMultiSelect,			CzUIListBox::_getMultiSelect));
	UIListBoxClassDef->addProperty(new CzXomlClassProperty("OnSelectionChanged",PT_String,		CzUIListBox::_setOnSelectionChanged,	NULL));
	UIListBoxClassDef->addProperty(new CzXomlClassProperty("Selection",			PT_String,		NULL,									CzUIListBox::_getSelection));
	UIListBoxClassDef->addProperty(new CzXomlClassProperty("SelectionIndex",	PT_Int,			CzUIListBox::_setSelectionIndex,		CzUIListBox::_getSelectionIndex));
	UIListBoxClassDef->addProperty(new CzXomlClassProperty("CaretColourOffset",	PT_Vec4,		CzUIListBox::_setCaretColourOffset,		CzUIListBox::_getCaretColourOffset));
	UIListBoxClassDef->addProperty(new CzXomlClassProperty("CaretIndex",		PT_Int,			CzUIListBox::_setCaretIndex,			CzUIListBox::_getCaretIndex));
}
void CzUIListBox::ReleaseClass()
{
	if (UIListBoxClassDef != NULL)
	{
		UIListBoxClassDef->Release();
		delete UIListBoxClassDef;
		UIListBoxClassDef = NULL;
	};
}

// //
// //
// //
// //
// //
// //
// //
// //
// XOML property setters and getters
// //
// //
// //
// //
// //
// //
// //
// //

CzXomlProperty	CzUIListBox::_getItemCount(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIListBox*)target)->getItemCount());
}

bool CzUIListBox::_setOrientation(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIListBox* actor = (CzUIListBox*)target;
	eCzOrientation o;

	unsigned int o_hash = CzString::CalculateHash((const char*)prop.p_data);
	if (o_hash == CzHashes::Horizontal_Hash)
		o = Orientation_Horizontal;
	else
	if (o_hash == CzHashes::Vertical_Hash)
		o = Orientation_Vertical;
	else
		return false;

	actor->setOrientation(o);

	return true;
}

CzXomlProperty	CzUIListBox::_getOrientation(IzXomlResource* target)
{
	return CzXomlProperty(CzXoml::OrientationNames[((CzUIListBox*)target)->getOrientation()]);
}

bool CzUIListBox::_setAlignH(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIListBox* actor = (CzUIListBox*)target;
	eCzAlignH align;

	unsigned int align_hash = CzString::CalculateHash((const char*)prop.p_data);
	if (align_hash == CzHashes::left_Hash)
		align = AlignH_Left;
	else
	if (align_hash == CzHashes::right_Hash)
		align = AlignH_Right;
	else
	if (align_hash == CzHashes::centre_Hash)
		align = AlignH_Centre;
	else
		return false;

	actor->setAlignH(align);

	return true;
}

bool CzUIListBox::_setAlignV(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIListBox* actor = (CzUIListBox*)target;
	eCzAlignV align;

	unsigned int align_hash = CzString::CalculateHash((const char*)prop.p_data);
	if (align_hash == CzHashes::top_Hash)
		align = AlignV_Top;
	else
	if (align_hash == CzHashes::bottom_Hash)
		align = AlignV_Bottom;
	else
	if (align_hash == CzHashes::middle_Hash)
		align = AlignV_Middle;
	else
		return false;

	actor->setAlignV(align);

	return true;
}

bool CzUIListBox::_setItemsTemplate(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIListBox* actor = (CzUIListBox*)target;

	if (prop.Type == PT_Resource)
		return actor->setItemsTemplate((CzTemplate*)prop.p_data);
	else
		return actor->setItemsTemplate((const char*)prop.p_data);
}

CzXomlProperty CzUIListBox::_getItemsTemplate(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIListBox*)target)->getItemsTemplate());
}

bool CzUIListBox::_setItemsData(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIListBox* actor = (CzUIListBox*)target;

	if (prop.Type == PT_Resource)
		return actor->setItemsData((CzXomlVariable*)prop.p_data);
	else
		return actor->setItemsData((const char*)prop.p_data, NULL);
}

CzXomlProperty CzUIListBox::_getItemsData(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIListBox*)target)->getItemsData());
}

bool CzUIListBox::_setItemsTargetType(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIListBox* actor = (CzUIListBox*)target;

	actor->setItemsTargetType((const char*)prop.p_data);

	return true;
}

bool CzUIListBox::_setOnSelectionChanged(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIListBox* actor = (CzUIListBox*)target;

	actor->getEventsManager()->addEvent("OnSelectionChanged", (const char*)prop.p_data, true);

	return true;
}

bool CzUIListBox::_setMultiSelect(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIListBox* actor = (CzUIListBox*)target;

	if (add)
		actor->setMultiSelect(!actor->isMultiSelect());
	else
		actor->setMultiSelect(prop.p_bool);

	return true;
}

CzXomlProperty CzUIListBox::_getMultiSelect(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIListBox*)target)->isMultiSelect());
}

CzXomlProperty CzUIListBox::_getSelection(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIListBox*)target)->getSelection());
}

bool CzUIListBox::_setSelectionIndex(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIListBox* actor = (CzUIListBox*)target;

	if (add)
		actor->setSelectionIndex(actor->getSelectionIndex() + prop.p_int);
	else
		actor->setSelectionIndex(prop.p_int);

	return true;
}

CzXomlProperty CzUIListBox::_getSelectionIndex(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIListBox*)target)->getSelectionIndex());
}

bool CzUIListBox::_setCaretColourOffset(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIListBox* actor = (CzUIListBox*)target;

	if (add)
	{
		CzIRect c = actor->getCaretColourOffset();
		c.x += (uint8)prop.p_vec[0];
		c.y += (uint8)prop.p_vec[1];
		c.w += (uint8)prop.p_vec[2];
		c.h += (uint8)prop.p_vec[3];
		actor->setCaretColourOffset(c);
	}
	else
	{
		CzIRect c;
		c.x = (uint8)prop.p_vec[0];
		c.y = (uint8)prop.p_vec[1];
		c.w = (uint8)prop.p_vec[2];
		c.h = (uint8)prop.p_vec[3];
		actor->setCaretColourOffset(c);
	}

	return true;
}

CzXomlProperty CzUIListBox::_getCaretColourOffset(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIListBox*)target)->getCaretColourOffset());
}

bool CzUIListBox::_setCaretIndex(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIListBox* actor = (CzUIListBox*)target;

	if (add)
		actor->setCaretIndex(actor->getCaretIndex() + prop.p_int);
	else
		actor->setCaretIndex(prop.p_int);

	return true;
}

CzXomlProperty CzUIListBox::_getCaretIndex(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIListBox*)target)->getCaretIndex());
}


//
//
//
//
//
// CzUIListBox implementation
//
//
//
//
//

CzUIListBox::~CzUIListBox()
{
	if (!Scene->isDestroyed())
		RemoveAllActors();
	for (_Iterator it = begin(); it != end(); ++it)
		delete *it;
}

bool CzUIListBox::setItemsTemplate(const char* temp_name)
{
	// Find items template
	CzTemplate* temp = (CzTemplate*)Scene->getResourceManager()->findResource(temp_name, CzHashes::Template_Hash);
	return setItemsTemplate(temp);
}

bool CzUIListBox::setItemsTemplate(CzTemplate* temp)
{
	if (temp == NULL)
		return false;

	if (ItemsTemplate == temp)
		return true;
	
	ItemsTemplate = temp;
	RemoveAllActors();
	CreateAllActors();
	UpdateAllActors(true);
	setLayoutDirty(true);

	return true;
}

void CzUIListBox::setCaretIndex(int index)
{
	CzUIBase* ui = NULL;

	if (CaretIndex < 0)
		CaretIndex = 0;
	else
	{
		ui = Children[CaretIndex]->Actor;
		ui->resetColourOffset();
	}

	if (index < 0) index = Children.size() - 1;
	if (index > (int)Children.size() - 1) index = 0;

	ui = Children[index]->Actor;
	ui->setColourOffset(CaretColourOffset);
	ScrollToIndex(index);

	CaretIndex = index;
}

void CzUIListBox::ScrollToIndex(int index)
{
	CzUIBase* actor = Children[index]->Actor;

	CzVec2 pos = CalculateItemPosition(index);

	if (Orientation == Orientation_Horizontal)
	{
		if (ScrollRange.w != 0)
		{
			if (pos.x < (float)ScrollRange.x)
				pos.x = (float)ScrollRange.x;
			else
			if (pos.x > (float)ScrollRange.x + ScrollRange.w)
				pos.x = (float)ScrollRange.x + ScrollRange.w;
		}
		setScrollPos(pos.x, ScrollPos.y);
	}
	else
	{
		if (ScrollRange.h != 0)
		{
			if (pos.y < (float)ScrollRange.y)
				pos.y = (float)ScrollRange.y;
			else
			if (pos.y > (float)ScrollRange.y + ScrollRange.h)
				pos.y = (float)ScrollRange.y + ScrollRange.h;
		}
		setScrollPos(ScrollPos.x, pos.y);
	}

	setLayoutDirty(true);
}

void CzUIListBox::addItem(CzUIListBoxItem* item)
{
	if (item->Actor != NULL)
		item->Actor->setLinkedTo(this);
}

void CzUIListBox::removeItem(CzUIListBoxItem* item)
{
	for (_Iterator it = begin(); it != end(); ++it)
	{
		if ((*it)->Actor != NULL)
		{
			(*it)->Actor->getScene()->removeActor((*it)->Actor);
		}
		delete *it;
		Children.erase(it);
		break;
	}
}

CzUIListBoxItem* CzUIListBox::findItem(CzUIBase* actor)
{
	for (_Iterator it = begin(); it != end(); ++it)
	{
		if ((*it)->Actor == actor)
			return *it;
	}

	return NULL;
}

bool CzUIListBox::setItemsData(const char* variable_name, CzXmlNode* node)
{
	if (variable_name != NULL)
	{
		CzString var_name = variable_name;
		CzXomlVariable* items_var = CzXomlVariable::GetVariable(var_name, Scene);
		if (items_var != NULL)
		{
			if (!items_var->isArray())
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ListBox - ItemsData should be an array variable type - ", variable_name, DebugInfo.c_str());
				return false;
			}
			else
			{
				if (node == NULL)
					setItemsData((CzXomlVariableArray*)items_var, ItemsTargetTypeHash);
				else
					ItemsData = (CzXomlVariableArray*)items_var;
			}
		}
		else
		{
			if (node != NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ListBox - ItemsData could not be found - ", variable_name, DebugInfo.c_str());
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ListBox - ItemsData could not be found - ", variable_name, DebugInfo.c_str());
			return false;
		}
	}

	return true;
}

void CzUIListBox::setItemsTargetType(const char* type)
{
	unsigned int hash = CzString::CalculateHash(type);

	if (ItemsTargetTypeHash != hash)
	{
		RemoveAllActors();
		ItemsTargetTypeHash = hash;
		CreateAllActors();
		setLayoutDirty(true);
		UpdateAllActors(true);
	}
}

void CzUIListBox::setItemsData(CzXomlVariableArray* data, unsigned int target_type_hash)
{
	if (data == NULL)
		return;

	// If type of data is different then destroy previous grid actors and re-create new ones
	if (ItemsTargetTypeHash != target_type_hash)
	{
		RemoveAllActors();
		ItemsTargetTypeHash = target_type_hash;
		ItemsData = data;
		CreateAllActors();
	}
	else
		ItemsData = data;

	setLayoutDirty(true);
	UpdateAllActors(true);
}

bool CzUIListBox::setItemsData(CzXomlVariable* data)
{
	if (data == NULL)
		return true;
	if (data->isArray())
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Listbox - ItemsData should be an array variable type - ", data->Name.c_str());
		return false;
	}

	RemoveAllActors();
	ItemsData = (CzXomlVariableArray*)data;
	CreateAllActors();

	setLayoutDirty(true);
	UpdateAllActors(true);

	return true;
}

void CzUIListBox::ClearAllSelections(CzUIBase* ignore)
{
	for (_Iterator it = begin(); it != end(); ++it)
	{
		CzUIBase* actor = (*it)->Actor;
		if (actor != ignore && actor != NULL)
		{
			((CzUIBase*)actor)->setSelected(false);
		}
	}
}

void CzUIListBox::ClearAllToggles(CzUIBase* ignore)
{
	for (_Iterator it = begin(); it != end(); ++it)
	{
		CzUIBase* actor = (*it)->Actor;
		if (actor != ignore && actor != NULL)
		{
			((CzUIBase*)actor)->setToggledOn(false);
		}
	}
}

void CzUIListBox::LinkChanged(CzActor* child, bool remove)
{
	if (child->getCategory() != AC_UI)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Listbox - Only actors that are derived from UI actor classes can be added to list box - ", child->getName().c_str(), DebugInfo.c_str());
#endif	// _DEBUG
		return;
	}

	if (remove)
	{
		for (_Iterator it = begin(); it != end(); ++it)
		{
			if ((*it)->Actor == child)
			{
				delete *it;
				Children.erase(it);
				break;
			}
		}
	}
	else
	{
		CzUIListBoxItem* item = new CzUIListBoxItem();
		item->Actor = (CzUIBase*)child;
		Children.push_back(item);
	}
	setLayoutDirty(true);
	NotifyParentsResize();
}

void CzUIListBox::InitListBox()
{
	CreateAllActors();
	UpdateAllActors(true);
}

void CzUIListBox::RemoveAllActors()
{
	while (Children.begin() != Children.end())
	{
		CzUIBase* actor = Children[0]->Actor;
		actor->getScene()->removeActor(actor);
	}
	Children.clear();
}

void CzUIListBox::CreateAllActors()
{
	if (ItemsTemplate == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Listbox - No template assigned - ", Name.c_str(), DebugInfo.c_str());
#endif	// _DEBUG
		return;
	}

	if (ItemsData == NULL)
		return;
	
	// Remove all previous actors
	RemoveAllActors();

	// Create a set of XML attributes that will replace the template parameters
	CzXmlNode* replacements = new CzXmlNode();
	replacements->Managed = false;

	// Set base template paramater
	CzXmlAttribute* index_attrib = new CzXmlAttribute();
	index_attrib->Managed = false;
	index_attrib->setName("index");
	replacements->AddAttribute(index_attrib);

	// Create new actors
	int index = 0;
	for (int t = 0; t < ItemsData->getSize(); t++)
	{
		if (ItemsData->getElement(t) != NULL)
		{
			// Instantiate actor from item template
			index_attrib->setValue(CzString(index).c_str());
			if (!ItemsTemplate->Instantiate(this, replacements))
			{
#if defined(_DEBUG)
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Listbox - Could not instantiate ListBoxactor from items template - ", ItemsTemplate->getName().c_str(), DebugInfo.c_str());
#endif	// _DEBUG
				delete replacements;
				return;
			}
			index++;
		}
	}

	ItemsCount = ItemsData->getSize();
	setLayoutDirty(true);

	// Clean up replacement attributes
	delete replacements;
}

void CzUIListBox::UpdateAllActors(bool force_update)
{
	if (ItemsTemplate == NULL || ItemsData == NULL)
		return;

	int index = 0;
	for (_Iterator it = begin(); it != end(); ++it)
	{
		CzUIBase* actor = (*it)->Actor;
		if (actor != NULL)
		{
			CzXomlVariable* var = ItemsData->getElement(index);
			if (var != NULL && (var->Modified || force_update))
				actor->setProperty(ItemsTargetTypeHash, var->getValue(), false);
		}
		index++;

		// If too many items then exit
		if (index >= ItemsData->getSize())
			break;
	}
}

CzVec2 CzUIListBox::CalculateItemPosition(int item_index)
{
	int index = 0;
	
	_Iterator it = begin();
	if (it == end())
		return CzVec2(0, 0);

	int left = -Size.x / 2 + ClippingMargin.x;
	int right = Size.x / 2 - ClippingMargin.y;
	int top = -Size.y / 2 + ClippingMargin.w;
	int bottom = Size.y / 2 - ClippingMargin.h;

	CzIRect margin;

	if (Orientation == Orientation_Vertical)
	{
		float y = (float)(-Size.y / 2 + (*it)->Actor->getSize().y / 2);

		for (; it != end(); ++it)
		{
			CzUIBase* actor = (*it)->Actor;
			if (actor != NULL)
			{
				CzIVec2 size = actor->getSize();
				margin = actor->getMargin();
				if ((*it)->Visible)
				{
					int sx = size.x >> 1;
					int sy = size.y >> 1;
					y += (float)margin.w;
					y += (float)(size.y + margin.h);
					if (index == item_index)
					{
						int xo = 0;

						if (AlignH == AlignH_Left)
							xo = -Size.x / 2 + sx / 2 + margin.x;
						else
						if (AlignH == AlignH_Right)
							xo = Size.x / 2 - sx / 2 - margin.y;

						return CzVec2((float)xo, y);
					}
					index++;
				}
			}
		}
	}
	else
	{
		float x = (float)(-Size.x / 2 + (*it)->Actor->getSize().x / 2);

		ScrollRange.w = 0;
		for (; it != end(); ++it)
		{
			CzUIBase* actor = (*it)->Actor;
			if (actor != NULL)
			{
				CzIVec2 size = actor->getSize();
				margin = actor->getMargin();
				if ((*it)->Visible)
				{
					int sx = size.x >> 1;
					int sy = size.y >> 1;

					x += (float)margin.x;
					x += (float)(size.x + margin.y);
					if (index == item_index)
					{
						int yo = 0;

						if (AlignV == AlignV_Top)
							yo = -Size.y / 2 + sy / 2 + margin.w;
						else
						if (AlignV == AlignV_Bottom)
							yo = Size.y / 2 - sy / 2 - margin.h;

						return CzVec2(x, (float)yo);
					}
					index++;
				}
			}
		}
	}

	return CzVec2(0, 0);
}

bool CzUIListBox::UpdateLayout()
{
	int index = 0;
	
	_Iterator it = begin();
	if (it == end())
		return true;

	int left = -Size.x / 2 + ClippingMargin.x;
	int right = Size.x / 2 - ClippingMargin.y;
	int top = -Size.y / 2 + ClippingMargin.w;
	int bottom = Size.y / 2 - ClippingMargin.h;

	CzIRect margin;

	if (Orientation == Orientation_Vertical)
	{
		float y = (float)(-Size.y / 2 + (*it)->Actor->getSize().y / 2) - ScrollPos.y;

		ScrollRange.h = 0;
		for (; it != end(); ++it)
		{
			CzUIBase* actor = (*it)->Actor;
			if (actor != NULL)
			{
				CzIVec2 size = actor->getSize();
				margin = actor->getMargin();
				if ((*it)->Visible)
				{
					int sx = size.x >> 1;
					int sy = size.y >> 1;
					int xo = 0;

					if (AlignH == AlignH_Left)
						xo = -Size.x / 2 + sx / 2 + margin.x;
					else
					if (AlignH == AlignH_Right)
						xo = Size.x / 2 - sx / 2 - margin.y;

					y += (float)margin.w;
					actor->setPosition((float)xo - ScrollPos.x, y);
					if ((y + sy) < top || (y - sy) > bottom)
						actor->setVisible(false);
					else
						actor->setVisible(true);
					index++;
					y += (float)(size.y + margin.h);
				}
				else
				{
					CzUIBase* actor = (*it)->Actor;
					if (actor != NULL)
						actor->setVisible(false);
				}
				ScrollRange.h += size.y + margin.w + margin.h;
			}
		}

		// Calculate vertical scroll range 
		ScrollRange.w = 0;
		ScrollRange.h -= Size.y;
		if (ScrollRange.h < 0)
			ScrollRange.h = 0;
	}
	else
	{
		float x = (float)(-Size.x / 2 + (*it)->Actor->getSize().x / 2) - ScrollPos.x;

		ScrollRange.w = 0;
		for (; it != end(); ++it)
		{
			CzUIBase* actor = (*it)->Actor;
			if (actor != NULL)
			{
				CzIVec2 size = actor->getSize();
				margin = actor->getMargin();
				if ((*it)->Visible)
				{
					int sx = size.x >> 1;
					int sy = size.y >> 1;
					int yo = 0;

					if (AlignV == AlignV_Top)
						yo = -Size.y / 2 + sy / 2 + margin.w;
					else
					if (AlignV == AlignV_Bottom)
						yo = Size.y / 2 - sy / 2 - margin.h;

					x += (float)margin.x;
					actor->setPosition(x, (float)yo - ScrollPos.y);
					if ((x + sx) < left || (x - sx) > right)
						actor->setVisible(false);
					else
						actor->setVisible(true);
					index++;
					x += (float)(size.x + margin.y);
				}
				else
				{
					CzUIBase* actor = (*it)->Actor;
					if (actor != NULL)
						actor->setVisible(false);
				}
				ScrollRange.w += size.x + margin.x + margin.y;
			}
		}

		// Calculate vertical scroll range 
		ScrollRange.h = 0;
		ScrollRange.w -= Size.x;
		if (ScrollRange.w < 0)
			ScrollRange.w = 0;
	}

	return CzUIBase::UpdateLayout();
}

bool CzUIListBox::Update(float dt)
{
	if (!CzUIBase::Update(dt))
		return false;

	if (ItemsData != NULL &&  ItemsData->getSize() != ItemsCount)
		InitListBox();

	if (Scene->getParent()->getKeyFocus() == this && CZ_INPUT->isKeysAvailable())
	{
		int key = CZ_INPUT->getLastKey();

		if (key != 0)
		{
			if (key == eKeyUp || key == eKeyAbsUp || key == eKeyLeft || key == eKeyAbsLeft)
				setCaretIndex(getCaretIndex() - 1);
			else
			if (key == eKeyDown || key == eKeyAbsDown || key == eKeyRight || key == eKeyAbsRight)
				setCaretIndex(getCaretIndex() + 1);
			else
			if (key == eKeyOk || key == eKeyAbsOk || key == eKeyAbsGameA || key == eKeyEnter)
				setSelectionIndex(CaretIndex, true);
		}
	}

	// Check to see if any of the contained actors have been tapped
	CzActor* last_tapped = Scene->getParent()->getLastTappedActor(0);
	if (last_tapped != NULL)
	{
		int index = 0;
		for (_Iterator it = begin(); it != end(); ++it)
		{
			if ((*it)->Actor != NULL)
			{
				CzUIBase* actor = (CzUIBase*)(*it)->Actor;
				if (actor == last_tapped)
				{
					if (MultiSelect)
					{
//						actor->setSelected(!actor->isSelected());
					}
					else
					{
						ClearAllSelections(actor);
						ClearAllToggles(actor);
//						actor->setSelected(true);
					}
					NotifySelectionChanged(SelectionIndex, index);
					SelectionIndex = index;
					Selection = actor;
					break;
				}
			}
			index++;
		}
	}

	// Set clip rect for the list box
	CzVec4 rc;
	rc.x = (float)((-Size.x >> 1) + ClippingMargin.x);
	rc.y = (float)((-Size.y >> 1) + ClippingMargin.w);
	rc.z = (float)(Size.x - (ClippingMargin.x + ClippingMargin.y));
	rc.w = (float)(Size.y - (ClippingMargin.w + ClippingMargin.h));
	Visual->setClipRect(rc);

	UpdateAllActors(false);

	return true;
}


void CzUIListBox::setSelectionIndex(int index, bool call_events)
{
	if (!MultiSelect)
	{
		ClearAllSelections(NULL);
		ClearAllToggles(NULL);
	}
	if (index < 0)
	{
		if (Selection != NULL)
			Selection->setSelected(false);
		index = -1;
		Selection = NULL;
		return;
	}
	if (index >= (int)Children.size())
		return;

	SelectionIndex = index;
	Selection = Children[index]->Actor;
	Selection->setSelected(true);

	if (call_events)	
		Selection->ProcessEventActions(CzHashes::OnTapped_Hash);
}

/**
 @fn	int CzUIListBox::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)

 @brief	Creates an instance of this class from XML

 LoadFromXoml is the main method used by all classes that can be instantiated from XOML mark-up. This method creates an instance of this class from the supplied XML node structure specified by node. 
 if parent is not NULL then the created instance will utilise the supplied parent to determine where it should live within the app. For example, if the parent is a scene then the actor will be placed 
 in that scene. if the parent is another actor then this actor will be placed into the scene of the supplied actor and linked as a child to the parent actor. If load_chlldren is true then any child 
 nodes contained within the actor will also be loaded. Note that as CzUIListBox is derived from CzUIBase, CzUIBase::LoadFromXoml() will also be called to load CzActor specific properties. 

 @param [in]	parent		If non-null, the parent.
 @param	load_children	  	true to load child nodes.
 @param [in]	node  		If non-null, the XOML markup node that defines this object

 @return	negative value for error. if 0 then processing of further XOML will stop.
 */

int CzUIListBox::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Listbox - Needs to be declared inside a scene or an actor", DebugInfo.c_str());
		return 0;
	}

	// Load main actor attributes
	int ret = CzUIBase::LoadFromXoml(parent, false, node);
	if (ret <= 0)
		return ret;

	CzString*	items_data = NULL, *items_template = NULL, *items_target_type = NULL, *selection = NULL;
	int			selection_index = -1;

	// Process list box specific attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Orientation_Hash)
		{
			unsigned int hash = (*it)->getValue().getHash();
			if (hash == CzHashes::Vertical_Hash)
				Orientation = Orientation_Vertical;
			else
				Orientation = Orientation_Horizontal;
		}
		else
		if (name_hash == CzHashes::AlignH_Hash)
		{
			unsigned int align_hash = (*it)->getValue().getHash();
			if (align_hash == CzHashes::left_Hash)
				AlignH = AlignH_Left;
			else
			if (align_hash == CzHashes::right_Hash)
				AlignH = AlignH_Right;
			else
				AlignH = AlignH_Centre;
		}
		else
		if (name_hash == CzHashes::AlignV_Hash)
		{
			unsigned int align_hash = (*it)->getValue().getHash();
			if (align_hash == CzHashes::top_Hash)
				AlignV = AlignV_Top;
			else
			if (align_hash == CzHashes::bottom_Hash)
				AlignV = AlignV_Bottom;
			else
				AlignV = AlignV_Middle;
		}
		else
		if (name_hash == CzHashes::ItemsTargetType_Hash)
			items_target_type = &(*it)->getValue();
		else
		if (name_hash == CzHashes::ItemsData_Hash)
			items_data = &(*it)->getValue();
		else
		if (name_hash == CzHashes::ItemsTemplate_Hash)
			items_template = &(*it)->getValue();
		else
		if (name_hash == CzHashes::MultiSelect_Hash)
			MultiSelect = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::OnSelectionChanged_Hash)
			EventsManager->addEvent("OnSelectionChanged", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Selection_Hash)
			selection = &(*it)->getValue();
		else
		if (name_hash == CzHashes::SelectedIndex_Hash)
			selection_index = (*it)->getValueAsInt();
		else
		if (name_hash == CzHashes::CaretColourOffset_Hash)
		{
			if (!(*it)->getValueAsRect(CaretColourOffset))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Listbox - Invalid value for Actor::CaretColourOffset, expected rect - ", DebugInfo.c_str());
		}
	}

	// Find and attach the items data array
	if (items_data != NULL)
		setItemsData(items_data->c_str(), node);

	// Find and attach the selection variable
	if (selection != NULL)
	{
		CzXomlVariable* var = CzXomlVariable::GetVariable(*selection, Scene);
		if (var == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Listbox - Selection binding variable not found - ", selection->c_str(), DebugInfo.c_str());
		SelectionVar = var;
	}

	if (items_target_type != NULL)
	{
		// Set Items target type
#if defined(_DEBUG)
		ItemsTargetType = *items_target_type;
#endif	// _DEBUG
		ItemsTargetTypeHash = items_target_type->getHash();
	}

	// Attach the items template
	if (items_template != NULL)
	{
		ItemsTemplate = (CzTemplate*)Scene->getResourceManager()->findResource(items_template->getHash(), CzHashes::Template_Hash);
		if (ItemsTemplate == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ListBox - ItemsTemplate could not be found - ", items_template->c_str(), DebugInfo.c_str());
	}

	// Initialise the list box
	InitListBox();

	// Process inner tags
	if (load_children)
	{
		if (!CZ_XOML->Process(this, node))
			return 0;
	}

	// Set default selection
	if (selection_index >= 0)
		setSelectionIndex(selection_index);

	return 1;
}


void CzUIListBox::NotifySelectionChanged(int old_index, int new_index)
{
	// Update attached variable
	if (SelectionVar != NULL)
	{
		SelectionVar->setValue(CzString(new_index).c_str());
	}

	ProcessEventActions(CzHashes::OnSelectionChanged_Hash);
}

/**
 @fn	bool CzUIListBox::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the actor.

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types. Note that as 
 CzUIListBox is derived from CzUIBase, all CzUIBase properties will also be checked against.
UIListBox
 @param	property_name		Name of the property.
 @param [out]	prop	The property data that.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIListBox::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	if (CzUIBase::getProperty(property_name, prop))
		return true;

	int err = IzXomlResource::getProperty(UIListBoxClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIListBoxClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Listbox - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIListBox::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the UIListBox.
    
 Sets the named property of this UIListBox. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzUIListBox::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	if (CzUIBase::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(UIListBoxClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIListBoxClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Listbox - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIListBox::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced. Note that as CzUIListBox is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in]	data	The property data.
 @param	delta				true to property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIListBox::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	if (CzUIBase::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(UIListBoxClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIListBoxClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Listbox - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIListBox::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates actor property from supplied animation.

 Updates the actors property that is specified by the supplied animation. When a timeline animation targets the property of an actor this method will be called by the animation to move the 
 animations frame data into its target property. Note that as CzUIListBox is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param [in]	animation	If non-null, the animation.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIListBox::UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzUIBase::UpdateFromAnimation(animation))
		return true;

	int err = IzXomlResource::setProperty(UIListBoxClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIListBoxClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Listbox - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIListBox::UpdateBinding(unsigned int property_name, CzXomlVariable* var)

 @brief	Updates an actor property from the supplied XOML variable.

 Updates a single property from the value of the bound XOML variable. Note that as CzUIListBox is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param	property_name  	Name of the property.
 @param [in,out]	var	If non-null, the variable.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIListBox::UpdateBinding(unsigned int property_name, CzXomlVariable* var)
{
	if (!var->Modified || var->getValue().isEmpty())
		return false;

	if (CzUIBase::UpdateBinding(property_name, var))
		return true;

	int err = IzXomlResource::setProperty(UIListBoxClassDef, this, property_name, var);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIListBoxClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Listbox - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}








