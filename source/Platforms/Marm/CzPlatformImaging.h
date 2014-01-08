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

#if !defined(_CCZ_PLATFORM_IMAGING_H_)
#define _CCZ_PLATFORM_IMAGING_H_

#include "IzPlatformImaging.h"
#include "CzImage.h"
#include "IwGx.h"

class CzPlatformImaging : public IzPlatformImaging
{
	// Properties
protected:
public:
	// Properties end
protected:
	CIwImage::Format	toMarmImageFormat(CzImage::eFormat format) const;
	CzImage::eFormat	toCzImageFormat(CIwImage::Format format) const;

public:
	// Init
	int				Init();
	void			Release();
	// Textures
	CzTexture		CreateTexture(void* memory_file, int memory_file_size);
	CzTexture		CreateTexture(void* pixels, int width, int height, int pitch, CzImage::eFormat format, bool modifiable);
	CzTexture		CreateTexture(CzTexture source, CzImage::eFormat format);
	void			ChangeTexture(CzTexture texture, void* pixels, CzImage::eFormat format);
	void			UploadTexture(CzTexture texture);
	void			DestroyTexture(CzTexture texture);
	void			setTextureFiltering(CzTexture texture, bool enable);
	void			getTextureInfo(CzTexture texture, CzTextureinfo& texture_info);
	void			SaveTextureAsPng(CzTexture texture, const char* filename);
	void			SaveTextureAsJpeg(CzTexture texture, const char* filename);
	

};


#endif	// _CCZ_PLATFORM_IMAGING_H_
