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

#if !defined(_CCZ_PLATFORM_H_)
#define _CCZ_PLATFORM_H_

#include "CzTypes.h"
#include "CzUtil.h"

class IzPlatformAds;
class IzPlatformAudio;
class IzPlatformCam;
class IzPlatformComms;
class IzPlatformDisplay;
class IzPlatformFacebook;
class IzPlatformFont;
class IzPlatformFile;
class IzPlatformMarket;
class IzPlatformImaging;
class IzPlatformInput;
class IzPlatformVideo;
class IzPlatformOther;
class IzPlatformRender;
class IzPlatformSys;
class IzPlatformUI;

//
// IzPlatform - Abstract interface that provides access to platform specific hardware
//
// To add a new platforn, derive your own class from IzPlatform and all other platfrom sub systems and implement all methods
// 
//
// NOTES: The platform system will eventually abstract all Marmalade specific platform code into its own class / classes
//
// IzPlatformAds -		Abstract interface to ads system
// IzPlatformAudio -	Abstract interface to audio system
// IzPlatformCam -		Abstract interface to Camera API
// IzPlatformComms -	Abstract interface to communications system
// IzPlatformDisplay -	Abstract interface to display system
// IzPlatformFacebook -	Abstract interface to Facebook API
// IzPlatformFile -		Abstract interface to file system
// IzPlatformFont -		Abstract interface to font system
// IzPlatformMarket -	Abstract interface to In app purchasing API
// IzPlatformImaging -	Abstract interface to imaging system
// IzPlatformInput -	Abstract interface to input system
// IzPlatformVideo -	Abstract interface to Video systems
// IzPlatformOther -	Abstract interface to other systems not directly related to the above or utility functionality
// IzPlatformRender -	Abstract interface to 2d rendering system
// IzPlatformSys -		Abstract interface to main hardware systen
// IzPlatformUI -		Abstract interface to UI systems
// 

class IzPlatform
{
public:
	virtual int		Init() = 0;
	virtual void	Release() = 0;

	// Query
	virtual const char*	getName() const	= 0;

};

#define	PLATFORM			CzPlatform::getInstance()

class CzPlatform
{
private:
	static CzPlatform* _instance;
	CzPlatform() :	Initialised(false), Sys(NULL), Display(NULL), Input(NULL), Audio(NULL), Comms(NULL), File(NULL), Font(NULL), Imaging(NULL), Video(NULL), Other(NULL), 
					Render(NULL), UI(NULL), Platform(NULL), Cam(NULL), Market(NULL), Facebook(NULL), Ads(NULL)  {}
	virtual ~CzPlatform() {}
	CzPlatform(const CzPlatform &);
	CzPlatform& operator=(const CzPlatform &);
public:
	static void Create();
	static void Destroy();
	static CzPlatform* getInstance();

	// Properties
protected:
	bool					Initialised;
	IzPlatform*				Platform;
public:
	bool					isInitialised() const	{ return Initialised; }
	IzPlatform*				getPlatform()		{ return Platform; }
	IzPlatformSys*			getSys()			{ return Sys; }
	IzPlatformDisplay*		getDisplay()		{ return Display; }
	IzPlatformInput*		getInput()			{ return Input; }
	IzPlatformAudio*		getAudio()			{ return Audio; }
	IzPlatformComms*		getComms()			{ return Comms; }
	IzPlatformFont*			getFont()			{ return Font; }
	IzPlatformFile*			getFile()			{ return File; }
	IzPlatformImaging*		getImaging()		{ return Imaging; }
	IzPlatformVideo*		getVideo()			{ return Video; }
	IzPlatformOther*		getOther()			{ return Other; }
	IzPlatformRender*		getRender()			{ return Render; }
	IzPlatformUI*			getUI()				{ return UI; }
	IzPlatformMarket*		getMarket()			{ return Market; }
	IzPlatformCam*			getCam()			{ return Cam; }
	IzPlatformFacebook*		getFacebook()		{ return Facebook; }
	IzPlatformAds*			getAds()			{ return Ads; }
	void setSys(IzPlatformSys* p)				{ Sys = p; }
	void setDisplay(IzPlatformDisplay* p)		{ Display = p; }
	void setInput(IzPlatformInput* p)			{ Input = p; }
	void setAudio(IzPlatformAudio* p)			{ Audio = p; }
	void setComms(IzPlatformComms* p)			{ Comms = p; }
	void setFont(IzPlatformFont* p)				{ Font = p; }
	void setFile(IzPlatformFile* p)				{ File = p; }
	void setImaging(IzPlatformImaging* p)		{ Imaging = p; }
	void setVideo(IzPlatformVideo* p)			{ Video = p; }
	void setOther(IzPlatformOther* p)			{ Other = p; }
	void setRender2d(IzPlatformRender* p)		{ Render = p; }
	void setUI(IzPlatformUI* p)					{ UI = p; }
	void setMarket(IzPlatformMarket* p)			{ Market = p; }
	void setCam(IzPlatformCam* p)				{ Cam = p; }
	void setFacebook(IzPlatformFacebook* p)		{ Facebook = p; }
	void setAds(IzPlatformAds* p)				{ Ads = p; }
	// Properties end

protected:
	IzPlatformSys*		Sys;
	IzPlatformDisplay*	Display;
	IzPlatformInput*	Input;
	IzPlatformAudio*	Audio;
	IzPlatformComms*	Comms;
	IzPlatformFile*		File;
	IzPlatformFont*		Font;
	IzPlatformVideo*	Video;
	IzPlatformImaging*	Imaging;
	IzPlatformOther*	Other;
	IzPlatformRender*	Render;
	IzPlatformUI*		UI;
	IzPlatformCam*		Cam;
	IzPlatformMarket*	Market;
	IzPlatformFacebook*	Facebook;
	IzPlatformAds*		Ads;

public:
	virtual int		Init(IzPlatform* platform, bool comms, bool use_gl);
	virtual void	Release();

};

void	CzPlatform_Create();
void	CzPlatform_Destroy();

#endif	// _CCZ_PLATFORM_H_
