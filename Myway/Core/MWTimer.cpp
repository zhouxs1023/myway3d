#include "MWTimer.h"

#ifdef MW_PLATFORM_WIN32
#include <windows.h>
#else
#error not suppored.
#endif


using namespace Myway;

#define ON				1
#define OFF				2
#define SUPENDED		4

#ifdef MW_PLATFORM_WIN32

#define SAMPLE_TIME(t)  { QueryPerformanceCounter(&Sample); t = Sample.QuadPart; }
static LARGE_INTEGER	Sample;

#define SAMPLE_TIME_L(t) { t = timeGetTime(); }

#else

#error not suppored.

#endif


Timer_h::Timer_h()
{
	SecondsFrequency = 0;
	MillisecondsFrequency = 0;
	InvSecondsFrequency = 0.0f;
	StartTime = 0;
	StopTime = 0;
	State = OFF;

	//setup time frequency
	if (SecondsFrequency == 0)
	{
#ifdef MW_PLATFORM_WIN32

		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		SecondsFrequency = frequency.QuadPart;
		MillisecondsFrequency = frequency.QuadPart / 1000;
		InvSecondsFrequency = 1.0f / float(SecondsFrequency);

#else

        #error not suppored.

#endif
	}
}

Timer_h::~Timer_h()
{
}

void Timer_h::Start()
{
	SAMPLE_TIME(StartTime);
	StopTime = StartTime;
	State = ON;
}

void Timer_h::Stop()
{
	if (State == ON)
	{
		SAMPLE_TIME(StopTime);
		State = OFF;
	}
}

void Timer_h::Supend()
{
	if (State == ON)
	{
		SAMPLE_TIME(StopTime);
		State = SUPENDED;
	}
}

void Timer_h::Resume()
{
	if (State == SUPENDED)
	{
		uint64 elapsed;
		SAMPLE_TIME(elapsed);
		StartTime += elapsed - StopTime;
		State = ON;
	}
}

float Timer_h::ElapsedTime()
{
	if (State != ON)
		return float(StopTime - StartTime) * InvSecondsFrequency;
	else 
	{
		SAMPLE_TIME(StopTime);
		return float(StopTime - StartTime) * InvSecondsFrequency;
	}
}

uint64 Timer_h::ElapsedSeconds()
{
	if (State != ON)
		return (StopTime - StartTime) / SecondsFrequency;
	else
	{
		SAMPLE_TIME(StopTime);
		return (StopTime - StartTime) / SecondsFrequency;
	}
}

uint64 Timer_h::ElapsedMilliseconds()
{
	if (State != ON)
		return (StopTime - StartTime) / MillisecondsFrequency;
	else
	{
		SAMPLE_TIME(StopTime);
		return (StopTime - StartTime) / MillisecondsFrequency;
	}
}

uint64 Timer_h::ElapsedCount()
{
	if (State != ON)
		return (StopTime - StartTime);
	else
	{
		SAMPLE_TIME(StopTime);
		return (StopTime - StartTime);
	}
}







Timer_l::Timer_l()
{
	StartTime = 0;
	StopTime = 0;
	State = OFF;
}

Timer_l::~Timer_l()
{
}

void Timer_l::Start()
{
	SAMPLE_TIME_L(StartTime);
	StopTime = StartTime;
	State = ON;
}

void Timer_l::Stop()
{
	if (State == ON)
	{
		SAMPLE_TIME_L(StopTime);
		State = OFF;
	}
}

void Timer_l::Supend()
{
	if (State == ON)
	{
		SAMPLE_TIME_L(StopTime);
		State = SUPENDED;
	}
}

void Timer_l::Resume()
{
	if (State == SUPENDED)
	{
		uint64 elapsed;
		SAMPLE_TIME_L(elapsed);
		StartTime += elapsed - StopTime;
		State = ON;
	}
}

float Timer_l::ElapsedTime()
{
	if (State != ON)
		return float(StopTime - StartTime) / 1000.0f;
	else 
	{
		SAMPLE_TIME_L(StopTime);
		return float(StopTime - StartTime) / 1000.0f;
	}
}

uint64 Timer_l::ElapsedSeconds()
{
	if (State != ON)
		return (StopTime - StartTime) / 1000;
	else
	{
		SAMPLE_TIME_L(StopTime);
		return (StopTime - StartTime) / 1000;
	}
}

uint64 Timer_l::ElapsedMilliseconds()
{
	if (State != ON)
		return (StopTime - StartTime);
	else
	{
		SAMPLE_TIME_L(StopTime);
		return (StopTime - StartTime);
	}
}

uint64 Timer_l::ElapsedCount()
{
	if (State != ON)
		return (StopTime - StartTime);
	else
	{
		SAMPLE_TIME_L(StopTime);
		return (StopTime - StartTime);
	}
}







SYSTEMTIME SysTime;

SystemTimer gSysTimer;

SystemTimer::SystemTimer()
{
	timeBeginPeriod(1);
}

SystemTimer::~SystemTimer()
{
	timeEndPeriod(1);
}

void SystemTimer::Setup()
{
#ifdef MW_PLATFORM_WIN32

    ::GetSystemTime(&SysTime);
    ::SystemTimeToTzSpecificLocalTime(NULL, &SysTime, &SysTime);

#else

    #error not suppored.

#endif
}

int SystemTimer::Year()
{
    return (int)SysTime.wYear;
}

int SystemTimer::Month()
{
    return (int)SysTime.wMonth;
}

int SystemTimer::Day()
{
    return (int)SysTime.wDay;
}

int SystemTimer::Hour()
{
    return (int)SysTime.wHour;
}

int SystemTimer::Minute()
{
    return (int)SysTime.wMinute;
}

int SystemTimer::Second()
{
    return (int)SysTime.wSecond;
}

int SystemTimer::MilliSecond()
{
    return (int)SysTime.wMilliseconds;
}