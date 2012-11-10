#pragma once

#include "MWCamera.h"

namespace Myway
{

class MW_ENTRY SceneRender : public AllocObj
{
public:
    SceneRender() {}
    virtual ~SceneRender() {}

    virtual void RenderScene(Camera * cam) = 0;
};

}