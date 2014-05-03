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

// TODO:
// * Add support for gradient brush

#include "CzBrush.h"
#include "CzString.h"
#include "CzScene.h"
#include "CzResources.h"
#include "CzActor.h"
#include "CzHashes.h"
#include "CzXomlResourceManager.h"

int IzBrush::LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node)
{
	// Get brush data
	eBrushType		type = BT_None;
	CzVec4			colour;
	CzString*		name = NULL;
	CzString*		image_name = NULL;
	CzString*		uvs = NULL;
	CzIRect			rect = CzIRect(0, 0, 0, 0);
	CzIRect			scale_area = CzIRect(0, 0, 0, 0);
	CzString*		condition = NULL;

	CzScene* scene = NULL;
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Actor_Hash)
		scene = ((CzActor*)parent)->getScene();
	else
	if (parent != NULL && parent->getClassTypeHash() == CzHashes::Scene_Hash)
		scene = (CzScene*)parent;

	for (CzXmlNode::_AttribIterator it = node->attribs_begin(); it != node->attribs_end(); ++it)
	{
		unsigned int attrib_hash = (*it)->getName().getHash();

		if (attrib_hash == CzHashes::Name_Hash)
			name = &(*it)->getValue();
		else
		if (attrib_hash == CzHashes::Type_Hash)
		{
			unsigned int type_hash = (*it)->getValue().getHash();
			if (type_hash == CzHashes::Solid_Hash)
				type = BT_Solid;
			else
			if (type_hash == CzHashes::Gradient_Hash)
				type = BT_Gradient;
			else
			if (type_hash == CzHashes::Image_Hash)
				type = BT_Image;
			else
			if (type_hash == CzHashes::Patch9_Hash)
				type = BT_9Patch;
			else
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Brush - Invalid brush type - ", (*it)->getValue().c_str(), DebugInfo.c_str());
				return false;
			}
		}
		else
		if (attrib_hash == CzHashes::Colour_Hash)
		{
			if (!(*it)->getValueAsPoint4(colour))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Brush - Invalid value for Colour, expected vec4", DebugInfo.c_str());
		}
		else
		if (attrib_hash == CzHashes::Image_Hash)
			image_name = &(*it)->getValue();
		else
		if (attrib_hash == CzHashes::SrcRect_Hash)
		{
			if (!(*it)->getValueAsRect(rect))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Brush - Invalid value for SrcRect, expected rect", DebugInfo.c_str());
		}
		else
		if (attrib_hash == CzHashes::UV_Hash)
			uvs = &(*it)->getValue();
		else
		if (attrib_hash == CzHashes::Tag_Hash)
			setTag((*it)->getValue().c_str());
		else
		if (attrib_hash == CzHashes::ScaleArea_Hash)
		{
			if (!(*it)->getValueAsRect(scale_area))
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Brush - Invalid value for ScaleArea, expected rect", DebugInfo.c_str());
		}
		else
		if (attrib_hash == CzHashes::Condition_Hash)
			condition = &(*it)->getValue();
	}

	if (condition != NULL)
	{
		// Find the condition variable
		bool condition_not = false;
		CzXomlVariable* var = NULL;
		if (*(condition->c_str()) == '!')
		{
			condition_not = true;
			CzString cond = condition->c_str() + 1;
			var = CzXomlVariable::GetVariable(cond, scene);
		}
		else
			var = CzXomlVariable::GetVariable(*condition, scene);
		if (var != NULL)
		{
			bool res = var->isTrue();
			if (condition_not)
				res = !res;
			if (!res)
				return -1;
		}
		else
		{
#if defined (_DEBUG)
			CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Brush - condition variable not found - ", condition->c_str(), DebugInfo.c_str());
#endif // _DEBUG
			return -1;
		}
	}

	if (name == NULL)
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Brush - Is not named", DebugInfo.c_str());
	if (type == BT_None)
	{
		type = BT_Image;
		CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Brush - Type was not defined, default set to image brush - ", name->c_str(), DebugInfo.c_str());
	}

	IzBrush* brush = NULL;
	switch (type)
	{
	case BT_Solid:
		{
			CzBrushSolid* b = new CzBrushSolid();
			b->setColour((uint8)colour.x, (uint8)colour.y, (uint8)colour.z, (uint8)colour.w);
			brush = b;
		}
		break;
	case BT_Gradient:
		{
		}
		break;
	case BT_Image:
	case BT_9Patch:
		{
			CzImage* image = NULL;

			if (image_name == NULL)
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_ERROR, "Brush - An image / patch9 brush requires an image to be specified", DebugInfo.c_str());
				return 0;
			}
	
			if (scene != NULL)
			{
				image = (CzImage*)scene->getResourceManager()->findResource(image_name->getHash(), CzHashes::Image_Hash);
				if (image == NULL)
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Brush - Could not find brush image - ", image_name->c_str(), DebugInfo.c_str());
			}
			else
			{
				image = (CzImage*)CZ_GLOBAL_RESOURCES->getResourceManager()->findResource(image_name->getHash(), CzHashes::Image_Hash);
				if (image == NULL)
					CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Brush - Could not find brush image - ", image_name->c_str(), DebugInfo.c_str());
			}

			if (image != NULL)
			{
				// if no rect supplied then use the full bitmap
				if (rect.w == 0 && rect.h == 0)
				{
					rect.w = image->getWidth();
					rect.h = image->getHeight();
				}

				if (type == BT_Image)
				{
					CzBrushImage* b = new CzBrushImage();
					b->setImage(image);
					b->setSrcRect(rect);
					brush = b;
				}
				else
				if (type == BT_9Patch)
				{
					CzBrushImage9* b = new CzBrushImage9();
					b->setImage(image);
					b->setSrcRect(rect);
					b->setScalableArea(scale_area);
					brush = b;
				}
			}
		}
		break;
	}

	if (brush != NULL)
	{
		if (name != NULL)
			brush->setName(name->c_str());
		brush->setBrushType(type);

		// If we are declared inside a scene then brush is local to the scene
		if (scene != NULL)
			scene->getResourceManager()->addResource(brush);
		else
			CZ_GLOBAL_RESOURCES->getResourceManager()->addResource(brush);

		if (uvs != NULL)
		{
			if (type == BT_Image)
			{
				int count = uvs->getAsListOfFloat(CzXmlTools::FloatListPool) >> 1;
				CzVec2* uvs = new CzVec2[count]();

				int index = 0;
				for (int t = 0; t < count; t++)
				{
					uvs[t].x = CzXmlTools::FloatListPool[index++];
					uvs[t].y = CzXmlTools::FloatListPool[index++];
				}
				((CzBrushImage*)brush)->setUVList(uvs);
			}
			else
			{
				CzDebug::Log(CZ_DEBUG_CHANNEL_WARNING, "Brush - Only image brush supports UV's - ", image_name->c_str(), DebugInfo.c_str());
			}
		}
	}

	return -1;
}






