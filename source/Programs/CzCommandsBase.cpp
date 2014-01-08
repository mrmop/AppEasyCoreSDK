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

#include "CzCommandsBase.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzApp.h"
#include "CzScript.h"
#include "CzRemoteReq.h"
#include "CzMath.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"
#include "CzXomlLoad.h"

//
// 
// 
//
// CzCommandNop Implementation
// 
// 
// 
//
void CzCommandNop::Init()
{
/*	CzApp* game = NULL;
	CzScene* scene = NULL;
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)Program->getManager()->getParent();
	else
	if (class_hash == CzHashes::Scene_Hash)
		scene = (CzScene*)Program->getManager()->getParent();*/

}

bool CzCommandNop::Execute(float dt)
{
	if (IfReturn == NULL && Parallel == false)
		return false;
	else
		return IzCommandExecutor::Execute(dt);
}

//
// 
// 
//
// CzCommandChangeProgram Implementation
// 
// 
// 
//
bool CzCommandChangeProgram::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	CzProgram* program = NULL;
	CzApp* game = NULL;
	CzScene* scene = NULL;

	if (Params[1].isEmpty())
		program = Program;
	else
	{
		unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
		if (class_hash == CzHashes::Scene_Hash)
		{
			scene = (CzScene*)Program->getManager()->getParent();
			program = scene->getProgramManager()->findProgram(getParameter2(scene).getHash());
		}
		if (program == NULL)
		{
			program = CZ_GLOBAL_PROGRAM_MANAGER->findProgram(getParameter2(scene).getHash());
			if (program == NULL)
			{
#if defined(_DEBUG)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command pause - Cannot find named program in scene or global resources - ", Params[1].c_str(), Program->getDebugInfo().c_str());
#endif	// _DEBUG
				return false;
			}
		}
	}
	unsigned int command_hash = getParameter1(scene).getHash();
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

	return false;
}

//
// 
// 
//
// CzCommandGoto Implementation
// 
// 
// 
//
bool CzCommandGoto::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	CzApp* game = NULL;
	CzScene* scene = NULL;
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)Program->getManager()->getParent();
	else
	if (class_hash == CzHashes::Scene_Hash)
		scene = (CzScene*)Program->getManager()->getParent();

	CzCommand* command = (CzCommand*)Program->findCommand(getParameter1(scene).getHash());
	if (command != NULL)
		command->gotoCommand();
	else
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command goto - Cannot find named command - ", Params[0].c_str(), Program->getDebugInfo().c_str());
#endif	// _DEBUG
	}
	return true;
}

//
// 
// 
//
// CzCommandPriority Implementation
// 
// 
// 
//
bool CzCommandPriority::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	CzApp* game = NULL;
	CzScene* scene = NULL;
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)Program->getManager()->getParent();
	else
	if (class_hash == CzHashes::Scene_Hash)
		scene = (CzScene*)Program->getManager()->getParent();

	// Find program
	CzProgram* p = NULL;
		
	if (Params[0].isEmpty())	// Passing no program will make program that ran thsi command the priority program
		p = Program;
	else
		p = Program->getManager()->findProgram(getParameter1(scene).getHash());
	if (p != NULL)
		Program->getManager()->setPriorityProgram(p);
#if defined (_DEBUG)
	else
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Command priority - could not find named program - ", getParameter1(scene).c_str(), Program->getDebugInfo().c_str());
#endif // (_DEBUG)

	return false;
}

//
// 
// 
//
// CzCommandCall Implementation
// 
// 
// 
//
bool CzCommandCall::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	CzApp* game = NULL;
	CzScene* scene = NULL;
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)Program->getManager()->getParent();
	else
	if (class_hash == CzHashes::Scene_Hash)
		scene = (CzScene*)Program->getManager()->getParent();

	// Find program
	CzProgram* p = Program->getManager()->findProgram(getParameter1(scene).getHash());
	if (p != NULL)
		Program->call(p);
#if defined (_DEBUG)
	else
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "Command call - could not find named program - ", getParameter1(scene).c_str(), Program->getDebugInfo().c_str());
#endif // (_DEBUG)

	return false;
}

