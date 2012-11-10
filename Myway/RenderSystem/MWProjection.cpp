#include "MWProjection.h"
#include "MWNode.h"
#include "MWMaterialManager.h"

using namespace Myway;


DECLARE_GUID128_CONST(Projection::Guid, 'p', 'r', 'o', 'j', 'e', 'c', 't', 'i',
                                        'o', 'n', 0, 0, 0, 0, 0, 0);

Projection::Projection(const String & sName, const String & sMaterialName)
: mName(sName),
  mNear(0.1f),
  mFar(10000.0f),
  mFov(Math::PI_1_4),
  mAspect(1.3333333f),
  mWidth(1.0f),
  mHeight(1.0f),
  mProjType(PROJTYPE_ORTHO),
  mMatView(Mat4::Identity),
  mMatProj(Mat4::Identity),
  mMatViewProj(Mat4::Identity),
  mNeedUpdateView(TRUE),
  mNeedUpdateProj(TRUE)
{
    SetMaterial(sMaterialName);
}

Projection::~Projection()
{
}

const guid128 & Projection::GetGuid()
{
    return Guid;
}

const Mat4 & Projection::GetProjectionMatrix()
{
    _Update();
    return mMatViewProj;
}

void Projection::NotifyUpdate()
{
    mNeedUpdateView = TRUE;
}

void Projection::SetNearClip(float fNear)
{
    mNear = fNear;
    mNeedUpdateProj = TRUE;
}

void Projection::SetFarClip(float fFar)
{
    mFar = fFar;
    mNeedUpdateProj = TRUE;
}

void Projection::SetOrthoWidth(float fWidth)
{
    mWidth = fWidth;
    mNeedUpdateProj = TRUE;
}

void Projection::SetOrthoHeight(float fHeight)
{
    mHeight = fHeight;
    mNeedUpdateProj = TRUE;
}

void Projection::SetPerspectiveFov(float fFov)
{
    mFov = fFov;
    mNeedUpdateProj = TRUE;
}

void Projection::SetPerspectiveAspect(float fAspect)
{
    mAspect = fAspect;
    mNeedUpdateProj = TRUE;
}

const String & Projection::GetName() const
{
    return mName;
}

float Projection::GetNearClip() const
{
    return mNear;
}

float Projection::GetFarClip() const
{
    return mFar;
}

float Projection::GetOrthoWidth() const
{
    return mWidth;
}

float Projection::GetOrthoHeight() const
{
    return mHeight;
}

float Projection::GetPerspectiveFov() const
{
    return mFov;
}

float Projection::GetPerspectiveAspect() const
{
    return mAspect;
}

Material * Projection::GetMaterial()
{
    return mMaterial;
}


Projection::Visibility Projection::GetVisibility(const Sphere & sph)
{
    _Update();

    const Plane * planes = (const Plane *)&mFrustum;
    bool full = true;

    for (int i = 0; i < 6; ++i)
    {
        Plane::Side side = Math::PlaneSide(planes[i], sph);

        if (side == Plane::NEGATIVE)
            return NONE;
        else if (side == Plane::BOTH)
            full = FALSE;
    }

    return full ? FULL : PARTIAL;
}

Projection::Visibility Projection::GetVisibility(const Vec3 & point)
{
    _Update();

    const Plane * planes = (const Plane *)&mFrustum;

    for (int i = 0; i < 6; ++i)
    {
        Plane::Side side = Math::PlaneSide(planes[i], point);

        if (side == Plane::NEGATIVE)
            return NONE;
        else if (side == Plane::NONE)
            return FULL;
    }

    return FULL;
}

Projection::Visibility Projection::GetVisibility(const Aabb & box)
{
    _Update();

    const Plane * planes = (const Plane *)&mFrustum;
    Vec3 center = box.GetCenter();
    Vec3 half = box.GetHalfSize();
    bool full = true;

    for (int i = 0; i < 6; ++i)
    {
        Plane::Side side = Math::PlaneSide(planes[i], center, half);

        if (side == Plane::NEGATIVE)
            return NONE;
        else if (side == Plane::BOTH)
            full = FALSE;
    }

    return full ? FULL : PARTIAL;
}

bool Projection::IsVisible(const Aabb & box)
{
    Visibility v = GetVisibility(box);

    return v == FULL || v == PARTIAL;
}

bool Projection::IsVisible(const Sphere & sph)
{
    Visibility v = GetVisibility(sph);

    return v == FULL || v == PARTIAL;
}



void Projection::_Update()
{
    if (m_node->_NeedUpdate())
    {
        mNeedUpdateView = TRUE;
    }

    if (mNeedUpdateView)
    {
        Math::MatViewLH(mMatView, m_node->GetWorldPosition(), m_node->GetWorldOrientation());
    }

    if (mNeedUpdateProj)
    {
        if (mProjType == PROJTYPE_ORTHO)
        {
            Math::MatOrthoLH(mMatProj, mWidth, mHeight, mNear, mFar);
        }
        else if (mProjType == PROJTYPE_PROJECTION)
        {
            Math::MatPerspectiveFovLH(mMatProj, mFov, mAspect, mNear, mFar);
        }
    }

    if (mNeedUpdateView || mNeedUpdateProj)
    {
        Math::MatMultiply(mMatViewProj, mMatView, mMatProj);
        Math::FrustumFromMatrix(mFrustum, mMatViewProj);
    }

    mNeedUpdateView = FALSE;
    mNeedUpdateProj = FALSE;
}

void Projection::SetProjectionType(PROJECTION_TYPE type)
{
    mProjType = type;
    mNeedUpdateProj = TRUE;
}

PROJECTION_TYPE Projection::GetProjectionType() const
{
    return mProjType;
}
