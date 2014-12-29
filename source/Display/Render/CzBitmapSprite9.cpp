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

#include "CzBitmapSprite9.h"
#include "CzSpriteManager.h"
#include "CzUtil.h"

uint16 CzBitmapSprite9::Sprite9Indices[] = 
{
	0, 4, 5, 1, 
	1, 5, 6, 2, 
	2, 6, 7, 3, 
	4, 8, 9, 5, 
	5, 9, 10, 6, 
	6, 10, 11, 7, 
	8, 12, 13, 9, 
	9, 13, 14, 10, 
	10, 14, 15, 11, 
};
uint16 CzBitmapSprite9::Sprite9ClipIndices[] = 
{
	 0, 3, 15, 12 
};

//
//
//
//
// CzBitmapSprite9 implementation
//
//
//
//

void CzBitmapSprite9::Init(int vertex_count)
{
	CzBitmapSprite::Init(vertex_count);
	Prim->Type = PrimType_QuadList;
	Prim->FaceCount = 9;
	Prim->IndicesCount = Prim->FaceCount * 4;
	Prim->Indices = Sprite9Indices;
}

void CzBitmapSprite9::RebuildUVList()
{
	CzImage* image = Material->Image;
	if (image == NULL)
		return;

	int		w = image->getWidth();
	int		h = image->getHeight();
	CzIRect area = ((CzBrushImage9*)Brush)->getScalableArea();
	float		px = 0.5f / w;
	float		py = 0.5f / h;
	float		x0 = (float)SrcX / w;
	float		x1 = (float)(SrcX + area.x) / w;
	float		x2 = (float)(SrcX + area.x + area.w) / w - px;
	float		x3 = (float)(SrcX + SrcWidth) / w - px;
	float		y0 = (float)SrcY / h;
	float		y1 = (float)(SrcY + area.y) / h;
	float		y2 = (float)(SrcY + area.y + area.h) / h;
	float		y3 = (float)(SrcY + SrcHeight) / h - py;

	if (ImageTransform & ImageTransform_FlipX)
	{
		float t = x0;
		x0 = x3;
		x3 = t;
		t = x1;
		x1 = x2;
		x2 = t;
	}
	if (ImageTransform & ImageTransform_FlipY)
	{
		float t = y0;
		y0 = y3;
		y3 = t;
		t = y1;
		y1 = y2;
		y2 = t;
	}

	CzVec2* uv = Prim->UVs;
	uv->x = x0;
	uv->y = y0;
	uv++;
	uv->x = x1;
	uv->y = y0;
	uv++;
	uv->x = x2;
	uv->y = y0;
	uv++;
	uv->x = x3;
	uv->y = y0;
	uv++;
	uv->x = x0;
	uv->y = y1;
	uv++;
	uv->x = x1;
	uv->y = y1;
	uv++;
	uv->x = x2;
	uv->y = y1;
	uv++;
	uv->x = x3;
	uv->y = y1;
	uv++;
	uv->x = x0;
	uv->y = y2;
	uv++;
	uv->x = x1;
	uv->y = y2;
	uv++;
	uv->x = x2;
	uv->y = y2;
	uv++;
	uv->x = x3;
	uv->y = y2;
	uv++;
	uv->x = x0;
	uv->y = y3;
	uv++;
	uv->x = x1;
	uv->y = y3;
	uv++;
	uv->x = x2;
	uv->y = y3;
	uv++;
	uv->x = x3;
	uv->y = y3;

	UVsDirty = false;

	// We also need to rebuol the transform as the cetrices are based on the textures size
	RebuildTransform();
}

