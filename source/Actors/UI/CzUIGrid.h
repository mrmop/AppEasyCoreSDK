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

#if !defined(_CCZ_UI_GRID_H_)
#define _CCZ_UI_GRID_H_

#include "CzActor.h"
#include "CzScene.h"
#include "CzUIBase.h"
#include "CzTemplates.h"
#include "CzHashes.h"

/**
 @struct	CzUIGridColumn

 @brief	A grid column.

 */

struct CzUIGridColumn
{
	CzString			Name;					///< Name of column
	int					OriginalWidth;			///< Original width of column
	int					Width;					///< Current width of column
	eCzAlignH			Align;					///< Column actor alignment
	bool				Visible;				///< Column visibility
	CzTemplate*			ItemsTemplate;			///< Items template used to instantiate grid column items
	CzXomlVariableArray* ItemsData;				///< Bound data array
	unsigned int		ItemsTargetTypeHash;		///< Target item type hash
	int					ItemCount;				///< Number of items in column
public:

	CzUIGridColumn(const char* name, int width, eCzAlignH align, bool visible)
	{
		Name = name;
		Width = width;
		Align = align;
		Visible = visible;
		ItemsTemplate = NULL;
		ItemsData = NULL;
		ItemsTargetTypeHash = 0;
		ItemCount = 0;
	}
};

/**
 @struct	CzUIGridRow

 @brief	A grid row.

 */

struct CzUIGridRow
{
	CzString			Name;					///< Name of row
	int					OriginalHeight;			///< Original height of row
	int					Height;					///< Current height of row
	eCzAlignV			Align;					///< Row actor alignment
	bool				Visible;				///< Row visibility

	CzUIGridRow(const char* name, int height, eCzAlignV align, bool visible)
	{
		Name = name;
		Height = height;
		Align = align;
		Visible = visible;
	}
};

/**
 @class	CzUIGrid

 @brief	An image based actor that acts as a grid, allowing other actors to be arranged into a grid.

 */

