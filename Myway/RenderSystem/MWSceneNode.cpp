#include "MWSceneNode.h"
#include "MWMover.h"
#include "MWRenderQueue.h"
#include "MWScene.h"

using namespace Myway;

SceneNode::SceneNode(const TString128 & name)
: mName(name),
  mPosition(Vec3::Zero),
  mOrientation(Quat::Identity),
  mScale(Vec3::Unit),
  mWorldPosition(Vec3::Zero),
  mWorldOrientation(Quat::Identity),
  mWorldScale(Vec3::Unit),
  mAabbWorld(Aabb::Zero),
  mSphWorld(Sphere::Zero),
  mNeedUpdate(true),
  mShowBound(false),
  mVisibleMask(true),
  mVisible(true),
  mParent(NULL),
  mScene(NULL),
  mPhyData(NULL)
{
    ZeroMemory(mDirt, MAX_DIRT_DATA);
}

SceneNode::~SceneNode()
{
    if (mScene)
        mScene->RemoveSceneNode(this);

    if (mParent)
        mParent->RemoveChildNode(this);

    ClearChildNode();

    DetachAll();

	d_assert (mPhyData == NULL);
}

const TString128 & SceneNode::GetName() const
{
    return mName;
}

void SceneNode::SetPosition(const Vec3 & vPos)
{
    mPosition = vPos;
    _NotifyUpdate();
}

void SceneNode::SetOrientation(const Quat & qOrientation)
{
    mOrientation = qOrientation;
    _NotifyUpdate();
}

void SceneNode::SetScale(const Vec3 & vScale)
{
    mScale = vScale;
    _NotifyUpdate();
}

void SceneNode::SetPosition(float x, float y, float z)
{
    mPosition.x = x;
    mPosition.y = y;
    mPosition.z = z;
    _NotifyUpdate();
}

void SceneNode::SetOrientation(float x, float y, float z, float w)
{
    mOrientation.x = x;
    mOrientation.y = y;
    mOrientation.z = z;
    mOrientation.w = w;
    _NotifyUpdate();
}

void SceneNode::SetScale(float x, float y, float z)
{
    mScale.x = x;
    mScale.y = y;
    mScale.z = z;
    _NotifyUpdate();
}

void SceneNode::Translate(const Vec3 & vTrans, TRANSFORM_TYPE ts)
{
    switch (ts)
    {
    case TS_WORLD:
        {
            if (mParent)
            {
                Quat qinv;
                Vec3 t;

                Math::QuatInverse(qinv, mParent->GetWorldOrientation());
                Math::QuatTranslate(t, qinv, vTrans);
                mPosition += t / mParent->GetWorldScale();
            }
            else
            {
                mPosition += vTrans;
            }
        }
        break;

    case TS_PARENT:
        {
            mPosition += vTrans;
        }
        break;

    case TS_LOCAL:
        {
            Vec3 t;
            Math::QuatTranslate(t, mOrientation, vTrans);
            mPosition += t;
        }
        break;
    }

    _NotifyUpdate();
}

void SceneNode::Rotate(const Quat & qRotate, TRANSFORM_TYPE ts)
{
    Quat qn;

    Math::QuatNormalize(qn, qRotate);

    switch (ts)
    {
    case TS_WORLD:
        {
            if (mParent)
            {
                Quat qp, qpi;

                qp = mParent->GetWorldOrientation();
                Math::QuatInverse(qpi, qp);

                mOrientation = mOrientation * qpi * qn * qp;
            }
            else
            {
                mOrientation = qn * mOrientation;
            }
        }
        break;

    case TS_PARENT:
        {
            mOrientation = qn * mOrientation;
        }
        break;

    case TS_LOCAL:
        {
            mOrientation = mOrientation * qn;
        }
        break;
    }

    _NotifyUpdate();
}

void SceneNode::Scale(const Vec3 & vScale)
{
    mScale.x *= vScale.x;
    mScale.y *= vScale.y;
    mScale.z *= vScale.z;
    _NotifyUpdate();
}

