/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Timer.h"


namespace MCore
{

inline double Timer::GetTime()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
	return (((double)curTime * timerFreqD)) - startTimeD;
}


inline double Timer::GetTimeDelta()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);

	double time = (((double)curTime * timerFreqD));
	double timeDelta = time - lastTimeD;
	lastTimeD = time;

	return timeDelta;
}

}	// namespace MCore