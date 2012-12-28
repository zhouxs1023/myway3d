#include "stdafx.h"

#include "xApp.h"
#include "xEvent.h"
#include "xDebugRender.h"

xDebugRender::xDebugRender()
	: OnInit(&xEvent::OnInit, this, &xDebugRender::_init)
	, OnShutdown(&xEvent::OnShutdown, this, &xDebugRender::_shutdown)
	, OnRender(&RenderEvent::OnDebugRender, this, &xDebugRender::_render)
{
	mMode = K_Default;
}

xDebugRender::~xDebugRender()
{
}

void xDebugRender::_init(void *, void *)
{
	mTech_Normal = xApp::Instance()->GetHelperShaderLib()->GetTechnique("DebugNormal");

	d_assert (mTech_Normal);
}

void xDebugRender::_shutdown(void *, void *)
{
}

void xDebugRender::_render(void *, void *)
{
	if (mMode == K_DisplayNormal)
	{
		Texture * tex = RenderScheme::Instance()->GetNormalTexture();

		SamplerState state;
		state.Address = TEXA_CLAMP;

		Camera * cam = World::Instance()->MainCamera();
		Vec3 v(0, 1, 0);

		v = v.TransformN(cam->GetViewMatrix());

		RenderSystem::Instance()->SetTexture(0, state, tex);

		RenderHelper::Instance()->DrawScreenQuad(BM_OPATICY, mTech_Normal);
	}
}