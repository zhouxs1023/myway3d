#pragma once

#include "MWSkeletonLoader.h"

namespace Myway
{

class MW_ENTRY SkeletonManager
{
    DECLARE_SINGLETON (SkeletonManager);

    typedef HashMap<TString128, Skeleton *, TString128::hash_t> SkeletonMap;

public:
    SkeletonManager();
    ~SkeletonManager();

    SkeletonPtr Create(const TString128 & name);

    void        Destroy(const TString128 & name);
    void        Destroy(Skeleton * skel);

    SkeletonPtr Load(const TString128 & sName, const TString128 & sSource);
    SkeletonPtr Find(const TString128 & sName);

protected:
    SkeletonMap         mSkeletons;
};

}