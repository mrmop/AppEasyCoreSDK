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

#include "CzGeometry.h"
#include "CzActor.h"
#include "CzScene.h"
#include "CzResources.h"
#include "CzHashes.h"
#include "CzMath.h"
#include "CzXomlResourceManager.h"


CzGeometry::~CzGeometry()
{
	SAFE_DELETE_ARRAY(Verts);
	SAFE_DELETE_ARRAY(UVs);
	SAFE_DELETE_ARRAY(Colours);
	SAFE_DELETE_ARRAY(Indices);
}

int CzGeometry::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	CzString*		vertices = NULL;
	CzString*		uvs = NULL;
	CzString*		colours = NULL;
	CzString*		indices = NULL;
	CzString*		type = NULL;
	int				vert_count = 0;
	int				index_count = 0;

	// Process attributes
	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
			setName((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Vertices_Hash)
			vertices = &(*it)->getValue();
		else
		if (name_hash == CzHashes::UV_Hash)
			uvs = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Colours_Hash)
			colours = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Indices_Hash)
			indices = &(*it)->getValue();
		else
		if (name_hash == CzHashes::Type_Hash)
			type = &(*it)->getValue();
		else
		if (name_hash == CzHashes::PercVerts_Hash)
			PercBased = (*it)->getValueAsBool();
		else
		if (name_hash == CzHashes::Count_Hash)
			vert_count = (*it)->getValueAsInt();
		else
		if (name_hash == CzHashes::IndexCount_Hash)
			index_count = (*it)->getValueAsInt();
	}

	if (type == NULL)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Geometry - Geometry type was not supplied - ", DebugInfo.c_str());
		return -1;
	}

	// Get primitive type
	eCzPrimType prim_type = PrimType_TriList;
	unsigned int type_hash = CzString::CalculateHash(type->c_str());
	if (type_hash == CzHashes::TriList_Hash)
		Type = PrimType_TriList;
	else
	if (type_hash == CzHashes::QuadList_Hash)
		Type = PrimType_QuadList;
	else
	if (type_hash == CzHashes::Poly_Hash)
		Type = PrimType_Poly;
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Geometry - Geometry type not supported - ", DebugInfo.c_str());
	}


	int index = 0;
	// Get vertices
	if (vertices != NULL)
	{
		index = 0;
		VertCount = vertices->getAsListOfFloat(CzXmlTools::FloatListPool) >> 1;
		if (vert_count < VertCount) vert_count = VertCount;
		Verts = new CzVec2[vert_count]();
		for (int t = 0; t < VertCount; t++)
		{
			Verts[t].x = CzXmlTools::FloatListPool[index++];
			Verts[t].y = CzXmlTools::FloatListPool[index++];
		}
	}
	else
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Geometry - Vertices are missing - ", DebugInfo.c_str());
		return -1;
	}

	// Get UV coordinates
	if (uvs != NULL)
	{
		index = 0;
		int count = uvs->getAsListOfFloat(CzXmlTools::FloatListPool) >> 1;
		if (count != VertCount)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Geometry - UV count is not the same as vertex count - ", DebugInfo.c_str());
		UVs = new CzVec2[vert_count]();
		for (int t = 0; t < count; t++)
		{
			UVs[t].x = CzXmlTools::FloatListPool[index++];
			UVs[t].y = CzXmlTools::FloatListPool[index++];
		}
	}

	// Get vertex colours
	if (colours != NULL)
	{
		index = 0;
		int count = colours->getAsListOfInt(CzXmlTools::IntListPool) >> 2;
		if (count != VertCount)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Geometry - Colours count is not the same as vertex count - ", DebugInfo.c_str());
		Colours = new CzColour[vert_count]();
		for (int t = 0; t < count; t++)
		{
			Colours[t].r = (uint8)(CzXmlTools::IntListPool[index++]);
			Colours[t].g = (uint8)(CzXmlTools::IntListPool[index++]);
			Colours[t].b = (uint8)(CzXmlTools::IntListPool[index++]);
			Colours[t].a = (uint8)(CzXmlTools::IntListPool[index++]);
		}
	}
	VertCount = vert_count;

	// Get vertex indices
	if (indices != NULL)
	{
		IndicesCount = indices->getAsListOfInt(CzXmlTools::IntListPool);
		if (index_count < IndicesCount) index_count = IndicesCount;
		Indices = new uint16[index_count];
		for (int t = 0; t < IndicesCount; t++)
			Indices[t] = (uint16)(CzXmlTools::IntListPool[t]);
	}
	else
	{
		IndicesCount = VertCount;
		if (index_count < IndicesCount) index_count = IndicesCount;
		Indices = new uint16[index_count];
		for (int t = 0; t < index_count; t++)
			Indices[t] = t;
	}
	IndicesCount = index_count;

	// Calculate face count
	switch (Type)
	{
	case PrimType_TriList:
		FaceCount = IndicesCount / 3;
		break;
	case PrimType_QuadList:
		FaceCount = IndicesCount / 4;
		break;
	case PrimType_Poly:
		FaceCount = 1;
		break;
	}

	// If we are declared inside a scene then geometry is local to the scene
	CzScene* scene = NULL;
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Actor_Hash)
		scene = ((CzActor*)parent)->getScene();
	else
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

	bool ret = false;
	if (scene != NULL)
		ret = scene->getResourceManager()->addResource(this);
	else
		ret = CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this);

	if (!ret)
		return 0;

	return 1;
}

