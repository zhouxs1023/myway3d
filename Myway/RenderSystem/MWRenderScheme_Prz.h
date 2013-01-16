#pragma once

#include "MWWorld.h"
#include "MWRenderScheme.h"
#include "MWRenderQueue.h"

#include "MWShaderProvider_Prz.h"
#include "MWRenderLoopMain_Prz.h"

namespace Myway
{
    class ShaderProvider;

    class MW_ENTRY RS_Scheme : public RenderScheme
    {
    public:
        RS_Scheme();
        virtual ~RS_Scheme();

        virtual void DoRender();

        ShaderProvider * GetMainShaderProvider() { return mShaderPrivderMain; }
        RenderLoop * GetRenderLoopMain() { return mRenderLoopMain; }
        RenderQueue * GetRenderQueue() { return &mRenderQueue; }
		VisibleCullResult * GetCullResult() { return &mMainResult; }

		virtual Texture * GetDepthTexture() { return mRenderLoopMain->mTex_Depth.c_ptr(); };
		virtual Texture * GetColorTexture() { return mRenderLoopMain->mTex_Color.c_ptr(); };
		virtual Texture * GetNormalTexture() { return mRenderLoopMain->mTex_Normal.c_ptr(); };
		virtual Texture * GetMaterialTexture() { return mRenderLoopMain->mTex_Material.c_ptr(); };

    protected:
        VisibleCullResult mMainResult;

        ShaderProvider * mShaderPrivderMain;

        RenderLoop * mRenderLoopMain;

        RenderQueue mRenderQueue;
    };

}