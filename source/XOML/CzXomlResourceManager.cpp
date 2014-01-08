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

#include "CzXoml.h"
#include "CzXomlResourceManager.h"
#include "CzHashes.h"
#include "CzUtil.h"
#include "CzString.h"
#include "CzResources.h"
#include "CzFont.h"
#include "CzActor.h"

//
//
//
//
// CzXomlResourceManager implementation
//
//
//
//
/*void CzXomlResourceManager::addResource(const char* resource_name, const char* resource_filename, bool preload)
{
	IzXomlResource* resource = new CzResourceResource();
	resource->setResourceName(resource_name);
	resource->setResourceFilename(resource_filename);
	Resources.push_back(resource);

	if (preload)
	{
		resource->Load();
	}
}*/

/**
 @fn	bool CzXomlResourceManager::addResource(IzXomlResource *resource)

 @brief	Adds a resource to the resource manager.

 Adds the specified resource to this resource manager and manages it. If this resource manager is later deleted then so will any of its managed resources. Note that you should not add the 
 same resource to multiple resource managers as the system will attempt to clean them up multiple times. 

 @param [in]	resource	If non-null, the resource.

 @return	true if it succeeds, false if it fails.
 */

bool CzXomlResourceManager::addResource(IzXomlResource *resource)
{
#if defined(CZ_REMOVE_DUPLICATES)
	IzXomlResource* res = findResource(resource->getNameHash(), resource->getClassTypeHash(), false);
	if (res != NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "ResourceManager - Resource already exists, resource was replaced - ", resource->getName().c_str());
		removeResource(res);
	}
#endif
	Resources.push_back(resource);
	resource->setParent(this);
	resource->setManaged(true);

	return true;
}

/**
 @fn	void CzXomlResourceManager::removeResource(IzXomlResource* resource)

 @brief	Removes the resource described by resource.

 Removes the specified resource from this resource manager and deletes it from memory.

 @param [in]	resource	If non-null, the resource.
 */

void CzXomlResourceManager::removeResource(IzXomlResource* resource)
{
	removeResource(resource->getNameHash(), resource->getClassTypeHash());
}

/**
 @fn	void CzXomlResourceManager::removeResource(unsigned int name_hash, unsigned int type_hash)

 @brief	Removes the resource described by resource.

 Removes the named resource of the specified type from this resource manager and deletes it from memory.

 @param	name_hash	The name of the resource as a string hash.
 @param	type_hash	The type of the resource as a string hash.
 */

void CzXomlResourceManager::removeResource(unsigned int name_hash, unsigned int type_hash)
{
	// Renove named resource from the manager
	for (_Iterator it = Resources.begin(); it != Resources.end(); ++it)
	{
		if (name_hash == (*it)->getNameHash() && type_hash == (*it)->getClassTypeHash())
		{
			delete *it;
			Resources.erase(it);
			break;
		}
	}
}

/**
 @fn	void CzXomlResourceManager::removeTaggedResources(unsigned int tag_hash)

 @brief	Removes the tagged resources.

 Removes all resources that belong to the specified tag group and deletes them.

 @param	tag_hash	The group tag name as a string hash.
 */

void CzXomlResourceManager::removeTaggedResources(unsigned int tag_hash)
{
	CzList<IzXomlResource*>		Removals;

	// Buildl ist of resources that are to be removed for tag
	for (_Iterator it = Resources.begin(); it != Resources.end(); ++it)
	{
		if (tag_hash == (*it)->getTagHash())
		{
			Removals.push_back(*it);
		}
	}

	// Remove the resources
	for (_Iterator it = Removals.begin(); it != Removals.end(); ++it)
	{
		for (_Iterator it2 = Resources.begin(); it2 != Resources.end(); ++it2)
		{
			if (*it2 == *it)
			{
				delete *it2;
				Resources.erase(it2);
				break;
			}
		}
	}
	Removals.clear();
}

/**
 @fn	void CzXomlResourceManager::removeTaggedResources(const char* tag)

 @brief	Removes the tagged resources.

 Removes all resources that belong to the specified tag group and deletes them.

 @param	tag	The tag name.
 */

void CzXomlResourceManager::removeTaggedResources(const char* tag)
{
	removeTaggedResources(CzString::CalculateHash(tag));
}

/**
 @fn	IzXomlResource* CzXomlResourceManager::findResource(unsigned int name_hash,
    unsigned int type_hash, bool global_search)

 @brief	Searches for the named resource.

 Searches the resource manager for the named resource of the specified type. If global_search is true and the resource was not found in this resource manager then the global resource 
 manager will also be searched.

 @param	name_hash	 	The resource name as a string hash.
 @param	type_hash	 	The resource type as a string hash.
 @param	global_search	true to do a global search.

 @return	null if it fails, else the found resource.
 */

IzXomlResource* CzXomlResourceManager::findResource(unsigned int name_hash, unsigned int type_hash, bool global_search)
{
	// Find and return named resource of specific type
	for (_Iterator it = Resources.begin(); it != Resources.end(); ++it)
	{
		if (name_hash == (*it)->getNameHash() && type_hash == (*it)->getClassTypeHash())
			return *it;
	}

	if (global_search)
	{
		// Not found so check global resources
		CzXomlResourceManager* m = CZ_GLOBAL_RESOURCES->getResourceManager();
		if (m != this)	// Prevent global resource manager from calling itself
			return m->findResource(name_hash, type_hash, false);
	}

	return NULL;
}