//
// 
// 
//
// CzCommandRunActions Implementation
// 
// 
// 
//
bool CzCommandRunActions::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	Finished = false;

	CzApp* game = NULL;
	CzScene* scene = NULL;
	CzActor* actor = NULL;
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)Program->getManager()->getParent();
	else
	if (class_hash == CzHashes::Scene_Hash)
		scene = (CzScene*)Program->getManager()->getParent();

	if (game != NULL)
		scene = game->findScene(Params[2].getHash());

	IzXomlResource* cont = (actor != NULL) ? (IzXomlResource*)actor : (IzXomlResource*)scene;
	CzActions* actions = CzActionsManager::FindActions(getParameter1(cont).getHash(), scene);
	if (actions != NULL)
	{
		CzActor* actor = scene->findActor(getParameter2(cont).getHash());
		if (actor == NULL && scene == NULL)
		{
#if defined(_DEBUG)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command run_actions - No valid target actor or scene could be found", Program->getDebugInfo().c_str());
#endif	// _DEBUG
			return false;
		}
		if (actor == NULL)
			actions->Execute(scene);
		else
			actions->Execute(actor);
	}
#if defined(_DEBUG)
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command run_actions - Actions could not be found - ", getParameter1(cont).c_str(), Program->getDebugInfo().c_str());
	}
#endif	// _DEBUG

	return false;
}

//
// 
// 
//
// CzCommandSetProperty Implementation
// 
// 
// 
//
bool CzCommandSetProperty::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	CzApp* game = NULL;
	CzScene* scene = NULL;
	CzActor* actor = NULL;
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)Program->getManager()->getParent();
	else
	if (class_hash == CzHashes::Scene_Hash)
		scene = (CzScene*)Program->getManager()->getParent();

	IzXomlResource* cont = (actor != NULL) ? (IzXomlResource*)actor : (IzXomlResource*)scene;
	if (Params[2].isEmpty() && !Params[3].isEmpty())
	{
		// User supplied a specific scene and no actor, so we set the scenes property instead
		if (game != NULL)
			scene = game->findScene(getParameter4(cont).getHash());
		else
			scene = scene->getParent()->findScene(getParameter4(cont).getHash());
		if (scene == NULL)
		{
#if defined(_DEBUG)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command set_property - Scene does not exist - ", getParameter4(cont).c_str(), Program->getDebugInfo().c_str());
#endif	// _DEBUG
		}
		else
			scene->setProperty(getParameter1(cont).getHash(), getParameter2(cont), false);
		return false;
	}

	// Check to see if we supplied a scene that the actor lives in
	if (game != NULL && !Params[3].isEmpty())
		scene = game->findScene(getParameter4(cont).getHash());

	if (scene != NULL)
	{
		CzActor* actor = scene->findActor(getParameter3(cont).getHash());
		if (actor != NULL)
			actor->setProperty(getParameter1(cont).getHash(), getParameter2(cont), false);
#if defined(_DEBUG)
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command set_property - Actor does not exist - ", getParameter3(cont).c_str(), Program->getDebugInfo().c_str());
#endif	// _DEBUG
	}
	else
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command set_property - Scene does not exist - ", getParameter4(cont).c_str(), Program->getDebugInfo().c_str());
#endif	// _DEBUG
	}

	return false;
}

//
// 
// 
//
// CzCommandSetUserProp Implementation
// 
// 
// 
//
bool CzCommandSetUserProp::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	CzApp* game = NULL;
	CzScene* scene = NULL;
	CzActor* actor = NULL;
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)Program->getManager()->getParent();
	else
	if (class_hash == CzHashes::Scene_Hash)
		scene = (CzScene*)Program->getManager()->getParent();

	IzXomlResource* cont = (actor != NULL) ? (IzXomlResource*)actor : (IzXomlResource*)scene;
	if (Params[2].isEmpty() && !Params[3].isEmpty())
	{
		// User supplied a specific scene and no actor, so we set the scenes property instead
		if (game != NULL)
			scene = game->findScene(getParameter4(cont).getHash());
		else
			scene = scene->getParent()->findScene(getParameter4(cont).getHash());
		if (scene == NULL)
		{
#if defined(_DEBUG)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command set_userprop - Scene does not exist - ", getParameter4(cont).c_str(), Program->getDebugInfo().c_str());
#endif	// _DEBUG
		}
		else
		{
			CzUserPropertyList* props = scene->getUserPropertyList();
			if (props != NULL)
				props->setProperty(getParameter1(cont).getHash(), getParameter2(cont), false);
		}
		return false;
	}

	// Check to see if we supplied a scene that the actor lives in
	if (game != NULL && !Params[3].isEmpty())
		scene = game->findScene(getParameter4(cont).getHash());

	if (scene != NULL)
	{
		CzActor* actor = scene->findActor(getParameter3(cont).getHash());
		if (actor != NULL)
		{
			CzUserPropertyList* props = actor->getUserPropertyList();
			if (props != NULL)
				actor->setProperty(getParameter1(cont).getHash(), getParameter2(cont), false);
		}
#if defined(_DEBUG)
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command set_userprop - Actor does not exist - ", getParameter3(cont).c_str(), Program->getDebugInfo().c_str());
#endif	// _DEBUG
	}
	else
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command set_userprop - Scene does not exist - ", getParameter4(cont).c_str(), Program->getDebugInfo().c_str());
#endif	// _DEBUG
	}

	return false;
}


