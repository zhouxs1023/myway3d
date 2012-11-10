#include "MWHDR.h"
#include "Engine.h"

namespace Myway {

    HDRLighting::HDRLighting()
    {
        _initRT();
        _initTech();

        RenderEvent::OnResize += this;
    }

    HDRLighting::~HDRLighting()
    {
        _deinitRT();
        _deinitTech();

        RenderEvent::OnResize -= this;
    }

    void HDRLighting::OnCall(Event * sender, void * data)
    {
        if (sender == &RenderEvent::OnResize)
        {
            _initRT();
        }
    }


    void HDRLighting::Render(Texture * sceneTex)
    {
        RenderSystem * render = RenderSystem::Instance();

        RenderTarget * oldRt = render->GetRenderTarget(0);

        _downScale_clr(sceneTex);
        _lumInitial();
        _lumDownScale_16x();
        _lumDownScale_4x();
        _lumFinal();
        _lumAdapted();
        _bright();

        render->SetRenderTarget(0, oldRt);

        _toneMap(sceneTex);
    }

    void HDRLighting::_initRT()
    {
        _deinitRT();

        const DeviceProperty * dp = Engine::Instance()->GetDeviceProperty();

        int width = dp->Width / 2;
        int height = dp->Height / 2;

        VideoBufferManager * video = VideoBufferManager::Instance();

        mRT_Quad = video->CreateRenderTarget("HDR_RT_Quad", width, height, FMT_A16B16G16R16F, MSAA_NONE);
        mRT_Bloom0 = video->CreateRenderTarget("HDR_RT_Bloom0", width, height, FMT_A8R8G8B8, MSAA_NONE);
        mRT_Bloom1 = video->CreateRenderTarget("HDR_RT_Bloom1", width / 2, height / 2, FMT_A8R8G8B8, MSAA_NONE);
        mRT_Bloom2 = video->CreateRenderTarget("HDR_RT_Bloom2", width / 4, height / 4, FMT_A8R8G8B8, MSAA_NONE);
        mRT_64x64 = video->CreateRenderTarget("HDR_RT_64x", 64, 64, FMT_R16F, MSAA_NONE);
        mRT_16x16 = video->CreateRenderTarget("HDR_RT_16x", 16, 16, FMT_R16F, MSAA_NONE);
        mRT_4x4 = video->CreateRenderTarget("HDR_RT_4x", 4, 4, FMT_R16F, MSAA_NONE);
        mRT_1x1 = video->CreateRenderTarget("HDR_RT_1x", 1, 1, FMT_R16F, MSAA_NONE);

        mTex_Quad = video->CreateTextureRT("HDR_Tex_Quad", width, height, FMT_A16B16G16R16F);
        mTex_Bloom0 = video->CreateTextureRT("HDR_Tex_Bloom0", width, height, FMT_A8R8G8B8);
        mTex_Bloom1 = video->CreateTextureRT("HDR_Tex_Bloom1", width / 2, height / 2, FMT_A8R8G8B8);
        mTex_Bloom2 = video->CreateTextureRT("HDR_Tex_Bloom2", width / 4, height / 4, FMT_A8R8G8B8);
        mTex_64x64 = video->CreateTextureRT("HDR_Tex_64x", 64, 64, FMT_R16F);
        mTex_16x16 = video->CreateTextureRT("HDR_Tex_16x", 16, 16, FMT_R16F);
        mTex_4x4 = video->CreateTextureRT("HDR_Tex_4x", 4, 4, FMT_R16F);
        mTex_1x1_0 = video->CreateTextureRT("HDR_Tex_1x_0", 1, 1, FMT_R16F);
        mTex_1x1_1 = video->CreateTextureRT("HDR_Tex_1x_1", 1, 1, FMT_R16F);
    }

    void HDRLighting::_initTech()
    {
        mTech_DownScale2x = Environment::Instance()->GetShaderLib()->GetTechnique("DownScale2x");
        mTech_DownScale4x = Environment::Instance()->GetShaderLib()->GetTechnique("DownScale4x");
        mTech_LumInitial = Environment::Instance()->GetShaderLib()->GetTechnique("HDR_LumInitial");
        mTech_LumFinal = Environment::Instance()->GetShaderLib()->GetTechnique("HDR_LumFinal");
        mTech_Adapted = Environment::Instance()->GetShaderLib()->GetTechnique("HDR_Adapted");
        mTech_Bright = Environment::Instance()->GetShaderLib()->GetTechnique("HDR_Bright");
        mTech_BlurH = Environment::Instance()->GetShaderLib()->GetTechnique("BlurH");
        mTech_BlurV = Environment::Instance()->GetShaderLib()->GetTechnique("BlurV");
        mTech_ToneMap = Environment::Instance()->GetShaderLib()->GetTechnique("HDR_ToneMap");

        d_assert (mTech_DownScale2x != NULL && mTech_DownScale4x != NULL );
        d_assert (mTech_LumInitial != NULL  && mTech_LumFinal != NULL );
        d_assert (mTech_Adapted != NULL  && mTech_Bright != NULL && mTech_ToneMap != NULL );
        d_assert (mTech_BlurH != NULL  && mTech_BlurV != NULL );
    }

