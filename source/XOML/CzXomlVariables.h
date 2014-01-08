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

#if !defined(_CZ_XOML_VARIABLES_H_)
#define _CZ_XOML_VARIABLES_H_

#include "CzString.h"
#include "CzXoml.h"
#include "CzSlotArray.h"
#include "CzVec2.h"
#include "CzIVec2.h"
#include "CzVec3.h"
#include "CzVec4.h"
#include "CzRect.h"

struct CzXomlVariable;
class CzXomlVariableManager;


//
//
//
//
//  CzXomlCondition - A condition represents the true or false state of a specific set of variables, operators and values
//
//
//
//
enum eCzXomlConditionOperator
{
	CO_None, 
	CO_Equal, 
	CO_NotEqual, 
	CO_Greater, 
	CO_GreaterEqual, 
	CO_Less, 
	CO_LessEqual, 
	CO_And, 
};
enum eCzXomlConditionJoinOp
{
	CJO_None, 
	CJO_And, 
	CJO_Or, 
};
struct CzXomlCondition
{
	CzXomlVariable*				Variable;			// The variable to check
	CzXomlVariable*				Variable2;			// The variable to check against
	eCzXomlConditionOperator	Operator;			// Determines how the variable and its value should be checked
	eCzXomlConditionJoinOp		Join;				// Join specifies the operator that should be applied to the next condition in a list of conditions
	CzString					Value;				// Value to compare variable against

	CzXomlCondition() : Operator(CO_None), Join(CJO_None), Variable(NULL), Variable2(NULL)	{}
};

typedef CzList<CzXomlCondition*> CzXomlConditions;


// 
// 
//
//
// CzXomlVariable - A basic XOML variable
//
//
//
//
enum eCzXomlVariableType
{
	VT_String, 
	VT_Bool, 
	VT_Float, 
	VT_Int, 
	VT_Vec2, 
	VT_Vec3, 
	VT_Vec4, 
	VT_ArrayString, 
	VT_ArrayBool, 
	VT_ArrayFloat, 
	VT_ArrayInt, 
	VT_Condition, 
	VT_XML, 
};

struct CzXomlVariable
{
	// Properties
protected:
	bool						Persist;			///< if true then this variable will persist when the app is closed down and restarted
	bool						InstantSave;		///< if true then this variable will saved as soon as it is changed otherwise it will be saved when it is destroyed
	CzString					Value;				///< Value of variable
	bool						Changed;			///< Internal changed status
public:
	void						setPersistent(bool persist)		{ Persist = persist; }
	bool						isPersistent() const			{ return Persist; }
	void						setInstantSave(bool instant)	{ InstantSave = instant; }
	bool						isInstantSave() const			{ return InstantSave; }
	void						setValueText(const char* value)	{ Value = value; Changed = true; }
	virtual void				setValue(const char* value, int count = -1)
	{
		if (count < 0)
			Value = value;
		else
			Value.setString(value, count);
		Changed = true;
	}
	virtual void				addValue(const char* value, const char* limit = NULL)	
	{
		Value += value; Changed = true;
	}
	CzString&					getValue()						{ return Value; }
	virtual bool				isTrue()						{ return !Value.isEmpty(); }
	virtual bool				checkCondition(eCzXomlConditionOperator condition, const CzString& val);
	// Properties end
	CzXomlVariableManager*		Parent;				///< Parent manager
	CzString					Name;				///< Name of variable
	eCzXomlVariableType			Type;				///< Type of variable
	bool						Modified;			///< Modified since last read flag

	CzXomlVariable() : Type(VT_String), Modified(false), Changed(true), Persist(false), InstantSave(false) {}
	virtual ~CzXomlVariable() { Save(); }

	virtual void				Update()
	{
		if (Changed)
		{
			Modified = true;
			Changed = false;
			if (InstantSave)
				Save();
		}
		else
			Modified = false;
	}

	bool						isArray() const			{ return Type >= VT_ArrayString && Type <= VT_ArrayInt; }
	bool						isXML() const			{ return Type == VT_XML; }

	virtual bool				Save();
	virtual bool				Load();

	// Utility
	static CzXomlVariable* GetVariable(const CzString& value, IzXomlResource* parent);

};

struct CzXomlVariableBool : CzXomlVariable
{
	bool						NativeValue;		///< Native value of the variable
	CzXomlVariableBool() : CzXomlVariable() { Type = VT_Bool; }
	void						setValue(const char* value, int count = -1)
	{
		CzXomlVariable::setValue(value, count);
		NativeValue = Value.getAsBool();
	}
	void					addValue(const char* value, const char* limit = NULL)
	{
		NativeValue = !NativeValue;
		if (NativeValue)
			Value = "true";
		else
			Value = "false";
		Changed = true;
	}
	bool					isTrue()			{ return NativeValue; }
	bool					checkCondition(eCzXomlConditionOperator condition, const CzString& val);
};