//
// 
// 
//
// CzCommandSetVariable Implementation
// 
// 
// 
//
bool CzCommandSetVariable::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	CzApp* game = NULL;
	CzScene* scene = NULL;
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)Program->getManager()->getParent();
	else
	if (class_hash == CzHashes::Scene_Hash)
		scene = (CzScene*)Program->getManager()->getParent();

	if (game != NULL)
		scene = game->findScene(Params[2].getHash());

	IzXomlResource* cont = (IzXomlResource*)scene;
	// Set the variables value
	CzXomlVariable* var = CzXomlVariable::GetVariable(Params[0], scene);
	if (var != NULL)
	{
		unsigned int hash = getParameter2(cont).getHash();
		if (hash == CzHashes::Rand_Hash || hash == CzHashes::RandChar_Hash)
		{
			CzString rnd;
			rnd.setAutoHash(false);
			float min, max;

			if (hash == CzHashes::Rand_Hash)
			{
				min = getParameter4(cont).getAsFloat();
				max = getParameter5(cont).getAsFloat();
			}
			else
			{
				min = (float)getParameter4(cont)[0];
				max = (float)getParameter5(cont)[0];
			}

			if (var->isArray())
			{
				CzXomlVariableArray* arr = (CzXomlVariableArray*)var;
				for (int t = 0; t < arr->getSize(); t++)
				{
					CzXomlVariable* var2 = arr->getElement(t);
					float num = min + CzMath::Rand(max - min);

					if (hash == CzHashes::Rand_Hash)
					{
						if (var2->Type == VT_Bool)
							rnd = (bool)(num > 0.5f);
						else
						if (var2->Type == VT_Int)
							rnd = (int)num;
						else
							rnd = num;
					}
					else
						rnd = (char)num;
					var2->setValue(rnd.c_str());
				}
			}
			else
			{
				float num = min + CzMath::Rand(max - min);
				if (hash == CzHashes::Rand_Hash)
				{
					if (var->Type == VT_Bool)
						rnd = (bool)(num > 0.5f);
					else
					if (var->Type == VT_Int)
						rnd = (int)num;
					else
						rnd = num;
				}
				else
					rnd = (char)num;
				var->setValue(rnd.c_str());
			}
		}
		else
			var->setValue(getParameter2(cont).c_str());
	}

	return false;
}

//
// 
// 
//
// CzCommandGetVariable Implementation
// 
// 
// 
//
bool CzCommandGetVariable::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	CzApp* game = NULL;
	CzScene* scene = NULL;
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)Program->getManager()->getParent();
	else
	if (class_hash == CzHashes::Scene_Hash)
		scene = (CzScene*)Program->getManager()->getParent();

	if (game != NULL)
		scene = game->findScene(Params[2].getHash());

	// Get the variables value
	CzXomlVariable* var = CzXomlVariable::GetVariable(Params[0], scene);
	if (var != NULL)
		ReturnValue = var->getValue().getAsInt();

	return false;
}

//
// 
// 
//
// CzCommandAddVariable Implementation
// 
// 
// 
//
bool CzCommandAddVariable::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	Finished = false;

	CzApp* game = NULL;
	CzScene* scene = NULL;
	CzActor* actor = NULL;
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)Program->getManager()->getParent();
	else
	if (class_hash == CzHashes::Scene_Hash)
		scene = (CzScene*)Program->getManager()->getParent();

	if (game != NULL)
		scene = game->findScene(Params[2].getHash());

	IzXomlResource* cont = (actor != NULL) ? (IzXomlResource*)actor : (IzXomlResource*)scene;
	// Set the variables value
	CzXomlVariable* var = CzXomlVariable::GetVariable(Params[0], scene);
	if (var != NULL)
		var->addValue(getParameter2(cont).c_str());

	return false;
}

