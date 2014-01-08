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

#if !defined(_CZ_COMMANDS_BASE_H_)
#define _CZ_COMMANDS_BASE_H_

#include "CzProgram.h"
#include "CzTime.h"

//
//
//
// CzCommandNop - Nop command - This command does nothing and is used for commands with no method
// 
// 
//
class CzCommandNop : public CzCommand
{
public:
	void			Init();
	bool			Execute(float dt);
};
class CzCommandNopCreator : public CzCommandCreator
{
public:
	CzCommandNopCreator()
	{
		setCommandName("nop");
	}
	CzCommand* CreateInstance() { return new CzCommandNop(); }
};

//
//
//
// CzCommandChangeProgram - Change command - Changes a program
// 
// 
//
class CzCommandChangeProgram : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandChangeProgramCreator : public CzCommandCreator
{
public:
	CzCommandChangeProgramCreator()
	{
		setCommandName("change");
	}
	CzCommand* CreateInstance() { return new CzCommandChangeProgram(); }
};

//
//
//
// CzCommandGoto - Goto command - This command changed execution from the current command to the spcific command
// 
// 
//
class CzCommandGoto : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandGotoCreator : public CzCommandCreator
{
public:
	CzCommandGotoCreator()
	{
		setCommandName("goto");
	}
	CzCommand* CreateInstance() { return new CzCommandGoto(); }
};

//
//
//
// CzCommandStop - Stop command - This command stops execution of the program
// 
// 
//
class CzCommandStop : public CzCommand
{
public:
	bool			Execute(float dt)
	{
		if (!IzCommandExecutor::Execute(dt))
			return false;
		Program->stop();
		return false;
	}
};
class CzCommandStopCreator : public CzCommandCreator
{
public:
	CzCommandStopCreator()
	{
		setCommandName("stop");
	}
	CzCommand* CreateInstance() { return new CzCommandStop(); }
};

//
//
//
// CzCommandPriority - Priority command - This command changes the current program that has priority exection
// 
// 
//
class CzCommandPriority : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandPriorityCreator : public CzCommandCreator
{
public:
	CzCommandPriorityCreator()
	{
		setCommandName("priority");
	}
	CzCommand* CreateInstance() { return new CzCommandPriority(); }
};

//
//
//
// CzCommandCall - Call command - Calls another program, pausing the current program
// 
// 
//
class CzCommandCall : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandCallCreator : public CzCommandCreator
{
public:
	CzCommandCallCreator()
	{
		setCommandName("call");
	}
	CzCommand* CreateInstance() { return new CzCommandCall(); }
};

//
//
//
// CzCommanReturn - Return command - Returns another program, pausing the current program
// 
// 
//
class CzCommandReturn : public CzCommand
{
public:
	bool			Execute(float dt)
	{
		if (!IzCommandExecutor::Execute(dt))
			return false;

		Program->returnToCaller();

		return false;
	}
};
class CzCommandReturnCreator : public CzCommandCreator
{
public:
	CzCommandReturnCreator()
	{
		setCommandName("return");
	}
	CzCommand* CreateInstance() { return new CzCommandReturn(); }
};

//
//
//
// CzCommandRunActions - Run Actions - Applies an actions list to a scene or actor. Param 1 - action to execute, Param 2 - Actor or scene to apply actions, Param3 - Scene in which actor lives (optional)
// 
// 
//
class CzCommandRunActions : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandRunActionsCreator : public CzCommandCreator
{
public:
	CzCommandRunActionsCreator()
	{
		setCommandName("run_actions");
	}
	CzCommand* CreateInstance() { return new CzCommandRunActions(); }
};

//
//
//
// CzCommandSetProperty - Set property - Sets a property of an actor/ scene. Param 1 - Property to set, Param2 - Property value,  Param 3 - Actor whos property to change, Param4 - Scene in which actor lives (optional) or scene who's property to change
// 
// 
//
class CzCommandSetProperty : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandSetPropertyCreator : public CzCommandCreator
{
public:
	CzCommandSetPropertyCreator()
	{
		setCommandName("set_property");
	}
	CzCommand* CreateInstance() { return new CzCommandSetProperty(); }
};

//
//
//
// CzCommandSetUserProp - Set property - Sets a user property of an actor / scene. Param 1 - User property to set, Param2 - Property value,  Param 3 - Actor whos property to change, Param4 - Scene in which actor lives (optional) or scene who's user property to change
// 
// 
//
class CzCommandSetUserProp : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandSetUserPropCreator : public CzCommandCreator
{
public:
	CzCommandSetUserPropCreator()
	{
		setCommandName("set_userprop");
	}
	CzCommand* CreateInstance() { return new CzCommandSetUserProp(); }
};

//
//
//
// CzCommandAddVariable - Add variable - Adds a value onto a variable. Param 1 - variable to modify, Param2 - Value to add,  Param3 - Scene in which variable lives (optional)
// 
// 
//
class CzCommandAddVariable : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandAddVariableCreator : public CzCommandCreator
{
public:
	CzCommandAddVariableCreator()
	{
		setCommandName("add_var");
	}
	CzCommand* CreateInstance() { return new CzCommandAddVariable(); }
};

//
//
//
// CzCommandSetVariable - Set variable - Sets a variables value. Param 1 - variable to set, Param2 - Variables value,  Param3 - Scene in which variable lives (optional)
// 
// 
//
class CzCommandSetVariable : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandSetVariableCreator : public CzCommandCreator
{
public:
	CzCommandSetVariableCreator()
	{
		setCommandName("set_var");
	}
	CzCommand* CreateInstance() { return new CzCommandSetVariable(); }
};

