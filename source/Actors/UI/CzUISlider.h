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

#if !defined(_CCZ_UI_SLIDER_H_)
#define _CCZ_UI_SLIDER_H_

#include "CzUIBase.h"
#include "CzScene.h"
#include "CzBrush.h"
#include "CzInput.h"

//
//
//
//
// CzUISlider - UI class that allows the user to select a value using a sliding scale (horizontal, vertical and round)
//
//
//
//


class CzUISlider : public CzUIBase
{
public:
	enum eSliderType
	{
		SliderType_Horizontal, 
		SliderType_Vertical, 
	};

	// Properties
protected:
	eSliderType				SliderType;				// Type of slider
	float					Value;					// Position of thumb within the range
	CzVec2					ValueRange;				// The tange of the thumb
	int						OriginalSliderSize;		// Original size of slider
	int						SliderSize;				// Size of slider
public:
	void					setValue(float pos, bool affect_thumb);
	float					getValue() const				{ return Value; }
	void					setValueRange(CzVec2& range);
	CzVec2					getValueRange() const			{ return ValueRange; }
	void					setSliderType(eSliderType type);
	eSliderType				getSliderType() const			{ return SliderType; }
	void					setSliderSize(int size);
	int						getSliderSize() const			{ return SliderSize; }

	bool					setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool					setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool					getProperty(unsigned int property_name, CzXomlProperty& prop);
	// Properties end
protected:
	bool					ThumbChanged;			// True when slidee value changed
	bool					UpdateBinding(unsigned int property_name, CzXomlVariable* var);
	void					UpdateSliderSize();
		
public:

	CzUISlider() : CzUIBase(), Value(0), ValueRange(0, 1.0f), SliderType(SliderType_Horizontal), SliderSize(0), ThumbChanged(false) { IsDraggable = true; HoldFocus = true; }

	bool				Update(float dt);

	// Event handlers
	void				NotifyOrientationChange(CzScene::eOrientation old_orientation, CzScene::eOrientation new_orientation);
	void				NotifyBeginTouch(int index, int x, int y, bool allow_bubble = true);
	void				NotifyEndTouch(int index, int x, int y, bool allow_bubble = true);
	virtual void		NotifyValueChanged(float old_value, float new_value);

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Implementation of IzAnimTarget interface
	bool				UpdateFromAnimation(CzAnimInstance *animation);

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	UISliderClassDef;								// XOML class definition
public:
	static void				InitClass();
	static void				ReleaseClass();

	static bool				_setSliderType(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSliderType(IzXomlResource* target);
	static bool				_setValue(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getValue(IzXomlResource* target);
	static bool				_setValueRange(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getValueRange(IzXomlResource* target);
	static bool				_setSliderSize(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSliderSize(IzXomlResource* target);
	static bool				_setOnValueChanged(IzXomlResource* target, const CzXomlProperty& prop, bool add);

};

//
//  CzUISliderCreator - Creates an instance of a Slider UI actor object
//
class CzUISliderCreator : public IzXomlClassCreator
{
public:
	CzUISliderCreator()
	{
		setClassName("slider");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzUISlider(); }
};

#endif	// _CCZ_UI_SLIDER_H_

