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

#include "Marm/CzPlatformRender.h"
#include "IwGL.h"
#include "Iw2D.h"
#include "IwGxFont.h"

// TODO:
// - Move vertex list building into AddPrimitives from DrawPrimitives
// - Add support for pushing 9-patch primitives into batch system using quad lists
// - Add option to set type of primitive that batch renderer renders
// - Add support for rendering none textured polygons
// - Add support for batching polygons != 4 sides

//
//
//
// Marmalade Rendering system
//
//
//

IwGxPrimType CzPlatformRender::CzToIwGxPrimType[16] = 
{
	IW_GX_TRI_LIST, 
	IW_GX_TRI_STRIP, 
	IW_GX_TRI_FAN, 
	IW_GX_QUAD_LIST, 
	IW_GX_QUAD_STRIP, 
	IW_GX_NPOLY_LIST, 
	IW_GX_LINE_LIST, 
	IW_GX_LINE_STRIP, 
};

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
	BatchVerts = new CzVec2*[max_materials];
	BatchUVs = new CzVec2*[max_materials];
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
	CurrentTiled = false;
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
		CIwMaterial::AlphaMode am = (CIwMaterial::AlphaMode)materials->AlphaMode;	// TODO Add proper method to map Marmalade Alpha mode to Marmalade
		int vc = prims->VertCount;

		// Set up vertex streams
		if (prims->UVs != NULL)
		{
			IwGxSetUVStream((CIwFVec2*)prims->UVs, 0);
//			IwGxSetUVStream((CIwSVec2*)prims->UVs, 0, vc);
		}
		else
			IwGxSetUVStream(NULL);
		IwGxSetVertStreamScreenSpace((CIwFVec2*)prims->Verts, vc);
		if (prims->Colours != NULL)
			IwGxSetColStream((CIwColour*)prims->Colours);
		else
			IwGxSetColStream(NULL);
		IwGxSetNormStream(NULL);

		CzTexture texture = materials->Image->getTexture();
		CIwTexture* mt = static_cast<CIwTexture*>(texture);
		bool filter;
		if (materials->Image->isFilterSet())
			filter = mt->GetFiltering();
		else
			filter = materials->Filter;

		// Only create new render material if something important has changed
		if (texture != CurrentTexture || CurrentAlphaMode != materials->AlphaMode || CurrentFilter != filter || CurrentTexture == NULL || CurrentTiled != materials->Tiled)
		{
			CIwMaterial* mat = IW_GX_ALLOC_MATERIAL();
			mat->SetTexture(mt);
			mat->SetDepthWriteMode(CIwMaterial::DEPTH_WRITE_DISABLED);
			mat->SetClamping(!materials->Tiled);
			mat->SetFiltering(filter);
			mat->SetAlphaMode(am);
			mat->SetCullMode(CIwMaterial::CULL_BACK);
//			mat->SetCullMode(CIwMaterial::CULL_NONE);
			IwGxSetMaterial(mat);
			CurrentTexture = texture;
			CurrentAlphaMode = materials->AlphaMode;
			CurrentFilter = filter;
			CurrentTiled = materials->Tiled;
		}
		else
		{
			RedundantTextureCalls++;
		}

		// Render the primitive
		IwGxDrawPrims(CzToIwGxPrimType[prims->Type], prims->Indices, prims->IndicesCount);

		// Move to next primitive
		prims++;
		if (!single_material)
			materials++;
	}
}

