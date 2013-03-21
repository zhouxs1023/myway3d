#pragma once

#include "MWMover.h"
#include "MWNode.h"
#include "MWRenderQueue.h"

using namespace Myway;


Mover::Mover(const TString128 & name)
: mName(name),
  mNode(NULL),
  mAabbLocal(Aabb::Zero),
  mAabbWorld(Aabb::Zero),
  mSphLocal(Sphere::Zero),
  mSphWorld(Sphere::Zero),
  mVisible(true),
  mCastShadow(true),
  mMirrorEnable(true)
{
}

Mover::~Mover()
{
    if (mNode)
        mNode->Detach(this);
}

void Mover::SetName(const TString128 & name)
{
    mName = name;
}

const TString128 & Mover::GetName() const
{
    return mName;
}

Vec3 Mover::GetPosition()
{
    d_assert (mNode);

    return mNode->GetWorldPosition();
}

Quat Mover::GetOrientation()
{
    d_assert (mNode);

    return mNode->GetWorldOrientation();
}

Vec3 Mover::GetDirection()
{
    return GetOrientation().AxisZ();
}

Vec3 Mover::GetUp()
{
    return GetOrientation().AxisY();
}

Vec3 Mover::GetRight()
{
    return GetOrientation().AxisX();
}

void Mover::SetBounds(const Aabb & aabb, const Sphere & sph)
{
    mAabbLocal = aabb;
    mSphLocal = sph;

    if (mNode)
    {
		mNode->_NotifyUpdate();
    }
    else
    {
        mAabbWorld = mAabbLocal;
        mAabbWorld = mAabbLocal;
    }
}

const Aabb & Mover::GetLocalAabb() const
{
    return mAabbLocal;
}

const Sphere & Mover::GetLocalSphere() const
{
    return mSphLocal;
}
 
const Aabb & Mover:: GetWorldAabb() const
{
    if (mNode && mNode->_NeedUpdate())
        mNode->_UpdateTransform();

    return mAabbWorld;
}

const Sphere & Mover::GetWorldSphere() const
{
    if (mNode && mNode->_NeedUpdate())
        mNode->_UpdateTransform();

    return mSphWorld;
}

void Mover::NotifyUpdate()
{
    const Mat4 & form = mNode->GetWorldMatrix();

    Math::AABBTransform(mAabbWorld, mAabbLocal, form);
    Math::SphereTransform(mSphWorld, mSphLocal, form);
}

void Mover::NotifyCamera(Camera * cam)
{
}

void Mover::UpdateGeometry()
{
}

void Mover::AddRenderQueue(RenderQueue * rq)
{
}

Node * Mover::GetAttachNode()
{
    return mNode;
}

bool Mover::IsAttached() const
{
    return mNode != NULL;
}

void Mover::NotifyAttach(Node * node)
{
    mNode = node;
}

void Mover::NotifyDetach()
{
    mNode = NULL;
}

BFlag32 & Mover::GetFlag()
{
    return mFlag;
}

void Mover::SetVisible(bool visible)
{
    mVisible = visible;
}

bool Mover::IsVisible() const
{
    return mVisible;
}

void Mover::SetCastShadow(bool cast)
{
    mCastShadow = cast;
}

bool Mover::IsCastShadow() const
{
    return mCastShadow;
}

void Mover::SetMirrorEnable(bool enable)
{
	mMirrorEnable = enable;
}

bool Mover::IsMirrorEnable() const
{
	return mMirrorEnable;
}