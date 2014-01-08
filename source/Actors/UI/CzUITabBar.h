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

#if !defined(_CCZ_UI_TABBAR_H_)
#define _CCZ_UI_TABBAR_H_

#include "CzUIBase.h"
#include "CzUILabel.h"
#include "CzScene.h"
#include "CzBrush.h"
#include "CzInput.h"
#include "CzUIListBox.h"
#include "CzUICanvas.h"

class CzUITabBar;

//
//
//
//
// CzUITab - A UI element that represents a tab
//
//
//
//
class CzUITab : public CzUILabel
{
	// Properties
protected:
	CzUIBase*			View;
	CzUITabBar*			TabBar;
public:
	void				setView(CzUIBase* view)			{ View = view; }
	CzUIBase*			getView()						{ return View; }
	bool				setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool				setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool				getProperty(unsigned int property_name, CzXomlProperty& prop);
	// Properties end
protected:
	bool				UpdateBinding(unsigned int property_name, CzXomlVariable* var);
public:
	CzUITab() : CzUILabel(), View(NULL), TabBar(NULL)
	{
		setActualClassType("tab");
	}

	void				InitTab();
	void				PostInitTab(CzUITabBar* tabbar);			// Called after views have been created

	// Event handlers
	void				NotifyToggledOn();
	void				NotifyToggledOff();
	void				NotifyOrientationChange(CzScene::eOrientation old_orientation, CzScene::eOrientation new_orientation);

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Implementation of IzAnimTarget interface
	bool				UpdateFromAnimation(CzAnimInstance *animation);

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	UITabClassDef;								// XOML class definition
public:
	static void				InitClass();
	static void				ReleaseClass();

	static CzXomlProperty	_getView(IzXomlResource* target);
	static bool				_setView(IzXomlResource* target, const CzXomlProperty& prop, bool add);
};

//
//  CzUITabCreator - Creates an instance of a tab actor object
//
class CzUITabCreator : public IzXomlClassCreator
{
public:
	CzUITabCreator()
	{
		setClassName("tab");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzUITab(); }
};

//
//
//
//
// CzUITabs - A UI element that represents a collection of tabs
//
//
//
//
class CzUITabs : public CzUIListBox
{
	// Properties
protected:
public:
	// Properties end
protected:
public:
	CzUITabs() : CzUIListBox()
	{
		setActualClassType("tabs");
	}

	void				InitTabs();
	void				PostInitTabs();		// Called after views have been created

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);
};

//
//  CzUITabsCreator - Creates an instance of a tabs actor object
//
class CzUITabsCreator : public IzXomlClassCreator
{
public:
	CzUITabsCreator()
	{
		setClassName("tabs");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzUITabs(); }
};



//
//
//
//
// CzUITabBar - UI class that allows the user to select between different groups of controls using tabs
//
//
//
//
class CzUITabBar : public CzUICanvas
{
public:
	// Properties
protected:
	eCzOrientation				Orientation;			///< Stacking orientation (set from the tabs list box)
	bool						AutoHide;				///< If set to true then the outgoing view will automatically be hidden
public:
	void						setAutoHide(bool enable)		{ AutoHide = enable; }
	bool						isAutoHide() const				{ return AutoHide; }
	eCzOrientation				getOrientation() const			{ return Orientation; }
	bool						setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool						setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool						getProperty(unsigned int property_name, CzXomlProperty& prop);
	CzUITabs*					getTabs();
	bool						UpdateBinding(unsigned int property_name, CzXomlVariable* var);
	// Properties end
protected:
	bool						UpdateLayout();
	bool						UpdatePanels();
	CzUIBase*					PrevView;
	void						LinkChanged(CzActor* child, bool remove);

public:
	CzUITabBar() : CzUICanvas(), PrevView(NULL), AutoHide(true), Orientation(Orientation_Vertical)	{ setActualClassType("tabbar"); }

	void				InitTabBar();
	bool				Update(float dt);

	// Event handlers
	void				NotifyViewChanged(CzUIBase* prev_view, CzUIBase* new_view);

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Implementation of IzAnimTarget interface
	bool				UpdateFromAnimation(CzAnimInstance *animation);

	void				ShowView(CzUIBase* view, bool show);

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	UITabBarClassDef;								// XOML class definition
public:
	static void				InitClass();
	static void				ReleaseClass();

	static CzXomlProperty	_getOrientation(IzXomlResource* target);
	static bool				_setAutoHide(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getAutoHide(IzXomlResource* target);
	static bool				_addTab(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_removeTab(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnViewChanged(IzXomlResource* target, const CzXomlProperty& prop, bool add);
};

//
//  CzUITabBarCreator - Creates an instance of a tabbar actor object
//
class CzUITabBarCreator : public IzXomlClassCreator
{
public:
	CzUITabBarCreator()
	{
		setClassName("tabbar");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzUITabBar(); }
};


#endif	// _CCZ_UI_TABBAR_H_

