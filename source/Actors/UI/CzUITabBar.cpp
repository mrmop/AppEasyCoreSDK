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

#include "CzUITabBar.h"
#include "CzHashes.h"


//
//
//
//
// CzUITab implementation
//
//
//
//

CzXomlClassDef* CzUITab::UITabClassDef = NULL;

/**
 @fn	void CzUITab::InitClass()

 @brief	Initialises the class.
    
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzUITab::InitClass()
{
	// Create class properties
	UITabClassDef = new CzXomlClassDef();
	UITabClassDef->addProperty(new CzXomlClassProperty("View",		PT_Resource,	CzUITab::_setView,	CzUITab::_getView));
}
void CzUITab::ReleaseClass()
{
	if (UITabClassDef != NULL)
	{
		UITabClassDef->Release();
		delete UITabClassDef;
		UITabClassDef = NULL;
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
// CzUITab XOML property setters and getters
// //
// //
// //
// //
// //
// //
// //
// //

bool CzUITab::_setView(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUITab* actor = (CzUITab*)target;
	CzActor* parent;
	if (prop.Type == PT_Resource)
		parent = (CzActor*)prop.p_data;
	else
		parent = actor->getScene()->findActor((const char*)prop.p_data);

	if (parent != NULL)
		actor->setView((CzUIBase*)parent);
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Tab - setProperty - view actor not found - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());
		return false;
	}

	if (parent->getCategory() != AC_UI)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Tab - setProperty - view actor must be derived from UIbase - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());
		return false;
	}

	return true;
}

CzXomlProperty	CzUITab::_getView(IzXomlResource* target)
{
	return CzXomlProperty(((CzUITab*)target)->getView());
}

void CzUITab::InitTab()
{
}

void CzUITab::PostInitTab(CzUITabBar* tabbar)
{
	TabBar = tabbar;

	if (Selected)
		setToggledOn(true);
}

void CzUITab::NotifyToggledOn()
{
	CzUIBase::NotifyToggledOn();

	if (TabBar != NULL)
		TabBar->ShowView(View, true);
}

void CzUITab::NotifyToggledOff()
{
	CzUIBase::NotifyToggledOff();

	if (TabBar != NULL)
		TabBar->ShowView(View, false);
}

void CzUITab::NotifyOrientationChange(CzScene::eOrientation old_orientation, CzScene::eOrientation new_orientation)
{
	CzUILabel::NotifyOrientationChange(old_orientation, new_orientation);
}

int CzUITab::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Tab - Tab needs to be declared inside a scene or an actor - ", DebugInfo.c_str());
		return 0;
	}

	// Load main actor attributes
	int ret = CzUILabel::LoadFromXoml(parent, false, node);
	if (ret <= 0)
		return ret;

	InitTab();

	if (load_children)
	{
		// Process inner tags
		if (!CZ_XOML->Process(this, node))
			return 0;
	}

	return 1;
}

/**
 @fn	bool CzUITab::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the actor.

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types. Note that as 
 CzUITab is derived from CzUILabel, all CzUILabel properties will also be checked against.

 @param	property_name		Name of the property.
 @param [out]	prop	The property data that.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUITab::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	if (CzUILabel::getProperty(property_name, prop))
		return true;

	int err = IzXomlResource::getProperty(UITabClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UITabClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Tab - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUITab::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the UITab.
    
 Sets the named property of this UITab. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzUITab::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	if (CzUILabel::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(UITabClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UITabClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Tab - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUITab::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced. Note that as CzUITab is derived from CzUILabel, all CzUILabel properties will also be checked against.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in]	data	The property data.
 @param	delta				true to property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUITab::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	if (CzUILabel::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(UITabClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UITabClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Tab - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUITab::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates actor property from supplied animation.

 Updates the actors property that is specified by the supplied animation. When a timeline animation targets the property of an actor this method will be called by the animation to move the 
 animations frame data into its target property. Note that as CzUITab is derived from CzUILabel, all CzUILabel properties will also be checked against.

 @param [in]	animation	If non-null, the animation.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUITab::UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzUILabel::UpdateFromAnimation(animation))
		return true;

	int err = IzXomlResource::setProperty(UITabClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UITabClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Tab - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}

/**
 @fn	bool CzUITab::UpdateBinding(unsigned int property_name, CzXomlVariable* var)

 @brief	Updates an actor property from the supplied XOML variable.

 Updates a single property from the value of the bound XOML variable. Note that as CzUITab is derived from CzUILabel, all CzUILabel properties will also be checked against.

 @param	property_name  	Name of the property.
 @param [in,out]	var	If non-null, the variable.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUITab::UpdateBinding(unsigned int property_name, CzXomlVariable* var)
{
	if (!var->Modified || var->getValue().isEmpty())
		return false;

	if (CzUILabel::UpdateBinding(property_name, var))
		return true;

	int err = IzXomlResource::setProperty(UITabClassDef, this, property_name, var);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UITabClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Tab - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str(), DebugInfo.c_str());
	}

	return true;
}



//
//
//
//
// CzUITabs implementation
//
//
//
//
void CzUITabs::InitTabs()
{
}

int CzUITabs::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Tabs - Tabs needs to be declared inside a scene or an actor", DebugInfo.c_str());
		return 0;
	}

	Orientation = Orientation_Horizontal;

	// Load main actor attributes
	int ret = CzUIListBox::LoadFromXoml(parent, false, node);
	if (ret <= 0)
		return ret;

	InitTabs();

	if (load_children)
	{
		// Process inner tags
		if (!CZ_XOML->Process(this, node))
			return 0;
	}

	return 1;
}

//
//
//
//
// CzUITabBar implementation
//
//
//
//

CzXomlClassDef* CzUITabBar::UITabBarClassDef = NULL;

/**
 @fn	void CzUILabel::InitClass()

 @brief	Initialises the class.
    
 Sets up the classes avaiiable properties. Used by the XOML system to find, set amd get class properties.

 */

