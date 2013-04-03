#include "MWDeferredRenderer.h"
#include "MWRenderEvent.h"

namespace Myway
{
    DeferredRenderer::DeferredRenderer()
    {
        mShaderPrivder = new ShaderProvider();
        mRenderLoop = new RenderLoop(this);
		mEnvironment = new Environment();

		mMirrorRenderQueue = new MirrorRenderQueue();
		mMirrorRenderLoop = new MirrorRenderLoop(this);

		mMirrorCamera = World::Instance()->CreateCamera("Core_Water_Refl");
		World::Instance()->CreateSceneNode("Core_Water_Refl_Node")->Attach(mMirrorCamera);
    }

    DeferredRenderer::~DeferredRenderer()
    {
		World::Instance()->DestroyCamera(mMirrorCamera);

		delete mMirrorRenderLoop;
		delete mMirrorRenderQueue;

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

		RenderEvent::OnFilterCullResult(&mMainResult, cam);

		mRenderQueue.Clear();
		mRenderQueue.PushRenderer(mMainResult.nodes);

        mRenderLoop->DoRender();

        RenderEvent::OnPostRenderEvent(NULL, NULL);
    }

	void DeferredRenderer::RenderInMirror(const Plane & mirrorPlane)
	{
		Camera * mainCam = World::Instance()->MainCamera();

		mMirrorCamera->SetPosition(mainCam->GetPosition());
		mMirrorCamera->SetOrientation(mainCam->GetOrientation());
		mMirrorCamera->SetMirrorPlane(mirrorPlane);
		mMirrorCamera->EnableMirror(true);
		mMirrorCamera->SetNearClip(mainCam->GetNearClip());
		mMirrorCamera->SetFarClip(mainCam->GetFarClip());
		mMirrorCamera->SetAspect(mainCam->GetAspect());
		mMirrorCamera->SetFov(mainCam->GetFov());

		RenderSystem * render = RenderSystem::Instance();

		mMainResult.nodes.Clear();
		mMainResult.lights.Clear();

		World::Instance()->ImpVisibleCull(mMainResult, mMirrorCamera, false, false);

		RenderEvent::OnFilterCullResult(&mMainResult, mMirrorCamera);

		mMirrorRenderQueue->Clear();
		mMirrorRenderQueue->PushRenderer(mMainResult.nodes);

		render->SetViewTransform(mMirrorCamera->GetViewMatrix());
		render->SetProjTransform(mMirrorCamera->GetProjMatrix());

		RenderRegister::Instance()->SetClipPlane(mMirrorCamera->GetNearClip(), mMirrorCamera->GetFarClip());

		render->SetFlipCullMode(true);
		mMirrorRenderLoop->DoRender(mMirrorCamera);
		render->SetFlipCullMode(false);
	}
}