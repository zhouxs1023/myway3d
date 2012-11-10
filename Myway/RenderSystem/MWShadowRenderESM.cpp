#if 0

#include "d3dx9.h"

#endif

#include "MWShadowRenderESM.h"
#include "MWRenderQueue.h"
#include "MWRenderSystem.h"
#include "MWSceneManager.h"

namespace Myway
{


ShadowRenderESM::ShadowRenderESM()
: m_distance(50.0f),
  m_near_clip(1.0f),
  mar_clip(10000.0f),
  m_offset(200.0f)
{
    m_caster_camera = new Camera("Sys_ShaderRenderUniform_Caster_Camera");
    m_receiver_camera = new Camera("Sys_ShaderRenderUniform_Reciever_Camera");
}

ShadowRenderESM::~ShadowRenderESM()
{
    safe_delete(m_caster_camera);
    safe_delete(m_receiver_camera);
}

bool ShadowRenderESM::UpdateCamera(Camera * pShadowCamera, int direction, Light * light, Camera * pSceneCamera)
{
    LIGHT_TYPE type = light->GetType();

    if (type == LIGHTTYPE_DIRECTIONAL)
    {
        Vec3 right, up, dir;

        dir = light->GetDirection();
        up = pSceneCamera->GetUp();

        Math::VecCross(right, up, dir);
        Math::VecCross(up, dir, right);
        Math::VecNormalize(right, right);
        Math::VecNormalize(up, up);
        Math::VecNormalize(dir, dir);

        //transform camera projective space
        Vec3 v(0, 0, m_distance);
        Math::VecTransform(v, v, pSceneCamera->GetProjMatrix());

        float f = v.z;
        float n = pSceneCamera->GetNearClip();
        f = Math::Clamp(f, 0.0f, 1.0f);

        Vec3 p[8], p2[8];

        p[0] = Vec3(-1, 1, n);
        p[1] = Vec3(-1, 1, f);
        p[2] = Vec3(1, 1, n);
        p[3] = Vec3(1, 1, f);
        p[4] = Vec3(-1, -1, n);
        p[5] = Vec3(-1, -1, f);
        p[6] = Vec3(1, -1, n);
        p[7] = Vec3(1, -1, f);

        Mat4 mat1, mat2;

        //transform world space
        Math::MatInverse(mat1, pSceneCamera->GetViewProjMatrix());
        Math::VecTransform(p, p, mat1, 8);

        //transform light space
        Math::MatViewLH(mat2, pSceneCamera->GetPosition() - dir * m_offset, pSceneCamera->GetPosition(), up);
        Math::VecTransform(p2, p, mat2, 8);

        Vec3 minimum = p2[0], maximum = p2[0];

        for (int i = 1; i < 8; ++i)
        {
            Math::VecMinimum(minimum, minimum, p2[i]);
            Math::VecMaximum(maximum, maximum, p2[i]);
        }

        float width = maximum.x - minimum.x;
        float height = maximum.y - minimum.y;

        //transform world space
        Math::MatInverse(mat2, mat2);
        Math::VecTransform(minimum, minimum, mat2);
        Math::VecTransform(maximum, maximum, mat2);

        Vec3 eye = (maximum + minimum) * 0.5f;
        eye += -dir * m_offset;

        Math::MatViewLH(mat2, eye, eye + dir, up);
        Math::VecTransformZ(f, maximum, mat2);

        //shadow camera
        pShadowCamera->SetPosition(eye);
        pShadowCamera->SetRight(right);
        pShadowCamera->SetUp(up);
        pShadowCamera->SetDirection(dir);

        pShadowCamera->SetProjectionType(PROJTYPE_ORTHO);
        pShadowCamera->SetOrthoWidth(width);
        pShadowCamera->SetOrthoHeight(height);
        pShadowCamera->SetNearClip(m_near_clip);
        pShadowCamera->SetFarClip(mar_clip);

        //caster camera
        m_caster_camera->SetPosition(eye);
        m_caster_camera->SetRight(right);
        m_caster_camera->SetUp(up);
        m_caster_camera->SetDirection(dir);

        m_caster_camera->SetProjectionType(PROJTYPE_ORTHO);
        m_caster_camera->SetOrthoWidth(width);
        m_caster_camera->SetOrthoHeight(height);
        m_caster_camera->SetNearClip(m_near_clip);
        m_caster_camera->SetFarClip(f);

#if 0
        Vec3 _min, _max;

        const Mat4 & _v1 = mat2;
        const Mat4 & _v2 = m_caster_camera->GetViewMatrix();
        D3DXMATRIX __d3dv;
        Vec3 __at = eye + dir;
        D3DXMatrixLookAtLH(&__d3dv, (const D3DXVECTOR3 *)&eye, (const D3DXVECTOR3 *)&__at, (const D3DXVECTOR3 *)&up);

        Vec3 p3[8];
        const Mat4 & _mvp = m_caster_camera->GetViewProjMatrix();
        Math::VecTransform(_max, maximum, _mvp);
        Math::VecTransform(_min, minimum, _mvp);
        Math::VecTransform(p3, p, _mvp, 8);

#endif

        //reciever camera
        m_receiver_camera->SetPosition(pSceneCamera->GetPosition());
        m_receiver_camera->SetRight(pSceneCamera->GetRight());
        m_receiver_camera->SetUp(pSceneCamera->GetUp());
        m_receiver_camera->SetDirection(pSceneCamera->GetDirection());

        m_receiver_camera->SetProjectionType(pSceneCamera->GetProjectionType());
        m_receiver_camera->SetFov(pSceneCamera->GetFov());
        m_receiver_camera->SetAscept(pSceneCamera->GetAscept());
        m_receiver_camera->SetNearClip(pSceneCamera->GetNearClip());
        m_receiver_camera->SetFarClip(m_distance);

        return TRUE;
    }

    return FALSE;
}

void ShadowRenderESM::RenderShadow(Camera * cam, Light * light)
{
    profile_code();

    if (m_technique != SHADOW_TEXTURE)
        return;

    RenderSystem * render = RenderSystem::Instance().InstancePtr();

    int max_dir = 1;
    if (light->GetType() == LIGHTTYPE_POINT)
        max_dir = 6;

    for (int dir = 0; dir < max_dir; ++dir)
    {
        if (UpdateCamera(m_cameras[0], dir, light, cam))
        {
            render->SetLight(0, light);

            UpdateShadowCasterQueue(cam);

            render->SetRenderTarget(0, m_render_targets[0].GetPointer());
            render->SetDepthStencil(m_depth_stencil.GetPointer());
            render->ClearBuffer(NULL, TRUE, TRUE, FALSE, Color::White, 1.0f, 0);

            RenderShadowCasterQueue();
        }

        render->SetShadowViewTransform(0, m_cameras[0]->GetViewMatrix());
        render->SetShadowProjTransform(0, m_cameras[0]->GetProjMatrix());

        render->SetRenderTarget(0, m_shadowRT.GetPointer());
        render->ClearBuffer(NULL, TRUE, TRUE, FALSE, Color::White, 1.0f, 0);

        UpdateShadowReceiverQueue();
        RenderShadowReceiverQueue();
    }
}


void ShadowRenderESM::RenderShadowCasterQueue()
{
    RenderSystem * render = RenderSystem::Instance().InstancePtr();

    render->SetShadowViewTransform(0, m_cameras[0]->GetViewMatrix());
    render->SetShadowProjTransform(0, m_cameras[0]->GetProjMatrix());

    RenderQueue::Instance().Render(0);
}

void ShadowRenderESM::RenderShadowReceiverQueue()
{
    RenderSystem * render = RenderSystem::InstancePtr();

    int t = 0;

    for (; t < m_texture_count; ++t)
    {
        m_render_targets[t]->Present();
        render->SetTexture(t, m_render_targets[t].GetPointer());
    }

    RenderQueue::Instance().Render(t);
}


void ShadowRenderESM::UpdateShadowCasterQueue(Camera * pSceneCamera)
{
    RenderQueue * rq = RenderQueue::InstancePtr();

    SceneNodeList::Iterator iter;
    SceneNodeList::Iterator end;

    SceneNodeList list;
    SceneManager::Instance().GetVisibleSceneNodes(list, m_caster_camera);

    rq->Clear();
    rq->SetScheme(m_caster_scheme);
    rq->SetDefault(m_caster_material);

    iter = list.Begin();
    end = list.End();

    while (iter != end)
    {
        SceneNode * node = *iter;
        if (node->IsVisible())
        {
            SceneNode::MoverVisitor v = node->GetGeometres();

            while (!v.Endof())
            {
                Mover * geo = *v.Cursor();

                if (geo->GetCastShadow())
                {
                    geo->NotifyCamera(pSceneCamera);
                    geo->UpdateMover();

                    geo->AddRenderQueue(rq);
                }

                ++v;
            }
        }

        ++iter;
    }
}

void ShadowRenderESM::UpdateShadowReceiverQueue()
{
    RenderQueue * rq = RenderQueue::InstancePtr();

    SceneManager::VisibleNodeVisitor visitor = SceneManager::Instance().GetVisibleSceneNodes();

    assert (!m_cameras.Empty());

    rq->Clear();
    rq->SetScheme(m_receiver_scheme);
    rq->SetDefault(m_receiver_material);

    while (!visitor.Endof())
    {
        SceneNode * node = *visitor.Cursor();
        if (node->IsVisible())
        {
            const Aabb & aabb = node->GetWorldAabb();

            if (m_receiver_camera->IsVisible(aabb))
            {
                SceneNode::MoverVisitor v = node->GetGeometres();

                while (!v.Endof())
                {
                    Mover * geo = *v.Cursor();

                    if (geo->GetReceiveShadow())
                    {
                        geo->AddRenderQueue(rq);
                    }

                    ++v;
                }
            }
        }

        ++visitor;
    }
}


void ShadowRenderESM::SetDistance(float dist)
{
    m_distance = dist;
}

void ShadowRenderESM::SetNearClip(float clip)
{
    m_near_clip = clip;
}

void ShadowRenderESM::SetFarClip(float clip)
{
    mar_clip =clip;
}

void ShadowRenderESM::SetOffset(float offset)
{
    m_offset = offset;
}

float ShadowRenderESM::GetDistance() const
{
    return m_distance;
}

float ShadowRenderESM::GetNearClip() const
{
    return m_near_clip;
}

float ShadowRenderESM::GetFarClip() const
{
    return mar_clip;
}

float ShadowRenderESM::GetOffset() const
{
    return m_offset;
}



}
