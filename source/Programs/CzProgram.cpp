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

#include "CzProgram.h"
#include "CzCommandsBase.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzHashes.h"
#include "CzUserProperty.h"

// 
// 
//
//
// IzCommandExecutor implementation
//
//
//
//

void IzCommandExecutor::addCommand(IzCommandExecutor* command)
{
	command->setCommandIndex(Commands.size());
	Commands.push_back(command);
	command->setParent(this);
}

bool IzCommandExecutor::removeCommand(IzCommandExecutor* command)
{
	for (_Iterator it = Commands.begin(); it != Commands.end(); ++it)
	{
		if (*it == command)
		{
			delete *it;
			Commands.erase(it);
			RebuildCommandIndices();
			return true;
		}
	}

	return false;
}

IzCommandExecutor* IzCommandExecutor::findCommand(unsigned int name_hash)
{
	if (CommandNameHash == name_hash)
		return this;

	for (_Iterator it = Commands.begin(); it != Commands.end(); ++it)
	{
		IzCommandExecutor* found = (*it)->findCommand(name_hash);
		if (found != NULL)
			return found;
	}

	return NULL;
}

IzCommandExecutor* IzCommandExecutor::findCommand(const char* name)
{
	return findCommand(CzString::CalculateHash(name));
}

void IzCommandExecutor::setCurrentCommand(IzCommandExecutor* command)
{
	if (Parent != NULL)
	{
		int index = command->getCommandIndex();
		Parent->setCommandIndex(index);
		Parent->setCurrentCommand(index);
	}
}

bool IzCommandExecutor::gotoCommand()
{
	setCurrentCommand(this);

	return true;
}

void IzCommandExecutor::RebuildCommandIndices()
{
	int index = 0;
	for (_Iterator it = Commands.begin(); it != Commands.end(); ++it, index++)
		(*it)->setCommandIndex(index);
}


void IzCommandExecutor::Uninit()
{
	Initialised = false;

	for (_Iterator it = Commands.begin(); it != Commands.end(); ++it)
		(*it)->Uninit();
}


void IzCommandExecutor::restart()
{
	CurrentCommand = 0;
	Initialised = false;
	Finished = false;

	for (_Iterator it = Commands.begin(); it != Commands.end(); ++it)
		(*it)->restart();
}

void IzCommandExecutor::clearCommands()
{
	// Remove and delete all images from the manager
	for (_Iterator it = Commands.begin(); it != Commands.end(); ++it)
	{
		(*it)->clearCommands();
		delete *it;
	}

	Commands.clear();
}

bool IzCommandExecutor::nextCommand()
{
	IzCommandExecutor* current_command = Commands[CurrentCommand];
	if (current_command != NULL)
	{
		// Reset the old ocmmand so it can be re-used
		current_command->restart();
	}

	if (Parallel)
	{
		// return to any previous command executor
		if (Parent != NULL)
			Parent->nextCommand();

		return false;
	}

	CurrentCommand++;
	if (CurrentCommand >= (int)Commands.size())
	{
		// return to any previous command executor
		if (Parent != NULL)
			Parent->nextCommand();
		else
		{
			((CzProgram*)this)->restart();
			((CzProgram*)this)->stop();
		}

		return false;
	}

	// Skip to next command if the new command fails a return value check
	current_command = Commands[CurrentCommand];
	current_command->setInitialised(false);
	current_command->setCurrentCommand(0);
	if (current_command->getIfReturn() != NULL)
	{
		if (current_command->getIfReturn()->getReturnValue() != current_command->getIfReturnValue())
			nextCommand();
	}
	
	return true;
}

void IzCommandExecutor::setCurrentCommand(int index)
{
	if (index < 0 || index >= (int)Commands.size())
		return;

	CurrentCommand = index;
}

void IzCommandExecutor::setIfReturn(IzCommandExecutor* command, int value)
{
	IfReturn = command;
	IfReturnValue = value;
}

