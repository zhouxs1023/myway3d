#include "MWSkeleton.h"
#include "MWSkeletonManager.h"
#include "MWResourceManager.h"

using namespace Myway;

Skeleton::Skeleton()
{
	mRelative = false;
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


void Skeleton::Shutdown()
{
	mJoints.Clear();
	mHiberarchys.Clear();
}
