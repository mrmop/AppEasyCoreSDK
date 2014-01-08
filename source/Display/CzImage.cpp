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

#include "IzPlatformImaging.h"
#include "CzImage.h"
#include "CzFile.h"
#include "CzString.h"
#include "CzScene.h"
#include "CzResources.h"
#include "CzActor.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"
//#include "png.h"
//#include "pngstruct.h"

CzTextureinfo::CzTextureinfo()
{
	Width = Height = 0;
	Format = CzImage::Format_Invalid;
	Filter = false;
}

//
//
//
//
// CzImage implementation
//
//
//
//
int CzImage::getWidth() const					{ return TextureInfo.Width; }
int CzImage::getHeight() const					{ return TextureInfo.Height; }
CzImage::eFormat CzImage::getFormat() const		{ return (CzImage::eFormat)TextureInfo.Format; }
bool CzImage::isFilter() const					{ return TextureInfo.Filter; }

CzImage::~CzImage()
{
	if (Texture != NULL)
		PLATFORM_IMAGING->DestroyTexture(Texture);

	SAFE_DELETE(File)
}

CzImage* CzImage::getCopy()
{
	if (Texture == NULL)
		return NULL;

	CzImage* image = new CzImage();
	image->NameHash = NameHash;
	image->State = State;
//	image->ResourceGroup = ResourceGroup;
//	image->Width = Width;
//	image->Height = Height;
//	image->Image2D = Iw2DCreateImage(Image2D->GetMaterial()->GetTexture()->GetImage());

	return image;
}

CzTexture CzImage::getTexture()
{
	if (State == State_Loading)
		return NULL;

	if (Load())
	{
		if (Texture != NULL)
			return Texture;
	}

	return NULL;
}

void CzImage::setFilter(bool enable)
{
	if (Texture != NULL)
		PLATFORM_IMAGING->setTextureFiltering(Texture, enable);
	FilterSet = true;
}

int32 CzImage_FileRetrievedAsyncCallback(void* caller, void* data)
{
	CzImage* image = (CzImage*)data;
	image->FinishLoad();

	return 0;
}

bool CzImage::Load(bool blocking)
{
	// If already loaded return true
	if (Texture != NULL)
		return true;

	// Image is file based
	if (File != NULL)
	{
		State = State_Loading;
		File->setFileAvailableCallback(CzImage_FileRetrievedAsyncCallback, this);
		if (File->Open(NULL, "rb", blocking))
			return true;
		else
			return false;
	}

/*	if (ResourceGroup != NULL)
	{
		// Create image from the resource group
		IwGetResManager()->SetCurrentGroup(ResourceGroup);
		Image2D = Iw2DCreateImageResource(NameHash);
		if (Image2D != NULL)
		{
			Width = Image2D->GetWidth();
			Height = Image2D->GetHeight();
			State = State_Loaded;
			return true;
		}
	}*/

	return false;
}

bool CzImage::Reload(const char* filename, bool blocking)
{
	Release();
	File = new CzFile();
	File->setFilename(filename);

	return Load(blocking);
}

void CzImage::FinishLoad()
{
	if (File != NULL)
	{
		if (CzFile::isHttp(File->getFilename().c_str(), File->getFilename().getLength()))
		{
			if (File->isFileAvailable() && File->getError() == CzFileErrorNone)
			{
				Init(File->getContent(), File->getContentLength());
			}
		}
		else
		{
			char* data = new char [File->getFileSize()];
			File->Read(data, File->getFileSize());
			Init(data, File->getFileSize());
			delete data;
		}
		State = State_Loaded;
		SAFE_DELETE(File)
	}
}

void CzImage::Init(const char* filename)
{
	File = new CzFile();
	File->setFilename(filename);
}

