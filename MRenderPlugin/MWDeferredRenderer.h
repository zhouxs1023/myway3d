#pragma once

#include "MRenderEntry.h"
#include "MWWorld.h"
#include "MWRenderScheme.h"
#include "MWRenderQueue.h"

#include "MWShaderProvider.h"
#include "MWRenderLoop.h"
#include "MWMirrorRenderer.h"
#include "MWEnvironment.h"

namespace Myway
{
    class ShaderProvider;

    class MRENDER_ENTRY DeferredRenderer : public RenderScheme
    {
    public:
        DeferredRenderer();
        virtual ~DeferredRenderer();

		virtual void Resize(int w, int h);
        virtual void DoRender();
		virtual void RenderInMirror(const Plane & mirrorPlane);

        ShaderProvider * GetMainShaderProvider() { return mShaderPrivder; }
        RenderLoop * GetRenderLoopMain() { return mRenderLoop; }
        RenderQueue * GetRenderQueue() { return &mRenderQueue; }
		VisibleCullResult * GetCullResult() { return &mMainResult; }

		MirrorRenderQueue * GetMirrorRenderQueue() { return mMirrorRenderQueue; }
		MirrorRenderLoop * GetMirrorRenderLoop() { return mMirrorRenderLoop; }

		virtual Texture * GetDepthTexture() { return mRenderLoop->mTex_Depth.c_ptr(); };
		virtual Texture * GetColorTexture() { return mRenderLoop->mTex_Color.c_ptr(); };
		virtual Texture * GetNormalTexture() { return mRenderLoop->mTex_Normal.c_ptr(); };
		virtual Texture * GetMaterialTexture() { return mRenderLoop->mTex_Material.c_ptr(); };

    protected:
        VisibleCullResult mMainResult;
        ShaderProvider * mShaderPrivder;
        RenderLoop * mRenderLoop;
        RenderQueue mRenderQueue;
		Environment * mEnvironment;

		MirrorRenderQueue  * mMirrorRenderQueue;
		MirrorRenderLoop  * mMirrorRenderLoop;
		Camera * mMirrorCamera;
		Plane mMirrorPlane;
    };

}