bool IzCommandExecutor::Execute(float dt)
{
	if (Conditions != 0)
	{
		// Find the condition variable
		CzXomlVariable* var = NULL;
		
		if (Program == NULL)
			var = CzXomlVariable::GetVariable(_Conditions, NULL);
		else
		{
			unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
			if (class_hash == CzHashes::Game_Hash)
				var = CzXomlVariable::GetVariable(_Conditions, NULL);
			else
			if (class_hash == CzHashes::Scene_Hash)
				var = CzXomlVariable::GetVariable(_Conditions, (CzScene*)Program->getManager()->getParent());
		}
		
		if (var != NULL)
		{
			bool res = var->isTrue();
			if (Not)
				res = !res;
			if (!res)
				return false;
		}
#if defined (_DEBUG)
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Program - Condition variable not found - ", _Conditions.c_str(), Program->getDebugInfo().c_str());
#endif // _DEBUG
	}

	// If commands fails its return value check and the command is part of a parallel processnig command then dont allow user to run the command
	if (IfReturn != NULL)
	{
		if (IfReturn->getReturnValue() != IfReturnValue)
		{
			if (Parent != NULL && Parent->isParallel())
				return false;
		}
	}

	if (Program != NULL)
		Program->setLastCommand(this);

	if (Commands.size() == 0)
		return true;

	// Process command children
	if (Parallel)
	{
		// Process all commands in parallel
		int num_finished = 0;
		for (_Iterator it = begin(); it != end(); ++it)
		{
			if (!(*it)->isInitialised())
			{
				(*it)->Init();
				(*it)->setInitialised(true);
				if (!(*it)->Execute(dt))
				{
					num_finished++;
				}
			}
			else
			{
				if (!(*it)->Execute(dt))
				{
					num_finished++;
				}
			}
		}

		if (num_finished == (int)Commands.size())
			return false;
	}
	else
	{
		// Process current command in this command executor
		CzCommand* command = (CzCommand*)Commands[CurrentCommand];
		if (command != NULL)
		{
			if (!command->isInitialised())
			{
				command->Init();
				command->setInitialised(true);
				if (!command->Execute(dt))
				{
					nextCommand();
				}
			}
			else
			{
				if (!command->Execute(dt))
				{
					nextCommand();
				}
			}
		}
	}

	return true;
}


// 
// 
//
//
// CzCommand implementation
//
//
//
//
CzString& CzCommand::getParameter1(IzXomlResource* parent)
{
	if (Variables[0] == NULL)
		return Params[0];

	CzXomlVariable* var = CzXomlVariable::GetVariable(Params[0], parent);
	Variables[0] = var;
	if (var != NULL)
	{
		return var->getValue();
	}

	return Params[0];
}
CzString& CzCommand::getParameter2(IzXomlResource* parent)
{
	if (Variables[1] == NULL)
		return Params[1];

	CzXomlVariable* var = CzXomlVariable::GetVariable(Params[1], parent);
	Variables[1] = var;
	if (var != NULL)
		return var->getValue();

	return Params[1];
}
CzString& CzCommand::getParameter3(IzXomlResource* parent)
{
	if (Variables[2] == NULL)
		return Params[2];

	CzXomlVariable* var = CzXomlVariable::GetVariable(Params[2], parent);
	Variables[2] = var;
	if (var != NULL)
		return var->getValue();

	return Params[2];
}
CzString& CzCommand::getParameter4(IzXomlResource* parent)
{
	if (Variables[3] == NULL)
		return Params[3];

	CzXomlVariable* var = CzXomlVariable::GetVariable(Params[3], parent);
	Variables[3] = var;
	if (var != NULL)
		return var->getValue();

	return Params[3];
}
CzString& CzCommand::getParameter5(IzXomlResource* parent)
{
	if (Variables[4] == NULL)
		return Params[4];

	CzXomlVariable* var = CzXomlVariable::GetVariable(Params[4], parent);
	Variables[4] = var;
	if (var != NULL)
		return var->getValue();

	return Params[4];
}



// 
// 
//
//
// CzProgram implementation
//
//
//
//

