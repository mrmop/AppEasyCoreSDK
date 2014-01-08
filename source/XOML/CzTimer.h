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

#if !defined(_CCZ_TIMER_H_)
#define _CCZ_TIMER_H_

#include "CzXoml.h"
#include "CzTypes.h"
#include "CzTime.h"
#include "CzEvents.h"

/**
 @addtogroup XOML
 @{
 */
 
/**
 @class	CzXomlTimer

 @brief	Polled timer

 <h1>Introduction</h1>

 Time plays a very  important role in app and game development. Time allows us to perform useful tasks such as time events, fire off events at regular intervals and stabilise animations etc..

 CzXomlTimer provides a software based timer mechanism for timing events as well a static method for retrieving the current time in milliseconds. Timers will not automatically fire off events 
 when they expire, instead they have to be polled.

 Timers provide additional functionality for calculating how much time is left on the timer as well as how much time has expired since the timer was started.

 Timers don't really have much of an overhead so you can create as many as you like.

 <h1>Getting the Current Time</h1>

 To retrieve the current time in milliseconds CzXomlTimer provides a static method:

 @code
uint64	GetCurrentTimeMs()
 @endcode

 To retrieve the current time in UTC (universal coordinated time) CzXomlTimer provides a static method:

 @code
int64	GetCurrentTimeUTC()
 @endcode

 <h1>Creating and Using Timers</h1>

 Creating a timer is a simple case of declaring or allocating a CzXomlTimer then setting it off going. To check the timer you then poll it to check to see if it has timed out. Here's an example:

 @code
// Create a timer that expires after 10 seconds
CzXomlTimer BusyTimer;
BusyTimer.setDuration(10000);

// Check to see if the timer has timed out
if (BusyTimer.HasTimedOut())
{
}
 @endcode

 Timers can be reset, stopped and started using Reset(), Stop() and Start().

 */

class CzXomlTimer : public IzXomlResource
{
	// Properties
protected:
	bool		Running;				///< Timer running status
	float		CurrentTime;			///< Current timer time
	float		Duration;				///< Duration of timer
	int			RepeatCount;			///< Number of times to repeat before stopping, 0 means repeat forever
	int			OriginalRepeatCount;	///< Original number of times to repeat before stopping
	bool		AutoDelete;				///< If set to true then this timer will de4stroy itself when it times out
public:
	void		setCurrentTime(float time)	{ CurrentTime = time; }
	float		getCurrentTime() const		{ return CurrentTime; }
	void		setDuration(float duration)	{ Duration = duration; }
	float		getDuration() const			{ return Duration; }
	void		setRepeatCount(int count)	{ RepeatCount = count; }
	int			getRepeatCount() const		{ return RepeatCount; }
	void		setAutoDelete(bool auto_delete)	{ AutoDelete = auto_delete; }
	bool		isAutoDelete() const		{ return AutoDelete; }
	void		start()						{ Running = true; }
	void		stop()						{ Running = false; }
	void		restart()					{ Running = true; CurrentTime = 0; RepeatCount = OriginalRepeatCount; }
	bool		isRunning() const			{ return Running; }
	CzEventManager*	getEventsManager()		{ return EventsManager; }
	// Properties end

protected:
	CzEventManager* EventsManager;				///< List of events that the timer handles
	void			addEventsManager();

public:
	CzXomlTimer() : IzXomlResource(), CurrentTime(0), Duration(0), RepeatCount(1), OriginalRepeatCount(0), AutoDelete(false), EventsManager(NULL), Running(true)
	{
		setClassType("timer");
	}
	virtual ~CzXomlTimer();

	// Update the timer
	bool		Update(float dt);

	// Event notification
	void		ProcessEventActions(unsigned int event_name);
	void		NotifyTimeout();
	void		NotifyRepeat();
	
	// Implementation of IzXomlResource interface
	int			LoadFromXoml(IzXomlResource* parent, bool load_children, CzXmlNode* node);

};

/**
 @class	CzXomlTimerCreator

 @brief	Creates an instance of a Timer object.

 */

class CzXomlTimerCreator : public IzXomlClassCreator
{
public:
	CzXomlTimerCreator()
	{
		setClassName("timer");
	}
	IzXomlResource*	CreateInstance(IzXomlResource* parent)	{ return new CzXomlTimer(); }
};

/**
 @class	CzTimersManager

 @brief	Manager a collection of XOML timers

 */

class CzTimersManager
{
public:
	// Public access to iteration
	typedef CzList<CzXomlTimer*>::iterator _Iterator;
	_Iterator				begin()		{ return Timers.begin(); }
	_Iterator				end()		{ return Timers.end(); }

protected:
	// Properties
	CzList<CzXomlTimer*>	Timers;				// A collection of Timers
	IzXomlResource*			Parent;				// Parent container
	CzList<CzXomlTimer*>	Removals;			// Timers that require removal
public:
	void					addTimer(CzXomlTimer* Timer);
	void					removeTimer(CzXomlTimer* Timer);
	void					removeTimer(unsigned int name_hash);
	CzXomlTimer*			findTimer(unsigned int name_hash);
	CzXomlTimer*			findTimer(const char* name);
	void					clearTimers();
	void					setParent(IzXomlResource* scene)		{ Parent = scene; }
	IzXomlResource*			getParent()								{ return Parent; }
	// Properties end

public:
	CzTimersManager() : Parent(NULL) {}
	virtual ~CzTimersManager() { clearTimers(); }

	void	Update(float dt);

	// Utility
	static CzXomlTimer*	FindTimer(const char* name, IzXomlResource* container);
	static CzXomlTimer*	FindTimer(unsigned int name_hash, IzXomlResource* container);
	static CzXomlTimer*	FindTimer(unsigned int name_hash, CzScene* container, unsigned int actor_hash);
	static CzXomlTimer*	FindTimer(const char* name, CzScene* container, const char* actor_name);
};




/// @}

#endif	// _CCZ_TIMER_H_
