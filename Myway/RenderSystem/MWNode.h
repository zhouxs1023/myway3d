#pragma once

#include "MWRenderDefine.h"

namespace Myway
{

class Mover;
class RenderQueue;
class Camera;

class MW_ENTRY Node : public AllocObj
{
public:
    Node() {}
    virtual ~Node() {}

    virtual void                    SetPosition(const Vec3 & v)  = 0;
    virtual void                    SetOrientation(const Quat & q) = 0;
    virtual void                    SetScale(const Vec3 & v) = 0;

    virtual void                    SetPosition(float x, float y, float z) = 0;
    virtual void                    SetOrientation(float x, float y, float z, float w) = 0;
    virtual void                    SetScale(float x, float y, float z) = 0;

    virtual void                    Translate(const Vec3 & v, TRANSFORM_TYPE ts = TS_PARENT) = 0;
    virtual void                    Rotate(const Quat & q, TRANSFORM_TYPE ts = TS_PARENT) = 0;
    virtual void                    Scale(const Vec3 & v) = 0;

    virtual void                    Translate(float x, float y, float z, TRANSFORM_TYPE ts = TS_PARENT) = 0;
    virtual void                    Rotate(float x, float y, float z, float w, TRANSFORM_TYPE ts = TS_PARENT) = 0;
    virtual void                    Scale(float x, float y, float z) = 0;

    virtual void                    Pitch(float x, TRANSFORM_TYPE ts) = 0;
    virtual void                    Yaw(float x, TRANSFORM_TYPE ts) = 0;
    virtual void                    Roll(float x, TRANSFORM_TYPE ts) = 0;

    virtual void                    Move(float x) = 0;
    virtual void                    Fly(float x) = 0;
    virtual void                    Right(float x) = 0;

    virtual void                    SetVisible(bool visible) = 0;
    virtual bool                    IsVisible() const = 0;

    virtual const Vec3 &            GetPosition() const = 0;
    virtual const Quat &            GetOrientation() const = 0;
    virtual const Vec3 &            GetScale() const = 0;

    virtual Vec3                    GetDirection() const = 0;
    virtual Vec3                    GetRight() const = 0;
    virtual Vec3                    GetUp() const = 0;

    virtual const Vec3 &            GetWorldPosition() = 0;
    virtual const Quat &            GetWorldOrientation() = 0;
    virtual const Vec3 &            GetWorldScale() = 0;
    virtual const Mat4 &            GetWorldMatrix() = 0;
    virtual const Aabb &            GetWorldAabb() = 0;
    virtual const Sphere &          GetWorldBoundingSphere() = 0;

    virtual void                    Attach(Mover * geo) = 0;
    virtual void                    Detach(Mover * geo) = 0;
    virtual void                    DetachAll() = 0;

    virtual void                    _UpdateTransform() = 0;
    virtual bool                    _NeedUpdate() const = 0;
};

}