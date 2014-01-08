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

#if !defined(_CCZ_UI_LISTBOX_H_)
#define _CCZ_UI_LISTBOX_H_

#include "CzActor.h"
#include "CzScene.h"
#include "CzUIBase.h"
#include "CzTemplates.h"

//
//
//
//
// CzUIListBox - An image based game actor hthat acts as a list box, allowing the selection from a list of items
//
//
//
//
struct CzUIListBoxItem
{
	CzUIBase*		Actor;			// Actor that represents this list box item
	bool			Visible;		// Visible state

	CzUIListBoxItem()
	{
		Actor = NULL;
		Visible = true;
	}
};

class CzUIListBox : public CzUIBase
{
public:
	// Public access to items iteration
	typedef CzVector<CzUIListBoxItem*>::iterator _Iterator;
	_Iterator					begin() { return Children.begin(); }
	_Iterator					end() { return Children.end(); }

	// Properties
protected:
	eCzOrientation			Orientation;			// Stacking orientation
	eCzAlignH				AlignH;					// Horizontal alignment of contained actors (vertical only)
	eCzAlignV				AlignV;					// Horizontal alignment of contained actors (horizontal only)
#if defined(_DEBUG)
	CzString				ItemsTargetType;		// Target type for items (Text, Brush etc..)
#endif	// _DEBUG
	CzTemplate*				ItemsTemplate;			// Items template used to instantiate grid actor items
	CzXomlVariableArray*	ItemsData;				// Bound data array
	unsigned int			ItemsTargetTypeHash;	// Target item type hash
	bool					MultiSelect;			// If true then multiple selections are allowed
	CzUIBase*				Selection;				// Last selections actor
	int						SelectionIndex;			// Last selection index
	CzXomlVariable*			SelectionVar;			// Variable that is updated with the current selection index
	int						CaretIndex;				// Caret index
	CzIRect					CaretColourOffset;		// Amount the caret offsets the selected control
public:
	void					setOrientation(eCzOrientation o)		{ Orientation = o; setLayoutDirty(true); }
	eCzOrientation			getOrientation() const					{ return Orientation; }
	void					setAlignH(eCzAlignH align)				{ AlignH = align; setLayoutDirty(true); }
	eCzAlignH				getAlignH() const						{ return AlignH; }
	void					setAlignV(eCzAlignV align)				{ AlignV = align; setLayoutDirty(true); }
	eCzAlignV				getAlignV() const						{ return AlignV; }
	int						getItemCount() const					{ return Children.size(); }
	void					addItem(CzUIListBoxItem* item);
	void					removeItem(CzUIListBoxItem* item);
	CzUIListBoxItem*		getItem(int index)						{ return Children[index]; }
	CzUIListBoxItem*		findItem(CzUIBase* actor);
	void					setAllVisible(bool visible);
	bool					setItemsTemplate(const char* temp_name);
	bool					setItemsTemplate(CzTemplate* temp);
	CzTemplate*				getItemsTemplate()						{ return ItemsTemplate; }
	bool					setItemsData(const char* variable_name, CzXmlNode* node);
	void					setItemsData(CzXomlVariableArray* data, unsigned int  type_hash);
	bool					setItemsData(CzXomlVariable* data);
	CzXomlVariableArray*	getItemsData()							{ return ItemsData; }
	void					setItemsTargetType(const char* type);
	unsigned int			getItemsTargetType() const				{ return ItemsTargetTypeHash; }
	void					setMultiSelect(bool enable)				{ MultiSelect = enable; }
	bool					isMultiSelect() const					{ return MultiSelect; }
	void					setSelectionIndex(int index, bool call_events = false);
	int						getSelectionIndex() const				{ return SelectionIndex; }
	CzUIBase*				getSelection()							{ return Selection; }
	void					setSelectionVar(CzXomlVariable* var)	{ SelectionVar = var; }
	void					setCaretIndex(int index);
	int						getCaretIndex() const					{ return CaretIndex; }
	CzIRect					getCaretColourOffset() const			{ return CaretColourOffset; }
	void					setCaretColourOffset(const CzIRect& offset)	{ CaretColourOffset = offset; }
	bool					setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool					setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool					getProperty(unsigned int property_name, CzXomlProperty& prop);
	// Properties end
protected:
	int						ItemsCount;								///< Total items
	CzVector<CzUIListBoxItem*> Children;							// Collection of items that are linked to this list box
	void					LinkChanged(CzActor* child, bool remove);
	void					RemoveActor(CzActor* actor);
	void					RemoveAllActors();						// Removes and destroys all actors in the list box
	void					CreateAllActors();						// Re-creates all actors within the list box from a template
	void					UpdateAllActors(bool force_update);		// Moves data from the bound items data array to the list box actors
	bool					UpdateLayout();							// Updates the layout of the child items
	CzVec2					CalculateItemPosition(int item_index);
	bool					UpdateBinding(unsigned int property_name, CzXomlVariable* var);
public:
	CzUIListBox() : CzUIBase(), AlignH(AlignH_Centre), AlignV(AlignV_Middle), ItemsData(NULL), ItemsTargetTypeHash(0), MultiSelect(false), SelectionIndex(-1), Selection(NULL), 
								SelectionVar(NULL), Orientation(Orientation_Vertical), ItemsCount(-1), CaretIndex(-1), CaretColourOffset(0, 0, 0, 0)
								{ setActualClassType("listbox"); ReceiveEventFromChild = true; }
	virtual ~CzUIListBox();

	virtual void				InitListBox();
	bool						Update(float dt);

	// Event handlers
	virtual void				NotifySelectionChanged(int old_index, int new_index);

	// Implementation of IzXomlClass interface
	int							LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Implementation of IzAnimTarget interface
	bool						UpdateFromAnimation(CzAnimInstance *animation);

	// Utility
	void						ClearAllSelections(CzUIBase* ignore);
	void						ClearAllToggles(CzUIBase* ignore);
	void						ScrollToIndex(int index);

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	UIListBoxClassDef;								// XOML class definition
public:
	static void				InitClass();
	static void				ReleaseClass();

	static CzXomlProperty	_getItemCount(IzXomlResource* target);
	static bool				_setOrientation(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getOrientation(IzXomlResource* target);
	static bool				_setAlignH(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setAlignV(IzXomlResource* target, const CzXomlProperty& prop, bool add);
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
	static bool				_setCaretColourOffset(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getCaretColourOffset(IzXomlResource* target);
	static bool				_setCaretIndex(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getCaretIndex(IzXomlResource* target);
};

//
//  CzUIListBoxCreator - Creates an instance of a list box actor object
//
class CzUIListBoxCreator : public IzXomlClassCreator
{
public:
	CzUIListBoxCreator()
	{
		setClassName("listbox");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzUIListBox(); }
};

//
//
//
//
//  Helper macros
//
//
//
//
#define CZ_NEW_LISTBOX(scene, listbox, name, background, width, height)				\
	CzUIListBox* listbox = new CzUIListBox();										\
	listbox->setName(name);															\
	scene->addActor(listbox);														\
	listbox->Init(background, width, height);										\
	listbox->InitListBox();




#endif	// _CCZ_UI_LISTBOX_H_
