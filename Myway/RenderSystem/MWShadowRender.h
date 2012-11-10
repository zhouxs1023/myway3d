#pragma once

#include "MWCamera.h"
#include "MWLight.h"

namespace Myway
{

//class MW_ENTRY ShadowRender : public AllocObj
//{
//    DECLARE_SINGLETON(ShadowRender);
//
//public:
//                                ShadowRender();
//    virtual                     ~ShadowRender();
//
//    virtual void                RenderShadow(Camera * cam, Light * light) = 0;
//
//    virtual void                NotifyShadowTexture();
//    void                        DestroyShadowTexture();
//
//    void                        SetShadowTechnique(ShadowTechnique tech);
//    void                        SetShadowTextureSetting(int size, FORMAT format, int count);
//
//    ShadowTechnique             GetShadowTechnique() const;
//    int                      GetShadowTextureSize() const;
//    FORMAT                      GetShadowTextureFormat() const;
//
//    void                        SetShadowCasterMaterial(const String & material);
//    void                        SetShadowReceiverMaterial(const String & material);
//    MaterialPtr                 GetShadowCasterMaterial() const;
//    MaterialPtr                 GetShadowReceiverMaterial() const;
//
//    void                        SetShadowCasterScheme(const String & scheme);
//    void                        SetShadowReceiverScheme(const String & scheme);
//
//    const String                GetShadowCasterScheme() const;
//    const String                GetShadowReceiverScheme() const;
//
//    TexturePtr                  GetShadowTexture();
//
//protected:
//    Vector<RenderTargetPtr>     m_render_targets;
//    RenderTargetPtr             m_shadowRT;
//    TexturePtr                  m_noShadowRT;
//    DepthStencilPtr             m_shadowDS;
//
//    DepthStencilPtr             m_depth_stencil;
//
//    Vector<Camera*>             m_cameras;
//
//    ShadowTechnique             m_technique;
//    int                      m_texture_size;
//    int                      m_texture_count;
//    FORMAT                      m_texture_format;
//    bool                        m_need_update;
//
//    MaterialPtr                 m_caster_material;
//    MaterialPtr                 m_receiver_material;
//    
//    String                      m_caster_scheme;
//    String                      m_receiver_scheme;
//};

}