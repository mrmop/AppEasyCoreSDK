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

#include "IzPlatformFile.h"
#include "IzPlatformComms.h"
#include "CzFile.h"
#include "CzUtil.h"
#include "CzActor.h"
#include "CzScript.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"

/**
 @fn	int CzFile::getFileSize()

 @brief	Returns the size of the file in bytes.

 @return	The file size.
 */

int	CzFile::getFileSize()
{
	if (File == NULL)
		return -1;

	return PLATFORM_FILE->getSize(File);
}

/**
 @fn	bool CzFile::Read(void* buffer, int len)

 @brief	Reads len bytes from the file into buffer.

 @param [out]	buffer	If non-null, the buffer.
 @param			len		Number of bytes to read.

 @return	true if it succeeds, false if it fails.
 */

bool CzFile::Read(void* buffer, int len)
{
	if (File == NULL)
		return false;

	IzPlatformFile* pf = PLATFORM_FILE;

	if (pf->Read(File, buffer, (uint)len) != 1)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "CzFile::Read(): ", pf->getErrorString());
#endif	// _DEBUG
		pf->Close(File);
		return false;
	}

	return true;
}

/**
 @fn	bool CzFile::Write(void* buffer, int len)

 @brief	Writes len bytes from buffer to the file

 @param [in]	buffer	If non-null, the buffer.
 @param			len		Number of bytes to write.

 @return	true if it succeeds, false if it fails.
 */

bool CzFile::Write(void* buffer, int len)
{
	if (File == NULL)
		return false;

	IzPlatformFile* pf = PLATFORM_FILE;

	if (pf->Write(File, buffer, (uint)len) != 1)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "CzFile::Write(): ", pf->getErrorString());
#endif	// _DEBUG
		pf->Close(File);
		return false;
	}

	return true;
}

/**
 @fn	bool CzFile::Open(const char* path, const char* mode, bool blocking)

 @brief	Opens the file specified by path.

 mode values are:
 - "r"  Opens file for reading only
 - "w"  Creates an empty file for writing. If file already exists then it is written over
 - "a"  Opens file for appending. If file does not exist then it is created
 - "r+" Opens file for reading and writing. File must already exist
 - "w+" Creates an empty file for reading and writing. If file already exists then it is written over
 - "a+" Opens file for reading and appending. If file does not exist then it is created
 - "b"  Opens the file in binary mode

 If an error occurs then you can get more information about the error by calling getError()

 @param	path		Path of the file.
 @param	mode		The mode.
 @param	blocking	true to block whilst loading.

 @return	true if it succeeds, false if it fails.
 */

bool CzFile::Open(const char* path, const char* mode, bool blocking)
{
	FileAvailable = false;
	InMemory = false;

	// make sure user cannot open the file twice
	if (File != NULL)
	{
		Error = CzFileErrorAlreadyOpen;
		return false;
	}

	// Get the path
	if (path != NULL)
		Filename.setString(path);
	else
		path = Filename.c_str();

	// Check we actually have a path
	if (path == NULL)
	{
		Error = CzFileErrorInvalidPath;
		return false;
	}

	// Check to see if the file is located on the web
	if (isHttp(Filename.c_str(), Filename.getLength()))
	{
		Download();

		if (blocking)
		{
			while (!FileAvailable)
			{
				CZ_HTTP_MANAGER->Update();
				PLATFORM_SYS->YieldToOS(0);
			}
			if (Error != CzFileErrorNone)
				return false;
		}

		return true;
	}
	else
	{
		if (mode == NULL)
		{
			Error = CzFileErrorEmptyMode;
			return false;
		}
	}

	// Open the file
	IzPlatformFile* pf = PLATFORM_FILE;
	File = pf->Open(Filename.c_str(), mode);
	if (File == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "CzFile::Open(): ", pf->getErrorString());
#endif	// _DEBUG
		Error = CzFileErrorOpenFailed;
		return false;
	}
	NotifyAvailable();

	FileAvailable = true;

	return true;
}

/**
 @fn	bool CzFile::Open(void* memory_buffer, int memory_buffer_len)

 @brief	Opens a memory buffer and provides file style access to it.

 @param [in,out]	memory_buffer	If non-null, buffer for memory data.
 @param	memory_buffer_len		 	Length of the memory buffer.

 @return	true if it succeeds, false if it fails.
 */