void CzPlatformRender::DrawPrimitives(CzRenderPrim3* prims, CzRenderMaterial* materials, int num_prims, bool single_material)
{
	while (num_prims-- > 0)
	{
		CIwMaterial::AlphaMode am = (CIwMaterial::AlphaMode)materials->AlphaMode;	// TODO Add proper method to map Marmalade Alpha mode to Marmalade
		int vc = prims->VertCount;

		// Set up vertex streams
		if (prims->UVs != NULL)
			IwGxSetUVStream((CIwFVec2*)prims->UVs, 0);
		else
			IwGxSetUVStream(NULL);
		if (prims->ModelSpace)
			IwGxSetVertStreamModelSpace((CIwFVec3*)prims->Verts, vc);
		else
			IwGxSetVertStreamViewSpace((CIwFVec3*)prims->Verts, vc);
		IwGxSetColStream((CIwColour*)prims->Colours);
		IwGxSetNormStream((CIwFVec3*)prims->Normals);

		CzTexture texture = materials->Image->getTexture();
		CIwTexture* mt = static_cast<CIwTexture*>(texture);
		bool filter;
		if (materials->Image->isFilterSet())
			filter = mt->GetFiltering();
		else
			filter = materials->Filter;

		// Only create new render material if something important has changed
		if (texture != CurrentTexture || CurrentAlphaMode != materials->AlphaMode || CurrentFilter != filter || CurrentTexture == NULL || CurrentTiled != materials->Tiled)
		{
			CIwMaterial* mat = IW_GX_ALLOC_MATERIAL();
			mat->SetTexture(mt);
			mat->SetDepthWriteMode(CIwMaterial::DEPTH_WRITE_DISABLED);
			mat->SetClamping(!materials->Tiled);
			mat->SetFiltering(filter);
			mat->SetAlphaMode(am);
			mat->SetCullMode(CIwMaterial::CULL_BACK);
//			mat->SetCullMode(CIwMaterial::CULL_NONE);
			IwGxSetMaterial(mat);
			CurrentTexture = texture;
			CurrentAlphaMode = materials->AlphaMode;
			CurrentTiled = materials->Tiled;
			CurrentFilter = filter;
		}
		else
		{
			RedundantTextureCalls++;
		}
	
		// Render the primitive
		IwGxDrawPrims(CzToIwGxPrimType[prims->Type], prims->Indices, prims->IndicesCount);

		// Move to next primitive
		prims++;
		if (!single_material)
			materials++;
	}
}

