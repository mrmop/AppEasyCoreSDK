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

#include "CzUIGrid.h"
#include "CzUtil.h"
#include "CzString.h"
#include "CzApp.h"
#include "CzXomlResourceManager.h"

CzXomlClassDef* CzUIGrid::UIGridClassDef = NULL;

/**
 @fn	void CzActorImage::InitClass()

 @brief	Initialises the class.
    
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzUIGrid::InitClass()
{
	// Create class properties
	UIGridClassDef = new CzXomlClassDef();
	UIGridClassDef->addProperty(new CzXomlClassProperty("ColumnCount",			PT_Int,			NULL,							CzUIGrid::_getColumnCount));
	UIGridClassDef->addProperty(new CzXomlClassProperty("RowCount",				PT_Int,			NULL,							CzUIGrid::_getRowCount));
	UIGridClassDef->addProperty(new CzXomlClassProperty("ItemsTemplate",		PT_Resource,	CzUIGrid::_setItemsTemplate,	CzUIGrid::_getItemsTemplate));
	UIGridClassDef->addProperty(new CzXomlClassProperty("ItemsData",			PT_Resource,	CzUIGrid::_setItemsData,		CzUIGrid::_getItemsData));
	UIGridClassDef->addProperty(new CzXomlClassProperty("ItemsTargetType",		PT_String,		CzUIGrid::_setItemsTargetType,	NULL));
	UIGridClassDef->addProperty(new CzXomlClassProperty("OnSelectionChanged",	PT_String,		CzUIGrid::_setOnSelectionChanged,	NULL));
	UIGridClassDef->addProperty(new CzXomlClassProperty("MultiSelect",			PT_Bool,		CzUIGrid::_setMultiSelect,		CzUIGrid::_getMultiSelect));
	UIGridClassDef->addProperty(new CzXomlClassProperty("Selection",			PT_String,		NULL,							CzUIGrid::_getSelection));
	UIGridClassDef->addProperty(new CzXomlClassProperty("SelectionIndex",		PT_Int,			CzUIGrid::_setSelectionIndex,	CzUIGrid::_getSelectionIndex));
	UIGridClassDef->addProperty(new CzXomlClassProperty("ShowRow",				PT_Vec2,		CzUIGrid::_setShowRow,			NULL));
	UIGridClassDef->addProperty(new CzXomlClassProperty("ShowColumn",			PT_Vec2,		CzUIGrid::_setShowColumn,		NULL));

}
void CzUIGrid::ReleaseClass()
{
	if (UIGridClassDef != NULL)
	{
		UIGridClassDef->Release();
		delete UIGridClassDef;
		UIGridClassDef = NULL;
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

CzXomlProperty CzUIGrid::_getColumnCount(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIGrid*)target)->getColumnCount());
}

CzXomlProperty CzUIGrid::_getRowCount(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIGrid*)target)->getRowCount());
}

bool CzUIGrid::_setItemsTemplate(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIGrid* actor = (CzUIGrid*)target;

	if (prop.Type == PT_Resource)
		return actor->setItemsTemplate((CzTemplate*)prop.p_data);
	else
		return actor->setItemsTemplate((const char*)prop.p_data);
}

CzXomlProperty CzUIGrid::_getItemsTemplate(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIGrid*)target)->getItemsTemplate());
}

bool CzUIGrid::_setItemsData(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIGrid* actor = (CzUIGrid*)target;

	if (prop.Type == PT_Resource)
		return actor->setItemsData((CzXomlVariable*)prop.p_data);
	else
		return actor->setItemsData((const char*)prop.p_data);
}

CzXomlProperty CzUIGrid::_getItemsData(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIGrid*)target)->getItemsData());
}

bool CzUIGrid::_setItemsTargetType(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIGrid* actor = (CzUIGrid*)target;

	actor->setItemsTargetType((const char*)prop.p_data);

	return true;
}

bool CzUIGrid::_setOnSelectionChanged(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIGrid* actor = (CzUIGrid*)target;

	actor->getEventsManager()->addEvent("OnSelectionChanged", (const char*)prop.p_data, true);

	return true;
}

bool CzUIGrid::_setMultiSelect(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIGrid* actor = (CzUIGrid*)target;

	if (add)
		actor->setMultiSelect(!actor->isMultiSelect());
	else
		actor->setMultiSelect(prop.p_bool);

	return true;
}

CzXomlProperty CzUIGrid::_getMultiSelect(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIGrid*)target)->isMultiSelect());
}

CzXomlProperty CzUIGrid::_getSelection(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIGrid*)target)->getSelection());
}

bool CzUIGrid::_setSelectionIndex(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIGrid* actor = (CzUIGrid*)target;

	if (add)
		actor->setSelectionIndex(actor->getSelectionIndex() + prop.p_int);
	else
		actor->setSelectionIndex(prop.p_int);

	return true;
}

CzXomlProperty CzUIGrid::_getSelectionIndex(IzXomlResource* target)
{
	return CzXomlProperty(((CzUIGrid*)target)->getSelectionIndex());
}

bool CzUIGrid::_setShowRow(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIGrid* actor = (CzUIGrid*)target;

	if (add)
		actor->setRowVisible((int)prop.p_vec[0], !actor->isRowVisible((int)prop.p_vec[0]));
	else
		actor->setRowVisible((int)prop.p_vec[0], prop.p_vec[1] > 0);

	return true;
}

bool CzUIGrid::_setShowColumn(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUIGrid* actor = (CzUIGrid*)target;

	if (add)
		actor->setColumnVisible((int)prop.p_vec[0], !actor->isColumnVisible((int)prop.p_vec[0]));
	else
		actor->setColumnVisible((int)prop.p_vec[0], prop.p_vec[1] > 0);

	return true;
}


CzUIGrid::~CzUIGrid()
{
	if (!Scene->isDestroyed())
		RemoveAllActors();
	for (CzVector<CzUIGridRow*>::iterator ity = RowDefinitions.begin(); ity != RowDefinitions.end(); ++ity)
		delete *ity;
	for (CzVector<CzUIGridColumn*>::iterator itx = ColumnDefinitions.begin(); itx != ColumnDefinitions.end(); ++itx)
		delete *itx;
}

CzUIGridColumn* CzUIGrid::AddColumn(const char* name, int width, eCzAlignH align, bool visible)
{
	int owidth = width;
	if (width < 0)
		width = (Size.x * -width) / 100;

	CzUIGridColumn* col = new CzUIGridColumn(name, width, align, visible);
	col->OriginalWidth = owidth;
	ColumnDefinitions.push_back(col);

	return col;
}	

CzUIGridRow* CzUIGrid::AddRow(const char* name, int height, eCzAlignV align, bool visible)
{
	int oheight = height;
	if (height < 0)
		height = (Size.y * -height) / 100;

	CzUIGridRow* row = new CzUIGridRow(name, height, align, visible);
	row->OriginalHeight = oheight;
	RowDefinitions.push_back(row);

	return row;
}

bool CzUIGrid::setItemsData(const char* variable_name, CzXmlNode* node)
{
	if (variable_name != NULL)
	{
		CzString var_name = variable_name;
		CzXomlVariable* items_var = CzXomlVariable::GetVariable(var_name, Scene);
		if (items_var != NULL)
		{
			if (!items_var->isArray())
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - ItemsData should be an array variable type - ", variable_name, DebugInfo.c_str());
				return false;
			}
			else
			{
				int num_rows = getRowCount();
				int num_columns = getColumnCount();
				if (((CzXomlVariableArray*)items_var)->getSize() < num_rows * num_columns)
				{
/*					if (node != NULL)
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - ItemsData array is not large enough, should be >= ", CzString(num_rows * num_columns).c_str(), DebugInfo.c_str());
					else
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - ItemsData array is not large enough, should be >= ", CzString(num_rows * num_columns).c_str(), DebugInfo.c_str());
					return false;*/
					if (node == NULL)
						setItemsData((CzXomlVariableArray*)items_var, ItemsTargetTypeHash);
					else
						ItemsData = (CzXomlVariableArray*)items_var;
				}
				else
				{
					if (node == NULL)
						setItemsData((CzXomlVariableArray*)items_var, ItemsTargetTypeHash);
					else
						ItemsData = (CzXomlVariableArray*)items_var;
				}
			}
		}
		else
		{
			if (node != NULL)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - ItemsData could not be found - ", variable_name, DebugInfo.c_str());
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - ItemsData could not be found - ", variable_name, DebugInfo.c_str());
			return false;
		}
	}

	return true;
}