    void HDRLighting::_deinitRT()
    {
        mRT_Quad = NULL;
        mRT_Bloom0 = NULL;
        mRT_Bloom1 = NULL;
        mRT_Bloom2 = NULL;
        mRT_64x64 = NULL;
        mRT_16x16 = NULL;
        mRT_4x4 = NULL;
        mRT_1x1 = NULL;

        mTex_Quad = NULL; 
        mTex_Bloom0 = NULL; 
        mTex_Bloom1 = NULL; 
        mTex_Bloom2 = NULL; 
        mTex_64x64 = NULL;

        mTex_16x16 = NULL;
        mTex_4x4 = NULL;
        mTex_1x1_0 = NULL;
        mTex_1x1_1 = NULL;
    }

    void HDRLighting::_deinitTech()
    {
        mTech_DownScale2x = NULL;
        mTech_DownScale4x = NULL;
        mTech_LumInitial = NULL;
        mTech_LumFinal = NULL;
        mTech_Adapted = NULL;
        mTech_Bright = NULL;
        mTech_BlurH = NULL;
        mTech_BlurV = NULL;
        mTech_ToneMap = NULL;
    }

    void HDRLighting::_downScale_clr(Texture * sceneTex)
    {
        RenderSystem * render = RenderSystem::Instance();

        render->SetRenderTarget(0, mRT_Quad.c_ptr());

        float invWidth = 1.0f / mRT_Quad->GetWidth();
        float invHeight = 1.0f / mRT_Quad->GetHeight();

        Vec4 uvOffs[4] = {
            Vec4(0, 0, 0, 0), Vec4(invWidth, 0, 0, 0),
            Vec4(0, invHeight, 0, 0), Vec4(invWidth, invHeight, 0, 0),
        };

        ShaderParam * uUVOffs = mTech_DownScale2x->GetPixelShaderParamTable()->GetParam("gUVOffsets");
        uUVOffs->SetVector(uvOffs, 4);

        SamplerState state;
        render->SetTexture(0, state, sceneTex);
        
        RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_DownScale2x);

