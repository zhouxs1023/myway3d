#include "MWBone.h"

using namespace Myway;

/* :) Bone
---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
*/

Bone::Bone(const TString128 & sName, short handle)
: mName(sName),
  mHandle(handle),
  mPos(Vec3::Zero),
  mOrientation(Quat::Identity),
  mScale(Vec3::Unit),
  mAABB(MAX_FLOAT, MAX_FLOAT, MAX_FLOAT, MIN_FLOAT, MIN_FLOAT, MIN_FLOAT),
  mSphere(0, 0, 0, 0),
  mNeedUpdate(TRUE),
  mParent(NULL),
  mInitPos(Vec3::Zero),
  mInitOrient(Quat::Identity),
  mInitScale(Vec3::Unit),
  mMatCombine(Mat4::Identity),
  mMatInvCombine(Mat4::Identity),
  mMatBone(Mat4::Identity),
  mMatWorld(Mat4::Identity)
{
}

Bone::~Bone()
{
}

const TString128 & Bone::GetName() const
{
    return mName;
}

void Bone::SetPosition(const Vec3 & v)
{
    mPos = v;
    mNeedUpdate = TRUE;
}

void Bone::SetOrientation(const Quat & q)
{
    mOrientation = q;
    mNeedUpdate = TRUE;
}

void Bone::SetScale(const Vec3 & v)
{
    mScale = v;
    mNeedUpdate = TRUE;
}

void Bone::SetPosition(float x, float y, float z)
{
    mPos.x = x;
    mPos.y = y;
    mPos.z = z;
    mNeedUpdate = TRUE;
}

void Bone::SetOrientation(float x, float y, float z, float w)
{
    mOrientation.x = x;
    mOrientation.y = y;
    mOrientation.z = z;
    mOrientation.w = w;
    mNeedUpdate = TRUE;
}

void Bone::SetScale(float x, float y, float z)
{
    mScale.x = x;
    mScale.y = y;
    mScale.z = z;
    mNeedUpdate = TRUE;
}

void Bone::Translate(const Vec3 & v, TRANSFORM_TYPE ts)
{
    switch (ts)
    {
    case TS_WORLD:
        {
             mPos += v;
        }
        break;

    case TS_PARENT:
        {
            mPos += v;
        }
        break;

    case TS_LOCAL:
        {
            Vec3 dir;
            Math::QuatAxisZ(dir, mOrientation);
            mPos += dir * v;
        }
        break;
    }

    mNeedUpdate = TRUE;
}

void Bone::Rotate(const Quat & q, TRANSFORM_TYPE ts)
{
    switch (ts)
    {
    case TS_WORLD:
        {
            mOrientation = q * mOrientation;
        }
        break;

    case TS_PARENT:
        {
            mOrientation = q * mOrientation;
        }
        break;

    case TS_LOCAL:
        {
            mOrientation = mOrientation * q;
        }
        break;
    }

    mNeedUpdate = TRUE;
}

void Bone::Scale(const Vec3 & v)
{
    mScale.x *= v.x;
    mScale.y *= v.y;
    mScale.z *= v.z;
    mNeedUpdate = TRUE;
}

void Bone::Translate(float x, float y, float z, TRANSFORM_TYPE ts)
{
   Translate(Vec3(x, y, z), ts);
}

void Bone::Rotate(float x, float y, float z, float w, TRANSFORM_TYPE ts)
{
    Rotate(Quat(x, y, z, w), ts);
}

void Bone::Scale(float x, float y, float z)
{
    mScale.x *= x;
    mScale.y *= y;
    mScale.z *= z;
    mNeedUpdate = TRUE;
}

void Bone::Pitch(float x, TRANSFORM_TYPE ts)
{
    Quat q;
    Math::QuatFromAxis(q, Vec3::UnitX, x);
    Rotate(q, ts);
}