void CzProgram::restart()
{
	IzCommandExecutor::restart();

	LastCommand = NULL;
	Status = PS_Running;
}

void CzProgram::gotoNextCommand()
{
	bool ret = false;

	// Next command could have been called from anywhere so we find out where based on last called command
	if (LastCommand == NULL)
	{
		IzCommandExecutor::nextCommand();
	}
	else
	{
		if (LastCommand->getParent() != NULL && LastCommand->getParent() != LastCommand->getProgram())
			LastCommand->getParent()->nextCommand();
		else
			nextCommand();
	}
	if (CurrentCommand >= (int)Commands.size())
	{
		stop();
	}
}

void CzProgram::call(CzProgram* program)
{
	program->setCaller(this);
	program->restart();
	getManager()->setPriorityProgram(program);
	pause();
}

void CzProgram::returnToCaller()
{
	if (Caller != NULL)
	{
		Caller->start();
		getManager()->setPriorityProgram(Caller);
		stop();
	}
}

bool CzProgram::Execute(float dt)
{
	if (Status == PS_Running)
		return IzCommandExecutor::Execute(dt);

	return false;
}

bool CzProgram::ProcessCommands(CzCommand* parent, bool load_children, CzXmlNode* node)
{
	for (CzXmlNode::_Iterator it = node->begin(); it != node->end(); ++it)
	{
		unsigned int name_hash = (*it)->GetName().getHash();
		if (name_hash == CzHashes::Command_Hash)
		{
			CzString	name, method, ifreturn, condition;
			CzString	params[5];
			bool		active = true;
			bool		parallel = false;
			int			value = 0;

			for (CzXmlNode::_AttribIterator it2 = (*it)->attribs_begin(); it2 != (*it)->attribs_end(); ++it2)
			{
				unsigned int attrib_hash = (*it2)->getName().getHash();

				if (attrib_hash == CzHashes::Name_Hash)
					name = (*it2)->getValue();
				else
				if (attrib_hash == CzHashes::Method_Hash)
					method = (*it2)->getValue();
				else
				if (attrib_hash == CzHashes::Param1_Hash || attrib_hash == CzHashes::P1_Hash)
					params[0] = (*it2)->getValue();
				else
				if (attrib_hash == CzHashes::Param2_Hash || attrib_hash == CzHashes::P2_Hash)
					params[1] = (*it2)->getValue();
				else
				if (attrib_hash == CzHashes::Param3_Hash || attrib_hash == CzHashes::P3_Hash)
					params[2] = (*it2)->getValue();
				else
				if (attrib_hash == CzHashes::Param4_Hash || attrib_hash == CzHashes::P4_Hash)
					params[3] = (*it2)->getValue();
				else
				if (attrib_hash == CzHashes::Param5_Hash || attrib_hash == CzHashes::P5_Hash)
					params[4] = (*it2)->getValue();
				else
				if (attrib_hash == CzHashes::Parallel_Hash)
					parallel = (*it2)->getValueAsBool();
				else
				if (attrib_hash == CzHashes::IfReturn_Hash)
					ifreturn = (*it2)->getValue();
				else
				if (attrib_hash == CzHashes::Value_Hash)
					value = (*it2)->getValueAsInt();
				else
				if (attrib_hash == CzHashes::Condition_Hash)
					condition = (*it2)->getValue();
			}

			// Find the commands creator
			CzCommand* command = NULL;
			if (method.isEmpty())
				method="nop";

			CzCommandCreator* creator = CZ_PROGRAMS->findCreator(method.getHash());
			if (creator != NULL)
			{
				// Create the command
				command = creator->CreateInstance();
				if (command != NULL)
				{
					command->setProgram(this);
					command->setParallel(parallel);
					command->setCommandName(name.c_str());
					if (!ifreturn.isEmpty())
					{
						IzCommandExecutor* ifcommand = findCommand(ifreturn.getHash());
						if (ifcommand == NULL)
							CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Program - IfReturn command could not be found - ", ifreturn.c_str(), (*it)->getParserFileName(), (*it)->Line);
						else
							command->setIfReturn(ifcommand, value);
					}

					for (int t = 0; t < 5; t++)
						command->setParameter(t, params[t]);

					// If no parent command then add to the program else add to the parent commands child list
					if (parent == NULL)
						addCommand(command);
					else
						parent->addCommand(command);

					if (!condition.isEmpty())
					{
						if (*(condition.c_str()) == '!')
							command->setConditions(condition.c_str() + 1, true);
						else
							command->setConditions(condition.c_str());
					}

				}
			}
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Program - No creator for command method could be found - ", method.c_str(), (*it)->getParserFileName(), (*it)->Line);

			// If command contains sub commands then load them also
			if ((*it)->Children.size() != 0)
			{
				ProcessCommands(command, true, *it);
			}
		}
	}

	return true;
}

