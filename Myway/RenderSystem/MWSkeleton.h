#pragma once

#include "MWBone.h"
#include "MWAnimation.h"
#include "MWResource.h"

namespace Myway
{

struct MW_ENTRY  joint : public AllocObj
{
    TString128 sName;
    Vec3 position;
    Quat orientation;
    Vec3 scale;
};

struct MW_ENTRY  hiberarchy : public AllocObj
{
    short parent;
    short child;
};

class MW_ENTRY Skeleton : public Resource, public RefObj
{
    DECLARE_ALLOC();
    DeclareRefObj();

public:
    Skeleton(const TString128 & sName);
    ~Skeleton();

    const TString128 & GetName();

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

    virtual void Load();
    virtual void Reload();
    virtual void Unload();

    virtual void LoadImp(DataStreamPtr stream);

public:
    TString128             mName;
    Array<joint>           mJoints;
    Array<hiberarchy>      mHiberarchys;
    Array<Animation*>      mAnimations;
};

DeclareSmartPtr(Skeleton);


}