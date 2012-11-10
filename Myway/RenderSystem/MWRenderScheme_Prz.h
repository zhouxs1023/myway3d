#pragma once

#include "MWWorld.h"
#include "MWRenderScheme.h"
#include "MWRenderQueue.h"

#include "MWShaderProvider_Prz.h"
#include "MWRenderLoopMain_Prz.h"

namespace Myway
{
    class ShaderProvider_Main;

    class MW_ENTRY RS_Scheme : public RenderScheme
    {
    public:
        RS_Scheme();
        virtual ~RS_Scheme();

        virtual void DoRender();

        ShaderProvider_Main * GetMainShaderProvider() { return mShaderPrivderMain; }
        RenderLoop_Main * GetRenderLoopMain() { return mRenderLoopMain; }
        RenderQueue * GetRenderQueue() { return &mRenderQueue; }

    protected:
        VisibleCullResult mMainResult;

        ShaderProvider_Main * mShaderPrivderMain;

        RenderLoop_Main * mRenderLoopMain;

        RenderQueue mRenderQueue;
    };

}