void CzBitmapSprite9::TransformVertices()
{
	BuildFinalTransform();

	TransformClipRect();

	// Calcuklate 9-patch vertices
	float x0 = -Width / 2.0f;
	float y0 = -Height / 2.0f;
	float x1 = Width / 2.0f;
	float y1 = Height / 2.0f;
	float sw = (float)SrcWidth;
	float sh = (float)SrcHeight;
	CzIRect area = ((CzBrushImage9*)Brush)->getScalableArea();

	CzVec2* v = Prim->Verts;
	// Build horizontal vertices
	v[0].x = x0;
	v[4].x = x0;
	v[8].x = x0;
	v[12].x = x0;
	x0 += area.x;
	v[1].x = x0;
	v[5].x = x0;
	v[9].x = x0;
	v[13].x = x0;
	v[3].x = x1;
	v[7].x = x1;
	v[11].x = x1;
	v[15].x = x1;
	x1 -= sw - (float)(area.x + area.w);
	v[2].x = x1;
	v[6].x = x1;
	v[10].x = x1;
	v[14].x = x1;

	// Build vertical vertices
	v[0].y = y0;
	v[1].y = y0;
	v[2].y = y0;
	v[3].y = y0;
	y0 += area.y;
	v[4].y = y0;
	v[5].y = y0;
	v[6].y = y0;
	v[7].y = y0;
	v[12].y = y1;
	v[13].y = y1;
	v[14].y = y1;
	v[15].y = y1;
	y1 -= sh - (float)(area.y + area.h);
	v[8].y = y1;
	v[9].y = y1;
	v[10].y = y1;
	v[11].y = y1;

	float m00 = FinalTransform.m[0][0];
	float m01 = FinalTransform.m[0][1];
	float m10 = FinalTransform.m[1][0];
	float m11 = FinalTransform.m[1][1];
	float tx = FinalTransform.getX();
	float ty = FinalTransform.getY();
	if (Anchor == TopLeft)
	{
		tx += Width / 2.0f;
		ty += Height / 2.0f;
	}
	CzVec2* screen_v = Prim->Verts;
	for (int t = 0; t < Prim->VertCount; t++)
	{
		float x = screen_v->x;
		float y = screen_v->y;
		screen_v->x = ((m00 * x) + (m10 * y)) + tx;
		screen_v->y = ((m01 * x) + (m11 * y)) + ty;
		screen_v++;
	}
}

void CzBitmapSprite9::Draw()
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
		if (AccumDepth > 0 && image != NULL && image->getTexture() != NULL)
		{
			if (!isClippedByManager(&Sprite9ClipIndices[0], 4))
			{
				if (UVsDirty)
					RebuildUVList();

//				if (Manager != NULL &&  Manager->getBatching())
//					PLATFORM_RENDER->AddPrimtives(Prim, Material, 1, true);
//				else
				PLATFORM_RENDER->DrawPrimitives(Prim, Material, 1, true);
			}
		}
	}

	if (BeforeChildren)
		DrawChildren();
}

bool CzBitmapSprite9::isClipped()
{
	CzVec4 rc = FindFirstScreenClipRect();
	float x1 = rc.x;
	float x2 = x1 + rc.z;
	float y1 = rc.y;
	float y2 = y1 + rc.w;

	CzVec2* screen_v = Prim->Verts;
	int i1 = 0;
	int i2 = 3;
	for (int t = 0; t < Prim->VertCount; t++)
	{
		int ii1 = Sprite9ClipIndices[i1];
		int ii2 = Sprite9ClipIndices[i2];
		float x = (float)screen_v[ii1].x;
		float y = (float)screen_v[ii2].y;
		if (x >= x1 && x < x2 && y >= y1 && y < y2)
			return false;
	}

	return true;
}

bool CzBitmapSprite9::HitTest(float x, float y)
{
	CzVec4 rc = FindFirstScreenClipRect();
	if (x < rc.x || x > rc.x + rc.z || y < rc.y || y > rc.y + rc.w)
		return false;

	return HitTestNoClip(x, y);
}


bool CzBitmapSprite9::HitTestNoClip(float x, float y)
{
	int i1 = 0;
	int i2 = 3;
	CzVec2* tv = Prim->Verts;
	for (int t = 0; t < 4; t++)
	{
		int ii1 = Sprite9ClipIndices[i1];
		int ii2 = Sprite9ClipIndices[i2];
		float x0 = tv[ii1].x - tv[ii2].x;
		float y0 = tv[ii1].y - tv[ii2].y;
		float x1 = x - tv[ii2].x;
		float y1 = y - tv[ii2].y;

		if ((x1 * y0 - x0 * y1) >= 0)
			return false;

		i2 = i1;
		i1++;
	}

	return true;
}

bool CzBitmapSprite9::isOutsideFocusRange(float x, float y, float scale)
{
	float cx = 0;
	float cy = 0;
	float min_x = 99999999.0f;
	float max_x = -99999999.0f;
	float min_y = 99999999.0f;
	float max_y = -99999999.0f;
	CzVec2* tv = Prim->Verts;
	int idx[4] = { 0, 3, 15, 12 };

	for (int t = 0; t < 4; t++)
	{
		int i = idx[t];
		float mx = tv[i].x;
		if (mx < min_x)
			min_x = mx;
		if (mx > max_x)
			max_x = mx;
		float my = tv[i].y;
		if (my < min_y)
			min_y = my;
		if (my > max_y)
			max_y = my;
		cx  += mx;
		cy  += my;
	}
	cx /= 4;
	cy /= 4;

	float dx = x - cx;
	float dy = y - cy;
	float d = (max_x - min_x) * (max_x - min_x) + (max_y - min_y) * (max_y - min_y);
	float d2 = dx * dx + dy * dy;

	return d2 > d * scale;
}

