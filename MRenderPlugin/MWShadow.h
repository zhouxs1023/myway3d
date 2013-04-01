#pragma once

#include "MRenderEntry.h"
#include "MWRenderSystem.h"

namespace Myway {

	class ShadowRenderQueue : public RenderQueue
	{
		DECLARE_ALLOC();

	public:
		ShadowRenderQueue() {}
		virtual ~ShadowRenderQueue() {}

		virtual void PushRenderer(const List<SceneNode *> & nodes)
		{
			List<SceneNode *>::ConstIterator whr = nodes.Begin();
			List<SceneNode *>::ConstIterator end = nodes.End();

			while (whr != end)
			{
				_pushRenderer(*whr);

				++whr;
			}
		}

		virtual void AddRenderer(Renderer * obj)
		{
			if (!obj->GetMaterial()->IsTransparency())
			{
				mSolidEntry.PushBack(obj);
			}
		}

	protected:
		void _pushRenderer(SceneNode * node)
		{
			if (node->_getVisibleMask())
			{
				SceneNode::MoverVisitor vr = node->GetMovers();

				while (!vr.Endof())
				{
					Mover * m = *vr.Cursor();

					if (m->IsVisible() && m->IsCastShadow())
						m->AddRenderQueue(this);

					++vr;
				}
			}
		}
	};

	class MRENDER_ENTRY Shadow
	{
	public:
		static const int K_NumShadowLayers = 3;

		static Event OnVisibleCull;
		static Event OnRenderDepth;

		struct CascadedMatrixs
		{
			Mat4 mView;
			Mat4 mProj;
			Mat4 mViewProj;
		};

	public:
		Shadow();
		~Shadow();

		void Do(Texture * depthTex);

		void SetDist(int index, float dist) { mDist[index] = dist; }
		float GetDist(int index) const { return mDist[index]; }

		void SetBias(int index, float bias) { mBias[index] = bias; }
		float getBias(int index) const { return mBias[index]; }

		void SetOffset(float off) { mOffset = off; }
		float GetOffset() const { return mOffset; }

		TexturePtr GetShadowMap() { return mTex_Shadow; }

		const CascadedMatrixs & GetCascadedMatrix(int layer);

		static bool IsVisible(const Aabb & bound, const Mat4 & matViewPorj);

	protected:
		void _initRT();

		Mat4 _calcuCropMatrix(int layer);
		Aabb _calcuAabb(const Vec3 * v);
		void _calcuCascadedMatrix(int layer);
		void _updateCamera();
		void _impVisibleCull();
		void _renderDepth(int layer);
		void _genShadowMap(int layer, Texture * depthTex);

	protected:
		float mDist[K_NumShadowLayers];
		float mBias[K_NumShadowLayers];
		float mOffset;

		Camera * mLightCamera;
		SceneNode * mLightCameraNode;
		Mat4 mInverseWorldCameraVP;
		CascadedMatrixs mCascadedMatrix[K_NumShadowLayers];

		RenderTargetPtr mRT_Depth;
		DepthStencilPtr mDepthStencil;
		TexturePtr mTex_Depth;

		RenderTargetPtr mRT_Shadow;
		TexturePtr mTex_Shadow;
		TexturePtr mTex_Random;

		VisibleCullResult mCullResult;
		ShadowRenderQueue mRenderQueue;
		Technique * mTech_ShadowDepth;
		Technique * mTech_ShadowDepthSkin;
		Technique * mTech_Shadow[K_NumShadowLayers];
	};

}