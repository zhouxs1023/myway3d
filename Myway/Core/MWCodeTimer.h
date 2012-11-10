#pragma once

#include "MWDefine.h"
#include "MWString.h"
#include "MWSingleton.h"

namespace Myway
{

#define _max_func_size          256

#ifdef MW_PROFILE_CODE_TIMER

#define profile_code()          profile_code_time(__LINE__)

#define profile_code_time(name) static CodeTimer __ct_##name(__FUNCTION__); \
                                FuncTimer _ct_func_##name(&__ct_##name)

#define begin_code_time(name)   static CodeTimer(#name) __ct_##name; \
                                __ct_##name.Begin()

#define end_code_time(name)     static __s_##name.End()

#define output_all_code_time()  Myway::CodeTimerManager::Instance()->OuputAll()

#else

#define profile_code()
#define profile_code_time(name)
#define begin_code_time(name)
#define end_code_time(name)
#define output_all_code_time()

#endif


class MW_ENTRY CodeTimer
{
    friend class CodeTimerManager;

public:
                    CodeTimer(const char * func);
                    ~CodeTimer();

    void            Begin();
    void            End();
    void            Reset();
    void            Output() const;

    const char *    Function() const;
    float           AverageTime() const;
    float           TotalTime() const;
    float           CurrentTime() const;
    int             TotalCalls() const;
    float           MaxTime() const;
    float           MinTime() const;

protected:
    char            munction[_max_func_size];
    float           mStartTime;
    float           mTotalTime;
    float           mCurrentTime;
    int             mTotalCalls;
    float           mMaxTime;
    float           mMinTime;
    float           mAveTime;
    CodeTimer *     mNext;
};

class MW_ENTRY FuncTimer
{
public:
    FuncTimer(CodeTimer * timer);
    ~FuncTimer();

protected:
    CodeTimer * mCodeTime;
};

class MW_ENTRY CodeTimerManager
{
    DECLARE_SINGLETON (CodeTimerManager);

public:
                    CodeTimerManager();
                    ~CodeTimerManager();

    void            AddTime(CodeTimer * time);

    void            OuputAll();

protected:
    CodeTimer *     mTimers;
};

#include "MWCodeTimer.inl"

}
