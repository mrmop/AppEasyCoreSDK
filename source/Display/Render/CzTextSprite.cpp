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

#include "CzTextSprite.h"
#include "CzSpriteManager.h"
#include "CzString.h"
#include "CzUtil.h"

//
//
//
//
// CzTextSprite implementation
//
//
//
//
CzTextSprite::~CzTextSprite()
{
	if (PreparedText != NULL)
		PLATFORM_FONT->DestroyPreparedText(PreparedText);
}

void CzTextSprite::Init(int vertex_count)
{
	CzSprite::Init(vertex_count);
	PreparedText = PLATFORM_FONT->CreatePreparedText();
}

void CzTextSprite::RebuildTransform()
{
	// Build the transform
	AccumDepth = Depth;

	// Set the rotation transform
	Transform.Rotate(Angle);
	// Scale the transform
//	Transform.ScaleRotation(ScaleX, ScaleY);
	if (ScaleX != 1.0f || ScaleY != 1.0f)
	{
		CzMatrix3 scale;
		scale.Scale(ScaleX, ScaleY);
		Transform.Multiply(&scale);
	}
	// Translate the transform
	Transform.TranslateSet(&Position);

	// Apply origin
	CzMatrix3 trans;
	CzVec2 org = Origin;
	org.x -= (float)Rect.w / 2;
	org.y -= (float)Rect.h / 2;
	trans.TranslateSet(&org);
	Transform.Multiply(&trans);

	// Apply linked sprites transform if linked
	if (LinkedTo != NULL)
	{
		Transform.MultiplyPost(&LinkedTo->getTransform());
		AccumDepth = Depth + LinkedTo->getAccumDepth();
	}
/*	else
	{
		// Apply Manager transform if sprite is managed by a Manager sprite manager (doesnt work with none uniform scaling)
		if (Manager != NULL)
			Transform.MultiplyPost(&Manager->getTransform());
	}*/

	if (Anchor == TopLeft)
	{
		trans.TranslateSet((float)Rect.w / 2, (float)Rect.h / 2);
		Transform.MultiplyPost(&trans);
	}

}

void CzTextSprite::TransformVertices()
{
	// Apply Manager transform if sprite is managed by a Manager sprite manager (doesnt work with none uniform scaling)
	FinalTransform = Transform;
	if (Manager != NULL)
	{
		if (AccumDepth == 1.0f || AccumDepth == 0)
		{
			if (IgnoreCamera)
				FinalTransform.MultiplyPost(&Manager->getTransformNoCamera());
			else
				FinalTransform.MultiplyPost(&Manager->getTransform());
		}
		else
		{
			CzVec2 cop = Manager->getCOP();
			CzVec2 centre = Manager->getScreenCentre();

			CzMatrix3 m;
			if (IgnoreCamera)
				m = Manager->getTransformNoCamera();
			else
				m = Manager->getTransform();
			m.m[0][0] /= AccumDepth;
			m.m[0][1] /= AccumDepth;
			m.m[1][0] /= AccumDepth;
			m.m[1][1] /= AccumDepth;
			m.TranslateSet((m.getX() - centre.x) / AccumDepth + cop.x, (m.getY() - centre.y) / AccumDepth + cop.y);

			FinalTransform.MultiplyPost(&m);
		}
	}

	TransformClipRect();

	// Generate transformed vertices
	float x0 = 0;
	float y0 = 0;
	float x1 = (float)Rect.w;
	float y1 = (float)Rect.h;
	CzVec2* v = Prim->Verts;
	v->x = x0;
	v->y = y0; v++;
	v->x = x1;
	v->y = y0; v++;
	v->x = x1;
	v->y = y1; v++;
	v->x = x0;
	v->y = y1;
	FinalTransform.TransformN(Prim->Verts, Prim->Verts, 4);
/*	if (Anchor == TopLeft)
	{
		float dx = x1 / 2.0f;
		float dy = x1 / 2.0f;
		for (int t = 0; t < 4; t++)
		{
			TransformedV[t].x =- dx;
			TransformedV[t].y =- dy;
		}
	}*/
}

void CzTextSprite::RebuildText()
{
	if (Font == NULL || Text.c_str() == NULL || Font->getFontHandle() == NULL)
		return;

	IzPlatformFont* fp = PLATFORM_FONT;
	fp->SetCurrentFont(Font->getFontHandle());
	fp->SetCurrentFontFlags(Flags);
	fp->SetCurrentFontRect(Rect);
	fp->SetCurrentFontAlignment(AlignH, AlignV);
	fp->PrepareText(PreparedText, Text.c_str());

	TextDirty = false;
}

void CzTextSprite::Draw()
{
	if (!Visible)
		return;

	TransformDirty = false;

	if (!BeforeChildren)
		DrawChildren();

	if (AccumDepth > 0 && !isClippedByManager())
	{
		// Do not render if not visible
		if (Font != NULL && Text.c_str() != NULL && Font->getFontHandle() != NULL)
		{
			if (TextDirty)
				RebuildText();
			else
			{
				IzPlatformFont* fp = PLATFORM_FONT;
				fp->SetCurrentFont(Font->getFontHandle());
				fp->SetCurrentFontFlags(Flags);
				fp->SetCurrentFontRect(Rect);
				fp->SetCurrentFontAlignment(AlignH, AlignV);
			}

			// Cannot batch as we cannot get access to glyph UV coordinates
			PLATFORM_RENDER->DrawText(PreparedText, Font, &FinalTransform, Colour, Skew, Material->Filter, Material->AlphaMode);
		}
	}

	if (BeforeChildren)
		DrawChildren();
}