void SceneNode::Translate(float x, float y, float z, TRANSFORM_TYPE ts)
{
    Translate(Vec3(x, y, z), ts);
}

void SceneNode::Rotate(float x, float y, float z, float w, TRANSFORM_TYPE ts)
{
    Rotate(Quat(x, y, z, w), ts);
}

void SceneNode::Scale(float x, float y, float z)
{
    Scale(Vec3(x, y, z));
}

void SceneNode::Pitch(float x, TRANSFORM_TYPE ts)
{
    Quat q;
    Math::QuatFromAxis(q, Vec3::UnitX, x);
    Rotate(q, ts);
}

void SceneNode::Yaw(float x, TRANSFORM_TYPE ts)
{
    Quat q;
    Math::QuatFromAxis(q, Vec3::UnitY, x);
    Rotate(q, ts);
}

void SceneNode::Roll(float x, TRANSFORM_TYPE ts)
{
    Quat q;
    Math::QuatFromAxis(q, Vec3::UnitZ, x);
    Rotate(q, ts);
}

void SceneNode::Move(float x)
{
    Vec3 dir;
    Math::QuatAxisZ(dir, mOrientation);
    mPosition += dir * x;
    _NotifyUpdate();
}

void SceneNode::Fly(float x)
{
    Vec3 up;
    Math::QuatAxisY(up, mOrientation);
    mPosition += up * x;
    _NotifyUpdate();
}

void SceneNode::Right(float x)
{
    Vec3 right;
    Math::QuatAxisX(right, mOrientation);
    mPosition += right * x;
    _NotifyUpdate();
}

const Vec3 & SceneNode::GetPosition() const
{
    return mPosition;
}

const Quat & SceneNode::GetOrientation() const
{
    return mOrientation;
}

const Vec3 & SceneNode::GetScale() const
{
    return mScale;
}

Vec3 SceneNode::GetDirection() const
{
    Vec3 dir;
    Math::QuatAxisZ(dir, mOrientation);
    return dir;
}

Vec3 SceneNode::GetRight() const
{
    Vec3 right;
    Math::QuatAxisX(right, mOrientation);
    return right;
}

Vec3 SceneNode::GetUp() const
{
    Vec3 up;
    Math::QuatAxisY(up, mOrientation);
    return up;
}

Vec3 SceneNode::WorldToLocal(const Vec3 & pos)
{
    if (_NeedUpdate())
        _UpdateTransform();

    Quat qinv;
    Math::QuatInverse(qinv, mWorldOrientation);

    Vec3 t;
    Math::QuatTranslate(t, qinv, pos - mWorldPosition);

    return t / mWorldScale;
}

Vec3 SceneNode::LocalToWorld(const Vec3 & pos)
{
    if (_NeedUpdate())
        _UpdateTransform();

    Vec3 t;

    Math::QuatTranslate(t, mWorldOrientation, pos);

    return t * mWorldScale + mWorldPosition;
}

Quat SceneNode::WorldToLocal(const Quat & quat)
{
    if (_NeedUpdate())
        _UpdateTransform();

    Quat qinv;

    Math::QuatInverse(qinv, mWorldOrientation);

    return qinv * quat;
}

Quat SceneNode::LocalToWorld(const Quat & quat)
{
    if (_NeedUpdate())
        _UpdateTransform();

    return mWorldOrientation * quat;
}

void SceneNode::SetWorldPosition(const Vec3 & p)
{
    SetPosition(WorldToLocal(p));
}

void SceneNode::SetWorldPosition(float x, float y, float z)
{
    SetWorldPosition(Vec3(x, y, z));
}

void SceneNode::SetWorldOrientation(const Quat & q)
{
    SetOrientation(WorldToLocal(q));
}

void SceneNode::SetWorldOrientation(float x, float y, float z, float w)
{
    SetWorldOrientation(Quat(x, y, z, w));
}

void SceneNode::SetVisible(bool visible)
{
    mVisible = visible;
}

bool SceneNode::IsVisible() const
{
    return mVisible;
}

