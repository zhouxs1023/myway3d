#include "MWSoftLeaf.h"
#include "MWEnvironment.h"
#include "Engine.h"

namespace Myway {

	SoftLeaf::SoftLeaf()
		: OnResize(&RenderEvent::OnResize, this, &SoftLeaf::_resize)
	{
		_resize(&RenderEvent::OnResize);
	}

	SoftLeaf::~SoftLeaf()
	{
	}

	void SoftLeaf::Do(Texture * colorTex, Texture * depthTex)
	{
		RS_RenderEvent(SoftLeaf);

		RenderTarget * oldRT = RenderSystem::Instance()->GetRenderTarget(0);

		RenderSystem::Instance()->SetRenderTarget(0, mRT_Quad.c_ptr());

		_downSample(colorTex, depthTex);

		_blur();

		RenderSystem::Instance()->SetRenderTarget(0, oldRT);

		_blend();
	}

	void SoftLeaf::_resize(Event * sender)
	{
		mTex_Quad = NULL;
		mRT_Quad = NULL;

		const DeviceProperty * dp = Engine::Instance()->GetDeviceProperty();

		int width = dp->Width;
		int height = dp->Height;

		VideoBufferManager * video = VideoBufferManager::Instance();

		mTex_Quad = video->CreateTextureRT("SoftLeaf_Tex_Quad", width / 2, height / 2, FMT_A16B16G16R16F);
		mRT_Quad = video->CreateRenderTarget("SoftLeaf_RT_Quad", width / 2, height / 2, FMT_A16B16G16R16F, MSAA_NONE);

		mTech_BlurH = Environment::Instance()->GetShaderLib()->GetTechnique("BlurH");
		mTech_BlurV = Environment::Instance()->GetShaderLib()->GetTechnique("BlurV");

		mTech_DownSample = Environment::Instance()->GetShaderLib()->GetTechnique("SoftLeaf_DownSample");
		mTech_Blend = Environment::Instance()->GetShaderLib()->GetTechnique("SoftLeaf_Blend");

		d_assert (mTech_DownSample && mTech_BlurH && mTech_BlurV && mTech_Blend);
	}

	void SoftLeaf::_downSample(Texture * colorTex, Texture * depthTex)
	{
		RenderSystem * render = RenderSystem::Instance();

		float invWidth = 1.0f / mRT_Quad->GetWidth();
		float invHeight = 1.0f / mRT_Quad->GetHeight();

		Vec4 uvOffs[4] = {
			Vec4(0, 0, 0, 0), Vec4(invWidth, 0, 0, 0),
			Vec4(0, invHeight, 0, 0), Vec4(invWidth, invHeight, 0, 0),
		};

		ShaderParam * uUVOffs = mTech_DownSample->GetPixelShaderParamTable()->GetParam("gUVOffsets");
		uUVOffs->SetVector(uvOffs, 4);

		SamplerState state;
		state.Address = TEXA_CLAMP;

		render->SetTexture(0, state, colorTex);
		render->SetTexture(1, state, depthTex);

		RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_DownSample);

		mRT_Quad->Stretch(mTex_Quad.c_ptr());
	}

	void SoftLeaf::_blur()
	{
		RenderSystem * render = RenderSystem::Instance();

		SamplerState state;
		state.Address = TEXA_CLAMP;
		render->SetTexture(0, state, mTex_Quad.c_ptr());

		{
			RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_BlurH);
			mRT_Quad->Stretch(mTex_Quad.c_ptr());
		}

		{
			RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_BlurV);
			mRT_Quad->Stretch(mTex_Quad.c_ptr());
		}
	}

	void SoftLeaf::_blend()
	{
		RenderSystem * render = RenderSystem::Instance();

		SamplerState state;
		state.Address = TEXA_CLAMP;
		render->SetTexture(0, state, mTex_Quad.c_ptr());

		RenderHelper::Instance()->DrawScreenQuad(BM_ALPHA_BLEND, mTech_Blend);
	}
}