bool CzFile::Open(void* memory_buffer, int memory_buffer_len)
{
	FileAvailable = false;
	InMemory = true;

	IzPlatformFile* pf = PLATFORM_FILE;
	File = pf->OpenFromMemory(memory_buffer, memory_buffer_len);
	if (File == NULL)
	{
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "CzFile::Open(memory): ", pf->getErrorString());
#endif	// _DEBUG
		Error = CzFileErrorOpenFailed;
		return false;
	}

	FileAvailable = true;

	return true;
}

/**
 @fn	void CzFile::Close()

 @brief	Closes the file.

 */

void CzFile::Close()
{
	if (File != NULL)
	{
		PLATFORM_FILE->Close(File);
		File = NULL;
	}
}

/**
 @fn	bool CzFile::Seek(int offset, eCzFileSeekOrigin origin)

 @brief	Seeks the file positin pointer back and forth.

 @param	offset	The offset.
 @param	origin	The origin.

 @return	true if it succeeds, false if it fails.
 */

bool CzFile::Seek(int offset, eCzFileSeekOrigin origin)
{
	if (File == NULL)
		return false;

	if (PLATFORM_FILE->Seek(File, offset, origin))
		return false;

	return true;
}

/**
 @fn	void* CzFile::getContent()

 @brief	Returns data that was downloaded.

 If a file is opened as a URL and the file has been downloaed then callnig getContent() will return the data that was downloaded.

 @return	null if it fails, else the content.
 */

void* CzFile::getContent()
{
	if (!InMemory || Request == NULL)
		return NULL;
	if (FileAvailable && Request->getError() == 0)
		return (void*)Request->getContent().c_str();

	return NULL;
}

/**
 @fn	int CzFile::getContentLength() const

 @brief	Gets downloaded contents length.

 If a file is opened as a URL and the file has been downloaed then calling getContentLength() will return the size of the downloaded data.

 @return	The content length.
 */

int CzFile::getContentLength() const
{
	if (!InMemory || Request == NULL)
		return -1;
	if (FileAvailable && Request->getError() == 0)
		return Request->getContentLength();

	return -1;
}

int32 FileRetrievedCallback(void* caller, void* data)
{
	CzHttpRequest* request = static_cast<CzHttpRequest*>(caller);
	CzHttpObject http_object = CZ_HTTP_MANAGER->getHttpObject();
	if (PLATFORM_COMMS->HttpGetResponseCode(http_object) == 302)	// Redirect
	{
		CzString header;
		PLATFORM_COMMS->HttpGetHeader(http_object, "Location", header);
//		printf("**** Redirected to: %s\n", header.c_str());
		((CzFile*)data)->setFilename(header.c_str());
		((CzFile*)data)->Download();
		return 0;
	}

	if (request->getProcessed())									// Check to see if our request was processed by the http manager
	{
		((CzFile*)data)->FileReceived(request, request->getError());
//		CZ_HTTP_MANAGER->RemoveRequest(request);				// Remove request from http manager queue
	}

	return 0;
}

/**
 @fn	bool CzFile::Download()

 @brief	Initiates down of the file.

 @return	true if it succeeds, false if it fails.
 */

bool CzFile::Download()
{
	FileAvailable = false;

	// Build download request
	if (Request == NULL)
		Request = new CzHttpRequest();
	Request->setGET();
	Request->setURI(Filename.c_str());
	Request->setContentAvailableCallback(&FileRetrievedCallback, (void*)this);
	CZ_HTTP_MANAGER->AddRequest(Request);

#if defined(_DEBUG)
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "CzFile::Download with URL:", Filename.c_str());
#endif	// _DEBUG

	return false;
}

/**
 @fn	void CzFile::FileReceived(CzHttpRequest* request, int error)

 @brief	Called when a flle download has finished.

 @param [in]	request	If non-null, the http request.
 @param			error			   	 The error if any.
 */

void CzFile::FileReceived(CzHttpRequest* request, int error)
{
	// If there wwas an error then set the error
	if (error != 0)
	{
		Error = CzFileErrorHttp;
#if defined(_DEBUG)
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "CzFile::FileReceived(): ", CzString(error).c_str());
#endif	// _DEBUG
	}

	// Open the file
	Open((void*)request->getContent().c_str(), request->getContentLength());

	// Notify caller that the file is available
	NotifyAvailable();
}

/**
 @fn	void CzFile::NotifyAvailable()

 @brief	Notifies the system that the file is available.

 Calls the user supplied callback to notify the user that the file is loaded / downloaded and is available to use.

 */

