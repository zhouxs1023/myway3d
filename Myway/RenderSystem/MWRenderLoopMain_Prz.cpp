#include "MWRenderLoopMain_Prz.h"
#include "MWRenderScheme_Prz.h"
#include "MWEnvironment.h"
#include "Engine.h"

namespace Myway {

    RenderLoop_Main::RenderLoop_Main(RS_Scheme * sch)
    {
        mScheme = sch;

        const DeviceProperty * dp = Engine::Instance()->GetDeviceProperty();

        mRT_Color = VideoBufferManager::Instance()->CreateRenderTarget("Core_RT_Color", -1, -1, FMT_A16B16G16R16F,  MSAA_NONE);
        mRT_Normal = VideoBufferManager::Instance()->CreateRenderTarget("Core_RT_Normal", -1, -1, FMT_G32R32F,  MSAA_NONE);
        //mRT_Material = VideoBufferManager::Instance()->CreateRenderTarget("Core_RT_Material", -1, -1, FMT_A16B16G16R16F,  MSAA_NONE);
        mRT_Depth = VideoBufferManager::Instance()->CreateRenderTarget("Core_RT_Depth", -1, -1, FMT_G32R32F,  MSAA_NONE);

        mDepthStencil = VideoBufferManager::Instance()->CreateDepthStencil("Core_DepthStencil", -1, -1, FMT_D24S8,  MSAA_NONE);

        mTex_Color = VideoBufferManager::Instance()->CreateTextureRT("Core_TX_Color", -1, -1, FMT_A16B16G16R16F);
        mTex_Normal = VideoBufferManager::Instance()->CreateTextureRT("Core_TX_Normal", -1, -1, FMT_G32R32F);
        mTex_Material = VideoBufferManager::Instance()->CreateTextureRT("Core_TX_Material", -1, -1, FMT_A16B16G16R16F);
        mTex_Depth = VideoBufferManager::Instance()->CreateTextureRT("Core_TX_Depth", -1, -1, FMT_G32R32F);
    }

    RenderLoop_Main::~RenderLoop_Main()
    {
    }

