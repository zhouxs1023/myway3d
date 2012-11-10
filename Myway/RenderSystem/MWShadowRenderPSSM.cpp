#include "MWShadowRenderPSSM.h"
#include "MWRenderQueue.h"
#include "MWRenderSystem.h"
#include "MWSceneManager.h"

using namespace Myway;

ShadowRenderPSSM::ShadowRenderPSSM()
: m_distance(150.0f),
  m_near_clip(1.0f),
  mar_clip(10000.0f),
  m_offset(500.0f)
{
    m_caster_camera = new Camera("Sys_ShaderRenderUniform_Caster_Camera");
    m_receiver_camera = new Camera("Sys_ShaderRenderUniform_Reciever_Camera");

    Memzero(m_split, sizeof(float) * MAX_SHADOW_CAMERAS);

    m_split[0] = 0.1f;
    m_split[1] = 50.0f;
    m_split[2] = 100.0f;
    m_split[3] = 150.0f;

    m_split_param = NULL;

    m_blur_count = 0;
}

ShadowRenderPSSM::~ShadowRenderPSSM()
{
    safe_delete(m_caster_camera);
    safe_delete(m_receiver_camera);
}

bool ShadowRenderPSSM::UpdateCamera(Camera * pShadowCamera, int number, int direction, const Light * light, Camera * pSceneCamera)
{
    LIGHT_TYPE type = light->GetType();

    if (type == LIGHTTYPE_DIRECTIONAL)
    {
        Vec3 right, up, dir;

        dir = light->GetDirection();
        up = Vec3::UnitY;

        if (Math::Abs(Math::VecDot(dir, up)) >= 1.0f)
        {
            up = Vec3::UnitZ;
        }

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

        float s0, s1;
        s0 = m_split[number];
        s1 = m_split[number + 1];
        Vec3 vs0(0, 0, s0);
        Vec3 vs1(0, 0, s1);
        Math::VecTransform(vs0, vs0, pSceneCamera->GetProjMatrix());
        Math::VecTransform(vs1, vs1, pSceneCamera->GetProjMatrix());
        s0 = vs0.z;
        s1 = vs1.z;

        p[0] = Vec3(-1, 1, s0);
        p[1] = Vec3(-1, 1, s1);
        p[2] = Vec3(1, 1, s0);
        p[3] = Vec3(1, 1, s1);
        p[4] = Vec3(-1, -1, s0);
        p[5] = Vec3(-1, -1, s1);
        p[6] = Vec3(1, -1, s0);
        p[7] = Vec3(1, -1, s1);

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

        //receiver camera
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


void ShadowRenderPSSM::RenderShadow(Camera * cam, Light * light)
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
        for (int i = 0; i < m_texture_count; ++i)
        {
            if (UpdateCamera(m_cameras[i], i, dir, light, cam))
            {
                render->SetRenderTarget(0, m_render_targets[i].GetPointer());
                render->SetDepthStencil(m_depth_stencil.GetPointer());
                render->ClearBuffer(NULL, TRUE, TRUE, FALSE, Color::White, 1.0f, 0);

				render->SetLight(0, light);

				UpdateShadowCasterQueue(m_cameras[i], cam);
                RenderShadowCasterQueue(i);
            }
        }

        /*
        if (UpdateCamera(m_cameras[0], 0, dir, light, cam))
        {
            render->SetRenderTarget(0, m_render_targets[0].GetPointer());
            render->SetDepthStencil(m_depth_stencil.GetPointer());
            render->SetViewport(NULL);
            render->ClearBuffer(NULL, TRUE, TRUE, FALSE, Color::White, 1.0f, 0);

			render->SetLight(0, light);
			UpdateShadowCasterQueue(m_cameras[0], cam);
            RenderShadowCasterQueue(0);
        }
        */

        render->SetRenderTarget(0, m_shadowRT.GetPointer());
        render->SetDepthStencil(m_shadowDS.GetPointer());
        render->ClearBuffer(NULL, TRUE, TRUE, FALSE, Color::White, 1.0f, 0);

        float split[4] = { 0 };

        for (int i = 1; i < m_texture_count; ++i)
        {
            Math::VecTransformZ(split[i - 1], Vec3(0, 0, m_split[i]), cam->GetProjMatrix());
        }

        m_split_param->SetVector(Vec4(split[0], split[1], split[2], split[3]));

        UpdateShadowReceiverQueue();
        RenderShadowReceiverQueue();
    }
}