int CzProgram::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	CzScene* scene = NULL;
	if (parent->getClassTypeHash() == CzHashes::Actor_Hash)
	{
		CzActor* actor = (CzActor*)parent;
		scene = actor->getScene();
	}
	else
	if (parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

	bool auto_run = false;
	bool priority = false;

	// Process program attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
			setName((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::AutoRun_Hash)
			auto_run = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Priority_Hash)
			priority = (*it)->getValueAsBool();
	}

	// Prrocess commands
	ProcessCommands(NULL, true, node);

/*	// If we are declared inside a scene then program is local to the scene
	if (scene != NULL)
		scene->getResourceManager()->addResource(this);
	else
		CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this);*/

	// Add the program to a program manager
	if (scene != NULL)
		scene->getProgramManager()->addProgram(this);
	else
		CZ_GLOBAL_RESOURCES->getProgramManager()->addProgram(this);

	// if auto_run then start the program running
	if (auto_run)
		start();

	// If set as priority program then inform manager
	if (priority)
		Manager->setPriorityProgram(this);

	return 1;
}

//
// 
// 
//
// CzProgramManager Implementation
// 
// 
// 
//
void CzProgramManager::addProgram(CzProgram* program)
{
#if defined(CZ_REMOVE_DUPLICATES)
	CzProgram* prog = findProgram(program->getNameHash());
	if (prog != NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Program - Already exists, program was replaced - ", program->getName().c_str());
		removeProgram(program);
	}
#endif

	Programs.push_back(program);
	program->setManager(this);
}

bool CzProgramManager::removeProgram(CzProgram* Program)
{
	for (_Iterator it = Programs.begin(); it != Programs.end(); ++it)
	{
		if (*it == Program)
		{
			delete *it;
			Programs.erase(it);
			if (*it == PriorityProgram)
				PriorityProgram = NULL;
			return true;
		}
	}

	return false;
}

CzProgram* CzProgramManager::findProgram(unsigned int name_hash)
{
	for (_Iterator it = Programs.begin(); it != Programs.end(); ++it)
	{
		if ((*it)->getNameHash() == name_hash)
			return *it;
	}

	return NULL;
}

CzProgram* CzProgramManager::findProgram(const char* name)
{
	return findProgram(CzString::CalculateHash(name));
}

void CzProgramManager::clearPrograms()
{
	for (_Iterator it = Programs.begin(); it != Programs.end(); ++it)
		delete *it;
	Programs.clear();
	PriorityProgram = NULL;
}
void CzProgramManager::stopAll()
{
	for (_Iterator it = Programs.begin(); it != Programs.end(); ++it)
		(*it)->stop();
}

void CzProgramManager::startAll()
{
	for (_Iterator it = Programs.begin(); it != Programs.end(); ++it)
		(*it)->start();
}

void CzProgramManager::pauseAll()
{
	for (_Iterator it = Programs.begin(); it != Programs.end(); ++it)
		(*it)->pause();
}

void CzProgramManager::Execute(float dt)
{
	// Execute priority program first
	if (PriorityProgram != NULL)
		PriorityProgram->Execute(dt);

	// Execute other programs
	for (_Iterator it = Programs.begin(); it != Programs.end(); ++it)
	{
		if (*it != PriorityProgram)
			(*it)->Execute(dt);
	}
}