void CzUIGrid::setItemsData(CzXomlVariableArray* data, unsigned int target_type_hash)
{
	if (data == NULL)
		return;

	// if cell count changes then rebuild the grid
	int num_rows = getRowCount();
	int num_columns = getColumnCount();
	int new_num_rows = (data->getSize() + num_columns - 1) / num_columns;
	if (new_num_rows != num_rows)
	{
		ItemsTargetTypeHash = target_type_hash;
		ItemsData = data;
		InitGrid(new_num_rows, num_columns);
		return;
	}

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

bool CzUIGrid::setItemsData(CzXomlVariable* data)
{
	if (data == NULL)
		return true;
	if (data->isArray())
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - ItemsData should be an array variable type - ", data->Name.c_str());
		return false;
	}

	// if cell count changes then rebuild the grid
	int num_rows = getRowCount();
	int num_columns = getColumnCount();
	int new_num_rows = (((CzXomlVariableArray*)data)->getSize() + num_columns - 1) / num_columns;
	if (new_num_rows != num_rows)
	{
		ItemsData = (CzXomlVariableArray*)data;
		InitGrid(new_num_rows, num_columns);
		return true;
	}

	RemoveAllActors();
	ItemsData = (CzXomlVariableArray*)data;
	CreateAllActors();

	setLayoutDirty(true);
	UpdateAllActors(true);

	return true;
}

void CzUIGrid::setItemsTargetType(const char* type)
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

void CzUIGrid::ClearAllSelections(CzUIBase* ignore)
{
	for (int t = 0; t < RowCount * ColumnCount; t++)
	{
		CzUIBase* actor = Children.element_at(t);
		if (actor != ignore && actor != NULL)
			actor->setSelected(false);
	}
}

