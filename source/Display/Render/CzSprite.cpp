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

#include "CzSprite.h"
#include "CzString.h"
#include "CzUtil.h"
#include "CzRender.h"
#include "CzActor.h"
#include "CzSpriteManager.h"

uint16 CzSprite::SpriteIndices[] = { 0, 3, 1, 2 };

/**
 @fn	void CzSprite::setLinkedTo(CzSprite* sprite)

 @brief	Changes sprite linkage

 Sprites by default are placed in the sprite manager. When a sprite is linked to another sprite it is removed from the sprite manager and added to that sprites child list unless 
 it is an orphan sprite. 

 @param [in,out]	sprite	If non-null, the sprite.
 */

void CzSprite::setLinkedTo(CzSprite* sprite)
{
	if (LinkedTo == sprite)
		return;

	if (!Orphan)
	{
		// Remove sprite from previous sprites child list or the managers sprite list
		Manager->removeSprite(this, false);

		if (sprite != NULL)	
		{
			// Insert the sprite into the new linked to sprites sprite list
			sprite->addChild(this);
		}
		else
		{
			Manager->addSprite(this);
			LinkedTo = NULL;
			return;
		}

	}

	LinkedTo = sprite;
}

/**
 @fn	void CzSprite::addChild(CzSprite* sprite)

 @brief	Adds a child sprite to the sprites child list.

 @param [in,out]	sprite	If non-null, the sprite.
 */

void CzSprite::addChild(CzSprite* sprite)
{
	Children.push_back(sprite);
	sprite->setManager(this->Manager);
	sprite->setParent(this);
}

/**
 @fn	void CzSprite::removeChild(CzSprite* sprite, bool delete_sprites)

 @brief	Removes a child sprite from teh child list and optionally destroys it

 @param [in]	sprite	If non-null, the sprite.
 @param	delete_sprites	  	true to delete the sprites.
 */

void CzSprite::removeChild(CzSprite* sprite, bool delete_sprites)
{
	Children.remove(sprite);
	if (delete_sprites && !sprite->isPooled())
		delete sprite;
/*	for (Iterator it = Children.begin(); it != Children.end(); ++it)
	{
		if (*it == sprite)
		{
			if (delete_sprites && !(*it)->isPooled())
				delete *it;
			Children.erase(it);
			break;
		}
	}*/
}

/**
 @fn	void CzSprite::RebuildTransform()

 @brief	Rebuilds the sprites display transform.

 */

void CzSprite::RebuildTransform()
{
	// Build the transform
	AccumDepth = Depth;
	CzMatrix3 trans;
	trans.Translate(&Origin);
	// Set the rotation transform
	Transform.Rotate(Angle);
	// Scale the transform
	if (ScaleX != 1.0f || ScaleY != 1.0f)
	{
		CzMatrix3 scale;
		scale.Scale(ScaleX, ScaleY);
		Transform.Multiply(&scale);
	}
	// Translate the transform
	Transform.TranslateSet(&Position);
	// Apply origin
	Transform.Multiply(&trans);
	// Apply linked sprites transform if linked
	if (LinkedTo != NULL)
	{
		Transform.MultiplyPost(&LinkedTo->getTransform());
		AccumDepth = Depth + LinkedTo->getAccumDepth();
	}
/*	else
	{
		// Apply Manager transform if sprite is managed by a Manager sprite manager (doesnt work with none uniform scaling)
		if (Manager != NULL)
			Transform.MultiplyPost(Manager->getTransform());
	}*/
}

/**
 @fn	bool CzSprite::isClippedByManager(uint16 *indices, int count)

 @brief	Checks to see if the sprite is clipped by the sprite manager.

 Utility method that checks the sprites current transformed vertices and the order defined by the supplied vertex indices list to see if they are fully clipped.

 @param [in]	indices	If non-null, the sprites vertex indices.
 @param	count			   	Number of vertices.

 @return	true if clipped by manager, false if not.
 */

