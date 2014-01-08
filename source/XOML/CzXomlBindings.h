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

#if !defined(_CZ_XOML_BINDINGS_H_)
#define _CZ_XOML_BINDINGS_H_

#include "CzXoml.h"
#include "CzXomlVariables.h"

// 
// 
//
//
// CzXomlBinding - Represents a bindable property and variable
//
//
//
//
struct CzXomlBinding
{
	unsigned int			Property;			///< Target property name as a string hash that is updatd with the variables value
#if defined(_DEBUG)
	CzString				_Property;			///< Target property name that is updated with the variables value (only available in debug builds)
#endif	// _DEBUG
	CzXomlVariable*			Variable;			///< Variable that contains the value that is bound to the target property
	CzXomlVariableInt*		IndexVar;			///< Cached index variable
	CzString				VIndex;				///< Index variable (if not empty then this will be taken as a variable that holds the index)
	int						Index;				///< Index into array of binding variables

	CzXomlBinding() : Index(-1), IndexVar(NULL) {}
};

// 
// 
//
//
// CzXomlBindings - Manages a collection of XOML bindings. If an object contains a bindings manager then it should traverse the bindings list 
// and update the specified properties within the object with the vaukes located in the paired variables
//
//
//
//
class CzXomlBindings : public IzXomlResource
{
public:
	// Public access to iteration
	typedef CzList<CzXomlBinding*>::iterator _Iterator;
	_Iterator				begin()		{ return Bindings.begin(); }
	_Iterator				end()		{ return Bindings.end(); }

protected:
	// Properties
	CzList<CzXomlBinding*>	Bindings;			// A collection of bindings
public:
	void					addBinding(CzXomlBinding* binding);
	void					addBinding(const CzString& propetty_name, CzXomlVariable* variable);
	void					clearBindings();
	// Properties end

public:
	CzXomlBindings() : IzXomlResource()	{ setClassType("bindings"); }
	virtual ~CzXomlBindings()			{ clearBindings(); }

	// Implementation of IzXomlClass interface
	int						LoadFromXoml(IzXomlResource* parebt, bool load_children, CzXmlNode* node);
};

//
//  CzXomlBindingsCreator - Creates an instance of a bingings manager object
//
class CzXomlBindingsCreator : public IzXomlClassCreator
{
public:
	CzXomlBindingsCreator()
	{
		setClassName("bindings");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzXomlBindings(); }
};





#endif	// _CZ_XOML_BINDINGS_H_