void CzUIGrid::setSelectionIndex(int index)
{
	if (index < 0)
	{
		if (Selection != NULL)
			Selection->setSelected(false);
		index = -1;
		Selection = NULL;
		return;
	}
	if (index >= Children.getSize())
		return;

	SelectionIndex = index;
	Selection = Children.element_at(index);
	if (Selection != NULL)
		Selection->setSelected(true);
}

bool CzUIGrid::setItemsTemplate(const char* temp_name)
{
	CzTemplate* temp = (CzTemplate*)Scene->getResourceManager()->findResource(temp_name, CzHashes::Template_Hash);
	return setItemsTemplate(temp);
}

bool CzUIGrid::setItemsTemplate(CzTemplate* temp)
{
	if (temp == NULL)
		return false;

	if (ItemsTemplate == temp)
		return true;

	RemoveAllActors();
	ItemsTemplate = temp;
	CreateAllActors();
	UpdateAllActors(true);
	setLayoutDirty(true);

	return true;
}

void CzUIGrid::LinkChanged(CzActor* child, bool remove)
{
	if (child->getCategory() != AC_UI)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Grid - Only actors that are derived from UI actor classes can be added to grid - ", child->getName().c_str(), DebugInfo.c_str());
#endif	// _DEBUG
		return;
	}

	if (remove)
		Children.remove((CzUIBase*)child);
	else
	{
		unsigned int grid_pos = child->getGridPos();
		if ((grid_pos >> 16) >= (uint)ColumnCount)
		{
#if defined(_DEBUG)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - Attempted to link actor into a grid but grid column was out of range - ", child->getName().c_str(), DebugInfo.c_str());
#endif	// _DEBUG
			return;
		}
		if ((grid_pos & 0xffff) >= (uint)RowCount)
		{
#if defined(_DEBUG)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - Attempted to link actor into a grid but grid row was out of range - ", child->getName().c_str(), DebugInfo.c_str());
#endif	// _DEBUG
			return;
		}
		int index = (grid_pos & 0xffff) * ColumnCount + (grid_pos >> 16);

//		if (Children.element_at(index) != NULL)
//			Children.element_at(index)->getScene()->removeActor(Children.element_at(index));
		Children.set(index, (CzUIBase*)child);
	}
	setLayoutDirty(true);
	NotifyParentsResize();
}

void CzUIGrid::InitGrid(int row_count, int column_count)
{
	RemoveAllActors();
//	Scene->CleanupRemovedActors();

	ColumnCount = column_count;
	RowCount = row_count;
	Children.growToSize(column_count * row_count);

	// Add / remove rows to balance row count
	int curr_row_count = RowDefinitions.size();
	if (row_count > curr_row_count)
	{
		CzUIGridRow* row0 = RowDefinitions[0];
		for (int t = 0; t < row_count - curr_row_count; t++)
			AddRow("", row0->Height,row0->Align, row0->Visible);
	}
	else
	if (row_count < curr_row_count)
	{
		CzVector<CzUIGridRow*>::iterator it = RowDefinitions.begin();
		for (int t = 0; t < curr_row_count -1; t++)
			it++;
		for (int t = row_count; t < curr_row_count; t++)
		{
			CzVector<CzUIGridRow*>::iterator it2 = it--;
			delete *it2;
			RowDefinitions.erase(it2);
		}
	}

	// Size columns
	// Calculate total size of all specified grid columns
	int total_size = 0;
	int not_set_count = 0;
	for (CzVector<CzUIGridColumn*>::iterator it = ColumnDefinitions.begin(); it != ColumnDefinitions.end(); ++it)
	{
		int d = (*it)->Width;
		if (d > 0)
			total_size += d;
		else
			not_set_count++;
	}
	if (not_set_count != 0)
	{
		// Some grid column widths were not set so we share the remaining space equally between them
		int d = (Size.x - total_size) / not_set_count;
		for (CzVector<CzUIGridColumn*>::iterator it = ColumnDefinitions.begin(); it != ColumnDefinitions.end(); ++it)
		{
			if ((*it)->Width == 0)
				(*it)->Width = d;
		}
	}

	// Size rows
	// Calculate total size of all specified grid rows
	total_size = 0;
	not_set_count = 0;
	for (CzVector<CzUIGridRow*>::iterator it = RowDefinitions.begin(); it != RowDefinitions.end(); ++it)
	{
		int d = (*it)->Height;
		if (d > 0)
			total_size += d;
		else
			not_set_count++;
	}
	if (not_set_count != 0)
	{
		// Some grid row heights  were not set so we share the remainnig space equally between them
		int d = (Size.y - total_size) / not_set_count;
		for (CzVector<CzUIGridRow*>::iterator it = RowDefinitions.begin(); it != RowDefinitions.end(); ++it)
		{
			if ((*it)->Height == 0)
				(*it)->Height = d;
		}
	}

	CreateAllActors();
	UpdateAllActors(true);
	setLayoutDirty(true);
}

bool CzUIGrid::isRowVisible(int index) const
{
	return RowDefinitions[index]->Visible;
}

