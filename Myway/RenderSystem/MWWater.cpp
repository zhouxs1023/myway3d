#include "MWWater.h"
#include "MWEnvironment.h"
#include "MWUnderWater.h"
#include "MWProjectionGrid.h"
#include "Engine.h"

namespace Myway {

    Water::Water()
    {
        _init();

        mScale = Vec3(400, 0, 400);
        mPosition = Vec3(400, 180, 400);
        mPlane = Plane(Vec3(0, 1, 0), mPosition);

        mFog = new UnderWaterFog();
        mNoise = new UnderWaterNoise();
        mGodRay = new UnderWaterGodRay();
        mCaustics = new UnderWaterCaustics();
        mBubble = new UnderWaterBubble();

        mProjGrid = new ProjectedGrid(mPlane);

        RenderEvent::OnPreRenderEvent += this;
    }

    Water::~Water()
    {
        RenderEvent::OnPreRenderEvent -= this;

        delete mFog;
        delete mNoise;
        delete mGodRay;
        delete mCaustics;
        delete mBubble;
        delete mProjGrid;
    }

    void Water::OnCall(Event * sender, void * data)
    {
        if (sender == &RenderEvent::OnPreRenderEvent)
            _renderRelfection();
    }

    bool Water::IsUnderWater()
    {
        Camera * cam = World::Instance()->MainCamera();

        const Vec3 & pos = cam->GetPosition();

        return pos.y < 180;
    }


    void Water::Render(Texture * depthTex, Texture * colorTex)
    {
        if (IsUnderWater())
            _renderUnderWater(depthTex, colorTex);
        else
            _renderUpWater(depthTex, colorTex);
    }

    void Water::RenderUnderFog(Texture * depthTex, Texture * colorTex)
    {
        if (IsUnderWater())
            mFog->Render(depthTex, colorTex);
    }

    void Water::RenderUnderNoise(Texture * colorTex)
    {
        if (IsUnderWater())
            mNoise->Render(colorTex);
    }

    void Water::RenderUnderGodRay()
    {
        if (IsUnderWater())
            mGodRay->Render(this);
    }

    void Water::RenderUnderCaustics(Texture * depthTex, Texture * colorTex)
    {
        if (IsUnderWater())
            mCaustics->Render(colorTex, depthTex);
    }

    void Water::RenderUnderBubble()
    {
        if (IsUnderWater())
            mBubble->Render();
    }

