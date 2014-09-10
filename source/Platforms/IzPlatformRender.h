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

#if !defined(_CIZ_PLATFORM_RENDER_H_)
#define _CIZ_PLATFORM_RENDER_H_

#include "CzPlatform.h"

#define	PLATFORM_RENDER	PLATFORM->getRender()

class CzImage;
class CzFont;

enum eCzPrimType
{
	PrimType_TriList	= 0, 
	PrimType_TriStrip	= 1, 
	PrimType_TriFan		= 2, 
	PrimType_QuadList	= 3, 
	PrimType_QuadStrip	= 4, 
	PrimType_Poly		= 5, 
	PrimType_LineList	= 6, 
	PrimType_LineStrip	= 7, 
};

enum eCzAlphaMode
{
	AlphaMode_None, 
	AlphaMode_Half, 
	AlphaMode_Add, 
	AlphaMode_Sub, 
	AlphaMode_Blend, 
	AlphaMode_Normal, 
};

struct CzRenderMaterial
{
	CzImage*			Image;			///< Texture image
	bool				Filter;			///< Texture filtering enable
	bool				Tiled;			///< Texture coordinate tiling / clamping
	eCzAlphaMode		AlphaMode;		///< Alpha blending mode
};

struct IzRenderPrim
{
	eCzPrimType			Type;			///< Type of primitive
	CzVec2*				UVs;			///< Texture UV coordinmaes
	CzColour*			Colours;		///< Colour vertices
	uint16*				Indices;		///< Face indices
	uint16				VertCount;		///< Vertex count
	uint16				FaceCount;		///< Face count
	uint16				IndicesCount;	///< Indices count
	int16				MaterialID;		///< Material ID
	bool				SharedIndices;	///< Set to true if indices are shared and should not be deleted

	IzRenderPrim() : UVs(NULL), Colours(NULL), Indices(NULL), SharedIndices(false) {}
	~IzRenderPrim()
	{
		SAFE_DELETE_ARRAY(UVs);
		SAFE_DELETE_ARRAY(Colours);
		if (!SharedIndices)
			SAFE_DELETE(Indices);
	}

};

struct CzRenderPrim : public IzRenderPrim
{
	CzVec2*			Verts;			///< Screen vertices

	CzRenderPrim() : IzRenderPrim(), Verts(NULL) {}
	~CzRenderPrim()
	{
		SAFE_DELETE_ARRAY(Verts);
	}

};

struct CzRenderPrim3 : public IzRenderPrim
{
	CzVec3*			Verts;			///< 3D vertices
	CzVec3*			Normals;		///< 3D normals
	bool			ModelSpace;		///< Set to true if this primitive should be rendered in model space, false if view space

	CzRenderPrim3() : IzRenderPrim(), Verts(NULL), Normals(NULL), ModelSpace(false) {}
	~CzRenderPrim3()
	{
		SAFE_DELETE_ARRAY(Verts);
		SAFE_DELETE_ARRAY(Normals);
	}

};

//
//
//
//
// IzPlatformRender
//
//
//
//
class IzPlatformRender
{
	// Properties
protected:
	bool		Initialised;
public:
	bool		isInitialised() const	{ return Initialised; }
	// Properties end
public:
	virtual ~IzPlatformRender() {}
	// Init
	virtual int		Init(int max_primitives = 1024, int max_materials = 100) = 0;
	virtual void	Release() = 0;

	// Pre-post render
	virtual void	Begin() = 0;
	virtual void	End() = 0;

	// Primitive rendering
	virtual void	DrawPrimitives(CzRenderPrim* prims, CzRenderMaterial* materials, int num_prims, bool single_material) = 0;
	virtual void	DrawPrimitives(CzRenderPrim3* prims, CzRenderMaterial* materials, int num_prims, bool single_material) = 0;
	virtual void	DrawText(CzFontPreparedText prepared_text, CzFont* font, CzMatrix3* transform, const CzColour& colour, CzVec4& skew, bool filter, eCzAlphaMode alpha_mode) = 0;

	// Batch rendering
	virtual void	AddPrimtives(CzRenderPrim* prims, CzRenderMaterial* materials, int num_prims, bool single_material) = 0;
	virtual int		AddMaterial(CzRenderMaterial* material) = 0;
	virtual void	BatchDrawPrims(bool filter = true) = 0;

	// Global clipping
	virtual void	SetClipRect(int x, int y, int w, int h) = 0;
	virtual void	UpdateClipRect(int x, int y, int w, int h) = 0;
	virtual CzIRect	GetClipRect() const = 0;
	virtual void	ResetClipRect() = 0;
	virtual bool	isFullyClipped(const CzIRect& rc) const = 0;
	virtual void	setClipRectModified(bool modified) = 0;
	virtual bool	wasClipRectModified() const = 0;

	// Matrices and projection
	virtual void	SetOrthoProjection(float left, float right, float bottom, float top, float nearz, float farz) = 0;
	virtual void	SetPerspectiveMatrix(CzMatrix4* mat) = 0;
	virtual void	SetModelMatrix(CzMatrix4* mat) = 0;
	virtual void	SetViewMatrix(CzMatrix4* mat) = 0;
};


#endif	// _CIZ_PLATFORM_RENDER_H_