void CzFile::NotifyAvailable()
{
	// Call any user supplied callback
	if (FileAvailableCallback != NULL)
		FileAvailableCallback((void*)this, FileAvailableCallbackData);

	FileAvailable = true;
}

//
//
//
// Utility methods
//
//
//

/**
 @fn	void CzFile::GetComponents(const char* file_path, CzFilePathComponents& components)

 @brief	Extracts the components of a file path.

 Splits the supplied file path into path, filename and extension components.

 @param			file_path			  	Full pathname of the file.
 @param [out]	components				The components.
 */

void CzFile::GetComponents(const char* file_path, CzFilePathComponents& components)
{
	int							len = strlen(file_path) - 1;
	const char*					name_ptr = file_path + len;

	// Scan backwards looking for dot
	int index = 0;
	while (len >= 0)
	{
		if (*name_ptr == '.')
		{
			components.Extension.setString(name_ptr + 1, index);
			name_ptr--;
			len--;
			break;
		}
		else
		if (len == 0)
		{
			components.Extension.setString(name_ptr, index + 1);
			name_ptr--;
			len--;
			break;
		}
		name_ptr--;
		index++;
		len--;
	}
	if (len < 0)
		return;

	// Continue scanning for filename
	index = 0;
	while (len >= 0)
	{
		if (*name_ptr == '/' || *name_ptr == '\\')
		{
			components.Filename.setString(name_ptr + 1, index);
			name_ptr--;
			len--;
			break;
		}
		else
		if (len == 0)
		{
			components.Filename.setString(name_ptr, index + 1);
			name_ptr--;
			len--;
			break;
		}

		name_ptr--;
		index++;
		len--;
	}
	if (len < 0)
		return;

	len = (name_ptr - file_path) + 1;
	if (len > 0)
		components.Folder.setString(file_path, len);
}

/**
 @fn	bool CzFile::GetFileType(const char* file_path, CzString& type)

 @brief	Gets file type (its extension).

 @param				file_path	Full pathname of the file.
 @param [out]		type		The type.

 @return	true if it succeeds, false if it fails.
 */

bool CzFile::GetFileType(const char* file_path, CzString& type)
{
	int						len = strlen(file_path) - 1;
	const char*				name_ptr = file_path + len;

	// Scan backwards looking for dot
	int index = 0;
	while (len >= 0)
	{
		if (*name_ptr == '.')
		{
			type.setString(name_ptr + 1, index);
			type.ToLower();
			break;
		}
		else
		if (len == 0)
		{
			type.setString(name_ptr, index + 1);
			type.ToLower();
			break;
		}
		name_ptr--;
		index++;
		len--;
	}
	
	return true;
}

/**
 @fn	bool CzFile::isHttp(const char* file_path, int path_len)

 @brief	Query if 'file_path' is a web based file.

 @param	file_path	Full pathname of the file.
 @param	path_len 	Length of the path.

 @return	true if http, false if not.
 */

bool CzFile::isHttp(const char* file_path, int path_len)
{
	// Ignore very short path names because they cannot be a URI
	if (path_len < 10)
		return false;

	int check = *((int*)file_path);
	if (check == (((int)'h') | ((int)'t' << 8) | ((int)'t' << 16) | ((int)'p' << 24)) || check == (((int)'H') | ((int)'T' << 8) | ((int)'T' << 16) | ((int)'P' << 24)))
	{
		if (*(file_path + 4) == ':' || *(file_path + 5) == ':')
			return true;
	}

	return false;
}

/**
 @fn	bool CzFile::isFile(const char* file_path, int path_len)

 @brief	Query if 'file_path' is a file.

 Note that this method does a simple check for the presence of a file extension. It will not work for http based files.

 @param	file_path	Full pathname of the file.
 @param	path_len 	Length of the path.

 @return	true if file, false if not.
 */

bool CzFile::isFile(const char* file_path, int path_len)
{
	// Ignore very short path names because they cannot be a URI
	if (path_len < 3)
		return false;

	for (int t = 0; t < path_len; t++)
	{
		if (*file_path++ == '.')
			return true;
	}

	return false;
}

/**
 @fn	bool CzFile::FileExists(const char* file_path)

 @brief	Queries if a given file exists.

 @param	file_path	Full pathname of the file.

 @return	true if it exists, false if it does not.
 */

bool CzFile::FileExists(const char* file_path)
{
	return PLATFORM_FILE->Exists(file_path);
}

/**
 @fn	bool CzFile::DeleteFile(const char* file_path)

 @brief	Deletes the file described by file_path.

 @param	file_path	Full pathname of the file.

 @return	true if it succeeds, false if it fails.
 */

