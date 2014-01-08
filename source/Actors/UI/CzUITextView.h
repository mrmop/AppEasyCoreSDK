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

#if !defined(_CCZ_UI_TEXT_VIEW_H_)
#define _CCZ_UI_TEXT_VIEW_H_

#include "CzUIBase.h"
#include "CzUILabel.h"
#include "CzScene.h"
#include "CzBrush.h"
#include "CzInput.h"

//
//
//
//
// CzUITextView - UI class that allows the user to view and edit multi-line scrolling text
//
//
//
//
class CzUITextView : public CzUILabel
{
public:
	// Properties
protected:
	CzIVec2				Area;							///< The area that the etxt should fit into
	float				Zoom;							///< Current zoom
	float				MinZoom;						///< The minimum zoom point
	float				MaxZoom;						///< The maximum zoom point
public:
	void				setArea(CzIVec2& size);
	CzIVec2				getArea() const					{ return Area; }
	void				setZoom(float scale);
	float				getZoom() const					{ return Zoom; }
	void				setMinZoom(float scale)			{ MinZoom = scale; }
	float				getMinZoom() const				{ return MinZoom; }
	void				setMaxZoom(float scale)			{ MaxZoom = scale; }
	float				getMaxZoom() const				{ return MaxZoom; }
	bool				setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool				setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool				getProperty(unsigned int property_name, CzXomlProperty& prop);

	// Properties end
protected:
	float				PrevZoom;
	CzVec2				Midpoint;
	bool				ZoomValid;
	float				PreviousDist;
	float				CompoundScale;
	CzVec2				CompoundPosition;
	float				SavedCompoundScale;
	CzVec2				SavedCompoundPosition;
	CzVec2				PanVelocity;
	int					TouchIndices[2];
	int					NumTouches;
	bool				UpdateBinding(unsigned int property_name, CzXomlVariable* var);
public:
	CzUITextView();

	virtual bool		InitView(bool native_res, float min_scale, float max_scale);

	bool				Update(float dt);
	bool				UpdateVisual();

	// Event handlers
	void				NotifyBeginTouch(int index, int x, int y, bool allow_bubble);
	void				NotifyEndTouch(int index, int x, int y, bool allow_bubble);

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Implementation of IzAnimTarget interface
	bool				UpdateFromAnimation(CzAnimInstance *animation);

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	TextViewClassDef;								// XOML class definition
public:
	static void				InitClass();
	static void				ReleaseClass();

	static bool				_setMinZoom(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getMinZoom(IzXomlResource* target);
	static bool				_setMaxZoom(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getMaxZoom(IzXomlResource* target);
	static bool				_setZoom(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getZoom(IzXomlResource* target);
	static bool				_setArea(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getArea(IzXomlResource* target);
};

//
//  CzUITextViewCreator - Creates an instance of a TextView actor object
//
class CzUITextViewCreator : public IzXomlClassCreator
{
public:
	CzUITextViewCreator()
	{
		setClassName("textview");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzUITextView(); }
};


#endif	// _CCZ_UI_TEXT_VIEW_H_