//
// 
// 
//
// CzCommandIfVar Implementation
// 
// 
// 
//
bool CzCommandIfVar::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	CzApp* game = NULL;
	CzScene* scene = NULL;
	CzActor* actor = NULL;
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)Program->getManager()->getParent();
	else
	if (class_hash == CzHashes::Scene_Hash)
		scene = (CzScene*)Program->getManager()->getParent();

	if (game != NULL)
		scene = game->findScene(Params[3].getHash());

	IzXomlResource* cont = (actor != NULL) ? (IzXomlResource*)actor : (IzXomlResource*)scene;
	CzXomlVariable* var = CzXomlVariable::GetVariable(Params[0], scene);
	if (var != NULL)
	{
		CzString op = getParameter2(cont);
		op.ToLower();
		if (op == "==")
			ReturnValue = (int)var->checkCondition(CO_Equal, getParameter3(cont));
		else
		if (op == "!=")
			ReturnValue = (int)var->checkCondition(CO_NotEqual, getParameter3(cont));
		else
		if (op == "gt")
			ReturnValue = (int)var->checkCondition(CO_Greater, getParameter3(cont));
		else
		if (op == "lt")
			ReturnValue = (int)var->checkCondition(CO_Less, getParameter3(cont));
		else
		if (op == "gte")
			ReturnValue = (int)var->checkCondition(CO_GreaterEqual, getParameter3(cont));
		else
		if (op == "lte")
			ReturnValue = (int)var->checkCondition(CO_LessEqual, getParameter3(cont));
		else
		if (op == "and")
			ReturnValue = (int)var->checkCondition(CO_And, getParameter3(cont));
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command IfVar - operator invalid - ", op.c_str(), Program->getDebugInfo().c_str());
	}
	else
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command IfVar - variable not found - ", Params[0].c_str(), Program->getDebugInfo().c_str());

	return false;
}
//
// 
// 
//
// CzCommandWaitVarIsValue Implementation
// 
// 
// 
//
bool CzCommandWaitVarIsValue::Execute(float dt)
{
//	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "WaitItemsCommand::Update()");
	if (!IzCommandExecutor::Execute(dt))
		return false;

	CzApp* game = NULL;
	CzScene* scene = NULL;
	CzActor* actor = NULL;
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)Program->getManager()->getParent();
	else
	if (class_hash == CzHashes::Scene_Hash)
		scene = (CzScene*)Program->getManager()->getParent();

	if (game != NULL)
		scene = game->findScene(Params[2].getHash());

	IzXomlResource* cont = (actor != NULL) ? (IzXomlResource*)actor : (IzXomlResource*)scene;
	CzXomlVariable* var = CzXomlVariable::GetVariable(Params[0], scene);
	if (var->getValue().getHash() != getParameter2(cont).getHash())
		return true;

	return false;
}

//
// 
// 
//
// CzCommandCallScript Implementation
// 
// 
// 
//
bool CzCommandCallScript::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	CzApp* game = NULL;
	CzScene* scene = NULL;
	IzXomlResource* parent = Program->getManager()->getParent();
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)parent;
	else
	if (class_hash == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

	if (game != NULL && !Params[1].isEmpty())
		scene = game->findScene(Params[1].getHash());

	if (scene->getScriptEngine() == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command call_script - Scene does not contain a script engine", Program->getDebugInfo().c_str());
		return false;
	}

	IzXomlResource* cont = (IzXomlResource*)scene;
	if (scene != NULL)
	{
		CzString *p2 = NULL;
		CzString *p3 = NULL;
		CzString *p4 = NULL;

		if (!Params[2].isEmpty())
			p2 = &getParameter3(cont);
		if (!Params[3].isEmpty())
			p3 = &getParameter4(cont);
		if (!Params[4].isEmpty())
			p4 = &getParameter5(cont);

		if (scene != NULL)
			ReturnValue = scene->getScriptEngine()->CallFunction(scene, &getParameter1(cont), p2, p3, p4);
	}

	return false;
}

