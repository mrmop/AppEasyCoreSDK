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

#if !defined(_CZ_BOX2D_GEOMETRY_H_)
#define _CZ_BOX2D_GEOMETRY_H_

#include "CzUtil.h"
#include "CzString.h"
#include "CzXoml.h"
#include "IzPlatformRender.h"

/**
 @addtogroup Display
 @{
 */


/**
 @struct	CzGeometry

 @brief	CzGeometry - A renderable geometry shape or collection of shapes.

 Geometries are resources that contain collections of vertices, UV texture coordinates, RGBA colours and face vertex index lists that can be used to 
 create 2D geometry that can be attached to Actors to modify their visual shape. Currently 3 types of geometry are supported by XOML:
 - Poly – A convex polygon with any number of vertices
 - TriList – A list of triangles
 - QuadList – A list of quadrangles

 Note that concave polygons are not supported however you can create the same concave shape using a triangle list. A geometry resource is declared 
 using the Geometry XOML tag, e.g:

 @par XOML Example:
 @code
<!--Create a single triangle-->
<Geometry Name="Geoms1" Vertices="-200,-200,200,200,-200,200" UV="0,0,1,1,0,1" Type="TriList" />
<!--Create two triangles-->
<Geometry Name="Geoms2" Vertices="-200,-200,200,200,-200,200,-400,-400,-200,-400,-200,0" Type="TriList" />
<!--Create a house shape -->
<Geometry Name="Geoms3" Vertices="-200,-200,0,-300,200,-200,200,200,-200,200"  Indices="0,1,2,3,4" Colours="255,0,0,255,0,255,0,255,0,0,255,255,255,255,255,255,255,255,255,255" Type="Poly"/>
 @endcode

 The above XOML shows 3 different examples of creating geometries.

 The Geometry XOML tag has the following properties:

 - Name (string) – The name of the geometry
 - Tag (string) – Resource tag name
 - Vertices (vec2 list) – A list of pairs of x,y coordinates that describe the shape of the geometry in 2D
 - UV (vec2 list) – A list of pairs of x,y texture coordinates that describe how the assigned image is mapped to the polygon(s) vertices. If this 
 not supplied then it is automatically calculated
 - Colours (vec4 list) – A list of RGBA colours that are assigned to each vertex. If this is not supplied then all vertices will be assigned RGBA 
 of 255,255,255,255
 - Indices (list of numbers)– The face index list which determines the order in which vertices form face. If this is not supplied then this will 
 be automatically be generated
 - Type (geometry-type) – The type of geometry:
   - Poly – A polygon
   - TriList – A list of triangles
   - QuadList – A list of quadrangles
 - PercVerts (boolean) – If set to true then the geometries vertices will be treated as proportional percentage based. The geometry when attached 
 to an actor will be resized to fit the width and height of the host actor.

 Assigning a geometry to an Actor will force the actor to render the geometry in place of its default rectangular shape, e.g:

 @code
<Icon Name="Sprite1" Position="200,0" Background="bg1" Geometry="Geoms1" />
 @endcode

 Note that hit detection and overlap detection is supported for all types of geometry, including disconnected geometry.

 */

struct CzGeometry : public IzXomlResource
{
public:
	// Properties
	eCzPrimType			Type;			///< Type of primitive
	CzVec2*				Verts;			///< Screen vertices
	CzVec2*				UVs;			///< Texture UV coordinmaes
	CzColour*			Colours;		///< Colour vertices
	uint16*				Indices;		///< Face indices
	uint16				VertCount;		///< Vertex count
	uint16				FaceCount;		///< Face count
	uint16				IndicesCount;	///< indices count
	bool				PercBased;		///< If true then the vertices are pecentage based
	// Properties end
public:
	CzGeometry() : Verts(NULL), UVs(NULL), Colours(NULL), Indices(NULL), VertCount(0), FaceCount(0), IndicesCount(0), PercBased(false) { setClassType("geometry"); }
	~CzGeometry();

	// Implementation of IzXomlResource interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	void				CalculateDimensions(float& min_x, float& max_x, float& min_y, float& max_y);

	void				setCounts(int vertices, int indices);
};

/**
 @class	CzGeometryCreator

 @brief	CzGeometryCreator - Creates an instance of a geometry shape.

 */

class CzGeometryCreator : public IzXomlClassCreator
{
public:
	CzGeometryCreator()
	{
		setClassName("geometry");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzGeometry(); }
};

/// @}


#endif // _CZ_BOX2D_GEOMETRY_H_
