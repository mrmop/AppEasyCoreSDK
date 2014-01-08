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

#include "IzPlatformSys.h"
#include "CzTime.h"

uint64 CzTimer::getDuration() const				{ return Duration; }
void CzTimer::setAutoReset(bool auto_reset)		{ AutoReset = auto_reset; }
bool CzTimer::getAutoReset() const				{ return AutoReset; }
bool CzTimer::hasStarted() const				{ return Started; }
bool CzTimer::hasStopped() const				{ return !Started; }

void CzTimer::setDuration(uint64 millseconds_duration, bool start)
{
	Duration = millseconds_duration;
	if (start)
		Start();
}

CzTimer::CzTimer(int millseconds_duration)
{
	LastTime = 0;
	Duration = 0;
	AutoReset = false;
	Started = false;
	setDuration(millseconds_duration);
}

bool CzTimer::HasTimedOut()
{
	if (!Started)
		return false;

	if (Duration == 0)	// Timer of 0 duration never times out
	{
		return false;
	}
		
	uint64 time_diff = GetElapsedTime();
	
	if (time_diff > Duration)
	{
		if (AutoReset)
		{
			LastTime = PLATFORM_SYS->getTimeInMs();
		}
		else
			Started = false;
		return true;
	}
	
	return false;
}

uint64 CzTimer::GetElapsedTime() const
{
	return PLATFORM_SYS->getTimeInMs() - LastTime;
}

uint64 CzTimer::GetTimeDiff(uint64 this_time) const
{
	return this_time - LastTime;
}

uint64 CzTimer::GetTimeDiff() const
{
	return PLATFORM_SYS->getTimeInMs() - LastTime;
}

uint64 CzTimer::GetTimeLeft() const
{
	return Duration - (PLATFORM_SYS->getTimeInMs() - LastTime);
}

void CzTimer::Reset()
{
	setDuration(Duration);
}

void CzTimer::Start()
{
	LastTime = PLATFORM_SYS->getTimeInMs();
	Started = true;
}

void CzTimer::Stop()
{
	Started = false;
}

uint64 CzTimer::GetCurrentTimeMs()
{
	return PLATFORM_SYS->getTimeInMs();
}

int64 CzTimer::GetCurrentTimeUTC()
{
	return PLATFORM_SYS->getTimeUTC();
}




