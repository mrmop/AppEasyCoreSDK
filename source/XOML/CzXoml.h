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

#if !defined(_CZ_XOML_H_)
#define _CZ_XOML_H_

#include "CzString.h"
#include "CzUtil.h"
#include "CzXml.h"
#include "CzXomlActions.h"
#include "CzXomlResource.h"

class CzStyle;
class CzXomlClassDef;
class IzXomlResource;
struct CzXomlVariable;

/**
 @addtogroup XOML
 @{
 */


/**
 @enum	eCzXomlPropertyType

 @brief	The different types of XOML properties
 */
enum eCzXomlPropertyType
{
	PT_Invalid, 
	PT_String, 
	PT_Bool, 
	PT_Float, 
	PT_Int, 
	PT_Vec2, 
	PT_Vec3, 
	PT_Vec4, 
	PT_Resource, 
	PT_Var, 
};


/**
 @struct	CzXomlProperty

 @brief	A basic XOML property.

 */

struct CzXomlProperty
{
	eCzXomlPropertyType			Type;		///< Type of property
	bool						OwnData;	/// <If true then the property owns p_data
	union
	{
		void*					p_data;		///< Value of property as string or object
		bool					p_bool;		///< Value of property as a bool
		float					p_float;	///< Value of property as a float
		int						p_int;		///< Value of property as an integer
		float					p_vec[4];	///< Value of property as a vector
	};

	CzXomlProperty() : Type(PT_Invalid), OwnData(false)		{ p_data = NULL; }
	CzXomlProperty(const char* data)		{ OwnData = false; Type = PT_String; p_data = (void*)data; }
	CzXomlProperty(float data)				{ OwnData = false; Type = PT_Float; p_float = data; }
	CzXomlProperty(bool data)				{ OwnData = false; Type = PT_Bool; p_bool = data; }
	CzXomlProperty(int data)				{ OwnData = false; Type = PT_Int; p_int = data; }
	CzXomlProperty(const CzVec2& data)		{ OwnData = false; Type = PT_Vec2; p_vec[0] = data.x; p_vec[1] = data.y; }
	CzXomlProperty(const CzIVec2& data)		{ OwnData = false; Type = PT_Vec2; p_vec[0] = (float)data.x; p_vec[1] = (float)data.y; }
	CzXomlProperty(const CzVec3& data)		{ OwnData = false; Type = PT_Vec3; p_vec[0] = data.x; p_vec[1] = data.y; p_vec[2] = data.z; }
	CzXomlProperty(const CzVec4& data)		{ OwnData = false; Type = PT_Vec4; p_vec[0] = data.x; p_vec[1] = data.y; p_vec[2] = data.z; p_vec[3] = data.w; }
	CzXomlProperty(const CzColour& data)	{ OwnData = false; Type = PT_Vec4; p_vec[0] = (float)data.r; p_vec[1] = (float)data.g; p_vec[2] = (float)data.b; p_vec[3] = (float)data.a; }
	CzXomlProperty(const CzRect& data)		{ OwnData = false; Type = PT_Vec4; p_vec[0] = data.x; p_vec[1] = data.y; p_vec[2] = data.w; p_vec[3] = data.h; }
	CzXomlProperty(const CzIRect& data)		{ OwnData = false; Type = PT_Vec4; p_vec[0] = (float)data.x; p_vec[1] = (float)data.y; p_vec[2] = (float)data.w; p_vec[3] = (float)data.h; }
	CzXomlProperty(IzXomlResource* data)	{ OwnData = false; Type = PT_Resource; p_data = (void*)data; }
	CzXomlProperty(CzXomlVariable* data)	{ OwnData = false; Type = PT_Var; p_data = (void*)data; }
	CzXomlProperty(void* data)				{ OwnData = false; Type = PT_Resource; p_data = data; }
	~CzXomlProperty()
	{
		if (OwnData && Type == PT_String)
			delete [] (char*)p_data;
	}

	bool		setValue(const CzString& data, bool delta);
	bool		setValue(const CzXomlProperty& prop, bool delta);
	void		setToVariable(CzXomlVariable& var);
	void		toString(CzString& string) const;

	static CzXomlProperty* createOfType(eCzXomlPropertyType type);
	static CzXomlProperty* createOfType(const char* type_name);

};

/**
 @struct	CzXomlNamedProperty

 @brief	A basic XOML property that has a name.

 */

struct CzXomlNamedProperty
{
	CzString				Name;			///< Name of property
	CzXomlProperty*			Property;		///< The property

	CzXomlNamedProperty() : Property(NULL)	{  }
	~CzXomlNamedProperty()	{ if (Property != NULL) delete Property; }
};

/**
 @typedef	bool (*CzXomPropertySetter)(IzXomlResource* target, const CzXomlProperty& prop,
    bool add)

 @brief	Defines an alias for a XOML class property setter.
 */

typedef bool (*CzXomPropertySetter)(IzXomlResource* target, const CzXomlProperty& prop, bool add);

/**
 @typedef	CzXomlProperty (*CzXomPropertyGetter)(IzXomlResource* target,
    const CzXomlProperty& prop)

 @brief	Defines an alias for a XOML class property getter.
 */

typedef CzXomlProperty (*CzXomPropertyGetter)(IzXomlResource* target);

/**
 @class	CzXomlClassProperty

 @brief	Used by classes definitions to list available properties

 */