void CzPlatformRender::DrawText(CzFontPreparedText prepared_text, CzFont* font, CzMatrix3* transform, const CzColour& colour, CzVec4& skew, bool filter, eCzAlphaMode alpha_mode)
{
	CIwMaterial::AlphaMode am = (CIwMaterial::AlphaMode)alpha_mode; // TODO Add proper method map Marmalade Alpha mode to Marmalade

	// Cache the transform
	float m00 = transform->m[0][0];
	float m01 = transform->m[0][1];
	float m10 = transform->m[1][0];
	float m11 = transform->m[1][1];
	float tx = transform->getX();
	float ty = transform->getY();
	float sx1 = (float)skew.x;
	float sx2 = (float)skew.y;
	float sy1 = (float)skew.z;
	float sy2 = (float)skew.w;
/*	CzIRect clip = ScreenClipRect;
	clip.x <<= 3;
	clip.y <<= 3;
	clip.w = clip.x + (clip.w << 3);
	clip.h = clip.y + (clip.h << 3);*/

	IwGxLightingOn();
	IwGxSetColStream(NULL);
	IwGxSetNormStream(NULL);

	CIwGxFont* mfont = static_cast<CIwGxFont*>(font->getFontHandle());
	CIwGxFontPreparedData* prep_text = static_cast<CIwGxFontPreparedData*>(prepared_text);

	// A font can consist of multiple materials so we need to process all of them
	for (int t = 0; t < mfont->GetNumberMaterials(); t++)
	{
		// Set UV stream
		uint32* char_ids;
		int num_chars = IwGxFontSetUVs(*prep_text, -1, t, &char_ids);

		// Generate transformed vertices from glyphs
		int nv = num_chars << 2;
		CzVec2* pVerts = IW_GX_ALLOC(CzVec2, nv);
		CzVec2* pVert = pVerts;
		if (m01 == 0 && m10 == 0)
		{
			// No rotation optimisation
			for (int t2 = 0; t2 < num_chars; t2++)
			{
				CIwRect rc = prep_text->GetCharacterArea(char_ids[t2]);
				float x1 = (float)rc.x;
				float y1 = (float)rc.y;
				float x2 = x1 + (float)rc.w;
				float y2 = y1 + (float)rc.h;
				float ax = (m00 * x1) + tx;
				float ay = (m11 * y1) + ty;
				float bx = (m00 * x2) + tx;
				float by = (m11 * y2) + ty;

//				if ((ax < clip.w && bx >= clip.x) && (ay < clip.h && by >= clip.y))
//				{
					pVert->x = ax + sx1;
					pVert->y = ay + sy1;
					pVert++;
					pVert->x = ax - sx1;
					pVert->y = by + sy2;
					pVert++;
					pVert->x = bx - sx2;
					pVert->y = by - sy2;
					pVert++;
					pVert->x = bx + sx2;
					pVert->y = ay - sy1;
					pVert++;
//				}
			}
		}
		else
		{
			for (int t2 = 0; t2 < num_chars; t2++)
			{
				CIwRect rc = prep_text->GetCharacterArea(char_ids[t2]);
				float x1 = (float)rc.x;
				float y1 = (float)rc.y;
				float x2 = x1 + (float)rc.w;
				float y2 = y1 + (float)rc.h;
				pVert->x = (m00 * x1 + m10 * y1 + tx + sx1);
				pVert->y = (m01 * x1 + m11 * y1 + ty + sy1);
				pVert++;
				pVert->x = (m00 * x1 + m10 * y2 + tx - sx1);
				pVert->y = (m01 * x1 + m11 * y2 + ty + sy2);
				pVert++;
				pVert->x = (m00 * x2 + m10 * y2 + tx - sx2);
				pVert->y = (m01 * x2 + m11 * y2 + ty - sy2);
				pVert++;
				pVert->x = (m00 * x2 + m10 * y1 + tx + sx2);
				pVert->y = (m01 * x2 + m11 * y1 + ty - sy1);
				pVert++;
			}
		}

		if (nv > 0)
		{
			// Set vertex stream
			IwGxSetVertStreamScreenSpace((CIwFVec2*)pVerts, nv);

			// Create a material
			CIwMaterial* mat = IW_GX_ALLOC_MATERIAL();
			mat->Copy(*IwGxFontGetFont()->GetMaterial(t));
			mat->SetDepthWriteMode(CIwMaterial::DEPTH_WRITE_DISABLED);
			mat->SetColEmissive(colour.get());
			mat->SetClamping(true);
			mat->SetFiltering(filter);
			mat->SetAlphaMode(am);
			mat->SetCullMode(CIwMaterial::CULL_BACK);
//			mat->SetCullMode(CIwMaterial::CULL_NONE);
			IwGxSetMaterial(mat);

			// Finally draw the glyphs
			IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, nv);

			CurrentAlphaMode = alpha_mode;
		}
	}
	IwGxLightingOff();
	CurrentTexture = NULL;
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
	if (NextMaterial == 0)
		return;

	// Allocate memory from data cache for verts, UV's and colours
	for (int t = 0; t < NextMaterial; t++)
	{
		int prim_count = MaterialUsedCounts[t] * 4;
		BatchVerts[t] = IW_GX_ALLOC(CzVec2, prim_count);
		BatchUVs[t] = IW_GX_ALLOC(CzVec2, prim_count);
		BatchColours[t] = IW_GX_ALLOC(CzColour, prim_count);
		BatchIndices[t] = IW_GX_ALLOC(uint16, prim_count);
	}

	// Populate the data cache
	CzRenderPrim** prims = Primitives;
	for (int t = 0; t < NextPrimitive; t++)
	{
		CzRenderPrim *prim = *prims;
		int mat_id = prim->MaterialID;
		int idx = MaterialIndices[mat_id];
		CzVec2* v = BatchVerts[mat_id] + idx;
		CzVec2* uv = BatchUVs[mat_id] + idx;
		CzColour* c = BatchColours[mat_id] + idx;
		uint16* i = BatchIndices[mat_id] + idx;
		for (int t2 = 0; t2 < 4; t2++)
		{
			v->x = (prim->Verts + t2)->x;
			v->y = (prim->Verts + t2)->y;
			uv->x = (prim->UVs + t2)->x;
			uv->y = (prim->UVs + t2)->y;
			c->set(*(prim->Colours + t2));
			v++; uv++; c++;
		}
// TODO: ERROR - Does not work with batched ngon
		*i++ = idx;
		*i++ = idx + 3;
		*i++ = idx + 2;
		*i++ = idx + 1;
		MaterialIndices[mat_id] += 4;
		prims++;
	}

	// Render batched streams
	CzRenderMaterial** mats = Materials;
	for (int t = 0; t < NextMaterial; t++)
	{
		int count = MaterialUsedCounts[t] * 4;
		IwGxSetUVStream((CIwFVec2*)BatchUVs[t], 0);
		IwGxSetVertStreamScreenSpace((CIwFVec2*)BatchVerts[t], count);
		IwGxSetColStream((CIwColour*)(BatchColours[t]), count);
		IwGxSetNormStream(NULL);
		CIwMaterial* mat = IW_GX_ALLOC_MATERIAL();
		CIwTexture* texture = static_cast<CIwTexture*>((*mats)->Image->getTexture());
		mat->SetTexture(texture);
		mat->SetDepthWriteMode(CIwMaterial::DEPTH_WRITE_DISABLED);
		mat->SetClamping(false);
		mat->SetFiltering(filter);
		mat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		mat->SetCullMode(CIwMaterial::CULL_BACK);
//		mat->SetCullMode(CIwMaterial::CULL_NONE);
		IwGxSetMaterial(mat);
		IwGxDrawPrims(IW_GX_QUAD_LIST, BatchIndices[t], count);
		mats++;
	}

	// Reset batch pointers
	for (int t = 0; t < NextMaterial; t++)
		MaterialUsedCounts[t] = 0;
	for (int t = 0; t < NextMaterial; t++)
		MaterialIndices[t] = 0;
	NextPrimitive = 0;
	NextMaterial = 0;
}