bool CzSprite::isClippedByManager(uint16 *indices, int count)
{
	if (Manager == NULL)
		return false;

	CzVec4 clip_rect = Manager->getScreenClipRect();
	float left = clip_rect.x;
	float top = clip_rect.y;
	float right = left + clip_rect.z;
	float bottom = top + clip_rect.w;

	int left_off = 0;
	int right_off = 0;
	int top_off = 0;
	int bottom_off = 0;

	CzVec2* tv = Prim->Verts;
	for (int t = 0; t < count; t++)
	{
		float x, y;
		if (indices != NULL)
		{
			int i = *(indices + t);
			x = tv[i].x;
			y = tv[i].y;
		}
		else
		{
			x = tv[t].x;
			y = tv[t].y;
		}

		if (x < left)
			left_off++;
		else
		if (x > right)
			right_off++;

		if (y < top)
			top_off++;
		else
		if (y > bottom)
			bottom_off++;
	}

	if (left_off == count)
		return true;
	if (right_off == count)
		return true;
	if (top_off == count)
		return true;
	if (bottom_off == count)
		return true;

	return false;
}

// Gather up one-dimensional extents of the projection of the polygon onto the specified axis.
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
}

/**
 @fn	bool CzSprite::TestOverlap(CzSprite* other, int i1, int i2, int i3)

 @brief	Tests if the supplied triangular area of the sprite overlaps this sprite.

 Using 3 vertices at the supplied indices i1, i2, i3 of the supplied sprites transformed vertices to see if the overlap this sprites vertices.

 Note that this method can also test against 16 vertex patch sprites

 @param [in]	other	If non-null, the other.
 @param	i1				First vertex index.
 @param	i2				Second vertex index.
 @param	i3				Third vertex index.

 @return	true if overlap, false if no overlap.
 */

bool CzSprite::TestOverlap(CzSprite* other, int i1, int i2, int i3)
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
	for (int t = 0; t < vc; t++)
	{
		float x = other_v->x;
		float y = other_v->y;
		bool side = (rx * (x - x1) + ry * (y - y1)) >= 0;

		if (side == ref_side)
			return false;
		other_v++;
	}

	return true;
}

/**
 @fn	bool CzSprite::SimpleTestOverlap(CzSprite* other)

 @brief	A simple method to test for overlapping sprites.

 Note that this nethod does not take into account rotation.

 @param [in]	other	If non-null, the other sprite.

 @return	true if overlap, false if no overlap.
 */

bool CzSprite::SimpleTestOverlap(CzSprite* other)
{
	CzVec2* tv = Prim->Verts;
	float x1 = tv[0].x;
	float y1 = tv[0].y;
	float dx, dy;
	CzVec2* other_v = other->getScreenV();
	float x2 = other_v->x;
	float y2 = other_v->y;
	float dx2, dy2;

	if (Geometry != NULL)
	{
		float min_x1 = tv[0].x, max_x1 = tv[0].x, min_y1 = tv[0].y, max_y1 = tv[0].y;
		for (int t = 1; t < Prim->VertCount; t++)
		{
			float x = tv[t].x;
			float y = tv[t].y;
			if (x < min_x1)
				min_x1 = x;
			if (x > max_x1)
				max_x1 = x;
			if (y < min_y1)
				min_y1 = y;
			if (y > max_y1)
				max_y1 = y;
		}
		CzVec2* v = other->getScreenV();
		float min_x2 = v->x, max_x2 = v->x, min_y2 = v->y, max_y2 = v->y;
		for (int t = 1; t < Prim->VertCount; t++)
		{
			v++;
			float x = v->x;
			float y = v->y;
			if (x < min_x2)
				min_x2 = x;
			if (x > max_x2)
				max_x2 = x;
			if (y < min_y2)
				min_y2 = y;
			if (y > max_y2)
				max_y2 = y;
		}
		x1 = min_x1;
		y1 = min_y1;
		x2 = min_x2;
		y2 = min_y2;
		dx = max_x1 - min_x1;
		dy = max_y1 - min_y1;
		dx2 = max_x2 - min_x2;
		dy2 = max_y2 - min_y2;
	}
	else
	{
		x1 = tv[0].x;
		y1 = tv[0].y;
		x2 = other_v->x;
		y2 = other_v->y;
		dx = tv[1].x - x1;
		dy = tv[3].y - y1;
		dx2 = (other_v + 1)->x - x2;
		dy2 = (other_v + 3)->y - y2;
	}


	if ((x2 > x1 && x2 < (x1 + dx)) || ((x2 + dx2) > x1 && (x2 + dx2) < (x1 + dx)))
	{
		if ((y2 > y1 && y2 < (y1 + dy)) || ((y2 + dy2) > y1 && (y2 + dy2) < (y1 + dy)))
			return true;
	}

	return false;
}

