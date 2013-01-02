#include "MWShadow.h"
#include "MWRenderHelper.h"
#include "MWEnvironment.h"

namespace Myway {

	Event Shadow::OnVisibleCull;
	Event Shadow::OnRenderDepth;

	Shadow::Shadow()
	{
		mLightCamera = World::Instance()->CreateCamera("Core_LightCamera");
		mLightCameraNode = World::Instance()->CreateSceneNode();

		mLightCameraNode->Attach(mLightCamera);

		mDist[0] = 100;
		mDist[1] = 400;
		mDist[2] = 1200;
		mDist[3] = 2000;

		mBias[0] = 0.0005f;
		mBias[1] = 0.0015f;
		mBias[2] = 0.003f;
		mBias[3] = 0.008f;

		mOffset = 5000;

		_initRT();

		mTech_ShadowDepth = Environment::Instance()->GetShaderLib()->GetTechnique("ShadowDepth");
		mTech_Shadow = Environment::Instance()->GetShaderLib()->GetTechnique("Shadow");
	}

	Shadow::~Shadow()
	{
		World::Instance()->DestroyCamera(mLightCamera);
		World::Instance()->DestroySceneNode(mLightCameraNode);
	}

	void Shadow::_initRT()
	{
		mTex_Depth = VideoBufferManager::Instance()->CreateTextureRT("Core_Shadow_DepthTex", 2048, 2048, FMT_R32F);
		mRT_Depth = VideoBufferManager::Instance()->CreateRenderTarget(mTex_Depth);

		mDepthStencil = VideoBufferManager::Instance()->CreateDepthStencil("Core_Shadow_DepthStencil", 2048, 2048, FMT_D24S8, MSAA_NONE);

		mTex_Shadow = VideoBufferManager::Instance()->CreateTextureRT("Core_Shadow_Tex", -1, -1, FMT_X8R8G8B8);
		mRT_Shadow = VideoBufferManager::Instance()->CreateRenderTarget(mTex_Shadow);
	}

	void Shadow::Do(Texture * depthTex)
	{
		RenderSystem * render = RenderSystem::Instance();

		RenderTarget * oldRt = render->GetRenderTarget(0);
		DepthStencil * oldDs = render->GetDepthStencil();

		_updateCamera();

		/*for (int i = 0; i < 4; ++i)
		{
		Mat4 matCrop = _calcuCropMatrix(i);
		mCascadedViewProjMatrix[i] = mLightCamera->GetViewProjMatrix() * matCrop;
		}*/

		_impVisibleCull();

		RS_BeginEvent("ShadowMap");


		for (int i = 0; i < K_NumShadowLayers; ++i)
		{
			_calcuCascadedMatrix(i);
			_renderDepth(i);
			_genShadowMap(i, depthTex);
		}

		RS_EndEvent();

		render->SetRenderTarget(0, oldRt);
		render->SetDepthStencil(oldDs);
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
		Vec3 frustum[8];

		float nearClip = mDist[layer];
		float farClip = mDist[layer + 1];

		World::Instance()->MainCamera()->GetWorldCorner(frustum, nearClip, farClip);
		Aabb cropBB = _calcuAabb(frustum);

		cropBB.minimum.z = 0.0f; 

		float scaleX, scaleY;
		float offsetX, offsetY;
		scaleX = 2.0f / (cropBB.maximum.x - cropBB.minimum.x);
		scaleY = 2.0f / (cropBB.maximum.y - cropBB.minimum.y);
		offsetX = -cropBB.minimum.x * scaleX - 1;
		offsetY = -cropBB.minimum.y * scaleY - 1;

		return Mat4(scaleX,		0,			0,	0,
					0,			scaleY,     0,	0,
					0,			0,			1,	0,
					offsetX,	offsetY,	0,	1);
	}

