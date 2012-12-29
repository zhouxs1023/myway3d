#pragma once

#include "MWRenderSystem.h"

namespace Myway {

	class MW_ENTRY Shadow
	{
	public:
		static const int K_NumShadowLayers = 4;

		static Event OnVisibleCull;
		static Event OnRenderDepth;

	public:
		Shadow();
		~Shadow();

		void Do(Texture * depthTex);

		void SetDist(int index, float dist) { d_assert (index + 1 < K_NumShadowLayers); mDist[index + 1] = dist; }
		float GetDist(int index) const { d_assert (index + 1 < K_NumShadowLayers); return mDist[index + 1]; }

		void SetOffset(float off) { mOffset = off; }
		float GetOffset() const { return mOffset; }

		TexturePtr GetShadowMap() { return mTex_Shadow; }

	protected:
		void _initRT();

		Mat4 _calcuCropMatrix(int layer);
		Aabb _calcuAabb(const Vec3 * v);
		void _updateCamera();
		void _impVisibleCull();
		void _renderDepth();
		void _genShadowMap(Texture * depthTex);


	protected:
		float mDist[K_NumShadowLayers + 1];
		float mOffset;

		Camera * mLightCamera;
		SceneNode * mLightCameraNode;
		Mat4 mInverseWorldCameraVP;
		Mat4 mCascadedViewProjMatrix[K_NumShadowLayers];

		RenderTargetPtr mRT_Depth[K_NumShadowLayers];
		DepthStencilPtr mDepthStencil;
		TexturePtr mTex_Depth[K_NumShadowLayers];

		RenderTargetPtr mRT_Shadow;
		TexturePtr mTex_Shadow;

		VisibleCullResult mCullResult;
		RenderQueue mRenderQueue;
		Technique * mTech_ShadowDepth;
		Technique * mTech_ShadowDepthSkin;
		Technique * mTech_Shadow;
	};

}