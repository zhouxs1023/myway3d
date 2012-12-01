#include "stdafx.h"
#include "xToolBar.h"
#include "xApp.h"
#include "xBaseOperator.h"

IMP_SLN(xOpToolBar);

xOpToolBar::xOpToolBar()
	: mOnInitUI(&xEvent::OnInitUI, this, &xOpToolBar::Init)
	, mOnRender(&RenderEvent::OnAfterRender, this, &xOpToolBar::Render)
	, mOnShutdown(&xEvent::OnShutdown, this, &xOpToolBar::Shutdown)
	, mOnUpdate(&xEvent::OnUpdate, this, &xOpToolBar::Update)
{
	INIT_SLN;
	mNumButtons = 0;
	mCheckedButton = -1;
}

xOpToolBar::~xOpToolBar()
{
	SHUT_SLN;
}

void xOpToolBar::Add(const char * image, int id)
{
	d_assert (mNumButtons < MAX_BUTTONS);

	mButtons[mNumButtons].image = image;
	mButtons[mNumButtons].id = id;
	++mNumButtons;
}

void xOpToolBar::Init(void * param0, void * param1)
{
	for (int i = 0; i < mNumButtons; ++i)
	{
		mTexture[i] = VideoBufferManager::Instance()->Load2DTexture(mButtons[i].image, mButtons[i].image);
	}

	mTech_Back = xApp::Instance()->GetHelperShaderLib()->GetTechnique("OpToolBarBack");
	mTech_Button = xApp::Instance()->GetHelperShaderLib()->GetTechnique("OpToolBarButton");
	mTech_Checked = xApp::Instance()->GetHelperShaderLib()->GetTechnique("OpToolBarChecked");
}

void xOpToolBar::Shutdown(void * param0, void * param1)
{
	for (int i = 0; i < mNumButtons; ++i)
	{
		mTexture[i] = NULL;
	}
}

void xOpToolBar::Update(void * param0, void * param1)
{
	if (!IMouse::Instance()->KeyUp(MKC_BUTTON0))
		return ;

	Point2f pt = IMouse::Instance()->GetPositionUnit();
	int wndWidth = Engine::Instance()->GetDeviceProperty()->Width;
	int wndHeight = Engine::Instance()->GetDeviceProperty()->Height;

	int offset = 0;
	for (int i = 0; i < mNumButtons; ++i)
	{
		float x0 = offset / (float)wndWidth;
		float y0 = 0;
		float x1 = x0 + ICON_SIZE / (float)wndWidth;
		float y1 = y0 + ICON_SIZE / (float)wndHeight;

		if (pt.x > x0 && pt.x < x1 &&
			pt.x > y0 && pt.y < y1)
		{
			mCheckedButton = mButtons[i].id;

			xEvent::OnOperatorBarChecked(&mCheckedButton, NULL);

			return ;
		}

		offset += ICON_SIZE;
	}
}

void xOpToolBar::Render(void * param0, void * param1)
{
	int wndWidth = Engine::Instance()->GetDeviceProperty()->Width;
	int wndHeight = Engine::Instance()->GetDeviceProperty()->Height;

	// draw back ground.
	{
		ShaderParam * uTransform = mTech_Back->GetVertexShaderParamTable()->GetParam("gTransform");
		ShaderParam * uColor = mTech_Back->GetPixelShaderParamTable()->GetParam("gColor");

		float scale = ICON_SIZE / (float)wndHeight;

		uTransform->SetUnifom(1, scale, 0, 0);
		uColor->SetUnifom(0, 0, 0, 0.5f);

		RenderHelper::Instance()->DrawScreenQuad(BM_ALPHA_BLEND, mTech_Back);
	}

	// draw buttons.
	{
		int offset = 0;
		float scale = ICON_SIZE / (float)wndHeight;

		ShaderParam * uTransform = mTech_Button->GetVertexShaderParamTable()->GetParam("gTransform");

		for (int i = 0; i < mNumButtons; ++i)
		{
			float x = offset / (float)wndWidth;

			uTransform->SetUnifom(scale, scale, x, 0);
			
			SamplerState state;
			RenderSystem::Instance()->SetTexture(0, state, mTexture[i].c_ptr());
			RenderHelper::Instance()->DrawScreenQuad(BM_ALPHA_BLEND, mTech_Button);

			offset += ICON_SIZE;
		}
	}

	// draw checked.
	{
		int offset = 0;
		float scale = ICON_SIZE / (float)wndHeight;

		ShaderParam * uTransform = mTech_Checked->GetVertexShaderParamTable()->GetParam("gTransform");

		for (int i = 0; i < mNumButtons; ++i)
		{
			float x = offset / (float)wndWidth;

			if (mCheckedButton == mButtons[i].id)
			{
				uTransform->SetUnifom(scale, scale, x, 0);

				SamplerState state;
				RenderSystem::Instance()->SetTexture(0, state, mTexture[i].c_ptr());
				RenderHelper::Instance()->DrawScreenQuad(BM_MULTIPLY, mTech_Checked);

				break;
			}

			offset += ICON_SIZE;
		}
	}
}
