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

#if !defined(_CCZ_TIME_H_)
#define _CCZ_TIME_H_

#include "CzTypes.h"

/**
 @addtogroup Core
 @{
 */
 
/**
 @class	CzTimer

 @brief	Polled timer

 <h1>Introduction</h1>

 Time plays a very  important role in app and game development. Time allows us to perform useful tasks such as time events, fire off events at regular intervals and stabilise animations etc..

 CzTimer provides a software based timer mechanism for timing events as well a static method for retrieving the current time in milliseconds. Timers will not automatically fire off events 
 when they expire, instead they have to be polled.

 Timers provide additional functionality for calculating how much time is left on the timer as well as how much time has expired since the timer was started.

 Timers don't really have much of an overhead so you can create as many as you like.

 <h1>Getting the Current Time</h1>

 To retrieve the current time in milliseconds CzTimer provides a static method:

 @code
uint64	GetCurrentTimeMs()
 @endcode

 To retrieve the current time in UTC (universal coordinated time) CzTimer provides a static method:

 @code
int64	GetCurrentTimeUTC()
 @endcode

 <h1>Creating and Using Timers</h1>

 Creating a timer is a simple case of declaring or allocating a CzTimer then setting it off going. To check the timer you then poll it to check to see if it has timed out. Here's an example:

 @code
// Create a timer that expires after 10 seconds
CzTimer BusyTimer;
BusyTimer.setDuration(10000);

// Check to see if the timer has timed out
if (BusyTimer.HasTimedOut())
{
}
 @endcode

 Timers can be reset, stopped and started using Reset(), Stop() and Start().

 */

class CzTimer
{
protected:
	bool		Started;			///< true if timer started
	uint64 		LastTime;			///< Time that the timer was set (milliseconds)

	// Properties
private:
	uint64		Duration;			///< Duration of timer in milliseconds
	bool		AutoReset;			///< Auto reset the timer when it times out
public:
	void		setDuration(uint64 millseconds_duration, bool start = true);		///< Sets the duration of the timer
	uint64		getDuration() const;												///< Gets the duration of the timer
	void		setAutoReset(bool auto_reset);										///< If true the timer will aito restart once it times out
	bool		getAutoReset() const;												///< Returns auto resset status
	bool		hasStarted() const;													///< Returns true if the timer has started
	bool		hasStopped() const;													///< Returns true if the timer has stopped
	// Properties end

public:
	CzTimer() : LastTime(0), Duration(0), AutoReset(false), Started(false) { }
	CzTimer(int millseconds_duration);
	virtual ~CzTimer() {	}
	
	virtual bool HasTimedOut();														///< Returns true if the timer has timed out
	uint64		GetElapsedTime() const;												///< Return the amount of time since the timer was started
	uint64		GetTimeDiff(uint64 this_time) const;								///< returns the difference in time between the supplied time and the timers current time
	uint64		GetTimeDiff() const;												///< returns the difference in time between the current time and the timers current time
	uint64		GetTimeLeft() const;												///< Returns the amount of time left before the timer times out
	void		Reset();															///< Restarts the timer
	void		Start();															///< Starts the timer
	void		Stop();																///< Stops the timer

	static uint64	GetCurrentTimeMs();												///< Static method that returns the current system time in millseconds
	static int64	GetCurrentTimeUTC();											///< Static method that returns the current system UTC time

};

/// @}

#endif	// _CCZ_TIME_H_
