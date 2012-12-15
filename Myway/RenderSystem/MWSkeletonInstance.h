#pragma once

#include "MWSkeleton.h"

namespace Myway
{

class MW_ENTRY SkeletonInstance
{
	DECLARE_ALLOC();

public:
    SkeletonInstance(Skeleton * skel);
    virtual ~SkeletonInstance();

    int GetBoneMatrix(Mat4 * forms);
    void UpdateBoneMatrix();
    void ResetBone();

	int GetRootBoneCount();
    Bone * GetRootBone(int i);

    int GetBoneCount();
    Bone * GetBone(const TString128 & sName);
    Bone * GetBone(short handle);

    Animation * GetAnimation(const TString128 & sAnimation);
    int GetAnimationCount();
    Animation * GetAnimation(int index);

protected:
    void _Initialize();
    void _Shutdown();
    Bone * CreateBone(const TString128 & sName);
    void _Dump(Bone * bn, const String & sText, const String & sSplit);

protected:
    Skeleton *     mSkeleton;
    Array<Bone*>   mBones;
	Array<Bone*>   mRoots;
};

}