	void Shadow::_calcuCascadedMatrix(int layer)
	{
		Camera * worldCam = World::Instance()->MainCamera();

		mInverseWorldCameraVP = World::Instance()->MainCamera()->GetViewProjMatrix().Inverse();

		float nearClip, farClip = mDist[layer];

		if (layer == 0)
			nearClip = worldCam->GetNearClip();
		else
			nearClip = mDist[layer - 1];

		Vec3 xAixs = worldCam->GetDirection();
		Vec3 yAixs = worldCam->GetUp();
		Vec3 zAixs = Environment::Instance()->GetEvParam()->LightDir;

		if (Math::Abs(zAixs.Dot(yAixs)) > 0.99f)
		{
			yAixs = zAixs.CrossN(xAixs);
			xAixs = yAixs.CrossN(zAixs);
		}
		else
		{
			xAixs = yAixs.CrossN(zAixs);
			yAixs = zAixs.CrossN(xAixs);
		}

		if (xAixs.Dot(worldCam->GetDirection()) < 0)
			xAixs = -xAixs;

		yAixs = zAixs.CrossN(xAixs);

		Mat4 matView, matProj;
		Quat qOrient = Quat::S_FromAxis(xAixs, yAixs, zAixs);

		matView.MakeViewLH(worldCam->GetPosition(), qOrient);

		Vec3 corner[8], t_corner[8];

		worldCam->GetWorldCorner(t_corner, nearClip, farClip);

		float dist = t_corner[4].Distance(t_corner[5]);

		for (int i = 0; i < 8; ++i)
		{
			corner[i] = t_corner[i] * matView;
		}

		Aabb aabb = Aabb::Invalid;

		for (int i = 0; i < 8; ++i)
		{
			aabb.minimum = aabb.minimum.Minimum(corner[i]);
			aabb.maximum = aabb.maximum.Maximum(corner[i]);
		}

		Vec3 center = aabb.GetCenter();
		float width = aabb.GetWidth();
		float height = aabb.GetHeight();
		float depth = aabb.GetDepth();

		center *= matView.Inverse();

		Vec3 lightPos = center - zAixs * mOffset;

		matView.MakeViewLH(lightPos, qOrient);
		matProj.MakeOrthoLH(width, height, nearClip, mOffset + depth);

		mCascadedViewProjMatrix[layer] = matView * matProj;
	}

