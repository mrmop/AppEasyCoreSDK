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

#if !defined(_CIZ_PLATFORM_FILE_H_)
#define _CIZ_PLATFORM_FILE_H_

#include "CzPlatform.h"

#define	PLATFORM_FILE		PLATFORM->getFile()

enum eCzFileSeekOrigin
{
	CzFileSeek_Set, 
	CzFileSeek_Current, 
	CzFileSeek_End, 
};

enum eCzFileError
{
	CzFileErrorNone, 
	CzFileErrorInvalidPath, 
	CzFileErrorOpenFailed, 
	CzFileErrorAlreadyOpen, 
	CzFileErrorEmptyMode, 
	CzFileErrorHttp, 
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IzPlatformFile
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IzPlatformFile
{
	// Properties
protected:
	bool		Initialised;
public:
	bool		isInitialised() const	{ return Initialised; }
	// Properties end
public:
	virtual int		Init() = 0;
	virtual void	Release() = 0;

	virtual CzFileHandle	Open(const char* filename, const char* mode) = 0;
	virtual CzFileHandle	OpenFromMemory(void* memory, uint num_bytes) = 0;
	virtual void			Close(CzFileHandle file) = 0;
	virtual bool			Seek(CzFileHandle file, int offset, eCzFileSeekOrigin origin) = 0;
	virtual uint			Read(CzFileHandle file, void* buffer, uint num_bytes) = 0;
	virtual uint			Write(CzFileHandle file, void* buffer, uint num_bytes) = 0;
	virtual bool			Exists(const char* filename) = 0;
	virtual bool			Delete(const char* filename) = 0;
	virtual int				getSize(CzFileHandle file) = 0;
	virtual const char*		getErrorString() = 0;
};

#endif	// _CIZ_PLATFORM_FILE_H_