void CzUIGrid::setRowVisible(int index, bool visible)
{
	RowDefinitions[index]->Visible = visible;
	index = index * ColumnCount;
	for (int t = 0; t < ColumnCount; t++)
	{
		CzUIBase* actor = Children.element_at(index);
		if (actor != NULL)
			actor->setVisible(visible);
		index++;
	}
	UpdateAllActors(false);
	setLayoutDirty(true);
}

bool CzUIGrid::isColumnVisible(int index) const
{
	return ColumnDefinitions[index]->Visible;
}

void CzUIGrid::setColumnVisible(int index, bool visible)
{
	ColumnDefinitions[index]->Visible = visible;
	for (int t = 0; t < RowCount; t++)
	{
		CzUIBase* actor = Children.element_at(index);
		if (actor != NULL)
			actor->setVisible(visible);
		index += ColumnCount;
	}
	UpdateAllActors(false);
	setLayoutDirty(true);
}

void CzUIGrid::setAllVisible(bool visible)
{
	for (int t = 0; t < RowCount * ColumnCount; t++)
	{
		CzUIBase* actor = Children.element_at(t);
		if (actor != NULL)
			actor->setVisible(visible);
	}
	for (CzVector<CzUIGridRow*>::iterator ity = RowDefinitions.begin(); ity != RowDefinitions.end(); ++ity)
		(*ity)->Visible = visible;
	for (CzVector<CzUIGridColumn*>::iterator itx = ColumnDefinitions.begin(); itx != ColumnDefinitions.end(); ++itx)
		(*itx)->Visible = visible;
	UpdateAllActors(false);
	setLayoutDirty(true);
}

void CzUIGrid::RemoveAllActors()
{
//	for (int t = 0; t < RowCount * ColumnCount; t++)
	for (int t = 0; t < Children.getSize(); t++)
	{
		CzUIBase* actor = Children.element_at(t);
		if (actor != NULL)
			actor->getScene()->removeActor(actor);
	}
	Children.clear();
}

void CzUIGrid::RemoveColumnActors(int column)
{
/*	for (int t = 0; t < RowCount; t++)
	{
		int index = t * ColumnCount + column;
		CzUIBase* actor = Children.element_at(index);
		if (actor != NULL)
		{
			actor->getScene()->removeActor(actor);
			Children.set(index, NULL);
		}
	}*/
}

void CzUIGrid::CreateAllActors()
{
	// If no items data attached the look to columns instead
	if (ItemsData == NULL)
	{
		CreateAllColumnActors();
		return;
	}

	// Find items template
	CzTemplate* temp = ItemsTemplate;
	if (temp == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Grid - No items template attached to the grid - ", DebugInfo.c_str());
#endif	// _DEBUG
		return;
	}

	// Create a set of XML attributes that will replace the template parameters
	CzXmlNode* replacements = new CzXmlNode();
	replacements->Managed = false;

	// Set base template paramater
	CzXmlAttribute* index_attrib = new CzXmlAttribute();
	index_attrib->Managed = false;
	index_attrib->setName("index");
	replacements->AddAttribute(index_attrib);

	CzXmlAttribute* gridpos_attrib = new CzXmlAttribute();
	gridpos_attrib->Managed = false;
	gridpos_attrib->setName("gridpos");
	replacements->AddAttribute(gridpos_attrib);

	CzString grid_pos;
	int index = 0;
	for (int y = 0; y < RowCount; y++)
	{
		for (int x = 0; x < ColumnCount; x++)
		{
			CzUIBase* actor = Children.element_at(index);
//			if (actor != NULL)
//				actor->getScene()->removeActor(actor);

			// Instantiate actor from item template
			index_attrib->setValue(CzString(index).c_str());
			grid_pos = CzString(x);
			grid_pos += ",";
			grid_pos += CzString(y);
			gridpos_attrib->setValue(grid_pos.c_str());
			if (!temp->Instantiate(this, replacements))
			{
#if defined(_DEBUG)
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Grid - Could not instantiate grid actor from items template - ", temp->getName().c_str(), DebugInfo.c_str());
#endif	// _DEBUG
				delete replacements;
				return;
			}

			index++;
		}
	}

	// Clean up replacement attributes
	delete replacements;
}

