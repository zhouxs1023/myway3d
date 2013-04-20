#pragma once

#include "MWRenderDefine.h"

namespace Myway
{

class SkeletonInstance;

enum INTERPOLATION_TYPE
{
    INTERPOLATION_LINEAR,

    MW_ALIGN_ENUM(INTERPOLATION_TYPE)
};

struct MotionBlendInfo
{
	float BlendInTime;

	float Weight;
	float Speed;

	bool Looped;
	bool BlendMask[MAX_BLEND_MATRIX];

	MotionBlendInfo()
	{
		BlendInTime = 0.3f;

		Weight = 1.0f;
		Speed = 1.0f;

		Looped = true;

		ZeroMemory(BlendMask, sizeof(bool) * MAX_BLEND_MATRIX);
	}
};

/* :) KeyFrame
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/
class MW_ENTRY KeyFrame : public AllocObj
{
public:
    KeyFrame();
    ~KeyFrame();

    void SetTime(float time);

    void SetTranslate(const Vec3 & v);
    void SetRotation(const Quat & q);
    void SetScale(const Vec3 & v);

    void SetTranslate(float x, float y, float z);
    void SetRotation(float x, float y, float z, float w);
    void SetScale(float x, float y, float z);

    float GetTime() const;

    const Vec3 & GetTranslate() const;
    const Quat & GetRotation() const;
    const Vec3 & GetScale() const;

protected:
    float       mTime;
    Vec3        mTrans;
    Quat        mRotate;
    Vec3        mScale;
};


class AnimationController;

/* :) SkeletonAnimation
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/
class MW_ENTRY SkeletonAnimation : public AllocObj
{

public:
    SkeletonAnimation(short handle);
    ~SkeletonAnimation();

    void UpdateAnimation(SkeletonInstance * skel, AnimationController * controller);
    KeyFrame * CreateKeyFrame();
    int GetFrameCount();
    KeyFrame * GetKeyFrame(int index);
    short GetBoneHandle();

protected:
    short             mHandle;
    Array<KeyFrame>   mFrames;
};


/* :) Animation
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/
class MW_ENTRY Animation : public AllocObj
{
public:
    Animation(const TString128 & name);
    ~Animation();

    //Skeleton Animation
    SkeletonAnimation * CreateSkeletonAnimation(short handle);
    SkeletonAnimation * GetSkeletonAnimation(int index);
    int GetSkeletonAnimationCount();

    void SetLength(float len);

    const TString128 & GetName() const;
    float GetLength() const;

    void _UpdateAnimation(SkeletonInstance * skel, AnimationController * controller);

	void _calcuLength();

	//void _convertSkinAnim(SkeletonInstance * skel);

protected:
    TString128                  mName;
    float                       mLen;
    Array<SkeletonAnimation*>  mSkelAnims;
};


/* :) AnimationState
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/
class MW_ENTRY AnimationController : public AllocObj
{
public:
    AnimationController(Animation * anim);
    ~AnimationController();

	void SetBlendInfo(const MotionBlendInfo & info);
	const MotionBlendInfo & GetBlendInfo() const;

    void SetPosition(float pos);

    float GetPosition() const;
	Animation * GetAinmation();

	void Play();
    bool _UpdateAnimation(float elapsedTime, SkeletonInstance * skel);

	float GetWeight() { return mWeight; }

protected:
    float mPos;
	MotionBlendInfo mBlendInfo;
    Animation * mAnim;

	bool mEnable;
	float mWeightDelta;
	float mWeight;
};

/* :) AnimationSet
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/
class MW_ENTRY AnimationSet : public AllocObj
{
public:
    AnimationSet();
    ~AnimationSet();

	bool IsPlay(const char * anim);
	void Play(AnimationController * controller);

	void UpdateAnimation(float elapsedTime, SkeletonInstance * skel);

protected:
	Array<AnimationController*> mControllers;
};

#include "MWAnimation.inl"

}
