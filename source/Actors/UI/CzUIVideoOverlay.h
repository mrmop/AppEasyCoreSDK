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

#if !defined(_CCZ_UI_VIDEO_OVERLAY_H_)
#define _CCZ_UI_VIDEO_OVERLAY_H_

#include "CzUIBase.h"
#include "CzVideo.h"

//
//
//
//
// CzUIVideoOverlay- A video overlay based UI actor
//
//
//
//
class CzUIVideoOverlay : public CzUIBase
{
	// Properties
protected:
	bool			AutoPlay;
	CzVideo*		Video;
	bool			Playing;
	bool			Started;
public:
	void			setAutoPlay(bool auto_play)		{ AutoPlay = auto_play; }
	bool			isAutoPlay() const				{ return AutoPlay; }
	void			setVideo(CzVideo* video)		{ Video = video; }
	CzVideo*		getVideo()						{ return Video; }
	bool			isPlaying() const				{ return Playing; }
	bool			isStarted() const				{ return Started; }
	bool			setProperty(unsigned int property_name, const CzXomlProperty& data, bool delta);
	bool			setProperty(unsigned int property_name, const CzString& data, bool delta);
	bool			getProperty(unsigned int property_name, CzXomlProperty& prop);
	// Properties end
protected:
	bool			UpdateBinding(unsigned int property_name, CzXomlVariable* var);
public:
	CzUIVideoOverlay() : CzUIBase(), AutoPlay(false), Video(NULL), Playing(false), Started(false)	{ setActualClassType("videooverlay"); }

	bool			Update(float dt);

	// Implementation of IzXomlClass interface
	int				LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Implementation of IzAnimTarget interface
	bool			UpdateFromAnimation(CzAnimInstance *animation);

	bool			Play();
	void			Stop();
	void			Pause();
	void			Resume();

	// Internal (used by XOML system to setup and cleanup the XOML class properties system
protected:
	static CzXomlClassDef*	VideoOverlayClassDef;								// XOML class definition
public:
	static void				InitClass();
	static void				ReleaseClass();

	static bool				_setVideo(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getVideo(IzXomlResource* target);
	static bool				_setAutoPlay(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getAutoPlay(IzXomlResource* target);
	static bool				_setRepeat(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getRepeat(IzXomlResource* target);
	static bool				_setVolume(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getVolume(IzXomlResource* target);
	static bool				_setCommand(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static CzXomlProperty	_getIsPlaying(IzXomlResource* target);
	static CzXomlProperty	_getStarted(IzXomlResource* target);
	static bool				_setOnStart(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnEnd(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnPause(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnResume(IzXomlResource* target, const CzXomlProperty& prop, bool add);
	static bool				_setOnError(IzXomlResource* target, const CzXomlProperty& prop, bool add);

};

//
//  CzUIVideoOverlayCreator - Creates an instance of an video overlay actor object
//
class CzUIVideoOverlayCreator : public IzXomlClassCreator
{
public:
	CzUIVideoOverlayCreator()
	{
		setClassName("videooverlay");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzUIVideoOverlay(); }
};







#endif	// _CCZ_UI_VIDEO_OVERLAY_H_