void CzGeometry::CalculateDimensions(float& min_x, float& max_x, float& min_y, float& max_y)
{
	CzVec2* v = Verts;
	min_x = v->x;
	max_x = v->x;
	min_y = v->y;
	max_y = v->y;
	for (int t = 1; t < VertCount; t++)
	{
		v++;
		float x = v->x;
		float y = v->y;
		if (x < min_x)
			min_x = x;
		if (x > max_x)
			max_x = x;
		if (y < min_y)
			min_y = y;
		if (y > max_y)
			max_y = y;
	}
}

void CzGeometry::setCounts(int vertices, int indices)
{
	VertCount = vertices;
	IndicesCount = indices;
}

/**
 @fn	void CzGeometry::generateEllipse(float radius_x, float radius_y, float start_angle, float end_angle, int resolution, CzColour& colour)

 @brief	Geberates a circle geometry

 @param [in]	radius_x	X axis radius
 @param [in]	radius_y	Y axis radius
 @param [in]	start_angle	Start angle of ellipse in degrees
 @param [in]	end_angle	End angle of sllipse in degrees
 @param [in]	resolution	Resolution of sllipse segments
 @param [in]	colour		Colour of circle
 */

void CzGeometry::generateEllipse(float radius_x, float radius_y, float start_angle, float end_angle, int resolution, CzColour& colour)
{
	if (resolution == 0)
	{
		resolution = (int)(radius_x > radius_y ? radius_x : radius_y);
		if ((end_angle - start_angle) != 360.0f)
			resolution = (int)((resolution * (end_angle - start_angle)) / 360);
		if (resolution < 8) resolution = 8;
	}
	start_angle = start_angle * (2.0f * PI) / 360.0f;
	end_angle = end_angle * (2.0f * PI) / 360.0f;

	int nv = resolution + 1;
	int ni = (resolution - 1) * 3;

	// Allocate space for geometry data
	SAFE_DELETE_ARRAY(Verts)
	SAFE_DELETE_ARRAY(Colours)
	SAFE_DELETE_ARRAY(UVs)
	SAFE_DELETE_ARRAY(Indices)
	Verts = new CzVec2[nv]();
	Colours = new CzColour[nv]();
	VertCount = nv;
	Indices = new uint16[ni];
	IndicesCount = ni;
	FaceCount = resolution;
	Type = PrimType_TriList;

	// Generate circle geometry
	CzVec2* tv = Verts;
	CzColour* cv = Colours;

	tv->x = 0;
	tv->y = 0;
	tv++;
	cv->r = colour.r;
	cv->g = colour.g;
	cv->b = colour.b;
	cv->a = colour.a;
	cv++;
	float da = (end_angle - start_angle) / (resolution - 1);
	for (int i = 0; i < resolution; i++)
	{
		tv->x = radius_x * CzMath::cos(start_angle);
		tv->y = -radius_y * CzMath::sin(start_angle);
		tv++;
		cv->r = colour.r;
		cv->g = colour.g;
		cv->b = colour.b;
		cv->a = colour.a;
		cv++;
		start_angle += da;
	}

	// Set face indices
	uint16* ind = Indices + (resolution - 1) * 3 - 1;
	for (int i = 0; i < resolution - 1; i++)
	{
		*ind-- = 0;
		*ind-- = i + 1;
		*ind-- = (i < (resolution - 1)) ? i + 2 : 1;
	}
}