    void Water::_renderUpWater(Texture * depthTex, Texture * colorTex)
    {
        float elapsedTime = Engine::Instance()->GetFrameTime();

        mProjGrid->Update(elapsedTime);

        RenderSystem * render = RenderSystem::Instance();
        Camera * cam = World::Instance()->MainCamera();
        float time = Engine::Instance()->GetTime();

        const Vec3 * corner = cam->GetCorner();

        Vec3 cornerLeftTop = corner[4];
        Vec3 cornerRightDir = corner[5] - corner[4];
        Vec3 cornerDownDir = corner[6] - corner[4];

        ShaderParam * uPosition = mTech->GetVertexShaderParamTable()->GetParam("gPosition");
        ShaderParam * uScale = mTech->GetVertexShaderParamTable()->GetParam("gScale");

        ShaderParam * uTintColor = mTech->GetPixelShaderParamTable()->GetParam("gTintColor");
        ShaderParam * uFresnelParam = mTech->GetPixelShaderParamTable()->GetParam("gFresnelParam");
        ShaderParam * uCamPos = mTech->GetPixelShaderParamTable()->GetParam("gCameraPos");
        ShaderParam * uCornerLeftTop = mTech->GetPixelShaderParamTable()->GetParam("gCornerLeftTop");
        ShaderParam * uCornerRightDir = mTech->GetPixelShaderParamTable()->GetParam("gCornerRightDir");
        ShaderParam * uCornerDownDir = mTech->GetPixelShaderParamTable()->GetParam("gCornerDownDir");
        ShaderParam * uDeepColor = mTech->GetPixelShaderParamTable()->GetParam("gDeepColor");
        ShaderParam * uRefractionDist = mTech->GetPixelShaderParamTable()->GetParam("gRefractionDist");

        ShaderParam * uSunDir = mTech->GetPixelShaderParamTable()->GetParam("gSunDir");
        ShaderParam * uSunColor = mTech->GetPixelShaderParamTable()->GetParam("gSunColor");
        ShaderParam * uSunLightParam = mTech->GetPixelShaderParamTable()->GetParam("gSunLightParam");

        ShaderParam * uNoiseScale = mTech->GetPixelShaderParamTable()->GetParam("gNoiseScale");
        ShaderParam * uNoiseUVParam = mTech->GetPixelShaderParamTable()->GetParam("gNoiseUVParam");

        ShaderParam * uNormalUVParam = mTech->GetPixelShaderParamTable()->GetParam("gNormalUVParam");
        ShaderParam * uNormalWeight = mTech->GetPixelShaderParamTable()->GetParam("gNormalWeight");

        Vec3 camPos = cam->GetPosition();
        Vec3 sunDir = Environment::Instance()->GetEvParam()->SunDir;
        Color4 sunColor = Environment::Instance()->GetEvParam()->SunColor;
        Color4 deepColor = Environment::Instance()->GetEvParam()->WaterParam.deepColor;

        if (mProjGrid)
        {
            uPosition->SetUnifom(0, 0, 0, 0);
            uScale->SetUnifom(1, 1, 1, 1);
        }
        else
        {
            uPosition->SetUnifom(mPosition.x, mPosition.y, mPosition.z, 0);
            uScale->SetUnifom(mScale.x, 1, mScale.z, 1);
        }
        
        uCamPos->SetUnifom(camPos.x, camPos.y, camPos.z, 0);

        uTintColor->SetUnifom(0.0f, 0.05f, 0.05f, 1);
        //uFresnelParam->SetUnifom(-0.15f, 1.0f, 1.5f, 0);

        uCornerLeftTop->SetUnifom(cornerLeftTop.x, cornerLeftTop.y, cornerLeftTop.z, 0);
        uCornerRightDir->SetUnifom(cornerRightDir.x, cornerRightDir.y, cornerRightDir.z, 0);
        uCornerDownDir->SetUnifom(cornerDownDir.x, cornerDownDir.y, cornerDownDir.z, 0);

        uDeepColor->SetUnifom(deepColor.r, deepColor.g, deepColor.b, 1);
        uRefractionDist->SetUnifom(40, 0, 0, 0);

        uSunColor->SetUnifom(sunColor.r, sunColor.g, sunColor.b, 0);
        uSunDir->SetUnifom(-sunDir.x, -sunDir.y, sunDir.z, 0);
        uSunLightParam->SetUnifom(0.8f, 200, 0, 0);


        float uvNoiseScroll = time * 0.02f;

        uNoiseUVParam->SetUnifom(0.01f, 0.01f, uvNoiseScroll, uvNoiseScroll);
        uNoiseScale->SetUnifom(0.03f, 0, 0 , 0);

        Vec4 uvNormalParam[4] = {
            Vec4 (0.02f, 0.02f, time * 0.03f, time * 0.03f),
            Vec4 (0.01f, 0.01f, time * 0.03f, time * 0.03f),
            Vec4 (0.015f, 0.015f, time * 0.03f, time * 0.03f),
            Vec4 (0.005f, 0.005f, time * 0.03f, time * 0.03f)
        };

        uNormalUVParam->SetVector(uvNormalParam, 4);
        uNormalWeight->SetUnifom(0.08f, 0.1f, 0.06f, 0.1f);

        SamplerState state;
        render->SetTexture(0, state, mTex_Wave.c_ptr());
        state.Address = TEXA_CLAMP;
        state.Filter = TEXF_POINT;
        render->SetTexture(1, state, depthTex);
        render->SetTexture(2, state, colorTex);
        render->SetTexture(3, state, mTex_Refl.c_ptr());
        render->SetTexture(4, state, mTex_Fresnel.c_ptr());

        SamplerState state1;
        render->SetTexture(5, state1, mTex_Wave.c_ptr());
        render->SetTexture(6, state1, mTex_Wave.c_ptr());

        RenderDesc * rd = mProjGrid->GetRender();

        render->Render(mTech, rd);
    }

