/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __TIMER_H
#define __TIMER_H

#include "StandardHeaders.h"
#include "SourceDeclaration.h"
#include "Time.h"


#if PLATFORM == PLATFORM_WINDOWS	// only use this when compiling for Windows

#include <windows.h>
#include <time.h>


namespace MCore
{

/**
 * A high frequency timer, using QueryPerformanceCounter.
 */
class Timer
{
	DECLARE_CLASS(Timer)

	public:
		/**
		 * Default constructor.
		 */
		Timer();

		/**
		 * Destructor.
		 */
		virtual ~Timer();

		/**
		 * Returns the current time value in seconds since the timer has been created or since
		 * the last Reset() has been called.
		 * @result The time in seconds, since the timer has been created.
		 */
		inline double GetTime();

		/**
		 * Returns the time value in seconds since the last call to GetTimeDelta().
		 * @result Time delta, in seconds.
		 */
		inline double GetTimeDelta();

		/**
		 * Reset the timer.
		 * This will re-init the start time.
		 */
		void Reset();


	private:
		signed __int64	timerFreq, startTime, curTime;
		double			timerFreqD, startTimeD;
		double			lastTimeD;
};

} // namespace MCore

#include "Timer.inl"

#endif	// #if PLATFORM == PLATFORM_WINDOWS


#endif