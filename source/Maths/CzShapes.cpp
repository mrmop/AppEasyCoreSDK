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

#include "CzShapes.h"
#include "CzUtil.h"
#include "CzXoml.h"
#include "CzScene.h"
#include "CzActor.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"


//
//
//
//
// CzShape implementation
//
//
//
//

int CzShape::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Process attributes
	float	width = 0, height = 0, radius = 0;
	CzGeomShape::eGeomShapeType type = CzGeomShape::ST_None;
	CzString* points = NULL;
	CzString* convex_counts = NULL;
	int num_convex = 0;

	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); it++)
	{
		unsigned int name_hash = (*it)->getName().getHash();

		if (name_hash == CzHashes::Name_Hash)
		{
			setName((*it)->getValue().c_str());
		}
		else
		if (name_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (name_hash == CzHashes::Type_Hash)
		{
			unsigned int type_hash = CzString::CalculateHash((*it)->getValue().c_str());
			if (type_hash == CzHashes::box_Hash)
				type = CzGeomShape::ST_Box;
			else
			if (type_hash == CzHashes::circle_Hash)
				type = CzGeomShape::ST_Circle;
			else
			if (type_hash == CzHashes::polygon_Hash)
				type = CzGeomShape::ST_Polygon;
		}
		else
		if (name_hash == CzHashes::Width_Hash)
			width = (*it)->getValueAsFloat();
		else
		if (name_hash == CzHashes::Height_Hash)
			height = (*it)->getValueAsFloat();
		else
		if (name_hash == CzHashes::Radius_Hash)
			radius = (*it)->getValueAsFloat();
		else
		if (name_hash == CzHashes::Points_Hash)
			points = &(*it)->getValue();
		else
		if (name_hash == CzHashes::ConvexCounts_Hash)
			convex_counts = &(*it)->getValue();
		else
		if (name_hash == CzHashes::NumConvex_Hash)
			num_convex = (*it)->getValueAsInt();
	}

	if (type == CzGeomShape::ST_None)
	{
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Shape - Invalid shape type", DebugInfo.c_str());
		return -1;
	}
	else
	if (type == CzGeomShape::ST_Box)
	{
		CzGeomShapeBox* shape = new CzGeomShapeBox();
		shape->ShapeType = type;
		shape->Width = width;
		shape->Height = height;
		Shape = shape;
	}
	else
	if (type == CzGeomShape::ST_Circle)
	{
		CzGeomShapeCircle* shape = new CzGeomShapeCircle();
		shape->ShapeType = type;
		shape->Radius = radius;
		Shape = shape;
	}
	else
	if (type == CzGeomShape::ST_Polygon)
	{
		CzGeomShapePolygon* shape = new CzGeomShapePolygon();
		shape->ShapeType = type;
		Shape = shape;

		if (convex_counts != NULL && num_convex > 0)
		{
			shape->NumConvex = num_convex;
			int count = convex_counts->getAsListOfInt(CzXmlTools::IntListPool);
			shape->ConvexCounts = new int [count];
			for (int t = 0; t < count; t++)
				shape->ConvexCounts[t] = CzXmlTools::IntListPool[t];
		}

		if (points != NULL)
		{
			shape->NumVertices = points->getAsListOfFloat(CzXmlTools::FloatListPool) >> 1;
			shape->Vertices = new CzVec2[shape->NumVertices]();
			int index = 0;
			for (int t = 0; t < shape->NumVertices; t++)
			{
				shape->Vertices[t].x = CzXmlTools::FloatListPool[index++];
				shape->Vertices[t].y = CzXmlTools::FloatListPool[index++];
			}
		}
		else
		{
			// Calculate number of points
			int num_points = node->Children.size();
			if (num_points == 0)
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Shape - Polygon shape has no points", DebugInfo.c_str());
				return 0;
			}
			shape->Vertices = new CzVec2[num_points]();
			shape->NumVertices = num_points;

			// Process polygon points
			int index = 0;
			for (CzXmlNode::_Iterator it2 = node->begin(); it2 != node->end(); ++it2)
			{
				unsigned int name_hash = (*it2)->GetName().getHash();
				if (name_hash == CzHashes::Point_Hash)
				{
					// Get points data
					for (CzXmlNode::_AttribIterator it = (*it2)->attribs_begin(); it != (*it2)->attribs_end(); ++it)
					{
						unsigned int attrib_hash = (*it)->getName().getHash();

						if (attrib_hash == CzHashes::Value_Hash)
						{
							if (!(*it)->getValueAsPoint(*(shape->Vertices + index)))
							{
								CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Shape - Invalid value for point SHape::Polygon", DebugInfo.c_str());
								return true;
							}
							break;
						}
					}
					index++;
				}
			}
		}
	}

	// If we are declared inside a scene then shape is local to the scene
	CzScene* scene = NULL;
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Actor_Hash)
		scene = ((CzActor*)parent)->getScene();
	else
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

	if (scene != NULL)
		return scene->getResourceManager()->addResource(this) ? 1 : 0;
	else
		return CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(this) ? 1 : 0;

	return 1;
}

