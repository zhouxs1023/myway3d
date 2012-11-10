#include "MWSkeleton.h"
#include "MWSkeletonManager.h"
#include "MWResourceManager.h"

using namespace Myway;

Skeleton::Skeleton(const TString128 & sName)
: mName(sName)
{
}

Skeleton::~Skeleton()
{
    Array<Animation*>::Iterator iter;
    Array<Animation*>::Iterator end;

    iter = mAnimations.Begin();
    end = mAnimations.End();

    while (iter != end)
    {
        delete *iter;

        ++iter;
    }
}

const TString128 & Skeleton::GetName()
{
    return mName;
}

joint * Skeleton::CreateJoint(const TString128 & sBoneName)
{
    debug_assert(!GetJoint(sBoneName),  "bone exist.");

    joint * bn = mJoints.Increase();

    bn->sName = sBoneName;
    bn->position = Vec3::Zero;
    bn->orientation = Quat::Identity;
    bn->scale = Vec3::Unit;

    return bn;
}

void Skeleton::SetupHiberarchy(short parent, short child)
{
    hiberarchy * bp = mHiberarchys.Increase();
    
    bp->parent = parent;
    bp->child = child;
}

Animation * Skeleton::CreateAnimation(const TString128 & sAnimation)
{
    debug_assert(!GetAnimation(sAnimation), "animation exist.");

    Animation * anim = new Animation(sAnimation);

    mAnimations.PushBack(anim);

    return anim;
}

joint * Skeleton::GetJoint(const TString128 & sBoneName)
{
    Array<joint>::Iterator iter;
    Array<joint>::Iterator end;

    iter = mJoints.Begin();
    end = mJoints.End();

    while (iter != end && (iter)->sName != sBoneName)
    {
        ++iter;
    }

    return iter != end ? &(*iter) : NULL;
}

int Skeleton::GetJointCount()
{
    return mJoints.Size();
}

joint * Skeleton::GetJoint(short index)
{
    assert(index < mJoints.Size());
    return &mJoints[index];
}

int Skeleton::GetHiberarchyCount()
{
    return mHiberarchys.Size();
}

hiberarchy * Skeleton::GetHiberarchy(int index)
{
    assert(index < mHiberarchys.Size());
    return &mHiberarchys[index];
}

Animation * Skeleton::GetAnimation(const TString128 & sAnimation)
{
    Array<Animation*>::Iterator iter;
    Array<Animation*>::Iterator end;

    iter = mAnimations.Begin();
    end = mAnimations.End();

    while (iter != end && (*iter)->GetName() != sAnimation)
    {
        ++iter;
    }

    return iter != end ? (*iter) : NULL;
}

int Skeleton::GetAnimationCount()
{
    return mAnimations.Size();
}

Animation * Skeleton::GetAnimation(int index)
{
    assert(index < mAnimations.Size());
    return mAnimations[index];
}

void Skeleton::DeleteSelf()
{
    SkeletonManager::Instance()->Destroy(this);
}





void Skeleton::Load()
{
    if (mLoadState == Resource::LOADED)
        return ;

    if (CanLoad())
        ResourceManager::Instance()->GetResourceLoader()->Load(this);
    else
        mLoadState = Resource::LOADED;

}

void Skeleton::Reload()
{
    if (!CanLoad())
        return ;

    if (mLoadState == Resource::LOADED)
        Unload();

    ResourceManager::Instance()->GetResourceLoader()->ForceLoad(this);
}

void Skeleton::Unload()
{
    if (!CanLoad())
        return ;

    Array<Animation*>::Iterator whr = mAnimations.Begin();
    Array<Animation*>::Iterator end = mAnimations.End();

    while (whr != end)
    {
        delete *whr;
        ++whr;
    }

    mAnimations.Clear();
    mJoints.Clear();
    mHiberarchys.Clear();
}

void Skeleton::LoadImp(DataStreamPtr stream)
{
    SkeletonLoader::Load(this, stream);
}
