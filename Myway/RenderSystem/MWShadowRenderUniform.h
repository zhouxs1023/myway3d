#pragma once

#include "MWShadowRender.h"

namespace Myway
{

class MW_ENTRY ShadowRenderUniform : public ShadowRender
{
public:
    ShadowRenderUniform();
    virtual ~ShadowRenderUniform();

    void            RenderShadow(Camera * cam, Light * light);

    void            SetDistance(float dist);
    void            SetNearClip(float clip);
    void            SetFarClip(float clip);
    void            SetOffset(float offset);    //only effect directional light

    float           GetDistance() const;
    float           GetNearClip() const;
    float           GetFarClip() const;
    float           GetOffset() const;

protected:
    bool            UpdateCamera(Camera * pShadowCamera,
                                 int direction,
                                 Light * light,
                                 Camera * pSceneCamera);

    void            UpdateShadowCasterQueue(Camera * pSceneCamera);
    void            UpdateShadowReceiverQueue();
    void            RenderShadowCasterQueue();
    void            RenderShadowReceiverQueue();

protected:
    float           m_distance;
    float           m_offset;
    float           m_near_clip;
    float           mar_clip;
    Camera *        m_caster_camera;
    Camera *        m_receiver_camera;
};


}