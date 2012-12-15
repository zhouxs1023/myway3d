#include "MWSkeleton.h"
#include "MWSkeletonManager.h"
#include "MWResourceManager.h"

using namespace Myway;

Skeleton::Skeleton()
{
}

Skeleton::~Skeleton()
{
	Shutdown();
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

void Skeleton::Shutdown()
{
	for (int i = 0; i < mAnimations.Size(); ++i)
		delete mAnimations[i];

	mJoints.Clear();
	mHiberarchys.Clear();
	mAnimations.Clear();
}
