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
// Notes:
// * Batching will not work with anything other than quads at the moment (does not include quad lists)

#include "CzBitmapSprite.h"
#include "CzBitmapSprite9.h"
#include "CzSpriteManager.h"
#include "CzUtil.h"

//
//
//
//
// CzBitmapSprite implementation
//
//
//
//

void CzBitmapSprite::Init(int vertex_count)
{
	CzSprite::Init(vertex_count);
	Prim->Type = PrimType_QuadStrip;
	Prim->UVs = new CzVec2[vertex_count];
	Prim->Colours = new CzColour[vertex_count];
	Prim->VertCount = vertex_count;
	Prim->Indices = CzBitmapSprite9::SpriteIndices;
}

void CzBitmapSprite::setImage(CzImage* image)
{
	if (Material->Image != image)
	{
		Material->Image = image;
		UVsDirty = true;
	}
}

void CzBitmapSprite::setUVsDirty(bool dirty)
{
	UVsDirty = dirty;
}

void CzBitmapSprite::setColour(const CzColour& colour, bool use_parent)
{
	Colour = colour;
	if (use_parent && LinkedTo != NULL)
		Colour.a = (uint8)(((int)colour.a * LinkedTo->getOpacity()) / 255);
	uint32 c = Colour.get();
	if (Geometry == NULL)
	{
		for (int t = 0; t < Prim->VertCount; t++)
			Prim->Colours[t] = c;
	}
	else
	{
		if (Geometry->Colours == NULL)
		{
			for (int t = 0; t < Prim->VertCount; t++)
				Prim->Colours[t] = c;
		}
		else
		{
			for (int t = 0; t < Prim->VertCount; t++)
			{
				Prim->Colours[t].r = (uint8)(((int)Geometry->Colours[t].r * colour.r) / 255);
				Prim->Colours[t].g = (uint8)(((int)Geometry->Colours[t].g * colour.g) / 255);
				Prim->Colours[t].b = (uint8)(((int)Geometry->Colours[t].b * colour.b) / 255);
				if (use_parent && LinkedTo != NULL)
					Prim->Colours[t].a = (uint8)(((int)Geometry->Colours[t].a * colour.a) / 255);
			}
		}
	}
	if (Colour.a == 0 && colour.a != 0)
		TransformDirty = true;
}

void CzBitmapSprite::setColour(int index, const CzColour& colour)
{
	if (Geometry == NULL)
		Prim->Colours[index] = colour.get();
	else
	{
		if (Geometry->Colours == NULL)
			Prim->Colours[index] = colour.get();
		else
		{
			Prim->Colours[index].r = (uint8)(((int)Geometry->Colours[index].r * colour.r) / 255);
			Prim->Colours[index].g = (uint8)(((int)Geometry->Colours[index].g * colour.g) / 255);
			Prim->Colours[index].b = (uint8)(((int)Geometry->Colours[index].b * colour.b) / 255);
			Prim->Colours[index].a = (uint8)(((int)Geometry->Colours[index].a * colour.a) / 255);
		}
	}
}

void CzBitmapSprite::setSrcDest(int x, int y, int width, int height)
{
	setDestSize(width, height);
	if (SrcX != x || SrcY != y || SrcWidth != width || SrcHeight != height)
	{
		SrcX = x;
		SrcY = y; 
		SrcWidth = width;
		SrcHeight = height;
		UVsDirty = true;
	}
}

void CzBitmapSprite::setSrcRect(int x, int y, int width, int height)
{
	if (SrcX != x || SrcY != y || SrcWidth != width || SrcHeight != height)
	{
		SrcX = x;
		SrcY = y; 
		SrcWidth = width;
		SrcHeight = height;
		UVsDirty = true;
	}
}

void CzBitmapSprite::setSrcRect(CzIRect* src)
{
	if (SrcX != src->x || SrcY != src->y || SrcWidth != src->w || SrcHeight != src->h)
	{
		SrcX = src->x;
		SrcY = src->y; 
		SrcWidth = src->w;
		SrcHeight = src->h;
		UVsDirty = true;
	}
}

void CzBitmapSprite::setSrcTile(float x, float y)
{
	if (x != SrcTileX || y != SrcTileY)
	{
		SrcTileX = x;
		SrcTileY = y;
		UVsDirty = true;
		if (SrcTileX > 1.0f || SrcTileY > 1.0f)
			setTiled(true);
	}
}

void CzBitmapSprite::setSrcTiled(bool tiled)
{
	Material->Tiled = tiled;
}

void CzBitmapSprite::setFromBrush(CzBrushImage* brush)
{
	Brush = brush;
	if (brush != NULL)
	{
		Material->Image = brush->getImage();
		CzVec2* uvs = brush->getUVList();
		if (uvs != NULL)
		{
			setUVList(uvs, 0, Prim->VertCount);
			UVsDirty = false;
		}
		else
		{
			CzIRect rect = brush->getSrcRect();
			SrcX = rect.x;
			SrcY = rect.y;
			SrcWidth = rect.w;
			SrcHeight = rect.h;
			UVsDirty = true;
		}
	}
}

