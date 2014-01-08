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

#if !defined(_CCZ_UI_H_)
#define _CCZ_UI_H_

#include "CzActorImage.h"
#include "CzScene.h"
#include "CzBrush.h"
#include "CzInput.h"
#include "CzAnim.h"

/**
 @class	CzUIBase

 @brief	Base class of all UI components.

 CzUIBase is the basic class that all UI actors are derived from. CzUIBase appends additional functionality to the normal image actor such as:
 - Selected, normal and disabled states
 - Individual brushes amd colours for each of the 3 states
 - Toggle state
 - Hide and show timelines
 - Automatic re-sizing to contents
 - Content scrolling
 - Content clipping
 - Colour offsetting

 All new user interface actors should be derived from this class or a derivative of this class.

 The following UI classes are derived from this base class:
 - CzUIIcon - An icon / image based button
 - CzUILabel - A text label that can display text and an image based background
 - CzUIGrid - A grid container that organises content in a columns and rows. Supports data binding
 - CzUIImageView - An image container with support for pinch zoom / pan
 - CzUIListBox - A list box container that stacks selectable content either vertically or horizontally that can be scrolled. Supports data binding
 - CzUICanvas - A container that organises docked and undocked content onto a 2D canvas that can be scrolled
 - CzUIStackPanel - A container that stacks content either vertically or horizontally
 - CzUIWrapPanel - A container that stacks content either vertically or horizontally, wrapping over hanging content to thenext row or column
 - CzUISlider - Enables the user to select a value by sliding a thumb either up / down or left / right
 - CzUITabBar - Enables navigation of a number of different views (container UI components such as the Canvas).
 - CzUITextBox - A text entry box with a background
 - CzUITextView - A text based container with support for pinch zoom / pan
 - CzUIVideoOverlay - Displays video overlays
 - CzUIWebView - Displays web content

 */

class CzUIBase : public CzActorImage
{
public:

	/**
	 @enum	eSelectType
	
	 @brief	Values that represent different types of selection modes.
	 */
	enum eSelectType
	{
		SelectType_Normal, ///< Normal selection mode, the actor will remasin selected whilst it has touch focus.
		SelectType_Toggle, ///< Toggle selection mode, the actors sekected state will toggle on and off each time it is selected.
		SelectType_Sticky, ///< Sticky selection mode, the actors sekected state will remain selected once selected.
	};

