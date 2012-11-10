#pragma once

#include "MWAllocObj.h"

#ifdef MW_PLATFORM_WIN32

#if !defined(__AFXWIN_H__) && !defined(_WINDOWS_)
#include <windows.h>
#endif

typedef HANDLE                      _thread;
typedef DWORD                       (WINAPI *_thread_proc)(LPVOID param);

#define DECLARE_THREAD_PROC(func)   DWORD WINAPI func(LPVOID param)
#define _THREAD_PROC_END()          return 0

#endif

namespace Myway
{

class MW_ENTRY Thread : public AllocObj
{
public:
    Thread();
    ~Thread();

    void Intialize(_thread_proc proc);
    void Shudown(bool wait);

    void Run();
    void Pause();

    bool IsPause() const;

protected:
    _thread     mThread;
    bool        mPaused;
};

}