/**
 @fn	int CzXomlResourceManager::findResourcesOfType(unsigned int type_hash, bool global_search,
    CzVector<IzXomlResource*>& resources)

 @brief	Buiilds a list of resources of the specified type.
    
 Searches the resource manager for resource of the specified type adding them to the supplied list. If global_search is true and the resource was not found in this resource manager 
 then the global resource manager will also be searched.

 @param	type_hash			 	The resource type as a string hash.
 @param	global_search		 	true to do a global search.
 @param [out]	resources	[out] If non-null, the resources found.

 @return	Number of found resources.

 */

int CzXomlResourceManager::findResourcesOfType(unsigned int type_hash, bool global_search, CzVector<IzXomlResource*>& resources)
{
	// Find and return named resource of specific type
	for (_Iterator it = Resources.begin(); it != Resources.end(); ++it)
	{
		if (type_hash == (*it)->getClassTypeHash())
			resources.push_back(*it);
		return resources.size();
	}

	if (global_search)
	{
		// Not found so check global resources
		CzXomlResourceManager* m = CZ_GLOBAL_RESOURCES->getResourceManager();
		if (m != this)	// Prevent global resource manager from calling itself
			return m->findResourcesOfType(type_hash, false, resources);
	}

	return 0;
}


#if defined (_DEBUG)

/**
 @fn	IzXomlResource* CzXomlResourceManager::findResourceDebug(unsigned int name_hash,
    unsigned int type_hash, bool global_search)

 @brief	Searches for the named resource, displaying debug information.

 Searches the resource manager for the named resource of the specified type. If global_search is true and the resource was not found in this resource manager then the global resource 
 manager will also be searched.

 This method will display debug information as it searches.

 @param	name_hash	 	The resource name as a string hash.
 @param	type_hash	 	The resource type as a string hash.
 @param	global_search	true to do a global search.

 @return	null if it fails, else the found resource debug.
 */

IzXomlResource* CzXomlResourceManager::findResourceDebug(unsigned int name_hash, unsigned int type_hash, bool global_search)
{
	if (CZ_GLOBAL_RESOURCES->getResourceManager() != this)
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "**** Searching local resources ****");
	else
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "**** Searching global resources ****");
	// Find and return named image of specific type
	for (_Iterator it = Resources.begin(); it != Resources.end(); ++it)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "- ", (*it)->getName().c_str());
		if (name_hash == (*it)->getNameHash() && type_hash == (*it)->getClassTypeHash())
			return *it;
	}

	if (global_search)
	{
		// Not found so check global resources
		CzXomlResourceManager* m = CZ_GLOBAL_RESOURCES->getResourceManager();
		if (m != this)	// Prevent global resource manager from calling itself
			return m->findResourceDebug(name_hash, type_hash, false);
	}

	return NULL;
}
#endif // _DEBUG

/**
 @fn	IzXomlResource* CzXomlResourceManager::findResource(const char* name,
    unsigned int type_hash, bool global_search)

 @brief	Searches for the named resource.

 Searches the resource manager for the named resource of the specified type. If global_search is true and the resource was not found in this resource manager then the global resource 
 manager will also be searched.

 @param	name		 	The resource name.
 @param	type_hash	 	The resource type as a string hash.
 @param	global_search	true to do a global search.

 @return	null if it fails, else the found resource.
 */

IzXomlResource* CzXomlResourceManager::findResource(const char* name, unsigned int type_hash, bool global_search)
{
	return findResource(CzString::CalculateHash(name), type_hash, global_search);
}

/**
 @fn	IzXomlResource* CzXomlResourceManager::findResource(const char* name, const char* type,
    bool global_search)

 @brief	Searches for the named resource.

 Searches the resource manager for the named resource of the specified type. If global_search is true and the resource was not found in this resource manager then the global resource 
 manager will also be searched.

 @param	name	 		The resource name.
 @param	type	 		The resource type.
 @param	global_search	true to do a global search.

 @return	null if it fails, else the found resource.
 */

IzXomlResource* CzXomlResourceManager::findResource(const char* name, const char* type, bool global_search)
{
	unsigned int name_hash = CzString::CalculateHash(name);
	unsigned int type_hash = 0;

	if (type != NULL)
		type_hash = CzString::CalculateHash(type);

	// Find and return named resource of specific type
	for (_Iterator it = Resources.begin(); it != Resources.end(); ++it)
	{
		if (type == NULL)
		{
			if (name_hash == (*it)->getNameHash())
				return *it;
		}
		else
		{
			if (name_hash == (*it)->getNameHash() && type_hash == (*it)->getClassTypeHash())
				return *it;
		}
	}

	if (global_search)
	{
		// Not found so check global resources
		CzXomlResourceManager* m = CZ_GLOBAL_RESOURCES->getResourceManager();
		if (m != this)	// Prevent global resource manager from calling itself
		{
			if (type != NULL)
				return m->findResource(name_hash, type_hash, false);
			else
				return m->findResource(name, type, false);
		}
	}

	return NULL;
}

