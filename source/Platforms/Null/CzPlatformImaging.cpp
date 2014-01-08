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

#include "CzPlatformImaging.h"


//
// Marmalade Imaging system
//
int		CzPlatformImaging::Init()
{
	Initialised = true;

	return 1;
}

void	CzPlatformImaging::Release()
{
}

CzTexture CzPlatformImaging::CreateTexture(void* memory_file, int memory_file_size)
{
	CzFile file;
	if (file.Open(memory_file, memory_file_size))
	{
		return (CzTexture)NULL;
	}

	return NULL;
}

CzTexture CzPlatformImaging::CreateTexture(void* pixels, int width, int height, int pitch, CzImage::eFormat format, bool modifiable)
{
	if (pixels == NULL || width <= 0 || height <= 0)
		return false;

	return NULL;
}

void CzPlatformImaging::ChangeTexture(CzTexture texture, void* pixels, CzImage::eFormat format)
{
}

CzTexture CzPlatformImaging::CreateTexture(CzTexture source, CzImage::eFormat format)
{
	return (CzTexture)NULL;
}

void CzPlatformImaging::UploadTexture(CzTexture texture)
{
}

void CzPlatformImaging::DestroyTexture(CzTexture texture)
{
}

void CzPlatformImaging::setTextureFiltering(CzTexture texture, bool enable)
{
}

void CzPlatformImaging::getTextureInfo(CzTexture texture, CzTextureinfo& texture_info)
{
}

void CzPlatformImaging::SaveTextureAsPng(CzTexture texture, const char* filename)
{
}

void CzPlatformImaging::SaveTextureAsJpeg(CzTexture texture, const char* filename)
{
}