void CzUITabBar::InitClass()
{
	// Create class properties
	UITabBarClassDef = new CzXomlClassDef();
	UITabBarClassDef->addProperty(new CzXomlClassProperty("AutoHide",		PT_Bool,		CzUITabBar::_setAutoHide,	CzUITabBar::_getAutoHide));
	UITabBarClassDef->addProperty(new CzXomlClassProperty("OnViewChanged",	PT_String,		CzUITabBar::_setOnViewChanged,	NULL));
	UITabBarClassDef->addProperty(new CzXomlClassProperty("AddTab",			PT_Resource,	CzUITabBar::_addTab,	NULL));
	UITabBarClassDef->addProperty(new CzXomlClassProperty("RemoveTab",		PT_Resource,	CzUITabBar::_removeTab,	NULL));
}
void CzUITabBar::ReleaseClass()
{
	if (UITabBarClassDef != NULL)
	{
		UITabBarClassDef->Release();
		delete UITabBarClassDef;
		UITabBarClassDef = NULL;
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
// CzUITabBar XOML property setters and getters
// //
// //
// //
// //
// //
// //
// //
// //

CzXomlProperty	CzUITabBar::_getOrientation(IzXomlResource* target)
{
	return CzXomlProperty(CzXoml::OrientationNames[((CzUITabBar*)target)->getOrientation()]);
}

bool CzUITabBar::_setAutoHide(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUITabBar* actor = (CzUITabBar*)target;

	if (add)
		actor->setAutoHide(!actor->isAutoHide());
	else
		actor->setAutoHide(prop.p_bool);

	return true;
}

CzXomlProperty	CzUITabBar::_getAutoHide(IzXomlResource* target)
{
	return CzXomlProperty(((CzUITabBar*)target)->isAutoHide());
}

bool CzUITabBar::_addTab(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUITabBar* actor = (CzUITabBar*)target;
	CzUITab* tab;
	if (prop.Type == PT_Resource)
		tab = (CzUITab*)prop.p_data;
	else
		tab = (CzUITab*)actor->getScene()->findActor((const char*)prop.p_data);
	if (tab == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "TabBar - AddTab - Tab actor not found - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());
		return false;
	}
	if (tab->getActualClassTypeHash() != CzHashes::Tab_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "TabBar - AddTab - actor must be derived from Tab actor - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());
		return false;
	}

	CzUITabs* tabs = actor->getTabs();
	if (tabs == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "TabBar - AddTab - no Tabs actor found - ", actor->getName().c_str());
		return false;
	}

	tab->setLinkedTo(tabs);
	tab->setSelected(false, true);
	actor->InitTabBar();

	return true;
}

bool CzUITabBar::_removeTab(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUITabBar* actor = (CzUITabBar*)target;
	CzUITab* tab;
	if (prop.Type == PT_Resource)
		tab = (CzUITab*)prop.p_data;
	else
		tab = (CzUITab*)actor->getScene()->findActor((const char*)prop.p_data);
	if (tab == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "TabBar - AddTab - Tab actor not found - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());
		return false;
	}
	if (tab->getActualClassTypeHash() != CzHashes::Tab_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "TabBar - AddTab - actor must be derived from Tab actor - ", (const char*)prop.p_data, actor->getDebugInfo().c_str());
		return false;
	}

	if (tab->getView() != NULL)
		tab->getView()->setLinkedTo(NULL);
	tab->setLinkedTo(NULL);
	actor->InitTabBar();

	return true;
}