/**
 @fn	CzFont* CzXomlResourceManager::findFontWithFilename(const char* filename,
    bool global_search)

 @brief	Searches for the first font with specified filename.

 @param	filename	 	Filename of the file.
 @param	global_search	true to do a global search.

 @return	null if it fails, else the found font with filename.
 */

CzFont* CzXomlResourceManager::findFontWithFilename(const char* filename, bool global_search)
{
	unsigned int name_hash = CzString::CalculateHash(filename);
	unsigned int type_hash = CzHashes::Font_Hash;

	// Find and return named resource r specific type
	for (_Iterator it = Resources.begin(); it != Resources.end(); ++it)
	{
		if (type_hash == (*it)->getClassTypeHash())
		{
			CzFont* font =	(CzFont*)*it;
			if (font->getFilename().getHash() == name_hash && font->getUsingFont() == NULL)
				return (CzFont*)*it;
		}
	}

	if (global_search)
	{
		// Not found so check global resources
		CzXomlResourceManager* m = CZ_GLOBAL_RESOURCES->getResourceManager();
		if (m != this)	// Prevent global resource manager from calling itself
			return m->findFontWithFilename(filename, false);
	}

	return NULL;
}

/**
 @fn	IzXomlResource* CzXomlResourceManager::FindResourcesOfType(unsigned int type_hash, IzXomlResource* container, CzVector<IzXomlResource*>& resources)

 @brief	Builda a list of resources of the specified type

 Searches the resource manager for resource of the specified type adding them to the supplied list. If global_search is true and the resource was not found in this resource manager 
 then the global resource manager will also be searched.

 @param	name_hash	 	The resource name as a string hash.
 @param	type_hash	 	The resource type as a string hash.
 @param [out]	resources	[out] If non-null, the resources found.

 @return	Number of found resources.
 */

int	CzXomlResourceManager::FindResourcesOfType(unsigned int type_hash, IzXomlResource* container, CzVector<IzXomlResource*>& resources)
{
	// If container class is an actor then check the actors scenes resource manager
	if (container != NULL && container->getClassTypeHash() == CzHashes::Actor_Hash)
		container = ((CzActor*)container)->getScene();

	// If container class is a scene then check the scenes resource manager
	if (container != NULL && container->getClassTypeHash() == CzHashes::Scene_Hash)
		return ((CzScene*)container)->getResourceManager()->findResourcesOfType(type_hash, true, resources);

	return CZ_GLOBAL_RESOURCES->getResourceManager()->findResourcesOfType(type_hash, false, resources);
}


/**
 @fn	IzXomlResource* CzXomlResourceManager::FindResource(unsigned int name_hash,
    unsigned int type_hash, IzXomlResource* container)

 @brief	Searches for the named resource.

 Static method that searches the supplied containers resource manager for the named resource of the specified type. If the resource was not found in this resource manager then the global resource 
 manager will also be searched.

 @param	name_hash	 	The resource name as a string hash.
 @param	type_hash	 	The resource type as a string hash.
 @param [in]	container	If non-null, the container.

 @return	null if it fails, else the found resource.
 */

IzXomlResource* CzXomlResourceManager::FindResource(unsigned int name_hash, unsigned int type_hash, IzXomlResource* container)
{
	IzXomlResource* res = NULL;

	// If container class is an actor then check the actors scenes resource manager
	if (container != NULL && container->getClassTypeHash() == CzHashes::Actor_Hash)
		container = ((CzActor*)container)->getScene();

	// If container class is a scene then check the scenes resource manager
	if (container != NULL && container->getClassTypeHash() == CzHashes::Scene_Hash)
		res = ((CzScene*)container)->getResourceManager()->findResource(name_hash, type_hash, true);
	else
		res = CZ_GLOBAL_RESOURCES->getResourceManager()->findResource(name_hash, type_hash, false);

	return res;
}

/**
 @fn	IzXomlResource* CzXomlResourceManager::FindResource(const char* name,
    unsigned int type_hash, IzXomlResource* container)

 @brief	Searches for the named resource.

 Static method that searches the supplied containers resource manager for the named resource of the specified type. If the resource was not found in this resource manager then the global resource 
 manager will also be searched.

 @param	name		 	The resource name.
 @param	type_hash	 	The resource type as a string hash.
 @param [in]		container	If non-null, the container.

 @return	null if it fails, else the found resource.
 */

IzXomlResource* CzXomlResourceManager::FindResource(const char* name, unsigned int type_hash, IzXomlResource* container)
{
	return FindResource(CZ_HASH(name), type_hash, container);
}

/**
 @fn	void CzXomlResourceManager::clearResources()

 @brief	Clears and destroys all resources.

 */

void CzXomlResourceManager::clearResources()
{
	// Remove and delete all images from the manager
	for (_Iterator it = Resources.begin(); it != Resources.end(); ++it)
	{
		IzXomlResource* res = *it;
		delete res;
	}
	Resources.clear();
}