bool CzImage::Init(void* memory_file, int memory_file_size)
{
	Texture = PLATFORM_IMAGING->CreateTexture(memory_file, memory_file_size);
	if (Texture == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "CzImage::Init() - Could not create image from memory file!", DebugInfo.c_str());
		return false;
	}

	PLATFORM_IMAGING->getTextureInfo(Texture, TextureInfo);

	// if required format is different then convert it
	if (ToFormatSet && TextureInfo.Format != ToFormat)
	{
		CzTexture t = PLATFORM_IMAGING->CreateTexture(Texture, ToFormat);
		PLATFORM_IMAGING->DestroyTexture(Texture);
		Texture = t;
		PLATFORM_IMAGING->getTextureInfo(Texture, TextureInfo);
	}

	if (FilterSet)
		setFilter(Filter);

#if defined(_DEBUG)
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "CzImage::Init() - Size = ", CzString(memory_file_size).c_str());
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "CzImage::Init() - Width = ", CzString(TextureInfo.Width).c_str());
	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "CzImage::Init() - Height = ", CzString(TextureInfo.Height).c_str());
#endif	// _DEBUG

	// Sanity check
	if (TextureInfo.Width <= 0 || TextureInfo.Height <= 0 || TextureInfo.Width > 32768 || TextureInfo.Height > 32768)
		return false;

	State = State_Loaded;

	PLATFORM_IMAGING->UploadTexture(Texture);

	return true;
}

bool CzImage::Init(void* pixels, int width, int height, int pitch, eFormat format, bool modifiable)
{
	if (pixels == NULL || width <= 0 || height <= 0)
		return false;

	Texture = PLATFORM_IMAGING->CreateTexture(pixels, width, height, pitch, format, modifiable);
	if (Texture == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "CzImage::Init() - Could not create image!", DebugInfo.c_str());
		return false;
	}
	PLATFORM_IMAGING->getTextureInfo(Texture, TextureInfo);

	State = State_Loaded;

	PLATFORM_IMAGING->UploadTexture(Texture);

	return true;
}

void CzImage::Release()
{
	if (Texture != NULL)
	{
		PLATFORM_IMAGING->DestroyTexture(Texture);
		Texture = NULL;
	}
	SAFE_DELETE(File)
	State = State_Invalid;
}

void CzImage::ChangePixels(void* data, CzImage::eFormat format)
{
	PLATFORM_IMAGING->ChangeTexture(Texture, data, format);
}

CzTexture CzImage::ConvertToFormat(CzImage& source, CzImage::eFormat format)
{
	// if required format is different then convert it
	if (TextureInfo.Format != format)
		return PLATFORM_IMAGING->CreateTexture(source.getTexture(), ToFormat);

	return NULL;
}

void CzImage::SavePng(const char* filename)
{
	if (Texture != NULL)
		PLATFORM_IMAGING->SaveTextureAsPng(Texture, filename);
}

void CzImage::SaveJpg(const char* filename, int quality)
{
	if (Texture != NULL)
		PLATFORM_IMAGING->SaveTextureAsJpeg(Texture, filename);
}