/**
 @fn	void CzGeometry::generateEllipseWire(float radius_x, float radius_y, float start_angle, float end_angle, int resolution, CzColour& colour, float thickness)

 @brief	Geberates a circle geometry

 @param [in]	radius_x	X axis radius
 @param [in]	radius_y	Y axis radius
 @param [in]	start_angle	Start angle of ellipse in degrees
 @param [in]	end_angle	End angle of sllipse in degrees
 @param [in]	resolution	Resolution of sllipse segments
 @param [in]	colour		Colour of circle
 @param [in]	thickness	Wire thickness
 */

void CzGeometry::generateEllipseWire(float radius_x, float radius_y, float start_angle, float end_angle, int resolution, CzColour& colour, float thickness)
{
	thickness /= 2.0f;
	if (resolution == 0)
	{
		resolution = (int)(radius_x > radius_y ? radius_x : radius_y);
		if ((end_angle - start_angle) != 360.0f)
			resolution = (int)((resolution * (end_angle - start_angle)) / 360);
		if (resolution < 8) resolution = 8;
	}
	start_angle = start_angle * (2.0f * PI) / 360.0f;
	end_angle = end_angle * (2.0f * PI) / 360.0f;

	int nv = resolution << 1;
	int ni = (resolution - 1) * 3 * 2;

	// Allocate space for geometry data
	SAFE_DELETE_ARRAY(Verts)
	SAFE_DELETE_ARRAY(Colours)
	SAFE_DELETE_ARRAY(UVs)
	SAFE_DELETE_ARRAY(Indices)
	Verts = new CzVec2[nv]();
	Colours = new CzColour[nv]();
	VertCount = nv;
	Indices = new uint16[ni];
	IndicesCount = ni;
	FaceCount = resolution;
	Type = PrimType_TriList;

	// Generate circle geometry
	CzVec2* tv = Verts;
	CzColour* cv = Colours;
	float da = (end_angle - start_angle) / (resolution - 1);
    for (int i = 0; i < resolution; i++)
    {
        float xs = radius_x * CzMath::cos(start_angle);
        float ys = -radius_y * CzMath::sin(start_angle);
        start_angle += da;
        float xe = radius_x * CzMath::cos(start_angle);
        float ye = -radius_y * CzMath::sin(start_angle);
        float dx = xe - xs;
        float dy = ye - ys;
        float d = CzMath::sqrt(dx * dx + dy * dy);
        float ddx = -dy * thickness / d;
        float ddy = dx * thickness / d;

        tv->x = xs - ddx;
		tv->y = ys - ddy;
		tv++;
		cv->r = colour.r;
		cv->g = colour.g;
		cv->b = colour.b;
		cv->a = colour.a;
		cv++;
        tv->x = xs + ddx;
		tv->y = ys + ddy;
		tv++;
		cv->r = colour.r;
		cv->g = colour.g;
		cv->b = colour.b;
		cv->a = colour.a;
		cv++;
    }

	// Set face indices
	uint16* ind = Indices;	// + (resolution - 1) * 3 - 1;
	int index = 0;
	for (int i = 0; i < resolution - 1; i++)
	{
		*ind++ = index;
		*ind++ = index + 2;
		*ind++ = index + 1;
		*ind++ = index + 2;
		*ind++ = index + 3;
		*ind++ = index + 1;
		index += 2;
	}
}

/**
 @fn	void CzGeometry::generateRect(float width, float height, CzColour& colour)

 @brief	Geberates a circle geometry

 @param [in]	width		Width of rect
 @param [in]	height		Height of rect
 @param [in]	colour		Colour of circle
 */

