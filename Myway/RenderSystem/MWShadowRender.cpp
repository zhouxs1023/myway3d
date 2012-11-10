#include "MWShadowRender.h"
#include "MWMaterialManager.h"

using namespace Myway;

IMPLEMENT_SINGLETON(ShadowRender);

ShadowRender::ShadowRender()
: m_technique(SHADOW_NONE),
  m_texture_size(1024),
  m_texture_count(1),
  m_texture_format(FMT_R32F),
  m_need_update(TRUE)
{
    INITIALIZE_SINGLETON();

    m_noShadowRT = VideoBufferManager::Instance().CreateTexture("_sys_noshadow_texture_",
                                                                 1, 1, 1,
                                                                 USAGE_NORMAL,
                                                                 POOL_MANAGED,
                                                                 FMT_A8R8G8B8);

    LockedBox box;
    m_noShadowRT->Lock(0, &box, NULL, LOCK_NORMAL);
    *(int *)box.pData = 0xFFFFFFFF; // white
    m_noShadowRT->Unlock(0);
}

ShadowRender::~ShadowRender()
{
    DestroyShadowTexture();
    SHUTDOWN_SINGLETON();
}

void ShadowRender::SetShadowTechnique(ShadowTechnique tech)
{
    m_technique = tech;
}

void ShadowRender::SetShadowTextureSetting(int size, FORMAT format, int count)
{
    m_texture_size = size;
    m_texture_format = format;
    m_texture_count = count;

    m_need_update = TRUE;
}

ShadowTechnique ShadowRender::GetShadowTechnique() const
{
    return m_technique;
}

int ShadowRender::GetShadowTextureSize() const
{
    return m_texture_size;
}

FORMAT ShadowRender::GetShadowTextureFormat() const
{
    return m_texture_format;
}

void ShadowRender::NotifyShadowTexture()
{
    if (m_technique == SHADOW_TEXTURE)
    {
        if (m_need_update)
        {
            DestroyShadowTexture();

            const String shadow_map_name("Sys_ShadowMap");
            const String depth_stencil_name("Sys_ShadowDepthStencil");
            const String camera_name("Sys_ShadowCamera");
            const String shadow_ds_name("Sys_ShadowDS");

            for (int i = 0; i < m_texture_count; ++i)
            {
                RenderTargetPtr rt = VideoBufferManager::Instance().CreateRenderTarget(shadow_map_name + i,
                                                                                       m_texture_size,
                                                                                       m_texture_size,
                                                                                       m_texture_format,
                                                                                       MULTISAMPLE_NONE);
                m_render_targets.PushBack(rt);
                m_cameras.PushBack(new Camera(camera_name + i));
            }

            m_depth_stencil = VideoBufferManager::Instance().CreateDepthStencil(depth_stencil_name,
                                                                                m_texture_size,
                                                                                m_texture_size,
                                                                                FMT_D24X8,
                                                                                MULTISAMPLE_NONE);

            m_shadowRT = VideoBufferManager::Instance().CreateRenderTarget(shadow_map_name, -1, -1,
                                                                           FMT_A8R8G8B8, MULTISAMPLE_NONE, TRUE);

            m_shadowDS = VideoBufferManager::Instance().CreateDepthStencil(shadow_ds_name, -1, -1,
                                                                           FMT_D24X8, MULTISAMPLE_NONE, TRUE);

            m_need_update = FALSE;
        }
    }

    else if (m_technique == SHADOW_VOLUME)
    {
        DestroyShadowTexture();
    }

    else
    {
        DestroyShadowTexture();
    }
}

void ShadowRender::DestroyShadowTexture()
{
    Vector<Camera*>::Iterator citer;
    Vector<Camera*>::Iterator cend;

    citer = m_cameras.Begin();
    cend = m_cameras.End();

    while (citer != cend)
    {
        delete *citer;

        ++citer;
    }

    m_cameras.Clear();

    Vector<RenderTargetPtr>::Iterator iter;
    Vector<RenderTargetPtr>::Iterator end;

    iter = m_render_targets.Begin();
    end = m_render_targets.End();

    while (iter != end)
    {
        (*iter).SetNull();

        ++iter;
    }

    m_depth_stencil.SetNull();
    m_shadowDS = NULL;
    m_shadowRT = NULL;

    m_render_targets.Clear();
}

void ShadowRender::SetShadowCasterMaterial(const String & material)
{
    m_caster_material = MaterialManager::Instance().Find(material);

    if (m_caster_material.IsNull())
        m_caster_material = MaterialManager::Instance().Load(material, material);
}

void ShadowRender::SetShadowReceiverMaterial(const String & material)
{
    m_receiver_material = MaterialManager::Instance().Find(material);

    if (m_receiver_material.IsNull())
        m_receiver_material = MaterialManager::Instance().Load(material, material);
}

MaterialPtr ShadowRender::GetShadowCasterMaterial() const
{
    return m_caster_material;
}

MaterialPtr ShadowRender::GetShadowReceiverMaterial() const
{
    return m_receiver_material;
}

void ShadowRender::SetShadowCasterScheme(const String & scheme)
{
    m_caster_scheme = scheme;
}

void ShadowRender::SetShadowReceiverScheme(const String & scheme)
{
    m_receiver_scheme = scheme;
}

const String ShadowRender::GetShadowCasterScheme() const
{
    return m_caster_scheme;
}

const String ShadowRender::GetShadowReceiverScheme() const
{
    return m_receiver_scheme;
}


TexturePtr ShadowRender::GetShadowTexture()
{
    if (m_technique == SHADOW_TEXTURE)
    {
        return TexturePtr(m_shadowRT.GetPointer());
    }
    else
    {
        return TexturePtr(m_noShadowRT.GetPointer());
    }
}