    void Water::_renderUnderWater(Texture * depthTex, Texture * colorTex)
    {
        Camera * cam = World::Instance()->MainCamera();
        RenderSystem * render = RenderSystem::Instance();
        float time = Engine::Instance()->GetTime();

        ShaderParam * uPosition = mTech_UnderWater->GetVertexShaderParamTable()->GetParam("gPosition");
        ShaderParam * uScale = mTech_UnderWater->GetVertexShaderParamTable()->GetParam("gScale");
        ShaderParam * uUVParam = mTech_UnderWater->GetVertexShaderParamTable()->GetParam("gUVParam");

        ShaderParam * uTintColor = mTech_UnderWater->GetPixelShaderParamTable()->GetParam("gTintColor");
        ShaderParam * uNoiseScale = mTech_UnderWater->GetPixelShaderParamTable()->GetParam("gNoiseScale");
        ShaderParam * uFogParam = mTech_UnderWater->GetPixelShaderParamTable()->GetParam("gFogParam");
        ShaderParam * uFogColor = mTech_UnderWater->GetPixelShaderParamTable()->GetParam("gFogColor");
        ShaderParam * uCameraPos = mTech_UnderWater->GetPixelShaderParamTable()->GetParam("gCameraPos");

        float uvScroll = time * 0.02f;
        float fogStart = Environment::Instance()->GetEvParam()->WaterParam.fogStart;
        float fogEnd = Environment::Instance()->GetEvParam()->WaterParam.fogEnd;
        Color4 fogColor = Environment::Instance()->GetEvParam()->WaterParam.fogColor;

        Vec3 camPos = cam->GetPosition();

        uPosition->SetUnifom(mPosition.x, mPosition.y, mPosition.z, 0);
        uScale->SetUnifom(mScale.x, 1, mScale.z, 1);
        uUVParam->SetUnifom(0.01f, 0.01f, uvScroll, uvScroll);

        uTintColor->SetUnifom(0.1f, 0.1f, 0.3f, 1);
        uNoiseScale->SetUnifom(0.08f, 0, 0 , 0);
        uFogParam->SetUnifom(fogStart, fogEnd, 1 / (fogEnd - fogStart), 0.3f);
        uFogColor->SetUnifom(fogColor.r, fogColor.g, fogColor.b, 0);
        uCameraPos->SetUnifom(camPos.x, camPos.y, camPos.z, 0);

        SamplerState state;
        render->SetTexture(0, state, mTex_Wave.c_ptr());
        state.Address = TEXA_CLAMP;
        render->SetTexture(1, state, colorTex);

        render->Render(mTech_UnderWater, &mRender);
    }

    void Water::_init()
    {
        mTech = Environment::Instance()->GetShaderLib()->GetTechnique("Water");
        mTech_UnderWater = Environment::Instance()->GetShaderLib()->GetTechnique("UnderWater");
        mTex_Wave = VideoBufferManager::Instance()->Load2DTexture("waves2.dds", "waves2.dds");
        mTex_Fresnel = VideoBufferManager::Instance()->Load2DTexture("Fresnel.bmp", "Fresnel.bmp");
        mTex_Normal0 = VideoBufferManager::Instance()->Load2DTexture("WaterNormal1.tga", "WaterNormal1.tga");
        mTex_Normal1 = VideoBufferManager::Instance()->Load2DTexture("WaterNormal2.tga", "WaterNormal2.tga");

        d_assert (mTech != NULL && mTex_Normal0 != NULL && mTech_UnderWater != NULL);

        VertexStream * vxStream = &mRender.vxStream;
        IndexStream * ixStream = &mRender.ixStream;

        int xSegments = 1;
        int zSegments = 1;

        int iVertexCount = 4;
        int iPrimCount = 2;
        int iStride = 24;

        VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
        decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
        decl->AddElement(0, 12, DT_FLOAT3, DU_TEXCOORD, 0);
        decl->Init();

        vxStream->SetDeclaration(decl);

        VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * iStride);

        float * vert = (float *)vb->Lock(0, 0, LOCK_NORMAL);

