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

#if !defined(_CZ_XOML_ACTIONS_H_)
#define _CZ_XOML_ACTIONS_H_

#include "CzString.h"
#include "CzUtil.h"

struct CzAction;
class IzXomlResource;
class CzApp;
class CzActor;
class CzScene;

/**
 @addtogroup XOML
 @{
 */


/**
 @class	IzXomlAction

 @brief	Base class that is used by classes that create actions that can be executed in XOML.

 All XOML actions that can be called from XOML are derived from this class. See CzAction for more information.

 */
class IzXomlAction
{
	// Properties
protected:
#if defined(_DEBUG)
	CzString				ActionName;				///< Name of action
#endif
	unsigned int			ActionNameHash;			///< Name of action as a string hash
public:
	void					setActionName(const char* name)
	{
#if defined(_DEBUG)
		ActionName = name;
#endif
		ActionNameHash = CzString::CalculateHash(name);
	}
	unsigned int			getActionNameHash() const		{ return ActionNameHash; }
#if defined(_DEBUG)
	CzString&				getActionName()					{ return ActionName; }
#endif
	// Properties end
public:
	virtual void Execute(IzXomlResource* source, CzAction* action) = 0;

	static IzXomlResource* getContainer(IzXomlResource* source, CzApp*& app, CzScene*& scene, CzActor*& actor);
};



/// @}

#endif	// _CZ_XOML_ACTIONS_H_