/*
// Compression requires version 8 of libjpg
void CzImage_init_buffer(jpeg_compress_struct* cinfo) {}
boolean CzImage_empty_buffer(jpeg_compress_struct* cinfo) { return TRUE; }
void CzImage_term_buffer(jpeg_compress_struct* cinfo) {}

char* CzImage::CreateJPEG()
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr       jerr;
	struct jpeg_destination_mgr dmgr;
 
	CIwTexture* texture = Image2D->GetMaterial()->GetTexture();
	int bpp = (texture->GetDepth() + 7) >> 3;
	CIwImage::Format format = texture->GetFormat();
	uint8* texels = texture->GetTexels();

	// Create our memory buffer to hold compressed JPEG
	JOCTET * out_buffer   = new JOCTET[Width * Height * 3];
 
	// Set up manager
	dmgr.init_destination    = CzImage_init_buffer;
	dmgr.empty_output_buffer = CzImage_empty_buffer;
	dmgr.term_destination    = CzImage_term_buffer;
	dmgr.next_output_byte    = out_buffer;
	dmgr.free_in_buffer      = Width * Height * bpp;
 
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
 
	cinfo.dest = &dmgr;
	cinfo.image_width      = Width;
	cinfo.image_height     = Height;
	cinfo.input_components = 3;
	cinfo.in_color_space   = JCS_RGB;
 
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality (&cinfo, 90, true);
	jpeg_start_compress(&cinfo, true);

	uint8* converted = new uint8 [Width * 3];
	while (cinfo.next_scanline < cinfo.image_height)
	{
		JSAMPROW row_pointer;
		if (format == CIwImage::Format::RGB_888)
			row_pointer = (JSAMPROW)&texels[cinfo.next_scanline * Width * bpp];
		else
		{
			row_pointer = (JSAMPROW)converted;
			if (format == CIwImage::Format::RGB_565)
			{
				uint8* in_data = &texels[cinfo.next_scanline * Width * bpp];
				uint8* out_data = &converted[cinfo.next_scanline * Width * bpp];
				for (int t = 0; t < Width; t++)
				{
					int rgb = *in_data++;
					*out_data++ = (rgb & 0xf800) >> 8;
					*out_data++ = (rgb & 0x07e0) >> 3;
					*out_data++ = (rgb & 0x001f) << 3;
				}
			}
		}
		jpeg_write_scanlines(&cinfo, &row_pointer, 1);
	}
	jpeg_finish_compress(&cinfo);

	delete converted;

	return (char*)out_buffer;
}*/

/*struct CzImage_png_encode
{
  char*		buffer;
  size_t	size;
};

void CzImage_png_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	CzImage_png_encode* pd = (CzImage_png_encode*)png_ptr->io_ptr;
	memcpy(pd->buffer + pd->size, data, length);

	pd->size += length;
}

void CzImage_png_flush(png_structp png_ptr)
{
}

uint8* CzImage::CreatePNG(int& size)
{
	int			width = Width;
	int			height = Height;
	uint8*		buffer = (uint8*)Image2D->GetMaterial()->GetTexture()->GetTexels();
	int			code = 0;
	FILE*		fp = NULL;
	png_structp png_ptr = NULL;
	png_infop	info_ptr = NULL;
	png_bytep	row = NULL;
	CzImage_png_encode	png_state;
	bool		error = false;
	CIwImage::Format format = Image2D->GetMaterial()->GetTexture()->GetFormat();

	if (format != CIwImage::RGB_565 && format != CIwImage::RGB_888 && format != CIwImage::RGBA_8888)
		return NULL;

	int bbp = 2;
	if (format == CIwImage::RGB_888)
		bbp = 3;
	else
	if (format == CIwImage::RGBA_8888)
		bbp = 4;

	png_state.buffer = new char [width * height * bbp];
	png_state.size = 0;
	if (png_state.buffer == NULL)
	{
		goto cleanup;
		error = true;
	}

	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
	{
		goto cleanup;
		error = true;
	}

	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		goto cleanup;
		error = true;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		goto cleanup;
		error = true;
	}

	// Set write function and data buffer
	png_set_write_fn(png_ptr, &png_state, CzImage_png_write_data, CzImage_png_flush);
	png_init_io(png_ptr, (png_FILE_p)&png_state);

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_ptr, info_ptr);

	// Allocate memory for one row
	row = (png_bytep)new char[3 * width * sizeof(png_byte)];

	// Write image data
	int x, y;
	for (y = 0; y < height; y++)
	{
		png_byte* out_data = row;
		if (format == CIwImage::RGB_565)
		{
			uint16* in_data = (uint16*)&buffer[y * width * 2];
			for (x = 0; x < width; x++)
			{
				uint16 rgb = *in_data++;
				*out_data++ = (rgb & 0xf800) >> 8;
				*out_data++ = (rgb & 0x07e0) >> 3;
				*out_data++ = (rgb & 0x001f) << 3;
			}
		}
		else
		if (format == CIwImage::RGB_888)
		{
			uint8* in_data = (uint8*)&buffer[y * width * 3];
			for (x = 0; x < width; x++)
			{
				*out_data++ = *in_data++;
				*out_data++ = *in_data++;
				*out_data++ = *in_data++;
			}
		}
		else
		if (format == CIwImage::RGBA_8888)
		{
			uint8* in_data = (uint8*)&buffer[y * width * 4];
			for (x = 0; x < width; x++)
			{
				*out_data++ = *in_data++;
				*out_data++ = *in_data++;
				*out_data++ = *in_data++;
				in_data++;
			}
		}
		png_write_row(png_ptr, row);
	}

	// End write
	png_write_end(png_ptr, NULL);

	// Cleanup
cleanup:
	if (info_ptr != NULL)
		png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL)
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if (row != NULL)
		delete row;
	if (error && png_state.buffer != NULL)
	{
		delete png_state.buffer;
		png_state.buffer = NULL;
		png_state.size = 0;
	}

	size = png_state.size;
	return (uint8*)png_state.buffer;
}*/

