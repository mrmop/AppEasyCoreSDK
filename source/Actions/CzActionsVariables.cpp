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

#include "CzActionsVariables.h"
#include "CzApp.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzMath.h"
#include "CzHashes.h"


/**
 @fn	void CzXomlActions_Variable::Execute(IzXomlResource* source, CzAction* action)

 @brief	The main actions executor for variable related actions. All variable specific actions such as SetVariable are implemented by this method

 @param [in]	source - The source of the action
 @param [in]	action - The action to execute
 */
void CzXomlActions_Variable::Execute(IzXomlResource* source, CzAction* action)
{
	if (source == NULL)
	{
#if defined (_DEBUG)
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
	case Action_SetVariable:
		{
			if (!action->Params[2].isEmpty())
			{
				CzScene* new_scene = game->findScene(action->getParameter3(cont).c_str());
				if (new_scene != NULL)
					scene = new_scene;
#if defined (_DEBUG)
				else
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - SetVariable - Invalid scene - ", action->Params[2].c_str(), action->Parent->getDebugInfo().c_str());
#endif
			}

			// Set the variables value
			CzXomlVariable* var = CzXomlVariable::GetVariable(action->Params[0], scene);
			if (var != NULL)
			{
				CzString& p2 = action->getParameter2(cont);
				unsigned int hash = p2.getHash();
				if (hash == CzHashes::Rand_Hash || hash == CzHashes::RandChar_Hash)
				{
					CzString rnd;
					rnd.setAutoHash(false);
					CzString& p4 = action->getParameter4(cont);
					CzString& p5 = action->getParameter5(cont);
					float min, max;

					if (hash == CzHashes::Rand_Hash)
					{
						min = p4.getAsFloat();
						max = p5.getAsFloat();
					}
					else
					{
						min = (float)p4[0];
						max = (float)p5[0];
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
					var->setValue(p2.c_str());
			}
#if defined (_DEBUG)
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - SetVariable - Invalid target variable - ", action->Params[0].c_str(), action->Parent->getDebugInfo().c_str());
#endif
		}
		break;
	case Action_AddVariable:
		{
			if (!action->Params[3].isEmpty())
			{
				CzScene* new_scene = game->findScene(action->getParameter4(cont).c_str());
				if (new_scene != NULL)
					scene = new_scene;
#if defined (_DEBUG)
				else
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - AddVariable - Invalid scene - ", action->getParameter4(cont).c_str(), action->Parent->getDebugInfo().c_str());
#endif
			}

			// Set the variables value
			CzXomlVariable* var = CzXomlVariable::GetVariable(action->Params[0], scene);
			if (var != NULL)
			{
				if (action->Params[2].isEmpty())
					var->addValue(action->getParameter2(cont).c_str());
				else
					var->addValue(action->getParameter2(cont).c_str(), action->getParameter3(cont).c_str());
			}
#if defined (_DEBUG)
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Action - AddVariable - Invalid target variable - ", action->Params[0].c_str(), action->Parent->getDebugInfo().c_str());
#endif
		}
		break;
	}
}
