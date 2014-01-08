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

#include "CzBitmapPoly.h"
#include "CzSpriteManager.h"
#include "CzUtil.h"

//
//
//
//
// CzBitmapPoly implementation
//
//
//
//

void CzBitmapPoly::Init(CzGeometry* geometry)
{
	CzBitmapSprite::Init(geometry->VertCount);
	Geometry = geometry;
	Prim->Type = geometry->Type;
	Prim->FaceCount = geometry->FaceCount;
	Prim->Indices = geometry->Indices;
	for (int t = 0; t < geometry->VertCount; t++)
	{
		Prim->Verts[t] = geometry->Verts[t];
		Prim->UVs[t] = geometry->UVs[t];
		Prim->Colours[t] = geometry->Colours[t];
	}
}

void CzBitmapPoly::RebuildUVList()
{
}

void CzBitmapPoly::TransformVertices()
{
	BuildFinalTransform();

	TransformClipRect();

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
	CzSVec2* screen_v = Prim->Verts;
	for (int t = 0; t < Prim->VertCount; t++)
	{
		float x = TransformedV[t].x;
		float y = TransformedV[t].y;
		float sx = TransformedV[t].x = ((m00 * x) + (m10 * y)) + tx;
		float sy = TransformedV[t].y = ((m01 * x) + (m11 * y)) + ty;
		screen_v->x = (int16)(sx * 8.0f);
		screen_v->y = (int16)(sy * 8.0f);
		screen_v++;
	}
}

void CzBitmapPoly::Draw()
{
	if (!Visible)
		return;

	TransformDirty = false;

	if (!BeforeChildren)
		DrawChildren();

	// Do not render if not visible
	CzImage* image = Material->Image;
	if (AccumDepth > 0 && image != NULL && image->getTexture() != NULL)
	{
		if (!isClippedByManager(Prim->Indices, Prim->VertCount))
		{
			if (UVsDirty)
				RebuildUVList();

			PLATFORM_RENDER->DrawPrimitives(Prim, Material, Prim->FaceCount, true);
		}
	}

	if (BeforeChildren)
		DrawChildren();
}

/**
 @fn	bool CzBitmapPoly::isClipped()

 @brief	Query if this sprite is clipped.

 @return	true if clipped, false if not.
 */

bool CzBitmapPoly::isClipped()
{
	CzVec4 rc = FindFirstScreenClipRect();
	float x1 = rc.x;
	float x2 = x1 + rc.z;
	float y1 = rc.y;
	float y2 = y1 + rc.w;

	CzSVec2* screen_v = Prim->Verts;
	for (int t = 0; t < Prim->VertCount; t++)
	{
		float x = screen_v->x;
		float y = screen_v->y;
		if (x >= x1 && x < x2 && y >= y1 && y < y2)
			return false;
		screen_v++;
	}

	return true;
}

/**
 @fn	bool CzBitmapPoly::HitTest(float x, float y)

 @brief	Tests if an x.y point is within the sprites boundaries

 Note that this method will automatically fail if the x,y point is outside the sprite managers clipping window rect.

 @param	x	The x coordinate.
 @param	y	The y coordinate.

 @return	true if the test passes, false if the test fails.
 */

bool CzBitmapPoly::HitTest(float x, float y)
{
	CzVec4 rc = FindFirstScreenClipRect();
	if (x < rc.x || x > rc.x + rc.z || y < rc.y || y > rc.y + rc.w)
		return false;

	return HitTestNoClip(x, y);
}

/**
 @fn	bool CzBitmapPoly::HitTestNoClip(float x, float y)

 @brief	Tests if an x.y point is within the sprites boundaries

 This method does not take into accuont any cliping rects that are assigned to the sprites manager.

 @param	x	The x coordinate.
 @param	y	The y coordinate.

 @return	true if the test passes, false if the test fails.
 */

bool CzBitmapPoly::HitTestNoClip(float x, float y)
{
	int start = 0;
	int nf = Prim->FaceCount;
	int nv = Prim->VertCount / nf;
	for (int f = 0; f < nf; f++)
	{
		bool failed = false;
		int i1 = start;
		int i2 = start + nv - 1;
		for (int t = 0; t < nv; t++)
		{
			float x0 = TransformedV[i1].x - TransformedV[i2].x;
			float y0 = TransformedV[i1].y - TransformedV[i2].y;
			float x1 = x - TransformedV[i2].x;
			float y1 = y - TransformedV[i2].y;

			if ((x1 * y0 - x0 * y1) >= 0)
			{
				failed = true;
				break;
			}

			i2 = i1;
			i1++;
		}
		if (!failed)
			return true;
		start += nv;
	}

	return false;
}