	/**
	 @enum	eAxis
	
	 @brief	Values that represent horizonal, vertical and both axis. 
	 */
	enum eAxis
	{
		Axis_None,			///< Affects no axis.
		Axis_X,				///< Affects x axis.
		Axis_Y,				///< Affects y axis.
		Axis_XY,			///< Affects both x and y axis.
	};
	// Properties
protected:
	bool				PrevSelected;						///< Previous Selected status
	bool				ToggledOn;							///< ToggledOn state
	bool				Selected;							///< Selected status
	IzBrush*			BackgroundBrush;					///< Default none selected brush
	CzColour			BackgroundColour;					///< Default none selected colour
	IzBrush*			SelectedBackgroundBrush;			///< Selected brush
	CzColour			SelectedBackgroundColour;			///< Selected colour
	IzBrush*			DisabledBackgroundBrush;			///< Disabled brush
	CzColour			DisabledBackgroundColour;			///< Disabled colour
	bool				Enabled;							///< Enabled state
	bool				LayoutDirty;						///< When true the layout of this element and any children is re-calculated
	eSelectType			SelectType;							///< Selection method used for this UI element
	CzIRect				OriginalClippingMargin;				///< Original margin that is used to adjust the clipping rectangle
	CzIRect				ClippingMargin;						///< Margin that is used to adjust the clipping rectangle
	CzVec2				PreviousScrollPos;					///< Previous scroll position
	CzVec2				ScrollPos;							///< Current scroll position
	CzVec2				ScrollVelocity;						///< Current scroll velocity
	CzIRect				ScrollRange;						///< X and Y ranges of scroll
	bool				EnableSpring;						///< Enables drag release spring
	eAxis				SizeToContentAxis;					///< Determines if and how an element should size to its content
	CzAnimTimeline*		ShowTimeline;						///< Timeline to play when this UI element is made visible
	CzAnimTimeline*		HideTimeline;						///< Timeline to play when this UI element is made invisible
	bool				NeedsResize;						///< Set to true when child actors sizes have changed
	CzIRect				ColourOffset;						///< Amount to offset the current colour of the actor
public:
	void				setVisible(bool visible, bool force_set = false);
	void				setBackgroundBrush(IzBrush* brush);
	IzBrush*			getBackgroundBrush() const						{ return BackgroundBrush; }
	void				setBackgroundColour(const CzColour& colour);
	CzColour			getBackgroundColour() const						{ return BackgroundColour; }
	void				setSelectedBackgroundBrush(IzBrush* brush);
	IzBrush*			getSelectedBackgroundBrush() const				{ return SelectedBackgroundBrush; }
	void				setSelectedBackgroundColour(const CzColour& colour);
	CzColour			getSelectedBackgroundColour() const				{ return SelectedBackgroundColour; }
	void				setDisabledBackgroundBrush(IzBrush* brush);
	IzBrush*			getDisabledBackgroundBrush() const 				{ return DisabledBackgroundBrush; }
	void				setDisabledBackgroundColour(const CzColour& colour);
	CzColour			getDisabledBackgroundColour() const				{ return DisabledBackgroundColour; }
	void				setSelectType(eSelectType type)					{ SelectType = type; }
	eSelectType			getSelectType() const							{ return SelectType; }
	void				setSelected(bool selected, bool force_select = false);
	bool				isSelected() const								{ return Selected; }
	virtual void 		setEnabled(bool enabled);
	bool				isEnabled() const								{ return Enabled; }
	bool				setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool				setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool				getProperty(unsigned int property_name, CzXomlProperty& prop);
	void				setLayoutDirty(bool dirty);
	bool				isLayoutDirty() const							{ return LayoutDirty; }
	void				setClippingMargin(int x, int y, int w, int h)	{ ClippingMargin.x = x; ClippingMargin.y = y; ClippingMargin.w = w; ClippingMargin.h = h; }
	CzIRect				getClippingMargin()								{ return ClippingMargin; }
	void				setScrollPos(float x, float y);
	CzVec2				getScrollPos() const							{ return ScrollPos; }
	CzVec2				getPreviousScrollPos() const					{ return PreviousScrollPos; }
	void				setScrollVelocity(float x, float y)				{ ScrollVelocity.x = x; ScrollVelocity.y = y; }
	CzVec2				getScrollVelocity() const						{ return ScrollVelocity; }
	void				setScrollRange(int x, int y, int w, int h)		{ ScrollRange.x = x; ScrollRange.y = y; ScrollRange.w = w; ScrollRange.h = h; } 
	CzIRect				getScrollRange() const							{ return ScrollRange; }
	void				setToggledOn(bool toggled_on);
	bool				isToggledOn() const								{ return ToggledOn; }
	void				setShowTimeline(CzAnimTimeline* timeline);
	CzAnimTimeline*		getShowTimeline() const 						{ return ShowTimeline; }
	void				setHideTimeline(CzAnimTimeline* timeline);
	CzAnimTimeline*		getHideTimeline() const 						{ return HideTimeline; }
	void				setNeedsResize(bool resize);
	bool				getNeedsResize() const							{ return NeedsResize; }
	void				setSizeToContentAxis(eAxis axis)				{ SizeToContentAxis = axis; }
	eAxis				getSizeToContentAxis() const					{ return SizeToContentAxis; }
	void				NotifyParentsResize();
	void				setColourOffset(const CzIRect& offset)			{ ColourOffset = offset; setEnabled(Enabled); }
	void				resetColourOffset()								{ ColourOffset.x = 0; ColourOffset.y = 0; ColourOffset.w = 0; ColourOffset.h = 0; setEnabled(Enabled); }
	CzIRect				getColourOffset() const							{ return ColourOffset; }
	void				setShowTimeline(const char* timeline_name);
	void				setHideTimeline(const char* timeline_name);
	void				setEnableSpring(bool enable)					{ EnableSpring = enable; }
	bool				getEnableSpring() const							{ return EnableSpring; }
	// Properties end
protected:
	bool				Tapped;
	CzVec2				InitialScrollPos;
	bool				UpdateBinding(unsigned int property_name, CzXomlVariable* var);
	virtual bool		UpdateLayout();									// Updates the layout of the child items
	bool				ScrollRangeCheck();

public:
	CzUIBase();
	virtual ~CzUIBase();
	bool				Init(IzBrush* brush, int width, int height);

