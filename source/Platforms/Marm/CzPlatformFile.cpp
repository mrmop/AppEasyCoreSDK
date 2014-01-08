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

#include "Marm/CzPlatformFile.h"
#include "s3eFile.h"


//
// Marmalade File system
//
int		CzPlatformFile::Init()
{
	Initialised = true;

	return 1;
}

void	CzPlatformFile::Release()
{
}

CzFileHandle	CzPlatformFile::Open(const char* filename, const char* mode)
{
	CzFileHandle fh = (CzFileHandle)s3eFileOpen(filename, mode);
	if (fh == NULL)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "CzPlatformFile::Open: Could not open file - ", filename);

	return fh;
}

CzFileHandle	CzPlatformFile::OpenFromMemory(void* memory, uint num_bytes)
{
	return (CzFileHandle)s3eFileOpenFromMemory(memory, num_bytes);
}

void	CzPlatformFile::Close(CzFileHandle file)
{
	s3eFileClose((s3eFile*)file);
}

bool	CzPlatformFile::Seek(CzFileHandle file, int offset, eCzFileSeekOrigin origin)
{
	s3eFileSeekOrigin o = S3E_FILESEEK_CUR;
	if (origin == CzFileSeek_Set)
		o = S3E_FILESEEK_SET;
	else
	if (origin == CzFileSeek_End)
		o = S3E_FILESEEK_END;

	return s3eFileSeek((s3eFile*)file, offset, o) == S3E_RESULT_SUCCESS;
}

uint	CzPlatformFile::Read(CzFileHandle file, void* buffer, uint num_bytes)
{
	return s3eFileRead(buffer, num_bytes, 1, (s3eFile*)file);
}

uint	CzPlatformFile::Write(CzFileHandle file, void* buffer, uint num_bytes)
{
	return s3eFileWrite(buffer, num_bytes, 1, (s3eFile*)file);
}

bool	CzPlatformFile::Exists(const char* filename)
{
	return s3eFileCheckExists(filename) == S3E_TRUE;
}

bool	CzPlatformFile::Delete(const char* filename)
{
	return s3eFileDelete(filename) == S3E_RESULT_SUCCESS;
}

int		CzPlatformFile::getSize(CzFileHandle file)
{
	return s3eFileGetSize((s3eFile*)file);
}

const char* CzPlatformFile::getErrorString()
{
	s3eFileGetError();
	return s3eFileGetErrorString();
}