    void RenderLoop_Main::DoRender()
    {
        RenderSystem * render = RenderSystem::Instance();
        RenderQueue * rq = mScheme->GetRenderQueue();

        Camera * cam = World::Instance()->MainCamera();

        render->SetViewTransform(cam->GetViewMatrix());
        render->SetProjTransform(cam->GetProjMatrix());

        RenderRegister::Instance()->SetClipPlane(cam->GetNearClip(), cam->GetFarClip());

        RenderTarget * finalRT = render->GetRenderTarget(0);

        // ---> Bind RenderTarget
        render->SetRenderTarget(0, mRT_Color.c_ptr());
        render->SetRenderTarget(1, mRT_Normal.c_ptr());
        render->SetRenderTarget(2, mRT_Material.c_ptr());
        render->SetRenderTarget(3, mRT_Depth.c_ptr());

        render->SetDepthStencil(mDepthStencil.c_ptr());

        // ---> clear buffer
        render->ClearBuffer(NULL, false, true, false, Color::Black, 1, 0);

        _clear();

		RenderEvent::OnBeginRender(NULL);
       
        // --->render terrain
        if (Environment::Instance()->GetTerrain())
            Environment::Instance()->GetTerrain()->Render();

        // --->render object
        _renderSolidObjects();

		RenderEvent::OnAfterRenderSolid(NULL);

        _updateTexture();

		render->SetRenderTarget(1, NULL);
		render->SetRenderTarget(2, NULL);
		render->SetRenderTarget(3, NULL);

        // --->sun lighting
        if (Environment::Instance()->GetSun())
            Environment::Instance()->GetSun()->Lighting(mTex_Color.c_ptr(), mTex_Normal.c_ptr());

        _updateColorTexture();

        // --->render fog
        if (Environment::Instance()->GetFog())
            Environment::Instance()->GetFog()->Render(mTex_Depth.c_ptr(), mTex_Color.c_ptr());

        // ---> render sky
        if (Environment::Instance()->GetSky2())
            Environment::Instance()->GetSky2()->Render();

        // ---> render sun
        if (Environment::Instance()->GetSun())
            Environment::Instance()->GetSun()->Render();

        // ---> render moons
        if (Environment::Instance()->GetMoon())
            Environment::Instance()->GetMoon()->Render();

        // ---> render cloud
        if (Environment::Instance()->GetCloud())
            Environment::Instance()->GetCloud()->Render();

        // ---> render godray
        if (Environment::Instance()->GetGodRay() && !Environment::Instance()->GetWater()->IsUnderWater())
            Environment::Instance()->GetGodRay()->Render(mTex_Depth.c_ptr());

       _updateColorTexture();

       // ---> render caustics
       if (Environment::Instance()->GetWater())
           Environment::Instance()->GetWater()->RenderUnderCaustics(mTex_Depth.c_ptr(), mTex_Color.c_ptr());

       _updateColorTexture();

       if (Environment::Instance()->GetWater())
           Environment::Instance()->GetWater()->RenderUnderFog(mTex_Depth.c_ptr(), mTex_Color.c_ptr());

       if (Environment::Instance()->GetWater())
           Environment::Instance()->GetWater()->RenderUnderBubble();

        // ---> render water
        if (Environment::Instance()->GetWater())
            Environment::Instance()->GetWater()->Render(mTex_Depth.c_ptr(), mTex_Color.c_ptr());

        _updateColorTexture();

        //
        if (Environment::Instance()->GetWater())
            Environment::Instance()->GetWater()->RenderUnderNoise(mTex_Color.c_ptr());

        if (Environment::Instance()->GetWater())
            Environment::Instance()->GetWater()->RenderUnderGodRay();

        _updateColorTexture();

        if (Environment::Instance()->GetHDR())
            Environment::Instance()->GetHDR()->Render(mTex_Color.c_ptr());

		RenderEvent::OnAfterDeffererShading(NULL);

		// render forward objects

		RenderEvent::OnAfterRender(NULL);

        _updateColorTexture();

        render->SetRenderTarget(0, finalRT);
        render->SetRenderTarget(1, NULL);
        render->SetRenderTarget(2, NULL);
        render->SetRenderTarget(3, NULL);

        _frush();
    }

    void RenderLoop_Main::_updateTexture()
    {
        mRT_Color->Stretch(mTex_Color.c_ptr());
        mRT_Normal->Stretch(mTex_Normal.c_ptr());
        //mRT_Material->Stretch(mTex_Material.c_ptr());
        mRT_Depth->Stretch(mTex_Depth.c_ptr());
    }

    void RenderLoop_Main::_updateColorTexture()
    {
        mRT_Color->Stretch(mTex_Color.c_ptr());
    }

    void RenderLoop_Main::_clear()
    {
        Technique * clearTech = mScheme->GetMainShaderProvider()->GetClearTech();
        RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, clearTech);
    }

    void RenderLoop_Main::_frush()
    {
        Technique * frushTech = mScheme->GetMainShaderProvider()->GetFrushTech();

        SamplerState state;
        state.Filter = TEXF_POINT;
        state.Address = TEXA_CLAMP;

        RenderSystem::Instance()->SetTexture(0, state, mTex_Color.c_ptr());
        RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, frushTech);
    }

    void RenderLoop_Main::_renderSolidObjects()
    {
        RenderSystem * render = RenderSystem::Instance();
        RenderQueue * rq = mScheme->GetRenderQueue();

        const Array<Renderer *> & objs = rq->GetSolidRender();
        Technique * tech = mScheme->GetMainShaderProvider()->GetTechnique(ShaderProvider_Main::R_Base);

        for (int i = 0; i < objs.Size(); ++i)
        {
            Renderer * rd = objs[i];

            render->Render(tech, rd);
        }

    }
}