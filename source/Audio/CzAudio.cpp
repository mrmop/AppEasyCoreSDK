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

#include "IzPlatformAudio.h"
#include "CzAudio.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzHashes.h"
#include "CzFile.h"
#include "CzXomlResourceManager.h"

//
//
//
//
// CzSound implementation
//
//
//
//

CzSound::~CzSound()
{
//	if (ResourceGroup == NULL)
	{
		PLATFORM_AUDIO->DeleteSoundSpec(SoundSpec);
		PLATFORM_AUDIO->DeleteSoundData(SoundData);
	}
	SAFE_DELETE(File)
}
CzSoundInst CzSound::Play(float volume, float pitch, float pan, bool looped)
{
	if (SoundSpec == NULL)
		Load();
	if (SoundSpec == NULL)
		return NULL;

	return PLATFORM_AUDIO->PlaySound(SoundSpec, volume, pitch, pan, looped);
}

int32 CzSound_FileRetrievedAsyncCallback(void* caller, void* data)
{
	CzSound* sound = (CzSound*)data;
	sound->FinishLoad();

	return 0;
}

bool CzSound::Load(bool blocking)
{
	// If already loaded return true
	if (SoundSpec != NULL)
		return true;

	// Image is file based
	if (File != NULL)
	{
		File->setFileAvailableCallback(CzSound_FileRetrievedAsyncCallback, this);
		if (File->Open(NULL, "rb", blocking))
			return true;
		else
			return false;
	}

/*	if (ResourceGroup != NULL)
	{
		// Create image from the resource group
		IwGetResManager()->SetCurrentGroup(ResourceGroup);
		SoundSpec = (CIwSoundSpec*)ResourceGroup->GetResHashed(NameHash, IW_SOUND_RESTYPE_SPEC);
		State = State_Loaded;
		return true;
	}*/

	return false;
}

void CzSound::FinishLoad()
{
	if (File != NULL)
	{
		if (CzFile::isHttp(File->getFilename().c_str(), File->getFilename().getLength()))
		{
			if (File->isFileAvailable() && File->getError() == CzFileErrorNone)
				Init(File->getContent(), File->getContentLength());
		}
		else
		{
			char* data = new char [File->getFileSize()];
			File->Read(data, File->getFileSize());
			Init(data, File->getFileSize());
			delete data;
		}
		State = State_Loaded;
		SAFE_DELETE(File)
	}
}

bool CzSound::Init(void* memory_file, int memory_file_size)
{
	CzFile file;

	if (!PLATFORM_AUDIO->InitSound(memory_file, memory_file_size, SoundData, SoundSpec))
		return false;

	State = State_Loaded;

	return true;
}

void CzSound::Init(const char* filename)
{
	File = new CzFile();
	File->setFilename(filename);
}

int CzSound::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Process image attributes
	CzString* name = NULL;
	CzString* location = NULL;
	bool preload = true;
	bool blocking = false;
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
		if (name_hash == CzHashes::Location_Hash)
			location = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Preload_Hash)
			preload = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Blocking_Hash)
			blocking = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Condition_Hash)
			condition = &(*it)->getValue();
	}

	if (location == NULL || name == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Audio - A sound requires a location and a name to be specified", DebugInfo.c_str());
		return 0;
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
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Audio - Condition variable not found - ", condition->c_str(), DebugInfo.c_str());
#endif // _DEBUG
	}

	// Check to see if image is located externally
	if (CzFile::isHttp(location->c_str(), location->getLength()))
	{
		Init(location->c_str());
	}
	else
	if (CzFile::isFile(location->c_str(), location->getLength()))
	{
		Init(location->c_str());
	}
	else
	{
		// Find resuorce group
/*		CzResourceGroup* group = NULL;
		if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
			group = (CzResourceGroup*)scene->getResourceManager()->findResource(location->c_str(), CzHashes::ResourceGroup_Hash);
		else
			group = (CzResourceGroup*)CZ_GLOBAL_RESOURCES->getResourceManager()->findResource(location->c_str(), CzHashes::ResourceGroup_Hash);

		if (group != NULL)
			Init(name->c_str(), group->getResourceGroup());
		else
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Audio - Invalid sound resource group name - ", location->c_str(), DebugInfo.c_str());
			return false;
		}*/
	}
	if (preload)
	{
		Load(blocking);
	}

	// If we are declared inside a scene then image is local to the scene
	if (scene != NULL)
		return scene->getResourceManager()->addResource(this) ? 1 : 0;
	else
		return CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this) ? 1 : 0;

	return 1;
}


