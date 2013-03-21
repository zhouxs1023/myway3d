#pragma once

#include "MWMover.h"

namespace Myway
{

class MW_ENTRY Camera : public Mover
{
    DECLARE_ALLOC();

public:
    enum Visibility
    {
        VB_NONE,
        
        VB_FULL,
        VB_PARTIAL,

        MW_ALIGN_ENUM(Visibility)
    };

public:
                    Camera(const TString128 & name);
                    ~Camera();

    void            NotifyUpdate();

    void            SetFov(float fFov);
    void            SetNearClip(float fNear);
    void            SetFarClip(float fFar);
    void            SetAspect(float fAsecpt);

    void            SetPosition(const Vec3 & p);
    void            SetOrientation(const Quat & q);
    void            SetDirection(const Vec3 & d);
    void            LookAt(const Vec3 & p);

    float           GetFov() const;
    float           GetNearClip() const;
    float           GetFarClip() const;
    float           GetAspect() const;

    const Mat4 &    GetViewMatrix();
    const Mat4 &    GetProjMatrix();
    const Mat4 &    GetViewProjMatrix();
    const Frustum & GetFrustum();
    const Vec3 *    GetCorner();
    const Vec3 *    GetWorldCorner();
	void			GetWorldCorner(Vec3 * corner, float nearClip, float farClip);

    Visibility      GetVisibility(const Aabb & box);
    Visibility      GetVisibility(const Sphere & sph);
    Visibility      GetVisibility(const Vec3 & point);

    bool            IsVisible(const Aabb & box);
    bool            IsVisible(const Sphere & sph);

    Ray             GetViewportRay(float x, float y);

    void            SetOrthoWidth(float width);
    void            SetOrthoHeight(float height);
    float           GetOrthoWidth() const;
    float           GetOrthoHeight() const;

    void            SetProjectionType(PROJECTION_TYPE type);
    PROJECTION_TYPE GetProjectionType() const;

    void            SetMirrorPlane(const Plane & plane);
    const Plane &   GetMirrorPlane();
    void            EnableMirror(bool b);
    bool            IsEnableMirror();

protected:
    void            Update();
    void            _makeClipProjMatrix();

protected:
    float mFov;
    float mNear;
    float mFar;
    float mAspect;

    float mOrthoWidth;
    float mOrthoHeight;

    Mat4 mMatView;
    Mat4 mMatProj;
    Mat4 mMatViewProj;

    Frustum mFrustum;
    Vec3 mCorner[8];
    Vec3 mWorldCorner[8];

    bool mNeedUpdate;

    PROJECTION_TYPE mProjType;

    Plane mMirrorPlane;
};

}