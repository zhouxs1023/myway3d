#include "MWShadow.h"
#include "MWRenderHelper.h"
#include "MWEnvironment.h"

namespace Myway {

	Event Shadow::OnVisibleCull;
	Event Shadow::OnRenderDepth;

	Shadow::Shadow()
	{
		mLightCamera = World::Instance()->CreateCamera("Core_LightCamera");

		mDist[0] = 0;
		mDist[1] = 50;
		mDist[2] = 150;
		mDist[3] = 500;
		mDist[4] = 1000;

		_initRT();

		mTech_ShadowDepth = Environment::Instance()->GetShaderLib()->GetTechnique("ShadowDepth");
		mTech_Shadow = Environment::Instance()->GetShaderLib()->GetTechnique("Shadow");
	}

	Shadow::~Shadow()
	{
	}

	void Shadow::_initRT()
	{
		for (int i = 0; i < K_NumShadowLayers; ++i)
		{
			mTex_Depth[i] = VideoBufferManager::Instance()->CreateTextureRT(TString128("Core_Shadow_Tex") + i, 2048, 2048, FMT_R16F);
			mRT_Depth[i] = VideoBufferManager::Instance()->CreateRenderTarget(mTex_Depth[i]);
		}

		mDepthStencil = VideoBufferManager::Instance()->CreateDepthStencil("Core_Shadow_DepthStencil", 2048, 2048, FMT_D24S8, MSAA_NONE);

		mTex_Shadow = VideoBufferManager::Instance()->CreateTextureRT("Core_Shadow_Tex", -1, -1, FMT_X8R8G8B8);
		mRT_Shadow = VideoBufferManager::Instance()->CreateRenderTarget(mTex_Shadow);
	}

	void Shadow::Do()
	{
		_updateCamera();

		for (int i = 0; i < 4; ++i)
		{
			Mat4 matCrop = _calcuCropMatrix(i);
			mCascadedViewProjMatrix[i] = mLightCamera->GetViewProjMatrix() * matCrop;
		}

		_impVisibleCull();

		_renderDepth();

		_genShadowMap();
	}

	Aabb Shadow::_calcuAabb(const Vec3 * v)
	{
		Aabb bound = Aabb::Invalid;

		const Mat4 & matViewProj = mLightCamera->GetViewProjMatrix();

		for (int i = 0; i < 8; ++i)
		{
			Vec3 pv = v[i] * matViewProj;

			bound.minimum = bound.minimum.Minimum(pv);
			bound.maximum = bound.maximum.Maximum(pv);
		}

		return bound;
	}

	Mat4 Shadow::_calcuCropMatrix(int layer)
	{
		float s0, s1;
		s0 = mDist[layer];
		s1 = mDist[layer + 1];

		Vec3 vs0(0, 0, s0), vs1(0, 0, s1);
		Math::VecTransform(vs0, vs0, mLightCamera->GetProjMatrix());
		Math::VecTransform(vs1, vs1, mLightCamera->GetProjMatrix());
		s0 = vs0.z, s1 = vs1.z;

		Vec3 frustum[8];

		frustum[0] = Vec3(-1, 1, s0);
		frustum[1] = Vec3(-1, 1, s1);
		frustum[2] = Vec3(1, 1, s0);
		frustum[3] = Vec3(1, 1, s1);
		frustum[4] = Vec3(-1, -1, s0);
		frustum[5] = Vec3(-1, -1, s1);
		frustum[6] = Vec3(1, -1, s0);
		frustum[7] = Vec3(1, -1, s1);

		Math::VecTransform(frustum, frustum, mInverseWorldCameraVP, 8);

		Aabb cropBB = _calcuAabb(frustum);

		cropBB.minimum.z = 0.0f; 

		float scaleX, scaleY, scaleZ;
		float offsetX, offsetY, offsetZ;
		scaleX = 2.0f / (cropBB.maximum.x - cropBB.minimum.x);
		scaleY = 2.0f / (cropBB.maximum.y - cropBB.minimum.y);
		offsetX = -0.5f * (cropBB.maximum.x + cropBB.minimum.x) * scaleX;
		offsetY = -0.5f * (cropBB.maximum.y + cropBB.minimum.y) * scaleY;
		scaleZ = 1.0f / (cropBB.maximum.z - cropBB.minimum.z);
		offsetZ = -cropBB.minimum.z * scaleZ; 

		return Mat4(scaleX,		0,			0,	0,
					0,			scaleY,     0,	0,
					0,			0,			0,	0,
					offsetX,	offsetY,	1,	1);
	}

