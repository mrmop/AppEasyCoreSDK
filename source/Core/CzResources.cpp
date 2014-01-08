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

#include "CzResources.h"
#include "CzUtil.h"
#include "CzString.h"
#include "CzAnim.h"
#include "CzImage.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzProgram.h"
#include "CzAnim.h"
#include "CzActions.h"
#include "CzScriptLua.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"


/*//
//
//
//
// CzResourceGroup implementation
//
//
//
//
CIwResGroup* CzResourceGroup::getResourceGroup()
{
	// If group has not been oaded then load it
	if (!Loaded)
		Load();

	return ResourceGroup;
}

bool CzResourceGroup::Load()
{
	if (Loaded)
		return true;

	ResourceGroup = IwGetResManager()->LoadGroup(GroupFilename.c_str());
	if (ResourceGroup != NULL)
	{
		Loaded = true;
		return true;
	}

	return false;
}

void CzResourceGroup::Destroy()
{
	if (Loaded)
	{
		IwGetResManager()->DestroyGroup(ResourceGroup);
		ResourceGroup = NULL;
		Loaded = false;
	}
}

int CzResourceGroup::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Process attributes
	bool preload = false;
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
		{
			setName((*it)->getValue().c_str());
		}
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::GroupFile_Hash)
		{
			setGroupFilename((*it)->getValue().c_str());
		}
		else
		if (name_hash == CzHashes::Preload_Hash)
		{
			preload = (*it)->getValueAsBool();
		}
	}

	// Process inner tags
	if (!CZ_XOML->Process(this, node))
		return 0;

	// If we are declared inside a scene then animation is local to the scene
	CzScene* scene = NULL;
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Actor_Hash)
		scene = ((CzActor*)parent)->getScene();
	else
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

	bool ret = false;
	if (scene != NULL)
		ret = scene->getResourceManager()->addResource(this);
	else
		ret = CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this);
	if (!ret)
		return 0;

	// Preload the resorce group if requested
	if (preload)
		Load();

	return 1;
}

//
//
//
//
// CzResourceGroupCreator implementation
//
//
//
//
IzXomlResource* CzResourceGroupCreator::CreateInstance(IzXomlResource* parent)
{
	return new CzResourceGroup();
}*/



//
//
//
//
// CzGlobalResources implementation
//
//
//
//

CDECLARE_SINGLETON(CzGlobalResources)

void CzGlobalResources::Init(IzXomlResource* parent)
{
	ResourceManager = new CzXomlResourceManager();
	ResourceManager->setParent(parent);
	VariableManager = new CzXomlVariableManager();
	VariableManager->setParent(parent);
	ActionsManager = new CzActionsManager();
	ActionsManager->setParent(parent);
	TimelinesManager = new CzAnimTimelinesManager();
	TimelinesManager->setParent(parent);
	TimersManager = new CzTimersManager();
	TimersManager->setParent(parent);
	ProgramManager = new CzProgramManager();
	ProgramManager->setParent(parent);
	ScriptEngine = NULL;
	getScriptEngine(CzScript::ST_LUA);		// TODO: REMOVE eventually - For some reason if we create the global script engine when the script has been loaded froma file the Lua GC throws a wobbler! For now we just create it on init instead
}

void CzGlobalResources::Release()
{
	SAFE_DELETE(ScriptEngine)
	SAFE_DELETE(VariableManager)
	SAFE_DELETE(ActionsManager)
	SAFE_DELETE(TimersManager)
	SAFE_DELETE(TimelinesManager)
	SAFE_DELETE(ResourceManager)
	SAFE_DELETE(ProgramManager)
}

void CzGlobalResources::Update(float dt)
{
	VariableManager->Update();
	TimersManager->Update(dt);
}

IzScriptEngine* CzGlobalResources::getScriptEngine(CzScript::eScriptType type)
{
	if (ScriptEngine != NULL)
		return ScriptEngine;

	switch (type)
	{
	case CzScript::ST_LUA:
		ScriptEngine = new CzScriptEngineLua();
		ScriptEngine->Init(NULL);
		break;
	case CzScript::ST_AngelScript:
		break;
	case CzScript::ST_Javascript:
		break;
	case CzScript::ST_Python:
		break;
	}

	return ScriptEngine;
}
