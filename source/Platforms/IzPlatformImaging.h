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

#if !defined(_CIZ_PLATFORM_IMAGING_H_)
#define _CIZ_PLATFORM_IMAGING_H_

#include "CzPlatform.h"
#include "CzImage.h"

#define	PLATFORM_IMAGING	PLATFORM->getImaging()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IzPlatformImaging
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IzPlatformImaging
{
	// Properties
protected:
	bool		Initialised;
public:
	bool		isInitialised() const	{ return Initialised; }
	// Properties end
public:
	// Init
	virtual int			Init() = 0;
	virtual void		Release() = 0;
	// Textures
	virtual CzTexture	CreateTexture(void* memory_file, int memory_file_size) = 0;
	virtual CzTexture	CreateTexture(void* pixels, int width, int height, int pitch, CzImage::eFormat format, bool modifiable) = 0;
	virtual CzTexture	CreateTexture(CzTexture source, CzImage::eFormat format) = 0;
	virtual void		ChangeTexture(CzTexture source, void* pixels, CzImage::eFormat format) = 0;
	virtual void		UploadTexture(CzTexture texture) = 0;
	virtual void		DestroyTexture(CzTexture texture) = 0;
	virtual void		setTextureFiltering(CzTexture texture, bool enable) = 0;
	virtual void		getTextureInfo(CzTexture texture, CzTextureinfo& texture_info) = 0;
	virtual void		SaveTextureAsPng(CzTexture texture, const char* filename) = 0;
	virtual void		SaveTextureAsJpeg(CzTexture texture, const char* filename) = 0;
};


#endif	// _CIZ_PLATFORM_IMAGING_H_
