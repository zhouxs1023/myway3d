#pragma once

#include "MWShadowRender.h"

namespace Myway
{

class MW_ENTRY ShadowRenderPSSM : public ShadowRender
{
public:
    ShadowRenderPSSM();
    virtual ~ShadowRenderPSSM();

    void RenderShadow(Camera * cam, Light * light);
    void SetSplitPoint(float * split, int size);
    void SetSplitParamName(const String & name);

    void SetDistance(float dist);
    void SetNearClip(float clip);
    void SetFarClip(float clip);
    void SetOffset(float offset);    //only effect directional light

    float GetDistance() const;
    float GetNearClip() const;
    float GetFarClip() const;
    float GetOffset() const;

    void SetBlurMaterial(const String & sBlurH, const String & sBlurV);
    void SetBlurCount(int count);

protected:
    virtual void NotifyShadowTexture();

    bool IsUseBlur() const;
    void NotUseBlur();

protected:
    bool UpdateCamera(Camera * pShadowCamera, int number, int direction, const Light * light, Camera * pSceneCamera);
    void UpdateShadowCasterQueue(Camera * pShadowCamera, Camera * pSceneCamera);
    void UpdateShadowReceiverQueue();
    void RenderShadowCasterQueue(int index);
    void RenderShadowReceiverQueue();


protected:
    float           m_split[MAX_SHADOW_CAMERAS];
    float           m_distance;
    float           m_offset;
    float           m_near_clip;
    float           mar_clip;
    Camera *        m_caster_camera;
    Camera *        m_receiver_camera;
    ShaderParam *   m_split_param;

    MaterialPtr     m_blurH;
    MaterialPtr     m_blurV;
    RenderTargetPtr m_blurRT;
    int             m_blur_count;
};


}