/**
 @fn	bool CzSprite::TestOverlap(CzSprite* other)

 @brief	Tests for the overlapping of two sprites.

 This method takes into account rotation of both sprites. However, it will revert to a simple method of overlap testing if both sprites are not rotated.

 @param [in,out]	other	If non-null, the other sprite.

 @return	true if overlap, false if no overlap.
 */

bool CzSprite::TestOverlap(CzSprite* other)
{
	if (Geometry != NULL)
	{
		// First, use all of A's edges to get candidate separating axes
		if (findSeparatingAxis(Prim->VertCount, Prim->Verts, other->getPrim()->VertCount, other->getScreenV()))
			return false;
 
		// Now swap roles, and use B's edges
		if (findSeparatingAxis(other->getPrim()->VertCount, other->getScreenV(), Prim->VertCount, Prim->Verts))
			return false;
	}
	else
	{
		// if sprites are not rotated then use simple overlap test
		if (Angle == 0 && other->getAngle() == 0)
			return SimpleTestOverlap(other);

		int i1 = 0;
		int i2, i3, i4;

		i2 = 1;
		i3 = 2;
		i4 = 3;

		if (TestOverlap(other, i1, i2, i3))
			return false;
		if (TestOverlap(other, i1, i4, i3))
			return false;
		if (TestOverlap(other, i4, i3, i1))
			return false;
		if (TestOverlap(other, i3, i2, i1))
			return false;

		if (other->TestOverlap(this, i1, i2, i3))
			return false;
		if (other->TestOverlap(this, i1, i4, i3))
			return false;
		if (other->TestOverlap(this, i4, i3, i1))
			return false;
		if (other->TestOverlap(this, i3, i2, i1))
			return false;
	}

	return true;
}


/**
 @fn	void CzSprite::RebuildTransformNow()

 @brief	Rebuild the sprites transform immediately (if dirty).

 */

void CzSprite::RebuildTransformNow()
{
	if (TransformDirty)
	{
		RebuildTransform();
		TransformVertices();
	}
}

CzSprite::~CzSprite()
{
#if defined(CZ_ENABLE_METRICS)
		CzMetrics::TotalSpritesDestroyed++;
#endif
	for (Iterator it = Children.begin(); it != Children.end(); ++it)
		delete *it;
	Children.clear();

	SAFE_DELETE(Material)
	SAFE_DELETE(Prim)
}


/**
 @fn	void CzSprite::Init(int vertex_count)

 @brief	Initialises the sprite.

 @param	vertex_count	Number of vertices that the sprite is made up from.
 */