bool CzFile::DeleteFile(const char* file_path)
{
	if (FileExists(file_path))
		PLATFORM_FILE->Delete(file_path);
	else
		return false;

	return true;
}


//
//
//
//
// CzDataFile implementation
//
//
//
//
CzDataFile::~CzDataFile()
{
	SAFE_DELETE(EventsManager);
	SAFE_DELETE(File)
	SAFE_DELETE(DataInput)
}

/**
 @fn	bool CzDataFile::Init(const char* filename, bool preload, bool blocking)

 @brief	Initialises the XOML file holder.

 @param	filename	Filename of the file.
 @param	preload 	true to preload.
 @param	blocking	true to use blocking.

 @return	true if it succeeds, false if it fails.
 */

bool CzDataFile::Init(const char* filename, bool preload, bool blocking)
{
	SAFE_DELETE(DataInput)
	SAFE_DELETE(File)
	State = State_Invalid;

	if (EventsManager == NULL)
		EventsManager = new CzEventManager();

	Filename = filename;
	File = new CzFile(filename);

	if (preload)
		return Load(blocking);

	return true;
}

int32 CzDataFile_FileRetrievedAsyncCallback(void* caller, void* data)
{
	CzDataFile* file = (CzDataFile*)data;
	file->FinishLoad();

	return 0;
}

/**
 @fn	bool CzDataFile::Load(bool blocking)

 @brief	Loads the file specified by this XOML file holder.

 @param	blocking	true if file load should block main thread.

 @return	true if it succeeds, false if it fails.
 */

bool CzDataFile::Load(bool blocking)
{
	if (DataInput != NULL)
		return true;

	if (File != NULL)
	{
		File->setFileAvailableCallback(CzDataFile_FileRetrievedAsyncCallback, this);
		if (File->Open(NULL, "rb", blocking))
			return true;
		else
			return false;
	}

	return false;
}

/**
 @fn	void CzDataFile::FinishLoad()

 @brief	Called internally when the file has been downloaded and is available.

 */

void CzDataFile::FinishLoad()
{
	if (File != NULL)
	{
		if (CzFile::isHttp(File->getFilename().c_str(), File->getFilename().getLength()))
		{
			// Store the retrieved data
			if (File->isFileAvailable() && File->getError() == CzFileErrorNone)
			{
				if (Converter == Converter_None)
				{
					if (File->getContentLength() == 0)
					{
						State = State_Invalid;
						NotifyError();
						return;
					}
					DataInput = new CzDataInput(true);
					DataInput->setData((char*)File->getContent(), File->getContentLength());
				}
				else
				{
					// Convert the retrieved data
					CzString data;
					data.setString((char*)File->getContent(), File->getContentLength());
					switch (Converter)
					{
					case Converter_FromHTML:
						data.ReplaceHTMLCodes();
						break;
					case Converter_FromHex:
						data.URLDecode();
						break;
					case Converter_FromURLEncoded:
						data.HexDecode();
						break;
					}
					int len = data.getLength();
					DataInput = new CzDataInput();
					DataInput->Init(len + 1);
					memcpy(DataInput->getData(), data.c_str(), len);
					DataInput->getData()[len] = 0;
					File->Close();
				}
			}
			else
			{
				State = State_Invalid;
				NotifyError();
				return;
			}
		}
		else
		{
			// Load data
			int len = File->getFileSize();
			if (len == 0)
			{
				State = State_Invalid;
				NotifyError();
				return;
			}
			DataInput = new CzDataInput();
			DataInput->Init(len + 1);
			File->Read(DataInput->getData(), len);
			DataInput->getData()[len] = 0;

			if (Converter != Converter_None)
			{
				// Convert the retrieved data
				CzString text;
				text.setString(DataInput->getData(), len);
				switch (Converter)
				{
				case Converter_FromHTML:
					text.ReplaceHTMLCodes();
					break;
				case Converter_FromHex:
					text.URLDecode();
					break;
				case Converter_FromURLEncoded:
					text.HexDecode();
					break;
				}
				DataInput->Release();
				DataInput->Init(text.getLength() + 1);
				memcpy(DataInput->getData(), text.c_str(), text.getLength() + 1);
			}

			File->Close();
		}

		// If target variable is set then write the contents of the file to the variable
		if (TargetVariable != NULL)
			TargetVariable->setValue(DataInput->getData(), DataInput->getLength());

		// If target script is set then write the contents of the file to the script
		if (TargetScript != NULL)
			TargetScript->Init(DataInput->getData(), DataInput->getLength() - 1, NULL);	// 1 less than length because Lua does not like null terminator

		State = State_Loaded;
		NotifyLoaded();
	}
}

