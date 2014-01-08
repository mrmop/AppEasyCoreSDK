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

#include "CzPlatformFile.h"


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
	CzFileHandle fh = NULL;
	if (fh == NULL)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "CzPlatformFile::Open: Could not open file - ", filename);

	return fh;
}

CzFileHandle	CzPlatformFile::OpenFromMemory(void* memory, uint num_bytes)
{
	return NULL;
}

void	CzPlatformFile::Close(CzFileHandle file)
{
}

bool	CzPlatformFile::Seek(CzFileHandle file, int offset, eCzFileSeekOrigin origin)
{
	return false;
}

uint	CzPlatformFile::Read(CzFileHandle file, void* buffer, uint num_bytes)
{
	return 0;
}

uint	CzPlatformFile::Write(CzFileHandle file, void* buffer, uint num_bytes)
{
	return 0;
}

bool	CzPlatformFile::Exists(const char* filename)
{
	return false;
}

bool	CzPlatformFile::Delete(const char* filename)
{
	return false;
}

int		CzPlatformFile::getSize(CzFileHandle file)
{
	return 0;
}

const char* CzPlatformFile::getErrorString()
{
	return NULL;
}




