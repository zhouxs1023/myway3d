#pragma once

#include "MWSceneRender.h"
#include "MWRenderTarget.h"
#include "MWShadowRender.h"
#include "MWSceneNode.h"

namespace Myway
{

enum RenderUniformStage
{
    RUS_DIFFUSE,
    RUS_PROJECTION,

    MW_ALIGN_ENUM(RenderUniformStage)
};


class MW_ENTRY SceneRenderUniform : public SceneRender
{
    DECLARE_SINGLETON(SceneRenderUniform)

public:
    SceneRenderUniform();
    virtual ~SceneRenderUniform();

    virtual void RenderScene(Camera * cam);

    void SetScheme(RenderUniformStage stage, const String & scheme);
    const String & GetScheme(RenderUniformStage stage);


protected:
    void RenderToCBuffer();
    void RenderProjection();

    void AddRenderQueue(SceneNode * node);

    void HDR_Init();
    void HDR_DeInit();
    void HDR_Render();
    void HDR_Bloom();
    void HDR_Lum();
    void HDR_ToneMap();

protected:
    String              m_diffuse_scheme;
    String              mroj_scheme;
    Camera *            m_camera;
};

}