//
//
//
// CzCommandGetVariable - Get variable - Gets a variables value. Param 1 - variable to get, Param3 - Scene in which variable lives (optional)
// 
// 
//
class CzCommandGetVariable : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandGetVariableCreator : public CzCommandCreator
{
public:
	CzCommandGetVariableCreator()
	{
		setCommandName("get_var");
	}
	CzCommand* CreateInstance() { return new CzCommandGetVariable(); }
};

//
// CzCommandIfVar - Checks variable against a value - Param1 - variable to check, Param2 - operator, Param3 - value, Param4 - Scene where variable is located (optional)
//
class CzCommandIfVar : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandIfVarCreator : public CzCommandCreator
{
public:
	CzCommandIfVarCreator()
	{
		setCommandName("if_var");
	}
	CzCommand* CreateInstance() { return new CzCommandIfVar(); }
};

//
// CzCommandWaitVarIsValue - Wait for a variable to becoem a specific value - Param1 - variable to check, Param2 - Value that variable should be to continue, Param3 - Scene where variable is located (optional)
//
class CzCommandWaitVarIsValue : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandWaitVarIsValueCreator : public CzCommandCreator
{
public:
	CzCommandWaitVarIsValueCreator()
	{
		setCommandName("wait_var_is_value");
	}
	CzCommand* CreateInstance() { return new CzCommandWaitVarIsValue(); }
};

//
// CzCommandCallScript - Calls a script - Param1 - script function to call, Param2 - scene to pass, Param3, Param4, Param5 - Params to pass to script function
//
class CzCommandCallScript : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandCallScriptCreator : public CzCommandCreator
{
public:
	CzCommandCallScriptCreator()
	{
		setCommandName("call_script");
	}
	CzCommand* CreateInstance() { return new CzCommandCallScript(); }
};

//
// CzCommandCallGlobalScript - Calls a global script - Param1 - script function to call, Param2, Param3, Parm4 - Params to pass to script function
//
class CzCommandCallGlobalScript : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandCallGlobalScriptCreator : public CzCommandCreator
{
public:
	CzCommandCallGlobalScriptCreator()
	{
		setCommandName("call_global_script");
	}
	CzCommand* CreateInstance() { return new CzCommandCallGlobalScript(); }
};

//
// CzCommandInline - Calls a script - Param1 - script to run, Param2 - scene to run script in, if not supplied then global script space is used
//
class CzCommandInline : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandInlineCreator : public CzCommandCreator
{
public:
	CzCommandInlineCreator()
	{
		setCommandName("inline");
	}
	CzCommand* CreateInstance() { return new CzCommandInline(); }
};

//
// CzCommandFromTemplate - Instantiates a template - Param1 - template name, Param2 - Parameters to pass to template (separated by colon character), Param3 - Scene to instantiate template into (optional)
//
class CzCommandFromTemplate : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandFromTemplateCreator : public CzCommandCreator
{
public:
	CzCommandFromTemplateCreator()
	{
		setCommandName("from_template");
	}
	CzCommand* CreateInstance() { return new CzCommandFromTemplate(); }
};

//
// CzCommandFromTemplate - Loads a XOMl file - Param1 - filename, Param2 - Name of scene to load XOML data into, if not supplied then XOML is loaded globally (default)
//
class CzCommandLoadXOML : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandLoadXOMLCreator : public CzCommandCreator
{
public:
	CzCommandLoadXOMLCreator()
	{
		setCommandName("load_xoml");
	}
	CzCommand* CreateInstance() { return new CzCommandLoadXOML(); }
};

//
// CzCommandMusic - Play or stop music, Param1 - command (play or stop), Param2 - music file name to play, Param3 - Repeat count (0 play forever)
//
class CzCommandMusic : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandMusicCreator : public CzCommandCreator
{
public:
	CzCommandMusicCreator()
	{
		setCommandName("music");
	}
	CzCommand* CreateInstance() { return new CzCommandMusic(); }
};

//
// CzCommandSound - Play sound effect, Param1 - Sound effect name to play, Param2 - volume, Param3 - pitch, Param4 - pan, Param5 - specific scene (optional)
//
class CzCommandSound : public CzCommand
{
public:
	bool			Execute(float dt);
};
class CzCommandSoundCreator : public CzCommandCreator
{
public:
	CzCommandSoundCreator()
	{
		setCommandName("sound");
	}
	CzCommand* CreateInstance() { return new CzCommandSound(); }
};

//
// CzCommandWait - Waits specified amount of time, Param1 - Time to wait in seconds
//
class CzCommandWait : public CzCommand
{
protected:
	CzTimer			Timer;
public:
	void			Init();
	bool			Execute(float dt);
};
class CzCommandWaitCreator : public CzCommandCreator
{
public:
	CzCommandWaitCreator()
	{
		setCommandName("wait");
	}
	CzCommand* CreateInstance() { return new CzCommandWait(); }
};

//
// CzCommandDebugText - Writes dbug text to the output, Param1 - text / var, Param2 = text / var
//
class CzCommandDebugText : public CzCommand
{
protected:
public:
	bool			Execute(float dt);
};
class CzCommandDebugTextCreator : public CzCommandCreator
{
public:
	CzCommandDebugTextCreator()
	{
		setCommandName("debug_text");
	}
	CzCommand* CreateInstance() { return new CzCommandDebugText(); }
};

//
// CzCommandRemoteReq - Requests remote data, Param1 - RemoteReq resource name, Param2 = optional data to pass to request
//
class CzCommandRemoteReq : public CzCommand
{
protected:
public:
	bool			Execute(float dt);
};
class CzCommandRemoteReqCreator : public CzCommandCreator
{
public:
	CzCommandRemoteReqCreator()
	{
		setCommandName("remote_req");
	}
	CzCommand* CreateInstance() { return new CzCommandRemoteReq(); }
};


#endif	// _CZ_COMMANDS_BASE_H_
