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

#if !defined(_CZ_FILE_H_)
#define _CZ_FILE_H_

#include "IzPlatformFile.h"
#include "CzString.h"
#include "CzHttp.h"
#include "CzDataIO.h"
#include "CzXoml.h"
#include "CzXomlVariables.h"
#include "CzEvents.h"



class CzScript;

/**
 @addtogroup Core
 @{
 */
 
/**
 @struct	CzFilePathComponents

 @brief	file path components.

 */

struct CzFilePathComponents
{
	CzString Folder;			///< Folder name
	CzString Filename;			///< Name of file
	CzString Extension;			///< File extension
};

/**
 @class	CzFile

 @brief	CzFile respresents a file.

 <h1>Introduction</h1>
 AppEasy encapsulates the file system neatly into a single class called CzFile. This class enables the following features:
 - Auto file close / cleanup when the file object goes out of scope
 - Reading and writing of local files
 - Reading and writing of memory based files
 - Blocking and none blocking reading of files from an external source such as a web site / server
 - File name splitting
 - File type retrieval

 <h1>Loading a Local File</h1>
 Loading a local file is very simple, as is shown in the following example:

@code
// Here we declare a string, open a file then read some data into it
CzString data;
CzFile file;
if (file.Open("\\my_data.txt", "rb"))
{
	int len = file.getFileSize();
	data.allocString(len);
	data.setLength(len);
	file.Read((void*)data.c_str(), len);
}
@endcode

 <h1>Saving a Local File</h1>
 Saving a local file is also a very simple, as is shown in the following example:

@code	
// Here we declare a string, open a file then write the string to it
CzString data("Hello storage, how you doing?");
CzFile file;
if (file.Open("\\my_data.txt", "wb"))
{
	file.Write((void*)data.c_str(), data.GetLength());
}
@endcode

 <h1>Loading a Memory Based File</h1>
 Loading a memory file is just as easy as opening a local file, as is shown in the following example:

 @code
// Here we declare a string, open a file then read some data into it from memory
CzString data;
CzFile file;
if (file.Open(my_data_in_memory, my_data_length))
{
	int len = file.getFileSize();
	data.allocString(len);
	data.setLength(len);
	file.Read((void*)data.c_str(), len);
}
 @endcode

 <h1>Loading a Remote File</h1>
 The format of loading a file for remote is the same as local file loading

 @code	
// Here we declare a string, open a remote file then read some data into it
CzString data;
CzFile file;
if (file.Open("http://www.myserver.com/my_data.txt", NULL, true))
{
	int len = file.getFileSize();
	data.allocString(len);
	data.setLength(len);
	file.Read((void*)data.c_str(), len);
}
 @endcode

 With a few differences. The first differenece is the very noticable filename change to that of a web address. The second more subtle difference is the inlusion of a 3rd parameter to 
 Open() which tells the method to block until the complete file has been downoaded or an error occurs.

 With modern games / apps the user expects action on the screen most of the time, so sitting loading your assets from a server with no viusal update would not be a great idea, ruling 
 blocking remote file loading out for anything more than a few files. A better alternative is to use asynchonrous file downloading that is none blocking, allowing the game loop to 
 proceed whilst your assets load. 

 Loading a remote file using a none blocking method can be achieved as shown below:

 @code
int32 WebFileRetrievedCallback(void* caller, void* data)
{
	CzFile* file = (CzFile*)caller;

	// file->getContent() and file->getContentLength() contain the data and data size

	delete file;

	return 0;
}
	// Initiate a none blocking file  download
	CzFile* image_file = new CzFile();
	image_file->setFileAvailableCallback(WebFileRetrievedCallback, NULL);
	image_file->Open("http://www.battleballz.com/bb_icon.gif", NULL, false);
@endcode

 Examining the above code we can see that we set up a callback so that we get notified when our file has been downloaded. Next we initiate the file download but this time passing "false" 
 as our blocking parameter to ensure that the download does not block the main thread.

 If you don't fancy setting up a callback, you can poll the CzFile instead to see if the file has been retrieved using:

 @code
bool		CzFile::isFileAvailable()
 @endcode

 <h1>Other Useful File Tools</h1>
 CzFile also contains a few additional useful tool type methods:

 @code
static void	GetComponents(const char* file_path, CzFilePathComponents& components); // Splits a path into its separate drive, path, name and extension components
static bool	GetFileType(const char* file_path, CzString& type); // Returns the file type of the supplied file name
static bool	isHttp(const char* file_path, int path_len); // Checks a file name to see if it uses the http protocol
static bool isFile(const char* file_path, int path_len); // Returns true if the path is a file (simple check for for an extension, will not work with url's)
static bool FileExists(const char* file_path);	// Returns true if the file exists
static bool DeleteFile(const char* file_path);	// Deletes a file
@endcode


 @todo
 - Add none blocking file I/O
 - Add timeout check to blocking http download
 - Add methods for reading and writing common types
 - Add support for directory creation / removal
 - Add support directory / file discovery

 */

