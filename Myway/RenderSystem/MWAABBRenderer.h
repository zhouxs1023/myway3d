#pragma once

#include "MWRenderer.h"

namespace Myway
{

class SceneNode;

class AABBRenderer : public Renderer
{
public:
    AABBRenderer(SceneNode * sn);
    virtual ~AABBRenderer();

    virtual void                    GetWorldPosition(Vec3 * pos);
    virtual void                    GetWorldTransform(Mat4 * form);

protected:
    void _Initialize();

protected:
    SceneNode *                     mSceneNode;
};

}