        mRT_Quad->Stretch(mTex_Quad.c_ptr());
    }

    void HDRLighting::_lumInitial()
    {
        RenderSystem * render = RenderSystem::Instance();

        render->SetRenderTarget(0, mRT_64x64.c_ptr());

        Vec4 uvOffs[9] = {
            Vec4(0, 0, 0, 0), Vec4(1, 0, 0, 0), Vec4(2, 0, 0, 0),
            Vec4(0, 1, 0, 0), Vec4(1, 1, 0, 0), Vec4(2, 1, 0, 0),
            Vec4(0, 2, 0, 0), Vec4(1, 2, 0, 0), Vec4(2, 2, 0, 0)
        };

        float invW = 1.0f / mTex_Quad->GetWidth();
        float invH = 1.0f / mTex_Quad->GetHeight();

        for (int i = 0; i < 9; ++i)
        {
            uvOffs[i] *= Vec4(invW, invH, 0, 0);
        }

        ShaderParam * uUVOffs = mTech_LumInitial->GetPixelShaderParamTable()->GetParam("gUVOffsets");

        uUVOffs->SetVector(uvOffs, 9);

        SamplerState state;
        render->SetTexture(0, state, mTex_Quad.c_ptr());

        RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_LumInitial);

        mRT_64x64->Stretch(mTex_64x64.c_ptr());
    }

    void HDRLighting::_lumDownScale_16x()
    {
        RenderSystem * render = RenderSystem::Instance();

        render->SetRenderTarget(0, mRT_16x16.c_ptr());

        Vec4 uvOffs[16] = {
            Vec4(0, 0, 0, 0), Vec4(1, 0, 0, 0), Vec4(2, 0, 0, 0), Vec4(3, 0, 0, 0),
            Vec4(0, 1, 0, 0), Vec4(1, 1, 0, 0), Vec4(2, 1, 0, 0), Vec4(3, 1, 0, 0),
            Vec4(0, 2, 0, 0), Vec4(1, 2, 0, 0), Vec4(2, 2, 0, 0), Vec4(3, 2, 0, 0),
            Vec4(0, 3, 0, 0), Vec4(1, 3, 0, 0), Vec4(2, 3, 0, 0), Vec4(3, 3, 0, 0)
        };

        for (int i = 0; i < 16; ++i)
        {
            uvOffs[i] *= 1 / 64.0f;
        }

        ShaderParam * uUVOffs = mTech_DownScale4x->GetPixelShaderParamTable()->GetParam("gUVOffsets");

        uUVOffs->SetVector(uvOffs, 16);

        SamplerState state;
        render->SetTexture(0, state, mTex_64x64.c_ptr());

        RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_DownScale4x);

        mRT_16x16->Stretch(mTex_16x16.c_ptr());
    }

    void HDRLighting::_lumDownScale_4x()
    {
        RenderSystem * render = RenderSystem::Instance();

        render->SetRenderTarget(0, mRT_4x4.c_ptr());

        float invWidth = 1.0f / mRT_Quad->GetWidth();

        Vec4 uvOffs[4] = {
            Vec4(0, 0, 0, 0), Vec4(1, 0, 0, 0), 
            Vec4(0, 1, 0, 0), Vec4(1, 1, 0, 0)
        };

        for (int i = 0; i < 4; ++i)
        {
            uvOffs[i] *= 1 / 16.0f;
        }

        ShaderParam * uUVOffs = mTech_DownScale2x->GetPixelShaderParamTable()->GetParam("gUVOffsets");

        uUVOffs->SetVector(uvOffs, 4);

        SamplerState state;
        render->SetTexture(0, state, mTex_16x16.c_ptr());

        RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_DownScale4x);

        mRT_4x4->Stretch(mTex_4x4.c_ptr());
    }

    void HDRLighting::_lumFinal()
    {
        RenderSystem * render = RenderSystem::Instance();

        render->SetRenderTarget(0, mRT_1x1.c_ptr());

        Vec4 uvOffs[16] = {
            Vec4(0, 0, 0, 0), Vec4(1, 0, 0, 0), Vec4(2, 0, 0, 0), Vec4(3, 0, 0, 0),
            Vec4(0, 1, 0, 0), Vec4(1, 1, 0, 0), Vec4(2, 1, 0, 0), Vec4(3, 1, 0, 0),
            Vec4(0, 2, 0, 0), Vec4(1, 2, 0, 0), Vec4(2, 2, 0, 0), Vec4(3, 2, 0, 0),
            Vec4(0, 3, 0, 0), Vec4(1, 3, 0, 0), Vec4(2, 3, 0, 0), Vec4(3, 3, 0, 0)
        };

        for (int i = 0; i < 16; ++i)
        {
            uvOffs[i] *= 1 / 4.0f;
        }

        ShaderParam * uUVOffs = mTech_LumFinal->GetPixelShaderParamTable()->GetParam("gUVOffsets");

        uUVOffs->SetVector(uvOffs, 16);

        SamplerState state;
        render->SetTexture(0, state, mTex_4x4.c_ptr());

        RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_LumFinal);

        mRT_1x1->Stretch(mTex_1x1_1.c_ptr());
    }

    void HDRLighting::_lumAdapted()
    {
        RenderSystem * render = RenderSystem::Instance();

        render->SetRenderTarget(0, mRT_1x1.c_ptr());

        float elapsedTime = Engine::Instance()->GetFrameTime();
        
        ShaderParam * uElapsedTime = mTech_Adapted->GetPixelShaderParamTable()->GetParam("gElapsedTime");

        uElapsedTime->SetUnifom(elapsedTime, 0, 0, 0);

        SamplerState state;
        render->SetTexture(0, state, mTex_1x1_0.c_ptr());
        render->SetTexture(1, state, mTex_1x1_1.c_ptr());

        RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_Adapted);

        mRT_1x1->Stretch(mTex_1x1_0.c_ptr());
    }

    void HDRLighting::_bright()
    {
        RenderSystem * render = RenderSystem::Instance();

        // bright
        {
            render->SetRenderTarget(0, mRT_Bloom0.c_ptr());

            float threshold = Environment::Instance()->GetEvParam()->HdrParam.threshold;

            ShaderParam * uThreshold = mTech_Bright->GetPixelShaderParamTable()->GetParam("gThreshold");

            uThreshold->SetUnifom(threshold, 0, 0, 0);

            SamplerState state;
            state.Address = TEXA_CLAMP;
            render->SetTexture(0, state, mTex_Quad.c_ptr());

            RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_Bright);

            mRT_Bloom0->Stretch(mTex_Bloom0.c_ptr());
        }

        _blurH(mRT_Bloom0.c_ptr(), mTex_Bloom0.c_ptr());
        _blurV(mRT_Bloom0.c_ptr(), mTex_Bloom0.c_ptr());

        // down scale 
        {
            render->SetRenderTarget(0, mRT_Bloom1.c_ptr());

            float invWidth = 1.0f / mTex_Bloom0->GetWidth();
            float invHeight = 1.0f / mTex_Bloom0->GetHeight();

            Vec4 uvOffs[4] = {
                Vec4(0, 0, 0, 0), Vec4(1, 0, 0, 0), 
                Vec4(0, 1, 0, 0), Vec4(1, 1, 0, 0)
            };

            for (int i = 0; i < 4; ++i)
            {
                uvOffs[i].x *= invWidth;
                uvOffs[i].y *= invHeight;
            }

            ShaderParam * uUVOffs = mTech_DownScale2x->GetPixelShaderParamTable()->GetParam("gUVOffsets");

            uUVOffs->SetVector(uvOffs, 4);

            SamplerState state;
            state.Address = TEXA_CLAMP;
            render->SetTexture(0, state, mTex_Bloom0.c_ptr());

            RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_DownScale2x);

            mRT_Bloom1->Stretch(mTex_Bloom1.c_ptr());
        }

        _blurH(mRT_Bloom1.c_ptr(), mTex_Bloom1.c_ptr());
        _blurV(mRT_Bloom1.c_ptr(), mTex_Bloom1.c_ptr());

        // down scale 
        {
            render->SetRenderTarget(0, mRT_Bloom2.c_ptr());

            float invWidth = 1.0f / mTex_Bloom1->GetWidth();
            float invHeight = 1.0f / mTex_Bloom1->GetHeight();

            Vec4 uvOffs[4] = {
                Vec4(0, 0, 0, 0), Vec4(1, 0, 0, 0), 
                Vec4(0, 1, 0, 0), Vec4(1, 1, 0, 0)
            };

            for (int i = 0; i < 4; ++i)
            {
                uvOffs[i].x *= invWidth;
                uvOffs[i].y *= invHeight;
            }

            ShaderParam * uUVOffs = mTech_DownScale2x->GetPixelShaderParamTable()->GetParam("gUVOffsets");

            uUVOffs->SetVector(uvOffs, 4);

            SamplerState state;
            state.Address = TEXA_CLAMP;
            render->SetTexture(0, state, mTex_Bloom1.c_ptr());

            RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_DownScale2x);

            mRT_Bloom2->Stretch(mTex_Bloom2.c_ptr());
        }

        _blurH(mRT_Bloom2.c_ptr(), mTex_Bloom2.c_ptr());
        _blurV(mRT_Bloom2.c_ptr(), mTex_Bloom2.c_ptr());
    }

    void HDRLighting::_blurH(RenderTarget * rt, Texture * tex)
    {
        RenderSystem * render = RenderSystem::Instance();

        render->SetRenderTarget(0, rt);

        SamplerState state;
        state.Address = TEXA_CLAMP;
        render->SetTexture(0, state, tex);

        RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_BlurH);

        rt->Stretch(tex);
    }

    void HDRLighting::_blurV(RenderTarget * rt, Texture * tex)
    {
        RenderSystem * render = RenderSystem::Instance();

        render->SetRenderTarget(0, rt);

        SamplerState state;
        state.Address = TEXA_CLAMP;
        render->SetTexture(0, state, tex);

        RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_BlurV);

        rt->Stretch(tex);
    }

    void HDRLighting::_toneMap(Texture * sceneTex)
    {
        RenderSystem * render = RenderSystem::Instance();

        float exposure = Environment::Instance()->GetEvParam()->HdrParam.exposure;
        float weight0 = Environment::Instance()->GetEvParam()->HdrParam.weight0;
        float weight1 = Environment::Instance()->GetEvParam()->HdrParam.weight1;
        float weight2 = Environment::Instance()->GetEvParam()->HdrParam.weight2;

        ShaderParam * uBloomWeight = mTech_ToneMap->GetPixelShaderParamTable()->GetParam("gBloomWeight");
        ShaderParam * uExposure = mTech_ToneMap->GetPixelShaderParamTable()->GetParam("gExposure");

        uBloomWeight->SetUnifom(weight0, weight1, weight2, 0);
        uExposure->SetUnifom(exposure, 0, 0, 0);

        SamplerState state;
        state.Address = TEXA_CLAMP;
        render->SetTexture(0, state, mTex_1x1_0.c_ptr());
        render->SetTexture(1, state, sceneTex);
        render->SetTexture(2, state, mTex_Bloom0.c_ptr());
        render->SetTexture(3, state, mTex_Bloom1.c_ptr());
        render->SetTexture(4, state, mTex_Bloom2.c_ptr());

        RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_ToneMap);
    }
}