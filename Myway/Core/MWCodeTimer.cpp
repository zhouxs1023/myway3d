#include "MWCodeTimer.h"
#include "MWSystem.h"

using namespace Myway;

CodeTimer::CodeTimer(const char * func)
: mStartTime(0),
  mTotalTime(0),
  mCurrentTime(0),
  mTotalCalls(0),
  mMaxTime(0),
  mMinTime(FLT_MAX),
  mAveTime(0),
  mNext(NULL)
{
    Strcpy(munction, _max_func_size, func);
    CodeTimerManager::Instance()->AddTime(this);
}

CodeTimer::~CodeTimer()
{
}

void CodeTimer::Begin()
{
    if (!mStartTime)
    {
        ++mTotalCalls;
        mStartTime = System::InternalTime.ElapsedTime();
    }
}

void CodeTimer::End()
{
    if (mStartTime)
    {
        mCurrentTime = System::InternalTime.ElapsedTime() - mStartTime;
        assert (mCurrentTime >= 0);
        mTotalTime +=  mCurrentTime;
        mAveTime = (mAveTime + mCurrentTime) * 0.5f;
        
        if (mMinTime > mCurrentTime)
            mMinTime = mCurrentTime;
        if (mMaxTime < mCurrentTime)
            mMaxTime = mCurrentTime;

        mStartTime = 0.0f;
    }
}

void CodeTimer::Reset()
{
    mStartTime = 0.0f;
    mTotalTime = 0.0f;
    mTotalCalls = 0;
    mMaxTime = 0.0f;
    mMinTime = FLT_MAX;
    mAveTime = 0.0f;
}

void CodeTimer::Output() const
{
    debug_printf("---------------------------------------------------------\n"  \
                 "    fuction:          %s\n" \
                 "    total time:       %f\n" \
                 "    current time:     %f\n" \
                 "    average time:     %f\n" \
                 "    total calls:      %d\n" \
                 "    max time:         %f\n" \
                 "    min time:         %f\n\n",
                 Function(),
                 TotalTime(),
                 CurrentTime(),
                 AverageTime(),
                 TotalCalls(),
                 MaxTime(),
                 MinTime());
}

CodeTimerManager gMgr;


IMP_SLN(CodeTimerManager);
CodeTimerManager::CodeTimerManager()
: mTimers(NULL)
{
    INIT_SLN;
}

CodeTimerManager::~CodeTimerManager()
{
    SHUT_SLN;
}

void CodeTimerManager::AddTime(CodeTimer * time)
{
    time->mNext = mTimers;
    mTimers = time;
}

void CodeTimerManager::OuputAll()
{
    CodeTimer * time = mTimers;

    while (time)
    {
        time->Output();
        time = time->mNext;
    }
}