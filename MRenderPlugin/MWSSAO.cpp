#include "MWSSAO.h"
#include "Engine.h"
#include "MWEnvironment.h"

namespace Myway {

	SSAO::SSAO()
	{
		_init();
	}

	SSAO::~SSAO()
	{
	}

	void SSAO::_init()
	{
		mRT_Ao = NULL;
		mTex_Ao = NULL;

		mRT_Quad = NULL;
		mTex_Quad = NULL;

		RenderScheme * scheme = Engine::Instance()->GetRenderScheme();

		int width = scheme->GetColorTexture()->GetWidth();
		int height = scheme->GetColorTexture()->GetHeight();

		VideoBufferManager * video = VideoBufferManager::Instance();

		mRT_Quad = video->CreateRenderTarget("Env_SSAO_RT_Quad", width / 2, height / 2, FMT_R16F, MSAA_NONE);
		mTex_Quad = video->CreateTextureRT("Env_SSAO_Tex_Quad0", width / 2, height / 2, FMT_R16F);

		mTex_Ao = video->CreateTextureRT("Env_SSAO_Tex", width, height, FMT_R16F);
		mRT_Ao = video->CreateRenderTarget(mTex_Ao);

		mTech_Ao = Environment::Instance()->GetShaderLib()->GetTechnique("SSAO");
		mTech_DownSample = Environment::Instance()->GetShaderLib()->GetTechnique("DownScale2x");
		mTech_BlurH = Environment::Instance()->GetShaderLib()->GetTechnique("BlurH");
		mTech_BlurV = Environment::Instance()->GetShaderLib()->GetTechnique("BlurV");
		mTech_Blend = Environment::Instance()->GetShaderLib()->GetTechnique("SSAOBlend");

		d_assert (mTech_Ao && mTech_Blend && mTech_BlurH && mTech_BlurV && mTech_DownSample);

		mTex_Random = video->Load2DTexture("random2.png", "Shaders\\random.png");
	}

	void SSAO::Resize(int w, int h)
	{
		_init();
	}

	void SSAO::Render(Texture * depthTex, Texture * normalTex)
	{
		RenderSystem * render = RenderSystem::Instance();

		RenderTarget * oldRt = render->GetRenderTarget(0);

		_renderAo(depthTex, normalTex);

		_downSample();

		_blur();

		render->SetRenderTarget(0, oldRt);

		_blend();
	}

	void SSAO::_renderAo(Texture * depthTex, Texture * normalTex)
	{
		RenderSystem * render = RenderSystem::Instance();

		render->SetRenderTarget(0, mRT_Ao.c_ptr());

		render->ClearBuffer(NULL, true, false, false, Color::White);

		Camera * cam = World::Instance()->MainCamera();
		const Vec3 * corner = cam->GetCorner();

		Vec3 cornerLeftTop = corner[4];
		Vec3 cornerRightDir = corner[5] - corner[4];
		Vec3 cornerDownDir = corner[6] - corner[4];

		float width = float(mRT_Ao->GetWidth());
		float height = float(mRT_Ao->GetHeight());

		ShaderParam * uCornerLeftTop = mTech_Ao->GetPixelShaderParamTable()->GetParam("gCornerLeftTop");
		ShaderParam * uCornerRightDir = mTech_Ao->GetPixelShaderParamTable()->GetParam("gCornerRightDir");
		ShaderParam * uCornerDownDir = mTech_Ao->GetPixelShaderParamTable()->GetParam("gCornerDownDir");
		ShaderParam * uFar = mTech_Ao->GetPixelShaderParamTable()->GetParam("far");
		ShaderParam * uMatProj = mTech_Ao->GetPixelShaderParamTable()->GetParam("ptMat");


		uCornerLeftTop->SetUnifom(cornerLeftTop.x, cornerLeftTop.y, cornerLeftTop.z, 0);
		uCornerRightDir->SetUnifom(cornerRightDir.x, cornerRightDir.y, cornerRightDir.z, 0);
		uCornerDownDir->SetUnifom(cornerDownDir.x, cornerDownDir.y, cornerDownDir.z, 0);
		//uFar->SetUnifom(cam->GetFarClip(), 0, 0, 0);
		uMatProj->SetMatrix(cam->GetProjMatrix());

		SamplerState state;
		state.Address = TEXA_CLAMP;
		state.Filter = TEXF_POINT;

		RenderSystem::Instance()->SetTexture(0, state, depthTex);
		RenderSystem::Instance()->SetTexture(1, state, normalTex);

		state.Address = TEXA_WRAP;
		state.Filter = TEXF_DEFAULT;
		RenderSystem::Instance()->SetTexture(2, state, mTex_Random.c_ptr());

		RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_Ao);
	}

	void SSAO::_downSample()
	{
		RenderSystem * render = RenderSystem::Instance();

		render->SetRenderTarget(0, mRT_Quad.c_ptr());

		float invWidth = 1.0f / mRT_Quad->GetWidth();
		float invHeight = 1.0f / mRT_Quad->GetHeight();

		Vec4 uvOffs[4] = {
			Vec4(0, 0, 0, 0), Vec4(invWidth, 0, 0, 0),
			Vec4(0, invHeight, 0, 0), Vec4(invWidth, invHeight, 0, 0),
		};

		ShaderParam * uUVOffs = mTech_DownSample->GetPixelShaderParamTable()->GetParam("gUVOffsets");
		uUVOffs->SetVector(uvOffs, 4);

		SamplerState state;
		render->SetTexture(0, state, mTex_Ao.c_ptr());

		RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_DownSample);

		mRT_Quad->Stretch(mTex_Quad.c_ptr());
	}

	void SSAO::_blur()
	{
		RenderSystem * render = RenderSystem::Instance();

		render->SetRenderTarget(0, mRT_Quad.c_ptr());

		SamplerState state;
		state.Address = TEXA_CLAMP;

		RenderSystem::Instance()->SetTexture(0, state, mTex_Quad.c_ptr());

		RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_BlurH);

		mRT_Quad->Stretch(mTex_Quad.c_ptr());

		RenderSystem::Instance()->SetTexture(0, state, mTex_Quad.c_ptr());

		RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_BlurV);

		mRT_Quad->Stretch(mTex_Quad.c_ptr());
	}

	void SSAO::_blend()
	{
		SamplerState state;
		state.Address = TEXA_CLAMP;

		RenderSystem::Instance()->SetTexture(0, state, mTex_Quad.c_ptr());

		RenderHelper::Instance()->DrawScreenQuad(BM_MULTIPLY, mTech_Blend);
	}
}