void CzUIGrid::CreateColumnActors(int column)
{
	RemoveColumnActors(column);
	// Check to see if columns have any bindings
	bool found = false;
	for (CzVector<CzUIGridColumn*>::iterator itx = ColumnDefinitions.begin(); itx != ColumnDefinitions.end(); ++itx)
	{
		if ((*itx)->ItemsData != NULL)
		{
			found = true;
			break;
		}
	}
	if (!found)
		return;

	CzUIGridColumn* col = ColumnDefinitions[column];
	if (col == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Grid - Column number is invalid - ", CzString(column).c_str(), DebugInfo.c_str());
#endif	// _DEBUG
		return;
	}

	// Create a set of XML attributes that will replace the template parameters
	CzXmlNode* replacements = new CzXmlNode();
	replacements->Managed = false;

	// Set base template paramater
	CzXmlAttribute* index_attrib = new CzXmlAttribute();
	index_attrib->Managed = false;
	index_attrib->setName("index");
	replacements->AddAttribute(index_attrib);

	CzXmlAttribute* gridpos_attrib = new CzXmlAttribute();
	gridpos_attrib->Managed = false;
	gridpos_attrib->setName("gridpos");
	replacements->AddAttribute(gridpos_attrib);

	CzString grid_pos;

	if (col->ItemsData != NULL)
	{
		// Find column items template
		CzTemplate* temp = col->ItemsTemplate;

		// if col;umn has not items template then look to the grids template instead
		if (temp == NULL)
			temp = ItemsTemplate;

		if (temp != NULL)
		{
			int y = 0;
			for (CzVector<CzUIGridRow*>::iterator ity = RowDefinitions.begin(); ity != RowDefinitions.end(); ++ity)
			{
				// Remove existing grid cell actor
				CzUIBase* actor = Children.element_at(y * ColumnCount + column);
//				if (actor != NULL)
//					actor->getScene()->removeActor(actor);

				// Instantiate actor from item template
				index_attrib->setValue(CzString(y * ColumnCount + column).c_str());
				grid_pos = CzString(column);
				grid_pos += ",";
				grid_pos += CzString(y);
				gridpos_attrib->setValue(grid_pos.c_str());
				if (!temp->Instantiate(this, replacements))
				{
#if defined(_DEBUG)
					CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Grid - Could not instantiate grid column actor from items template - ", temp->getName().c_str(), DebugInfo.c_str());
#endif	// _DEBUG
					delete replacements;
					return;
				}
				y++;
			}
		}
	}

	// Clean up replacement attributes
	delete replacements;
}

void CzUIGrid::CheckForColumnChanges()
{
	bool changed = false;
	int max_rows = 0;
	for (CzVector<CzUIGridColumn*>::iterator itx = ColumnDefinitions.begin(); itx != ColumnDefinitions.end(); ++itx)
	{
		if ((*itx)->ItemsData != NULL)
		{
			int num_items = (*itx)->ItemsData->getSize();
			if (num_items != (*itx)->ItemCount)
			{
				(*itx)->ItemCount = num_items;
				changed = true;
			}
			if (num_items > max_rows)
				max_rows = num_items;
		}
	}

	if (changed)
	{
		InitGrid(max_rows, ColumnDefinitions.size());
		UpdateLayout();
	}
}


void CzUIGrid::CreateAllColumnActors()
{
	int index = 0;
	for (CzVector<CzUIGridColumn*>::iterator it = ColumnDefinitions.begin(); it != ColumnDefinitions.end(); ++it)
		CreateColumnActors(index++);
}

void CzUIGrid::UpdateAllActors(bool force_update)
{
	if (ItemsData == NULL)
	{
		UpdateAllColumnActors(force_update);
		return;
	}

	for (int t = 0; t < RowCount * ColumnCount; t++)
	{
		CzUIBase* actor = Children.element_at(t);
		if (actor != NULL)
		{
			CzXomlVariable* var = ItemsData->getElement(t);
			if (var != NULL && (var->Modified || force_update))	// && !var->getValue().isEmpty())
			{
				actor->setProperty(ItemsTargetTypeHash, var->getValue(), false);
			}
		}
	}
}

void CzUIGrid::UpdateColumnActors(int column, bool force_update)
{
	// Check to see if columsn have any bindings
	bool found = false;
	for (CzVector<CzUIGridColumn*>::iterator itx = ColumnDefinitions.begin(); itx != ColumnDefinitions.end(); ++itx)
	{
		if ((*itx)->ItemsData != NULL)
		{
			found = true;
			break;
		}
	}
	if (!found)
		return;

	CzUIGridColumn* col = ColumnDefinitions[column];
	if (col == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Grid - Column number is invalid - ", CzString(column).c_str(), DebugInfo.c_str());
#endif	// _DEBUG
		return;
	}

	int y = 0;
	for (CzVector<CzUIGridRow*>::iterator ity = RowDefinitions.begin(); ity != RowDefinitions.end(); ++ity)
	{
		CzUIBase* actor = Children.element_at(y * ColumnCount + column);
		if (actor != NULL)
		{
			CzXomlVariable* var = col->ItemsData->getElement(y);
			if (var != NULL && (var->Modified || force_update))
				actor->setProperty(col->ItemsTargetTypeHash, var->getValue(), false);
		}
		y++;
	}
}

void CzUIGrid::UpdateAllColumnActors(bool force_update)
{
	int index = 0;
	for (CzVector<CzUIGridColumn*>::iterator it = ColumnDefinitions.begin(); it != ColumnDefinitions.end(); ++it)
		UpdateColumnActors(index++, force_update);
}