        {
            *vert++ = -1; *vert++ = 0; *vert++ =  1; *vert++ = 0; *vert++ = 1; *vert++ = 0;
            *vert++ =  1; *vert++ = 0; *vert++ =  1; *vert++ = 0; *vert++ = 1; *vert++ = 0;
            *vert++ = -1; *vert++ = 0; *vert++ = -1; *vert++ = 0; *vert++ = 1; *vert++ = 0;
            *vert++ =  1; *vert++ = 0; *vert++ = -1; *vert++ = 0; *vert++ = 1; *vert++ = 0;
        }

        vb->Unlock();

        vxStream->Bind(0,vb, iStride);
        vxStream->SetCount(iVertexCount);

        mRender.iPrimCount = iPrimCount;
        mRender.ePrimType = PRIM_TRIANGLESTRIP;
        mRender.rState.blendMode = BM_OPATICY;
        mRender.rState.cullMode = CULL_NONE;
        mRender.rState.fillMode = FILL_SOLID;
        mRender.rState.depthWrite = false;
        mRender.rState.depthCheck = DCM_LESS_EQUAL;


        mCamera = World::Instance()->CreateCamera("Core_Water_Refl");
        mCameraNode = World::Instance()->CreateSceneNode("Core_Water_Refl_Node");
        mCameraNode->Attach(mCamera);

        mRT_Refl = VideoBufferManager::Instance()->CreateRenderTarget("Core_Water_RT", -1, -1, FMT_X8R8G8B8, MSAA_NONE);
        mDepthStencil = VideoBufferManager::Instance()->CreateDepthStencil("Core_Water_DS", -1, 01, FMT_D24S8, MSAA_NONE);
        mTex_Refl = VideoBufferManager::Instance()->CreateTextureRT("Core_Water_Tex", -1, -1);
    }

    void Water::Update()
    {
    }

    void Water::_renderRelfection()
    {
        Camera * mainCam = World::Instance()->MainCamera();

        const Vec3 & pos = mainCam->GetPosition();
        const Quat & ort = mainCam->GetOrientation();
     
        mCameraNode->SetPosition(pos);
        mCameraNode->SetOrientation(ort);

        Plane mirrorPlane = Plane(Vec3(0, 1, 0), mPosition);

        mCamera->SetMirrorPlane(mirrorPlane);
        mCamera->EnableMirror(true);
        mCamera->SetNearClip(mainCam->GetNearClip());
        mCamera->SetFarClip(mainCam->GetFarClip());
        mCamera->SetAspect(mainCam->GetAspect());
        mCamera->SetFov(mainCam->GetFov());

        Vec3 p0 = mPosition;
        Vec3 p1 = p0 + Vec3(1, 0, 0);
        Vec3 p2 = p0 + Vec3(0, 0, 1);

        mCullResult.lights.Clear();
        mCullResult.nodes.Clear();

        World::Instance()->ImpVisibleCull(mCullResult, mainCam, false);

        mRenderQueue.Clear();
        mRenderQueue.PushRenderer(mCullResult.nodes);

        RenderSystem * render = RenderSystem::Instance();
        RenderQueue * rq = &mRenderQueue;

        RenderTarget * oldRT = render->GetRenderTarget(0);
        DepthStencil * oldDS = render->GetDepthStencil();

        render->SetViewTransform(mCamera->GetViewMatrix());
        render->SetProjTransform(mCamera->GetProjMatrix());

        RenderRegister::Instance()->SetClipPlane(mCamera->GetNearClip(), mCamera->GetFarClip());


        // ---> Bind RenderTarget
        render->SetRenderTarget(0, mRT_Refl.c_ptr());
        render->SetRenderTarget(1, NULL);
        render->SetRenderTarget(2, NULL);
        render->SetRenderTarget(3, NULL);
        render->SetDepthStencil(mDepthStencil.c_ptr());

        // ---> clear buffer
        render->ClearBuffer(NULL, true, true, false, Color::Black, 1, 0);

        // --->render terrain
        /*if (Environment::Instance()->GetTerrain())
            Environment::Instance()->GetTerrain()->RenderInMirror();*/

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
        //if (Environment::Instance()->GetCloud())
            //Environment::Instance()->GetCloud()->Render(false);

        mRT_Refl->Stretch(mTex_Refl.c_ptr());

        render->SetRenderTarget(0, oldRT);
        render->SetDepthStencil(oldDS);
    }
}