//
// 
// 
//
// CzCommandCallGlobalScript Implementation
// 
// 
// 
//
bool CzCommandCallGlobalScript::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	CzApp* game = NULL;
	CzScene* scene = NULL;
	IzXomlResource* parent = Program->getManager()->getParent();
	unsigned int class_hash = parent->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)parent;
	else
	if (class_hash == CzHashes::Scene_Hash)
		game = ((CzScene*)parent)->getParent();

	if (game != NULL && !Params[1].isEmpty())
		scene = game->findScene(Params[1].getHash());

	IzXomlResource* cont = (IzXomlResource*)scene;
	CzString *p2 = NULL;
	CzString *p3 = NULL;
	CzString *p4 = NULL;

	if (!Params[2].isEmpty())
		p2 = &getParameter3(cont);
	if (!Params[3].isEmpty())
		p3 = &getParameter4(cont);
	if (!Params[4].isEmpty())
		p4 = &getParameter5(cont);

	if (scene != NULL)
		ReturnValue = CZ_GLOBAL_RESOURCES->getScriptEngine()->CallFunction(scene, &getParameter1(cont), p2, p3, p4);
	else
	if (game != NULL)
		ReturnValue = CZ_GLOBAL_RESOURCES->getScriptEngine()->CallFunction(game, &getParameter1(cont), p2, p3, p4);

	return false;
}

//
// 
// 
//
// CzCommandInline Implementation
// 
// 
// 
//
bool CzCommandInline::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	CzApp* game = NULL;
	CzScene* scene = NULL;
	IzXomlResource* parent = Program->getManager()->getParent();
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)parent;
	else
	if (class_hash == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

	if (game == NULL)
		game = scene->getParent();

	CzString* p1 = &getParameter1(scene);
	if (!Params[1].isEmpty())	// Check for a specific scene
	{
		CzString* p2 = &getParameter2(scene);
		scene = game->findScene(p2->getHash());
		if (scene == NULL)
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command inline - scene not found - ", p2->c_str(), Program->getDebugInfo().c_str());
			return false;
		}
		if (scene->getScriptEngine() == NULL)
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command inline - Scene does not contain a script engine", Program->getDebugInfo().c_str());
			return false;
		}

		// Run script in scene script space
		scene->getScriptEngine()->LoadScript(p1->c_str(), p1->getLength(), "inline");
	}
	else
	{
		// Run script in global script space
		CZ_GLOBAL_RESOURCES->getScriptEngine()->LoadScript(p1->c_str(), p1->getLength(), "inline");
	}

	return false;
}

//
// 
// 
//
// CzCommandFromTemplate Implementation
// 
// 
// 
//
bool CzCommandFromTemplate::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	CzApp* game = NULL;
	CzScene* scene = NULL;
	CzScene* target_scene = NULL;
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)Program->getManager()->getParent();
	else
	if (class_hash == CzHashes::Scene_Hash)
		scene = (CzScene*)Program->getManager()->getParent();

	IzXomlResource* cont = (IzXomlResource*)scene;
	if (scene != NULL)
	{
		game = scene->getParent();
		if (!Params[2].isEmpty())
		{
			target_scene = game->findScene(getParameter3(cont).getHash());
			if (target_scene == NULL)
			{
#if defined (_DEBUG)
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command from_template - Invalid scene - ", getParameter3(cont).c_str(), Program->getDebugInfo().c_str());
#endif
			}
			else
				CzXomlLoad::CreateInstance(scene, target_scene, &getParameter1(cont), &getParameter2(cont));
		}
		else
			CzXomlLoad::CreateInstance(scene, scene, &getParameter1(cont), &getParameter2(cont));
	}

	return false;
}

//
// 
// 
//
// CzCommandLoadXOML Implementation
// 
// 
// 
//
bool CzCommandLoadXOML::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	CzApp* game = NULL;
	CzScene* scene = NULL;
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)Program->getManager()->getParent();
	else
	if (class_hash == CzHashes::Scene_Hash)
	{
		scene = (CzScene*)Program->getManager()->getParent();
		game = scene->getParent();
	}
	CzScene* target_scene = NULL;

	IzXomlResource* cont = (IzXomlResource*)scene;
	if (!Params[1].isEmpty())
	{
		target_scene = game->findScene(getParameter2(cont).getHash());
		if (target_scene == NULL)
		{
#if defined (_DEBUG)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command load_xoml - Invalid scene - ", getParameter2(cont).c_str(), Program->getDebugInfo().c_str());
			return false;
#endif
		}
		CZ_XOML->Process(target_scene, getParameter1(cont).c_str());
	}
	else
		CZ_XOML->Process(game, getParameter1(cont).c_str());


	return false;
}

