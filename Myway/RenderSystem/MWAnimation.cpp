#include "MWAnimation.h"
#include "MWSkeletonInstance.h"

using namespace Myway;


/* :) KeyFrame
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/
KeyFrame::KeyFrame()
: mTime(0),
  mTrans(Vec3::Zero),
  mRotate(Quat::Identity),
  mScale(Vec3::Unit)
{
}

KeyFrame::~KeyFrame()
{
}

void KeyFrame::SetTime(float time)
{
    mTime = time;
}

void KeyFrame::SetTranslate(const Vec3 & v)
{
    mTrans = v;
}

void KeyFrame::SetRotation(const Quat & q)
{
    mRotate = q;
}

void KeyFrame::SetScale(const Vec3 & v)
{
    mScale = v;
}

void KeyFrame::SetTranslate(float x, float y, float z)
{
    mTrans.x = x;
    mTrans.y = y;
    mTrans.z = z;
}

void KeyFrame::SetRotation(float x, float y, float z, float w)
{
    mRotate.x = x;
    mRotate.y = y;
    mRotate.z = z;
    mRotate.w = w;
}

void KeyFrame::SetScale(float x, float y, float z)
{
    mScale.x = x;
    mScale.y = y;
    mScale.z = z;
}

float KeyFrame::GetTime() const
{
    return mTime;
}

const Vec3 & KeyFrame::GetTranslate() const
{
    return mTrans;
}

const Quat & KeyFrame::GetRotation() const
{
    return mRotate;
}

const Vec3 & KeyFrame::GetScale() const
{
    return mScale;
}


/* :) SkeletonAnimation
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/
SkeletonAnimation::SkeletonAnimation(short handle)
: mHandle(handle)
{
}

SkeletonAnimation::~SkeletonAnimation()
{
}


void SkeletonAnimation::UpdateAnimation(SkeletonInstance * skel, float time)
{
    Bone * bone = skel->GetBone(mHandle);

    int key1, key2;

    for (key2 = 0; key2 < mFrames.Size(); ++key2)
    {
        if (mFrames[key2].GetTime() >= time)
            break;
    }

    EXCEPTION_DEBUG(key2 != mFrames.Size(), "time failed.");
    key1 = (key2 != 0) ? (key2 - 1) : 0;
    
    if (key1 != key2)
    {
        float elapsed = time - mFrames[key1].GetTime();
        float len = mFrames[key2].GetTime() - mFrames[key1].GetTime();
        float t = elapsed / len;

        Vec3 trans;
        Quat rotate;
        Vec3 scale;

        Math::VecLerp(trans, mFrames[key1].GetTranslate(), mFrames[key2].GetTranslate(), t);
        Math::QuatSlerp(rotate, mFrames[key1].GetRotation(), mFrames[key2].GetRotation(), t);
        Math::VecLerp(scale, mFrames[key1].GetScale(), mFrames[key2].GetScale(), t);

        bone->Translate(trans, TS_LOCAL);
        bone->Rotate(rotate, TS_LOCAL);
        bone->Scale(scale);
    }
}

KeyFrame * SkeletonAnimation::CreateKeyFrame()
{
    return mFrames.Increase();
}

int SkeletonAnimation::GetFrameCount()
{
    return mFrames.Size();
}

KeyFrame * SkeletonAnimation::GetKeyFrame(int index)
{
    return &mFrames[index];
}

short SkeletonAnimation::GetBoneHandle()
{
    return mHandle;
}


/* :) NodeAnimation
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/
Animation::Animation(const TString128 & name)
: mName(name),
  mLen(0)
{
}

Animation::~Animation()
{
    Array<SkeletonAnimation*>::Iterator biter;
    Array<SkeletonAnimation*>::Iterator bend;

    biter = mSkelAnims.Begin();
    bend = mSkelAnims.End();

    while (biter != bend)
    {
        delete *biter;

        ++biter;
    }
}

SkeletonAnimation * Animation::CreateSkeletonAnimation(short handle)
{
    SkeletonAnimation * anim = new SkeletonAnimation(handle);

    mSkelAnims.PushBack(anim);

    return anim;
}

SkeletonAnimation * Animation::GetSkeletonAnimation(int index)
{
    assert(index < mSkelAnims.Size());
    return mSkelAnims[index];
}

int Animation::GetSkeletonAnimationCount()
{
    return mSkelAnims.Size();
}

void Animation::SetLength(float len)
{
    mLen = len;
}

const TString128 & Animation::GetName() const
{
    return mName;
}

float Animation::GetLength() const
{
    return mLen;
}

void Animation::_UpdateAnimation(SkeletonInstance * skel, float time)
{
    Array<SkeletonAnimation*>::Iterator iter;
    Array<SkeletonAnimation*>::Iterator end;

    iter = mSkelAnims.Begin();
    end = mSkelAnims.End();

    while (iter != end)
    {
        (*iter)->UpdateAnimation(skel, time);

        ++iter;
    }
}



void AnimationState::_UpdateAnimation(SkeletonInstance * skel)
{
    float len = GetLength();

    if (mPos > len)
    {
        if (mLoop)
        {
            while (mPos > len)
            {
                mPos -= len;
            }
        }
        else
        {
            SetEnable(false);
            return;
        }
    }

    mAnim->_UpdateAnimation(skel, mPos);
}