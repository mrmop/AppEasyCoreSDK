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

#if !defined(_CZ_ACTIONS_RESOURCE_H_)
#define _CZ_ACTIONS_RESOURCE_H_

#include "CzActions.h"
#include "CzXoml.h"

/**
 @addtogroup Actions
 @{
 */
 
/**
 @class	CzXomlActions_Resource

 @brief	Implements actions that relate to resources

 The CzXomlActions_Resource class holds Actions (IzXomlAction) that deal with XOML resources such as RemoveResource
 
 The following actions are supported:
 - RemoveResource
 - RemoveResources

 The above actions have the following XOML prototypes:
 - RemoveResource(resource-name)
   - resource-name - Name of the resource to remove and destroy
 - RemoveResources(tag-name)
   - tag-name - Resource tag name to remove and destory (all resources matching the tag will be removd)

 For more information on resources see IzXomlResource and CzXomlResourceManager
 
 */
 
class CzXomlActions_Resource : public IzXomlAction
{
public:
	enum eActionType
	{
		Action_RemoveResource	= 0, 
		Action_RemoveResources	= 1, 
		Action_Max				= 2, 
	};
protected:
	eActionType	Type;
	CzXomlActions_Resource() {}
public:
	CzXomlActions_Resource(eActionType type)
	{
		Type = type;
		switch (Type)
		{
		case Action_RemoveResource:
			setActionName("removeresource");
			break;
		case Action_RemoveResources:
			setActionName("removeresources");
			break;
		}
	}
	void Execute(IzXomlResource* source, CzAction* action);
};

/// @}

#endif // _CZ_ACTIONS_RESOURCE_H_
