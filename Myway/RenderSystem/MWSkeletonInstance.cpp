#include "MWSkeletonInstance.h"

using namespace Myway;


SkeletonInstance::SkeletonInstance(Skeleton * skel)
{
    mSkeleton = skel;
    _Initialize();
}

SkeletonInstance::~SkeletonInstance()
{
    _Shutdown();
}

void SkeletonInstance::_Initialize()
{
    //create bones
    for (int i = 0; i < mSkeleton->GetJointCount(); ++i)
    {
        joint * data = mSkeleton->GetJoint(i);
        Bone * bone = CreateBone(data->sName);
        bone->SetPosition(data->position);
        bone->SetOrientation(data->orientation);
        bone->SetScale(data->scale);
    }

    //setup bone parent
    for (int i = 0; i < mSkeleton->GetHiberarchyCount(); ++i)
    {
        hiberarchy * bp = mSkeleton->GetHiberarchy(i);

		if (bp->parent != -1)
		{
			Bone * parent = GetBone(bp->parent);
			Bone * child = GetBone(bp->child);
			parent->AddChild(child);
		}
    }

    //setup root bone
	for (int i = 0; i < mBones.Size(); ++i)
	{
		Bone * bone = mBones[i];

		if (bone->_IsRoot())
			mRoots.PushBack(bone);
	}

	d_assert (mRoots.Size() > 0);

	for (int i = 0; i < mRoots.Size(); ++i)
	{
		mRoots[i]->_Intitalize();
	}
}

void SkeletonInstance::_Shutdown()
{
    Array<Bone*>::Iterator iter;
    Array<Bone*>::Iterator end;

    iter = mBones.Begin();
    end = mBones.End();

    while (iter != end)
    {
        delete *iter;

        ++iter;
    }

    mBones.Clear();
	mRoots.Clear();
}

void SkeletonInstance::_Dump(Bone * bn, const String & sText, const String & sSplit)
{
    const Vec3 & p = bn->GetPosition();
    const Quat & q = bn->GetOrientation();
    const Vec3 & s = bn->GetScale();

    debug_printf("%s%s,     p: %f %f %f,        o: %f %f %f %f,     s: %f %f %f\n",
                 sText.c_str(), bn->GetName().c_str(),
                 p.x, p.y, p.z,
                 q.x, q.y, q.z, q.w,
                 s.x, s.y, s.z);

    for (int i = 0; i < bn->GetNumChildren(); ++i)
    {
        _Dump(bn->GetChild(i), sText + sSplit, sSplit);
    }
}

int SkeletonInstance::GetBoneMatrix(Mat4 * forms)
{
    int index = 0;

    Array<Bone*>::Iterator iter;
    Array<Bone*>::Iterator end;

    iter = mBones.Begin();
    end = mBones.End();

    while (iter != end)
    {
        forms[index++] = (*iter)->GetBoneMatrix();

        ++iter;
    }

    return index;
}

void SkeletonInstance::UpdateBoneMatrix()
{
	for (int i = 0; i < mRoots.Size(); ++i)
	{
		mRoots[i]->_UpdateTransform();
	}
}

int SkeletonInstance::GetRootBoneCount()
{
	return mRoots.Size();
}

Bone * SkeletonInstance::GetRootBone(int i)
{
	return mRoots[i];
}

void SkeletonInstance::ResetBone()
{
    Array<Bone*>::Iterator iter;
    Array<Bone*>::Iterator end;

    iter = mBones.Begin();
    end = mBones.End();

    while (iter != end)
    {
        (*iter)->_Reset();
        ++iter;
    }
}

Bone * SkeletonInstance::CreateBone(const TString128 & sName)
{
    debug_assert(!GetBone(sName), "bone exist.");

    Bone * bone = new Bone(sName, mBones.Size());

    mBones.PushBack(bone);

    return bone;
}

int SkeletonInstance::GetBoneCount()
{
    return mBones.Size();
}

Bone * SkeletonInstance::GetBone(const TString128 & sName)
{
    Array<Bone*>::Iterator iter;
    Array<Bone*>::Iterator end;

    iter = mBones.Begin();
    end = mBones.End();

    while (iter != end && (*iter)->GetName() != sName)
    {
        ++iter;
    }

    return iter != end ? (*iter) : NULL;
}

Bone * SkeletonInstance::GetBone(short handle)
{
    assert(handle < mBones.Size());

    Array<Bone*>::Iterator iter;

    iter = mBones.Begin() + handle;

    return *iter;
}

Animation * SkeletonInstance::GetAnimation(const TString128 & sAnimation)
{
    return mSkeleton->GetAnimation(sAnimation);
}

int SkeletonInstance::GetAnimationCount()
{
    return mSkeleton->GetAnimationCount();
}

Animation * SkeletonInstance::GetAnimation(int index)
{
    return mSkeleton->GetAnimation(index);
}