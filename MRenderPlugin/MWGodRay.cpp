#include "MWGodRay.h"
#include "MWEnvironment.h"
#include "Engine.h"

namespace Myway {

    GodRay::GodRay()
    {
        _initGeometry();
        _initTechnique();
        _initRenderTarget();
    }

    GodRay::~GodRay()
    {
    }

    void GodRay::Render(Texture * depthTex)
    {
		RS_RenderEvent(GodRay);

        RenderSystem * render = RenderSystem::Instance();

        RenderTarget * oldRt = render->GetRenderTarget(0);

		_occlusion();
        _renderSun();
        _godray(depthTex);
        _blur();

        render->SetRenderTarget(0, oldRt);

        _blend();
    }

    void GodRay::Resize(int w, int h)
    {
		_initRenderTarget();
    }

    void GodRay::_initGeometry()
    {
        VertexStream * vxStream = &mRender_Sun.vxStream;
        IndexStream * ixStream = &mRender_Sun.ixStream;

        int iVertexCount = 4;
        int iPrimCount = 2;

        VertexDeclarationPtr decl = VideoBufferManager::Instance()->CreateVertexDeclaration();
        decl->AddElement(0, 0, DT_FLOAT3, DU_POSITION, 0);
        decl->AddElement(0, 12, DT_FLOAT2, DU_TEXCOORD, 0);
        decl->Init();

        vxStream->SetDeclaration(decl);

        VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(iVertexCount * 20, 20);

        float * vert = (float *)vb->Lock(0, 0, LOCK_DISCARD);
        {
            float x = 0, y = 0, z = 0;

            *vert++ = x; *vert++ = y; *vert++ = z;
            *vert++ = 0; *vert++ = 0;

            *vert++ = x; *vert++ = y; *vert++ = z;
            *vert++ = 1; *vert++ = 0;

            *vert++ = x; *vert++ = y; *vert++ = z;
            *vert++ = 0; *vert++ = 1;

            *vert++ = x; *vert++ = y; *vert++ = z;
            *vert++ = 1; *vert++ = 1;
        }
        vb->Unlock();

        vxStream->Bind(0, vb, 20);
        vxStream->SetCount(iVertexCount);

        mRender_Sun.iPrimCount = iPrimCount;
        mRender_Sun.ePrimType = PRIM_TRIANGLESTRIP;

        mRender_Sun.rState.cullMode = CULL_NONE;
        mRender_Sun.rState.blendMode = BM_ALPHA_BLEND;
        mRender_Sun.rState.depthWrite = false;
        mRender_Sun.rState.depthCheck = DCM_ALWAYS;
    }

    void GodRay::_initTechnique()
    {
		mTech_SunOcc = Environment::Instance()->GetShaderLib()->GetTechnique("GodRaySunOcc");
        mTech_Sun = Environment::Instance()->GetShaderLib()->GetTechnique("GodRaySun");
        mTech_GodRay = Environment::Instance()->GetShaderLib()->GetTechnique("GodRay");
        mTech_BlurH = Environment::Instance()->GetShaderLib()->GetTechnique("BlurH");
        mTech_BlurV = Environment::Instance()->GetShaderLib()->GetTechnique("BlurV");
        mTech_Blend = Environment::Instance()->GetShaderLib()->GetTechnique("GodRayBlend");

        d_assert (mTech_Sun && mTech_GodRay && mTech_BlurH && mTech_BlurV && mTech_Blend);
    }

    void GodRay::_initRenderTarget()
    {
        mRenderTarget = NULL;
        mTexture = NULL;

		RenderScheme * scheme = Engine::Instance()->GetRenderScheme();

		int width = scheme->GetColorTexture()->GetWidth() / 2;
		int height = scheme->GetColorTexture()->GetHeight() / 2;

        VideoBufferManager * video = VideoBufferManager::Instance();

        mRenderTarget = video->CreateRenderTarget("Core_GodRay_RT", width, height, FMT_R16F, MSAA_NONE);
        mTexture = video->CreateTextureRT("Core_GodRay_Texture", width, height, FMT_R16F);
    }

	void GodRay::_occlusion()
	{
		RenderSystem * render = RenderSystem::Instance();

		Camera * cam = World::Instance()->MainCamera();
		const DeviceProperty * dp = Engine::Instance()->GetDeviceProperty();

		int width = dp->Width;
		int height = dp->Height;

		float sunSize = Environment::Instance()->GetEvParam()->GodRaySunSize;
		Vec3 sunDir = Environment::Instance()->GetEvParam()->SunDir;
		float farclip = cam->GetFarClip() * 0.899f;
		Vec3 pos = cam->GetPosition() - farclip * sunDir;

		ShaderParam * uTransform = mTech_SunOcc->GetVertexShaderParamTable()->GetParam("gTransform");

		uTransform->SetUnifom(pos.x, pos.y, pos.z, sunSize);

		int pixels, pixelsToRendering;
		{
			mRender_Sun.rState.depthCheck = DCM_ALWAYS;
			mRender_Sun.rState.colorWrite = 0;

			render->BeginOcclusionQuery();

			render->Render(mTech_SunOcc, &mRender_Sun);

			pixels = render->EndOcclusionQuery();
		}

		{
			mRender_Sun.rState.depthCheck = DCM_ALWAYS;
			mRender_Sun.rState.colorWrite = 0;

			render->BeginOcclusionQuery();

			render->Render(mTech_SunOcc, &mRender_Sun);

			pixels = render->EndOcclusionQuery();
		}
		
		{
			mRender_Sun.rState.depthCheck = DCM_LESS;
			mRender_Sun.rState.colorWrite = 0;

			render->BeginOcclusionQuery();

			render->Render(mTech_SunOcc, &mRender_Sun);

			pixelsToRendering = render->EndOcclusionQuery();
		}

		{
			mRender_Sun.rState.depthCheck = DCM_ALWAYS;
			mRender_Sun.rState.colorWrite = CWE_ALL;
		}

		float k = float(pixelsToRendering + 1) / float(pixels + 1);

		if (k < 0.8f)
		{
			k = (0.8f - k) / 0.6f;

			mOccLighting = Math::Lerp(1.0f, 2.5f, k);
			mOccRadius = Math::Lerp(1.0f, 2.0f, k);
		}
		else
		{
			mOccLighting = 1;
			mOccRadius = 1;
		}
	}