/**
 @fn	int CzDataFile::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)

 @brief	Creates an instance of this class from XOML.

 LoadFromXoml is the main method used by all classes that can be instantiated from XOML mark-up. This method creates an instance of this class from the supplied XML node structure specified by node. 
 if parent is not NULL then the created instance will utilise the supplied parent to determine where it should live within the app. For example, if the parent is a scene then the file will be placed 
 in that scene. if the parent is the app then the file will be available globally to all scenes. 
 nodes contained within the actor will also be loaded.

 @param [in]	parent		If non-null, the parent.
 @param	load_children	  	true to load child nodes.
 @param [in]	node  		If non-null, the XOML markup node that defines this object

 @return	nergative value for error. if 0 then processing of further XOML will stop.
 */

int CzDataFile::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Process image attributes
	CzString* name = NULL;
	CzString* location = NULL;
	bool preload = true;
	bool blocking = true;
	CzString* condition = NULL;
	CzString* variable = NULL;
	CzString* script = NULL;

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
		else
		if (name_hash == CzHashes::FileType_Hash)
			FileType = (*it)->getValue();
		else
		if (name_hash == CzHashes::Variable_Hash)
			variable = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Script_Hash)
			script = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Converter_Hash)
		{
			unsigned int converter_hash = (*it)->getValue().getHash();
			if (converter_hash == CZ_HASH("html"))
				Converter = Converter_FromHTML;
			else
			if (converter_hash == CZ_HASH("hex"))
				Converter = Converter_FromHex;
			else
			if (converter_hash == CZ_HASH("urlencoded"))
				Converter = Converter_FromURLEncoded;
		}
		else
		if (name_hash == CzHashes::OnLoaded_Hash)
			EventsManager->addEvent("OnLoaded", (*it)->getValue().c_str(), true);
		else
		if (name_hash == CzHashes::OnError_Hash)
			EventsManager->addEvent("OnError", (*it)->getValue().c_str(), true);
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
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "File - Condition variable not found - ", condition->c_str(), DebugInfo.c_str());
#endif // _DEBUG 
	}

	// if target variable is set then assign the variable
	if (variable != NULL)
	{
		CzXomlVariable* var = CzXomlVariable::GetVariable(*variable, scene);
		if (var == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "File - Target variable does not exist - ", variable->c_str(), DebugInfo.c_str());
		else
			TargetVariable = var;
	}

	// if target script is set then assign the 
	if (script != NULL)
	{
		CzScript* s = (CzScript*)CzXomlResourceManager::FindResource(script->c_str(), CzHashes::Script_Hash, scene);
		if (s == NULL)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "File - Script target does not exist - ", script->c_str(), DebugInfo.c_str());
		else
			TargetScript = s;
	}

	// If we are declared inside a scene then image is local to the scene
	bool ret = false;
	if (scene != NULL)
		ret = scene->getResourceManager()->addResource(this);
	else
		ret = CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this);

	if (!ret)
		return 0;

	// Initialise the file
	Init(location->c_str(), preload, blocking);

	return 1;
}

/**
 @fn	void CzDataFile::ProcessEventActions(unsigned int event_name)

 @brief	Process the event actions described by event_name.

 Processes the actions list that is associated with the event named event_name (event_name is a hash of the actual event name to speed up searches)

 @param	event_name	Name of the event as a string hash
 */

void CzDataFile::ProcessEventActions(unsigned int event_name)
{
	// Find the event 
	CzEvent* evt = EventsManager->findEvent(event_name);
	if (evt != NULL)
	{
		// Find actions list
		IzXomlResource* parent = Parent->getParent();
		CzActions* actions = CzActionsManager::FindActions(evt->ActionsName, parent);
		if (actions != NULL)
			actions->Execute(parent);
	}
}

/**
 @fn	void CzDataFile::NotifyLoaded()

 @brief	Handles the file loaded event.

 */

void CzDataFile::NotifyLoaded()
{
	ProcessEventActions(CzHashes::OnLoaded_Hash);
}

/**
 @fn	void CzDataFile::NotifyError()

 @brief	Handles the file error event.

 */

void CzDataFile::NotifyError()
{
	ProcessEventActions(CzHashes::OnError_Hash);
}