bool CzUITabBar::_setOnViewChanged(IzXomlResource* target, const CzXomlProperty& prop, bool add)
{
	CzUITabBar* actor = (CzUITabBar*)target;

	actor->getEventsManager()->addEvent("OnViewChanged", (const char*)prop.p_data, true);

	return true;
}

void CzUITabBar::LinkChanged(CzActor* child, bool remove)
{
	if (child->getCategory() != AC_UI)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "TabBar - Only actors that are derived from UI actor classes can be added to tabbar - ", child->getName().c_str());
#endif	// _DEBUG
		return;
	}

	CzUIPanel::LinkChanged(child, remove);
}

void CzUITabBar::ShowView(CzUIBase* view, bool show)
{
	if (AutoHide || show)
		view->setVisible(show);

	if (show)
	{
		NotifyViewChanged(PrevView, view);
		PrevView = view;
	}
}

bool CzUITabBar::Update(float dt)
{
	return CzUICanvas::Update(dt);
}

bool CzUITabBar::UpdatePanels()
{
	// Position tabs and views
	bool top = true;
	_Iterator it = begin();
	if (it != end())
	{
		if ((*it)->getActualClassTypeHash() != CzHashes::Tabs_Hash)
			top = false;
	}

	if (Orientation == Orientation_Horizontal)
	{
		float dy = (float)-Size.y / 2;

		for (; it != end(); ++it)
		{
			CzVec2 pos = (*it)->getPosition();
			CzIVec2 size = (*it)->getSize();
			CzIRect margin = (*it)->getMargin();

			float y;
			if ((*it)->getActualClassTypeHash() == CzHashes::Tabs_Hash)
			{
				if (top)
					y = dy + size.y / 2 + margin.w;
				else
					y = (float)Size.y / 2 - size.y / 2 - margin.h;

				dy += size.y + margin.w + margin.h;
			}
			else
			{
				if (top)
					y = dy + size.y / 2 + margin.w;
				else
					y = dy + size.y / 2 + margin.w;

			}
			(*it)->setPosition(pos.x, y);
			(*it)->setOriginalPosition(0, y);
		}
	}
	else
	{
		float dx = (float)-Size.x / 2;

		for (; it != end(); ++it)
		{
			CzVec2 pos = (*it)->getPosition();
			CzIVec2 size = (*it)->getSize();
			CzIRect margin = (*it)->getMargin();

			float x;
			if ((*it)->getActualClassTypeHash() == CzHashes::Tabs_Hash)
			{
				if (top)
					x = dx + size.x / 2 + margin.x;
				else
					x = (float)Size.x / 2 - size.x / 2 - margin.y;

				dx += size.x + margin.x + margin.y;
			}
			else
			{
				if (top)
					x = dx + size.x / 2 + margin.x;
				else
					x = dx + size.x / 2 + margin.x;

			}
			(*it)->setPosition(x, pos.y);
			(*it)->setOriginalPosition(x, 0);
		}
	}

	return true;
}

