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

#if !defined(_CCZ_PLATFORM_FILE_H_)
#define _CCZ_PLATFORM_FILE_H_

#include "IzPlatformFile.h"

class CzPlatformFile : public IzPlatformFile
{
	// Properties
protected:
public:
	// Properties end
public:
	int		Init();
	void	Release();

	CzFileHandle	Open(const char* filename, const char* mode);
	CzFileHandle	OpenFromMemory(void* memory, uint num_bytes);
	void			Close(CzFileHandle file);
	bool			Seek(CzFileHandle file, int offset, eCzFileSeekOrigin origin);
	uint			Read(CzFileHandle file, void* buffer, uint num_bytes);
	uint			Write(CzFileHandle file, void* buffer, uint num_bytes);
	bool			Exists(const char* filename);
	bool			Delete(const char* filename);
	int				getSize(CzFileHandle file);
	const char*		getErrorString();
};


#endif	// _CCZ_PLATFORM_FILE_H_
