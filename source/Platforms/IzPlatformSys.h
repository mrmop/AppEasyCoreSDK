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

#if !defined(_CIZ_PLATFORM_SYS_H_)
#define _CIZ_PLATFORM_SYS_H_

#include "CzPlatform.h"

#define	PLATFORM_SYS		PLATFORM->getSys()

#define	CZ_Malloc(size)			(PLATFORM_SYS->Malloc(size))
#define	CZ_Realloc(mem, size)	(PLATFORM_SYS->Realloc(mem, size))
#define	CZ_Free(mem)			(PLATFORM_SYS->Free(mem))

enum eCzDeviceType
{
	CzDeviceType_Unsupported	= -1,
	CzDeviceType_iPhone			= 0,
	CzDeviceType_iPad			= 1,
	CzDeviceType_Android		= 2,
	CzDeviceType_QNX			= 3,
	CzDeviceType_Windows		= 4,
	CzDeviceType_OSX			= 5,
	CzDeviceType_WP8			= 6,
	CzDeviceType_WP81			= 7,
	CzDeviceType_WS8			= 8,
	CzDeviceType_WS81			= 9,
	CzDeviceType_TIZEN			= 10,
	CzDeviceType_ROKU			= 11,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// IzPlatformSys
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IzPlatformSys
{
	// Properties
protected:
	bool		Initialised;
	bool		Suspended;
	CzCallback	SuspendedCallback;			// Callback to be called when device is suspended
	CzCallback	ResumedCallback;			// Callback to be called when device is resuming
	CzCallback	VolumeUpCallback;			// Callback to be called when device volume up is pressed
	CzCallback	VolumeDownCallback;			// Callback to be called when device volume down is pressed
	CzCallback	NetworkOnlineCallback;		// Callback to be called when device network goes online
	CzCallback	NetworkOfflineCallback;		// Callback to be called when device network goes offline
	CzCallback	ScreenSizeChangedCallback;	// Callback to be called when device screen size changes
public:
	bool		isInitialised() const								{ return Initialised; }
	bool		isSuspended() const									{ return Suspended; }
	void		setSuspendedCallback(CzCallback callback)			{ SuspendedCallback			= callback; }
	void		setResumedCallback(CzCallback callback)				{ ResumedCallback			= callback; }
	void		setVolumeUpCallback(CzCallback callback)			{ VolumeUpCallback			= callback; }
	void		setVolumeDownCallback(CzCallback callback)			{ VolumeDownCallback		= callback; }
	void		setNetworkOnlineCallback(CzCallback callback)		{ NetworkOnlineCallback		= callback; }
	void		setNetworkOfflineCallback(CzCallback callback)		{ NetworkOfflineCallback	= callback; }
	void		setScreenSizeChangedCallback(CzCallback callback)	{ ScreenSizeChangedCallback	= callback; }
	// Properties end
public:
	virtual ~IzPlatformSys() {}
	IzPlatformSys() :	SuspendedCallback(NULL), ResumedCallback(NULL), VolumeUpCallback(NULL), VolumeDownCallback(NULL), NetworkOnlineCallback(NULL), NetworkOfflineCallback(NULL), 
						ScreenSizeChangedCallback(NULL), Suspended(false), Initialised(false) {}

	virtual int		Init() = 0;
	virtual void	Release() = 0;

	// Backlight
	virtual void	BacklightOn() = 0;

	// Time
	virtual uint64	getTimeInMs() = 0;
	virtual int64	getTimeUTC() = 0;

	// OS calls
	virtual void	YieldToOS(int time_ms) = 0;
	virtual bool	ExecuteViaOS(const char* name, bool exit_app) = 0;
	virtual void	RequestQuit() = 0;

	// Event notification
	virtual void	NotifySuspended() = 0;
	virtual void	NotifyResumed() = 0;
	virtual void	NotifyVolumeUp() = 0;
	virtual void	NotifyVolumeDown() = 0;
	virtual void	NotifyNetworkOnline() = 0;
	virtual void	NotifyNetworkOffline() = 0;
	virtual void	NotifyScreenSizeChanged() = 0;

	// App exit
	virtual bool	CheckAppQuit() = 0;

	// Memory
	virtual int		getFreeMemory() const = 0;
	virtual int		getUsedMemory() const = 0;
	virtual void*	Malloc(int size) = 0;
	virtual void*	Realloc(void* mem, int size) = 0;
	virtual void	Free(void* mem) = 0;

	// Console output
	virtual void	ConsoleWrite(const char* message) = 0;

	// Device type query
	virtual eCzDeviceType	getDeviceType() = 0;

	// Device OS version query
	virtual const char*		getDeviceOSVersion() = 0;

	// Device OS version query
	virtual int				getDeviceOSVersionInt() = 0;

	// Device UDID
	virtual int				getDeviceUDID() = 0;

	// Device language locale
	virtual const char*		getDeviceLocale() = 0;

	// Endianness query
	virtual bool			isLittleEndian() = 0;

};


#endif	// _CIZ_PLATFORM_SYS_H_
