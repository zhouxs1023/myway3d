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


void SkeletonAnimation::UpdateAnimation(SkeletonInstance * skel, AnimationController * controller)
{
    Bone * bone = skel->GetBone(mHandle);

	const MotionBlendInfo & blendInfo = controller->GetBlendInfo();

	if (blendInfo.BlendMask[mHandle])
		return ;

	float time = controller->GetPosition();

    int key1, key2;

    for (key2 = 0; key2 < mFrames.Size(); ++key2)
    {
        if (mFrames[key2].GetTime() >= time)
            break;
    }

    d_assert(key2 != mFrames.Size());

    key1 = (key2 != 0) ? (key2 - 1) : 0;

	Vec3 trans;
	Quat rotate;
	Vec3 scale;
    
    if (key1 != key2)
    {
        float elapsed = time - mFrames[key1].GetTime();
        float len = mFrames[key2].GetTime() - mFrames[key1].GetTime();
        float t = elapsed / len;

        Math::VecLerp(trans, mFrames[key1].GetTranslate(), mFrames[key2].GetTranslate(), t);
        Math::QuatSlerp(rotate, mFrames[key1].GetRotation(), mFrames[key2].GetRotation(), t);
		Math::VecLerp(scale, mFrames[key1].GetScale(), mFrames[key2].GetScale(), t);
	}
	else
	{
		trans = mFrames[key1].GetTranslate();
		rotate = mFrames[key1].GetRotation();
		scale = mFrames[key1].GetScale();
	}

	float w = controller->GetWeight();

	if (w > 0.99f)
	{
		bone->SetPosition(trans);
		bone->SetOrientation(rotate);
		bone->SetScale(scale);
	}
	else
	{
		Math::VecLerp(trans, bone->GetPosition(), trans, w);
		Math::QuatSlerp(rotate, bone->GetOrientation(), rotate, w);
		Math::VecLerp(scale, bone->GetScale(), scale, w);

		bone->SetPosition(trans);
		bone->SetOrientation(rotate);
		bone->SetScale(scale);
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

void Animation::_calcuLength()
{
	d_assert (mSkelAnims.Size() > 0);

	int count = mSkelAnims[0]->GetFrameCount();
	
	mLen = mSkelAnims[0]->GetKeyFrame(count - 1)->GetTime();

	d_assert (mLen > 0);
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

void Animation::_UpdateAnimation(SkeletonInstance * skel, AnimationController * controller)
{
	for (int i = 0; i < mSkelAnims.Size(); ++i)
	{
		mSkelAnims[i]->UpdateAnimation(skel, controller);
	}
}

//void Animation::_convertSkinAnim(SkeletonInstance * skel)
//{
//	for (int i = 0; i < mSkelAnims.Size(); ++i)
//	{
//		SkeletonAnimation * anim = mSkelAnims[i];
//
//		short boneId = anim->GetBoneHandle();
//		Bone * bone = skel->GetBone(boneId);
//
//		Quat bone_q = bone->GetInitOrientation().Inverse();
//		Vec3 bone_p = bone->GetInitPosition();
//		Vec3 bone_s = bone->GetInitScale();
//
//		for (int k = 0; k < anim->GetFrameCount(); ++k)
//		{
//			KeyFrame * kf = anim->GetKeyFrame(k);
//
//			Vec3 p = (kf->GetTranslate() - bone_p) * bone_q;
//			Quat q = bone_q * kf->GetRotation();
//			Vec3 s = kf->GetScale() / bone_s;
//
//			kf->SetTranslate(p);
//			kf->SetRotation(q);
//			kf->SetScale(s);
//		}
//	}
//}






AnimationController::AnimationController(Animation * anim)
	: mPos(0.0f)
	, mAnim(anim)
	, mWeightDelta(0)
	, mWeight(1)
	, mEnable(false)
{
}

AnimationController::~AnimationController()
{
}

void AnimationController::SetBlendInfo(const MotionBlendInfo & info)
{
	mBlendInfo = info;
}

const MotionBlendInfo & AnimationController::GetBlendInfo() const
{
	return mBlendInfo;
}

void AnimationController::SetPosition(float pos)
{
	mPos = pos;
}

float AnimationController::GetPosition() const
{
	return mPos;
}

Animation * AnimationController::GetAinmation()
{
	return mAnim;
}

void AnimationController::Play()
{
	if (!mEnable)
		mEnable = true;

	if (mBlendInfo.BlendInTime < 0.01f)
		mWeightDelta = 0;
	else
		mWeightDelta = 1 / mBlendInfo.BlendInTime;

	mWeight = 0;
}

bool AnimationController::_UpdateAnimation(float elapsedTime, SkeletonInstance * skel)
{
	float len = mAnim->GetLength();

	mPos += elapsedTime;

	if (mWeightDelta > 0)
	{
		mWeight += elapsedTime * mWeightDelta;

		if (mWeight >= 1)
		{
			mWeight = 1;
			mWeightDelta = 0;
		}
	}
	else
	{
		mWeight = 1;
	}


    if (mPos >= len)
    {
        if (mBlendInfo.Looped)
        {
            while (mPos > len)
                mPos -= len;

			return true;
        }
        else
        {
            return false;
        }
    }

    mAnim->_UpdateAnimation(skel, this);
}




/*
    AnimationSet
*/
AnimationSet::AnimationSet()
{
}

AnimationSet::~AnimationSet()
{
	for (int i = 0; i < mControllers.Size(); ++i)
		delete mControllers[i];

	mControllers.Clear();
}

bool AnimationSet::IsPlay(const char * anim)
{
	for (int i = 0; i < mControllers.Size(); ++i)
	{
		if (mControllers[i]->GetAinmation()->GetName() == anim)
			return true;
	}

	return false;
}

void AnimationSet::Play(AnimationController * controller)
{
	controller->Play();
	mControllers.PushBack(controller);
}

void AnimationSet::UpdateAnimation(float elapsedTime, SkeletonInstance * skel)
{
	for (int i = 0; i < mControllers.Size(); ++i)
	{
		mControllers[i]->_UpdateAnimation(elapsedTime, skel);
	}
}