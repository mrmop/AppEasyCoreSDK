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

#include "CzScript.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"

//
//
//
//
// CzScript implementation
//
//
//
//

bool CzScript::setScript(const char* script, int script_len)
{
/*	SAFE_DELETE(Script)
	Script = new char [script_len];
	if (Script == NULL)
		return false;
	memcpy(Script, script, script_len);
	ScriptLength = script_len;*/

	return true;
}


int CzScript::Init(const char* script, int script_len, CzScene* scene)
{
//	if (!setScript(script, script_len))
//		return 1;

	if (scene == NULL)
	{
		IzXomlResource* parent = Parent->getParent();
		if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
			scene = (CzScene*)parent;
		else
		{
			if (CZ_GLOBAL_RESOURCES->getScriptEngine(ScriptType) != NULL)
				return CZ_GLOBAL_RESOURCES->getScriptEngine(ScriptType)->LoadScript(script, script_len, Name.c_str());
//				return CZ_GLOBAL_RESOURCES->getScriptEngine(ScriptType)->RunScript(Script, ScriptLength);
		}
	}
	
	// Run the script
	if (scene != NULL && scene->getScriptEngine() != NULL)
		return scene->getScriptEngine()->LoadScript(script, script_len, Name.c_str());
//		return scene->getScriptEngine()->RunScript(Script, ScriptLength);

	return -1;
}

int CzScript::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Process image attributes
	CzString* name = NULL;
	CzString* file = NULL;
	CzString* condition = NULL;

	CzScene* scene = NULL;
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Actor_Hash)
		scene = ((CzActor*)parent)->getScene();
	else
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
		{
			name = &(*it)->getValue();
			setName(name->c_str());
		}
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::File_Hash)
			file = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Type_Hash)
		{
			unsigned int type_hash = (*it)->getValue().getHash();

			if (type_hash == CZ_HASH("lua"))
				setScriptType(ST_LUA);
			else
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Script - Unsupported format - ", (*it)->getValue().c_str(), DebugInfo.c_str());
		}
		else
		if (name_hash == CzHashes::Condition_Hash)
			condition = &(*it)->getValue();
	}

	if (condition != NULL)
	{
		// Find the condition variable
		bool condition_not = false;
		CzXomlVariable* var = NULL;
		if (*(condition->c_str()) == '!')
		{
			condition_not = true;
			CzString cond = condition->c_str() + 1;
			var = CzXomlVariable::GetVariable(cond, scene);
		}
		else
			var = CzXomlVariable::GetVariable(*condition, scene);
		if (var != NULL)
		{
			bool res = var->isTrue();
			if (condition_not)
				res = !res;
			if (!res)
				return -1;
		}
#if defined (_DEBUG)
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "condition variable not found - ", condition->c_str(), DebugInfo.c_str());
#endif // _DEBUG 
	}

	// If we are declared inside a scene then script is local to the scene
	if (scene != NULL)
		return scene->getResourceManager()->addResource(this) ? 1 : 0;
	else
		return CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this) ? 1 : 0;

	return 1;
}




