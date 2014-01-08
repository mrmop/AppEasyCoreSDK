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

#if !defined(_CZ_MODIFIER_H_)
#define _CZ_MODIFIER_H_

#include "CzUtil.h"
#include "CzString.h"
#include "CzXoml.h"

class CzModifierManager;

//
//
//
//
// IzModifier - A class Modifier is attached to a class to modify its behvaiour
//
//
//
//
class IzModifier
{
public:
	// Properties
protected:
	CzModifierManager*	Parent;						// Parent manager
	CzString			Params[4];					// Modifier parameters
#if defined (_DEBUG)
	CzString			Name;						// Modifier name
#endif	// _DEBUG
	unsigned int		NameHash;					// Modifier name hash
	bool				Active;						// Modifiers active state
	bool				Initialised;				// Initialised state
public:
	CzModifierManager* getParent()									{ return Parent; }
	void				setParent(CzModifierManager* parent)	{ Parent = parent; }
	void				setName(const char* name)
	{
#if defined (_DEBUG)
		Name = name;
#endif	// _DEBUG
		NameHash = CzString::CalculateHash(name);
	}
#if defined (_DEBUG)
	CzString&			getName()						{ return Name; }
#endif	// _DEBUG
	unsigned int		getNameHash()					{ return NameHash; }
	void				setActive(bool active)			{ Active = active; }
	bool				isActive() const				{ return Active; }
	void				setInitialised(bool init)		{ Initialised = init; }
	bool				isInitialised() const			{ return Initialised; }
	void				setParameter(int index, const CzString& data)			{ Params[index] = data; }
	void				setParameter(int index, const char* data)				{ Params[index] = data; }
	// Properties End
public:
	IzModifier() : Active(true), Initialised(false), Parent(NULL) {}
	virtual void		InitModifier(IzXomlResource* target) = 0;				// Initialise the Modifier
	virtual void		ReleaseModifier(IzXomlResource* target) = 0;			// Clean-up the Modifier
	virtual bool		UpdateModifier(IzXomlResource* target, float dt) = 0;	// Update the Modifier
};

// 
// 
//
//
// CzModifierManager - Manages a collection of class modifiers
//
//
//
//
class CzModifierManager : public IzXomlResource
{
public:
	// Public access to iteration
	typedef CzList<IzModifier*>::iterator _Iterator;
	_Iterator				begin()		{ return Modifiers.begin(); }
	_Iterator				end()		{ return Modifiers.end(); }

protected:
	// Properties
	CzList<IzModifier*>	Modifiers;			// A collection of class modifiers
public:
	bool					addModifier(IzModifier* modifier);
	bool					removeModifier(IzModifier* modifier);
	IzModifier*				findModifier(unsigned int name_hash);
	IzModifier*				findModifier(const char* name);
	void					clearModifiers();
	// Properties end

public:
	CzModifierManager() : IzXomlResource() { setClassType("modifiers"); }
	virtual ~CzModifierManager() { clearModifiers(); }

	void					Execute(IzXomlResource* target, float dt);

	// Implementation of IzXomlResource interface
	int						LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);
};

//
//  CzModifierManagerCreator - Creates an instance of a modifier manager
//
class CzModifierManagerCreator : public IzXomlClassCreator
{
public:
	CzModifierManagerCreator()
	{
		setClassName("modifiers");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzModifierManager(); }
};

// 
// 
//
//
// IzModifierCreator - Base class that is used by classes that create an instance of a class mdofier
//
//
//
//
class IzModifierCreator
{
#if defined(_DEBUG)
	CzString				ClassName;				// Name of class
#endif
	unsigned int			ClassNameHash;			// Hashed name of class
public:
	void					setClassName(const char* name)
	{
#if defined(_DEBUG)
		ClassName = name;
#endif
		ClassNameHash = CzString::CalculateHash(name);
	}
	unsigned int			getClassNameHash() const			{ return ClassNameHash; }
#if defined(_DEBUG)
	CzString&				getClasstName()						{ return ClassName; }
#endif
public:
	virtual IzModifier* CreateInstance() = 0;
};

// 
// 
//
//
// CzMods - CzMods is the main controller responsible for instantiating class modifiers
// 
// 
// 
//
#define CZ_MODS	CzMods::getInstance()
class CzMods
{
public:
	CDEFINE_SINGLETON(CzMods)

	// Public access to class creator iteration
	typedef CzList<IzModifierCreator*>::iterator _Iterator;
	_Iterator				begin() { return ModifierCreators.begin(); }
	_Iterator				end() { return ModifierCreators.end(); }

protected:
	// Properties
public:
	void					addModifier(IzModifierCreator* creator)
	{
#if defined(_DEBUG)
		if (findCreator(creator->getClassNameHash()))
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Mods - Modifier creator already exists in modifier creator list - ", creator->getClasstName().c_str());
			delete creator;
			return;
		}
#endif	//(_DEBUG)
		ModifierCreators.push_back(creator);
	}
	IzModifierCreator*		findCreator(unsigned int name_hash);
	IzModifierCreator*		findCreator(const char* name);
	// Properties end

protected:
	CzList<IzModifierCreator*> ModifierCreators;

public:
	void			Init();
	void			Release();
};

#endif	// _CZ_MODIFIER_H_