struct CzXomlVariableFloat : CzXomlVariable
{
	float						NativeValue;		///< Native value of the variable
	CzXomlVariableFloat() : CzXomlVariable()  { Type = VT_Float; }
	void						setValue(const char* value, int count = -1)
	{
		CzXomlVariable::setValue(value, count);
		NativeValue = Value.getAsFloat();
		Changed = true;
	}
	void						addValue(const char* value, const char* limit = NULL)
	{
		if (limit != NULL)
		{
			float val = CzString(value).getAsFloat();
			if (val == 0)
				return;
			float lim = CzString(limit).getAsFloat();
	
			NativeValue += val;
			if (val < 0)
			{
				if (NativeValue < lim)
					NativeValue = lim;
			}
			else
			if (val > 0)
			{
				if (NativeValue > lim)
					NativeValue = lim;
			}
		}
		else
			NativeValue += CzString(value).getAsFloat();
		Value = NativeValue;
		Changed = true;
	}
	bool						isTrue()			{ return NativeValue != 0; }
	bool						checkCondition(eCzXomlConditionOperator condition, const CzString& val);
};

struct CzXomlVariableInt : CzXomlVariable
{
	int							NativeValue;		///< Native value of the variable
	CzXomlVariableInt() : CzXomlVariable()  { Type = VT_Int; }
	void						setValue(const char* value, int count = -1)
	{
		CzXomlVariable::setValue(value, count);
		NativeValue = Value.getAsInt();
		Changed = true;
	}
	void						addValue(const char* value, const char* limit = NULL)
	{
		if (limit != NULL)
	{
			int val = CzString(value).getAsInt();
			int lim = CzString(limit).getAsInt();
		NativeValue += val;
		if (val < 0)
		{
			if (NativeValue < lim)
				NativeValue = lim;
		}
		else
		if (val > 0)
		{
			if (NativeValue > lim)
				NativeValue = lim;
		}
		}
		else
			NativeValue += CzString(value).getAsInt();
		Value = NativeValue;
		Changed = true;
	}
	bool						isTrue()			{ return NativeValue != 0; }
	bool						checkCondition(eCzXomlConditionOperator condition, const CzString& val);
};

struct CzXomlVariableVec2 : CzXomlVariable
{
	CzVec2						NativeValue;		///< Native value of the variable
	CzXomlVariableVec2() : CzXomlVariable()  { Type = VT_Vec2; }
	void						setValue(const char* value, int count = -1)
	{
		CzXomlVariable::setValue(value, count);
		float components[8];
		if (Value.getAsListOfFloat(components) == 2)
		{
			NativeValue.x = components[0];
			NativeValue.y = components[1];
			Changed = true;
		}
	}
	void						addValue(const char* value, const char* limit = NULL)
	{
		CzString add = value;
		float components[8];
		if (add.getAsListOfFloat(components) == 2)
		{
			NativeValue.x += components[0];
			NativeValue.y += components[1];
			Changed = true;
			Value = NativeValue.x;
			Value += ",";
			Value += NativeValue.y;
		}
	}
	bool						isTrue()			{ return NativeValue.x != 0 && NativeValue.y != 0; }
	bool						checkCondition(eCzXomlConditionOperator condition, const CzString& val);
};

struct CzXomlVariableVec3 : CzXomlVariable
{
	CzVec3						NativeValue;		///< Native value of the variable
	CzXomlVariableVec3() : CzXomlVariable()  { Type = VT_Vec3; }
	void						setValue(const char* value, int count = -1)
	{
		CzXomlVariable::setValue(value, count);
		float components[8];
		if (Value.getAsListOfFloat(components) == 3)
		{
			NativeValue.x = components[0];
			NativeValue.y = components[1];
			NativeValue.z = components[2];
			Changed = true;
		}
	}
	void						addValue(const char* value, const char* limit = NULL)
	{
		CzString add = value;
		float components[8];
		if (add.getAsListOfFloat(components) == 3)
		{
			NativeValue.x += components[0];
			NativeValue.y += components[1];
			NativeValue.z += components[2];
			Changed = true;
			Value = NativeValue.x;
			Value += ",";
			Value += NativeValue.y;
			Value += ",";
			Value += NativeValue.z;
		}
	}
	bool						isTrue()			{ return NativeValue.x != 0 && NativeValue.y != 0 && NativeValue.z != 0; }
	bool						checkCondition(eCzXomlConditionOperator condition, const CzString& val);
};