int CzImage::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Process image attributes
	CzString*	name = NULL;
	CzString*	location = NULL;
	bool		preload = true;
	bool		blocking = false;
	CzString*	condition = NULL;

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
		if (name_hash == CzHashes::Format_Hash)
		{
			ToFormatSet = true;
			unsigned int format_hash = (*it)->getValue().getHash();
			if (format_hash == CZ_HASH("RGB_565"))
				ToFormat = Format_RGB565;
			else
			if (format_hash == CZ_HASH("RGBA_4444"))
				ToFormat = Format_RGBA4444;
			else
			if (format_hash == CZ_HASH("RGBA_5551"))
				ToFormat = Format_RGBA5551;
			else
			if (format_hash == CZ_HASH("RGB_888"))
				ToFormat = Format_RGB888;
			else
			if (format_hash == CZ_HASH("RGBA_6666"))
				ToFormat = Format_RGBA6666;
			else
			if (format_hash == CZ_HASH("RGB_332"))
				ToFormat = Format_RGB332;
			else
			if (format_hash == CZ_HASH("RGBA_8888"))
				ToFormat = Format_RGBA8888;
			else
			{
				ToFormatSet = false;
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Image - Invalid texture format set - ", (*it)->getValue().c_str(), DebugInfo.c_str());
			}
		}
		else
		if (name_hash == CzHashes::Filter_Hash)
		{
			Filter = (*it)->getValueAsBool();
			FilterSet = true;
		}
	}

	if (location == NULL || name == NULL)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Image - An Image requires a location and a name to be specified", DebugInfo.c_str());

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
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Image - Condition variable not found - ", condition->c_str(), DebugInfo.c_str());
#endif // _DEBUG
	}

	if (location != NULL)
	{
		// Check to see if image is located externally
		if (CzFile::isHttp(location->c_str(), location->getLength()))
		{
			Init(location->c_str());
		}
		else
		if (CzFile::isFile(location->c_str(), location->getLength()))
		{
			Init(location->c_str());
		}
		else
		{
			CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Image - Marmalade resource groups no longer supported", DebugInfo.c_str());
			return 0;
/*			// Find resource group
			CzResourceGroup* group = NULL;
			if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
				group = (CzResourceGroup*)scene->getResourceManager()->findResource(location->c_str(), CzHashes::ResourceGroup_Hash);
			else
				group = (CzResourceGroup*)CZ_GLOBAL_RESOURCES->getResourceManager()->findResource(location->c_str(), CzHashes::ResourceGroup_Hash);

			if (group != NULL)
				Init(name->c_str(), group->getResourceGroup());
			else
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "XOML - Invalid image resource group name - ", location->c_str());
				return false;
			}*/
		}
		if (preload)
			Load(blocking);
	}

	// If we are declared inside a scene then image is local to the scene
	if (scene != NULL)
		return scene->getResourceManager()->addResource(this) ? 1 : 0;
	else
		return CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this) ? 1 : 0;

	return 1;
}


