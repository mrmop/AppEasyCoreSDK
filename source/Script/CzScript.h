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

#if !defined(_CZ_SCRIPT_H_)
#define _CZ_SCRIPT_H_

#include "CzXoml.h"
#include "CzXomlVariables.h"

class CzScene;

/**
 @class	IzScriptEngine

 @brief	Abstract base class that all script engines derive from

 */

class IzScriptEngine
{
public:
	// Proprties
protected:
	CzScene*	Scene;						///< Scene that contains this script engine
public:
	CzScene*	getScene()						{ return Scene; }
	// Properties end

protected:
public:

	IzScriptEngine() : Scene(NULL) {}
	virtual ~IzScriptEngine() {}

	virtual int			Init(CzScene* scene) = 0;
	virtual void		Release() = 0;
	virtual int			LoadScript(const char* script, int script_len, const char* name) = 0;
	virtual int			CallFunction(IzXomlResource* object, CzString* function_name, CzString* param1, CzString* param2, CzString* param3) = 0;
	virtual CzXomlProperty getVariable(CzString* var_name) = 0;
	virtual int			CallFunctionRef(int function_ref, CzString* param1, CzString* param2, CzString* param3, CzString* param4) = 0;
	virtual int			CallFunctionRefWithTable(int function_ref, CzList<const char*>* properties, CzList<const char*>* values) = 0;
	virtual int			CallFunctionRefWithObject(void* object, int function_ref, CzString* param1, CzString* param2, CzString* param3, CzString* param4) = 0;
	virtual CzString	getType() = 0;

	// Internal
};

/**
 @struct	CzScriptCallback

 @brief	A script callback.

 */

struct CzScriptCallback
{
	bool					Valid;				///< Ttue if this callback is still valid
	int						FunctionRef;		///< Functiom reference (usually an index)
	IzScriptEngine*	ScriptEngine;				///< Script engine that contains the callback

	CzScriptCallback() : Valid(false), FunctionRef(-1), ScriptEngine(NULL) {}
};

/**
 @class	CzScript

 @brief	A script.

 */

class CzScript : public IzXomlResource
{
public:
	enum eScriptType
	{
		ST_None,  
		ST_LUA, 
		ST_AngelScript, 
		ST_Javascript, 
		ST_Python, 
	};
	// Proprties
protected:
	eScriptType				ScriptType;			///< Script type
	char*					Script;				///< The actual script
	int						ScriptLength;		///< Length of script in bytes
public:
	void					setScriptType(eScriptType type)			{ ScriptType = type; }
	eScriptType				getScriptType() const					{ return ScriptType; }
	bool					setScript(const char* script, int script_len);
	const char*				getScript() const						{ return Script; }
	int						getScriptLength() const					{ return ScriptLength; }
	// Properties end

protected:
public:

	CzScript() : IzXomlResource(), ScriptType(ST_None), Script(NULL), ScriptLength(0)	{ setClassType("script"); }
	virtual ~CzScript()
	{
		SAFE_DELETE(Script)
	}

	int					Init(const char* script, int script_len, CzScene* scene = NULL);

	// Implementation of IzXomlResource interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Internal
};

/**
 @class	CzScriptCreator

 @brief	Creates an instance of a game script object from XOML.

 */

class CzScriptCreator : public IzXomlClassCreator
{
public:
	CzScriptCreator()
	{
		setClassName("script");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzScript(); }
};


#endif	// _CZ_SCRIPT_H_