struct CzXomlVariableVec4 : CzXomlVariable
{
	CzVec4						NativeValue;		///< Native value of the variable
	CzXomlVariableVec4() : CzXomlVariable()  { Type = VT_Vec4; }
	void						setValue(const char* value, int count = -1)
	{
		CzXomlVariable::setValue(value, count);
		float components[8];
		if (Value.getAsListOfFloat(components) == 4)
		{
			NativeValue.x = components[0];
			NativeValue.y = components[1];
			NativeValue.z = components[2];
			NativeValue.w = components[3];
			Changed = true;
		}
	}
	void						addValue(const char* value, const char* limit = NULL)
	{
		CzString add = value;
		float components[8];
		if (add.getAsListOfFloat(components) == 4)
		{
			NativeValue.x += components[0];
			NativeValue.y += components[1];
			NativeValue.z += components[2];
			NativeValue.w += components[3];
			Changed = true;
			Value = NativeValue.x;
			Value += ",";
			Value += NativeValue.y;
			Value += ",";
			Value += NativeValue.z;
			Value += ",";
			Value += NativeValue.w;
		}
	}
	bool						isTrue()			{ return NativeValue.x != 0 && NativeValue.y != 0 && NativeValue.z != 0 && NativeValue.w != 0; }
	bool						checkCondition(eCzXomlConditionOperator condition, const CzString& val);
};

struct CzXomlVariableXML : CzXomlVariable
{
	// Properties
protected:
	CzXmlParser*	Parser;
	CzXmlNode*		Root;					///< Root node
public:
	CzXmlNode*		getRoot()		{ return Root; }
	// Properties end
public:
	CzXomlVariableXML() : CzXomlVariable(), Root(NULL), Parser(NULL) { Type = VT_XML; }
	virtual ~CzXomlVariableXML()
	{
		SAFE_DELETE(Parser);
	}
	void						setValue(const char* value, int count = -1);
};

struct CzXomlVariableCondition : CzXomlVariable
{
	bool						NativeValue;		///< Native value of a condition is boolean
	CzXomlConditions			Conditions;			///< Conditions that should be checked to determine true or false
	CzXomlVariableCondition() : CzXomlVariable()  { Type = VT_Condition; }
	virtual ~CzXomlVariableCondition()
	{
		for (CzXomlConditions::iterator it = Conditions.begin(); it != Conditions.end(); ++it)
			delete *it;
	}
	void						setValue(const char* value, int count = -1);
	void						addValue(const char* value, const char* limit = NULL)	{ }
	bool						isTrue();
};

struct CzXomlVariableArray : public CzXomlVariable
{
	// Properties
protected:
	CzSlotArray<CzXomlVariable*>	Values;						///< Array of values
	CzXomlVariableXML*				XmlVar;						///< XML variable to copy attributes from
	unsigned int					XmlTagNameHash;				///< XML Tag name hash
	unsigned int					XmlAttribNameHash;			///< XML Attribute name hash
#if defined(_DEBUG)
	CzString						XmlTagName;
	CzString						XmlAttribName;
#endif
protected:
	void						UpdateFromXML();
public:
	CzXomlVariable*				getElement(int index)					{ return Values.element_at(index); }
	void						addElement(CzXomlVariable* var)			{ Values.add(var); }
	int							getSize() const							{ return Values.getSize(); }
	int							getCount() const						{ return Values.count(); }
	void						setValue(const char* value, int count = -1);
	virtual void				setValue(int index, const char* value)
	{
		if (index < 0 || index >= Values.getSize())
		{
#if defined(_DEBUG)
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Array index out of bounds - ", Name.c_str());
#endif	// _DEBUG
			return;
		}
		Changed = true;

		Values.element_at(index)->setValue(value);
	}
	CzXomlVariableXML*			getXmlVar()								{ return XmlVar; }
	void						setXmlTagName(const char* name)
	{
#if defined(_DEBUG)
		XmlTagName = name;
#endif
		XmlTagNameHash = CZ_HASH(name);
	}
	void						setXmlAttribName(const char* name)
	{
#if defined(_DEBUG)
		XmlAttribName = name;
#endif
		XmlAttribNameHash = CZ_HASH(name);
	}
	void						BindXML(const char* binding);
	// Properties end

	CzXomlVariableArray()
	{
		XmlVar = NULL;
		Type = VT_ArrayString;
		Modified = false;
		Changed = false;
		XmlTagNameHash = 0;
		XmlAttribNameHash = 0;
	}
	virtual ~CzXomlVariableArray()
	{
		Save();
		for (int t = 0; t < Values.getSize(); t++)
		{
			if (Values.element_at(t) != NULL)
				delete Values.element_at(t);
		}
	}

