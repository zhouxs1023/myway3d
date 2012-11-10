#include "MWThread.h"

using namespace Myway;

#ifdef MW_PLATFORM_WIN32

Thread::Thread()
: mThread(NULL),
  mPaused(TRUE)
{
}

Thread::~Thread()
{
}

void Thread::Intialize(_thread_proc proc)
{
    assert (!mThread);

    DWORD id = 0;
    mThread = CreateThread(NULL,0, proc, NULL, CREATE_SUSPENDED, &id);
    mPaused = TRUE;

    assert (mThread);
}

void Thread::Shudown(bool wait)
{
    if (mThread)
    {
        if (wait)
            WaitForSingleObject(mThread, INFINITE);

        TerminateThread(mThread, 0);
        CloseHandle(mThread);
        mThread = NULL;
    } 
}

void Thread::Run()
{
    assert (mThread);

    ResumeThread(mThread);
    mPaused = TRUE;
}

void Thread::Pause()
{
    assert (mThread);

    SuspendThread(mThread);
    mPaused = TRUE;
}

bool Thread::IsPause() const
{
    return mPaused;
}

#endif