void SceneNode::_UpdateTransform()
{
    _UpdateParent();

    Math::MatTransform(mMatWorld, mWorldPosition, mWorldOrientation, mWorldScale);

    mNeedUpdate = FALSE;

    //update geometry
    List<Mover*>::Iterator iter;
    List<Mover*>::Iterator end;

    iter = mMovers.Begin();
    end = mMovers.End();

    while (iter != end)
    {
        (*iter)->NotifyUpdate();
        ++iter;
    }

    iter = mMovers.Begin();
    if (iter != end)
    {
		mAabbWorld = (*iter)->GetWorldAabb();

        while (++iter != end)
            Math::AABBMerge(mAabbWorld, mAabbWorld, (*iter)->GetWorldAabb());

        mSphWorld.center = mAabbWorld.GetCenter();
        mSphWorld.radius = Math::VecDistance(mAabbWorld.maximum, mSphWorld.center);
    }
    else
    {
        mAabbWorld.minimum = mPosition;
		mAabbWorld.maximum = mPosition;

        mSphWorld.center = mPosition;
        mSphWorld.radius = 0;
    }

	{
		SceneNodeList::Iterator whr = mChildren.Begin();
		SceneNodeList::Iterator end = mChildren.End();

		while (whr != end)
		{
			Node * child = (*whr);
			mAabbWorld.Merge(child->GetWorldAabb());
			mSphWorld.radius = Math::VecDistance(mAabbWorld.maximum, mSphWorld.center);

			++whr;
		}
	}

    if (mScene)
        mScene->UpdateSceneNode(this);
}

bool SceneNode::_NeedUpdate() const
{
    return mNeedUpdate;
}

void SceneNode::_NotifyUpdate()
{
    mNeedUpdate = TRUE;

    ChildVisitor v = GetChildren();

    while (!v.Endof())
    {
        SceneNode * n = *v.Cursor();
        n->_NotifyUpdate();

        ++v;
    }
}

const Vec3 & SceneNode::GetWorldPosition()
{
    if (mNeedUpdate)
        _UpdateTransform();

    return mWorldPosition;
}

const Quat & SceneNode::GetWorldOrientation()
{
    if (mNeedUpdate)
        _UpdateTransform();

    return mWorldOrientation;
}

const Vec3 & SceneNode::GetWorldScale()
{
    if (mNeedUpdate)
        _UpdateTransform();

    return mWorldScale;
}

const Mat4 & SceneNode::GetWorldMatrix()
{
    if (mNeedUpdate)
        _UpdateTransform();

    return mMatWorld;
}

const Aabb & SceneNode::GetWorldAabb()
{
    if (mNeedUpdate)
        _UpdateTransform();

    return mAabbWorld;
}

const Sphere & SceneNode::GetWorldBoundingSphere()
{
    if (mNeedUpdate)
        _UpdateTransform();

    return mSphWorld;
}

void SceneNode::Attach(Mover * geo)
{
    mMovers.PushBack(geo);
    geo->NotifyAttach(this);

    mNeedUpdate = TRUE;
}

void SceneNode::Detach(Mover * geo)
{
    List<Mover*>::Iterator iter;
    List<Mover*>::Iterator end;

    iter = mMovers.Begin();
    end = mMovers.End();

    while (iter != end)
    {
        if (*iter == geo)
        {
            mMovers.Erase(iter);
            break;
        }

        ++iter;
    }

    geo->NotifyDetach();
}

void SceneNode::DetachAll()
{
    List<Mover*>::Iterator iter;
    List<Mover*>::Iterator end;

    iter = mMovers.Begin();
    end = mMovers.End();

    while (iter != end)
    {
        (*iter)->NotifyDetach();
        ++iter;
    }

    mMovers.Clear();

    mAabbWorld.minimum = mAabbWorld.maximum = mSphWorld.center = mPosition;
    mSphWorld.radius = 0;
}

BFlag32 & SceneNode::GetFlag()
{
    return mFlag;
}