bool CzUITabBar::UpdateLayout()
{
	UpdatePanels();

	return CzUIBase::UpdateLayout();
}

void CzUITabBar::InitTabBar()
{
	// Find the tabs actor
	CzUITabs* tabs = getTabs();
	if (tabs != NULL)
	{
		// Get orientation
		Orientation = tabs->getOrientation();

		// Hook views up to tabs
		CzUIListBox::_Iterator it2 = tabs->begin();
		for (_Iterator it = begin(); it != end(); ++it)
		{
			CzUIBase* view = (CzUIBase*)*it;
			if (view->getActualClassTypeHash() != CzHashes::Tabs_Hash)	// Ignore the tabs container
			{
				view->setVisible(false, true);
				CzUITab* tab = (CzUITab*)(*it2)->Actor;
				tab->setView(view);
				tab->PostInitTab(this);

				if (!tab->isSelected())
				{
					CzAnimTimeline* timeline = view->getShowTimeline();
					if (timeline != NULL)
						timeline->ForceUpdateTargetOnly();
				}

				it2++;
				if (it2 == tabs->end())	// Ran out of tabs
					break;
			}
		}
	}

	UpdatePanels();
}

CzUITabs* CzUITabBar::getTabs()
{
	// Find the tabs actor
	CzUITabs* tabs = NULL;
	for (_Iterator it = begin(); it != end(); ++it)
	{
		if ((*it)->getActualClassTypeHash() == CzHashes::Tabs_Hash)
			return (CzUITabs*)*it;
	}

	return NULL;
}

int CzUITabBar::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	if (parent->getClassTypeHash() != CzHashes::Scene_Hash && parent->getClassTypeHash() != CzHashes::Actor_Hash)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "TabBar - TabBar needs to be declared inside a scene or an actor", DebugInfo.c_str());
		return false;
	}

	// Load main actor attributes
	int ret = CzUICanvas::LoadFromXoml(parent, false, node);
	if (ret <= 0)
		return ret;

	// Process attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();
		if (name_hash == CzHashes::OnViewChanged_Hash)
			EventsManager->addEvent("OnViewChanged", (*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::AutoHide_Hash)
			AutoHide = (*it)->getValueAsBool();
	}

	if (load_children)
	{
		// Process inner tags
		if (!CZ_XOML->Process(this, node))
			return 0;
	}

	InitTabBar();

	return 1;
}

void CzUITabBar::NotifyViewChanged(CzUIBase* prev_view, CzUIBase* new_view)
{
	ProcessEventActions(CzHashes::OnViewChanged_Hash);
}