void Bone::Yaw(float x, TRANSFORM_TYPE ts)
{
    Quat q;
    Math::QuatFromAxis(q, Vec3::UnitY, x);
    Rotate(q, ts);
}

void Bone::Roll(float x, TRANSFORM_TYPE ts)
{
    Quat q;
    Math::QuatFromAxis(q, Vec3::UnitZ, x);
    Rotate(q, ts);
}

void Bone::Move(float x)
{
    Vec3 dir;
    Math::QuatAxisZ(dir, mOrientation);
    mPos += dir * x;
    mNeedUpdate = TRUE;
}

void Bone::Fly(float x)
{
    Vec3 up;
    Math::QuatAxisY(up, mOrientation);
    mPos += up * x;
    mNeedUpdate = TRUE;
}

void Bone::Right(float x)
{
    Vec3 right;
    Math::QuatAxisX(right, mOrientation);
    mPos += right * x;
    mNeedUpdate = TRUE;
}

void Bone::SetVisible(bool bVisible)
{
    debug_assert(0, "bone can't set visible.");
}

bool Bone::IsVisible() const
{
    return TRUE;
}

const Vec3 & Bone::GetPosition() const
{
    return mPos;
}

const Quat & Bone::GetOrientation() const
{
    return mOrientation;
}

const Vec3 & Bone::GetScale() const
{
    return mScale;
}

Vec3 Bone::GetDirection() const
{
    Vec3 dir;
    Math::QuatAxisZ(dir, mOrientation);
    return dir;
}

Vec3 Bone::GetRight() const
{
    Vec3 right;
    Math::QuatAxisX(right, mOrientation);
    return right;
}

Vec3 Bone::GetUp() const
{
    Vec3 up;
    Math::QuatAxisY(up, mOrientation);
    return up;
}

const Vec3 & Bone::GetWorldPosition()
{
    return mPos;
}

const Quat & Bone::GetWorldOrientation()
{
    return mOrientation;
}

const Vec3 & Bone::GetWorldScale()
{
    return mScale;
}

const Mat4 & Bone::GetWorldMatrix()
{
    if (mNeedUpdate)
        _UpdateTransform();

    return mMatWorld;
}

void Bone::GetTransformMatrix(Mat4 & matForm, const Mat4 & matViewProj)
{
    Math::MatMultiply(matForm, GetWorldMatrix(), matViewProj);
}

const Aabb & Bone::GetWorldAabb()
{
    if (mNeedUpdate)
        _UpdateTransform();
    return mAABB;
}

const Sphere & Bone::GetWorldBoundingSphere()
{
    if (mNeedUpdate)
        _UpdateTransform();
    return mSphere;
}

void Bone::Attach(Mover * geo)
{
    d_assert(0 && "Bone can't attach object.");
}

void Bone::Detach(Mover * geo)
{
}

void Bone::DetachAll()
{
}

void Bone::NotifyCamera(Camera * cam)
{
}

bool Bone::_NeedUpdate() const
{
    return mNeedUpdate;
}

void Bone::RayTracing(const Ray & ray, List<Mover*> & geos, const guid128 & id, int mark)
{
}

void Bone::_UpdateTransform()
{
    //transform local
    Math::MatTransform(mMatLocal, mPos, mOrientation, mScale);

    if (mParent)
    {
        mMatWorld = mMatLocal * mParent->mMatWorld;
    }
    else
        mMatWorld = mMatLocal;

    mMatBone = mMatInvCombine * mMatWorld;

    mNeedUpdate = FALSE;

    for (int i = 0; i < mChildren.Size(); ++i)
    {
        mChildren[i]->_UpdateTransform();
    }
}

void Bone::_Intitalize()
{
    UpdateCombine(Mat4::Identity);
}

void Bone::_Reset()
{
    mPos = mInitPos;
    mOrientation = mInitOrient;
    mScale = mInitScale;
    mNeedUpdate = TRUE;
}

const Mat4 & Bone::GetBoneMatrix()
{
    if (_NeedUpdate())
        _UpdateTransform();

    return mMatBone;
}