bool CzUIGrid::UpdateLayout()
{
	int index = 0;
	
	// Find first visible row
	CzVector<CzUIGridRow*>::iterator ity = RowDefinitions.begin();
	for (; ity != RowDefinitions.end(); ity++)
	{
		if ((*ity)->Visible)
			break;
	}
	if (ity == RowDefinitions.end())
	{
		setAllVisible(false);
		return true;
	}

	int left = -Size.x / 2 + ClippingMargin.x;
	int right = Size.x / 2 - ClippingMargin.y;
	int top = -Size.y / 2 + ClippingMargin.w;
	int bottom = Size.y / 2 - ClippingMargin.h;

	int y = -Size.y / 2 + (*ity)->Height / 2 - (int)ScrollPos.y;
	int pre_height = -1;

	ScrollRange.h = 0;
	for (ity = RowDefinitions.begin(); ity != RowDefinitions.end(); )
	{
		if ((*ity)->Visible)
		{
			eCzAlignV alignv = (*ity)->Align;
			int yo;

			if (alignv == AlignV_Top)
				yo = -(*ity)->Height / 2;
			else
			if (alignv == AlignV_Bottom)
				yo = (*ity)->Height / 2;
			else
				yo = 0;

			// Step to first visible column
			CzVector<CzUIGridColumn*>::iterator itx = ColumnDefinitions.begin();
			for (; itx != ColumnDefinitions.end(); itx++)
			{
				if ((*itx)->Visible)
					break;
			}
			if (itx == ColumnDefinitions.end())
			{
				setAllVisible(false);
				return true;
			}

			int x = -Size.x / 2;
			for (itx = ColumnDefinitions.begin(); itx != ColumnDefinitions.end(); ++itx)
			{
				if ((*itx)->Visible)
				{
					CzUIBase* actor = Children.element_at(index);
					if (actor != NULL)
					{
						CzIVec2 size = actor->getSize();
						int sx = size.x >> 1;
						int sy = size.y >> 1;
						eCzAlignH alignh = (*itx)->Align;
						CzIRect margin = actor->getMargin();
						int xo;

						if (alignh == AlignH_Left)
							xo = sx + margin.x;
						else
						if (alignh == AlignH_Right)
							xo = (*itx)->Width - sx - margin.y;
						else
							xo = (*itx)->Width / 2 + margin.x;

						float dx = (float)(x + xo) - ScrollPos.x;
						float dy = (float)(y + yo);
						if (alignv == AlignV_Top)
							dy += (float)(sy + margin.w);
						else
						if (alignv == AlignV_Bottom)
							dy += (float)(-sy - margin.h);
						else
							dy += (float)margin.w;

						actor->setPosition(dx, dy);

						if ((dy + sy) < top || (dy - sy) > bottom || (dx + sx) < left || (dx - sx) > right)
							actor->setVisible(false);
						else
							actor->setVisible(true);
					}
					x += (*itx)->Width;
				}
				else
				{
					CzUIBase* actor = Children.element_at(index);
					if (actor != NULL)
						actor->setVisible(false);
				}
				index++;
			}
			pre_height = (*ity)->Height;
			ScrollRange.h += pre_height;
			ity++;
			if (ity == RowDefinitions.end())
				break;
			y += ((*ity)->Height + pre_height) / 2;
		}
		else
		{
			for (CzVector<CzUIGridColumn*>::iterator itx = ColumnDefinitions.begin(); itx != ColumnDefinitions.end(); ++itx)
			{
				CzUIBase* actor = Children.element_at(index);
				if (actor != NULL)
					actor->setVisible(false);
				index++;
			}
			ScrollRange.h += (*ity)->Height;
			ity++;
		}
	}
	ScrollRange.h -= Size.y;
	if (ScrollRange.h < 0)
		ScrollRange.h = 0;

	// Calculate horizontal scroll range 
	ScrollRange.w = 0;
	for (CzVector<CzUIGridColumn*>::iterator itx = ColumnDefinitions.begin(); itx != ColumnDefinitions.end(); ++itx)
		ScrollRange.w += (*itx)->Width;
	ScrollRange.w -= Size.x;
	if (ScrollRange.w < 0)
		ScrollRange.w = 0;

	return CzUIBase::UpdateLayout();
}

bool CzUIGrid::Update(float dt)
{
	if (!CzUIBase::Update(dt))
		return false;

	if (ItemsData == NULL)
	{
		CheckForColumnChanges();
	}

	// Check to see if any of the contained actors have been tapped
	CzActor* last_tapped = Scene->getParent()->getLastTappedActor(0);
	if (last_tapped != NULL)
	{
		for (int t = 0; t < RowCount * ColumnCount; t++)
		{
			CzUIBase* actor = Children.element_at(t);
			if (actor != NULL)
			{
				if (actor == last_tapped)
				{
					if (MultiSelect)
					{
//						actor->setSelected(!actor->isSelected());
					}
					else
					{
						ClearAllSelections(actor);
//						actor->setSelected(true);
					}
					Selection = actor;
					NotifySelectionChanged(SelectionIndex, t);
					break;
				}
			}
		}
	}

	// Set clip rect for the grid
	CzVec4 rc;
	rc.x = (float)((-Size.x >> 1) + ClippingMargin.x);
	rc.y = (float)((-Size.y >> 1) + ClippingMargin.w);
	rc.z = (float)(Size.x - (ClippingMargin.x +  + ClippingMargin.y));
	rc.w = (float)(Size.y - (ClippingMargin.w +  + ClippingMargin.h));
	Visual->setClipRect(rc);

	UpdateAllActors(false);

	return true;
}

