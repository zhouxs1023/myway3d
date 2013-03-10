#include "MWDeferredRenderer.h"
#include "MWRenderEvent.h"

namespace Myway
{
    DeferredRenderer::DeferredRenderer()
    {
        mShaderPrivder = new ShaderProvider();
        mRenderLoop = new RenderLoop(this);
		mEnvironment = new Environment();
    }

    DeferredRenderer::~DeferredRenderer()
    {
		delete mEnvironment;
        delete mRenderLoop;
        delete mShaderPrivder;
    }

	void DeferredRenderer::Resize(int w, int h)
	{
		mRenderLoop->_resize(w, h);
		mEnvironment->_resize(w, h);
	}

    void DeferredRenderer::DoRender()
    {
        RenderEvent::OnPreRenderEvent(NULL, NULL);

        // render color
        Camera * cam = World::Instance()->MainCamera();
        mMainResult.nodes.Clear();
        mMainResult.lights.Clear();
        World::Instance()->ImpVisibleCull(mMainResult, cam, true, true);

        mRenderQueue.Clear();
        mRenderQueue.PushRenderer(mMainResult.nodes);

        mRenderLoop->DoRender();

        RenderEvent::OnPostRenderEvent(NULL, NULL);
    }
}