/**
 @fn	bool CzBitmapPoly::SimpleTestOverlap(CzSprite* other)

 @brief	A simple method to test for overlapping sprites.

 Note that this nethod does not take into account rotation.

 @param [in]	other	If non-null, the other sprite.

 @return	true if overlap, false if no overlap.
 */

bool CzBitmapPoly::SimpleTestOverlap(CzSprite* other)
{
	// Find min_x, max_x, min_y and max_y of this poly
	int min_x1 = TransformedV[0].x, max_x1 = TransformedV[0].x, min_y1 = TransformedV[0].y, max_y1 = TransformedV[0].y;
	for (int t = 1; t < Prim->VertCount; t++)
	{
		int x = TransformedV[t].x;
		int y = TransformedV[t].y;
		if (x < min_x1)
			min_x1 = x;
		if (x > max_x1)
			max_x1 = x;
		if (y < min_y1)
			min_y1 = y;
		if (y > max_y1)
			max_y1 = y;
	}
	CzVec2* v = other->getTransformedV();
	int min_x2 = v->x, max_x2 = v->x, min_y2 = v->y, max_y2 = v->y;
	for (int t = 1; t < Prim->VertCount; t++)
	{
		v++;
		int x = v->x;
		int y = v->y;
		if (x < min_x2)
			min_x2 = x;
		if (x > max_x2)
			max_x2 = x;
		if (y < min_y2)
			min_y2 = y;
		if (y > max_y2)
			max_y2 = y;
	}

	float x1 = min_x1;
	float y1 = min_y1;
	float dx, dy;
	float x2 = min_x2;
	float y2 = min_y2;
	float dx2, dy2;

	dx = max_x1 - min_x1;
	dy = max_y1 - min_y1;
	dx2 = max_x2 - min_x2;
	dy2 = max_y2 - min_y2;

	if ((x2 > x1 && x2 < (x1 + dx)) || ((x2 + dx2) > x1 && (x2 + dx2) < (x1 + dx)))
	{
		if ((y2 > y1 && y2 < (y1 + dy)) || ((y2 + dy2) > y1 && (y2 + dy2) < (y1 + dy)))
			return true;
	}

	return false;
}

/*// Gather up one-dimensional extents of the projection of the polygon onto the specified axis.
void gatherPolygonProjectionExtents(int vc, CzVec2 *v, CzVec2 *axis, float &out_min, float &out_max)
{
	// Initialize extents to a single point, the first vertex
	out_min = out_max = axis->Dot(v[0]);
 
	// Now scan all the rest, growing extents to include them
	for (int t = 1; t < vc; t++)
	{
		float d = axis->Dot(v[t]);
		if (d < out_min) out_min = d;
		else
			if (d > out_max) out_max = d;
	}
}

// Helper routine: test if two convex polygons overlap, using only the edges of the first polygon (polygon "a") to build the list of candidate separating axes.
bool findSeparatingAxis(int vc1, CzVec2 *v1, int vc2, CzVec2 *v2)
{
	// Iterate over all the edges
	int prev = vc1 - 1;
	for (int t = 0; t < vc1; t++)
	{
		// Get edge vector.  (Assume operator- is overloaded)
		CzVec2 edge = v1[t] - v1[prev];
 
		// Rotate vector 90 degrees (doesn't matter which way) to get
		// candidate separating axis.
		CzVec2 v;
		v.x = edge.y;
		v.y = -edge.x;
 
		// Gather extents of both polygons projected onto this axis
		float a_min, a_max, b_min, b_max;
		gatherPolygonProjectionExtents(vc1, v1, &v, a_min, a_max);
		gatherPolygonProjectionExtents(vc2, v2, &v, b_min, b_max);
 
		// Is this a separating axis?
		if (a_max < b_min) return true;
		if (b_max < a_min) return true;
 
		// Next edge, please
		prev = t;
	}
 
	// Failed to find a separating axis
	return false;
}*/

/**
 @fn	bool CzBitmapPoly::TestOverlap(CzSprite* other)

 @brief	Tests for the overlapping of two sprites.

 This method uses separation of axis to test if two polygons overlap, only supports none degenerative convex shapes

 @param [in,out]	other	If non-null, the other sprite.

 @return	true if overlap, false if no overlap.
 */

bool CzBitmapPoly::TestOverlap(CzSprite* other)
{
/*	// First, use all of A's edges to get candidate separating axes
	if (findSeparatingAxis(Prim->VertCount, TransformedV, other->getPrim()->VertCount, other->getTransformedV()))
		return false;
 
	// Now swap roles, and use B's edges
	if (findSeparatingAxis(other->getPrim()->VertCount, other->getTransformedV(), Prim->VertCount, TransformedV))
		return false;*/
 
	// No separating axis found.  They must overlap
	return true;
}