CzIRect	CzPlatformRender::ScreenClipRect = CzIRect(0, 0, 0, 0);
bool CzPlatformRender::ClipRectModified = false;

void CzPlatformRender::ResetClipRect()
{
	if (ScreenClipRect.w != 0)
	{
		IwGxClearScissorScreenSpace();
		ScreenClipRect.w = 0;
		ClipRectModified = true;
	}
}
void CzPlatformRender::SetClipRect(int x, int y, int w, int h)
{
	if (x != ScreenClipRect.x || y != ScreenClipRect.y || w != ScreenClipRect.w || h != ScreenClipRect.h)
	{
//		if (w == 0)
//			ResetClipRect();
//		else
		{
			ScreenClipRect.x = x;
			ScreenClipRect.y = y;
			ScreenClipRect.w = w;
			ScreenClipRect.h = h;
//			IwGxClearScissorScreenSpace();
			IwGxSetScissorScreenSpace(x, y, w, h);
			ClipRectModified = true;
		}
	}
}

void CzPlatformRender::UpdateClipRect(int x, int y, int w, int h)
{
	int x1 = x;
	int y1 = y;
	int x2 = x + w;
	int y2 = y + h;
	int cx1 = ScreenClipRect.x;
	int cy1 = ScreenClipRect.y;
	int cx2 = ScreenClipRect.x + ScreenClipRect.w;
	int cy2 = ScreenClipRect.y + ScreenClipRect.h;

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
	ScreenClipRect.w = (cx2 - cx1);
	ScreenClipRect.h = (cy2 - cy1);
	IwGxSetScissorScreenSpace(ScreenClipRect.x, ScreenClipRect.y, ScreenClipRect.w, ScreenClipRect.h);
	ClipRectModified = true;
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
	IwGxSetOrtho((iwfixed)left, (iwfixed)right, (iwfixed)bottom, (iwfixed)top, nearz, farz);
}

void CzPlatformRender::SetPerspectiveMatrix(CzMatrix4* mat)
{
	IwGxSetPerspectiveMatrix(&mat->m[0][0]);
}

void CzPlatformRender::SetViewMatrix(CzMatrix4* mat)
{
	CIwFMat m;
	m.SetIdentity();
	m.m[0][0] = mat->m[0][0];
	m.m[1][0] = mat->m[1][0];
	m.m[2][0] = mat->m[2][0];
	m.m[0][1] = mat->m[0][1];
	m.m[1][1] = mat->m[1][1];
	m.m[2][1] = mat->m[2][1];
	m.m[0][2] = mat->m[0][2];
	m.m[1][2] = mat->m[1][2];
	m.m[2][2] = mat->m[2][2];
	m.t.x = mat->m[0][3];
	m.t.y = mat->m[1][3];
	m.t.z = mat->m[2][3];

	IwGxSetViewMatrix(&m);
}

void CzPlatformRender::SetModelMatrix(CzMatrix4* mat)
{
	CIwFMat m;
	m.SetIdentity();
	m.m[0][0] = mat->m[0][0];
	m.m[1][0] = mat->m[1][0];
	m.m[2][0] = mat->m[2][0];
	m.m[0][1] = mat->m[0][1];
	m.m[1][1] = mat->m[1][1];
	m.m[2][1] = mat->m[2][1];
	m.m[0][2] = mat->m[0][2];
	m.m[1][2] = mat->m[1][2];
	m.m[2][2] = mat->m[2][2];
	m.t.x = mat->m[0][3];
	m.t.y = mat->m[1][3];
	m.t.z = mat->m[2][3];

	IwGxSetModelMatrix(&m);
}







