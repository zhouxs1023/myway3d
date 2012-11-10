#pragma once

#include "MWTString.h"
#include "MWList.h"
#include "MWSingleton.h"

namespace Myway
{

/*
---------------------------------------------------------
    dynamic link lib
---------------------------------------------------------
*/
class MW_ENTRY Dll : public AllocObj
{
public:
    Dll(const TString128 & sName);
    ~Dll();

    const TString128 & GetName();
    void Load();
    void Unload();
    void * GetPorc(const char * func);

protected:
    TString128      mName;
    void*           mHandle;
};


/*
---------------------------------------------------------
    dynamic link lib manager.
---------------------------------------------------------
*/
class MW_ENTRY DllManager
{
    DECLARE_SINGLETON(DllManager);

public:
    DllManager();
    ~DllManager();

    Dll *   Load(const TString128 & name);
    void    Unload(Dll * dll);
    
protected:
    List<Dll*>     mDlls;
};

}