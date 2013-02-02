/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Timer.h"


namespace MCore
{

#if PLATFORM == PLATFORM_WINDOWS	// only use this when compiling for Windows

// constructor
Timer::Timer() : lastTimeD(0.0), timerFreqD(0.0), startTimeD(0.0), startTime(0), timerFreq(0)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&timerFreq);
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
	timerFreqD = (1.0 / (double)timerFreq);

	// reset the timer
	Reset();
}


// destructor
Timer::~Timer()
{
}


// reset the timer
void Timer::Reset()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
	startTimeD = startTime * timerFreqD;
	lastTimeD  = startTimeD;
}

#endif	// #if PLATFORM == PLATFORM_WINDOWS

}	// namespace MCore