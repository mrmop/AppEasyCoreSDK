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

#if !defined(_CZ_PROGRAM_H_)
#define _CZ_PROGRAM_H_

#include "CzUtil.h"
#include "CzString.h"
#include "CzXoml.h"
#include "CzXomlVariables.h"

class CzProgram;
class CzProgramManager;

// 
// 
//
//
// IzCommandExecutor - Common base classs used by objects that execute a list of CzCommand's
// 
// 
// 
//
class IzCommandExecutor
{
public:
	// Public access to iteration
	typedef CzVector<IzCommandExecutor*>::iterator _Iterator;
	_Iterator				begin()		{ return Commands.begin(); }
	_Iterator				end()		{ return Commands.end(); }

protected:
	// Properties
	CzProgram*					Program;			// Parent container program
	IzCommandExecutor*			Parent;				// Parent command executor
	CzVector<IzCommandExecutor*> Commands;			// A collection of commands
	int							CurrentCommand;		// Index of currently executing command
	bool						Parallel;			// if true then child commands will be executed in parallel
	unsigned int				Conditions;			// Name of conditions variable that defines a set of conditions that must be met for this command to be executed
	CzString					_Conditions;
	bool						Not;				// if true then result of condition will be inverted
#if defined (_DEBUG)
	CzString					CommandName;		// Command name
#endif	// _DEBUG
	unsigned int				CommandNameHash;	// Command name hash
	bool						Initialised;		// Initialised state
	bool						Finished;			// Command has finished and is in a waiting state
	int							ReturnValue;		// Return value
	IzCommandExecutor*			IfReturn;			// Command that we check return value of
	int							IfReturnValue;		// Value to check return again against
	int							CommandIndex;		// Command index in parent program

public:
	void					setProgram(CzProgram* program)			{ Program = program; }
	CzProgram*				getProgram()							{ return Program; }
	void					setParent(IzCommandExecutor* parent)	{ Parent = parent; }
	IzCommandExecutor*		getParent()								{ return Parent; }
	void					setCommandName(const char* name)
	{
#if defined (_DEBUG)
		CommandName = name;
#endif	// _DEBUG
		CommandNameHash = CzString::CalculateHash(name);
	}
#if defined (_DEBUG)
	CzString&				getCommandName()					{ return CommandName; }
#endif	// _DEBUG
	unsigned int			getCommandNameHash()				{ return CommandNameHash; }
	void					setInitialised(bool init)			{ Initialised = init; }
	bool					isInitialised() const				{ return Initialised; }
	void					setFinished(bool finished)			{ Finished = finished; }
	bool					isFinished() const					{ return Finished; }
	void					setParallel(bool parallel)			{ Parallel = parallel; }
	bool					isParallel() const					{ return Parallel; }
	void					addCommand(IzCommandExecutor* command);
	bool					removeCommand(IzCommandExecutor* command);
	IzCommandExecutor*		findCommand(unsigned int name_hash);
	IzCommandExecutor*		findCommand(const char* name);
	bool					gotoCommand();
	virtual void			restart();
	void					clearCommands();
	bool					nextCommand();
	void					setCurrentCommand(IzCommandExecutor* command);
	void					setCurrentCommand(int index);
	int						getReturnValue() const				{ return ReturnValue; }
	IzCommandExecutor*		getIfReturn()						{ return IfReturn; }
	void					setIfReturn(IzCommandExecutor* command, int value);
	int						getIfReturnValue() const			{ return IfReturnValue; }
	void					setCommandIndex(int index)			{ CommandIndex = index; }
	int						getCommandIndex() const				{ return CommandIndex; }
	void					setConditions(const char* cond, bool _not = false)
	{
		_Conditions = cond;
		Conditions = CZ_HASH(cond);
		Not = _not;
	}
	unsigned int			getConditions()	const	{ return Conditions; }

	// Properties end
protected:
public:
	IzCommandExecutor() : Parent(NULL), Initialised(false), Parallel(false), Finished(false), CommandNameHash(0), Program(NULL), ReturnValue(0), IfReturnValue(0), IfReturn(NULL), Conditions(0), Not(false)	{ CurrentCommand = 0; CommandIndex = -1; }
	virtual ~IzCommandExecutor()							{ clearCommands(); }

	virtual void			Init() { Finished = false; }		// Initialise the command
	virtual bool			Execute(float dt) = 0;				// Execute the command
	void					Uninit();							// Unitialises this command all children

	// Utility
	void					RebuildCommandIndices();
};

//
//
//
//
// CzCommand - A program command - Derive program commands from CzCommand so they can be added to a CzProgram or another CzCommand derived command
//
//
//
//
class CzCommand : public IzCommandExecutor
{
public:
	// Public access to iteration

	// Properties
protected:
	CzString			Params[5];					// Commands parameters
	CzXomlVariable*		Variables[5];				// Cached variable parameters (prevents continually searching for variables)
public:
	void				setParameter(int index, const CzString& data)	{ Params[index] = data; }
	void				setParameter(int index, const char* data)		{ Params[index] = data; }
	// Properties End
public:
	CzCommand() : IzCommandExecutor() { for (int t = 0; t < 5; t++) Variables[t] = (CzXomlVariable*)1; } // Chances of pointer at address 1 is none existent 
	CzString&		getParameter1(IzXomlResource* parent);
	CzString&		getParameter2(IzXomlResource* parent);
	CzString&		getParameter3(IzXomlResource* parent);
	CzString&		getParameter4(IzXomlResource* parent);
	CzString&		getParameter5(IzXomlResource* parent);
};

