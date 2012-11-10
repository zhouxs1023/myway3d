#pragma once

#include "MWDefine.h"

#ifdef MW_PLATFORM_WIN32

#if !defined(__AFXWIN_H__) && !defined(_WINDOWS_)
#include <windows.h>
#endif

typedef CRITICAL_SECTION _mutex;

#endif

#include "MWAllocObj.h"

namespace Myway
{

class MW_ENTRY Mutex : public AllocObj
{
public:
    Mutex();
    ~Mutex();

    void Lock();
    void Unlock();

protected:
    _mutex  mMutex;
};

#include "MWMutex.inl"

}