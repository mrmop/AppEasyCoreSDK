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

#include "CzActionsProgram.h"
#include "CzHashes.h"
#include "CzApp.h"
#include "CzScene.h"
#include "CzActor.h"

/**
 @fn	void CzXomlActions_Program::Execute(IzXomlResource* source, CzAction* action)

 @brief	The main actions executor for program related actions. All program specific actions such as ChangeProgram are implemented by this method

 @param [in]	source - The source of the action
 @param [in]	action - The action to execute
 */
void CzXomlActions_Program::Execute(IzXomlResource* source, CzAction* action)
{
	if (source == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Source was not found for action - ", action->_MethodName.c_str(), action->Parent->getDebugInfo().c_str());
#endif
		return;
	}

	// Determine the container
	CzApp* game;
	CzScene* scene;
	CzActor* actor;
	IzXomlResource* cont = IzXomlAction::getContainer(source, game, scene, actor);

	switch (Type)
	{
	case Action_ChangeProgram:		// Changes a running program
		{
			if (!action->Params[2].isEmpty())
				scene = game->findScene(action->getParameter3(cont).c_str());

			CzProgram* program = NULL;
			if (scene != NULL)
			{
				if (!action->Params[0].isEmpty())
				{
					program = scene->getProgramManager()->findProgram(action->getParameter1(cont).c_str());
					if (program == NULL)
						program = CZ_GLOBAL_RESOURCES->getProgramManager()->findProgram(action->getParameter1(cont).c_str());
				}
				else
				{
					// If no program specified then target priority program
					program = scene->getProgramManager()->getPriorityProgram();
				}
			}

			if (program != NULL)
			{
				unsigned int command_hash = action->getParameter2(cont).getHash();
				if (command_hash == CzHashes::Start_Hash)
					program->start();
				else
				if (command_hash == CzHashes::Restart_Hash)
					program->restart();
				else
				if (command_hash == CzHashes::Stop_Hash)
					program->stop();
				else
				if (command_hash == CzHashes::Pause_Hash)
					program->pause();
				else
				if (command_hash == CzHashes::Next_Hash)
					program->gotoNextCommand();
				else
				if (command_hash == CzHashes::Priority_Hash)
					program->getManager()->setPriorityProgram(program);
				else
				if (command_hash == CzHashes::Goto_Hash)
				{
					IzCommandExecutor* command = program->findCommand(action->getParameter4(cont).getHash());
					if (command != NULL)
						command->gotoCommand();
					else
						CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - ChangeProgram - Goto command that does not exist - ", action->getParameter4(cont).c_str(), action->Parent->getDebugInfo().c_str());
				}
			}
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - ChangeProgram - No valid program found - ", action->getParameter1(cont).c_str(), action->Parent->getDebugInfo().c_str());
		}
		break;
	}
}