void CzGeometry::generateRect(float width, float height, CzColour& colour)
{
	width /= 2;
	height /= 2;
	int nv = 4;
	int ni = 4;

	// Allocate space for geometry data
	SAFE_DELETE_ARRAY(Verts)
	SAFE_DELETE_ARRAY(Colours)
	SAFE_DELETE_ARRAY(UVs)
	SAFE_DELETE_ARRAY(Indices)
	Verts = new CzVec2[nv]();
	Colours = new CzColour[nv]();
	VertCount = nv;
	Indices = new uint16[ni];
	IndicesCount = ni;
	FaceCount = 1;
	Type = PrimType_QuadList;

	CzVec2* tv = Verts;
	CzColour* cv = Colours;
	tv->x = -width;
	tv->y = -height;
	tv++;
	tv->x = width;
	tv->y = -height;
	tv++;
	tv->x = width;
	tv->y = height;
	tv++;
	tv->x = -width;
	tv->y = height;
	tv++;
	for (int t = 0; t < nv; t++)
	{
		cv->r = colour.r;
		cv->g = colour.g;
		cv->b = colour.b;
		cv->a = colour.a;
		cv++;
	}

	// Set face indices
	uint16* ind = Indices;
	*ind++ = 0;
	*ind++ = 1;
	*ind++ = 2;
	*ind++ = 3;
}

/**
 @fn	void CzGeometry::generateRectWire(float width, float height, CzColour& colour, float thickness)

 @brief	Geberates a circle geometry

 @param [in]	width		Width of rect
 @param [in]	height		Height of rect
 @param [in]	colour		Colour of circle
 @param [in]	thickness	Wire thickness
 */

void CzGeometry::generateRectWire(float width, float height, CzColour& colour, float thickness)
{
	width /= 2;
	height /= 2;
	int nv = 8;
	int ni = 16;

	// Allocate space for geometry data
	SAFE_DELETE_ARRAY(Verts)
	SAFE_DELETE_ARRAY(Colours)
	SAFE_DELETE_ARRAY(UVs)
	SAFE_DELETE_ARRAY(Indices)
	Verts = new CzVec2[nv]();
	Colours = new CzColour[nv]();
	VertCount = nv;
	Indices = new uint16[ni];
	IndicesCount = ni;
	FaceCount = 4;
	Type = PrimType_QuadList;

	CzVec2* tv = Verts;
	CzColour* cv = Colours;
	tv->x = -width;
	tv->y = -height;
	tv++;
	tv->x = width;
	tv->y = -height;
	tv++;
	tv->x = -width + thickness;
	tv->y = -height + thickness;
	tv++;
	tv->x = width - thickness;
	tv->y = -height + thickness;
	tv++;
	tv->x = -width + thickness;
	tv->y = height - thickness;
	tv++;
	tv->x = width - thickness;
	tv->y = height - thickness;
	tv++;
	tv->x = -width;
	tv->y = height;
	tv++;
	tv->x = width;
	tv->y = height;
	tv++;

	for (int t = 0; t < nv; t++)
	{
		cv->r = colour.r;
		cv->g = colour.g;
		cv->b = colour.b;
		cv->a = colour.a;
		cv++;
	}

	// Set face indices
	uint16* ind = Indices;
	*ind++ = 0;	*ind++ = 1;	*ind++ = 3;	*ind++ = 2;
	*ind++ = 4;	*ind++ = 5;	*ind++ = 7;	*ind++ = 6;
	*ind++ = 0;	*ind++ = 2;	*ind++ = 4;	*ind++ = 6;
	*ind++ = 3;	*ind++ = 1;	*ind++ = 7;	*ind++ = 5;
}

/**
 @fn	void CzGeometry::generateRoundedRect(float width, float height, CzColour& colour, float radius)

 @brief	Geberates a circle geometry

 @param [in]	width		Width of rect
 @param [in]	height		Height of rect
 @param [in]	colour		Colour of circle
 @param [in]	radius		Corner radius
 */

