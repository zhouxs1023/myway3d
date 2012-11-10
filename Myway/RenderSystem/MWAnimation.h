#pragma once

#include "MWMath.h"

namespace Myway
{

class SkeletonInstance;

enum INTERPOLATION_TYPE
{
    INTERPOLATION_LINEAR,

    MW_ALIGN_ENUM(INTERPOLATION_TYPE)
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


/* :) SkeletonAnimation
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/
class MW_ENTRY SkeletonAnimation : public AllocObj
{
public:
    SkeletonAnimation(short handle);
    ~SkeletonAnimation();

    void UpdateAnimation(SkeletonInstance * skel, float time);
    KeyFrame * CreateKeyFrame();
    int GetFrameCount();
    KeyFrame * GetKeyFrame(int index);
    short GetBoneHandle();

    friend class Animation;

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

    void _UpdateAnimation(SkeletonInstance * skel, float time);

protected:
    TString128                  mName;
    float                       mLen;
    Array<SkeletonAnimation*>  mSkelAnims;
};


/* :) AnimationState
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/
class MW_ENTRY AnimationState : public AllocObj
{
public:
    AnimationState();
    ~AnimationState();

    void SetEnable(bool enable);
    void SetLoop(bool loop);
    void SetWeight(float weight);

    void AddTime(float time);
    void SetPosition(float pos);

    const TString128 & GetName() const;
    bool GetEnable() const;
    bool GetLoop() const;
    float GetWeight() const;
    float GetPosition() const;
    float GetLength() const;

    void _UpdateAnimation(SkeletonInstance * skel);
    Animation * GetAinmation();

    friend class AnimationSet;

protected:
    bool        mEnable;            
    bool        mLoop;
    float       mWeight;
    float       mPos;
    Animation * mAnim;
    AnimationSet * mParent;
};

/* :) AnimationSet
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/
class MW_ENTRY AnimationSet : public AllocObj
{
public:
    typedef List<AnimationState>                StateList;
    typedef List<AnimationState*>               ActiveStateList;

    typedef Visitor<StateList::Iterator>        StateVisitor;
    typedef Visitor<ActiveStateList::Iterator>  ActiveStateVisitor;

    friend class AnimationState;

public:
    AnimationSet();
    ~AnimationSet();

    AnimationState *    CreateState(Animation * anim);
    AnimationState *    GetState(const TString128 & name);

    bool                HasState(const TString128 & name);

    int                 GetStateCount();
    int                 GetActiveStateCount() const;

    StateVisitor        GetStates();
    ActiveStateVisitor  GetActiveStates();

protected:
    void _NotifyActive(AnimationState * anim, bool enable);
    void _RemoveActive(AnimationState * anim);

protected:
    StateList           mStates;
    ActiveStateList     mActives;
};

#include "MWAnimation.inl"

}
