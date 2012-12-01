#include "MWRenderScheme_Prz.h"
#include "MWRenderEvent.h"

namespace Myway
{
    RS_Scheme::RS_Scheme()
    {
        mShaderPrivderMain = new ShaderProvider_Main();

        mRenderLoopMain = new RenderLoop_Main(this);
    }

    RS_Scheme::~RS_Scheme()
    {
        delete mRenderLoopMain;

        delete mShaderPrivderMain;
    }

    void RS_Scheme::DoRender()
    {
        RenderEvent::OnPreRenderEvent(NULL, NULL);

        // render color
        Camera * cam = World::Instance()->MainCamera();
        mMainResult.nodes.Clear();
        mMainResult.lights.Clear();
        World::Instance()->ImpVisibleCull(mMainResult, cam, true);

        mRenderQueue.Clear();
        mRenderQueue.PushRenderer(mMainResult.nodes);

        mRenderLoopMain->DoRender();

        RenderEvent::OnPostRenderEvent(NULL, NULL);
    }
}