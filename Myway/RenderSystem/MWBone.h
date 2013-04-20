#pragma once

#include "MWNode.h"

namespace Myway
{

class MW_ENTRY Bone : public Node
{
public:
	static const int K_Flag_CsBone = 0x01;

public:
    Bone(const TString128 & sName, short handle);
    ~Bone();

    virtual const TString128 &      GetName() const;

    virtual void                    SetPosition(const Vec3 & v) ;
    virtual void                    SetOrientation(const Quat & q);
    virtual void                    SetScale(const Vec3 & v);

    virtual void                    SetPosition(float x, float y, float z);
    virtual void                    SetOrientation(float x, float y, float z, float w);
    virtual void                    SetScale(float x, float y, float z);

    virtual void                    Translate(const Vec3 & v, TRANSFORM_TYPE ts = TS_PARENT);
    virtual void                    Rotate(const Quat & q, TRANSFORM_TYPE ts = TS_PARENT);
    virtual void                    Scale(const Vec3 & v);

    virtual void                    Translate(float x, float y, float z, TRANSFORM_TYPE ts = TS_PARENT);
    virtual void                    Rotate(float x, float y, float z, float w, TRANSFORM_TYPE ts = TS_PARENT);
    virtual void                    Scale(float x, float y, float z);

    virtual void                    Pitch(float x, TRANSFORM_TYPE ts = TS_PARENT);
    virtual void                    Yaw(float x, TRANSFORM_TYPE ts = TS_PARENT);
    virtual void                    Roll(float x, TRANSFORM_TYPE ts = TS_PARENT);

    virtual void                    Move(float x);
    virtual void                    Fly(float x);
    virtual void                    Right(float x);

    virtual void                    SetVisible(bool bVisible);
    virtual bool                    IsVisible() const;

    virtual const Vec3 &            GetPosition() const;
    virtual const Quat &            GetOrientation() const;
    virtual const Vec3 &            GetScale() const;

    virtual Vec3                    GetDirection() const;
    virtual Vec3                    GetRight() const;
    virtual Vec3                    GetUp() const;

    virtual const Vec3 &            GetWorldPosition();
    virtual const Quat &            GetWorldOrientation();
    virtual const Vec3 &            GetWorldScale();
    virtual const Mat4 &            GetWorldMatrix();
    virtual void                    GetTransformMatrix(Mat4 & matForm, const Mat4 & matViewProj);

    virtual const Aabb &            GetWorldAabb();
    virtual const Sphere &          GetWorldBoundingSphere();

    virtual void                    Attach(Mover * geo);
    virtual void                    Detach(Mover * geo);
    virtual void                    DetachAll();

    virtual void                    NotifyCamera(Camera * cam);

    virtual void                    _Intitalize();
    virtual void                    _Reset();

    virtual void                    _UpdateTransform();
    virtual bool                    _NeedUpdate() const;

    virtual void                    RayTracing(const Ray & ray, List<Mover*> & geos, const guid128 & id, int mark);

    virtual const Mat4 &            GetBoneMatrix();
    virtual const Mat4 &            GetCombineMatrix();
    virtual const Mat4 &            GetInvCombineMatrix();
    virtual void                    UpdateCombine(const Mat4 & mat);

    virtual int                     GetNumChildren() const;
    virtual Bone *                  GetChild(int index) const;

    virtual void                    AddChild(Bone * pChild);
    virtual Bone *                  GetParent();
            short					GetHandle();

            bool                   _IsRoot() const;
			bool				   _IsCSBone() const { return mFlag & K_Flag_CsBone; }

			void				   SetFlag(int flag) { mFlag = flag; }

			const Vec3 &			GetInitPosition() { return mInitPos; }
			const Quat &			GetInitOrientation() { return mInitOrient; }
			const Vec3 &			GetInitScale() { return mInitScale; }
			const Mat4 &			GetInitMatrix() { return mInitMatrix; }

protected:
    TString128                      mName;
    short							mHandle;
    bool                            mNeedUpdate;

    Vec3                            mPos;
    Quat                            mOrientation;
    Vec3                            mScale;
    Mat4                            mMatLocal;
    Mat4                            mMatWorld;

    Vec3                            mInitPos;
    Quat                            mInitOrient;
    Vec3                            mInitScale;
	Mat4							mInitMatrix;

    Mat4                            mMatBone;
    Mat4                            mMatCombine;
    Mat4                            mMatInvCombine;

    Aabb                            mAABB;
    Sphere                          mSphere;

    Bone *                          mParent;
    Array<Bone*>					mChildren;

	int								mFlag;							
};


//class BonePoint : public Node
//{
//public:
//    static const TString128 TYPE;
//
//public:
//    BonePoint();
//    ~BonePoint();
//
//    virtual const TString128 &          GetType() const;
//
//    virtual void                    SetPosition(const Vec3 & v) ;
//    virtual void                    SetOrientation(const Quat & q);
//    virtual void                    SetScale(const Vec3 & v);
//
//    virtual void                    SetPosition(float x, float y, float z);
//    virtual void                    SetOrientation(float x, float y, float z, float w);
//    virtual void                    SetScale(float x, float y, float z);
//
//    virtual void                    Translate(const Vec3 & v, TRANSFORM_TYPE ts = TS_PARENT);
//    virtual void                    Rotate(const Quat & q, TRANSFORM_TYPE ts = TS_PARENT);
//    virtual void                    Scale(const Vec3 & v);
//
//    virtual void                    Translate(float x, float y, float z, TRANSFORM_TYPE ts = TS_PARENT);
//    virtual void                    Rotate(float x, float y, float z, float w, TRANSFORM_TYPE ts = TS_PARENT);
//    virtual void                    Scale(float x, float y, float z);
//
//    virtual void                    Pitch(float x, TRANSFORM_TYPE ts = TS_PARENT);
//    virtual void                    Yaw(float x, TRANSFORM_TYPE ts = TS_PARENT);
//    virtual void                    Roll(float x, TRANSFORM_TYPE ts = TS_PARENT);
//
//    virtual void                    Move(float x);
//    virtual void                    Fly(float x);
//    virtual void                    Right(float x);
//
//    virtual void                    SetVisible(bool bVisible);
//    virtual bool                    IsVisible() const;
//
//    virtual const Vec3 &            GetPosition() const;
//    virtual const Quat &            GetOrientation() const;
//    virtual const Vec3 &            GetScale() const;
//
//    virtual Vec3                    GetDirection();
//    virtual Vec3                    GetRight();
//    virtual Vec3                    GetUp();
//
//    virtual const Mat4 &            GetWorldMatrix();
//    virtual void                    GetTransformMatrix(Mat4 & matForm, const Mat4 & matViewProj);
//
//    virtual const Aabb &  GetWorldAxisAlignedBoundingBox();
//    virtual const Sphere &          GetWorldBoundingSphere();
//
//    virtual void                    AttachObject(Moveable * pMoveable);
//    virtual void                    AddRenderQueue(RenderQueue * RQ);
//
//    friend class BonePoint;
//
//protected:
//    virtual void                    Update();
//
//protected:
//    Mat4                            mMatWorld;
//    Mat4                            mMatLocal;
//    Vec3                            mPos;
//    bool                            mNeedUpdate;
//    Quat                            mOrientation;
//    Vec3                            mScale;
//    bool                            mVisible;
//    Aabb                  mAABB;
//    Sphere                          mSphere;
//
//    Array<Moveable*>               mMoveables;
//
//    Bone *                          mParent;
//};

}