	void Shadow::_updateCamera()
	{
		Camera * worldCam = World::Instance()->MainCamera();

		mInverseWorldCameraVP = World::Instance()->MainCamera()->GetViewProjMatrix().Inverse();

		float nearClip = worldCam->GetNearClip();
		float farClip = mDist[3];

		Vec3 xAixs = worldCam->GetDirection();
		Vec3 yAixs = worldCam->GetUp();
		Vec3 zAixs = Environment::Instance()->GetEvParam()->LightDir;

		if (Math::Abs(zAixs.Dot(yAixs)) > 0.99f)
		{
			yAixs = zAixs.CrossN(xAixs);
			xAixs = yAixs.CrossN(zAixs);
		}
		else
		{
			xAixs = yAixs.CrossN(zAixs);
			yAixs = zAixs.CrossN(xAixs);
		}

		if (xAixs.Dot(worldCam->GetDirection()) < 0)
			xAixs = -xAixs;

		yAixs = zAixs.CrossN(xAixs);

		Mat4 matView;
		Quat qOrient = Quat::S_FromAxis(xAixs, yAixs, zAixs);

		matView.MakeViewLH(worldCam->GetPosition(), qOrient);

		Vec3 corner[8], t_corner[8];

		worldCam->GetWorldCorner(t_corner, nearClip, farClip);

		float dist = t_corner[4].Distance(t_corner[5]);

		for (int i = 0; i < 8; ++i)
		{
			corner[i] = t_corner[i] * matView;
		}

		Aabb aabb = Aabb::Invalid;

		for (int i = 0; i < 8; ++i)
		{
			aabb.minimum = aabb.minimum.Minimum(corner[i]);
			aabb.maximum = aabb.maximum.Maximum(corner[i]);
		}

		Vec3 center = aabb.GetCenter();
		float width = aabb.GetWidth();
		float height = aabb.GetHeight();
		float depth = aabb.GetDepth();

		center *= matView.Inverse();

		Vec3 lightPos = center - zAixs * mOffset;

		mLightCamera->SetPosition(lightPos);
		mLightCamera->SetOrientation(qOrient);
		mLightCamera->SetOrthoWidth(width);
		mLightCamera->SetOrthoHeight(height);
		mLightCamera->SetNearClip(nearClip);
		mLightCamera->SetFarClip(mOffset + depth);
		mLightCamera->SetProjectionType(PROJTYPE_ORTHO);
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

	bool Shadow::_isVisible(const Aabb & bound, const Mat4 & matViewPorj)
	{
		Vec3 point[8];

		bound.GetPoints(point);

		Math::VecTransform(point, point, matViewPorj, 8);

		Vec3 vMin, vMax;

		vMin = point[0];
		vMax = point[0];

		for (int i = 1; i < 8; ++i)
		{
			Math::VecMinimum(vMin, vMin, point[i]);
			Math::VecMaximum(vMax, vMax, point[i]);
		}

		if (vMax.x < -1 || vMin.x > +1 ||
			vMax.y < -1 || vMin.y > +1 ||
			vMax.z < -0 || vMin.z > +1)
			return false;

		return true;
	}

	void Shadow::_renderDepth(int layer)
	{
		RenderSystem * render = RenderSystem::Instance();

		render->SetRenderTarget(0, mRT_Depth.c_ptr());
		render->SetDepthStencil(mDepthStencil.c_ptr());

		render->ClearBuffer(NULL, true, true, false, Color::White);

		const Array<Renderer *> & objs = mRenderQueue.GetSolidRender();

		ShaderParam * uMatWVP = mTech_ShadowDepth->GetVertexShaderParamTable()->GetParam("matWVP");

		for (int j = 0; j < objs.Size(); ++j)
		{
			Renderer * rd = objs[j];

			if (!_isVisible(rd->GetWorldAabb(), mCascadedViewProjMatrix[layer]))
				continue;

			bool skined = (rd->GetBlendMatrix(NULL) > 0);

			Mat4 form;

			rd->GetWorldTransform(&form);

			form *= mCascadedViewProjMatrix[layer];

			uMatWVP->SetMatrix(form);

			render->Render(mTech_ShadowDepth, rd);
		}

		OnRenderDepth(this, &layer);
	}

	void Shadow::_genShadowMap(int layer, Texture * depthTex)
	{
		RenderSystem * render = RenderSystem::Instance();

		render->SetRenderTarget(0, mRT_Shadow.c_ptr());
		render->SetRenderTarget(1, NULL);
		render->SetRenderTarget(2, NULL);
		render->SetRenderTarget(3, NULL);
		render->SetDepthStencil(NULL);

		if (layer == 0)
			render->ClearBuffer(NULL, true, false, false, Color::White);

		Camera * cam = World::Instance()->MainCamera();
		const Vec3 * corner = cam->GetCorner();

		Mat4 matInverseView = cam->GetViewMatrix().Inverse();

		Vec3 cornerLeftTop = corner[4];
		Vec3 cornerRightDir = corner[5] - corner[4];
		Vec3 cornerDownDir = corner[6] - corner[4];

		Vec4 shadowInfo = Vec4::Zero;

		if (layer == 0)
			shadowInfo.x = cam->GetNearClip();
		else
			shadowInfo.x = mDist[layer - 1];

		shadowInfo.y = mDist[layer];
		shadowInfo.z = mBias[layer];

		ShaderParam * uShadowInfo = mTech_Shadow->GetPixelShaderParamTable()->GetParam("gShadowInfo");
		ShaderParam * uMatShadow = mTech_Shadow->GetPixelShaderParamTable()->GetParam("matShadow");
		ShaderParam * uCornerLeftTop = mTech_Shadow->GetPixelShaderParamTable()->GetParam("gCornerLeftTop");
		ShaderParam * uCornerRightDir = mTech_Shadow->GetPixelShaderParamTable()->GetParam("gCornerRightDir");
		ShaderParam * uCornerDownDir = mTech_Shadow->GetPixelShaderParamTable()->GetParam("gCornerDownDir");

		uCornerLeftTop->SetUnifom(cornerLeftTop.x, cornerLeftTop.y, cornerLeftTop.z, 0);
		uCornerRightDir->SetUnifom(cornerRightDir.x, cornerRightDir.y, cornerRightDir.z, 0);
		uCornerDownDir->SetUnifom(cornerDownDir.x, cornerDownDir.y, cornerDownDir.z, 0);

		uShadowInfo->SetUnifom(shadowInfo.x, shadowInfo.y, shadowInfo.z, 0);
		uMatShadow->SetMatrix(matInverseView * mCascadedViewProjMatrix[layer]);
			
		SamplerState state;
		state.Address = TEXA_CLAMP;
		state.Filter = TEXF_POINT;

		render->SetTexture(0, state, depthTex);

		state.Address = TEXA_BORDER;
		state.BorderColor = Color::White;
		render->SetTexture(1, state, mTex_Depth.c_ptr());

		RenderHelper::Instance()->DrawScreenQuad(BM_ALPHA_TEST, mTech_Shadow);
	}
}