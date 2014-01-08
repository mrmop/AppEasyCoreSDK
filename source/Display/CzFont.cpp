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

#include "IzPlatformDisplay.h"
#include "CzFont.h"
#include "CzResources.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"

//
//
//
//
// CzFont implementation
//
//
//
//

CzFont::~CzFont()
{
	SAFE_DELETE(FontBuffer);
	if (Delete)
		PLATFORM_FONT->DestroyFont(Font);
	SAFE_DELETE(File)
}

int CzFont::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Process image attributes
	CzString* name = NULL;
//	CzString* resource_name = NULL;
	CzString* location = NULL;
	CzString* condition = NULL;
	bool preload = true;
	bool blocking = false;
	float auto_point_size = 0;	// Approximate number of lines of font allows on screen down longest axis

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
//		else
//		if (name_hash == CzHashes::ResourceName_Hash)
//			resource_name = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Location_Hash)
			location = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Condition_Hash)
			condition = &(*it)->getValue();
		else
 		if (name_hash == CzHashes::Preload_Hash)
			preload = (*it)->getValueAsBool();
		else
 		if (name_hash == CzHashes::Blocking_Hash)
			blocking = (*it)->getValueAsBool();
		else
 		if (name_hash == CzHashes::PointSize_Hash)
			PointSize = (*it)->getValueAsFloat();
		else
 		if (name_hash == CzHashes::AutoPointSize_Hash)
			auto_point_size = (*it)->getValueAsFloat();
	}

	if (location == NULL || name == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Font - Requires a location and a name to be specified", DebugInfo.c_str());
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
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Font - Condition variable not found - ", condition->c_str(), DebugInfo.c_str());
#endif // _DEBUG
	}

	// If no resource_name specified then assign it the name
//	if (resource_name == NULL)
//		resource_name = name;

	// If auto point size then choose point size from screen resolution
	if (auto_point_size > 0)
	{
		int w = PLATFORM_DISPLAY->getCurrentWidth();
		int h = PLATFORM_DISPLAY->getCurrentHeight();
		int d = w > h ? w : h;
		float dy = (float)d / auto_point_size;		// Calculate approximate pixel size
		int point_size = (int)(dy / 1.34f);	// Calculate approximate point size from pixel height of glyph
		PointSize += point_size;
	}

	// Check to see if image is located externally
	if (CzFile::isHttp(location->c_str(), location->getLength()) || CzFile::isFile(location->c_str(), location->getLength()))
	{
		if (CzFile::isFile(location->c_str(), location->getLength()))
		{
			// Check for font already existing
			if (scene != NULL)
				UsingFont = scene->getResourceManager()->findFontWithFilename(location->c_str());
			else
				UsingFont = CZ_GLOBAL_RESOURCES->getResourceManager()->findFontWithFilename(location->c_str()), false;
		}

		Filename = *location;
		if (preload)
			Load(blocking);
	}
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Font - Invalid font location name - ", location->c_str(), DebugInfo.c_str());
		return 0;
	}

	// If we are declared inside a scene then font is local to the scene
	if (scene != NULL)
		return scene->getResourceManager()->addResource(this);
	else
		return CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this);

	return 1;
}

int32 CzFont_FileRetrievedAsyncCallback(void* caller, void* data)
{
	CzFont* font = (CzFont*)data;
	font->FinishLoad();

	return 0;
}

CzFontHandle CzFont::getFontHandle()
{
	if (State == State_Loading)
		return NULL;

	if (Load())
		return Font;
	
	return NULL;
}

bool CzFont::Load(bool blocking)
{
	// If already loaded return true
	if (Font != NULL)
		return true;

	if (CzFile::isHttp(Filename.c_str(), Filename.getLength()))
	{
		if (UsingFont != NULL)
		{
			if (UsingFont->getState() == State_Loaded)
			{
				Font = PLATFORM_FONT->CreateTTFontFromBuffer(UsingFont->File->getContent(), UsingFont->File->getContentLength(), PointSize);
				State = State_Loaded;
				Delete = true;
				return true;
			}
		}
		else
		{
			State = State_Loading;
			File = new CzFile();
			File->setFilename(Filename.c_str());
			File->setFileAvailableCallback(CzFont_FileRetrievedAsyncCallback, this);
			if (File->Open(NULL, "rb", blocking))
				return true;
			else
				return false;
		}
	}
	else
	if (CzFile::isFile(Filename.c_str(), Filename.getLength()))
	{
		// Create font from local file
/*		Font = IwGxFontCreateTTFont(Filename.c_str(), PointSize);
		if (Font == NULL)
			return false;
		else
		{
			State = State_Loaded;
			Delete = true;
		}*/

		if (UsingFont != NULL)
		{
			if (UsingFont->getState() == State_Loaded)
			{
				Font = PLATFORM_FONT->CreateTTFontFromBuffer(UsingFont->getFontBuffer(), UsingFont->getFontBufferSize(), PointSize);
				State = State_Loaded;
				Delete = true;
				return true;
			}
		}
		else
		{
			File = new CzFile();
			File->setFilename(Filename.c_str());
//			File->setFileAvailableCallback(CzFont_FileRetrievedAsyncCallback, this);
			if (!File->Open(NULL, "rb", blocking))
				return false;

			FontBufferSize = File->getFileSize();
			FontBuffer = new uint8 [FontBufferSize];
			File->Read(FontBuffer, FontBufferSize);
			Font = PLATFORM_FONT->CreateTTFontFromBuffer((void*)FontBuffer, FontBufferSize, PointSize);

			Delete = true;
			State = State_Loaded;
			SAFE_DELETE(File)
		}
	}

	return false;
}

void CzFont::FinishLoad()
{
	if (File != NULL)
	{
		if (CzFile::isHttp(File->getFilename().c_str(), File->getFilename().getLength()))
		{
			// Font info has to remain in memory so we do not delete the file once it is loaded
			if (File->isFileAvailable() && File->getError() == CzFileErrorNone)
				Font = PLATFORM_FONT->CreateTTFontFromBuffer(File->getContent(), File->getContentLength(), PointSize);
		}
		else
		{
		}
		Delete = true;
		State = State_Loaded;
	}
}



