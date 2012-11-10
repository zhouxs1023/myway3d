#pragma once

#include "MWMover.h"
#include "MWMaterial.h"

namespace Myway 
{

class MW_ENTRY Projection : public Mover
{
public:
    static const guid128 Guid;

    enum Visibility
    {
        NONE,

        FULL,
        PARTIAL,

        MW_ALIGN_ENUM(Visibility)
    };

public:
    Projection(const String & sName, const String & sMaterialName);
    virtual ~Projection();

    const guid128 &                 GetGuid();

    virtual const Mat4 &            GetProjectionMatrix();

    virtual void                    NotifyUpdate();

    virtual void                    SetNearClip(float fNear);
    virtual void                    SetFarClip(float fFar);

    void                            SetOrthoWidth(float fWidth);
    void                            SetOrthoHeight(float fHeight);

    void                            SetPerspectiveFov(float fFov);
    void                            SetPerspectiveAspect(float fAspect);

    const String &                  GetName() const;

    float                           GetNearClip() const;
    float                           GetFarClip() const;

    float                           GetOrthoWidth() const;
    float                           GetOrthoHeight() const;

    float                           GetPerspectiveFov() const;
    float                           GetPerspectiveAspect() const;

    Material *                      GetMaterial();

    Visibility                      GetVisibility(const Aabb & box);
    Visibility                      GetVisibility(const Sphere & sph);
    Visibility                      GetVisibility(const Vec3 & point);

    bool                            IsVisible(const Aabb & box);
    bool                            IsVisible(const Sphere & sph);

    void                            SetProjectionType(PROJECTION_TYPE type);
    PROJECTION_TYPE                 GetProjectionType() const;


protected:
    void                            _Update();

protected:
    String                          mName;

    Material                        mMaterial;

    float                           mNear;
    float                           mFar;

    float                           mFov;
    float                           mAspect;
    
    float                           mWidth;
    float                           mHeight;

    Mat4                            mMatView;
    Mat4                            mMatProj;
    Mat4                            mMatViewProj;
    Frustum                         mFrustum;

    bool                            mNeedUpdateView;
    bool                            mNeedUpdateProj;
    PROJECTION_TYPE                 mProjType;
};

}