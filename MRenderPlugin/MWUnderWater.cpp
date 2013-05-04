#include "MWUnderWater.h"
#include "MWEnvironment.h"
#include "MWRenderHelper.h"
#include "Engine.h"

namespace Myway {

    UnderWaterFog::UnderWaterFog()
    {
        mTech = Environment::Instance()->GetShaderLib()->GetTechnique("UnderWaterFog");
        d_assert (mTech);
    }

    UnderWaterFog::~UnderWaterFog()
    {
    }

    void UnderWaterFog::Render(Texture * depthTex, Texture * sceneTex)
    {
        float fogStart = Environment::Instance()->GetEvParam()->WaterParam.fogStart;
        float fogEnd = Environment::Instance()->GetEvParam()->WaterParam.fogEnd;
        Color4 fogColor = Environment::Instance()->GetEvParam()->WaterParam.fogColor;

        Camera * cam = World::Instance()->MainCamera();
        const Vec3 * corner = cam->GetCorner();

        Vec3 cornerLeftTop = corner[4];
        Vec3 cornerRightDir = corner[5] - corner[4];
        Vec3 cornerDownDir = corner[6] - corner[4];

        ShaderParam * uFogParam = mTech->GetPixelShaderParamTable()->GetParam("gFogParam");
        ShaderParam * uFogColor = mTech->GetPixelShaderParamTable()->GetParam("gFogColor");
        ShaderParam * uCornerLeftTop = mTech->GetPixelShaderParamTable()->GetParam("gCornerLeftTop");
        ShaderParam * uCornerRightDir = mTech->GetPixelShaderParamTable()->GetParam("gCornerRightDir");
        ShaderParam * uCornerDownDir = mTech->GetPixelShaderParamTable()->GetParam("gCornerDownDir");

        uFogParam->SetUnifom(fogStart, fogEnd, 1 / (fogEnd - fogStart), 0);
        uFogColor->SetUnifom(fogColor.r, fogColor.g, fogColor.b, 0);
        uCornerLeftTop->SetUnifom(cornerLeftTop.x, cornerLeftTop.y, cornerLeftTop.z, 0);
        uCornerRightDir->SetUnifom(cornerRightDir.x, cornerRightDir.y, cornerRightDir.z, 0);
        uCornerDownDir->SetUnifom(cornerDownDir.x, cornerDownDir.y, cornerDownDir.z, 0);

        SamplerState state;
        state.Address = TEXA_CLAMP;

        RenderSystem::Instance()->SetTexture(0, state, depthTex);
        RenderSystem::Instance()->SetTexture(1, state, sceneTex);

        RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech);
    }


    UnderWaterNoise::UnderWaterNoise()
    {
        mTech = Environment::Instance()->GetShaderLib()->GetTechnique("UnderWaterNoise");
        mTexture = VideoBufferManager::Instance()->Load2DTexture("Water\\UnderwaterDistortion.jpg", "Water\\UnderwaterDistortion.jpg");
        d_assert (mTech);
    }

    UnderWaterNoise::~UnderWaterNoise()
    {
    }

    void UnderWaterNoise::Render(Texture * sceneTex)
    {
        float time = Engine::Instance()->GetTime();
        ShaderParam * uUVParam = mTech->GetPixelShaderParamTable()->GetParam("gUVParam");

        uUVParam->SetUnifom(time * 0.05f, time * 0.05f, 0, 0);

        SamplerState state;
        RenderSystem::Instance()->SetTexture(0, state, mTexture.c_ptr());

        state.Address = TEXA_CLAMP;
        RenderSystem::Instance()->SetTexture(1, state, sceneTex);

        RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech);
    }














    UnderWaterGodRay::UnderWaterGodRay()
        : mPerlin(0)
        , mSimulationSpeed(5.0f)
        , mNumberOfRays(169)
        , mRaysSize(0.05f)
        , mNoiseDerivation(3)
        , mNoisePositionMultiplier(50)
        , mNoiseYNormalMultiplier(10)
        , mNoiseNormalMultiplier(0.175f)
        , mExposure(0.50f, 2.49f,2.29f)
        , mIntensity(0.005f)
    {
        _init();
    }

    UnderWaterGodRay::~UnderWaterGodRay()
    {
        delete mPerlin;
    }

    void UnderWaterGodRay::Render(const Vec3 & vWaterPosition)
    {
        mWaterPosition = vWaterPosition;

        _updateProjector();

        if (!mEnable)
             return ;

        float dtime = Engine::Instance()->GetFrameTime();
        mPerlin->update(dtime);

        _updateRays();

        RenderSystem * render = RenderSystem::Instance();
        RenderTarget * rt = render->GetRenderTarget(0);

        _renderRay();

        render->SetRenderTarget(0, rt);

        _blend();
    }

    void UnderWaterGodRay::_init()
    {
        mPerlin = new Perlin(Perlin::Options(8, 0.085f, 0.49f, 2, 0.672f));

        _createGodRays();

        VideoBufferManager * vm = VideoBufferManager::Instance();

		mTexture = vm->CreateTextureRT("Core_UnderWaterGodRayRT", 512, 512, FMT_R16F);
        mRenderTarget = vm->CreateRenderTarget(mTexture);

        mTech_Ray = Environment::Instance()->GetShaderLib()->GetTechnique("UnderWaterGodRay");
        mTech_Blend = Environment::Instance()->GetShaderLib()->GetTechnique("UnderWaterGodRayBlend");

        d_assert (mTech_Ray != NULL && mTech_Blend != NULL);
    }

    void UnderWaterGodRay::_createGodRays()
    {
        VertexStream * vxStream = &mRender.vxStream;
        IndexStream * ixStream = &mRender.ixStream;

        int iVertexCount = mNumberOfRays * 12;
        int iIndexCount = mNumberOfRays * 12;
        int iPrimCount = iIndexCount / 3;
        int iStride = 12;

        VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
        decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
        decl->Init();

        vxStream->SetDeclaration(decl);

        VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 20, USAGE_DYNAMIC);
        IndexBufferPtr ib = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));

        float * vert = (float *)vb->Lock(0, 0, LOCK_NORMAL);
        short * idx = (short *)ib->Lock(0, 0, LOCK_NORMAL);

        int index = 0;
        for (int j = 0; j < mNumberOfRays; ++j)
        {
            for (int i = 0; i < 12; ++i)
            {
                *vert++ = 0; *vert++ = 0; *vert++ = 0;
                *idx++ = index++; 
            }
        }

        vb->Unlock();
        ib->Unlock();

        vxStream->Bind(0,vb, iStride);
        vxStream->SetCount(iVertexCount);

        ixStream->Bind(ib, 0);
        ixStream->SetCount(iIndexCount);

        mRender.iPrimCount = iPrimCount;
        mRender.ePrimType = PRIM_TRIANGLELIST;
        mRender.rState.blendMode = BM_ADD;
        mRender.rState.cullMode = CULL_NONE;
        mRender.rState.fillMode = FILL_SOLID;
        mRender.rState.depthWrite = false;
        mRender.rState.depthCheck = DCM_NONE;
    }

    void UnderWaterGodRay::_updateRays()
    {
        Camera * cam = World::Instance()->MainCamera();
        const Vec3 * corner = cam->GetCorner();
        float FarWidth   = (corner[4] - corner[5]).Length();
        float RaysLength = cam->GetFarClip();

        VertexBufferPtr vb = mRender.vxStream.GetStream(0);
        Vec3 * vert = (Vec3*)vb->Lock(0, 0, LOCK_DISCARD);

        Vec2 Pos;
        float Dis, RayLength;

        for(int k = 0; k < mNumberOfRays; k++)
        {
            Pos       = _calculateRayPosition(k);
            Dis       = mRaysSize * RaysLength;
            RayLength = RaysLength * (0.3f + Pos.Length());

            Pos *= FarWidth/2;

            // 4 Planes, 3 vertices each plane, 12 vertices per ray
            // ----> 1/4
            // 0
            *vert++ = Vec3(0, 0, 0);
            // A
            *vert++ = Vec3(Pos.x, Pos.y, -RayLength);
            // B
            *vert++ = Vec3(Pos.x+Dis, Pos.y, -RayLength);
            // ----> 2/4
            // 0
            *vert++ = Vec3(0, 0, 0);
            // D
            *vert++ = Vec3(Pos.x+Dis, Pos.y+Dis, -RayLength);
            // B
            *vert++ = Vec3(Pos.x+Dis, Pos.y, -RayLength);
            // ----> 3/4
            // 0
            *vert++ = Vec3(0, 0, 0);
            // C
            *vert++ = Vec3(Pos.x, Pos.y+Dis, -RayLength);
            // D
            *vert++ = Vec3(Pos.x+Dis, Pos.y+Dis, -RayLength);
            // ----> 4/4
            // 0
            *vert++ = Vec3(0, 0, 0);
            // C
            *vert++ = Vec3(Pos.x, Pos.y+Dis, -RayLength);
            // A
            *vert++ = Vec3(Pos.x, Pos.y, -RayLength);
        }

        vb->Unlock();
    }

    void UnderWaterGodRay::_updateProjector()
    {
        Plane p(Vec3(0, 1, 0), mWaterPosition);
        Vec3 sunDir = Environment::Instance()->GetEvParam()->SunDir;
        Vec3 sunPos = Environment::Instance()->GetEvParam()->SunPos;
        Ray r(sunPos, sunDir);

        RayIntersectionInfo info = r.Intersection(p);

        mEnable = info.iterscetion;

        mProjPosition = sunPos + sunDir * info.distance;

        Vec3 y(0, 1, 0);

        if (sunDir.Dot(y) < 0.001)
            y = Vec3(0, 0, -1);

        Vec3 x = y.Cross(sunDir);
        y = sunDir.Cross(x);
        
        mProjMatrix.MakeRotationAxis(x, y, sunDir);
        mProjMatrix._41 = mProjPosition.x;
        mProjMatrix._42 = mProjPosition.y;
        mProjMatrix._43 = mProjPosition.z;
    }

    Vec2 UnderWaterGodRay::_calculateRayPosition(int RayNumber)
    {
        float sqrt_NumberOfRays = Math::Sqrt((float)mNumberOfRays);
        float XCoord = (float)RayNumber;

        while (XCoord >= sqrt_NumberOfRays)
        {
            XCoord -= sqrt_NumberOfRays;
        }

       Vec2 RayPos = Vec2(float(int(XCoord)), float(int((RayNumber+sqrt_NumberOfRays)/sqrt_NumberOfRays))-1);

       RayPos /= sqrt_NumberOfRays;
       RayPos -= Vec2(0.5f, 0.5f);
       RayPos *= 2;

       Vec2 Position = RayPos*mNoisePositionMultiplier + Vec2(mProjPosition.x, mProjPosition.z);

       float d0 = Position.x-mNoiseDerivation;
       float d1 = Position.x+mNoiseDerivation;
       float d2 = Position.y-mNoiseDerivation;
       float d3 = Position.y+mNoiseDerivation;

       float r0 = mPerlin->getValue(d0,0);
       float r1 = mPerlin->getValue(d1,0);
       float r2 = mPerlin->getValue(0,d2);
       float r3 = mPerlin->getValue(0,d3);

       Vec3
            m_x = Vec3(d0, r0, 0),
            p_x = Vec3(d1, r1, 0),
            m_y = Vec3(0, r2, d2),
            p_y = Vec3(0, r3, d3);

        m_x.y *= mNoiseYNormalMultiplier; p_x.y *= mNoiseYNormalMultiplier;
        m_y.y *= mNoiseYNormalMultiplier; p_y.y *= mNoiseYNormalMultiplier;

        Vec3 n0 = p_x - m_x;
        Vec3 n1 = p_y - m_y;

        Vec3 Normal = n0.Cross(n1);

        Normal *= mNoiseNormalMultiplier;

        return RayPos + Vec2(Normal.x, Normal.z);
    }

    void UnderWaterGodRay::_renderRay()
    {
        Camera * cam = World::Instance()->MainCamera();
        RenderSystem * render = RenderSystem::Instance();
        render->SetRenderTarget(0, mRenderTarget.c_ptr());

        render->ClearBuffer(NULL, true, false, false);

        ShaderParam * uWorldMatrix = mTech_Ray->GetVertexShaderParamTable()->GetParam("matWorld");

        uWorldMatrix->SetMatrix(mProjMatrix);

        render->Render(mTech_Ray, &mRender);

        mRenderTarget->Stretch(mTexture.c_ptr());
    }

    void UnderWaterGodRay::_blend()
    {
        RenderSystem * render = RenderSystem::Instance();
        Camera * cam = World::Instance()->MainCamera();

        ShaderParam * uHGg = mTech_Blend->GetPixelShaderParamTable()->GetParam("gHGg");
        ShaderParam * uIntensity = mTech_Blend->GetPixelShaderParamTable()->GetParam("gIntensity");
        ShaderParam * uSunColor = mTech_Blend->GetPixelShaderParamTable()->GetParam("gSunColor");
        ShaderParam * uCornerLeftTop = mTech_Blend->GetPixelShaderParamTable()->GetParam("gCornerLeftTop");
        ShaderParam * uCornerRightDir = mTech_Blend->GetPixelShaderParamTable()->GetParam("gCornerRightDir");
        ShaderParam * uCornerDownDir = mTech_Blend->GetPixelShaderParamTable()->GetParam("gCornerDownDir");
        ShaderParam * uSunDir = mTech_Blend->GetPixelShaderParamTable()->GetParam("gSunDir");

        const Vec3 * corner = cam->GetCorner();

        Vec3 cornerLeftTop = corner[4];
        Vec3 cornerRightDir = corner[5] - corner[4];
        Vec3 cornerDownDir = corner[6] - corner[4];
        Vec3 sunDir = Environment::Instance()->GetEvParam()->SunDir;
        sunDir = sunDir.TransformN(cam->GetViewMatrix());

        Color4 sunColor = Environment::Instance()->GetEvParam()->SunColor;
        mExposure.x = Environment::Instance()->GetEvParam()->WaterParam.godrayExp0;
        mExposure.y = Environment::Instance()->GetEvParam()->WaterParam.godrayExp1;
        mIntensity = Environment::Instance()->GetEvParam()->WaterParam.godrayLum;

        if (uHGg) uHGg->SetUnifom(mExposure.x, mExposure.y, mExposure.z, 0);
        if (uIntensity) uIntensity->SetUnifom(mIntensity, 0, 0, 0);
        if (uSunColor) uSunColor->SetUnifom(sunColor.r, sunColor.g, sunColor.b, 1);

        uCornerLeftTop->SetUnifom(cornerLeftTop.x, cornerLeftTop.y, cornerLeftTop.z, 0);
        uCornerRightDir->SetUnifom(cornerRightDir.x, cornerRightDir.y, cornerRightDir.z, 0);
        uCornerDownDir->SetUnifom(cornerDownDir.x, cornerDownDir.y, cornerDownDir.z, 0);
        uSunDir->SetUnifom(sunDir.x, sunDir.y, sunDir.z, 0);

        SamplerState state;
        render->SetTexture(0, state,  mTexture.c_ptr());

        RenderHelper::Instance()->DrawScreenQuad(BM_ADD, mTech_Blend);

    }








    UnderWaterCaustics::UnderWaterCaustics()
    {
        _init();
    }

    UnderWaterCaustics::~UnderWaterCaustics()
    {
    }

    void UnderWaterCaustics::Render(Texture * colorTex, Texture * depthTex)
    {
        RenderSystem * render = RenderSystem::Instance();
        Camera * cam = World::Instance()->MainCamera();

        ShaderParam * uCornerLeftTop = mTech->GetPixelShaderParamTable()->GetParam("gCornerLeftTop");
        ShaderParam * uCornerRightDir = mTech->GetPixelShaderParamTable()->GetParam("gCornerRightDir");
        ShaderParam * uCornerDownDir = mTech->GetPixelShaderParamTable()->GetParam("gCornerDownDir");
        ShaderParam * uCameraPos = mTech->GetPixelShaderParamTable()->GetParam("gCameraPos");
        ShaderParam * uCausticsParam = mTech->GetPixelShaderParamTable()->GetParam("gCausticsParam");

        const Vec3 * corner = cam->GetWorldCorner();
        const Vec3 & camPos = cam->GetPosition();

        Vec3 cornerLeftTop = corner[4] - camPos;
        Vec3 cornerRightDir = corner[5] - corner[4];
        Vec3 cornerDownDir = corner[6] - corner[4];

        float waterY = 180;
        float depthLimit = 15;
        float uvScale = 0.05f;

        uCornerLeftTop->SetUnifom(cornerLeftTop.x, cornerLeftTop.y, cornerLeftTop.z, 0);
        uCornerRightDir->SetUnifom(cornerRightDir.x, cornerRightDir.y, cornerRightDir.z, 0);
        uCornerDownDir->SetUnifom(cornerDownDir.x, cornerDownDir.y, cornerDownDir.z, 0);
        uCameraPos->SetUnifom(camPos.x, camPos.y, camPos.z, 0);
        uCausticsParam->SetUnifom(waterY, depthLimit, uvScale, 0);

        SamplerState state;
        render->SetTexture(0, state,  depthTex);
        render->SetTexture(1, state,  colorTex);
        render->SetTexture(2, state,  GetTexture());

        RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech);
    }

    void UnderWaterCaustics::_init()
    {
        mTech = Environment::Instance()->GetShaderLib()->GetTechnique("UnderWaterCaustics");
        mInternalTime = 0;

        d_assert (mTech != NULL);

        TString128 texName = "";
        for (int i = 0; i < nFrames; ++i)
        {
            texName.Format("Water\\Caustics_%d.bmp", i);
            mCausticsTex[i] = VideoBufferManager::Instance()->Load2DTexture(texName, texName);
        }
    }

    Texture * UnderWaterCaustics::GetTexture()
    {
        float AnimTime = 1.5f;

        float time = Engine::Instance()->GetFrameTime();

        mInternalTime += time;

        while (mInternalTime >= AnimTime)
            mInternalTime -= AnimTime;

        float d = mInternalTime / AnimTime * nFrames;
        int i = (int)d;

        return mCausticsTex[i % nFrames].c_ptr();
    }








    UnderWaterBubble::UnderWaterBubble()
    {
        _init();

        mLastEmitTime = 0;
        mInternalTime = 0;
        mRate = 300;
    }

    UnderWaterBubble::~UnderWaterBubble()
    {
        _deinit();
    }

    void UnderWaterBubble::Render()
    {
        _update();
        _emit();
        _geom();

        Camera * cam = World::Instance()->MainCamera();
        RenderSystem * render = RenderSystem::Instance();

        SamplerState state;

        render->SetTexture(0, state, mTex_Bubble0.c_ptr());

        render->Render(mTech, &mRender);
    }

    void UnderWaterBubble::_init()
    {
        {
            VertexStream * vxStream = &mRender.vxStream;
            IndexStream * ixStream = &mRender.ixStream;

            int iVertexCount = 4 * mMaxNum;
            int iPrimCount = 2 * mMaxNum;

            VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
            decl->AddElement(0, 0, DT_FLOAT4, DU_POSITION, 0);
            decl->AddElement(0, 16, DT_FLOAT1, DU_TEXCOORD, 0);
            decl->AddElement(1, 0, DT_FLOAT2, DU_TEXCOORD, 1);
            decl->Init();

            vxStream->SetDeclaration(decl);

            VertexBufferPtr vb0 = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 20, 20, USAGE_DYNAMIC);
            VertexBufferPtr vb1 = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 8, 8);

            float * tcoord = (float *)vb1->Lock(0, 0, LOCK_NORMAL);

            for (int i = 0; i < mMaxNum; ++i)
            {
                *tcoord++ = 0, *tcoord++ = 0;
                *tcoord++ = 1, *tcoord++ = 0;
                *tcoord++ = 0, *tcoord++ = 1;
                *tcoord++ = 1, *tcoord++ = 1;
            }

            vb1->Unlock();

            vxStream->Bind(0, vb0, 20);
            vxStream->Bind(1, vb1, 8);
            vxStream->SetCount(iVertexCount);

            IndexBufferPtr ib = VideoBufferManager::Instance()->CreateIndexBuffer(sizeof (short) * mMaxNum * 6);

            short * idx = (short *)ib->Lock(0, 0, LOCK_NORMAL);
            {
                for (int i = 0; i < mMaxNum; ++i)
                {
                    int j = i * 4;

                    *idx++ = j;
                    *idx++ = j + 1;
                    *idx++ = j + 2;

                    *idx++ = j + 2;
                    *idx++ = j + 1;
                    *idx++ = j + 3;
                }
            }
            ib->Unlock();

            ixStream->Bind(ib, 0);
            ixStream->SetCount(mMaxNum * 6);

            mRender.iPrimCount = iPrimCount;
            mRender.ePrimType = PRIM_TRIANGLELIST;
            mRender.rState.blendMode = BM_ADD;
            mRender.rState.cullMode = CULL_NONE;
            mRender.rState.fillMode = FILL_SOLID;
            mRender.rState.depthWrite = false;
            mRender.rState.depthCheck = DCM_LESS;
        }

        mTex_Bubble0 = VideoBufferManager::Instance()->Load2DTexture("Water\\Bubble0.dds", "Water\\bubble.dds");
        mTex_Bubble1 = VideoBufferManager::Instance()->Load2DTexture("Water\\Bubble1.dds", "Water\\bubble_2.dds");

        mTech = Environment::Instance()->GetShaderLib()->GetTechnique("Bubble");
    }

    void UnderWaterBubble::_deinit()
    {
        mTex_Bubble0 = NULL;
        mTex_Bubble1 = NULL;
        mTech = NULL;
    }

    void UnderWaterBubble::_update()
    {
        float frameTime = Engine::Instance()->GetFrameTime();

        List<Bubble>::Iterator whr = mBubbles.Begin();
        List<Bubble>::Iterator end = mBubbles.End();

        while (whr != end)
        {
            Bubble & b = *whr;

            b.life += frameTime;

            if (b.life >= b.maxlife)
            {
                whr = mBubbles.Erase(whr);
                continue;
            }

            b.position.y += frameTime * b.speed;

            float d = b.life / b.maxlife;
            d = Math::Minimum(d, 1.0f);

            if (d < 0.25f)
                b.alpha = d / 0.25f;
            else if (d > 0.75f)
                b.alpha = (1 - d) / 0.25f;
            else
                b.alpha = 1;

            b.alpha *= 0.45f;

            ++whr;
        }
    }

    void UnderWaterBubble::_emit()
    {
        float frameTime = Engine::Instance()->GetFrameTime();

        mInternalTime += frameTime;

        float dtime = mInternalTime - mLastEmitTime;

        int num = int(mRate * dtime);

        if (num > 0)
            mLastEmitTime = mInternalTime;

        while (num-- && mBubbles.Size() < mMaxNum)
        {
            Bubble b;

            b.position = _randomPosition();
            b.maxlife = _randomLife();
            b.size = _randomSize();
            b.speed = _randomSpeed();
            b.life = 0;
            b.alpha = 0;

            mBubbles.PushBack(b);
        }
    }

    void UnderWaterBubble::_geom()
    {
        VertexStream * vxStream = &mRender.vxStream;

        VertexBufferPtr vb = vxStream->GetStream(0);

        float * v = (float *)vb->Lock(0, 0, LOCK_DISCARD);
        {
            List<Bubble>::Iterator whr = mBubbles.Begin();
            List<Bubble>::Iterator end = mBubbles.End();

            while (whr != end)
            {
                const Bubble & b = *whr;
                *v++ = b.position.x;
                *v++ = b.position.y;
                *v++ = b.position.z;
                *v++ = b.size;
                *v++ = b.alpha;

                *v++ = b.position.x;
                *v++ = b.position.y;
                *v++ = b.position.z;
                *v++ = b.size;
                *v++ = b.alpha;

                *v++ = b.position.x;
                *v++ = b.position.y;
                *v++ = b.position.z;
                *v++ = b.size;
                *v++ = b.alpha;

                *v++ = b.position.x;
                *v++ = b.position.y;
                *v++ = b.position.z;
                *v++ = b.size;
                *v++ = b.alpha;

                ++whr;
            }
        }
        vb->Unlock();

        mRender.iPrimCount = mBubbles.Size() * 2;
    }

    Vec3 UnderWaterBubble::_randomPosition()
    {
        Camera * cam = World::Instance()->MainCamera();
        
        Vec3 pos = cam->GetPosition();

        Vec3 o;

        o.x = Math::RandRange(-50.0f, 50.0f);
        o.y = Math::RandRange(-10.0f, 10.0f);
        o.z = Math::RandRange(-50.0f, 50.0f);

        return pos + o;
    }

    float UnderWaterBubble::_randomLife()
    {
        return Math::RandRange(4.0f, 8.0f);
    }

    float UnderWaterBubble::_randomSize()
    {
        return Math::RandRange(0.05f, 0.1f);
    }

    float UnderWaterBubble::_randomSpeed()
    {
        return Math::RandRange(2.0f, 4.0f);
    }
}