void CzGeometry::generateRoundedRect(float width, float height, CzColour& colour, float radius)
{
    int resolution = (int)((radius / 2) + 1);
	int nv = 12 + (resolution + 2) * 4;
	int ni = 6 * 3 + 4 * resolution * 3;

	// Allocate space for geometry data
	SAFE_DELETE_ARRAY(Verts)
	SAFE_DELETE_ARRAY(Colours)
	SAFE_DELETE_ARRAY(UVs)
	SAFE_DELETE_ARRAY(Indices)
	Verts = new CzVec2[nv]();
	Colours = new CzColour[nv]();
	VertCount = nv;
	Indices = new uint16[ni];
	IndicesCount = ni;
	FaceCount = ni / 3;
	Type = PrimType_TriList;

    float w = width / 2.0f;
    float h = height / 2.0f;
    if (radius < 0)
        radius = 1.0f;
    if (radius > w)
        radius = w;
    if (radius > h)
        radius = h;
    float x1 = -w;
    float x2 = -w + radius;
    float x3 = w - radius;
    float x4 = w;
    float y1 = -h;
    float y2 = -h + radius;
    float y3 = h - radius;
    float y4 = h;

	CzVec2* tv = Verts;
	CzColour* cv = Colours;
	tv->x = x2;
	tv->y = y1;
	tv++;
	tv->x = x3;
	tv->y = y1;
	tv++;
	tv->x = x3;
	tv->y = y4;
	tv++;
	tv->x = x2;
	tv->y = y4;
	tv++;

	tv->x = x1;
	tv->y = y2;
	tv++;
	tv->x = x2;
	tv->y = y2;
	tv++;
	tv->x = x2;
	tv->y = y3;
	tv++;
	tv->x = x1;
	tv->y = y3;
	tv++;

	tv->x = x3;
	tv->y = y2;
	tv++;
	tv->x = x4;
	tv->y = y2;
	tv++;
	tv->x = x4;
	tv->y = y3;
	tv++;
	tv->x = x3;
	tv->y = y3;
	tv++;

	for (int t = 0; t < nv; t++)
	{
		cv->r = colour.r;
		cv->g = colour.g;
		cv->b = colour.b;
		cv->a = colour.a;
		cv++;
	}

	// Set face indices
	uint16* ind = Indices;
	int index = 0;
	for (int t = 0; t < 3; t++)
	{
		*ind++ = index;
		*ind++ = index + 1;
		*ind++ = index + 2;
		*ind++ = index;
		*ind++ = index + 2;
		*ind++ = index + 3;
		index += 4;
	}

    float d2r = (2 * PI) / 360.0f;
    float da = 90.0f / (float)resolution * d2r;

    // Add top left corner vertices
    float start = 90.0f * d2r;
	tv->x = x2;
	tv->y = y2;
	tv++;
    for (int i = 0; i < resolution + 1; i++)
    {
		tv->x = x2 + radius * CzMath::cos(start);
		tv->y = y2 - radius * CzMath::sin(start);
		tv++;
        start += da;
    }

    // Add bottom left corner vertices
    start = 180.0f * d2r;
	tv->x = x2;
	tv->y = y3;
	tv++;
    for (int i = 0; i < resolution + 1; i++)
    {
		tv->x = x2 + radius * CzMath::cos(start);
		tv->y = y3 - radius * CzMath::sin(start);
		tv++;
        start += da;
    }

    // Add bottom right corner vertices
    start = 270.0f * d2r;
	tv->x = x3;
	tv->y = y3;
	tv++;
    for (int i = 0; i < resolution + 1; i++)
    {
		tv->x = x3 + radius * CzMath::cos(start);
		tv->y = y3 - radius * CzMath::sin(start);
		tv++;
        start += da;
    }

    // Add top right corner vertices
    start = 0 * d2r;
	tv->x = x3;
	tv->y = y2;
	tv++;
    for (int i = 0; i < resolution + 1; i++)
    {
		tv->x = x3 + radius * CzMath::cos(start);
		tv->y = y2 - radius * CzMath::sin(start);
		tv++;
        start += da;
    }

    // Add corner point indices
    for (int t = 0; t < 4; t++)
    {
        for (int i = 0; i < resolution; i++)
        {
			*ind++ = index;
			*ind++ = index + i + 2;
			*ind++ = index + i + 1;
        }
        index += resolution + 2;
    }
}