	virtual void	Init(int size)
	{
		Values.resize(size);

		for (int t = 0; t < size; t++)
		{
			if (Values.element_at(t) == NULL)
				Values.set(t, new CzXomlVariable());
		}
	}

	void			Update();

	bool			Save();
	bool			Load();
	bool			checkCondition(eCzXomlConditionOperator condition, const CzString& val);

};

struct CzXomlVariableArrayBool : public CzXomlVariableArray
{
	// Properties
protected:
public:
	// Properties end

	CzXomlVariableArrayBool()
	{
		Type = VT_ArrayBool;
		Modified = false;
		Changed = false;
	}

	virtual void Init(int size)
	{
		Values.resize(size);

		for (int t = 0; t < Values.getSize(); t++)
			Values.set(t, new CzXomlVariableBool());
	}
	bool			checkCondition(eCzXomlConditionOperator condition, const CzString& val);
};

struct CzXomlVariableArrayFloat : public CzXomlVariableArray
{
	// Properties
protected:
public:
	// Properties end

	CzXomlVariableArrayFloat()
	{
		Type = VT_ArrayFloat;
		Modified = false;
		Changed = false;
	}

	virtual void Init(int size)
	{
		Values.resize(size);

		for (int t = 0; t < Values.getSize(); t++)
			Values.set(t, new CzXomlVariableFloat());
	}
	bool			checkCondition(eCzXomlConditionOperator condition, const CzString& val);
};

struct CzXomlVariableArrayInt : public CzXomlVariableArray
{
	// Properties
protected:
public:
	// Properties end

	CzXomlVariableArrayInt()
	{
		Type = VT_ArrayInt;
		Modified = false;
		Changed = false;
	}

	virtual void Init(int size)
	{
		Values.resize(size);

		for (int t = 0; t < Values.getSize(); t++)
			Values.set(t, new CzXomlVariableInt());
	}
	bool			checkCondition(eCzXomlConditionOperator condition, const CzString& val);
};


// 
// 
//
//
// CzXomlVariableManager - Manages a collection of XOML variables
//
//
//
//
class CzXomlVariableManager
{
public:
	// Public access to iteration
	typedef CzList<CzXomlVariable*>::iterator _Iterator;
	_Iterator				begin()		{ return Variables.begin(); }
	_Iterator				end()		{ return Variables.end(); }

protected:
	// Properties
	CzList<CzXomlVariable*>	Variables;			///< A collection of xoml variables
	IzXomlResource*			Parent;				///< Parent container
public:
	void					addVariable(CzXomlVariable* variable);
	CzXomlVariable*	addVariable(const char* name, eCzXomlVariableType type = VT_String);
	CzXomlVariable*	addVariable(const char* name, eCzXomlVariableType type, const char* value, int size);
	void					removeVariable(CzXomlVariable* var);
	void					removeVariable(unsigned int name_hash);
	void					setVariable(unsigned int name_hash, const char* value);
	void					setVariable(const char* name, const char* value);
	CzXomlVariable*	findVariable(unsigned int name_hash);
	CzXomlVariable*	findVariable(const char* name);
	void					clearVariables();
	void					setParent(IzXomlResource* scene)		{ Parent = scene; }
	IzXomlResource*			getParent()								{ return Parent; }
	// Properties end

public:
	CzXomlVariableManager() : Parent(NULL) {}
	virtual ~CzXomlVariableManager() { clearVariables(); }

	CzXomlVariable*	CreateVariable(const char* name, eCzXomlVariableType type = VT_String);

	void					Update()
	{
		for (_Iterator it = begin(); it != end(); ++it)
		{
			(*it)->Update();
		}
	}

	// Utility
	static CzXomlVariable*	FindVariable(const char* name, IzXomlResource* container);
	static CzXomlVariable*	FindVariable(unsigned int name_hash, IzXomlResource* container);
};

//
//
//
//
// CzXomlAddVariable utility class to allow XOML files to add variables
// 
//
// 
//
class CzXomlAddVariable : public IzXomlResource
{
public:
protected:
	// Properties
public:
	// Properties end
public:
	CzXomlAddVariable() : IzXomlResource() { setClassType("variable"); }
	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parebt, bool load_children, CzXmlNode* node);
};

class CzXomlAddVariableCreator : public IzXomlClassCreator
{
public:
	CzXomlAddVariableCreator()
	{
		setClassName("variable");
	}
	IzXomlResource* CreateInstance(IzXomlResource* parent)	{ return new CzXomlAddVariable(); }
};


#endif	// _CZ_XOML_VARIABLES_H_
