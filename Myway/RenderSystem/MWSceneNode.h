#pragma once

#include "MWNode.h"
#include "MWAABBRenderer.h"

namespace Myway
{

#define MAX_DIRT_DATA   256

class Scene;

class SceneNode;
typedef List<SceneNode*> SceneNodeList;

class MW_ENTRY SceneNode : public Node
{
    friend class SceneManager;

public:
    typedef Visitor<List<Mover*>::Iterator>  MoverVisitor;
    typedef Visitor<SceneNodeList::Iterator> ChildVisitor;

public:
    SceneNode(const TString128 & name);
    virtual ~SceneNode(void);

    virtual const TString128 &      GetName() const;

    virtual void                    SetPosition(const Vec3 & v);
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

    virtual Vec3                    WorldToLocal(const Vec3 & pos);
    virtual Vec3                    LocalToWorld(const Vec3 & pos);
    virtual Quat                    WorldToLocal(const Quat & quat);
    virtual Quat                    LocalToWorld(const Quat & quat);

    virtual void                    SetWorldPosition(const Vec3 & p);
    virtual void                    SetWorldPosition(float x, float y, float z);
    virtual void                    SetWorldOrientation(const Quat & q);
    virtual void                    SetWorldOrientation(float x, float y, float z, float w);

    virtual const Vec3 &            GetWorldPosition();
    virtual const Quat &            GetWorldOrientation();
    virtual const Vec3 &            GetWorldScale();
    virtual const Mat4 &            GetWorldMatrix();
    virtual const Aabb &            GetWorldAabb();
    virtual const Sphere &          GetWorldBoundingSphere();

    virtual void                    Attach(Mover * geo);
    virtual void                    Detach(Mover * geo);
    virtual void                    DetachAll();

    virtual void                    _UpdateTransform();
    virtual bool                    _NeedUpdate() const;
    virtual void                    _NotifyUpdate();

            void                    ShowBound(bool show);
            bool                    IsShowBound() const;

            BFlag32 &               GetFlag();

            MoverVisitor            GetMovers();

    virtual void                    AddChildNode(SceneNode * node);
    virtual void                    RemoveChildNode(SceneNode * node);
    virtual void                    ClearChildNode();
    virtual ChildVisitor            GetChildren();
    virtual SceneNode *             GetChild(int index);
    virtual int                     GetChildrenSize();
    virtual SceneNode *             GetChild(const TString128 & name);
    virtual SceneNode *             GetParentNode();

    virtual void                    _NotifyAttach(Scene * scene, void * dirt, int size);
    virtual void                    _NotifyDetach(Scene * scene);
    virtual Scene *                  _GetScene() const;
    virtual const void *            _GetSceneDirtData() const;

protected:
            void                    _NotifyAdded(SceneNode * node);   // when parent node add me.
            void                    _NotifyRemoved(); // when parent node removed me.
            void                    _UpdateParent();


protected:
    TString128                      mName;

    BFlag32                         mFlag;

    Mat4                            mMatWorld;
    Vec3                            mPosition;
    Quat                            mOrientation;
    Vec3                            mScale;
    Vec3                            mWorldPosition;
    Quat                            mWorldOrientation;
    Vec3                            mWorldScale;

    bool                            mVisible;
    bool                            mNeedUpdate;

    Aabb                            mAabbWorld;
    Sphere                          mSphWorld;

    List<Mover*>                    mMovers;

    bool                            mShowBound;

    SceneNode *                     mParent;
    SceneNodeList                   mChildren;

    Scene *                         mScene;
    char                            mDirt[MAX_DIRT_DATA];
};

}