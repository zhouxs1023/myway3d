#include "MWDynamicLinkLib.h"
#include "MWException.h"
#include "MWMemory.h"
#include "MWLogSystem.h"

#ifdef MW_PLATFORM_WIN32
#include <windows.h>
#else
#error "not support other platform"
#endif

using namespace Myway;

#ifdef MW_PLATFORM_WIN32

#define __DLL_INSTANCE          HMODULE
#define __DLL_LOAD(n)           (void*)LoadLibraryEx(n, NULL, LOAD_WITH_ALTERED_SEARCH_PATH)
#define __DLL_GETPORC(h, n)     GetProcAddress((HMODULE)h, n)
#define __DLL_UNLOAD(h)         FreeLibrary((HMODULE)h)

#else

#error "not support other platform"

#endif


/*
---------------------------------------------------------
    dynamic link lib.
---------------------------------------------------------
*/
Dll::Dll(const TString128 & name)
: mName(name),
  mHandle(NULL)
{
}

Dll::~Dll()
{
}

const TString128 & Dll::GetName()
{
    return mName;
}

void Dll::Load()
{
    mHandle = __DLL_LOAD(mName.c_str());

    d_assert (mHandle != NULL);
    
    if (!mHandle)
        LOG_PRINT_FORMAT("Load plugin '%s' failed.\n", mName.c_str());
}

void Dll::Unload()
{
    if (mHandle)
        __DLL_UNLOAD(mHandle);
}

void * Dll::GetPorc(const char * func)
{
    return __DLL_GETPORC(mHandle, func);
}



/*
---------------------------------------------------------
    dynamic link lib manager.
---------------------------------------------------------
*/
IMP_SLN(DllManager);
DllManager::DllManager()
{
    INIT_SLN;
}

DllManager::~DllManager()
{
    SHUT_SLN;

    List<Dll*>::Iterator whr = mDlls.Begin();
    List<Dll*>::Iterator end = mDlls.End();

    while (whr != end)
    {
        delete (*whr);
        ++whr;
    }
}

Dll * DllManager::Load(const TString128 & name)
{
    Dll * dll = new Dll(name);
    dll->Load();
    mDlls.PushBack(dll);
    return dll;
}

void DllManager::Unload(Dll * dll)
{
    List<Dll*>::Iterator whr = mDlls.Begin();
    List<Dll*>::Iterator end = mDlls.End();

    while (whr != end && (*whr) != dll)
    {
        ++whr;
    }

    d_assert (whr != end);

    (*whr)->Unload();
    delete (*whr);
    mDlls.Erase(whr);
}