	void Shadow::_updateCamera()
	{
		Camera * worldCam = World::Instance()->MainCamera();

		float nearClip = worldCam->GetNearClip();
		float farClip = worldCam->GetFarClip();

		mDist[0] = nearClip;

		Vec3 xAxis = Vec3::UnitX;
		Vec3 yAxis = Vec3::UnitY;
		Vec3 zAxis = Environment::Instance()->GetEvParam()->LightDir;
		
		if (Math::Abs(yAxis.Dot(zAxis)) >= 0.999f)
			yAxis = Vec3::UnitZ;

		xAxis = yAxis.CrossN(zAxis);
		yAxis = zAxis.CrossN(xAxis);

		Quat ort;
		ort.FromAxis(xAxis, yAxis, zAxis);
		mLightCamera->SetOrientation(ort);

		const Vec3 * worldCorner = World::Instance()->MainCamera()->GetWorldCorner();

		//transform light space
		Vec3 corner[8];
		Mat4 lightView;
		Math::MatViewLH(lightView, worldCam->GetPosition() - zAxis * mOffset, worldCam->GetPosition(), yAxis);
		Math::VecTransform(corner, worldCorner, lightView, 8);

		Vec3 minimum = corner[0], maximum = corner[0];

		for (int i = 1; i < 8; ++i)
		{
			Math::VecMinimum(minimum, minimum, corner[i]);
			Math::VecMaximum(maximum, maximum, corner[i]);
		}

		float width = maximum.x - minimum.x;
		float height = maximum.y - minimum.y;

		//transform world space
		Math::MatInverse(lightView, lightView);
		Math::VecTransform(minimum, minimum, lightView);
		Math::VecTransform(maximum, maximum, lightView);

		Vec3 eye = (maximum + minimum) * 0.5f;
		eye += -zAxis * mOffset;

		Math::MatViewLH(lightView, eye, eye + zAxis, yAxis);
		Math::VecTransformZ(farClip, maximum, lightView);

		mLightCamera->SetPosition(eye);
		mLightCamera->SetOrientation(ort);

		mLightCamera->SetProjectionType(PROJTYPE_ORTHO);
		mLightCamera->SetOrthoWidth(width);
		mLightCamera->SetOrthoHeight(height);
		mLightCamera->SetNearClip(nearClip);
		mLightCamera->SetFarClip(farClip);

		mInverseWorldCameraVP = World::Instance()->MainCamera()->GetViewProjMatrix().Inverse();
	}

	void Shadow::_impVisibleCull()
	{
		mCullResult.nodes.Clear();
		mCullResult.lights.Clear();

		World::Instance()->ImpVisibleCull(mCullResult, mLightCamera, false);

		OnVisibleCull(mLightCamera, NULL);

		mRenderQueue.Clear();
		mRenderQueue.PushRenderer(mCullResult.nodes);
	}

	void Shadow::_renderDepth()
	{
		RenderSystem * render = RenderSystem::Instance();

		for (int i = 0; i < K_NumShadowLayers; ++i)
		{
			render->SetRenderTarget(0, mRT_Depth[i].c_ptr());
			render->SetDepthStencil(mDepthStencil.c_ptr());

			render->ClearBuffer(NULL, true, true, false);

			const Array<Renderer *> & objs = mRenderQueue.GetSolidRender();

			ShaderParam * uMatWVP = mTech_ShadowDepth->GetVertexShaderParamTable()->GetParam("matWVP");

			for (int j = 0; j < objs.Size(); ++j)
			{
				Renderer * rd = objs[j];

				bool skined = (rd->GetBlendMatrix(NULL) > 0);

				Mat4 form;

				rd->GetWorldTransform(&form);

				form *= mCascadedViewProjMatrix[i];

				uMatWVP->SetMatrix(form);
				
				render->Render(mTech_ShadowDepth, rd);
			}

			OnRenderDepth(this, &i);
		}
	}

	void Shadow::_genShadowMap()
	{
		RenderSystem * render = RenderSystem::Instance();

		render->SetRenderTarget(0, mRT_Shadow.c_ptr());
		render->SetRenderTarget(1, NULL);
		render->SetRenderTarget(2, NULL);
		render->SetRenderTarget(3, NULL);
		render->SetDepthStencil(NULL);

		for (int i = 0; i < K_NumShadowLayers; ++i)
		{
			ShaderParam * uShadowInfo = mTech_Shadow->GetPixelShaderParamTable()->GetParam("gShadowInfo");
			ShaderParam * uShadowInfo = mTech_Shadow->GetPixelShaderParamTable()->GetParam("gShadowInfo");
			ShaderParam * uShadowInfo = mTech_Shadow->GetPixelShaderParamTable()->GetParam("gShadowInfo");
			ShaderParam * uShadowInfo = mTech_Shadow->GetPixelShaderParamTable()->GetParam("gShadowInfo");
			ShaderParam * uShadowInfo = mTech_Shadow->GetPixelShaderParamTable()->GetParam("gShadowInfo");

			uShadowInfo->SetUnifom(mDist[i], mDist[i + 1], 0, 0);



			RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_Shadow);
		}

		//for (int i = 0; i < K_NumShadowLayers; ++i)
		//{
		//}
	}
}