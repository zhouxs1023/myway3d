#pragma once

#include "MWWorld.h"

namespace Myway
{

class MW_ENTRY RenderQueue
{
    DECLARE_ALLOC();

public:
    RenderQueue();
    ~RenderQueue();

    void PushRenderer(const List<SceneNode *> & nodes);
    void AddRenderer(Renderer * obj);

    void SortTransparency(Camera * cam);
    void Clear();

    const Array<Renderer *> & GetSolidRender() { return mSolidEntry; }
    const Array<Renderer *> & GetTransEntry() { return mTransEntry; }

protected:
    void _pushRenderer(SceneNode * node);

protected:
    Array<Renderer *>   mSolidEntry;
    Array<Renderer *>   mTransEntry;
};

}