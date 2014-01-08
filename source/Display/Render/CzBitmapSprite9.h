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

#if !defined(_CCZ_BITMAP_SPRITE9_H_)
#define _CCZ_BITMAP_SPRITE9_H_

#include "CzBitmapSprite.h"

//
//
//
// CzBitmapSprite0 - A BitmapSprite9 is a bitmapped visual representation of an on screen game object that uses patch-9 rendering
//
//
//
class CzBitmapSprite9 : public CzBitmapSprite
{
public:
	static uint16		Sprite9Indices[36];
	static uint16		Sprite9ClipIndices[4];

	// Properties
protected:
public:
	// Properties End

protected:
	void				RebuildUVList();
	void				TransformVertices();


public:
	CzBitmapSprite9() : CzBitmapSprite()	{ setSpriteType(ST_9Patch); }
	virtual ~CzBitmapSprite9() {}
	
	void	Init(int vertex_count = 16);

	virtual bool	isClipped();
	bool	HitTest(float x, float y);
	bool	HitTestNoClip(float x, float y);

	bool	isOutsideFocusRange(float x, float y, float scale = 1.0f);
	void	Draw();

	bool	SimpleTestOverlap(CzSprite* other);
	bool	TestOverlap(CzSprite* other, int i1, int i2, int i3);
	bool	TestOverlap(CzSprite* other);

};





#endif // _CCZ_BITMAP_SPRITE9_H_
