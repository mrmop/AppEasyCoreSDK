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

#if !defined(_CZ_RESOURCES_H_)
#define _CZ_RESOURCES_H_


#include "CzString.h"
#include "CzUtil.h"
#include "CzXoml.h"
#include "CzScript.h"
#include "CzTimer.h"

class CzActionsManager;
class CzAnimTimelinesManager;
class CzProgramManager;
class IzScriptEngine;

/**
 @addtogroup Core
 @{
 */
 
/*
// TODO: Move this somewhere else
// 
// 
//
//
// CzResourceGroup - Wrapper class to allow Marmalade resource groups to be used as game resources
// 
// 
// 
//
class CzResourceGroup : public IzXomlResource
{
	// Properties
protected:
	CzString					GroupFilename;			// Resource group file name
	void*						ResourceGroup;			// Marmalade resource group
	bool						Loaded;					// Loaded state of resource group
public:
	void						setGroupFilename(const char* group_file_name)	{ GroupFilename = group_file_name; }
	CzString&					getGroupFilename()								{ return GroupFilename; }
	void*						getResourceGroup();
	// Properties End

public:
	CzResourceGroup() : IzXomlResource(), ResourceGroup(NULL), Loaded(false) { setClassType("resourcegroup"); }
	virtual ~CzResourceGroup() { Destroy(); }

	bool						Load();
	void						Destroy();

	// Implementation of IzXomlClass interface
	int							LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);
};

//
//
//
//
//  CzResourceGroupCreator - Creates an instance of a resource group ovject
//
//
//
//
class CzResourceGroupCreator : public IzXomlClassCreator
{
public:
	CzResourceGroupCreator()
	{
		setClassName("resourcegroup");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent);
};*/

/**
 @class	CzGlobalResources

 @brief	Manages application global resources.

 Represents a collection of resources that are meant to be accessible across the entire game / app and for the life time of the game / app. A CzGlobalResources contains a number of manager sthat 
 enable it to manage a number of different types of resources include:
 - CzXomlVariableManage - Manages XOML variables
 - CzActionsManager - Manages actions
 - CzAnimTimelinesManager - Manages timelines
 - CzTimersManager - Manages timers
 - CzProgramManager - Manages a collection of XOML programs
 - CzXomlResourceManager - Manages all other types of resources

 The CzGlobalResources class is a singleton that is available via the CZ_GLOBAL_RESOURCES macros.

 */

class CzGlobalResources
{
	CDEFINE_SINGLETON(CzGlobalResources)

protected:
	// Properties
	CzXomlResourceManager*		ResourceManager;		///< Manages all types of resources
	CzXomlVariableManager*		VariableManager;		///< Manages XOML variables
	CzActionsManager*			ActionsManager;			///< Manages actions
	CzAnimTimelinesManager*		TimelinesManager;		///< Manages timelines
	CzTimersManager*			TimersManager;			///< Manages timers
	CzProgramManager*			ProgramManager;			///< Manages a collection of XOML programs
	IzScriptEngine*				ScriptEngine;			///< manages a script
public:
	CzXomlResourceManager*		getResourceManager()				{ return ResourceManager; }
	CzXomlVariableManager*		getVariableManager()				{ return VariableManager; }
	CzActionsManager*			getActionsManager()					{ return ActionsManager; }
	CzAnimTimelinesManager*		getTimelinesManager()				{ return TimelinesManager; }
	CzTimersManager*			getTimersManager()					{ return TimersManager; }
	CzProgramManager*			getProgramManager()					{ return ProgramManager; }
	IzScriptEngine*				getScriptEngine(CzScript::eScriptType type = CzScript::ST_None);		///< Returns the script engine that is being used by the global resources manager
	// Properties end

public:
	void						Init(IzXomlResource* parent);		///< Initialises the global resources system
	void						Release();							///< Releases the global resources system, releasing all global resources
	void						Update(float dt);					///< Update the resource manager
};

/**
 @def	CZ_GLOBAL_RESOURCES

 @brief	A macro that returns the global resources singleton.
 */
#define CZ_GLOBAL_RESOURCES				CzGlobalResources::getInstance()

/**
 @def	CZ_GLOBAL_RESOURCE_MANAGER

 @brief	A macro that returns the global resource manager.
 */
#define CZ_GLOBAL_RESOURCE_MANAGER		CZ_GLOBAL_RESOURCES->getResourceManager()

/**
 @def	CZ_GLOBAL_VARIABLE_MANAGER

 @brief	A macro that returns the global XOML variables manager.
 */
#define CZ_GLOBAL_VARIABLE_MANAGER		CZ_GLOBAL_RESOURCES->getVariableManager()

/**
 @def	CZ_GLOBAL_ACTIONS_MANAGER

 @brief	A macro that returns the global actions manager.
 */
#define CZ_GLOBAL_ACTIONS_MANAGER		CZ_GLOBAL_RESOURCES->getActionsManager()

/**
 @def	CZ_GLOBAL_TIMELINE_MANAGER

 @brief	A macro that returns the global timelines manager.
 */
#define CZ_GLOBAL_TIMELINE_MANAGER		CZ_GLOBAL_RESOURCES->getTimelinesManager()

/**
 @def	CZ_GLOBAL_TIMERS_MANAGER

 @brief	A macro that returns the global timers manager.
 */
#define CZ_GLOBAL_TIMERS_MANAGER		CZ_GLOBAL_RESOURCES->getTimersManager()

/**
 @def	CZ_GLOBAL_PROGRAM_MANAGER

 @brief	A macro that returns the global XOML programs manager.
 */
#define CZ_GLOBAL_PROGRAM_MANAGER		CZ_GLOBAL_RESOURCES->getProgramManager()

/// @}

#endif	// _CZ_RESOURCES_H_
