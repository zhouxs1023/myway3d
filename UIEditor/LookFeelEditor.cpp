#include "stdafx.h"
#include "LookFeelEditor.h"

namespace UIEditor {

	LookFeelEditor * LookFeelEditor::msInstance = NULL;

	LookFeelEditor::LookFeelEditor()
		: mEditLookFeel(NULL)
		, mEditSkin(NULL)
	{
		msInstance = this;

		MGUI_SkinManager::Instance()->Load("UISkin.xml");

		mLookFeelPanel = new LookFeelPanel;
		mLookFeelProperty = new LookFeelProperty;
		mLookFeelSkinEd = new LookFeelSkinEd;
	}

	LookFeelEditor::~LookFeelEditor()
	{
		mEditLookFeel = NULL;
		mEditSkin = NULL;

		delete mLookFeelSkinEd;
		delete mLookFeelProperty;
		delete mLookFeelPanel;

		msInstance = NULL;
	}

	void LookFeelEditor::Layout()
	{
		mLookFeelPanel->Layout();
		mLookFeelProperty->Layout();
		mLookFeelSkinEd->Layout();
	}

	void LookFeelEditor::SetEditLookFeel(MGUI_LookFeel * _lookfeel)
	{
		if (mEditLookFeel != _lookfeel)
		{
			mEditLookFeel = _lookfeel;
			eventEditLookFeelChanged(_lookfeel);
		}
	}

	MGUI_LookFeel * LookFeelEditor::GetEditLookFeel()
	{
		return mEditLookFeel;
	}

	void LookFeelEditor::SetEditSkin(Texture * _skin)
	{
		if (mEditSkin != _skin)
		{
			mEditSkin = _skin;
			eventEditSkinChanged(_skin);
		}
	}

	Texture * LookFeelEditor::GetEditSkin()
	{
		return mEditSkin;
	}

	void LookFeelEditor::SetEditRect(const MGUI_Rect & _rect)
	{
		mEditRect = _rect;

		eventEditRectChanged(_rect);
	}

	const MGUI_Rect & LookFeelEditor::GetEditRect()
	{
		return mEditRect;
	}

}