/**
 @fn	int CzUIGrid::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)

 @brief	Creates an instance of this class from XML

 LoadFromXoml is the main method used by all classes that can be instantiated from XOML mark-up. This method creates an instance of this class from the supplied XML node structure specified by node. 
 if parent is not NULL then the created instance will utilise the supplied parent to determine where it should live within the app. For example, if the parent is a scene then the actor will be placed 
 in that scene. if the parent is another actor then this actor will be placed into the scene of the supplied actor and linked as a child to the parent actor. If load_chlldren is true then any child 
 nodes contained within the actor will also be loaded. Note that as CzUIGrid is derived from CzUIBase, CzUIBase::LoadFromXoml() will also be called to load CzActor specific properties. 

 @param [in]	parent		If non-null, the parent.
 @param	load_children	  	true to load child nodes.
 @param [in]	node  		If non-null, the XOML markup node that defines this object

 @return	negative value for error. if 0 then processing of further XOML will stop.
 */

int CzUIGrid::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Grid - Needs to be declared inside a scene or an actor", DebugInfo.c_str());
		return 0;
	}

	// Load main actor attributes
	int ret = CzUIBase::LoadFromXoml(parent, false, node);
	if (ret <= 0)
		return ret;

	CzVec2		grid_size = CzVec2(0, 0);
	CzString*	items_data = NULL, *items_template = NULL, *items_target_type = NULL, *selection = NULL;
	bool		global = false;
	int			selection_index = -1;

	// Process grid specific attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::ItemsTargetType_Hash)
			items_target_type = &(*it)->getValue();
		else
		if (name_hash == CzHashes::ItemsData_Hash)
		{
			items_data = &(*it)->getValue();
			global = true;
		}
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
	}

	// Process any inner row / column definitions
	int num_rows = 0, num_columns = 0;
	for (CzXmlNode::_Iterator it2 = node->begin(); it2 != node->end(); ++it2)
	{
		unsigned int name_hash = (*it2)->GetName().getHash();
		if (name_hash == CzHashes::RowDefinition_Hash)
		{
			CzString	name;
			int				height = 0;
			eCzAlignV aiign = AlignV_Middle;
			bool			visible = true;

			// Get row data
			for (CzXmlNode::_AttribIterator it = (*it2)->attribs_begin(); it != (*it2)->attribs_end(); ++it)
			{
				unsigned int attrib_hash = (*it)->getName().getHash();

				if (attrib_hash == CzHashes::Name_Hash)
					name = (*it)->getValue();
				else
				if (attrib_hash == CzHashes::Height_Hash)
					height = (*it)->getValueAsInt();
				else
				if (attrib_hash == CzHashes::AlignV_Hash)
				{
					unsigned int align_hash = (*it)->getValue().getHash();
					if (align_hash == CzHashes::top_Hash)
						aiign = AlignV_Top;
					else
					if (align_hash == CzHashes::bottom_Hash)
						aiign = AlignV_Bottom;
				}
				else
				if (attrib_hash == CzHashes::Visible_Hash)
					visible = (*it)->getValueAsBool();
			}

			AddRow(name.c_str(), height, aiign, visible);
			num_rows++;
		}
		else
		if (name_hash == CzHashes::ColumnDefinition_Hash)
		{
			CzString		name;
			int				width = 0;
			eCzAlignH aiign = AlignH_Centre;
			bool			visible = true;
			bool			local = false;
			CzString		items_data2, items_template2, items_target_type2;

			// Get column data
			for (CzXmlNode::_AttribIterator it = (*it2)->attribs_begin(); it != (*it2)->attribs_end(); ++it)
			{
				unsigned int attrib_hash = (*it)->getName().getHash();

				if (attrib_hash == CzHashes::Name_Hash)
					name = (*it)->getValue();
				else
				if (attrib_hash == CzHashes::Width_Hash)
					width = (*it)->getValueAsInt();
				else
				if (attrib_hash == CzHashes::AlignH_Hash)
				{
					unsigned int align_hash = (*it)->getValue().getHash();
					if (align_hash == CzHashes::left_Hash)
						aiign = AlignH_Left;
					else
					if (align_hash == CzHashes::right_Hash)
						aiign = AlignH_Right;
				}
				else
				if (attrib_hash == CzHashes::Visible_Hash)
					visible = (*it)->getValueAsBool();
				else
				if (attrib_hash == CzHashes::ItemsTargetType_Hash)
					items_target_type2 = (*it)->getValue();
				else
				if (attrib_hash == CzHashes::ItemsData_Hash)
				{
					items_data2 = (*it)->getValue();
					local = true;
				}
				else
				if (attrib_hash == CzHashes::ItemsTemplate_Hash)
					items_template2 = (*it)->getValue();
			}

			// Add the column
			CzUIGridColumn* col = AddColumn(name.c_str(), width, aiign, visible);

			if (local && global)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - Globally bound, column binding is ignored", DebugInfo.c_str());
			else
			{
				// Find and attach the items data array
				if (!items_data2.isEmpty())
				{
					CzXomlVariable* items_var = CzXomlVariable::GetVariable(items_data2, Scene);
					if (items_var != NULL)
					{
						if (!items_var->isArray())
							CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - Column ItemsData should be an array variable type", DebugInfo.c_str());
						else
						{
//							if (((CzXomlVariableArray*)items_var)->getSize() < num_rows)
//								CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - Column ItemsData array is not large enough, should be >= num_rows", CzString(num_rows).c_str(), DebugInfo.c_str());
//							else
							{
								col->ItemsData = (CzXomlVariableArray*)items_var;
								col->ItemCount = col->ItemsData->getSize();
							}
						}
					}
					else
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - Column ItemsData could not be found - ", items_data2.c_str(), DebugInfo.c_str());
				}

				// Attach the items template
				if (!items_template2.isEmpty())
				{
					col->ItemsTemplate = (CzTemplate*)Scene->getResourceManager()->findResource(items_template2.getHash(), CzHashes::Template_Hash);
					if (col->ItemsTemplate == NULL)
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - Column ItemsTemplate could not be found - ", items_template2.c_str(), DebugInfo.c_str());
				}

				// Set Items target type
				col->ItemsTargetTypeHash = items_target_type2.getHash();
			}

			num_columns++;
		}
	}

	// Find attach the items data array
	if (items_data != NULL)
		setItemsData(items_data->c_str(), node);

	// Attach the items template
	if (items_template != NULL)
	{
		ItemsTemplate = (CzTemplate*)Scene->getResourceManager()->findResource(items_template->getHash(), CzHashes::Template_Hash);
		if (ItemsTemplate == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - ItemsTemplate could not be found - ", items_template->c_str(), DebugInfo.c_str());
	}

	if (items_target_type != NULL)
	{
		// Set Items target type
		ItemsTargetTypeHash = items_target_type->getHash();
	}

	// Find and attach the selection variable
	if (selection != NULL)
	{
		CzXomlVariable* var = CzXomlVariable::GetVariable(*selection, Scene);
		if (var == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - Selection binding variable not found - ", selection->c_str(), DebugInfo.c_str());
		SelectionVar = var;
	}

	// Initialise the grid (dont init when data is bound as it will be initialised when the data has been bound)
//	if (ItemsTemplate == NULL)
		InitGrid(num_rows, num_columns);

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

void CzUIGrid::NotifyOrientationChange(CzScene::eOrientation old_orientation, CzScene::eOrientation new_orientation)
{
	CzUIBase::NotifyOrientationChange(old_orientation, new_orientation);

	// Recalculate widths and heights of columns and rows
	for (CzVector<CzUIGridRow*>::iterator ity = RowDefinitions.begin(); ity != RowDefinitions.end(); ++ity)
	{
		if ((*ity)->OriginalHeight < 0)
			(*ity)->Height = (-(*ity)->OriginalHeight * Size.y) / 100;
	}
	for (CzVector<CzUIGridColumn*>::iterator itx = ColumnDefinitions.begin(); itx != ColumnDefinitions.end(); ++itx)
	{
		if ((*itx)->OriginalWidth < 0)
			(*itx)->Width = (-(*itx)->OriginalWidth * Size.x) / 100;
	}

	setLayoutDirty(true);
}

void CzUIGrid::NotifySelectionChanged(int old_index, int new_index)
{
	// Update attached variable
	SelectionIndex = new_index;

	if (SelectionVar != NULL)
		SelectionVar->setValue(CzString(new_index).c_str());

	ProcessEventActions(CzHashes::OnSelectionChanged_Hash);
}

/**
 @fn	bool CzUIGrid::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the actor.

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types. Note that as 
 CzUIGrid is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param	property_name		Name of the property.
 @param [out]	prop	The property data that.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIGrid::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	if (CzUIBase::getProperty(property_name, prop))
		return true;

	int err = IzXomlResource::getProperty(UIGridClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIGridClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIGrid::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the UIGrid.
    
 Sets the named property of this UIGrid. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzUIGrid::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	if (CzUIBase::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(UIGridClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIGridClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIGrid::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced. Note that as CzUIGrid is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in]	data	The property data.
 @param	delta				true to property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIGrid::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	if (CzUIBase::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(UIGridClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIGridClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIGrid::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates actor property from supplied animation.

 Updates the actors property that is specified by the supplied animation. When a timeline animation targets the property of an actor this method will be called by the animation to move the 
 animations frame data into its target property. Note that as CzUIGrid is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param [in]	animation	If non-null, the animation.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIGrid::UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzUIBase::UpdateFromAnimation(animation))
		return true;

	int err = IzXomlResource::setProperty(UIGridClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIGridClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUIGrid::UpdateBinding(unsigned int property_name, CzXomlVariable* var)

 @brief	Updates an actor property from the supplied XOML variable.

 Updates a single property from the value of the bound XOML variable. Note that as CzUIGrid is derived from CzUIBase, all CzUIBase properties will also be checked against.

 @param	property_name  	Name of the property.
 @param [in,out]	var	If non-null, the variable.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUIGrid::UpdateBinding(unsigned int property_name, CzXomlVariable* var)
{
	if (!var->Modified || var->getValue().isEmpty())
		return false;

	if (CzUIBase::UpdateBinding(property_name, var))
		return true;

	int err = IzXomlResource::setProperty(UIGridClassDef, this, property_name, var);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UIGridClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Grid - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}