/**
 @fn	bool CzBitmapSprite9::TestOverlap(CzSprite* other, int i1, int i2, int i3)

 @brief	Tests if the supplied triangular area of the sprite overlaps this sprite.

 Using 3 vertices at the supplied indices i1, i2, i3 of the supplied sprites transformed vertices to see if the overlap this sprites vertices.

 Note that this method can also test against 16 vertex patch sprites

 @param [in]	other	If non-null, the other.
 @param	i1				First vertex index.
 @param	i2				Second vertex index.
 @param	i3				Third vertex index.

 @return	true if overlap, false if no overlap.
 */

bool CzBitmapSprite9::TestOverlap(CzSprite* other, int i1, int i2, int i3)
{
	CzVec2* tv = Prim->Verts;
	float x1 = tv[i1].x;
	float y1 = tv[i1].y;
	float x2 = tv[i2].x;
	float y2 = tv[i2].y;
	float x3 = tv[i3].x;
	float y3 = tv[i3].y;

	float v1x = x2 - x1;
	float v1y = y2 - y1;
	float rx = -v1y;
	float ry = v1x;
	bool ref_side = (rx * (x3 - x1) + ry * (y3 - y1)) >= 0;

	CzVec2* other_v = other->getScreenV();
	int vc = other->getVertexCount();
	int indices[4] = { 0, 3, 15, 12 };
	for (int t = 0; t < 4; t++)
	{
		float x = (other_v + indices[t])->x;
		float y = (other_v + indices[t])->y;

		bool side = (rx * (x - x1) + ry * (y - y1)) >= 0;

		if (side == ref_side)
			return false;
	}

	return true;
}

/**
 @fn	bool CzBitmapSprite9::SimpleTestOverlap(CzSprite* other)

 @brief	A simple method to test for overlapping sprites.

 Note that this nethod does not take into account rotation.

 @param [in]	other	If non-null, the other sprite.

 @return	true if overlap, false if no overlap.
 */

bool CzBitmapSprite9::SimpleTestOverlap(CzSprite* other)
{
	CzVec2* tv = Prim->Verts;
	float x1 = tv[0].x;
	float y1 = tv[0].y;
	float dx, dy;
	CzVec2* other_v = other->getScreenV();
	float x2 = other_v->x;
	float y2 = other_v->y;
	float dx2, dy2;

	dx = tv[3].x - x1;
	dy = tv[15].y - y1;
	dx2 = (other_v + 3)->x - x2;
	dy2 = (other_v + 15)->y - y2;

	if ((x2 > x1 && x2 < (x1 + dx)) || ((x2 + dx2) > x1 && (x2 + dx2) < (x1 + dx)))
	{
		if ((y2 > y1 && y2 < (y1 + dy)) || ((y2 + dy2) > y1 && (y2 + dy2) < (y1 + dy)))
			return true;
	}

	return false;
}

/**
 @fn	bool CzBitmapSprite9::TestOverlap(CzSprite* other)

 @brief	Tests for the overlapping of two sprites.

 This method takes into account rotation of both sprites. However, it will revert to a simple method of overlap testing if both sprites are not rotated.

 @param [in,out]	other	If non-null, the other sprite.

 @return	true if overlap, false if no overlap.
 */

bool CzBitmapSprite9::TestOverlap(CzSprite* other)
{
	// if sprites are not rotated then use simple overlap test
	if (Angle == 0 && other->getAngle() == 0)
		return SimpleTestOverlap(other);

	int i1 = 0;
	int i2, i3, i4;

	i2 = 3;
	i3 = 15;
	i4 = 12;
	if (TestOverlap(other, i1, i2, i3))
		return false;
	if (TestOverlap(other, i1, i4, i3))
		return false;
	if (TestOverlap(other, i4, i3, i1))
		return false;
	if (TestOverlap(other, i3, i2, i1))
		return false;

	i2 = 3;
	i3 = 15;
	i4 = 12;

	if (other->TestOverlap(this, i1, i2, i3))
		return false;
	if (other->TestOverlap(this, i1, i4, i3))
		return false;
	if (other->TestOverlap(this, i4, i3, i1))
		return false;
	if (other->TestOverlap(this, i3, i2, i1))
		return false;

	return true;
}




