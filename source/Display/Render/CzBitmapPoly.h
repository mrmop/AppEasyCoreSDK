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

#if !defined(_CCZ_BITMAP_POLY_H_)
#define _CCZ_BITMAP_POLY_H_

#include "CzBitmapSprite.h"
#include "CzGeometry.h"

//
//
//
// CzBitmapPoly - A BitmapSprite9 is a bitmapped visual representation of an on screen game object that uses patch-9 rendering
//
//
//
class CzBitmapPoly : public CzBitmapSprite
{
public:
	// Properties
protected:
	CzGeometry*			Geometry;
public:
	// Properties End

protected:
	void				RebuildUVList();
	void				TransformVertices();


public:
	CzBitmapPoly() : CzBitmapSprite()	{ setSpriteType(ST_9Patch); }
	virtual ~CzBitmapPoly() {}
	
	void	Init(CzGeometry* geometry);

	virtual bool	isClipped();
	bool	HitTest(float x, float y);
	bool	HitTestNoClip(float x, float y);

	void	Draw();

	bool	SimpleTestOverlap(CzSprite* other);
	bool	TestOverlap(CzSprite* other);

};





#endif // _CCZ_BITMAP_POLY_H_
