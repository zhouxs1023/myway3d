#include "MGUI_Timer.h"

#pragma comment(lib, "winmm.lib")

namespace Myway {

	MGUI_Timer::MGUI_Timer()
		: mTimeStart(0)
	{
	}

	void MGUI_Timer::Reset()
	{
		mTimeStart = GetCurrentMilliseconds();
	}

	unsigned long MGUI_Timer::GetMilliseconds()
	{
		return GetCurrentMilliseconds() - mTimeStart;
	}

	unsigned long MGUI_Timer::GetCurrentMilliseconds()
	{
		return timeGetTime();
	}

}