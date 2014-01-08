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

#if !defined(_CCZ_BRUSH_H_)
#define _CCZ_BRUSH_H_

#include "CzUtil.h"
#include "CzXoml.h"
#include "CzImage.h"

//
//
//
//
// IzBrush - Base class for all brushes
//
//
//
//
class IzBrush : public IzXomlResource
{
public:
	enum eBrushType
	{
		BT_None, 
		BT_Solid, 
		BT_Gradient, 
		BT_Image, 
		BT_9Patch, 
	};

	// Properties
protected:
	eBrushType			BrushType;
public:
	void				setBrushType(eBrushType type)		{ BrushType = type; }
	eBrushType			getBrushType() const				{ return BrushType; }
	// Properties End
protected:

public:
	IzBrush() : IzXomlResource()	{ setClassType("brush"); setBrushType(BT_None); }
	~IzBrush() {}

	// Implementation of IzXomlResource interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);
};

//
//
//
//
// CzBrushSolid - Represents a solid colour brush
//
//
//
//
class CzBrushSolid : public IzBrush
{
	// Properties
protected:
	CzColour		Colour;
public:
	void			setColour(uint8 r, uint8 g, uint8 b, uint8 a)	{ Colour.r = r; Colour.g = g; Colour.b = b; Colour.a = a; }
	void			setColour(const CzColour& colour)				{ Colour = colour; }
	CzColour		getColour() const								{ return Colour; }
	// Properties End
protected:

public:
	CzBrushSolid() : IzBrush()	{  setBrushType(BT_Solid); }

};

//
//
//
//
// CzBrushGradient - Represents a gradient colour brush
//
//
//
//
struct CzBrushGradientStop
{
	CzColour		Colour;
	float			offset;
};

class CzBrushGradient : public IzBrush
{
public:
	// Public access to gradient stops iteration
	typedef CzList<CzBrushGradientStop*>::iterator _Iterator;
	_Iterator					begin() { return Stops.begin(); }
	_Iterator					end() { return Stops.end(); }
	// Properties
protected:
	CzList<CzBrushGradientStop*>	Stops;
public:
	void						addStop(CzBrushGradientStop* stop)	{ Stops.push_back(stop); }
	// Properties End
protected:

public:
	CzBrushGradient() : IzBrush()	{ setBrushType(BT_Gradient); }

};

//
//
//
//
// CzBrushImage - Represents an image brush
//
//
//
//
class CzBrushImage : public IzBrush
{
	// Properties
protected:
	CzImage*			Image;					// Image used to render this brush
	CzIRect				SrcRect;				// Rectangular area od source image used to ggenerate UV coordinates
	CzVec2*				UVList;					// UVList, if present then SrcRect is ignored
public:
	void				setImage(CzImage* image)				{ Image = image; }
	CzImage*			getImage()								{ return Image; }
	void				setSrcRect(int x, int y, int w, int h)	{ SrcRect.x = x; SrcRect.y = y; SrcRect.w = w; SrcRect.h = h; }
	void				setSrcRect(const CzIRect& rect)			{ SrcRect = rect; }
	CzIRect				getSrcRect() const						{ return SrcRect; }
	void				setUVList(CzVec2* uvs)					{ SAFE_DELETE_ARRAY(UVList); UVList = uvs; }
	CzVec2*				getUVList()								{ return UVList; }
	// Properties End
protected:

public:
	CzBrushImage() : IzBrush(), Image(NULL), UVList(NULL) { setBrushType(BT_Image); }
	~CzBrushImage()
	{
		SAFE_DELETE_ARRAY(UVList);
	}

};

//
//
//
//
// CzBrushImage9 - Represents a 9 patch style image brush
//
//
//
//
class CzBrushImage9 : public CzBrushImage
{
	// Properties
protected:
	CzIRect				ScaleArea;
public:
	void				setScalableArea(int x, int y, int w, int h)	{ ScaleArea.x = x; ScaleArea.y = y; ScaleArea.w = w; ScaleArea.h = h; }
	void				setScalableArea(const CzIRect &rc)			{ ScaleArea = rc; }
	CzIRect				getScalableArea() const						{ return ScaleArea; }
	// Properties End
protected:

public:
	CzBrushImage9() : CzBrushImage(), ScaleArea(0, 0, 0, 0)	{ setBrushType(BT_9Patch); }

};



//
//  CzBrushCreator - Creates an instance of a brush object
//
class CzBrushCreator : public IzXomlClassCreator
{
public:
	CzBrushCreator()
	{
		setClassName("brush");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new IzBrush(); }
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

#define CZ_NEW_IMAGE_BRUSH(brush, name, image, src_rect)	\
	CzBrushImage* brush = new CzBrushImage();				\
	brush->setName(name);									\
	brush->setImage(image);									\
	brush->setSrcRect(src_rect);
	
#define CZ_NEW_IMAGE9_BRUSH(brush, name, image, src_rect, scale_area)	\
	CzBrushImage9* brush = new CzBrushImage9();							\
	brush->setName(name);												\
	brush->setImage(image);												\
	brush->setSrcRect(src_rect);										\
	brush->setScalableArea(scale_area);



#endif	// _CCZ_BRUSH_H_