	bool				Update(float dt);
	virtual void		ResizeToContent();

	// Event handlers
	void				NotifyTapped(int index, int x, int y, bool allow_bubble);
	void				NotifyBeginTouch(int index, int x, int y, bool allow_bubble);
	void				NotifyEndTouch(int index, int x, int y, bool allow_bubble);
	virtual void		NotifyToggledOn();
	virtual void		NotifyToggledOff();
	virtual void		NotifyBackKey();
	virtual void		NotifyMenuKey();
	void				NotifyOrientationChange(CzScene::eOrientation old_orientation, CzScene::eOrientation new_orientation);

	// Implementation of IzXomlClass interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Implementation of IzAnimTarget interface
	bool				UpdateFromAnimation(CzAnimInstance *animation);

	// Preset UI animations
public:
	static CzList<CzAnim*>	PresetAnimations;
	static void				InitPresetAnimations();
	static void				ReleasePresetAnimations();
	static void				UpdatePresetAnimations();
	static CzAnim*			FindPresetAnimation(unsigned int name_hash);

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	UIBaseClassDef;								// XOML class definition

public:
	static void				InitClass();
	static void				ReleaseClass();

	static bool				_setSize(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSize(IzXomlResource* target);
	static bool				_setBackground(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getBackground(IzXomlResource *target);
	static bool				_setSelectedBackground(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSelectedBackground(IzXomlResource *target);
	static bool				_setDisabledBackground(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getDisabledBackground(IzXomlResource *target);
	static bool				_setBackgroundColour(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getBackgroundColour(IzXomlResource *target);
	static bool				_setSelectedColour(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSelectedColour(IzXomlResource *target);
	static bool				_setDisabledColour(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getDisabledColour(IzXomlResource *target);
	static bool				_setEnabled(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getEnabled(IzXomlResource *target);
	static bool				_setSelected(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSelected(IzXomlResource *target);
	static bool				_setScrollPos(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getScrollPos(IzXomlResource *target);
	static bool				_setSelectType(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSelectType(IzXomlResource *target);
	static bool				_setSpring(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSpring(IzXomlResource *target);
	static bool				_setOnToggledOn(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnToggledOff(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnBackKey(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnMenuKey(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setClipMargin(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getClipMargin(IzXomlResource *target);
	static bool				_setScrollRange(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getScrollRange(IzXomlResource *target);
	static bool				_setShowTimeline(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getShowTimeline(IzXomlResource *target);
	static bool				_setHideTimeline(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getHideTimeline(IzXomlResource *target);
	static bool				_setSizeToContentAxis(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getSizeToContentAxis(IzXomlResource *target);
	static bool				_setKeyFocus(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getKeyFocus(IzXomlResource *target);
	static bool				_setToggledOn(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getToggledOn(IzXomlResource *target);
	static bool				_setColourOffset(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getColourOffset(IzXomlResource *target);

};






#endif	// _CCZ_UI_H_