const Mat4 & Bone::GetCombineMatrix()
{
    return mMatCombine;
}

const Mat4 & Bone::GetInvCombineMatrix()
{
    return mMatInvCombine;
}


void Bone::UpdateCombine(const Mat4 & mat)
{
    mInitPos = mPos;
    mInitOrient = mOrientation;
    mInitScale = mScale;
    Math::MatTransform(mMatLocal, mPos, mOrientation, mScale);
    Math::MatMultiply(mMatCombine, mMatLocal, mat);
    Math::MatInverse(mMatInvCombine, mMatCombine);

    for (int i = 0; i < mChildren.Size(); ++i)
    {
        mChildren[i]->UpdateCombine(mMatCombine);
    }
}

int Bone::GetNumChildren() const
{
    return mChildren.Size();
}

Bone * Bone::GetChild(int index) const
{
    return mChildren[index];
}

void Bone::AddChild(Bone * pNode)
{
    Bone * pBone = static_cast<Bone*>(pNode);
    pBone->mParent = this;
    mChildren.PushBack(pBone);
}

Bone * Bone::GetParent()
{
    return mParent;
}

bool Bone::_IsRoot() const
{
    return !mParent;
}

short Bone::GetHandle()
{
    return mHandle;
}


/* :) Bone Point
---------------------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------------------
*/
//const TString128 BonePoint::TYPE("Bone Point");
//
//BonePoint::BonePoint()
//: mPos(Vec3::Zero),
//  mOrientation(Quat::Identity),
//  mScale(Vec3::Unit),
//  mAABB(MAX_FLOAT, MAX_FLOAT, MAX_FLOAT, MIN_FLOAT, MIN_FLOAT, MIN_FLOAT),
//  mSphere(0, 0, 0, 0),
//  mNeedUpdate(TRUE),
//  mParent(NULL)
//{
//}
//
//BonePoint::~BonePoint()
//{
//}
//
//const TString128 & BonePoint::GetType() const
//{
//    return TYPE;
//}
//
//void BonePoint::SetPosition(const Vec3 & v)
//{
//    mPos = v;
//    mNeedUpdate = TRUE;
//}
//
//void BonePoint::SetOrientation(const Quat & q)
//{
//    mOrientation = q;
//    mNeedUpdate = TRUE;
//}
//
//void BonePoint::SetScale(const Vec3 & v)
//{
//    mScale = v;
//    mNeedUpdate = TRUE;
//}
//
//void BonePoint::SetPosition(float x, float y, float z)
//{
//    mPos.x = x;
//    mPos.y = y;
//    mPos.z = z;
//    mNeedUpdate = TRUE;
//}
//
//void BonePoint::SetOrientation(float x, float y, float z, float w)
//{
//    mOrientation.x = x;
//    mOrientation.y = y;
//    mOrientation.z = z;
//    mOrientation.w = w;
//    mNeedUpdate = TRUE;
//}
//
//void BonePoint::SetScale(float x, float y, float z)
//{
//    mScale.x = x;
//    mScale.y = y;
//    mScale.z = z;
//    mNeedUpdate = TRUE;
//}
//
//void BonePoint::Translate(const Vec3 & v, TRANSFORM_TYPE ts)
//{
//    switch (ts)
//    {
//    case TS_WORLD:
//        {
//            mPos += v;
//        }
//        break;
//
//    case TS_PARENT:
//        {
//            mPos += v;
//        }
//        break;
//
//    case TS_LOCAL:
//        {
//            Vec3 dir;
//            Math::QuatDirection(dir, mOrientation);
//            mPos += dir * v;
//        }
//        break;
//    }
//
//    mNeedUpdate = TRUE;
//}
//
//void BonePoint::Rotate(const Quat & q, TRANSFORM_TYPE ts)
//{
//    switch (ts)
//    {
//    case TS_WORLD:
//        {
//            mOrientation = q * mOrientation;
//        }
//        break;
//
//    case TS_PARENT:
//        {
//            mOrientation = q * mOrientation;
//        }
//        break;
//
//    case TS_LOCAL:
//        {
//            mOrientation = mOrientation * q;
//        }
//        break;
//    }
//
//    mNeedUpdate = TRUE;
//}
//
//void BonePoint::Scale(const Vec3 & v)
//{
//    mScale.x *= v.x;
//    mScale.y *= v.y;
//    mScale.z *= v.z;
//    mNeedUpdate = TRUE;
//}
//
//void BonePoint::Translate(float x, float y, float z, TRANSFORM_TYPE ts)
//{
//    Translate(Vec3(x, y, z), ts);
//}
//
//void BonePoint::Rotate(float x, float y, float z, float w, TRANSFORM_TYPE ts)
//{
//    Rotate(Quat(x, y, z, w), ts);
//}
//
//void BonePoint::Scale(float x, float y, float z)
//{
//    mScale.x *= x;
//    mScale.y *= y;
//    mScale.z *= z;
//    mNeedUpdate = TRUE;
//}
//
//void BonePoint::Pitch(float x, TRANSFORM_TYPE ts)
//{
//    Quat q;
//    Math::QuatFromAxis(q, Vec3::UnitX, x);
//    Rotate(q, ts);
//}
//
//void BonePoint::Yaw(float x, TRANSFORM_TYPE ts)
//{
//    Quat q;
//    Math::QuatFromAxis(q, Vec3::UnitY, x);
//    Rotate(q, ts);
//}
//
//void BonePoint::Roll(float x, TRANSFORM_TYPE ts)
//{
//    Quat q;
//    Math::QuatFromAxis(q, Vec3::UnitZ, x);
//    Rotate(q, ts);
//}
//
//void BonePoint::Move(float x)
//{
//    Vec3 dir;
//    Math::QuatDirection(dir, mOrientation);
//    mPos += dir * x;
//}
//
//void BonePoint::Fly(float x)
//{
//    Vec3 up;
//    Math::QuatUp(up, mOrientation);
//    mPos += up * x;
//}
//
//void BonePoint::Right(float x)
//{
//    Vec3 right;
//    Math::QuatRight(right, mOrientation);
//    mPos += right * x;
//}
//
//void BonePoint::SetVisible(bool bVisible)
//{
//    mVisible = TRUE;
//}
//
//bool BonePoint::IsVisible() const
//{
//    return mVisible;
//}
//
//const Vec3 & BonePoint::GetPosition() const
//{
//    return mPos;
//}
//
//const Quat & BonePoint::GetOrientation() const
//{
//    return mOrientation;
//}
//
//const Vec3 & BonePoint::GetScale() const
//{
//    return mScale;
//}
//
//Vec3 BonePoint::GetDirection()
//{
//    Vec3 dir;
//    Math::QuatDirection(dir, mOrientation);
//    return dir;
//}
//
//Vec3 BonePoint::GetRight()
//{
//    Vec3 right;
//    Math::QuatRight(right, mOrientation);
//    return right;
//}
//
//Vec3 BonePoint::GetUp()
//{
//    Vec3 up;
//    Math::QuatUp(up, mOrientation);
//    return up;
//}
//
//const Mat4 & BonePoint::GetWorldMatrix()
//{
//    Update();
//    return mMatWorld;
//}
//
//void BonePoint::GetTransformMatrix(Mat4 & matForm, const Mat4 & matViewProj)
//{
//    Math::MatMultiply(matForm, GetWorldMatrix(), matViewProj);
//}
//
//const Aabb & BonePoint::GetWorldAxisAlignedBoundingBox()
//{
//    Update();
//    return mAABB;
//}
//
//const Sphere & BonePoint::GetWorldBoundingSphere()
//{
//    Update();
//    return mSphere;
//}
//
//void BonePoint::AttachObject(Moveable * pMoveable)
//{
//    Update();
//    const Aabb * AABB;
//    const Sphere * SBB;
//
//    AABB = pMoveable->GetAxisAlignedBoundingBox();
//    SBB = pMoveable->GetBoundingSphere();
//
//    Vec3 vMin, vMax, vCenter;
//    Math::VecTransform(vMin, AABB->vMin, mMatWorld);
//    Math::VecTransform(vMax, AABB->vMax, mMatWorld);
//    Math::VecTransform(vCenter, SBB->vCenter, mMatWorld);
//
//    //update aabb box
//    if (mAABB.vMin.x > vMin.x)
//        mAABB.vMin.x = vMin.x;
//
//    if (mAABB.vMin.y > vMin.y)
//        mAABB.vMin.y = vMin.y;
//
//    if (mAABB.vMin.z > vMin.z)
//        mAABB.vMin.z = vMin.z;
//
//    if (mAABB.vMax.x < vMax.x)
//        mAABB.vMax.x = vMax.x;
//
//    if (mAABB.vMax.y < vMax.y)
//        mAABB.vMax.y = vMax.y;
//
//    if (mAABB.vMax.z < vMax.z)
//        mAABB.vMax.z = vMax.z;
//
//    //update sphere
//    mSphere.fRadius = mSphere.fRadius + SBB->fRadius + Math::VecLength(vCenter - mSphere.vCenter);
//    mSphere.vCenter = (mSphere.vCenter + vCenter) / 2.0f;
//
//    mMoveables.PushBack(pMoveable);
//    pMoveable->Attach(this);
//}
//
//void BonePoint::AddRenderQueue(RenderQueue * RQ)
//{
//    Array<Moveable*>::Iterator ROQIter;
//    ROQIter = mMoveables.Begin();
//    while (ROQIter != mMoveables.End())
//    {
//        (*ROQIter)->AddRenderQueue(RQ);
//        ++ROQIter;
//    }
//}
//
//void BonePoint::Update()
//{
//    EXCEPTION_DEBUG(mParent, "bone point parent is null.");
//
//    //update locate
//    if (mNeedUpdate)
//    {
//        Mat4 MatTranslate, matRotation, matScale;
//        Math::MatTranslate(MatTranslate, mPos.x, mPos.y, mPos.z);
//        Math::QuatToMatrix(matRotation, mOrientation);
//        Math::MatScale(matScale, mScale.x, mScale.y, mScale.z);
//        Math::MatMultiply(mMatLocal, matRotation, MatTranslate);
//        Math::MatMultiply(mMatLocal, mMatWorld, matScale);
//    }
//
//    //update world.
//    if (mNeedUpdate || mParent->_NeedUpdate())
//    {
//        Math::MatMultiply(mMatWorld, mParent->GetWorldMatrix(), mMatLocal);
//
//        //update aabb box
//        Vec3 vMin, vMax;
//        Math::VecTransform(vMin, mAABB.vMin, mMatWorld);
//        Math::VecTransform(vMax, mAABB.vMax, mMatWorld);
//
//        if (mAABB.vMin.x < vMin.x)
//            mAABB.vMin.x = vMin.x;
//
//        if (mAABB.vMin.y < vMin.y)
//            mAABB.vMin.y = vMin.y;
//
//        if (mAABB.vMin.z < vMin.z)
//            mAABB.vMin.z = vMin.z;
//
//        if (mAABB.vMax.x > vMax.x)
//            mAABB.vMax.x = vMax.x;
//
//        if (mAABB.vMax.y > vMax.y)
//            mAABB.vMax.y = vMax.y;
//
//        if (mAABB.vMax.z > vMax.z)
//            mAABB.vMax.z = vMax.z;
//
//        //update sphere
//        Math::VecTransform(mSphere.vCenter, mSphere.vCenter, mMatWorld);
//        mNeedUpdate = FALSE;
//    }
//}