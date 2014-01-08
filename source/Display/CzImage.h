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

#if !defined(_CCZ_IMAGE_H_)
#define _CCZ_IMAGE_H_

#include "CzUtil.h"
#include "CzXoml.h"
#include "CzFile.h"

class CzXomlResourceManager;

struct CzTextureinfo
{
	int					Width, Height;
	int					Format;
	bool				Filter;

	CzTextureinfo();
};


//
//
//
//
// CzImage - Represents a bitmapped image
//
// Note that images can be either loaded on demand (loaded on first call to getTexture() or pre loaded by calling Load()
//
//
//
//
class CzImage : public IzXomlResource
{
public:
	enum eState
	{
		State_Invalid, 
		State_Loading, 
		State_Loaded, 
		State_Uploaded, 
	};

	enum eFormat
	{
		Format_Invalid, 
		Format_RGB332, 
		Format_RGB565, 
		Format_RGB888, 
		Format_RGBA4444, 
		Format_RGBA6666, 
		Format_RGBA5551, 
		Format_RGBA8888, 
	};

	// Properties
protected:
	CzTexture				Texture;			// Texture
	CzTextureinfo			TextureInfo;		// Details of texture
	eState					State;				// State of image
	bool					Filter;				// Value to set filtering to
	bool					FilterSet;			// True if the filter was set
	eFormat					ToFormat;			// Format to convert texture to
	bool					ToFormatSet;		// If set then format conversion will take place
public:
	eState					getState() const					{ return State; }
	CzTexture				getTexture();
	int						getWidth() const;
	int						getHeight() const;
	CzImage*				getCopy();
	void					setFilter(bool enable);
	bool					isFilter() const;
	bool					isFilterSet() const					{ return FilterSet; }
	void					setToFormat(eFormat format)			{ ToFormat = format; ToFormatSet = true; }
	eFormat					getFormat() const;
	// Properties End
protected:
	CzFile*					File;				// File object (if image is file based)
	bool					DecompressJPEG(char* jpeg_data, int jpeg_data_size);

public:
	CzImage() :	IzXomlResource(), File(NULL), State(State_Invalid), ToFormat(Format_RGBA5551), ToFormatSet(false), Filter(true), FilterSet(false), Texture(NULL) { setClassType("image"); }
	virtual ~CzImage();

/*	void			Init(const char* ResourceName, CIwResGroup* resource_group)		// Init an image from an image located within a resource group (image is not loaded)
	{
		setName(ResourceName);
		ResourceGroup = resource_group;
	}*/

	bool			Init(void* memory_file, int memory_file_size);	// Init an image from a memory based file (image is loaded)
	void			Init(const char* filename);						// Init an image from a file (image is not loaded)
	bool			Init(void* pixels, int width, int height, int pitch, eFormat format, bool modifiable);	// Init an image from raw data
	void			Release();

	bool			Load(bool blocking = true);								// Load the image
	bool			Reload(const char* filename, bool blocking = true);		// Reload the image

	// Utility
	void			ChangePixels(void* data, CzImage::eFormat format);
	CzTexture		ConvertToFormat(CzImage& source, CzImage::eFormat format);
    void			SavePng(const char* filename);
    void			SaveJpg(const char* filename, int quality = 100);

	// Implementation of IzXomlClass interface
	int				LoadFromXoml(IzXomlResource* parebt, bool load_children, CzXmlNode* node);

	// Internal
	void			FinishLoad();									// Called back when aysnc loading is completed
};

//
//  CzImageCreator - Creates an instance of an image object
//
class CzImageCreator : public IzXomlClassCreator
{
public:
	CzImageCreator()
	{
		setClassName("image");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzImage(); }
};

//
//
//
//
//  Helper macros
//
//
//
//

#define CZ_NEW_IMAGE(image, name, location, filter)			\
	CzImage* image = new CzImage();							\
	image->setName(name);									\
	image->Init(location);									\
	image->setFilter(filter);								\
	image->Load();



#endif	// _CCZ_IMAGE_H_