/**
 @fn	void CzGeometry::generateRoundedRectWire(float width, float height, CzColour& colour, float radius, float thickness)

 @brief	Geberates a circle geometry

 @param [in]	width		Width of rect
 @param [in]	height		Height of rect
 @param [in]	colour		Colour of circle
 @param [in]	radius		Corner radius
 @param [in]	thickness	Wire thickness
 */

void CzGeometry::generateRoundedRectWire(float width, float height, CzColour& colour, float radius, float thickness)
{
    int resolution = (int)(radius / 2) + 1;
    float r1 = radius - thickness;
	int nv = 16 + (resolution + 1) * 8;
	int ni = 6 * 4 + 4 * resolution * 6;

	// Allocate space for geometry data
	SAFE_DELETE_ARRAY(Verts)
	SAFE_DELETE_ARRAY(Colours)
	SAFE_DELETE_ARRAY(UVs)
	SAFE_DELETE_ARRAY(Indices)
	Verts = new CzVec2[nv]();
	Colours = new CzColour[nv]();
	VertCount = nv;
	Indices = new uint16[ni];
	IndicesCount = ni;
	FaceCount = ni / 3;
	Type = PrimType_TriList;

    float w = width / 2.0f;
    float h = height / 2.0f;
    if (radius < 0)
        radius = 1.0f;
    if (radius > w)
        radius = w;
    if (radius > h)
        radius = h;
    float x1 = -w + radius;
    float x2 = x1 + width - radius * 2;
    float x3 = -w;
    float x4 = -w + thickness;
    float x5 = w - thickness;
    float x6 = w;
    float y1 = -h;
    float y2 = -h + thickness;
    float y3 = -h + radius;
    float y4 = y3 + height - radius * 2;
    float y5 = h - thickness;
    float y6 = h;

	CzVec2* tv = Verts;
	CzColour* cv = Colours;
	tv->x = x1;
	tv->y = y1;
	tv++;
	tv->x = x2;
	tv->y = y1;
	tv++;
	tv->x = x2;
	tv->y = y2;
	tv++;
	tv->x = x1;
	tv->y = y2;
	tv++;

	tv->x = x3;
	tv->y = y3;
	tv++;
	tv->x = x4;
	tv->y = y3;
	tv++;
	tv->x = x4;
	tv->y = y4;
	tv++;
	tv->x = x3;
	tv->y = y4;
	tv++;

	tv->x = x5;
	tv->y = y3;
	tv++;
	tv->x = x6;
	tv->y = y3;
	tv++;
	tv->x = x6;
	tv->y = y4;
	tv++;
	tv->x = x5;
	tv->y = y4;
	tv++;

	tv->x = x1;
	tv->y = y5;
	tv++;
	tv->x = x2;
	tv->y = y5;
	tv++;
	tv->x = x2;
	tv->y = y6;
	tv++;
	tv->x = x1;
	tv->y = y6;
	tv++;

	for (int t = 0; t < nv; t++)
	{
		cv->r = colour.r;
		cv->g = colour.g;
		cv->b = colour.b;
		cv->a = colour.a;
		cv++;
	}

	// Set face indices
	uint16* ind = Indices;
	int index = 0;
	for (int t = 0; t < 4; t++)
	{
		*ind++ = index;
		*ind++ = index + 1;
		*ind++ = index + 2;
		*ind++ = index;
		*ind++ = index + 2;
		*ind++ = index + 3;
		index += 4;
	}

    x2 = -w + radius;
    x3 = w - radius;
    y2 = -h + radius;
    y3 = h - radius;

    float d2r = (2 * PI) / 360.0f;
    float da = 90.0f / (float)resolution * d2r;

    // Add top left corner vertices
    float start = 90.0f * d2r;
    for (int i = 0; i < resolution + 1; i++)
    {
		tv->x = x2 + radius * CzMath::cos(start);
		tv->y = y2 - radius * CzMath::sin(start);
		tv++;
		tv->x = x2 + r1 * CzMath::cos(start);
		tv->y = y2 - r1 * CzMath::sin(start);
		tv++;
        start += da;
    }

    // Add bottom left corner vertices
    start = 180.0f * d2r;
    for (int i = 0; i < resolution + 1; i++)
    {
		tv->x = x2 + radius * CzMath::cos(start);
		tv->y = y3 - radius * CzMath::sin(start);
		tv++;
		tv->x = x2 + r1 * CzMath::cos(start);
		tv->y = y3 - r1 * CzMath::sin(start);
		tv++;
        start += da;
    }

    // Add bottom right corner vertices
    start = 270.0f * d2r;
    for (int i = 0; i < resolution + 1; i++)
    {
		tv->x = x3 + radius * CzMath::cos(start);
		tv->y = y3 - radius * CzMath::sin(start);
		tv++;
		tv->x = x3 + r1 * CzMath::cos(start);
		tv->y = y3 - r1 * CzMath::sin(start);
		tv++;
        start += da;
    }

    // Add top right corner vertices
    start = 0 * d2r;
    for (int i = 0; i < resolution + 1; i++)
    {
		tv->x = x3 + radius * CzMath::cos(start);
		tv->y = y2 - radius * CzMath::sin(start);
		tv++;
		tv->x = x3 + r1 * CzMath::cos(start);
		tv->y = y2 - r1 * CzMath::sin(start);
		tv++;
        start += da;
    }

    // Add corner point indices
    for (int t = 0; t < 4; t++)
    {
        for (int i = 0; i < resolution; i++)
        {
			*ind++ = index;
			*ind++ = index + 1;
			*ind++ = index + 2;
			*ind++ = index + 1;
			*ind++ = index + 3;
			*ind++ = index + 2;
            index += 2;
        }
        index += 2;
    }
}

