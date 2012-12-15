#pragma once

#include "MWBone.h"
#include "MWAnimation.h"
#include "MWResource.h"

namespace Myway
{

struct MW_ENTRY joint : public AllocObj
{
    TString128 sName;
    Vec3 position;
    Quat orientation;
    Vec3 scale;
};

struct MW_ENTRY hiberarchy : public AllocObj
{
    short parent;
    short child;
};

class MW_ENTRY Skeleton
{
    DECLARE_ALLOC();

public:
    Skeleton();
    ~Skeleton();

    joint * CreateJoint(const TString128 & sName);
    void SetupHiberarchy(short parent, short child);

    Animation * CreateAnimation(const TString128 & sAnimation);

    int GetJointCount();
    joint * GetJoint(const TString128 & sBoneName);
    joint * GetJoint(short handle);

    int GetHiberarchyCount();
    hiberarchy * GetHiberarchy(int index);

    Animation * GetAnimation(const TString128 & sAnimation);

    int GetAnimationCount();
    Animation * GetAnimation(int index);

	void Shutdown();

public:
    TString128             mName;
    Array<joint>           mJoints;
    Array<hiberarchy>      mHiberarchys;
    Array<Animation*>      mAnimations;
};

}