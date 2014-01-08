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

#include "CzPlatformSys.h"
#include "CzPlatformDisplay.h"
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
	Initialised = true;

	return 1;
}

void	CzPlatformSys::Release()
{
}

void	CzPlatformSys::BacklightOn()
{
}

uint64	CzPlatformSys::getTimeInMs()
{
}

int64	CzPlatformSys::getTimeUTC()
{
}

void	CzPlatformSys::YieldToOS(int time_ms)
{
}

bool	CzPlatformSys::ExecuteViaOS(const char* name, bool exit_app)
{
	return false;
}

void	CzPlatformSys::RequestQuit()
{
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
	return false;
}

int		CzPlatformSys::getFreeMemory() const
{
	return 0;
}

int		CzPlatformSys::getUsedMemory() const
{
	return 0;
}

void*	CzPlatformSys::Malloc(int size)
{
	return NULL;
}

void*	CzPlatformSys::Realloc(void* mem, int size)
{
	return NULL;
}

void	CzPlatformSys::Free(void* mem)
{
}

void	CzPlatformSys::ConsoleWrite(const char* message)
{
}

eCzDeviceType CzPlatformSys::getDeviceType()
{
	return CzDeviceType_Unsupported;
}

int CzPlatformSys::getDeviceUDID()
{
	return 0;
}

const char* CzPlatformSys::getDeviceLocale()
{
	return NULL;
}

bool CzPlatformSys::isLittleEndian()
{
	int test = 1;
	return (*((char*)&test) == 1);
}