/**
 @fn	void CzGeometry::generateRoundedRectWire(CzColour& colour, CzVec2* verts, int count, float thickness)

 @brief	Geberates a circle geometry

 @param [in]	colour		Colour of circle
 @param [in]	verts		Polygon vertices
 @param [in]	count		Number of vertices in polygon
 @param [in]	thickness	Wire thickness
 */

void CzGeometry::generatePolygonWire(CzColour& colour, CzVec2* verts, int count, float thickness)
{
    thickness /= 2;

	int nv = (count + 1) * 4;
	int ni = count * 12;

	// Allocate space for geometry data
	SAFE_DELETE_ARRAY(Verts)
	SAFE_DELETE_ARRAY(Colours)
	SAFE_DELETE_ARRAY(UVs)
	SAFE_DELETE_ARRAY(Indices)
	Verts = new CzVec2[nv]();
	Colours = new CzColour[nv]();
	VertCount = nv;
	Indices = new uint16[ni];
	IndicesCount = ni;
	FaceCount = ni / 3;
	Type = PrimType_TriList;

    CzVec2* tv = Verts;
	int i1 = 0;
    int i2 = 1;
    for (int i = 0; i < count + 1; i++)
    {
		CzVec2* v1 = verts + i1;
		CzVec2* v2 = verts + i2;
        float dx = v2->x - v1->x;
        float dy = v2->y - v1->y;
        float d = CzMath::sqrt(dx * dx + dy * dy);
        float ddx = -dy * thickness / d;
        float ddy = dx * thickness / d;

        tv->x = -ddx;
		tv->y = -ddy;
		tv++;
        tv->x = ddx;
		tv->y = ddy;
		tv++;
        tv->x = -ddx;
		tv->y = -ddy;
		tv++;
        tv->x = ddx;
		tv->y = ddy;
		tv++;

        i1 = i2;
        i2++;
        if (i2 >= count) i2 = 0;

    }
	// Set face indices
	uint16* ind = Indices;
	int index = 0;
    for (int i = 0; i < count; i++)
    {
		*ind++ = index;
		*ind++ = index + 2;
		*ind++ = index + 1;
		*ind++ = index + 2;
		*ind++ = index + 1;
		*ind++ = index + 3;
		*ind++ = index + 2;
		*ind++ = index + 3;
		*ind++ = index + 4;
		*ind++ = index + 3;
		*ind++ = index + 4;
		*ind++ = index + 5;
        index += 4;
    }
}


