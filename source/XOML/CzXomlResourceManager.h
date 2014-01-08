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

#if !defined(_CZ_XOML_RESOURCE_MANAGER_H_)
#define _CZ_XOML_RESOURCE_MANAGER_H_

#include "CzString.h"
#include "CzUtil.h"
#include "CzXoml.h"

class CzFont;

/**
 @addtogroup XOML
 @{
 */

/**
 @class	CzXomlResourceManager

 @brief	CzResourceManager manages a collection of resource groups.

 The CzXomlResourceManager manages the lifetime of a collection of IzXomlResource derived resources. When the manager is deleted all resources 
 contained with the resource manager will be destroyed. 

 Resource management is split into 3 types:
 - Global - The global resource manager contains resources that are global to the entire app and will persist until the resources are manually 
 removed or until the app closes down.
 - Scene local - Scenes have their own local resource manager which manage resources that are local to the scene. When the scene is destroyed, 
 all resources managed by the scene will also be destroyed. This enables you to free up memory by simply managing scenes.
 - Actor local - Some types of resource are local to an actor and will be destroyed when the actor is destroyed. These types include actions, 
 timelines and modifiers.

 For optimal searching generally the following types of resources will have their own managers and will not be managed by the resource system:
 - Actions (managed by action managers)
 - Timelines (managed by timeline managers)
 - Actors (managed by scenes)
 - Scenes (managed by the main CzApp singleton)
 - XOML Variables (managed by variable managers)
 - XOML Programs (managed by program managers)

 To access a scnes resource manager call CzScene::getResourceManager()
 To access the global resource managers resources use the CZ_GLOBAL_RESOURCE_MANAGER macro or call CZ_GLOBAL_RESOURCES->getResourceManager()

 For more information on the global resource manager see CzGlobalResources.

 */
class CzXomlResourceManager
{
public:
	// Public access to class creator iteration
	typedef CzList<IzXomlResource*>::iterator _Iterator;
	_Iterator				begin()		{ return Resources.begin(); }
	_Iterator				end()		{ return Resources.end(); }

protected:
	// Properties
	IzXomlResource*			Parent;				///< Parent container scene
	CzList<IzXomlResource*>	Resources;			///< A collection of resources
public:
	void					setParent(IzXomlResource* scene)	{ Parent = scene; }
	IzXomlResource*			getParent()							{ return Parent; }
	// Properties end

public:
	CzXomlResourceManager() : Parent(NULL) {}
	virtual ~CzXomlResourceManager() { clearResources(); }

	bool					addResource(IzXomlResource* resource);
	void					removeResource(IzXomlResource* resource);
	void					removeResource(unsigned int name_hash, unsigned int type_hash);
	void					removeTaggedResources(const char* tag);
	void					removeTaggedResources(unsigned int tag_hash);
#if defined (_DEBUG)
	IzXomlResource*			findResourceDebug(unsigned int name_hash, unsigned int type_hash, bool global_search = true);
#endif
	int						findResourcesOfType(unsigned int type_hash, bool global_search, CzVector<IzXomlResource*>& resources);
	IzXomlResource*			findResource(unsigned int name_hash, unsigned int type_hash, bool global_search = true);
	IzXomlResource*			findResource(const char* name, unsigned int type_hash, bool global_search = true);
	IzXomlResource*			findResource(const char* name, const char* type, bool global_search = true);
	CzFont*					findFontWithFilename(const char* filename, bool global_search = true);

	static int				FindResourcesOfType(unsigned int type_hash, IzXomlResource* container, CzVector<IzXomlResource*>& resources);
	static IzXomlResource*	FindResource(unsigned int name_hash, unsigned int type_hash, IzXomlResource* container = NULL);
	static IzXomlResource*	FindResource(const char* name, unsigned int type_hash, IzXomlResource* container = NULL);
	void					clearResources();
};

/// @}

#endif	// _CZ_XOML_RESOURCE_MANAGER_H_
