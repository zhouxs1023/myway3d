#pragma once

#include "MWAllocObj.h"

namespace Myway
{

// 高精度时间计时器， win7 双核下不稳定
class MW_ENTRY Timer_h : public AllocObj
{
public:
	Timer_h();
	~Timer_h();

	void		Start();
	void		Stop();
	void		Supend();
	void		Resume();

	float		ElapsedTime();
	uint64		ElapsedSeconds();
	uint64		ElapsedMilliseconds();
	uint64		ElapsedCount();

private:
	uint64		SecondsFrequency;
	uint64		MillisecondsFrequency;
	float		InvSecondsFrequency;
	uint64		StartTime;
	uint64		StopTime;
	int		    State;
};


// 低精度计时器
class MW_ENTRY Timer_l
{
	DECLARE_ALLOC();

public:
	Timer_l();
	~Timer_l();

	void		Start();
	void		Stop();
	void		Supend();
	void		Resume();

	float		ElapsedTime();
	uint64		ElapsedSeconds();
	uint64		ElapsedMilliseconds();
	uint64		ElapsedCount();

protected:
	uint64		StartTime;
	uint64		StopTime;
	int		State;
};


#ifdef MW_HIGHT_TIMER

typedef Timer_h Timer

#else

typedef Timer_l Timer;

#endif



// 系统时间
class MW_ENTRY SystemTimer
{
public:
	SystemTimer();
	~SystemTimer();

    static void  Setup();
    static int   Year();
    static int   Month();
    static int   Day();
    static int   Hour();
    static int   Minute();
    static int   Second();
    static int   MilliSecond();
};

}