CzProgram* CzProgramManager::FindProgram(const char* name, IzXomlResource* container)
{
	return FindProgram(CzString::CalculateHash(name), container);
}

CzProgram* CzProgramManager::FindProgram(unsigned int name_hash, IzXomlResource* container)
{
	CzProgram* program = NULL;
	
	// If container class is an actor then check the actors scenes program manager
	if (container != NULL && container->getClassTypeHash() == CzHashes::Actor_Hash)
		container = ((CzActor*)container)->getScene();
	// If container class is a scene then check the scenes program manager
	if (container != NULL && container->getClassTypeHash() == CzHashes::Scene_Hash)
		program = (CzProgram*)((CzScene*)container)->getProgramManager()->findProgram((name_hash));

	// If the program has no container or the program was not found in the supplied container then check the global programs manager
	if (program == NULL)
		program = (CzProgram*)CZ_GLOBAL_RESOURCES->getProgramManager()->findProgram((name_hash));

	return program;
}

//
// 
// 
//
// CzPrograms Implementation
// 
// 
// 
//
CDECLARE_SINGLETON(CzPrograms)

CzCommandCreator* CzPrograms::findCreator(unsigned int name_hash)
{
	for (_Iterator it = CommandCreators.begin(); it != CommandCreators.end(); ++it)
	{
		if ((*it)->getCommandNameHash() == name_hash)
			return *it;
	}

	return NULL;
}

CzCommandCreator* CzPrograms::findCreator(const char* name)
{
	return findCreator(CzString::CalculateHash(name));
}

void CzPrograms::Init()
{
	// Add default program commands
	CZ_PROGRAMS->addCommand(new CzCommandNopCreator());
	CZ_PROGRAMS->addCommand(new CzCommandCallCreator());
	CZ_PROGRAMS->addCommand(new CzCommandGotoCreator());
	CZ_PROGRAMS->addCommand(new CzCommandStopCreator());
	CZ_PROGRAMS->addCommand(new CzCommandReturnCreator());
	CZ_PROGRAMS->addCommand(new CzCommandPriorityCreator());
	CZ_PROGRAMS->addCommand(new CzCommandRunActionsCreator());
	CZ_PROGRAMS->addCommand(new CzCommandSetPropertyCreator());
	CZ_PROGRAMS->addCommand(new CzCommandSetUserPropCreator());
	CZ_PROGRAMS->addCommand(new CzCommandSetVariableCreator());
	CZ_PROGRAMS->addCommand(new CzCommandGetVariableCreator());
	CZ_PROGRAMS->addCommand(new CzCommandAddVariableCreator());
	CZ_PROGRAMS->addCommand(new CzCommandIfVarCreator());
	CZ_PROGRAMS->addCommand(new CzCommandWaitVarIsValueCreator());
	CZ_PROGRAMS->addCommand(new CzCommandCallScriptCreator());
	CZ_PROGRAMS->addCommand(new CzCommandInlineCreator());
	CZ_PROGRAMS->addCommand(new CzCommandChangeProgramCreator());
	CZ_PROGRAMS->addCommand(new CzCommandFromTemplateCreator());
	CZ_PROGRAMS->addCommand(new CzCommandLoadXOMLCreator());
	CZ_PROGRAMS->addCommand(new CzCommandMusicCreator());
	CZ_PROGRAMS->addCommand(new CzCommandSoundCreator());
	CZ_PROGRAMS->addCommand(new CzCommandWaitCreator());
	CZ_PROGRAMS->addCommand(new CzCommandCallGlobalScriptCreator());
	CZ_PROGRAMS->addCommand(new CzCommandDebugTextCreator());
	CZ_PROGRAMS->addCommand(new CzCommandRemoteReqCreator());
}

void CzPrograms::Release()
{
	// Clean up command creators
	for (_Iterator it = begin(); it != end(); ++it)
	{
		delete *it;
	}
	CommandCreators.clear();
}

