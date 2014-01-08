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

#if !defined(_CZ_SHAPES_H_)
#define _CZ_SHAPES_H_


#include "CzUtil.h"
#include "CzXoml.h"

// 
// 
//
//
// CzShape - A basic shape class
// 
// 
// 
//

struct CzGeomShape
{
	enum eGeomShapeType
	{
		ST_None, 
		ST_Box, 
		ST_Circle, 
		ST_Polygon, 
	};

	eGeomShapeType		ShapeType;				// Type of shape
	virtual ~CzGeomShape() {}
};

struct CzGeomShapeBox : public CzGeomShape
{
	float		Width, Height;						// Dimensions
	CzGeomShapeBox()								{ ShapeType = ST_Box; }
};

struct CzGeomShapeCircle : public CzGeomShape
{
	float		Radius;								// Radius of circle
	CzGeomShapeCircle()								{ ShapeType = ST_Circle; }
};

struct CzGeomShapePolygon : public CzGeomShape
{
	CzVec2*		Vertices;							// Radius of circle
	int			NumVertices;						// Number of vertices in the polygon
	CzGeomShapePolygon() : Vertices(NULL)			{ ShapeType = ST_Polygon; }
	virtual ~CzGeomShapePolygon()					{ SAFE_DELETE_ARRAY(Vertices) }
};

class CzShape : public IzXomlResource
{
public:
	// Properties
protected:
	CzGeomShape*			Shape;					// Shape data
public:
	void					setShape(CzGeomShape* shape)	{ Shape = shape; }
	CzGeomShape*			getShape()						{ return Shape; }
	// Properties end

public:
	CzShape() : IzXomlResource(), Shape(NULL)		{ setClassType("shape"); }
	virtual ~CzShape()								{ SAFE_DELETE(Shape); }

	// Implementation of IzXomlClass interface
	int						LoadFromXoml(IzXomlResource* parebt, bool load_children, CzXmlNode* node);
};

//
//
//
//
//  CzShapeCreator - Creates an instance of a shape
//
//
//
//
class CzShapeCreator : public IzXomlClassCreator
{
public:
	CzShapeCreator()
	{
		setClassName("shape");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzShape(); }
};




#endif	// _CZ_SHAPES_H_
