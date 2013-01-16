#include "MWRenderScheme_Prz.h"
#include "MWRenderEvent.h"
#include "MWEnvironment.h"

namespace Myway
{
    RS_Scheme::RS_Scheme()
    {
        mShaderPrivderMain = new ShaderProvider();

        mRenderLoopMain = new RenderLoop(this);
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
        World::Instance()->ImpVisibleCull(mMainResult, cam, true, true);

        mRenderQueue.Clear();
        mRenderQueue.PushRenderer(mMainResult.nodes);

        mRenderLoopMain->DoRender();

        RenderEvent::OnPostRenderEvent(NULL, NULL);
    }
}