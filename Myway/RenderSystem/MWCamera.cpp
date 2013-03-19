#include "MWCamera.h"
#include "MWNode.h"

using namespace Myway;

Camera::Camera(const TString128 & name)
: Mover(name)
, mFov(Math::PI_1_3)
, mNear(1.0f)
, mFar(10000.0f)
, mAspect(1.3333333f)
, mOrthoWidth(300.0f)
, mOrthoHeight(200.0f)
, mNeedUpdate(true)
, mProjType(PROJTYPE_PROJECTION)
, mMirrorPlane(0, 1, 0, 0)
, mEnableMirror(false)
{
}

Camera::~Camera()
{
}

void Camera::NotifyUpdate()
{
    mNeedUpdate = true;
}

void Camera::SetFov(float fFov)
{
    mFov = fFov;
    mNeedUpdate = TRUE;
}

void Camera::SetNearClip(float fNear)
{
   mNear = fNear;
   mNeedUpdate = TRUE;
}

void Camera::SetFarClip(float fFar)
{
    mFar = fFar;
    mNeedUpdate = TRUE;
}

void Camera::SetAspect(float fAspect)
{
    mAspect = fAspect;
    mNeedUpdate = TRUE;
}

void Camera::SetPosition(const Vec3 & v)
{
    d_assert (mNode);

    mNode->SetPosition(v);
}

void Camera::SetOrientation(const Quat & q)
{
    d_assert (mNode);

    mNode->SetOrientation(q);
}

void Camera::SetDirection(const Vec3 & v)
{
    d_assert (mNode);
   
    Vec3 d = v.Normalize();
    Vec3 u = mNode->GetUp();
	Vec3 r = mNode->GetRight();

	float du = d.Dot(u);

	if (fabs(du) > 0.999f)
	{
		u = d.Cross(r);
		r = u.Cross(d);
	}
	else
	{
		r = u.Cross(d);
		u = d.Cross(r);
	}

    Quat q;

    q.FromAxis(r, u, d);

    SetOrientation(q);
}

void Camera::LookAt(const Vec3 & p)
{
    Vec3 dir = p - GetPosition();
    dir.NormalizeL();
    SetDirection(dir);
}

float Camera::GetFov() const
{
    return mFov;
}


float Camera::GetNearClip() const
{
    return mNear;
}

float Camera::GetFarClip() const
{
    return mFar;
}

float Camera::GetAspect() const
{
    return mAspect;
}

const Mat4 & Camera::GetViewMatrix()
{
    Update();
    return mMatView;
}

const Mat4 & Camera::GetProjMatrix()
{
    Update();
    return mMatProj;
}

const Mat4 & Camera::GetViewProjMatrix()
{
    Update();
    return mMatViewProj;
}

const Frustum & Camera::GetFrustum()
{
    Update();
    return mFrustum;
}

const Vec3 * Camera::GetCorner()
{
    Update();
    return mCorner;
}

const Vec3 * Camera::GetWorldCorner()
{
    Update();
    return mWorldCorner;
}

void Camera::GetWorldCorner(Vec3 * corner, float nearClip, float farClip)
{
	Vec3 nearPos = Vec3(0, 0, nearClip);
	Vec3 farPos = Vec3(0, 0, farClip);

	nearPos *= GetProjMatrix();
	farPos *= GetProjMatrix();

	Vec3 t_corner[8] = { Vec3(-1, 1, 0), Vec3(1, 1, 0), Vec3(-1, -1, 0), Vec3(1, -1, 0), 
						 Vec3(-1, 1, 1), Vec3(1, 1, 1), Vec3(-1, -1, 1), Vec3(1, -1, 1) };

	for (int i = 0; i < 4; ++i)
	{
		t_corner[i].z = nearPos.z;
		t_corner[i + 4].z = farPos.z;
	}

	Mat4 matInverseVP = GetViewProjMatrix().Inverse();

	for (int i = 0; i < 8; ++i)
	{
		corner[i] = t_corner[i] * matInverseVP;
	}
}


void Camera::Update()
{
    d_assert (mNode);

    if (mNode && mNode->_NeedUpdate())
        mNode->_UpdateTransform();

    if (mNeedUpdate)
    {
        const Vec3 & pos = mNode->GetWorldPosition();
        const Quat & ort = mNode->GetWorldOrientation();

        Math::MatViewLH(mMatView, pos, ort);

        if (mProjType == PROJTYPE_PROJECTION)
            Math::MatPerspectiveFovLH(mMatProj, mFov, mAspect, mNear, mFar);
        else if (mProjType == PROJTYPE_ORTHO)
            Math::MatOrthoLH(mMatProj, mOrthoWidth, mOrthoHeight, mNear, mFar);

        if (mEnableMirror)
        {
            Mat4 matMirror;
            matMirror.MakeReflect(mMirrorPlane);
            mMatView = matMirror * mMatView;
        }

        
        Vec3 corner[8] = { Vec3(-1, 1, 0), Vec3(1, 1, 0), Vec3(-1, -1, 0), Vec3(1, -1, 0), 
                           Vec3(-1, 1, 1), Vec3(1, 1, 1), Vec3(-1, -1, 1), Vec3(1, -1, 1) };
		Mat4 matInvProj = mMatProj.Inverse();
		Mat4 matInvView = mMatView.Inverse();

        for (int i = 0; i < 8; ++i)
            mCorner[i] = corner[i] * matInvProj;

        for (int i = 0; i < 8; ++i)
            mWorldCorner[i] = mCorner[i] * matInvView;

        Math::MatMultiply(mMatViewProj, mMatView, mMatProj);
        Math::FrustumFromMatrix(mFrustum, mMatViewProj);

        if (mEnableMirror)
            _makeClipProjMatrix();

        mNeedUpdate = false;
    }
}

