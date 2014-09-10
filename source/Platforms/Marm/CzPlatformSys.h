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

#if !defined(_CCZ_PLATFORM_SYS_H_)
#define _CCZ_PLATFORM_SYS_H_

#include "IzPlatformSys.h"

class CzPlatformSys : public IzPlatformSys
{
	// Properties
protected:
public:
	// Properties end
public:
	CzPlatformSys() : IzPlatformSys() {}

	int		Init();
	void	Release();

	// Backlight
	void	BacklightOn();

	// Time
	uint64	getTimeInMs();
	int64	getTimeUTC();

	// OS calls
	void	YieldToOS(int time_ms);
	bool	ExecuteViaOS(const char* name, bool exit_app);
	void	RequestQuit();

	// Event notification
	void	NotifySuspended();
	void	NotifyResumed();
	void	NotifyVolumeUp();
	void	NotifyVolumeDown();
	void	NotifyNetworkOnline();
	void	NotifyNetworkOffline();
	void	NotifyScreenSizeChanged();

	// App exit
	bool	CheckAppQuit();

	// Memory
	int		getFreeMemory() const;
	int		getUsedMemory() const;
	void*	Malloc(int size);
	void*	Realloc(void* mem, int size);
	void	Free(void* mem);

	// Console output
	void	ConsoleWrite(const char* message);

	// Device type query
	eCzDeviceType	getDeviceType();

	// Device OS version query
	const char*		getDeviceOSVersion();
	int				getDeviceOSVersionInt();

	// Device UDID
	int				getDeviceUDID();

	// Device language locale
	const char*		getDeviceLocale();

	// Endianness query
	bool			isLittleEndian();
};


#endif	// _CCZ_PLATFORM_SYS_H_