    void GodRay::_renderSun()
    {
        RenderSystem * render = RenderSystem::Instance();

		render->SetRenderTarget(0, mRenderTarget.c_ptr());

        render->ClearBuffer(NULL, true, false, false, Color(0, 0, 0, 0));

        Camera * cam = World::Instance()->MainCamera();

        Color4 sunColor = Environment::Instance()->GetEvParam()->SunColor;
		float sunLum = Environment::Instance()->GetEvParam()->GodRaySunLum;
        float sunInner = Environment::Instance()->GetEvParam()->GodRaySunInner;
        float sunPower = Environment::Instance()->GetEvParam()->GodRaySunPower;
        Vec3 sunDir = Environment::Instance()->GetEvParam()->SunDir;
        float sunSize = Environment::Instance()->GetEvParam()->GodRaySunSize * mOccRadius;

        float farclip = cam->GetFarClip() * 0.9f;
        Vec3 pos = cam->GetPosition() - farclip * sunDir;

        ShaderParam * uTransform = mTech_Sun->GetVertexShaderParamTable()->GetParam("gTransform");
        //ShaderParam * uSunColor = mTech_Sun->GetPixelShaderParamTable()->GetParam("gSunColor");
        ShaderParam * uSunParam = mTech_Sun->GetPixelShaderParamTable()->GetParam("gSunParam");

        uTransform->SetUnifom(pos.x, pos.y, pos.z, sunSize);

        //uSunColor->SetUnifom(sunColor.r, sunColor.g, sunColor.b, sunColor.a);
		//uSunParam->SetUnifom(sunPower, sunLum, 0, 0);
		uSunParam->SetUnifom(sunInner, 1 / (0.5f - sunInner), 1, 1);

        render->Render(mTech_Sun, &mRender_Sun);
    }

    void GodRay::_godray(Texture * depthTex)
    {
        mRenderTarget->Stretch(mTexture.c_ptr());

        Camera * cam = World::Instance()->MainCamera();

        float uvStep = Environment::Instance()->GetEvParam()->GodRayUVStep;
        Vec3 sunDir = Environment::Instance()->GetEvParam()->SunDir;
        float sunSize = Environment::Instance()->GetEvParam()->GodRaySunSize;
        Vec3 sunPos = Environment::Instance()->GetEvParam()->SunPos;

        const Mat4 & matVP = cam->GetViewProjMatrix();

        Vec3 sunUV = sunPos * matVP;
        sunUV.x = (sunUV.x + 1) / 2;
        sunUV.y = (-sunUV.y + 1) / 2;

        ShaderParam * uGodRayParam = mTech_GodRay->GetPixelShaderParamTable()->GetParam("gGodRayParam");

        uGodRayParam->SetUnifom(sunUV.x, sunUV.y, 1 / uvStep, 0);

        SamplerState state;
        state.Address = TEXA_CLAMP;

        RenderSystem::Instance()->SetTexture(0, state, depthTex);
        RenderSystem::Instance()->SetTexture(1, state, mTexture.c_ptr());

        RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_GodRay);
    }

    void GodRay::_blur()
    {
		mRenderTarget->Stretch(mTexture.c_ptr());

		SamplerState state;
		state.Address = TEXA_CLAMP;

		RenderSystem::Instance()->SetTexture(0, state, mTexture.c_ptr());

		RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_BlurH);

		mRenderTarget->Stretch(mTexture.c_ptr());

		RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_BlurV);

		mRenderTarget->Stretch(mTexture.c_ptr());
    }

    void GodRay::_blend()
    {
		float sunLum = Environment::Instance()->GetEvParam()->GodRaySunLum;
		Color4 sunColor = Environment::Instance()->GetEvParam()->SunColor * sunLum * mOccLighting;
        float blendWeight = Environment::Instance()->GetEvParam()->GodRayBlendWeight;

        ShaderParam * uBlendWeight = mTech_Blend->GetPixelShaderParamTable()->GetParam("gBlendWeight");
		ShaderParam * uSunColor = mTech_Blend->GetPixelShaderParamTable()->GetParam("gSunColor");

		uSunColor->SetUnifom(sunColor.r, sunColor.g, sunColor.b, sunColor.a);
        uBlendWeight->SetUnifom(blendWeight, 0, 0, 0);

		SamplerState state;
		state.Address = TEXA_CLAMP;

		RenderSystem::Instance()->SetTexture(0, state, mTexture.c_ptr());

		RenderHelper::Instance()->DrawScreenQuad(BM_ALPHA_BLEND, mTech_Blend);
    }
}