/**
 @fn	bool CzUITabBar::getProperty(const char* property_name, CzXomlProperty& prop)

 @brief	Gets the named property of the actor.

 Returns the named property of the actor. The properties value is returned in a CzXomlProperty which is a structure that contains a union of all possible property types. Note that as 
 CzUITabBar is derived from CzUICanvas, all CzUICanvas properties will also be checked against.

 @param	property_name		Name of the property.
 @param [out]	prop	The property data that.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUITabBar::getProperty(unsigned int property_name, CzXomlProperty& prop)
{
	if (CzUICanvas::getProperty(property_name, prop))
		return true;

	int err = IzXomlResource::getProperty(UITabBarClassDef, this, property_name, prop);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UITabBarClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "TabBar - getProperty - Cannot get this property type, check the properties format - ", cprop->getName().c_str());
	}

	return true;
}

/**
 @fn	bool CzUITabBar::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)

 @brief	Sets the named property of the UITabBar.
    
 Sets the named property of this UITabBar. The properties value supplied will be converted. If delta is set to true then the existing value of the property will 
 be added to instead of replaced.

 @param	property_name	Name of the property as a string hash (faster searching).
 @param [in]	data 	The property data.
 @param	delta		 	true if property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */
bool CzUITabBar::setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta)
{
	if (CzUICanvas::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(UITabBarClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UITabBarClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "TabBar - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str());
	}

	return true;
}

/**
 @fn	bool CzUITabBar::setProperty(unsigned int property_name, const CzString& data, bool delta)

 @brief	Sets the named property of the actor.

 Sets the named property of this actor. The properties value (data) is supplied as a string and will be converted. If delta is set to true then the existing value of the property 
 will be added to instead of replaced. Note that as CzUITabBar is derived from CzUICanvas, all CzUICanvas properties will also be checked against.

 @param	property_name   	Name of the property as a string hash (faster searching).
 @param [in]	data	The property data.
 @param	delta				true to property should be added to instead of replaced.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUITabBar::setProperty(unsigned int property_name, const CzString& data, bool delta)
{
	if (CzUICanvas::setProperty(property_name, data, delta))
		return true;

	int err = IzXomlResource::setProperty(UITabBarClassDef, this, property_name, data, delta);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UITabBarClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "TabBar - setProperty - Cannot set this property type, check the properties format - ", cprop->getName().c_str());
	}

	return true;
}

/**
 @fn	bool CzUITabBar::UpdateFromAnimation(CzAnimInstance *animation)

 @brief	Updates actor property from supplied animation.

 Updates the actors property that is specified by the supplied animation. When a timeline animation targets the property of an actor this method will be called by the animation to move the 
 animations frame data into its target property. Note that as CzUITabBar is derived from CzUICanvas, all CzUICanvas properties will also be checked against.

 @param [in]	animation	If non-null, the animation.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUITabBar::UpdateFromAnimation(CzAnimInstance *animation)
{
	if (CzUICanvas::UpdateFromAnimation(animation))
		return true;

	int err = IzXomlResource::setProperty(UITabBarClassDef, this, animation);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UITabBarClassDef->findProperty(animation->getTargetPropertyHash());
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "TabBar - UpdateFromAnimation - Error setting the property type, check the properties format - ", cprop->getName().c_str());
	}

	return true;
}

/**
 @fn	bool CzUITabBar::UpdateBinding(unsigned int property_name, CzXomlVariable* var)

 @brief	Updates an actor property from the supplied XOML variable.

 Updates a single property from the value of the bound XOML variable. Note that as CzUITabBar is derived from CzUICanvas, all CzUICanvas properties will also be checked against.

 @param	property_name  	Name of the property.
 @param [in,out]	var	If non-null, the variable.

 @return	true if it succeeds, false if property does not exist.
 */

bool CzUITabBar::UpdateBinding(unsigned int property_name, CzXomlVariable* var)
{
	if (!var->Modified || var->getValue().isEmpty())
		return false;

	if (CzUICanvas::UpdateBinding(property_name, var))
		return true;

	int err = IzXomlResource::setProperty(UITabBarClassDef, this, property_name, var);
	if (err == IzXomlResource::NotFound)
		return false;	// property not found

	if (err < IzXomlResource::NotFound)
	{
		CzXomlClassProperty* cprop = UITabBarClassDef->findProperty(property_name);
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "TabBar - UpdateBindings - Error setting the property type, check the properties format - ", cprop->getName().c_str());
	}

	return true;
}

