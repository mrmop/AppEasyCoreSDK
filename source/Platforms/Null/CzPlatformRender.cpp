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

#include "CzPlatformRender.h"

// TODO:
// - Move vertex list building into AddPrimitives from DrawPrimitives
// - Add support for pushing 9-patch primitives into batch system using quad lists
// - Add option to set type of primitive that batch renderer renders
// - Add support for rendering none textured polygons
// - Add support for batching polygons != 4 sides

int CzPlatformRender::Init(int max_primitives, int max_materials)
{
	Primitives = new CzRenderPrim*[max_primitives];
	NextPrimitive = 0;
	MaxPrimitives = max_primitives;
	Materials = new CzRenderMaterial*[max_materials];
	NextMaterial = 0;
	MaxMaterials = max_materials;
	MaterialUsedCounts = new int [max_materials];
	for (int t = 0; t < max_materials; t++)
		MaterialUsedCounts[t] = 0;
	MaterialIndices = new int [max_materials];	
	for (int t = 0; t < max_materials; t++)
		MaterialIndices[t] = 0;
	BatchVerts = new CzSVec2*[max_materials];
	BatchUVs = new CzSVec2*[max_materials];
	BatchColours = new CzColour*[max_materials];
	BatchIndices = new uint16*[max_materials];

	Initialised = true;

	return 1;
}

void CzPlatformRender::Release()
{
	SAFE_DELETE(Primitives)
	SAFE_DELETE(Materials)
	SAFE_DELETE(MaterialUsedCounts)
	SAFE_DELETE(MaterialIndices)
	SAFE_DELETE(BatchVerts)
	SAFE_DELETE(BatchUVs)
	SAFE_DELETE(BatchColours)
	SAFE_DELETE(BatchIndices)
}

void CzPlatformRender::Begin()
{
	RedundantTextureCalls = 0;
	CurrentTexture = NULL;
	CurrentAlphaMode = AlphaMode_None;
}

void CzPlatformRender::End()
{
//	CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "RedundantTextureCalls - ", CzString(RedundantTextureCalls).c_str());
}

void CzPlatformRender::DrawPrimitives(CzRenderPrim* prims, CzRenderMaterial* materials, int num_prims, bool single_material)
{
	while (num_prims-- > 0)
	{
	}
}

void CzPlatformRender::DrawPrimitives(CzRenderPrim3* prims, CzRenderMaterial* materials, int num_prims, bool single_material)
{
	while (num_prims-- > 0)
	{
	}
}

void CzPlatformRender::DrawText(CzFontPreparedText prepared_text, CzFont* font, CzMatrix3* transform, const CzColour& colour, CzVec4& skew, bool filter, eCzAlphaMode alpha_mode)
{
}

int CzPlatformRender::AddMaterial(CzRenderMaterial* material)
{
	// If same type of material already exists then return it
	CzRenderMaterial** mats = Materials;
	for (int t = 0; t < NextMaterial; t++)
	{
		CzRenderMaterial* m = *mats;
		if (m->Image == material->Image && m->Filter == material->Filter && m->Tiled == material->Tiled && m->AlphaMode == material->AlphaMode)
		{
			MaterialUsedCounts[t]++;
			return t;
		}
		mats++;
	}

	// Same type of material not found so add new one
#if defined(_DEBUG)
	if (NextMaterial >= MaxMaterials - 1)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "CzPlatformRender - Ran out of materials, please increase maximum in CzPlatformRender::Init()");
		return - 1;
	}
#endif
	*(Materials + NextMaterial) = material;
	MaterialUsedCounts[NextMaterial++] = 1;

	return NextMaterial - 1;
}

void CzPlatformRender::AddPrimtives(CzRenderPrim* prims, CzRenderMaterial* materials, int num_prims, bool single_material)
{
#if defined(_DEBUG)
	if ((NextPrimitive + num_prims) >= MaxPrimitives)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_INFO, "CzPlatformRender - Ran out of primitives, please increase maximum in CzPlatformRender::Init()");
		return;
	}
#endif
	int mat_id = -1;
	if (single_material)
		mat_id = AddMaterial(materials);

	while (num_prims-- > 0)
	{
		if (!single_material)
			mat_id = AddMaterial(materials++);
		CzRenderPrim** prim = Primitives + NextPrimitive++;
		prims->MaterialID = mat_id;
		*prim = prims++;
	}
}

// BatchDrawPrims - Currently only supports batching of Quad lists
void CzPlatformRender::BatchDrawPrims(bool filter)
{
}

CzIRect	CzPlatformRender::ScreenClipRect = CzIRect(0, 0, 0, 0);
bool CzPlatformRender::ClipRectModified = false;

void CzPlatformRender::ResetClipRect()
{
}
void CzPlatformRender::SetClipRect(int x, int y, int w, int h)
{
}

void CzPlatformRender::UpdateClipRect(int x, int y, int w, int h)
{
}

bool CzPlatformRender::isFullyClipped(const CzIRect &rc) const
{
	int left = ScreenClipRect.x;
	int top = ScreenClipRect.y;
	int right = left + ScreenClipRect.w;
	int bottom = top + ScreenClipRect.h;
	int x1 = rc.x;
	int x2 = x1 + rc.w;
	int y1 = rc.y;
	int y2 = y1 + rc.h;

	if (x2 < left || x1 > right || y2 < top || y2 > bottom)
		return true;

	return false;
}

void CzPlatformRender::SetOrthoProjection(float left, float right, float bottom, float top, float nearz, float farz)
{
}

void CzPlatformRender::SetPerspectiveMatrix(CzMatrix4* mat)
{
}

void CzPlatformRender::SetViewMatrix(CzMatrix4* mat)
{
}

void CzPlatformRender::SetModelMatrix(CzMatrix4* mat)
{
}







