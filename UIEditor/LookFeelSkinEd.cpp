#include "stdafx.h"

#include "LookFeelSkinEd.h"
#include "LookFeelEditor.h"

namespace UIEditor {

	LookFeelSkinEd::LookFeelSkinEd()
	{
		mLayout = new MGUI_Layout(NULL);

		mImageBox_Back = new MGUI_ImageBox(mLayout);
		mImageBox_Back->SetAlign(MGUI_Align::Center);
		mImageBox_Back->SetSkin("BackgroundTile.png");

		mImageBox_Skin = new MGUI_ImageBox(mLayout);
		mImageBox_Skin->SetAlign(MGUI_Align::Center);
		mImageBox_Skin->SetOrder(1);

		mRectEd = new RectEd(mImageBox_Skin);

		LookFeelEditor::Instance()->eventEditSkinChanged += OnEditSkinChanged(this, &LookFeelSkinEd::OnEditSkinChanged_);
		LookFeelEditor::Instance()->eventEditRectChanged += OnEditRectChanged(this, &LookFeelSkinEd::OnEditRectChanged_);
	}

	LookFeelSkinEd::~LookFeelSkinEd()
	{
		delete mLayout;
	}

	void LookFeelSkinEd::Layout()
	{
		MGUI_Rect rect;

		rect.x0 = 256;
		rect.y0 = 32;
		rect.x1 = MGUI_Engine::Instance()->GetRect().x1 - 256;
		rect.y1 = MGUI_Engine::Instance()->GetRect().y1 - 32;

		mLayout->SetRect(rect);
	}

	void LookFeelSkinEd::OnEditSkinChanged_(Texture * _skin)
	{
		mImageBox_Skin->SetSkin(_skin);

		if (_skin)
		{
			int skinW = _skin->GetWidth();
			int skinH = _skin->GetHeight();
			int backW = mImageBox_Back->GetSkin()->GetWidth();
			int backH = mImageBox_Back->GetSkin()->GetHeight();

			mImageBox_Back->SetRect(0, 0, skinW - 1, skinH - 1);
			mImageBox_Skin->SetRect(0, 0, skinW - 1, skinH - 1);

			mImageBox_Back->SetUVRect(MGUI_RectF(0, 0, skinW / (float)backW, skinH / (float)backH));
		}
		else
		{
			mImageBox_Back->SetRect(0, 0, 0, 0);
			mImageBox_Skin->SetRect(0, 0, 0, 0);
		}
	}

	void LookFeelSkinEd::OnEditRectChanged_(MGUI_Rect _rect)
	{
		mRectEd->SetEditRect(_rect);
	}

}