struct CzXomlClassProperty
{
	// Properties
protected:
	eCzXomlPropertyType		Type;				///< Type of property
	CzString				Name;				///< Name of property (only available in debug builds)
	CzXomPropertySetter		Setter;				///< Method that is used to set the property
	CzXomPropertyGetter		Getter;				///< Method that is used to get the property
public:
	eCzXomlPropertyType		getType() const						{ return Type; }
	void					setType(eCzXomlPropertyType type)	{ Type = type; }
	void					setName(const char* name)			{ Name = name; }
	CzString&				getName()							{ return Name; }
	unsigned int			getNameHash() const					{ return Name.getHash(); }
	CzXomPropertySetter		getSetter()							{ return Setter; }
	CzXomPropertyGetter		getGetter()							{ return Getter; }
	
	// Properties end
public:
	CzXomlClassProperty(const char* name, eCzXomlPropertyType type, CzXomPropertySetter setter, CzXomPropertyGetter getter)
	{
		setType(type);
		setName(name);
		Setter = setter;
		Getter = getter;
	}
};

/**
 @class	CzXomlClassDef

 @brief	Used by classes to define accessible properties and methods.

 */
class CzXomlClassDef
{
public:
	// Public access to class creator iteration
	typedef CzList<CzXomlClassProperty*>::iterator _Iterator;
	_Iterator				begin()		{ return Properties.begin(); }
	_Iterator				end()		{ return Properties.end(); }
	// Properties
protected:
	CzList<CzXomlClassProperty*>	Properties;
	// Properties end
public:
	void Release()
	{
		for (_Iterator it = begin(); it != end(); ++it)
			delete *it;
	}

	void					addProperty(CzXomlClassProperty* prop);
	void					removeProperty(CzXomlClassProperty* prop);
	CzXomlClassProperty*	findProperty(unsigned int name_hash);
};

/**
 @class	IzXomlClassCreator

 @brief	Base class that is used by classes that create an instance of a XOML resource type.

 */
class IzXomlClassCreator
{
	// Properties
protected:
#if defined(_DEBUG)
	CzString				ClassName;				///< Name of class
#endif
	unsigned int			ClassNameHash;			///< Name of class as a string hash
public:
	void					setClassName(const char* name)
	{
#if defined(_DEBUG)
		ClassName = name;
#endif
		ClassNameHash = CzString::CalculateHash(name);
	}
	unsigned int			getClassNameHash() const		{ return ClassNameHash; }
#if defined(_DEBUG)
	CzString&				getClassName()					{ return ClassName; }
#endif
	// Properties end
public:
	virtual ~IzXomlClassCreator() {}
	virtual IzXomlResource* CreateInstance(IzXomlResource* parent) = 0;
};

/**
 @def	CZ_XOML

 @brief	A short cut macro that calls the global XOML singleton

 */
#define CZ_XOML	CzXoml::getInstance()

/**
 @class	CzXoml

 @brief	The main controller responsible for instantiating objects from XOML based files.

 CzXoml is the mother of the XOML system and is responsible for the following:
 - Loading and parsing XOML files
 - Instantiating objects from XOML
 - Managing XOML classes
 - Managing XOML actions

 */
class CzXoml
{
public:
	CDEFINE_SINGLETON(CzXoml)

	// Public access to class creator iteration
	typedef CzList<IzXomlClassCreator*>::iterator _Iterator;
	_Iterator				begin() { return ClassCreators.begin(); }
	_Iterator				end() { return ClassCreators.end(); }
	typedef CzList<IzXomlAction*>::iterator _ActionsIterator;
	_ActionsIterator		actions_begin() { return Actions.begin(); }
	_ActionsIterator		actions_end() { return Actions.end(); }

	static const char*		DockingNames[];	// TODO: Find new place for these
	static const char*		AspectLockNames[];
	static const char*		AlphaModeNames[];
	static const char*		AlignHNames[];
	static const char*		AlignVNames[];
	static const char*		SelectTypeNames[];
	static const char*		AxisNames[];
	static const char*		OrientationNames[];
	static const char*		InputTypeHintNames[];
protected:
	// Properties
public:
	void			addClass(IzXomlClassCreator* creator)
	{
#if defined(_DEBUG)
		if (findClass(creator->getClassNameHash()))
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "XOML - Class creator already exists - ", creator->getClassName().c_str());
			delete creator;
			return;
		}
#endif	//(_DEBUG)
		ClassCreators.push_back(creator);
	}
	void			addAction(IzXomlAction* action)
	{
/*#if defined(_DEBUG)
		if (findAction(action->getActionNameHash()))
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "XOML - Actions already exist - ", action->getActionName().c_str());
			delete action;
			return;
		}
#endif	//(_DEBUG)*/
		Actions.push_back(action);
	}
	IzXomlClassCreator*		findClass(unsigned int name_hash);
	IzXomlClassCreator*		findClass(const char* name);
	IzXomlAction*			findAction(unsigned int name_hash);
	IzXomlAction*			findAction(const char* name);
	// Properties end

protected:
	CzList<IzXomlClassCreator*> ClassCreators;
	CzList<IzXomlAction*>		Actions;

public:
	void			Init();
	void			Release();

	bool			Process(IzXomlResource* parent, const char* filename, bool reset_xml_pools = true);
	bool			Process(IzXomlResource* parent, CzXmlNode* node);
	IzXomlResource*	ProcessNode(IzXomlResource* parent, CzXmlNode* node);

	// Utility
	static void		ApplyStyle(CzStyle* style, CzXmlNode* node);
};


/// @}

#endif	// _CZ_XOML_H_