class CzUIGrid : public CzUIBase
{
	// Properties
protected:
	int							RowCount;				// Number of rows
	int							ColumnCount;			// Number of columns
	CzVector<CzUIGridColumn*>	ColumnDefinitions;		// Definition of columns
	CzVector<CzUIGridRow*>		RowDefinitions;			// Definition of rows
	CzTemplate*					ItemsTemplate;			// Items template used to instantiate grid actor items
	CzXomlVariableArray*		ItemsData;				// Bound data array
	unsigned int				ItemsTargetTypeHash;		// Target item type hash
	bool						MultiSelect;			// If true then multiple selections are allowed
	CzUIBase*					Selection;				// Last selections actor
	int							SelectionIndex;			// Last selection index
	CzXomlVariable*				SelectionVar;			// Variable that is updated with the current selection index
public:
	int							getRowCount() const						{ return RowCount; }
	int							getColumnCount() const					{ return ColumnCount; }
	CzUIGridColumn* 			AddColumn(const char* name, int width, eCzAlignH align, bool visible);
	CzUIGridRow*				AddRow(const char* name, int height, eCzAlignV align, bool visible);
	CzUIGridRow*				getRow(int index)						{ if (index < 0 || index >= RowCount) return NULL; else return RowDefinitions[index]; }
	CzUIGridColumn*				getColumn(int index)					{ if (index < 0 || index >= ColumnCount) return NULL; else return ColumnDefinitions[index]; }
	void						setRowVisible(int index, bool visible);
	bool						isRowVisible(int index) const;
	void						setColumnVisible(int index, bool visible);
	bool						isColumnVisible(int index) const;
	void						setAllVisible(bool visible);
	CzTemplate*					getItemsTemplate() const				{ return ItemsTemplate; }
	bool						setItemsTemplate(const char* temp_name);
	bool						setItemsTemplate(CzTemplate* temp);
	bool						setItemsData(const char* variable_name, CzXmlNode* node = NULL);
	void						setItemsData(CzXomlVariableArray* data, unsigned int  type_hash);
	bool						setItemsData(CzXomlVariable* data);
	CzXomlVariableArray*		getItemsData()							{ return ItemsData; }
	void						setItemsTargetType(const char* type);
	unsigned int				setItemsTargetType() const				{ return ItemsTargetTypeHash; }
	void						setMultiSelect(bool enable)				{ MultiSelect = enable; }
	bool						isMultiSelect() const					{ return MultiSelect; }
	void						setSelectionIndex(int index);
	int							getSelectionIndex() const				{ return SelectionIndex; }
	CzUIBase*					getSelection()							{ return Selection; }
	void						setSelectionVar(CzXomlVariable* var) { SelectionVar = var; }
	bool						setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool						setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool						getProperty(unsigned int property_name, CzXomlProperty& prop);
	// Properties end
protected:
	CzSlotArray<CzUIBase*>		Children;											// Collection of actors that are linked to this panel
	void						LinkChanged(CzActor* child, bool remove);			// Links a new actor into the grid
	void						RemoveActor(CzActor* actor);						// Removes the specified actor from the grid
	void						RemoveAllActors();									// Removes and destroys all actiors in the grid
	void						CreateAllActors();									// Re-creates all actors within the grid from a template
	void						UpdateAllActors(bool force_update);					// Moves data from the bound items data array to the actors
	void						RemoveColumnActors(int column);						// Removes all grid actors in a column
	void						CreateColumnActors(int column);						// Creates the grid actors for a column
	void						CreateAllColumnActors();							// Creates the grid actors for all columns
	void						UpdateColumnActors(int column, bool force_update);	// Moves data from the bound column items data array to the column actors
	void						UpdateAllColumnActors(bool force_update);			// Updates all grid column actors
	bool						UpdateLayout();										// Updates the layout of the child items
	void						CheckForColumnChanges();
	bool						UpdateBinding(unsigned int property_name, CzXomlVariable* var);

public:
	CzUIGrid() : CzUIBase(), RowCount(0), ColumnCount(0), ItemsData(NULL), ItemsTemplate(NULL), ItemsTargetTypeHash(0), MultiSelect(false), SelectionIndex(-1), Selection(NULL), SelectionVar(NULL)
								{ setActualClassType("grid"); ReceiveEventFromChild = true; }
	virtual ~CzUIGrid();

	virtual void				InitGrid(int row_count, int column_count);
	bool						Update(float dt);

	// Event handlers
	void						NotifyOrientationChange(CzScene::eOrientation old_orientation, CzScene::eOrientation new_orientation);
	virtual void				NotifySelectionChanged(int old_index, int new_index);

	// Implementation of IzXomlClass interface
	int							LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Implementation of IzAnimTarget interface
	bool						UpdateFromAnimation(CzAnimInstance *animation);

	// Utility
	void						ClearAllSelections(CzUIBase* ignore);

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	UIGridClassDef;								// XOML class definition

public:
	static void				InitClass();
	static void				ReleaseClass();

	static CzXomlProperty	_getColumnCount(IzXomlResource* target);
	static CzXomlProperty	_getRowCount(IzXomlResource* target);
	static bool				_setItemsTemplate(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getItemsTemplate(IzXomlResource* target);
	static bool				_setItemsData(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getItemsData(IzXomlResource* target);
	static bool				_setItemsTargetType(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnSelectionChanged(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setMultiSelect(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getMultiSelect(IzXomlResource* target);
	static CzXomlProperty	_getSelection(IzXomlResource* target);
	static bool				_setSelectionIndex(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSelectionIndex(IzXomlResource* target);
	static bool				_setShowRow(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setShowColumn(IzXomlResource* target, const CzXomlProperty& prop, bool add);

};
/**
 @class	CzUIGridCreator

 @brief	Creates an instance of a grid actor object.

 Used by the XOML system to instantiate a CzUIGrid object.

 */

class CzUIGridCreator : public IzXomlClassCreator
{
public:
	CzUIGridCreator()
	{
		setClassName("grid");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzUIGrid(); }
};



#endif	// _CCZ_UI_GRID_H_