SceneNode::MoverVisitor SceneNode::GetMovers()
{
    if (mNeedUpdate)
        _UpdateTransform();

    return MoverVisitor(mMovers.Begin(), mMovers.End());
}

void SceneNode::ShowBound(bool bShow)
{
    mShowBound = bShow;
}

bool SceneNode::IsShowBound() const
{
    return mShowBound;
}

void SceneNode::AddChildNode(SceneNode * node)
{
    d_assert (node != this);

    // remove from parent
    if (node->GetParentNode())
        node->GetParentNode()->RemoveChildNode(node);

	_NotifyUpdate();

    node->_NotifyAdded(this);
    mChildren.PushBack(node);

}

void SceneNode::RemoveChildNode(SceneNode * node)
{
    SceneNodeList::Iterator whr = mChildren.Begin();
    SceneNodeList::Iterator end = mChildren.End();

    while (whr != end && (*whr) != node)
    {
        ++whr;
    }

    d_assert (whr != end);

    if (whr != end)
    {
        mChildren.Erase(whr);
        node->_NotifyRemoved();
    }
}

void SceneNode::ClearChildNode()
{
    SceneNodeList::Iterator whr = mChildren.Begin();
    SceneNodeList::Iterator end = mChildren.End();

    while (whr != end)
    {
        (*whr)->_NotifyRemoved();

        ++whr;
    }

    mChildren.Clear();
}

SceneNode::ChildVisitor SceneNode::GetChildren()
{
    return ChildVisitor(mChildren.Begin(), mChildren.End());
}

SceneNode * SceneNode::GetChild(int index)
{
    if (GetChildrenSize() == 0 || index < (int)mChildren.Size())
        return NULL;

    SceneNodeList::Iterator whr = mChildren.Begin();
    SceneNodeList::Iterator end = mChildren.End();

    while (index-- > 0)
    {
        ++whr;
    }

    return *whr;
}

int SceneNode::GetChildrenSize()
{
    return mChildren.Size();
}

SceneNode * SceneNode::GetChild(const TString128 & name)
{
	SceneNodeList::Iterator whr = mChildren.Begin();
	SceneNodeList::Iterator end = mChildren.End();

    while (whr != end && (*whr)->GetName() != name)
    {
        ++whr;
    }

    return whr != end ? *whr : NULL;
}

SceneNode * SceneNode::GetParentNode()
{
    return mParent;
}

void SceneNode::_NotifyAdded(SceneNode * node)
{
    mParent = node;
    _NotifyUpdate();
}

void SceneNode::_NotifyRemoved()
{
    mParent = NULL;
    _NotifyUpdate();
}

void SceneNode::_UpdateParent()
{
    if (mParent)
    {
        if (mParent->_NeedUpdate())
            mParent->_UpdateTransform();

        const Quat & parentOrientation = mParent->GetWorldOrientation();
        mWorldOrientation = parentOrientation * mOrientation;

        const Vec3 & parentScale = mParent->GetWorldScale();
        mWorldScale = parentScale * mScale;

        Math::QuatTranslate(mWorldPosition, parentOrientation, parentScale * mPosition);

        mWorldPosition += mParent->GetWorldPosition();
    }
    else
    {
        mWorldPosition = mPosition;
        mWorldOrientation = mOrientation;
        mWorldScale = mScale;
    }

    Math::QuatNormalize(mWorldOrientation, mWorldOrientation);
    Math::QuatNormalize(mOrientation, mOrientation);
}





void SceneNode::_NotifyAttach(Scene * scene, void * dirt, int size)
{
    assert (!mScene && scene && size < MAX_DIRT_DATA);

    mScene = scene;
    Memcpy(mDirt, dirt, size);
}

void SceneNode::_NotifyDetach(Scene * scene)
{
    assert (scene && mScene == scene);

    mScene = NULL;
    ZeroMemory(mDirt, MAX_DIRT_DATA);
}

Scene * SceneNode::_GetScene() const
{
    return mScene;
}

const void * SceneNode::_GetSceneDirtData() const
{
    return mDirt;
}