void ShadowRenderPSSM::SetSplitPoint(float * split, int size)
{
    d_assert(size == m_texture_count + 1);
    Memcpy(m_split, split, size * sizeof(float));
}

void ShadowRenderPSSM::SetSplitParamName(const String & name)
{
    Pass * pass = m_receiver_material->GetTechnique(0)->GetPass(0);
    m_split_param = pass->GetPixelShaderParamTable()->GetParam(name);
    d_assert (m_split_param);
}

void ShadowRenderPSSM::RenderShadowCasterQueue(int index)
{
    profile_code();

    RenderSystem * render = RenderSystem::Instance().InstancePtr();

    render->SetShadowViewTransform(0, m_cameras[index]->GetViewMatrix());
    render->SetShadowProjTransform(0, m_cameras[index]->GetProjMatrix());

    RenderQueue::Instance().Render(0);
}

void ShadowRenderPSSM::RenderShadowReceiverQueue()
{
    profile_code();

    RenderSystem * render = RenderSystem::InstancePtr();

    int t = 0;

    for (; t < m_texture_count; ++t)
    {
        render->SetShadowViewTransform(t, m_cameras[t]->GetViewMatrix());
        render->SetShadowProjTransform(t, m_cameras[t]->GetProjMatrix());

        m_render_targets[t]->Present();
        render->SetTexture(t, m_render_targets[t].GetPointer());
    }

    RenderQueue::Instance().Render(t);
}


void ShadowRenderPSSM::UpdateShadowCasterQueue(Camera * pShadowCamera, Camera * pSceneCamera)
{
    profile_code();

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

void ShadowRenderPSSM::UpdateShadowReceiverQueue()
{
    profile_code();

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




void ShadowRenderPSSM::SetDistance(float dist)
{
    m_distance = dist;
}

void ShadowRenderPSSM::SetNearClip(float clip)
{
    m_near_clip = clip;
}

void ShadowRenderPSSM::SetFarClip(float clip)
{
    mar_clip =clip;
}

void ShadowRenderPSSM::SetOffset(float offset)
{
    m_offset = offset;
}

float ShadowRenderPSSM::GetDistance() const
{
    return m_distance;
}

float ShadowRenderPSSM::GetNearClip() const
{
    return m_near_clip;
}

float ShadowRenderPSSM::GetFarClip() const
{
    return mar_clip;
}

float ShadowRenderPSSM::GetOffset() const
{
    return m_offset;
}

void ShadowRenderPSSM::SetBlurMaterial(const String & sBlurH, const String & sBlurV)
{
    m_blurH = MaterialManager::Instance().Find(sBlurH);

    if (m_blurH .IsNull())
        m_blurH  = MaterialManager::Instance().Load(sBlurH, sBlurH);

    m_blurH = MaterialManager::Instance().Find(sBlurH);

    if (m_blurV .IsNull())
        m_blurV  = MaterialManager::Instance().Load(sBlurV, sBlurV);
}

void ShadowRenderPSSM::SetBlurCount(int count)
{
    m_blur_count = count;
}

void ShadowRenderPSSM::NotifyShadowTexture()
{
    ShadowRender::NotifyShadowTexture();

    if (m_technique == SHADOW_TEXTURE)
    {
        if (IsUseBlur())
        {
            if (m_blurRT == NULL)
            {
                m_blurRT = VideoBufferManager::Instance().CreateRenderTarget(
                    "Sys_PSSM_Blur_RT", -1, -1, FMT_A8R8G8B8, MULTISAMPLE_NONE, TRUE);
            }
        }
        else
        {
            m_blurRT = NULL;
        }
    }
    else
    {
        m_blurRT = NULL;
    }
}

bool ShadowRenderPSSM::IsUseBlur() const
{
    return m_blur_count > 0 && m_blurH != NULL && m_blurV != NULL;
}

/*
--------------------------------------------------------------------------
                    i / m
	c_log = n (f/n)
    
    c_uniform = n + (f - n) i / m


	c_i = c_log + c_uniform
 	                i/m
              n(f/n)    + n + (f-n)i/m
        = ------------------------------ + bais
                         2
--------------------------------------------------------------------------
*/

/*
void ShadowRenderPSSM::UpdateSplit(Camera * cam)
{
    int m = m_ShadowCameras.Size();
    float f = cam->GetFarClip();
    float n = cam->GetNearClip();

    for (int i = 1; i < m; ++i)
    {
        float c_log = n * Math::Pow(f / n, i / (float)m);
        float c_uniform = m + (f - n) * (i / (float)m);
    }
}
*/