void CzBitmapSprite::setUVList(CzVec2* uvs, int num_uvs)
{
	SAFE_DELETE(Prim->UVs);

	Prim->UVs = uvs;
	UVsDirty = false;
}

void CzBitmapSprite::setUVList(CzVec2* uvs, int offset, int num_uvs)
{
	if (offset + num_uvs > Prim->VertCount)
		return;

	CzVec2* uv = Prim->UVs;
	for (int t = offset; t < offset + num_uvs; t++)
	{
		uv->x = uvs->x;
		uv->y = uvs->y;
		uvs++;
		uv++;
	}
}

void CzBitmapSprite::setUV(CzVec2* uv, int offset)
{
	if (offset >= Prim->VertCount)
		return;

	Prim->UVs[offset].x = uv->x;
	Prim->UVs[offset].y = uv->y;
}

void CzBitmapSprite::RebuildUVList()
{
	if (Geometry != NULL)	// If geometry attached then geometry provides UV list
		return;

	CzImage* image = Material->Image;
	if (image == NULL)
		return;

	float	w = (float)image->getWidth();
	float	h = (float)image->getHeight();
	float	x0 = (float)SrcX / w;
	float	x1 = (float)(SrcX + (float)SrcWidth) / w;
	float	y0 = (float)SrcY / h;
	float	y1 = (float)(SrcY + (float)SrcHeight) / h;
	x1 *= SrcTileX;
	y1 *= SrcTileY;

	if (ImageTransform & ImageTransform_FlipX)
	{
		float t = x0;
		x0 = x1;
		x1 = t;
	}
	if (ImageTransform & ImageTransform_FlipY)
	{
		float t = y0;
		y0 = y1;
		y1 = t;
	}

	CzVec2* uv = Prim->UVs;
	uv->x = x0;
	uv->y = y0;
	uv++;
	uv->x = x1;
	uv->y = y0;
	uv++;
	uv->x = x1;
	uv->y = y1;
	uv++;
	uv->x = x0;
	uv->y = y1;
	UVsDirty = false;
}

void CzBitmapSprite::Draw()
{
	if (!Visible)
		return;

	TransformDirty = false;

	if (!BeforeChildren)
		DrawChildren();

	if (!NoDraw)
	{
		// Do not render if not visible
		CzImage* image = Material->Image;
		bool can_render = (image != NULL && image->getTexture()) || image == NULL;
		if (AccumDepth > 0 && can_render)
		{
			if (!isClippedByManager(NULL, Prim->VertCount))
			{
				if (UVsDirty)
					RebuildUVList();

				if (Manager != NULL && Manager->getBatching() && Prim->VertCount == 4)
					PLATFORM_RENDER->AddPrimtives(Prim, Material, 1, true);
				else
					PLATFORM_RENDER->DrawPrimitives(Prim, Material, 1, true);
			}
		}
#if defined (_DEBUG_)
		else
			CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "CzBitmapSprite - Image is not available!");
#endif // _DEBUG_
	}

	if (BeforeChildren)
		DrawChildren();
}

/**
 @fn	void CzBitmapSprite::setGeometry(CzGeometry* geom)

 @brief	Sets the sprites geometry.

 @param [in]	geom	If non-null, the geometry.
 */

void CzBitmapSprite::setGeometry(CzGeometry* geom)
{
	CzSprite::setGeometry(geom);

	// If UV's are missing from geometry then calculate them
	if (geom->UVs == NULL)
	{
		float min_x, max_x;
		float min_y, max_y;
		geom->CalculateDimensions(min_x, max_x, min_y, max_y);
		calculateVVs(min_x, max_x, min_y, max_y);
	}
}

void CzBitmapSprite::calculateVVs(float min_x, float max_x, float min_y, float max_y)
{
	CzImage* image = Material->Image;
	if (image == NULL)
		return;
	float bw = (float)image->getWidth() / SrcTileX;
	float bh = (float)image->getHeight() / SrcTileY;
	CzVec2* uvs = Prim->UVs;
	CzVec2* v = Prim->Verts;
	float dx = max_x - min_x;
	float dy = max_y - min_y;
	for (int t = 0; t < Prim->VertCount; t++)
	{
		float tu = (v->x - min_x) / dx;
		float tv = (v->y - min_y) / dy;
		uvs->x = (((float)SrcX + (float)SrcWidth * tu)) / bw;
		uvs->y = (((float)SrcY + (float)SrcHeight * tv)) / bh;
		uvs++;
		v++;
	}
}

void CzBitmapSprite::setCounts(int vertices, int indices)
{
	if (Geometry == NULL)
		return;
	Prim->VertCount = vertices;
	Prim->IndicesCount = indices;
	// Calculate face count
	switch (Geometry->Type)
	{
	case PrimType_TriList:
		Prim->FaceCount = indices / 3;
		break;
	case PrimType_QuadList:
		Prim->FaceCount = indices / 4;
		break;
	case PrimType_Poly:
		Prim->FaceCount = 1;
		break;
	}
	for (int t = 0; t < indices; t++)
		Prim->Indices[t] = Geometry->Indices[(indices-1) - t];
}