void Camera::_makeClipProjMatrix()
{
    Mat4 matClipProj;
    Mat4 WorldToProjection;
    WorldToProjection = mMatView * mMatProj;

    Plane clip_plane = mMirrorPlane.Normalize();

    WorldToProjection = WorldToProjection.Inverse().Transpose();

    Vec4 clipPlane(clip_plane.a, clip_plane.b, clip_plane.c, clip_plane.d);
    Vec4 projClipPlane;
    // transform clip plane into projection space
    projClipPlane = clipPlane * WorldToProjection;

    matClipProj = Mat4::Identity;

    if (projClipPlane.w == 0)  // or less than a really small value
    {
        // plane is perpendicular to the near plane
        return;
    }

    float k_len = projClipPlane.x * projClipPlane.x + projClipPlane.y * projClipPlane.y +
                  projClipPlane.z * projClipPlane.z + projClipPlane.w * projClipPlane.w;

    k_len = Math::Sqrt(k_len);
    projClipPlane.x /= k_len; projClipPlane.y /= k_len;
    projClipPlane.z /= k_len; projClipPlane.w /= k_len;

    if (projClipPlane.w > 0)
    {
        projClipPlane = -projClipPlane;
        projClipPlane.w += 1;
    }
    // put projection space clip plane in Z column
    matClipProj[0][2] = projClipPlane.x;
    matClipProj[1][2] = projClipPlane.y;
    matClipProj[2][2] = projClipPlane.z;
    matClipProj[3][2] = projClipPlane.w;

    // multiply into projection matrix
    mMatProj = mMatProj * matClipProj;
}

Camera::Visibility Camera::GetVisibility(const Sphere & sph)
{
    Update();

    const Plane * planes = (const Plane *)&mFrustum;
    bool full = true;

    for (int i = 0; i < 6; ++i)
    {
        Plane::Side side = Math::PlaneSide(planes[i], sph);

        if (side == Plane::NEGATIVE)
            return VB_NONE;
        else if (side == Plane::BOTH)
            full = FALSE;
    }

    return full ? VB_FULL : VB_PARTIAL;
}

Camera::Visibility Camera::GetVisibility(const Vec3 & point)
{
    Update();

    const Plane * planes = (const Plane *)&mFrustum;

    for (int i = 0; i < 6; ++i)
    {
        Plane::Side side = Math::PlaneSide(planes[i], point);

        if (side == Plane::NEGATIVE)
            return VB_NONE;
        else if (side == Plane::NONE)
            return VB_FULL;
    }

    return VB_FULL;
}

Camera::Visibility Camera::GetVisibility(const Aabb & box)
{
    Update();

    const Plane * planes = (const Plane *)&mFrustum;
    Vec3 center = box.GetCenter();
    Vec3 half = box.GetHalfSize();
    bool full = true;

    for (int i = 0; i < 6; ++i)
    {
        Plane::Side side = Math::PlaneSide(planes[i], center, half);

        if (side == Plane::NEGATIVE)
            return VB_NONE;
        else if (side == Plane::BOTH)
            full = FALSE;
    }

    return full ? VB_FULL : VB_PARTIAL;
}

bool Camera::IsVisible(const Aabb & box)
{
    Visibility v = GetVisibility(box);

    return v == VB_FULL || v == VB_PARTIAL;
}

bool Camera::IsVisible(const Sphere & sph)
{
    Visibility v = GetVisibility(sph);

    return v == VB_FULL || v == VB_PARTIAL;
}


Ray Camera::GetViewportRay(float x, float y)
{
    Ray result;

    x = Math::Clamp(x, 0.0f, 1.0f);
    y = Math::Clamp(y, 0.0f, 1.0f);

    Mat4 matInvViewProj;
    Math::MatInverse(matInvViewProj, GetViewProjMatrix());

    x = x * 2.0f - 1.0f;
    y = y * -2.0f + 1.0f;

    Vec3 p1(x, y, 0.0f);
    Vec3 p2(x, y, 0.5f);

    Math::VecTransform(p1, p1, matInvViewProj);
    Math::VecTransform(p2, p2, matInvViewProj);

    result.origin = p1;
    result.direction = p2 - p1;

    Math::VecNormalize(result.direction, result.direction);

    return result;
}

void Camera::SetOrthoWidth(float width)
{
    mOrthoWidth = width;
    mNeedUpdate = true;
}

void Camera::SetOrthoHeight(float height)
{
    mOrthoHeight = height;
    mNeedUpdate = true;
}

float Camera::GetOrthoWidth() const
{
    return mOrthoWidth;
}

float Camera::GetOrthoHeight() const
{
    return mOrthoHeight;
}

void Camera::SetProjectionType(PROJECTION_TYPE type)
{
    mProjType = type;
    mNeedUpdate = true;
}

PROJECTION_TYPE Camera::GetProjectionType() const
{
    return mProjType;
}


void Camera::SetMirrorPlane(const Plane & plane)
{
    mMirrorPlane = plane;
    mNeedUpdate = true;
}

const Plane & Camera::GetMirrorPlane()
{
    return mMirrorPlane;
}

void Camera::EnableMirror(bool b)
{
    mEnableMirror = b;
    mNeedUpdate = true;
}

bool Camera::IsEnableMirror()
{
    return mEnableMirror;
}
