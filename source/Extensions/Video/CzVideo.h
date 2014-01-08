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

#if !defined(_CZ_VIDEO_H_)
#define _CZ_VIDEO_H_

#include "CzUtil.h"
#include "CzXoml.h"
#include "CzRect.h"
#include "IzPlatformVideo.h"
#include "CzEvents.h"

//
//
//
//
// CzVideo - A video file
//
//
//
//
class CzVideo : public IzXomlResource
{
public:

	enum eVideo_State
	{
		State_Invalid, 
		State_Loaded, 
	};

	// Proprties
private:
	eVideo_State			State;				///< State of video
	CzString				Filename;			///< Name of video file
	int						RepeatCount;		///< Total times to repeat play
	CzIRect					Rect;				///< Position and size of video on screen
	eCzVideoCodec			Codec;				///< Codec to use
	float					Volume;				///< Volume of audio
	CzEventManager*			EventsManager;		///< List of events that the video handles
public:
	eVideo_State			getState() const					{ return State; }
	CzString&				getFilename()						{ return Filename; }
	void					setRepeatCount(int count)			{ RepeatCount = count; }
	int						getRepeatCount() const				{ return RepeatCount; }
	CzIRect					getRect() const						{ return Rect; }
	void					setRect(int x, int y, int w, int h)	{ Rect.x = x; Rect.y = y; Rect.w = w; Rect.h = h; }
	void					setRect(CzIRect &rect)				{ Rect = rect; }
	eCzVideoCodec			getCodec() const					{ return Codec; }
	void					setVolume(float volume)				{ Volume = volume; }
	float					getVolume() const					{ return Volume; }
	// Properties end

protected:
	char*					VideoData;			///< Raw video data
	uint32					VideoDataLen;		///< Length of raw video data
	CzFile*					File;				///< File object (if video if file based)
public:

	CzVideo() : IzXomlResource(), File(NULL), State(State_Invalid), VideoData(NULL), VideoDataLen(0), EventsManager(NULL), Rect(0, 0, 0, 0), Codec(Video_Codec_Mpeg4), Volume(1.0f)
	{
		setClassType("video");
		EventsManager = new CzEventManager();
	}
	virtual ~CzVideo();

	bool				Init(const char* filename, bool preload, bool blocking, eCzVideoCodec codec);
	bool				Init(char* video_data, uint32 video_data_len, eCzVideoCodec codec);
	bool				Play();
	bool				Load(bool blocking = true);						// Force load the video file

	// Implementation of IzXomlResource interface
	int					LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

	// Event handlers
	virtual void		ProcessEventActions(unsigned int event_name);
	virtual void		NotifyStopped();

	// Internal
	void				FinishLoad();									// Called back when aysnc loading is completed
};
//
//  CzVideoCreator - Creates an instance of a video object
//
class CzVideoCreator : public IzXomlClassCreator
{
public:
	CzVideoCreator()
	{
		setClassName("video");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzVideo(); }
};




#endif	// _CZ_VIDEO_H_