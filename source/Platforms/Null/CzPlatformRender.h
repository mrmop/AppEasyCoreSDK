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

#if !defined(_CCZ_PLATFORM_RENDER_H_)
#define _CCZ_PLATFORM_RENDER_H_

#include "IzPlatformRender.h"
#include "CzRender.h"



class CzPlatformRender : public IzPlatformRender
{
	// Properties
protected:
	CzTexture				CurrentTexture;
	eCzAlphaMode			CurrentAlphaMode;
	bool					CurrentFilter;
	int						RedundantTextureCalls;
	CzRenderPrim**			Primitives;
	int						NextPrimitive;
	int						MaxPrimitives;
	CzRenderMaterial**		Materials;
	int*					MaterialUsedCounts;
	int						NextMaterial;
	int						MaxMaterials;
	int*					MaterialIndices;
	CzSVec2**				BatchVerts;
	CzSVec2**				BatchUVs;
	CzColour**				BatchColours;
	uint16**				BatchIndices;

	static bool				ClipRectModified;		// true if clipping rect has been modified
	static CzIRect			ScreenClipRect;			// Current transformed screen clipping rect
public:
	// Properties end
protected:

public:
	// Init
	int				Init(int max_primitives = 1024, int max_materials = 100);
	void			Release();

	// Pre-post render
	void			Begin();
	void			End();

	// Primitive rendering
	void			DrawPrimitives(CzRenderPrim* prims, CzRenderMaterial* materials, int num_prims, bool single_material);
	void			DrawPrimitives(CzRenderPrim3* prims, CzRenderMaterial* materials, int num_prims, bool single_material);
	void			DrawText(CzFontPreparedText prepared_text, CzFont* font, CzMatrix3* transform, const CzColour& colour, CzVec4& skew, bool filter, eCzAlphaMode alpha_mode);

	// Batch rendering
	void			AddPrimtives(CzRenderPrim* prims, CzRenderMaterial* materials, int num_prims, bool single_material);
	int				AddMaterial(CzRenderMaterial* material);
	void			BatchDrawPrims(bool filter = true);

	// Global clipping
	void			SetClipRect(int x, int y, int w, int h);
	void			UpdateClipRect(int x, int y, int w, int h);
	CzIRect			GetClipRect() const						{ return ScreenClipRect; }
	void			ResetClipRect();
	bool			isFullyClipped(const CzIRect& rc) const;
	void			setClipRectModified(bool modified)		{ ClipRectModified = modified; }
	bool			wasClipRectModified() const				{ return ClipRectModified; }

	// Matrices and projection
	void			SetOrthoProjection(float left, float right, float bottom, float top, float nearz, float farz);
	void			SetPerspectiveMatrix(CzMatrix4* mat);
	void			SetModelMatrix(CzMatrix4* mat);
	void			SetViewMatrix(CzMatrix4* mat);
};


#endif	// _CCZ_PLATFORM_RENDER_H_
