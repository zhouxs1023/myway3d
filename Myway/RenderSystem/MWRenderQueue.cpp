#include "MWRenderQueue.h"
#include "MWShadowRender.h"
#include "MWRenderSystem.h"


using namespace Myway;

RenderQueue::RenderQueue()
{
}

RenderQueue::~RenderQueue()
{
    Clear();
}

void RenderQueue::PushRenderer(const List<SceneNode *> & nodes)
{
    List<SceneNode *>::ConstIterator whr = nodes.Begin();
    List<SceneNode *>::ConstIterator end = nodes.End();

    while (whr != end)
    {
        _pushRenderer(*whr);

        ++whr;
    }
}

void RenderQueue::_pushRenderer(SceneNode * node)
{
    SceneNode::MoverVisitor vr = node->GetMovers();

    while (!vr.Endof())
    {
        Mover * m = *vr.Cursor();

        if (m->IsVisible())
            m->AddRenderQueue(this);

        ++vr;
    }
}

void RenderQueue::AddRenderer(Renderer * obj)
{
    if (!obj->GetMaterial()->IsTransparency())
    {
        mSolidEntry.PushBack(obj);
    }
    else
    {
        mTransEntry.PushBack(obj);
    }
}

void RenderQueue::SortTransparency(Camera * cam)
{
    Renderer * e1, * e2;
    Vec3 pos1, pos2;
    float z1, z2;

    const Mat4 & view = cam->GetViewMatrix();

    for (int i = 0; i < mTransEntry.Size(); ++i)
    {
        e1 = mTransEntry[i];
        e1->GetWorldPosition(&pos1);
        Math::VecTransformZ(z1, pos1, view);

        for (int j = i + 1; j < mTransEntry.Size(); ++j)
        {
            e2 = mTransEntry[j];
            e2->GetWorldPosition(&pos2);

            Math::VecTransformZ(z2, pos1, view);

            if (z2 < z1)
            {
                mTransEntry[j] = e1;
                mTransEntry[i] = e2;

                e1 = e2;
            }
        }
    }
}

void RenderQueue::Clear()
{
    mSolidEntry.Clear();
    mTransEntry.Clear();
}

