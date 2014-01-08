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

#if !defined(_CCZ_LOADER_JPEG_H_)
#define _CCZ_LOADER_JPEG_H_

#include "CzLoaders.h"
#include "CzImage.h"


//
//
//
//
// CzLoaderPNG - Decodes a PNG file
//
//
//
//
//
class CzLoaderJPEG : public IzLoader
{
public:
	// Properties
protected:
public:
	// Properties End
protected:

public:
	CzLoaderJPEG() :	IzLoader() {}
	virtual ~CzLoaderJPEG()
	{
	}

//	char*	Save(char* data, int width, int height, CzImage::eFormat, int compression);

};


#endif	// _CCZ_LOADER_JPEG_H_