// 
// 
//
//
// CzCommandCreator - Base class that is used by classes that create an instance of a type of command
//
//
//
//
class CzCommandCreator
{
#if defined(_DEBUG)
	CzString			CommandName;				// Name of command
#endif
	unsigned int		CommandNameHash;			// Hashed name of class
public:
	void				setCommandName(const char* name)
	{
#if defined(_DEBUG)
		CommandName = name;
#endif
		CommandNameHash = CzString::CalculateHash(name);
	}
	unsigned int		getCommandNameHash() const				{ return CommandNameHash; }
#if defined(_DEBUG)
	CzString&			getCommandtName()						{ return CommandName; }
#endif
public:
	virtual ~CzCommandCreator() {}
	virtual CzCommand* CreateInstance() = 0;
};


// 
// 
//
//
// IzProgram - Manages a collection of commands (a program)
//
//
//
//
class CzProgram : public IzXomlResource, public IzCommandExecutor
{
public:
	enum eProgram_Status
	{
		PS_Stopped, 
		PS_Running, 
		PS_Paused, 
	};

protected:
	// Properties
	CzProgramManager*		Manager;			// Parent program manager
	eProgram_Status			Status;				// Status of the program
	IzCommandExecutor*		LastCommand;		// Last executed command
	CzProgram*				Caller;				// If this program was called from another then Caller represents the calling program
public:
	void					setManager(CzProgramManager* manager)		{ Manager = manager; }
	CzProgramManager*		getManager()								{ return Manager; }
	void					setCaller(CzProgram* program)				{ Caller = program; }
	CzProgram*				getCaller()									{ return Caller; }
	void					stop()										{ Status = PS_Stopped; }
	void					start()										{ Status = PS_Running; }
	void					pause()										{ Status = PS_Paused; }
	void					restart();
	bool					isRunning() const							{ return Status == PS_Running; }
	bool					isPaused() const							{ return Status == PS_Paused; }
	bool					isStopped() const							{ return Status == PS_Stopped; }
	void					call(CzProgram* program);
	void					returnToCaller();
	void					gotoNextCommand();
	void					setLastCommand(IzCommandExecutor* command)	{ LastCommand = command; }
	IzCommandExecutor*		getLastCommand()							{ return LastCommand; }

	// Properties end
protected:
	bool					ProcessCommands(CzCommand* parent, bool load_children, CzXmlNode* node);

public:
	CzProgram() : IzXomlResource(), Status(PS_Paused), Manager(NULL), LastCommand(NULL), Caller(NULL) { setClassType("program"); CurrentCommand = 0; }
	virtual ~CzProgram() { clearCommands(); }

	bool					Execute(float dt);

	// Implementation of IzXomlResource interface
	int						LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);
};

//
//  CzProgramCreator - Creates an instance of a program
//
class CzProgramCreator : public IzXomlClassCreator
{
public:
	CzProgramCreator()
	{
		setClassName("program");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzProgram(); }
};

// 
// 
//
//
// CzProgramManager - Manages a collection of programs
//
//
//
//
class CzProgramManager
{
public:
	// Public access to iteration
	typedef CzList<CzProgram*>::iterator _Iterator;
	_Iterator				begin()		{ return Programs.begin(); }
	_Iterator				end()		{ return Programs.end(); }

protected:
	// Properties
	IzXomlResource*	Parent;					// Parent container scene
	CzProgram*			PriorityProgram;		// This program has priority and is ran before all other programs
	CzList<CzProgram*> Programs;				// A collection of programms
public:
	void					setPriorityProgram(CzProgram* program)		{ PriorityProgram = program; }
	CzProgram*				getPriorityProgram()						{ return PriorityProgram; }
	void					addProgram(CzProgram* Program);
	bool					removeProgram(CzProgram* Program);
	CzProgram*				findProgram(unsigned int name_hash);
	CzProgram*				findProgram(const char* name);
	void					clearPrograms();
	void					stopAll();
	void					startAll();
	void					pauseAll();
	void					setParent(IzXomlResource* scene)		{ Parent = scene; }
	IzXomlResource*	getParent()										{ return Parent; }
	// Properties end
protected:

public:
	CzProgramManager() : PriorityProgram(NULL), Parent(NULL) {}
	virtual ~CzProgramManager() { clearPrograms(); }

	void					Execute(float dt);

	// Utility
	static CzProgram*	FindProgram(const char* name, IzXomlResource* container);
	static CzProgram*	FindProgram(unsigned int name_hash, IzXomlResource* container);

};

// 
// 
//
//
// CzPrograms - CzPrograms is the main controller responsible for storing command creators and creating instances of commands
// 
// 
// 
//
#define CZ_PROGRAMS	CzPrograms::getInstance()
class CzPrograms
{
public:
	CDEFINE_SINGLETON(CzPrograms)

	// Public access to command creator iteration
	typedef CzList<CzCommandCreator*>::iterator _Iterator;
	_Iterator				begin() { return CommandCreators.begin(); }
	_Iterator				end() { return CommandCreators.end(); }
protected:
	// Properties
public:
	void					addCommand(CzCommandCreator* creator)
	{
#if defined(_DEBUG)
		if (findCreator(creator->getCommandNameHash()))
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command creator already exists in command creator list - ", creator->getCommandtName().c_str());
			delete creator;
			return;
		}
#endif	//(_DEBUG)
		CommandCreators.push_back(creator);
	}
	CzCommandCreator*		findCreator(unsigned int name_hash);
	CzCommandCreator*		findCreator(const char* name);
	// Properties end

protected:
	CzList<CzCommandCreator*> CommandCreators;

public:
	void			Init();
	void			Release();
};


#endif	// _CZ_PROGRAM_H_
