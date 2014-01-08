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

#include "CzLoaderJPEG.h"
#include "jpeglib.h"

//
//
//
//
// CzLoaderJPEG implementation
//
//
//
//

// Compression requires version 8 of libjpg
/*void CzLoaderJPEG_init_buffer(jpeg_compress_struct* cinfo) {}
bool CzLoaderJPEG_empty_buffer(jpeg_compress_struct* cinfo) { return TRUE; }
void CzLoaderJPEG_term_buffer(jpeg_compress_struct* cinfo) {}
char* CzLoaderJPEG::Save(char* data, int width, int height, CzImage::eFormat, int compression)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr       jerr;
	struct jpeg_destination_mgr dmgr;
 
	// Create our memory buffer to hold compressed JPEG
	JOCTET* out_buffer   = new JOCTET[width * height * bpp];
 
	// Set up manager
	dmgr.init_destination    = CzLoaderJPEG_init_buffer;
	dmgr.empty_output_buffer = CzLoaderJPEG_empty_buffer;
	dmgr.term_destination    = CzLoaderJPEG_term_buffer;
	dmgr.next_output_byte    = out_buffer;
	dmgr.free_in_buffer      = width * height * bpp;
 
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
 
	cinfo.dest = &dmgr;
	cinfo.image_width      = width;
	cinfo.image_height     = height;
	cinfo.input_components = 3;
	cinfo.in_color_space   = JCS_RGB;
 
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality (&cinfo, 90, true);
	jpeg_start_compress(&cinfo, true);

	uint8* converted = new uint8 [width * 3];
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
}
*/

