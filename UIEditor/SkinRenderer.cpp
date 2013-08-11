#include "stdafx.h"

#include "SkinRenderer.h"
#include "App.h"

namespace UIEditor {

SkinRenderer * SkinRenderer::msInstance = NULL;

SkinRenderer::SkinRenderer()
	: OnRender(RenderEvent::OnRenderGUI2, this, &SkinRenderer::Render)
{
	msInstance = this;

	mEnable = false;

	mRenderOp = new RenderOp;

	VertexDeclarationPtr vdecl = VideoBufferManager::Instance()->CreateVertexDeclaration();
	vdecl->AddElement(0, 0, DT_FLOAT2, DU_POSITION, 0);
	vdecl->Init();

	mRenderOp->vxStream.SetDeclaration(vdecl);

	VertexBufferPtr vb = VideoBufferManager::Instance()->CreateVertexBuffer(8 * 4, 8);

	float * vert = (float *)vb->Lock(0, 0, LOCK_NORMAL);
	{
		*vert++ = 0; *vert++ = 0;
		*vert++ = 1; *vert++ = 0;
		*vert++ = 0; *vert++ = 1;
		*vert++ = 1; *vert++ = 1;
	}
	vb->Unlock();

	mRenderOp->vxStream.Bind(0, vb);
	mRenderOp->vxStream.SetCount(4);

	mRenderOp->rState.depthCheck = DCM_NONE;
	mRenderOp->rState.depthWrite = false;
	mRenderOp->ePrimType = PRIM_TRIANGLESTRIP;
	mRenderOp->iPrimCount = 2;
	
	mTech = App::Instance()->GetShaderLib()->GetTechnique("SkinRenderer");

	d_assert (mTech != NULL);

	mBackTexture = VideoBufferManager::Instance()->Load2DTexture("BackgroundTile.png", "BackgroundTile.png");
}

SkinRenderer::~SkinRenderer()
{
	delete mRenderOp;

	msInstance = NULL;
}

void SkinRenderer::SetEnable(bool _enable)
{
	mEnable = _enable;
}

void SkinRenderer::SetSkinTexture(TexturePtr _texture)
{
	mSkinTexture = _texture;
}

void SkinRenderer::Render(Event * _sender)
{
	if (!mEnable)
		return ;

	RenderSystem * render = RenderSystem::Instance();

	ShaderParam * gScale = mTech->GetVertexShaderParamTable()->GetParam("gScale");
	ShaderParam * gBackUVScale = mTech->GetVertexShaderParamTable()->GetParam("gBackUVScale");

	int viewW, viewH;
	App::Instance()->GetViewSize(viewW, viewH);

	int texW = 512, texH = 256;

	if (mSkinTexture != NULL)
	{
		texW = mSkinTexture->GetWidth();
		texH = mSkinTexture->GetHeight();
	}

	float xScale = texW / (float)viewW;
	float yScale = texH / (float)viewH;

	gScale->SetUnifom(xScale, yScale, 0, 0);

	int backW = mBackTexture->GetWidth();
	int backH = mBackTexture->GetHeight();

	xScale = backW / (float)texW;
	yScale = backH / (float)texH;

	gBackUVScale->SetUnifom(1 / xScale, 1 / yScale, 0, 0);

	SamplerState state;
	render->SetTexture(0, state, mBackTexture.c_ptr());
	render->SetTexture(1, state, mSkinTexture.c_ptr());

	render->Render(mTech, mRenderOp);
}

}