class CzFile
{
public:

protected:
	// Properties
	CzFileHandle		File;								///< File handle
	CzString			Filename;							///< Name of file
	bool				FileAvailable;						///< True when file is available
	CzCallback			FileAvailableCallback;				///< Callback to be called when file is available
	void*				FileAvailableCallbackData;			///< Callback data to be passed back with callback
	eCzFileError		Error;								///< Contains error code if any if file not received
public:
	CzFileHandle		getFileHandle()						{ return File; }
	void				setFilename(const char* filename)	{ Filename.setString(filename); }
	CzString&			getFilename()						{ return Filename; }
	void				setFileAvailable(bool available)	{ FileAvailable = available; }
	bool				isFileAvailable()					{ return FileAvailable; }
	void				setFileAvailableCallback(CzCallback callback, void *data)	{ FileAvailableCallback = callback; FileAvailableCallbackData = data; }
	int					getFileSize();
	eCzFileError		getError() const					{ return Error; }
	void*				getContent();
	int					getContentLength() const;
	// Properties end

protected:
	bool				InMemory;
	CzHttpRequest*		Request;
	void				NotifyAvailable();

public:
	CzFile() :						File(NULL), Error(CzFileErrorNone), Request(NULL), FileAvailableCallback(NULL), FileAvailableCallbackData(NULL) 	{	}
	CzFile(const char* filename) :	File(NULL), Error(CzFileErrorNone), Request(NULL), FileAvailableCallback(NULL), FileAvailableCallbackData(NULL)
	{
		Filename.setString(filename);
	}
	virtual		~CzFile()
	{
		SAFE_DELETE(Request)
		Close();
	}
	bool				Open(const char* filename = NULL, const char* mode = NULL, bool blocking = false);	// Open file for read or write
	bool				Open(void* memory_buffer, int memory_buffer_len);		// Open file for read or write from a memory buffer
	bool				Invalid();												// Releases file
	bool				Download();												// Download file from an external location

	bool				Read(void* buffer, int len);
	bool				Write(void* buffer, int len);
	bool				Seek(int offset, eCzFileSeekOrigin origin);
	void				Close();

	// Utility
	static void			GetComponents(const char* file_path, CzFilePathComponents& components);
	static bool			GetFileType(const char* file_path, CzString& type);
	static bool			isHttp(const char* file_path, int path_len);
	static bool			isFile(const char* file_path, int path_len);
	static bool			FileExists(const char* file_path);
	static bool			DeleteFile(const char* file_path);

	// Internal
	void				FileReceived(CzHttpRequest* request, int error);	// Called by the http callback internally when the file is received
	
};

