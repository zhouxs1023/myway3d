#include "MWSkeletonManager.h"

using namespace Myway;

IMP_SLN(SkeletonManager);
SkeletonManager::SkeletonManager()
{
    INIT_SLN;
}

SkeletonManager::~SkeletonManager()
{
    SHUT_SLN;
    d_assert(mSkeletons.Size() == 0);
}

SkeletonPtr SkeletonManager::Load(const TString128 & name, const TString128 & source)
{
    SkeletonPtr sk = Find(name);

    if (sk == NULL)
    {
        sk = new Skeleton(name);
        mSkeletons.Insert(name, sk.c_ptr());
        sk->Load();
    }

    return sk;
}

SkeletonPtr SkeletonManager::Create(const TString128 & name)
{
    d_assert (Find(name).IsNull());

    Skeleton * skel = new Skeleton(name);
    mSkeletons.Insert(name, skel);

    return SkeletonPtr(skel);
}

void SkeletonManager::Destroy(const TString128 & name)
{
    SkeletonMap::Iterator whr = mSkeletons.Find(name);
    SkeletonMap::Iterator end = mSkeletons.End();

    d_assert (whr != end);

    delete whr->second;

    mSkeletons.Erase(whr);
}

void SkeletonManager::Destroy(Skeleton * skel)
{
   Destroy(skel->GetName());
}

SkeletonPtr SkeletonManager::Find(const TString128 & name)
{
    SkeletonMap::Iterator whr = mSkeletons.Find(name);
    SkeletonMap::Iterator end = mSkeletons.End();

    return whr != end ? SkeletonPtr(whr->second) : NULL;
}