//
// 
// 
//
// CzCommandMusic Implementation
// 
// 
// 
//
bool CzCommandMusic::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	if (!Params[0].isEmpty())
	{
		CzApp* game = NULL;
		CzScene* scene = NULL;
		unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
		if (class_hash == CzHashes::Game_Hash)
			game = (CzApp*)Program->getManager()->getParent();
		else
		if (class_hash == CzHashes::Scene_Hash)
			scene = (CzScene*)Program->getManager()->getParent();

		IzXomlResource* cont = (IzXomlResource*)scene;

		unsigned int hash = getParameter1(cont).getHash();
		if (hash == CZ_HASH("play"))
		{
			PLATFORM_AUDIO->PlayMusic(getParameter2(cont).c_str(), getParameter3(cont).getAsInt());
		}
		else
		if (hash == CZ_HASH("stop"))
		{
			PLATFORM_AUDIO->StopMusic();
		}
	}

	return false;
}

//
// 
// 
//
// CzCommandSound Implementation
// 
// 
// 
//
bool CzCommandSound::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	if (!Params[0].isEmpty() && !Params[1].isEmpty() && !Params[2].isEmpty() && !Params[3].isEmpty())
	{
		CzApp* game = NULL;
		CzScene* scene = NULL;
		unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
		if (class_hash == CzHashes::Game_Hash)
			game = (CzApp*)Program->getManager()->getParent();
		else
		if (class_hash == CzHashes::Scene_Hash)
			scene = (CzScene*)Program->getManager()->getParent();

		IzXomlResource* cont = (IzXomlResource*)scene;
		CzSound* sound = (CzSound*)CzXomlResourceManager::FindResource(getParameter1(cont).getHash(), CzHashes::Sound_Hash, scene);
		if (scene != NULL)
			sound->Play(getParameter2(cont).getAsFloat(), getParameter3(cont).getAsFloat(), getParameter4(cont).getAsFloat(), getParameter5(cont).getAsBool());
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command sound - Could not find named sound - ", getParameter1(cont).c_str(), Program->getDebugInfo().c_str());
	}
	else
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command sound - You must specify sound name, volume, pitch and pan - ", Params[0].c_str(), Program->getDebugInfo().c_str());

	return false;
}

//
// 
// 
//
// CzCommandWait Implementation
// 
// 
// 
//
void CzCommandWait::Init()
{
	IzCommandExecutor::Init();

	CzApp* game = NULL;
	CzScene* scene = NULL;
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)Program->getManager()->getParent();
	else
	if (class_hash == CzHashes::Scene_Hash)
		scene = (CzScene*)Program->getManager()->getParent();

	Timer.setDuration((uint64)(getParameter1(scene).getAsFloat() * 1000.0f));
}
bool CzCommandWait::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	if (Timer.HasTimedOut())
	{
		Timer.Stop();
		return false;
	}

	return true;
}

//
// 
// 
//
// CzCommandDebugText Implementation
// 
// 
// 
//
bool CzCommandDebugText::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	CzApp* game = NULL;
	CzScene* scene = NULL;
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)Program->getManager()->getParent();
	else
	if (class_hash == CzHashes::Scene_Hash)
		scene = (CzScene*)Program->getManager()->getParent();

	CzDebug::Log(CZ_DEBUG_CHANNEL_NONE, getParameter1(scene).c_str(), getParameter2(scene).c_str(), Program->getDebugInfo().c_str());

	return false;
}

//
// 
// 
//
// CzCommandRemoteReq Implementation
// 
// 
// 
//
bool CzCommandRemoteReq::Execute(float dt)
{
	if (!IzCommandExecutor::Execute(dt))
		return false;

	CzApp* game = NULL;
	CzScene* scene = NULL;
	unsigned int class_hash = Program->getManager()->getParent()->getClassTypeHash();
	if (class_hash == CzHashes::Game_Hash)
		game = (CzApp*)Program->getManager()->getParent();
	else
	if (class_hash == CzHashes::Scene_Hash)
		scene = (CzScene*)Program->getManager()->getParent();

	CzString* p1 = NULL;
	CzString* p2 = NULL;
	if (Params[0].isEmpty())
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Command remote_req - Requires Remote Request resource name as Param1", Program->getDebugInfo().c_str());
		return false;
	}
	p1 = &getParameter1(scene);
	CzRemoteReq* request = (CzRemoteReq*)scene->getResourceManager()->findResource(p1->c_str(), CzHashes::RemoteReq_Hash);
	if (request == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Command remote_req - Could not find named remote request resource in the scene or in the global resources - ", p1->c_str(), Program->getDebugInfo().c_str());
		return false;
	}
	if (!Params[1].isEmpty())
	{
		p2 = &getParameter2(scene);
		request->setData(p2->c_str());
	}

	request->MakeRequest();

	return false;
}