/**
 @class	CzDataFile

 @brief	A XOML data file.

 CzDataFile is the XOML representation of a file (a file holder) and enables files to be created and loaded using the File XOML tag. 

 Its often useful to be able to load files into an app and do something with the data that they contain. Files in XOML are just like any other resource and can be declared inside a scene 
 in which case they become local to the scene or declared outside a scene in which case they become global. They can also be loaded from local storage or from a web server, Files can also 
 convert their data after loading from an number of different formats. We will begin by taking a look at the properties that are available for the Files tag:
 - Name (string) - Name of this file resource
 - Tag (string) - Resource tag (used to group resources together) (optional)
 - Location (filename) - Name of the file including extension (can include web addresses)
 - Preload (boolean) - If set to true then the file will be loaded immediately. By setting to false the file will be loaded when it is first used or can be loaded by an action later (default is true).
 - Blocking (boolean) - Web based files take time to download from the web so its useful to allow execution of the app to continue whilst it downloads. To prevent the file download from blocking the app set Blocking="false" (default is to block). 
 - Condition (variable) - A condition variable that must evaluate to true for this resource to be loaded (this is an optional feature and can be used to conditionally load resources based on certain conditions such as screen size or device type etc..) (optional)
 - FileType (string) – Type of file (does not affect how the file is loaded) (optional)
 - Variable (variable) – Name of the variable that the contents of the file should be written into once loaded (optional)
 - Script (script) - Name of the script that the contents of the file should be written into once loaded. This is used only to load a script resource with script (optional)
 - Converter (type) – How to convert the loaded data to text (html, hex, urlencoded). This is useful if you have data in a common web format such as url-encoded. Passing urlencoded will cause the data to be converted from url-encoded to plain text format (optional)
 - OnLoaded (actions list) - Actions list to call when the file is loaded
 - OnError (actions list) - Actions list to call when an error occurs

 The Files example has been provided to show how to load a file and reload its contents. Lets take a look at the XOML for this example:

 @code
<!-- Create a variable to load a file into -->
<Variable Name="FileContents" Type="string" />

<!-- Declare a file -->
<File Name="File1" Location="file1.txt" FileType="txt" Variable="FileContents"
Preload="true" />

<!-- Create a scene -->
<Scene Name="Scene1" Current="true" >

    <!-- Create a label to display our files contents -->
    <Label Font="serif" Size="200, 200" TextColour="255, 255, 128, 255"
	Background="Button1Brush" BackgroundColour="255, 80, 80, 255"
	Binding="[Text]FileContents" />

    <!-- Create a group of buttons to load 3 different files -->
    <Label Font="serif" Size="80, 50" Position="-100, 100" Text="Load File1"
	Background="Button1Brush" BackgroundColour="80, 80, 255, 255" OnTapped="Load">
        <Actions Name="Load">
            <Action Method="LoadFile" Param1="File1" Param2="true" Param3="file1.txt" />
        </Actions>
    </Label>
    <Label Font="serif" Size="80, 50" Position="0, 100" Text="Load File2"
	Background="Button1Brush" BackgroundColour="80, 80, 255, 255" OnTapped="Load">
        <Actions Name="Load">
            <Action Method="LoadFile" Param1="File1" Param2="true" Param3="file2.txt" />
        </Actions>
    </Label>
    <Label Font="serif" Size="80, 50" Position="100, 100" Text="Load File3"
	Background="Button1Brush" BackgroundColour="80, 80, 255, 255" OnTapped="Load">
        <Actions Name="Load">
            <Action Method="LoadFile" Param1="File1" Param2="true" Param3="file3.txt" />
        </Actions>
    </Label>

</Scene>
@endcode

 We begin this example by creating a variable called FileContents that we will later load our files contents into. Next we create a File that loads the contents of file1.txt and 
 writes it to the FileContents variable. Next we create a label that will sho the files contents due to the binding to the FileContents variable. Lastly we create 3 label buttons 
 that each call the LoadFile action when tapped. Each of the LoadFile actions load different files into the FileContents variable.


 */

class CzDataFile : public IzXomlResource
{
public:
	enum eDataFile_State
	{
		State_Invalid, 
		State_Loaded, 
	};

	enum eDataFile_Converter
	{
		Converter_None, 
		Converter_FromHTML, 
		Converter_FromHex, 
		Converter_FromURLEncoded, 
	};

	// Proprties
protected:
	eDataFile_State		State;				///< State of file
	CzString			Filename;			///< Name of file
	CzString			FileType;			///< Type of file (has not affect on how file is loaded)
	CzXomlVariable*		TargetVariable;		///< The file will be loaded into this variable if it is set
	CzScript*			TargetScript;		///< The file will be loaded into this script if it is set
	CzDataInput*		DataInput;			///< Input stream
	eDataFile_Converter Converter;			///< Conversion method used on input data
	CzEventManager*		EventsManager;		///< List of events that the file handles
public:
	eDataFile_State		getState() const					{ return State; }
	CzString&			getFilename()						{ return Filename; }
	CzDataInput*		getDataInput()						{ if (Load()) return DataInput; return NULL; }
	void				setFileType(const char* type)		{ FileType = type; }
	CzString&			getFileType()						{ return FileType; }
	void				setTargetVariable(const char* var);
	CzXomlVariable*		getTargetVariable()					{ return TargetVariable; }
	void				setConverter(eDataFile_Converter method)	{ Converter = method; }
	eDataFile_Converter getConverter() const				{ return Converter; }
	CzEventManager*		getEventsManager()					{ return EventsManager; }
	// Properties end

protected:
	CzFile*			File;				// File object
public:

	CzDataFile() : IzXomlResource(), File(NULL), State(State_Invalid), DataInput(NULL), TargetVariable(NULL), Converter(Converter_None), TargetScript(NULL), EventsManager(NULL) { setClassType("file"); }
	virtual ~CzDataFile();

	bool		Init(const char* filename, bool preload, bool blocking);
	bool		Load(bool blocking = true);						// Force load the file

	// Implementation of IzXomlResource interface
	int				LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Event handlers
	virtual void		ProcessEventActions(unsigned int event_name);
	virtual void		NotifyLoaded();
	virtual void		NotifyError();

	// Internal
	void			FinishLoad();									// Called back when aysnc loading is completed
};

/**
 @class	CzDataFileCreator

 @brief	Creates an instance of a file object.

 Enables a file to be created from XOML.

 */

class CzDataFileCreator : public IzXomlClassCreator
{
public:
	CzDataFileCreator()
	{
		setClassName("file");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzDataFile(); }
};

/// @}

#endif	// _CZ_FILE_H_
