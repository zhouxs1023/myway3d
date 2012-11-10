#pragma once

#include "MWSkeleton.h"

namespace Myway
{

class MW_ENTRY SkeletonInstance : public AllocObj
{
public:
    SkeletonInstance(SkeletonPtr skel);
    virtual ~SkeletonInstance();

    void GetBoneMatrix(Mat4 * forms, int * count);
    void UpdateBoneMatrix();
    void ResetBone();

    Bone * GetRootBone();

    int GetBoneCount();
    Bone * GetBone(const TString128 & sName);
    Bone * GetBone(short handle);

    Animation * GetAnimation(const TString128 & sAnimation);
    int GetAnimationCount();
    Animation * GetAnimation(int index);
    const TString128 & GetSkeletonName();

protected:
    void _Initialize();
    void _Shutdown();
    Bone * CreateBone(const TString128 & sName);
    void _Dump(Bone * bn, const String & sText, const String & sSplit);

protected:
    SkeletonPtr     mSkeleton;
    Array<Bone*>   mBones;
    Bone *          mRoot;
};

}