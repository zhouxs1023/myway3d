#include "MWCloud.h"
#include "MWEnvironment.h"
#include "MWRenderEvent.h"
#include "Engine.h"

namespace Myway {

    Cloud::Cloud()
    {
        _initGeometry();
        _initTechnique();
        _initRenderTarget();
        _initLayer();

		mFarClip = 10000.0f;
    }

    Cloud::~Cloud()
    {
    }

    void Cloud::Render(bool lighting)
	{
		RS_RenderEvent(Cloud);

		Camera * cam = World::Instance()->MainCamera();

		mMatProj.MakePerspectiveFovLH(cam->GetFov(), cam->GetAspect(), cam->GetNearClip(), mFarClip);

		RenderSystem * render = RenderSystem::Instance();
		RenderTarget * oldRt = render->GetRenderTarget(0);

        if (lighting)
        {
			_cloud();

            _lighting();

            _blur();
        }

		render->SetRenderTarget(0, oldRt);

        _shading(lighting);
    }

	void Cloud::Resize(int w, int h)
	{
		_initRenderTarget();
	}


    void Cloud::_initGeometry()
    {
        VertexStream * vxStream = &mRender.vxStream;
        IndexStream * ixStream = &mRender.ixStream;

        int xSegments = 80;
        int zSegments = 80;

        int iVertexCount = (xSegments + 1) * (zSegments + 1);
        int iIndexCount = xSegments * zSegments * 2 * 3;
        int iPrimCount = xSegments * zSegments * 2;
        int iStride = 20;

        VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
        decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
        decl->AddElement(0, 12, DT_FLOAT2, DU_TEXCOORD, 0);
        decl->Init();

        vxStream->SetDeclaration(decl);

        VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 20, 20);

        float * vert = (float *)vb->Lock(0, 0, LOCK_NORMAL);

        float xstep = 2.0f / xSegments;
        float zstep = 2.0f / zSegments;
        float ustep = 1.0f / xSegments;
        float vstep = 1.0f / zSegments;

        for (int z = 0; z < zSegments + 1; ++z)
        {
            for (int x = 0; x < xSegments + 1; ++x)
            {
                float dx = (x - xSegments / 2.0f) / (float)xSegments;
                float dz = (z - zSegments / 2.0f) / (float)zSegments;
                float dt = Math::Sqrt(dx * dx + dz * dz) / (Math::SQRT_2 * 0.5f);

                float px = -1 + x * xstep;
                float pz = +1 - z * zstep;
                float py = Math::Cos(dt * Math::PI_1 / 2);

                float u = 0 + x * ustep;
                float v = 0 + z * vstep;

                *vert++ = px; *vert++ = py; *vert++ = pz;

                *vert++ = u; *vert++ = v;
            }
        }

        vb->Unlock();
        
        vxStream->Bind(0,vb, iStride);
        vxStream->SetCount(iVertexCount);

        IndexBufferPtr ib = VideoBufferManager::Instance()->CreateIndexBuffer(iIndexCount * sizeof(short));
        short * idx = (short *)ib->Lock(0, 0, LOCK_NORMAL);
        {
            short row = 0, row_n = 0;
            short i, j;

            for (i = 0; i < zSegments; ++i)
            {
                row_n = row + xSegments + 1;

                for (j = 0; j < xSegments; ++j)
                {
                    *idx++ = row + j;
                    *idx++ = row + j + 1;
                    *idx++ = row_n + j;

                    *idx++ = row_n + j;
                    *idx++ = row + j + 1;
                    *idx++ = row_n + j + 1;

                }

                row += xSegments + 1;
            }
        }
        ib->Unlock();

        ixStream->Bind(ib, 0);
        ixStream->SetCount(iIndexCount);

