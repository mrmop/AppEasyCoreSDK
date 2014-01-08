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

#if !defined(_CZ_XOML_RESOURCE_H_)
#define _CZ_XOML_RESOURCE_H_

#include "CzString.h"
#include "CzUtil.h"
#include "CzXml.h"
#include "CzXomlResourceManager.h"

class CzAnimInstance;
class IzAnimTarget;
class CzXomlClassDef;
struct CzXomlProperty;
struct CzXomlVariable;

/**
 @addtogroup XOML
 @{
 */

/**
 @class	IzXomlResource

 @brief	Base class that is used by all classes that can be instantiated from XOML markup

 All classes that can be created from XOML are derived from IzXomlResource. IzXomlResource provides the base details of the XOML resource including:
 - Name of resource
 - Generic type of class
 - Actual type of class
 - Pure virtual LoadFromXoml method that should be implemented by derived classes. The XOML system will call this method to initialise the class from its XML attriibutes and inner tags

 Each class that is derived from IzXomlResource must have a creator which is a class derived from IzXomlClassCreator which creates an instance of our XOML class. The creator class is 
 a very simple class as is shown by the scene creator class example below:

 @code
class CzSceneCreator : public IzXomlClassCreator
{
public:
	CzSceneCreator()
	{
		setClassName("scene");
	}
	IzXomlResource* CreateInstance(IzXomlResource* parent) { return new CzScene(); }
};
 @endcode

 To create a class that can be instantiated from XOML you should:
 - Derive the class from IzXomlResource
 - Set its XOML base type (game, actor, scene, animation etc..)
 - Implement a class derived from IzXomlClassCreator that implements CreateInstance() which creates an instance of the class
 - Add the class type to the list of class creators in CZ_XOML using addClass()

 */
class IzXomlResource
{
public:

	/**
	 @enum	eSetPropertyError
	
	 @brief	Values that represent a SetProperty error

		NotFound - The requested property was not found
		Success - Property was set successfully
		IncompatibleTypes - The supplied data is not compatible with the target property type
		ReadOnly - The property is read only
		SearchForResource - The property that was supplied is of type string but the target property is a resuorce. If this occurs then usually the caller will search for the resuorce
	 */
	enum eSetPropertyError
	{
		NotFound = 0, 
		Success = 1, 
		SearchForResource = 2, 
		IncompatibleTypes = -1, 
		NoAccess = -2, 
	};

	// Properties
protected:
	CzString				Name;					///< Resource name (unique per resource type)
	CzString				DebugInfo;				///< Filename and line number for debug info
#if defined (_DEBUG)
	CzString				ClassType;				///< Resource class type
	CzString				ActualClassType;		///< Actual class type
	CzString				Tag;					///< Tag name (used to group resource into groups)
#endif	// _DEBUG
	CzXomlResourceManager*	Parent;					///< Parent manager
	bool					Managed;				///< True if managed by the resource system. Managed resources are auto cleaned up by the resource system
	unsigned int			NameHash;				///< Resource name hash
	unsigned int			ClassTypeHash;			///< Base type of XOML class (e.g. Scene, Actor, Animation, Game etc..)
	unsigned int			ActualClassTypeHash;	///< Actual type of XOML class
	unsigned int			TagHash;				///< Tag name as a string hash, used to put resources into loading groups
	IzXomlResource*			Container;				///< The resource that acts as the container for this resource
public:
	void					setParent(CzXomlResourceManager* parent) { Parent = parent; }
	CzXomlResourceManager*	getParent()								{ return Parent; }
	void					setName(const char* name);
	CzString&				getName()								{ return Name; }
	void					setDebugInfo(const char* info)			{ DebugInfo = info; }
	CzString&				getDebugInfo()							{ return DebugInfo; }
	unsigned int			getNameHash()							{ return NameHash; }
	void					setClassType(const char* type_name);
	unsigned int			getClassTypeHash()						{ return ClassTypeHash; }
	void					setActualClassType(const char* type_name);
	unsigned int			getActualClassTypeHash()				{ return ActualClassTypeHash; }
	void					setTag(const char* Tag);
#if defined (_DEBUG)
	CzString&				getTag()								{ return Tag; }
#endif	// _DEBUG
	unsigned int			getTagHash()							{ return TagHash; }
	void					setManaged(bool managed)				{ Managed = managed; }
	bool					isManaged() const						{ return Managed; }
	void					setContainer(IzXomlResource* container)	{ Container = container; }
	IzXomlResource*			getContainer()							{ return Container; }
	// Properties end
protected:
public:
	IzXomlResource() : Managed(false), Parent(NULL) {}
	virtual ~IzXomlResource() {}

	virtual int		LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node) = 0;
	virtual bool	PostLoadFromXoml(IzXomlResource* parent, CzXmlNode* node) { return true; }

	virtual void	Remove();

	static int		setProperty(CzXomlClassDef* class_def, IzXomlResource* target, unsigned int property_name, const CzString& data, bool delta);
	static int		setProperty(CzXomlClassDef* class_def, IzXomlResource* target, unsigned int property_name, const CzXomlProperty& prop, bool delta);
	static int		setProperty(CzXomlClassDef* class_def, IzAnimTarget* target, CzAnimInstance* animation);
	static int		setProperty(CzXomlClassDef* class_def, IzXomlResource* target, unsigned int property_name, CzXomlVariable* variable);
	static int		getProperty(CzXomlClassDef* class_def, IzXomlResource* target, unsigned int property_name, CzXomlProperty& prop);
	// Internal
};


/// @}

#endif	// _CZ_XOML_RESOURCE_H_
