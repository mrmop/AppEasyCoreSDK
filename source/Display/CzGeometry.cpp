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