void CzSprite::Init(int vertex_count)
{
	Parent = NULL;
	if (Prim == NULL)
	{
		Prim = new CzRenderPrim();
		Prim->SharedIndices = true;
		Prim->Verts = new CzVec2[vertex_count]();
		Prim->VertCount = vertex_count;
		Prim->IndicesCount = vertex_count;
		Prim->FaceCount = 1;
	}
	Material = new CzRenderMaterial();
	Material->AlphaMode = AlphaMode_Blend;
	Material->Filter = true;
	Material->Image = NULL;
	Material->Tiled = false;
	Pooled = false;
	Manager = NULL;
	LinkedTo = NULL;
	Brush = NULL;
	TransformDirty = true;
	Position.x = 0;
	Position.y = 0;
	Origin.x = 0;
	Origin.y = 0;
	Angle = 0;
	ScaleX = 1.0f;
	ScaleY = 1.0f;
	Colour.r = 0xff;
	Colour.g = 0xff;
	Colour.b = 0xff;
	Colour.a = 0xff;
	Width = 0;
	Height = 0;
	Depth = 1.0f;
	AccumDepth = Depth;
	Visible = true;
	Layer = 0;
	FinalTransform.Identity();
	Transform.Identity();
	ClipRect.x = 0;
	ClipRect.y = 0;
	ClipRect.z = -1;	// No clipping
	ClipRect.w = -1;
	ScreenClipRect.x = 0;
	ScreenClipRect.y = 0;
	ScreenClipRect.z = 0;
	ScreenClipRect.w = 0;
	Skew.x = 0;
	Skew.y = 0;
	Skew.z = 0;
	Skew.w = 0;
	BeforeChildren = true;
	ChildChangeClip = false;
	Orphan = false;
	IgnoreCamera = false;
	Anchor = Centre;
}

/**
 @fn	bool CzSprite::isClipped()

 @brief	Query if this sprite is clipped.

 @return	true if clipped, false if not.
 */