        mRender.iPrimCount = iPrimCount;
        mRender.ePrimType = PRIM_TRIANGLELIST;
        mRender.rState.blendMode = BM_ALPHA_BLEND;
        mRender.rState.cullMode = CULL_NONE;
        mRender.rState.fillMode = FILL_SOLID;
        mRender.rState.depthWrite = false;
		mRender.rState.depthCheck = DCM_LESS_EQUAL;
    }

    void Cloud::_initTechnique()
    {
		mTech_Cloud = Environment::Instance()->GetShaderLib()->GetTechnique("Cloud");
        mTech_Lighting = Environment::Instance()->GetShaderLib()->GetTechnique("CloudLighting");
        mTech_BlurH = Environment::Instance()->GetShaderLib()->GetTechnique("BlurH");
        mTech_BlurV = Environment::Instance()->GetShaderLib()->GetTechnique("BlurV");
        mTech_Shading = Environment::Instance()->GetShaderLib()->GetTechnique("CloudShading");
    }

    void Cloud::_initRenderTarget()
    {
		mRT_Cloud = NULL;
		mTex_Cloud = NULL;

        mRT_Lighting = NULL;
        mTex_Lighting= NULL;

		RenderScheme * scheme = Engine::Instance()->GetRenderScheme();

		int width = scheme->GetColorTexture()->GetWidth() / 2;
		int height = scheme->GetColorTexture()->GetHeight() / 2;

        VideoBufferManager * video = VideoBufferManager::Instance();

		mTex_Cloud = video->CreateTextureRT("Core_Cloud_Tex_Cloud", width, height, FMT_R16G16F);
		mRT_Cloud = video->CreateRenderTarget(mTex_Cloud);

        mRT_Lighting = video->CreateRenderTarget("Core_Cloud_RT_Lighting", width, height, FMT_R16F, MSAA_NONE);
        mTex_Lighting = video->CreateTextureRT("Core_Cloud_Tex_Lighting", width, height, FMT_R16F);
    }

    void Cloud::_initLayer()
    {
        const char * layer0 = "Sky\\CloudLayer1.dds";
        const char * layer1 = "Sky\\CloudLayer2.dds";

        mTex_Layer0 = VideoBufferManager::Instance()->Load2DTexture(layer0, layer0);
        mTex_Layer1 = VideoBufferManager::Instance()->Load2DTexture(layer1, layer1);

		d_assert (mTex_Layer0 != NULL && mTex_Layer1 != NULL);
    }

	void Cloud::_cloud()
	{
		RenderSystem * render = RenderSystem::Instance();

		render->SetRenderTarget(0, mRT_Cloud.c_ptr());

		render->ClearBuffer(NULL, true, false, false);

		ShaderParam * uTranslate = mTech_Cloud->GetVertexShaderParamTable()->GetParam("gTranslate");
		ShaderParam * uScale = mTech_Cloud->GetVertexShaderParamTable()->GetParam("gScale");
		ShaderParam * uMatWVP = mTech_Cloud->GetVertexShaderParamTable()->GetParam("matWVP");
		ShaderParam * uUVScale = mTech_Cloud->GetVertexShaderParamTable()->GetParam("gUVScale");
		ShaderParam * uUVScroll = mTech_Cloud->GetVertexShaderParamTable()->GetParam("gUVScroll");

		ShaderParam * uMass = mTech_Cloud->GetPixelShaderParamTable()->GetParam("gMass");
		ShaderParam * uWeight = mTech_Cloud->GetPixelShaderParamTable()->GetParam("gWeight");

		Camera * cam = World::Instance()->MainCamera();
		float farclip = cam->GetFarClip() * 0.9f;
		Vec3 pos = cam->GetPosition();

		const EvParam * param = Environment::Instance()->GetEvParam();
		float time = Engine::Instance()->GetTime();

		float height = param->CloudParam.height;
		float curved = param->CloudParam.curved;
	
		uTranslate->SetUnifom(pos.x, pos.y + farclip * height, pos.z, 1);
		uScale->SetUnifom(mFarClip, farclip * curved, mFarClip, 1);
		uMatWVP->SetMatrix(cam->GetViewMatrix() * mMatProj);

		mRender.rState.depthCheck = DCM_ALWAYS;
		render->Render(mTech_Cloud, &mRender);
		mRender.rState.depthCheck = DCM_LESS_EQUAL;

		mRT_Cloud->Stretch(mTex_Cloud.c_ptr());
	}

    void Cloud::_lighting()
    {
        RenderSystem * render = RenderSystem::Instance();

		render->SetRenderTarget(0, mRT_Lighting.c_ptr());

        Camera * cam = World::Instance()->MainCamera();
        float farclip = cam->GetFarClip() * 0.9f;
        Vec3 pos = cam->GetPosition();

        ShaderParam * uMass = mTech_Lighting->GetPixelShaderParamTable()->GetParam("gMass");
        ShaderParam * uWeight = mTech_Lighting->GetPixelShaderParamTable()->GetParam("gWeight");
        ShaderParam * uUVScale = mTech_Lighting->GetPixelShaderParamTable()->GetParam("gUVScale");
        ShaderParam * uUVScroll = mTech_Lighting->GetPixelShaderParamTable()->GetParam("gUVScroll");
        ShaderParam * uUVSun = mTech_Lighting->GetPixelShaderParamTable()->GetParam("gSunUV");
        ShaderParam * uAlphaParam = mTech_Shading->GetPixelShaderParamTable()->GetParam("gAlphaParam");
        ShaderParam * uLightingParam = mTech_Lighting->GetPixelShaderParamTable()->GetParam("gLightingParam");

        const EvParam * param = Environment::Instance()->GetEvParam();
        float time = Engine::Instance()->GetTime();

        float height = param->CloudParam.height;
        float curved = param->CloudParam.curved;
        float mass = param->CloudParam.mass;
        float wieght0 = param->CloudParam.weight0;
        float wieght1 = param->CloudParam.weight1;
        float uvScale0 = param->CloudParam.uvScale0;
        float uvScale1 = param->CloudParam.uvScale1;
        float uvScroll0 = param->CloudParam.uvScroll0;
        float uvScroll1 = param->CloudParam.uvScroll1;
        float alpha = param->CloudParam.alpha;
        float alphaStrength = param->CloudParam.alphaStrength;
        float lightStrength = param->CloudParam.lightStrength;
        float fadeStart = param->CloudParam.fadeStart;
        float fadeStrength = param->CloudParam.fadeStrength;
        Color4 ambient = param->CloudParam.ambient;
        Color4 diffuse = param->CloudParam.diffuse;
        float ambientScale = param->CloudParam.ambientScale;
        float diffuseScale = param->CloudParam.diffuseScale;
        Vec2 windDir = Vec2(1, 0);

        uvScroll0 *= time;
        uvScroll1 *= time;

        uMass->SetUnifom(mass, 0, 0, 0);
        uWeight->SetUnifom(wieght0, wieght1, 0, 0);
        uUVScale->SetUnifom(uvScale0, uvScale1, 0, 0);
        uUVScroll->SetUnifom(windDir.x * uvScroll0, windDir.y * uvScroll0,
                             windDir.x * uvScroll1, windDir.y * uvScroll1);
        uUVSun->SetUnifom(0.5f, 0.5f, 0.001f, 1);
        uAlphaParam->SetUnifom(alpha, alphaStrength, 0, 0);
        uLightingParam->SetUnifom(lightStrength, 0, 0, 0);

        SamplerState state;
		state.Address = TEXA_CLAMP;
        render->SetTexture(0, state, mTex_Cloud.c_ptr());

		state.Address = TEXA_WRAP;
		render->SetTexture(1, state, mTex_Layer0.c_ptr());
		render->SetTexture(2, state, mTex_Layer1.c_ptr());

		RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_Lighting);

		mRT_Lighting->Stretch(mTex_Lighting.c_ptr());
    }

    void Cloud::_blur()
    {
        RenderSystem * render = RenderSystem::Instance();

        SamplerState state;
        state.Address = TEXA_CLAMP;

        render->SetTexture(0, state, mTex_Lighting.c_ptr());

        for (int i = 0; i < 1; ++i)
        {
            RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_BlurH);

            mRT_Lighting->Stretch(mTex_Lighting.c_ptr());

            RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_BlurV);

			mRT_Lighting->Stretch(mTex_Lighting.c_ptr());
        }
    }

    void Cloud::_shading(bool lighting)
    {
        RenderSystem * render = RenderSystem::Instance();
        Camera * cam = World::Instance()->MainCamera();

        float farclip = cam->GetFarClip() * 0.9f;

        Vec3 pos = cam->GetPosition();

		ShaderParam * uTranslate = mTech_Shading->GetVertexShaderParamTable()->GetParam("gTranslate");
		ShaderParam * uScale = mTech_Shading->GetVertexShaderParamTable()->GetParam("gScale");
		ShaderParam * uMatWVP = mTech_Shading->GetVertexShaderParamTable()->GetParam("matWVP");
        ShaderParam * uUVScale = mTech_Shading->GetVertexShaderParamTable()->GetParam("gUVScale");
        ShaderParam * uUVScroll = mTech_Shading->GetVertexShaderParamTable()->GetParam("gUVScroll");

        ShaderParam * uMass = mTech_Shading->GetPixelShaderParamTable()->GetParam("gMass");
        ShaderParam * uWeight = mTech_Shading->GetPixelShaderParamTable()->GetParam("gWeight");
        ShaderParam * uAlphaParam = mTech_Shading->GetPixelShaderParamTable()->GetParam("gAlphaParam");
        ShaderParam * uFadeParam = mTech_Shading->GetPixelShaderParamTable()->GetParam("gFadeParam");
        ShaderParam * uAmbient = mTech_Shading->GetPixelShaderParamTable()->GetParam("gAmbient");
        ShaderParam * uDiffuse = mTech_Shading->GetPixelShaderParamTable()->GetParam("gDiffuse");

        const EvParam * param = Environment::Instance()->GetEvParam();
        float time = Engine::Instance()->GetTime();

        float height = param->CloudParam.height;
        float curved = param->CloudParam.curved;
        float mass = param->CloudParam.mass;
        float wieght0 = param->CloudParam.weight0;
        float wieght1 = param->CloudParam.weight1;
        float uvScale0 = param->CloudParam.uvScale0;
        float uvScale1 = param->CloudParam.uvScale1;
        float uvScroll0 = param->CloudParam.uvScroll0;
        float uvScroll1 = param->CloudParam.uvScroll1;
        float alpha = param->CloudParam.alpha;
        float alphaStrength = param->CloudParam.alphaStrength;
        float lightStrength = param->CloudParam.lightStrength;
        float fadeStart = param->CloudParam.fadeStart;
        float fadeStrength = param->CloudParam.fadeStrength;
        Color4 ambient = param->CloudParam.ambient;
        Color4 diffuse = param->CloudParam.diffuse;
        float ambientScale = param->CloudParam.ambientScale;
        float diffuseScale = param->CloudParam.diffuseScale;
        Vec2 windDir = Vec2(1, 0);

        uvScroll0 *= time;
        uvScroll1 *= time;

        //uTranslate->SetUnifom(pos.x, pos.y + farclip * 0.1f, pos.z, 1);
        uTranslate->SetUnifom(pos.x, pos.y + farclip * height, pos.z, 1);
		uScale->SetUnifom(mFarClip, farclip * curved, mFarClip, 1);
		uMatWVP->SetMatrix(cam->GetViewMatrix() * mMatProj);

        uMass->SetUnifom(mass, 0, 0, 0);
        uWeight->SetUnifom(wieght0, wieght1, 0, 0);
        uUVScale->SetUnifom(uvScale0, uvScale1, 0, 0);
        uUVScroll->SetUnifom(windDir.x * uvScroll0, windDir.y * uvScroll0,
                             windDir.x * uvScroll1, windDir.y * uvScroll1);

        uAlphaParam->SetUnifom(alpha, alphaStrength, 0, 0);
        uFadeParam->SetUnifom(fadeStart * farclip, 1 / (farclip - fadeStart * farclip), 1, 0);
        uAmbient->SetUnifom(ambient.r, ambient.g, ambient.b, ambientScale);
        uDiffuse->SetUnifom(diffuse.r, diffuse.g, diffuse.b, diffuseScale);

        SamplerState state;
		render->SetTexture(0, state, mTex_Layer0.c_ptr());
        render->SetTexture(1, state, mTex_Layer1.c_ptr());

        if (lighting)
            render->SetTexture(2, state, mTex_Lighting.c_ptr());
        else
            render->SetTexture(2, state, RenderHelper::Instance()->GetWhiteTexture().c_ptr());

        render->Render(mTech_Shading, &mRender);
    }

}