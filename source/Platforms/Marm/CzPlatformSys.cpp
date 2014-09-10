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

#include "Marm/CzPlatformSys.h"
#include "Marm/CzPlatformDisplay.h"
#include "s3eOSExec.h"
#include "s3e.h"
#include "CzApp.h"

//
// System callbacks
// 
int32 CzPlatformSys_Suspend(void *systemData, void *userData)
{
	PLATFORM_SYS->NotifySuspended();
	return 1;
}
int32 CzPlatformSys_Resume(void *systemData, void *userData)
{
	PLATFORM_SYS->NotifyResumed();
	return 1;
}
int32 CzPlatformSys_VolumeUp(void *systemData, void *userData)
{
	PLATFORM_SYS->NotifyVolumeUp();
	return 1;
}
int32 CzPlatformSys_VolumeDown(void *systemData, void *userData)
{
	PLATFORM_SYS->NotifyVolumeDown();
	return 1;
}
int32 CzPlatformSys_NetworkOnline(void *systemData, void *userData)
{
	PLATFORM_SYS->NotifyNetworkOnline();
	return 1;
}
int32 CzPlatformSys_NetworkOffline(void *systemData, void *userData)
{
	PLATFORM_SYS->NotifyNetworkOffline();
	return 1;
}
int32 CzPlatformSys_ScreenSizeChanged(void *systemData, void *userData)
{
	PLATFORM_SYS->NotifyScreenSizeChanged();
	return 1;
}


//
// Marmalade System
//
int		CzPlatformSys::Init()
{
	s3eDeviceRegister(S3E_DEVICE_PAUSE, CzPlatformSys_Suspend, NULL);
	s3eDeviceRegister(S3E_DEVICE_UNPAUSE, CzPlatformSys_Resume, NULL);
	s3eDeviceRegister(S3E_DEVICE_VOL_UP, CzPlatformSys_VolumeUp, NULL);
	s3eDeviceRegister(S3E_DEVICE_VOL_DOWN, CzPlatformSys_VolumeDown, NULL);
	s3eDeviceRegister(S3E_DEVICE_NETWORK_ONLINE, CzPlatformSys_NetworkOnline, NULL);
	s3eDeviceRegister(S3E_DEVICE_NETWORK_OFFLINE, CzPlatformSys_NetworkOffline, NULL);
	s3eSurfaceRegister(S3E_SURFACE_SCREENSIZE, CzPlatformSys_ScreenSizeChanged, NULL);

	Initialised = true;

	return 1;
}

void	CzPlatformSys::Release()
{
}

void	CzPlatformSys::BacklightOn()
{
	s3eDeviceBacklightOn();
}

uint64	CzPlatformSys::getTimeInMs()
{
	return s3eTimerGetMs();
}

int64	CzPlatformSys::getTimeUTC()
{
	return s3eTimerGetUTC();
}

void	CzPlatformSys::YieldToOS(int time_ms)
{
	s3eDeviceYield(time_ms);
}

bool	CzPlatformSys::ExecuteViaOS(const char* name, bool exit_app)
{
	return s3eOSExecExecute(name, exit_app) == S3E_RESULT_SUCCESS;
}

void	CzPlatformSys::RequestQuit()
{
	s3eDeviceRequestQuit();
}

void	CzPlatformSys::NotifySuspended()
{
	if (SuspendedCallback != NULL)
		SuspendedCallback(this, NULL);
	Suspended = true;
}
void	CzPlatformSys::NotifyResumed()
{
	if (ResumedCallback != NULL)
		ResumedCallback(this, NULL);
	Suspended = false;
}
void	CzPlatformSys::NotifyVolumeUp()
{
	if (SuspendedCallback != NULL)
		SuspendedCallback(this, NULL);
}
void	CzPlatformSys::NotifyVolumeDown()
{
	if (SuspendedCallback != NULL)
		SuspendedCallback(this, NULL);
}
void	CzPlatformSys::NotifyNetworkOnline()
{
	if (SuspendedCallback != NULL)
		SuspendedCallback(this, NULL);
}
void	CzPlatformSys::NotifyNetworkOffline()
{
	if (SuspendedCallback != NULL)
		SuspendedCallback(this, NULL);
}

void	CzPlatformSys::NotifyScreenSizeChanged()
{
	if (ScreenSizeChangedCallback != NULL)
		ScreenSizeChangedCallback(this, NULL);
}

bool	CzPlatformSys::CheckAppQuit()
{
	return s3eDeviceCheckQuitRequest() == S3E_TRUE;
}

int		CzPlatformSys::getFreeMemory() const
{
	return s3eMemoryGetInt(S3E_MEMORY_FREE);
}

int		CzPlatformSys::getUsedMemory() const
{
	return s3eMemoryGetInt(S3E_MEMORY_USED);
}

void*	CzPlatformSys::Malloc(int size)
{
	return s3eMalloc(size);
}

void*	CzPlatformSys::Realloc(void* mem, int size)
{
	return s3eRealloc(mem, size);
}

void	CzPlatformSys::Free(void* mem)
{
	s3eFree(mem);
}

void	CzPlatformSys::ConsoleWrite(const char* message)
{
	s3eDebugOutputString(message);
}

eCzDeviceType CzPlatformSys::getDeviceType()
{
	// Get the devices operating system
	int width = PLATFORM_DISPLAY->getCurrentWidth();
	int height = PLATFORM_DISPLAY->getCurrentHeight();
	int os = s3eDeviceGetInt(S3E_DEVICE_OS);

	// Calculate portal type from OS
	switch (os)
	{
	case S3E_OS_ID_QNX:
		return CzDeviceType_QNX;
	case S3E_OS_ID_ANDROID:
		return CzDeviceType_Android;
	case S3E_OS_ID_IPHONE:
		if (width == 1024 && height == 768 || height == 768 && width == 1024 || width == 2048 && height == 1536 || height == 1536 && width == 2048)
			return CzDeviceType_iPad;
		else
			return CzDeviceType_iPhone;
	case S3E_OS_ID_WINDOWS:
		return CzDeviceType_Windows;
	case S3E_OS_ID_OSX:
		return CzDeviceType_OSX;
	case S3E_OS_ID_WP8:
		return CzDeviceType_WP8;
	case S3E_OS_ID_WP81:
		return CzDeviceType_WP81;
	case S3E_OS_ID_WS8:
		return CzDeviceType_WS8;
	case S3E_OS_ID_WS81:
		return CzDeviceType_WS81;
	case S3E_OS_ID_TIZEN:
		return CzDeviceType_TIZEN;
	case S3E_OS_ID_ROKU:
		return CzDeviceType_ROKU;
	}

	return CzDeviceType_Unsupported;
}

const char* CzPlatformSys::getDeviceOSVersion()
{
	return s3eDeviceGetString(S3E_DEVICE_OS_VERSION);
}

int CzPlatformSys::getDeviceOSVersionInt()
{
	return s3eDeviceGetInt(S3E_DEVICE_OS_VERSION);
}

int CzPlatformSys::getDeviceUDID()
{
	return s3eDeviceGetInt(S3E_DEVICE_UNIQUE_ID);
}

const char* CzPlatformSys::getDeviceLocale()
{
	return s3eDeviceGetString(S3E_DEVICE_LOCALE);
}

bool CzPlatformSys::isLittleEndian()
{
	int test = 1;
	return (*((char*)&test) == 1);
}

