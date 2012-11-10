

inline const char * CodeTimer::Function() const
{
    return munction;
}

inline float CodeTimer::AverageTime() const
{
    return mAveTime;
}

inline float CodeTimer::TotalTime() const
{
    return mTotalTime;
}

inline float CodeTimer::CurrentTime() const
{
    return mCurrentTime;
}

inline int CodeTimer::TotalCalls() const
{
    return mTotalCalls;
}

inline float CodeTimer::MaxTime() const
{
    return mMaxTime;
}

inline float CodeTimer::MinTime() const
{
    return mMinTime;
}


inline FuncTimer::FuncTimer(CodeTimer * timer)
: mCodeTime(timer)
{
    mCodeTime->Begin();
}

inline FuncTimer::~FuncTimer()
{
    mCodeTime->End();
}