bool CzSprite::isClipped()
{
	CzVec4 rc = FindFirstScreenClipRect();
	float x1 = rc.x;
	float x2 = x1 + rc.z;
	float y1 = rc.y;
	float y2 = y1 + rc.w;

	CzVec2* screen_v = Prim->Verts;
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
 @fn	void CzSprite::setGeometry(CzGeometry* geom)

 @brief	Sets the sprites geometry. The geometry defines what shape will be rendered by the sprite

 @param [in]	geom	If non-null, the geometry.
 */

void CzSprite::setGeometry(CzGeometry* geom)
{
	int nv = geom->VertCount;
	// Allocate space for geometry data
	if (Prim->VertCount != nv)
	{
		SAFE_DELETE_ARRAY(Prim->Verts)
		SAFE_DELETE_ARRAY(Prim->UVs)
		SAFE_DELETE_ARRAY(Prim->Colours)
		Prim->Verts = new CzVec2[nv]();
		Prim->UVs = new CzVec2[nv]();
		Prim->Colours = new CzColour[nv]();
		Prim->VertCount = nv;
	}
	if (Prim->FaceCount != geom->FaceCount || Prim->SharedIndices)
	{
		if (!Prim->SharedIndices)
			SAFE_DELETE(Prim->Indices)
		Prim->Indices = new uint16[geom->IndicesCount];
		Prim->IndicesCount = geom->IndicesCount;
		Prim->FaceCount = geom->FaceCount;
		Prim->SharedIndices = false;
	}
	Prim->Type = geom->Type;

	// Copy geometry data over
	for (int t = 0; t < nv; t++)
	{
		Prim->Verts[t] = geom->Verts[t];

		if (geom->UVs)
			Prim->UVs[t] = geom->UVs[t];
		if (geom->Colours)
			Prim->Colours[t] = geom->Colours[t];
		else
		{
			Prim->Colours[t].r = 255;
			Prim->Colours[t].g = 255;
			Prim->Colours[t].b = 255;
			Prim->Colours[t].a = 255;
		}
	}
	for (int t = 0; t < geom->IndicesCount; t++)
		Prim->Indices[t] = geom->Indices[(geom->IndicesCount-1) - t];

	Geometry = geom;
}

/**
 @fn	bool CzSprite::HitTest(float x, float y)

 @brief	Tests if an x.y point is within the sprites boundaries

 Note that this method will automatically fail if the x,y point is outside the sprite managers clipping window rect.

 @param	x	The x coordinate.
 @param	y	The y coordinate.

 @return	true if the test passes, false if the test fails.
 */

bool CzSprite::HitTest(float x, float y)
{
	CzVec4 rc = FindFirstScreenClipRect();
	if (x < rc.x || x > rc.x + rc.z || y < rc.y || y > rc.y + rc.w)
		return false;

	return HitTestNoClip(x, y);
}

/**
 @fn	bool CzSprite::HitTestNoClip(float x, float y)

 @brief	Tests if an x.y point is within the sprites boundaries

 This method does not take into accuont any cliping rects that are assigned to the sprites manager.

 @param	x	The x coordinate.
 @param	y	The y coordinate.

 @return	true if the test passes, false if the test fails.
 */

bool CzSprite::HitTestNoClip(float x, float y)
{
	int start = 0;
	int nf = Prim->FaceCount;
	int nv = Prim->VertCount / nf;
	CzVec2* tv = Prim->Verts;
	for (int f = 0; f < nf; f++)
	{
		bool failed = false;
		int i1 = start;
		int i2 = start + nv - 1;
		for (int t = 0; t < nv; t++)
		{
			float x0 = tv[i1].x - tv[i2].x;
			float y0 = tv[i1].y - tv[i2].y;
			float x1 = x - tv[i2].x;
			float y1 = y - tv[i2].y;

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
 @fn	bool CzSprite::isOutsideFocusRange(float x, float y, float scale)

 @brief	Tests to see if the supplied point is outside a specific distance

 The focus range is the distance that a point has to move to to be declared as no longer having touch focus. The default focus range is calculated as the longest distance 
 between each end of the sprite. The supplied scale value can be used to increase the focus range. For example a scale value of 2.0 will increase the focus range to 
 double the size of the sprites visible dimensions.

 @param	x	 	The x coordinate.
 @param	y	 	The y coordinate.
 @param	scale	The amount to scale the focus range.

 @return	true if outside focus range, false if not.
 */

bool CzSprite::isOutsideFocusRange(float x, float y, float scale)
{
	float cx = 0;
	float cy = 0;
	float min_x = 99999999.0f;
	float max_x = -99999999.0f;
	float min_y = 99999999.0f;
	float max_y = -99999999.0f;
	int nv = Prim->VertCount;
	CzVec2* tv = Prim->Verts;
	for (int t = 0; t < nv; t++)
	{
		float mx = tv[t].x;
		if (mx < min_x)
			min_x = mx;
		if (mx > max_x)
			max_x = mx;
		float my = tv[t].y;
		if (my < min_y)
			min_y = my;
		if (my > max_y)
			max_y = my;
		cx  += mx;
		cy  += my;
	}
	cx /= (float)nv;
	cy /= (float)nv;

	float dx = x - cx;
	float dy = y - cy;
	float d = (max_x - min_x) * (max_x - min_x) + (max_y - min_y) * (max_y - min_y);
	float d2 = dx * dx + dy * dy;

	return d2 > (d * scale);
}

/**
 @fn	CzVec2 CzSprite::TransformPoint(float x, float y)

 @brief	Transform supplied point by current sprite transform.

 @param	x	The x coordinate.
 @param	y	The y coordinate.

 @return	Transformed point.
 */

CzVec2 CzSprite::TransformPoint(float x, float y)
{
	return Transform.Transform(x, y);
}

/**
 @fn	CzVec2 CzSprite::TransformPointToScreen(float x, float y)

 @brief	Transform supplied point by current sprite final transform (to screen coordinates).

 @param	x	The x coordinate.
 @param	y	The y coordinate.

 @return	Transformed point in screen coordinates.
 */

CzVec2 CzSprite::TransformPointToScreen(float x, float y)
{
	return FinalTransform.Transform(x, y);
}

/**
 @fn	void CzSprite::BringToFront()

 @brief	Bring sprite to front of other sprites.

 If sprite is assigned to a manager then it will be ulinked and relinked back into the manager so that it will be drawn last. If the sprite is assigned as a child of another 
 sprite then it will be ulinked and relinked back into the sprites child list so it will be the last drawn sprite in its parents sprite list.

 */

void CzSprite::BringToFront()
{
	if (Parent != NULL)
	{
		Parent->removeChild(this, false);
		Parent->addChild(this);
	}
	else
	if (Manager != NULL)
	{
		Manager->removeSprite(this, false);
		Manager->addSprite(this);
	}
}

/**
 @fn	void CzSprite::UpdateClipping()

 @brief	Updates the gobal clipper to the sprites currenmt clipping rect.

 */

void CzSprite::UpdateClipping()
{
	if (ClipRect.w <= 0)
		return;

	if (ClipRect.w != 0)
	{
		PLATFORM_RENDER->SetClipRect((int)ScreenClipRect.x, (int)ScreenClipRect.y, (int)ScreenClipRect.z, (int)ScreenClipRect.w);
//	else
//		PLATFORM_RENDER->ResetClipRect();
	}

	// Notify parent that we changed the clip rect
	if (Parent != NULL)
		Parent->ChildChangeClip = true;
}

/**
 @fn	void CzSprite::BuildFinalTransform()

 @brief	Builds the sprites final transform.

 The sprites local transform is combined with the sprite managers transform to create a final transform that transforms the sprite from its local sprite coordinates to screen 
 coordinates.

 Note that if the sprite has depth then basic perspective projection will be applied to the sprites position.

 */

void CzSprite::BuildFinalTransform()
{
	// Apply Manager transform if sprite is managed by a Manager sprite manager (doesnt work with none uniform scaling)
	FinalTransform = Transform;
	if (Manager != NULL)
	{
		if (AccumDepth == 1.0f || AccumDepth == 0)
		{
			if (IgnoreCamera)
				FinalTransform.MultiplyPost(&Manager->getTransformNoCamera());
			else
				FinalTransform.MultiplyPost(&Manager->getTransform());
		}
		else
		{
			CzVec2 cop = Manager->getCOP();
			CzVec2 centre = Manager->getScreenCentre();

			CzMatrix3 m;
			if (IgnoreCamera)
				m = Manager->getTransformNoCamera();
			else
				m = Manager->getTransform();
			float ooa = 1.0f / AccumDepth;
			m.m[0][0] *= ooa;
			m.m[0][1] *= ooa;
			m.m[1][0] *= ooa;
			m.m[1][1] *= ooa;
			m.TranslateSet((m.getX() - centre.x) * ooa + cop.x, (m.getY() - centre.y) * ooa + cop.y);

			FinalTransform.MultiplyPost(&m);
		}
	}
}

/**
 @fn	void CzSprite::TransformClipRect()

 @brief	Transform the sprites clipping rectangle.

 */

void CzSprite::TransformClipRect()
{
	// Transform clipping rect
	if (ClipRect.w > 0)
	{
		CzVec2 tl = CzVec2(ClipRect.x, ClipRect.y);
		CzVec2 br = CzVec2((ClipRect.x + ClipRect.z), (ClipRect.y + ClipRect.w));
		FinalTransform.Transform(&tl, &tl);
		FinalTransform.Transform(&br, &br);
		float x1 = tl.x;
		float y1 = tl.y;
		float x2 = br.x;
		float y2 = br.y;

		// Ensure clip rect always shrinks
		CzVec4 current = FindFirstScreenClipRect();
		float cx1 = current.x;
		float cy1 = current.y;
		float cx2 = current.x + current.z;
		float cy2 = current.y + current.w;
		if (x1 > cx1)
			cx1 = x1;
		if (x2 < cx2)
			cx2 = x2;
		if (y1 > cy1)
			cy1 = y1;
		if (y2 < cy2)
			cy2 = y2;

		ScreenClipRect.x = cx1;
		ScreenClipRect.y = cy1;
		ScreenClipRect.z = (cx2 - cx1);
		ScreenClipRect.w = (cy2 - cy1);
	}
}

/**
 @fn	bool CzSprite::isVisibleWithParents() const

 @brief	Checks up the sprite hierarchy to see if the sprite and its parents are visible.

 @return	true if visible and its parents are visible, false if not.
 */

bool CzSprite::isVisibleWithParents() const
{
	if (Parent != NULL)
	{
		if (!Parent->isVisibleWithParents())
			return false;
	}

	return Visible;
}

/**
 @fn	CzVec4 CzSprite::FindFirstClipRect()

 @brief	Searches up the sprite hierarchy for the first clip rectangle.

 @return	The found clip rectangle.
 */

CzVec4 CzSprite::FindFirstClipRect()
{
	if (Parent != NULL)
	{
		if (Parent->getClipRect().w > 0)
			return Parent->getClipRect();

		return Parent->FindFirstClipRect();
	}
	else
	if (Manager != NULL)
		return Manager->getClipRect();

	return CzVec4(0, 0, 0, 0);
}

/**
 @fn	CzVec4 CzSprite::FindFirstScreenClipRect()

 @brief	Searches up the sprite hierarchy for the first valid screen clip rectangle.

 This method does not check the current sprites screen clip rect.

 @return	The found screen clip rectangle.
 */

CzVec4 CzSprite::FindFirstScreenClipRect()
{
	if (Parent != NULL)
	{
		if (Parent->getClipRect().w > 0)
			return Parent->getScreenClipRect();

		return Parent->FindFirstScreenClipRect();
	}
	else
	if (Manager != NULL)
		return Manager->getScreenClipRect();

	return CzVec4(0, 0, 0, 0);
}

/**
 @fn	CzVec4 CzSprite::FindFirstScreenClipRect2()

 @brief	Searches up the sprite hierarchy for the first valid screen clip rectangle.

 This method will check the current sprites screen clip rect 

 @return	The found screen clip rectangle 2.
 */

CzVec4 CzSprite::FindFirstScreenClipRect2()
{
	if (ScreenClipRect.w > 0)
		return ScreenClipRect;

	if (Parent != NULL)
	{
		return Parent->FindFirstScreenClipRect2();
	}
	else
		return Manager->getScreenClipRect();

	return CzVec4(0, 0, 0, 0);
}

/**
 @fn	void CzSprite::setClipRect(CzVec4& rc)

 @brief	Sets the sprites clip rectangle.

 @param [in,out]	rc	The clip rectangle.
 */

void CzSprite::setClipRect(CzVec4& rc)
{
	ClipRect = rc;
/*	// Ensure clip rect doesnt grow compared to parent
	int x1 = rc.x;
	int y1 = rc.y;
	int x2 = x1 + rc.w;
	int y2 = y1 + rc.h;

	CzIRect current = FindFirstClipRect();
	int cx1, cx2, cy1, cy2;
	cx1 = current.x;
	cy1 = current.y;
	cx2 = current.x + current.w;
	cy2 = current.y + current.h;
	if (x1 > cx1)
		cx1 = x1;
	if (x2 < cx2)
		cx2 = x2;
	if (y1 > cy1)
		cy1 = y1;
	if (y2 < cy2)
		cy2 = y2;

	ClipRect.x = cx1;
	ClipRect.y = cy1;
	ClipRect.w = (cx2 - cx1);
	ClipRect.h = (cy2 - cy1);*/

}

/**
 @fn	void CzSprite::TransformVertices()

 @brief	Transform sprites vertices by current sprite transform.

 */

void CzSprite::TransformVertices()
{
	BuildFinalTransform();

	TransformClipRect();

	float m00 = FinalTransform.m[0][0];
	float m01 = FinalTransform.m[0][1];
	float m10 = FinalTransform.m[1][0];
	float m11 = FinalTransform.m[1][1];
	float tx = FinalTransform.getX();
	float ty = FinalTransform.getY();
	float x0 = -Width / 2.0f;
	float y0 = -Height / 2.0f;
	if (Anchor == TopLeft)
	{
		tx -= x0;
		ty -= y0;
	}

	CzVec2* gv;
	CzVec2* screen_v = Prim->Verts;
	if (Geometry == NULL)
	{
		gv = Prim->Verts;
		// Generate transformed vertices
		float x1 = Width / 2.0f;
		float y1 = Height / 2.0f;
		float sx1 = Skew.x;
		float sx2 = Skew.y;
		float sy1 = Skew.z;
		float sy2 = Skew.w;
		CzVec2* v = gv;
		v->x = x0 + sx1;
		v->y = y0 + sy1; v++;
		v->x = x1 + sx2;
		v->y = y0 - sy1; v++;
		v->x = x1 - sx2;
		v->y = y1 - sy2; v++;
		v->x = x0 - sx1;
		v->y = y1 + sy2;
		for (int t = 0; t < Prim->VertCount; t++)
		{
			float x = gv->x;
			float y = gv->y;
			screen_v->x = ((m00 * x) + (m10 * y)) + tx;
			screen_v->y = ((m01 * x) + (m11 * y)) + ty;
			screen_v++; gv++;
		}
	}
	else
	{
		gv = Geometry->Verts;
		for (int t = 0; t < Prim->VertCount; t++)
		{
			float x = gv->x;
			float y = gv->y;
			if (Geometry->PercBased)
			{
				x = (Width * x) / 100.0f;
				y = (Height * y) / 100.0f;
			}
			screen_v->x = ((m00 * x) + (m10 * y)) + tx;
			screen_v->y = ((m01 * x) + (m11 * y)) + ty;
			screen_v++; gv++;
		}
	}
}

/**
 @fn	bool CzSprite::Update()

 @brief	Updates the sprite and any child sprites.

 @return	true if visible, false if not.
 */

bool CzSprite::Update()
{
	// Do not render if not visible
//	if (!Visible || Colour.a == 0)
	if (!Visible)
		return false;

#if defined(CZ_ENABLE_METRICS)
	CzMetrics::TotalSpritesProcessed++;
#endif

	bool dirty = TransformDirty;

	if (Orphan)
	{
		if (LinkedTo != NULL)
		{
			if (!LinkedTo->isVisible())
				return false;
			if (LinkedTo->isTransformDirty())
				TransformDirty = true;
		}
	}

	if (TransformDirty)
	{
		RebuildTransform();
		TransformVertices();
	}

	// Update children
	for (Iterator it = Children.begin(); it != Children.end(); ++it)
	{
		if (dirty)
			(*it)->forceTransformDirty();
		(*it)->Update();
	}

	return true;
}

/**
 @fn	void CzSprite::DrawChildren()

 @brief	Draw the sprites child sprites.

 */

void CzSprite::DrawChildren()
{
	if (Children.size() == 0)
		return;

	CzIRect current_clip = PLATFORM_RENDER->GetClipRect();

	// Set current clip rect
	if (ClipRect.w > 0)
		UpdateClipping();

	ChildChangeClip = false;

	CzIRect restored_clip = PLATFORM_RENDER->GetClipRect();

	// Draw children
	for (Iterator it = Children.begin(); it != Children.end(); ++it)
	{
		// Draw child
		(*it)->Draw();

		// if child or any of its children modified clipping then restore it
		if (ChildChangeClip)
		{
			if (ClipRect.w > 0)
				PLATFORM_RENDER->SetClipRect(restored_clip.x, restored_clip.y, restored_clip.w, restored_clip.h);
			else
				PLATFORM_RENDER->SetClipRect(current_clip.x, current_clip.y, current_clip.w, current_clip.h);		// We have no clip rect defined at this poibnt so we simply restore the clip rect that was there before we starting drawing the children
			ChildChangeClip = false;
		}
	}

	// Put the original clipping back
	if (ClipRect.w >= 0)
		PLATFORM_RENDER->SetClipRect(current_clip.x, current_clip.y, current_clip.w, current_clip.h);
}




