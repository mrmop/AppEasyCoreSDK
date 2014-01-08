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

#include "Marm/CzPlatformImaging.h"


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

CIwImage::Format CzPlatformImaging::toMarmImageFormat(CzImage::eFormat format) const
{
	switch (format)
	{
		case CzImage::Format_RGB332:
			return CIwImage::RGB_332;
			break;
		case CzImage::Format_RGB565:
			return CIwImage::RGB_565;
			break;
		case CzImage::Format_RGB888:
			return CIwImage::RGB_888;
			break;
		case CzImage::Format_RGBA4444:
			return CIwImage::RGBA_4444;
			break;
		case CzImage::Format_RGBA5551:
			return CIwImage::RGBA_5551;
			break;
		case CzImage::Format_RGBA6666:
			return CIwImage::RGBA_6666;
			break;
		case CzImage::Format_RGBA8888:
			return CIwImage::RGBA_8888;
			break;
	}

	return CIwImage::FORMAT_UNDEFINED;
}

CzImage::eFormat CzPlatformImaging::toCzImageFormat(CIwImage::Format format) const
{
	switch (format)
	{
		case CIwImage::RGB_332:
			return CzImage::Format_RGB332;
			break;
		case CIwImage::RGB_565:
			return CzImage::Format_RGB565;
			break;
		case CIwImage::RGB_888:
			return CzImage::Format_RGB888;
			break;
		case CIwImage::RGBA_4444:
			return CzImage::Format_RGBA4444;
			break;
		case CIwImage::RGBA_5551:
			return CzImage::Format_RGBA5551;
			break;
		case CIwImage::RGBA_6666:
			return CzImage::Format_RGBA6666;
			break;
		case CIwImage::RGBA_8888:
			return CzImage::Format_RGBA8888;
			break;
	}

	return CzImage::Format_Invalid;
}

CzTexture CzPlatformImaging::CreateTexture(void* memory_file, int memory_file_size)
{
	CzFile file;
	if (file.Open(memory_file, memory_file_size))
	{
		// Load the image
		CIwImage* image = new CIwImage();
		image->ReadFile((s3eFile*)file.getFileHandle());

		CIwTexture* texture = new CIwTexture();
		texture->CopyFromImage(image);

		delete image;

		return (CzTexture)texture;
	}

	return NULL;
}

CzTexture CzPlatformImaging::CreateTexture(void* pixels, int width, int height, int pitch, CzImage::eFormat format, bool modifiable)
{
	if (pixels == NULL || width <= 0 || height <= 0)
		return false;

	CIwImage::Format f = toMarmImageFormat(format);
	if (f == CIwImage::FORMAT_UNDEFINED)
		return NULL;

	CIwTexture* texture = new CIwTexture();
	texture->SetMipMapping(false);
	texture->SetModifiable(modifiable);
	texture->CopyFromBuffer(width, height, f, pitch, (uint8*)pixels, NULL);

	return (CzTexture)texture;
}

void CzPlatformImaging::ChangeTexture(CzTexture texture, void* pixels, CzImage::eFormat format)
{
	CIwImage::Format f = toMarmImageFormat(format);
	if (f == CIwImage::FORMAT_UNDEFINED)
		return;

	CIwTexture* t = static_cast<CIwTexture*>(texture);
	t->ChangeTexels((uint8*)pixels, f);
}

CzTexture CzPlatformImaging::CreateTexture(CzTexture source, CzImage::eFormat format)
{
	CIwImage::Format f = toMarmImageFormat(format);
	if (f == CIwImage::FORMAT_UNDEFINED)
		return NULL;
	CIwTexture* t = static_cast<CIwTexture*>(source);

	CIwImage* image = new CIwImage();
	image->SetFormat(f);
	image->SetWidth(t->GetWidth());
	image->SetHeight(t->GetHeight());
	t->GetImage().ConvertToImage(image);

	CIwTexture* texture = new CIwTexture();
	texture->SetMipMapping(t->GetMipMapping());
	texture->SetFiltering(t->GetFiltering());
	texture->SetModifiable(t->GetModifiable());
	texture->CopyFromImage(image);

	delete image;

	return (CzTexture)texture;
}

void CzPlatformImaging::UploadTexture(CzTexture texture)
{
	CIwTexture* t = static_cast<CIwTexture*>(texture);
	t->Upload();
}

void CzPlatformImaging::DestroyTexture(CzTexture texture)
{
	CIwTexture* t = static_cast<CIwTexture*>(texture);

	delete t;
}

void CzPlatformImaging::setTextureFiltering(CzTexture texture, bool enable)
{
	CIwTexture* t = static_cast<CIwTexture*>(texture);
	t->SetFiltering(enable);
}

void CzPlatformImaging::getTextureInfo(CzTexture texture, CzTextureinfo& texture_info)
{
	CIwTexture* t = static_cast<CIwTexture*>(texture);
	texture_info.Width = t->GetWidth();
	texture_info.Height = t->GetHeight();
	texture_info.Format = toCzImageFormat(t->GetFormat());
	texture_info.Filter = t->GetFiltering();
}

void CzPlatformImaging::SaveTextureAsPng(CzTexture texture, const char* filename)
{
	CIwTexture* t = static_cast<CIwTexture*>(texture);
	t->GetImage().SavePng(filename);
}

void CzPlatformImaging::SaveTextureAsJpeg(CzTexture texture, const char* filename)
{
	CIwTexture* t = static_cast<CIwTexture*>(texture);
	t->GetImage().SaveJpg(filename);
}


