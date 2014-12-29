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

#if !defined(_CCZ_BITMAP_SPRITE_H_)
#define _CCZ_BITMAP_SPRITE_H_

#include "CzSprite.h"

/**
 @class	CzBitmapSprite

 @brief	A BitmapSprite is a bitmapped visual representation of an on screen game object.

 */
class CzBitmapSprite : public CzSprite
{
public:
	enum eImageTransform
	{
		ImageTransform_None = 0, 
		ImageTransform_FlipX = 1, 
		ImageTransform_FlipY = 2, 
		ImageTransform_FlipXY = 3, 
	};

	// Properties
protected:
	int					SrcX, SrcY;				///< Top left position in source texture
	int					SrcWidth, SrcHeight;	///< Width and height of sprite in source texture
	float				SrcTileX, SrcTileY;		///< X and Y axis tiling factors
	eImageTransform		ImageTransform;			///< Marmalade image transform
	bool				UVsDirty;				///< Dirty when source texture has changed
public:
	void		setImage(CzImage* image);
	CzImage*	getImage()											{ return Material->Image; }
	void		setColour(const CzColour& colour, bool use_parent = false);
	void		setColour(int index, const CzColour& colour);
	void		setSrcDest(int x, int y, int width, int height);
	void		setSrcRect(int x, int y, int width, int height);
	void		setSrcTile(float x, float y);
	void		setSrcTiled(bool tiled);
	void		setSrcRect(CzIRect* src);
	CzIRect		getSrcRect() const									{ return CzIRect(SrcX, SrcY, SrcWidth, SrcHeight); }
	int			getSrcWidth() const									{ return SrcWidth; }
	int			getSrcHeight() const								{ return SrcHeight; }
	CzVec2		getSrcTile() const									{ return CzVec2(SrcTileX, SrcTileY); }
	bool		getSrcTiled() const									{ return Material->Tiled; }
	void		setFromBrush(CzBrushImage* brush);
	void		setImageTransform(eImageTransform transform)		{ ImageTransform = transform; }
	eImageTransform getImageTransform() const						{ return ImageTransform; }
	CzVec2*	getUVList()												{ return Prim->UVs; }
	void		setUVList(CzVec2* uvs, int num_uvs);					// Replaces the entire UV list (Note that the sprite will take over management of the UV List)
	void		setUVList(CzVec2* uvs, int offset, int num_uvs);		// Modifies UV's in an existing UV list
	void		setUV(CzVec2* uv, int offset);							// Modifies single UV's in an existing UV list
	void		setGeometry(CzGeometry* geom);
	void		calculateVVs(float min_x, float max_x, float min_y, float max_y);
	void		setUVsDirty(bool dirty);
	void		setCounts(int vertices, int indices);					// Changes vertex and face indices cuonts

	// Properties End

protected:
	void				RebuildUVList();

public:

	CzBitmapSprite() : CzSprite(), SrcX(0), SrcY(0), SrcWidth(0), SrcHeight(0), SrcTileX(1.0f), SrcTileY(1.0f), ImageTransform(ImageTransform_None)	{ setSpriteType(ST_Image); }
	virtual ~CzBitmapSprite()
	{
	}

	void	Init(int vertex_count = 4);
	
	void	Draw();
};





#endif // _CCZ_BITMAP_SPRITE_H_
