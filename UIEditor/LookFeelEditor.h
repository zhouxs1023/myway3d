#pragma once

#include "LookFeelPanel.h"
#include "LookFeelProperty.h"
#include "LookFeelSkinEd.h"

namespace UIEditor {

	class LookFeelEditor
	{
		DECLARE_SINGLETON(LookFeelEditor);

	public:
		tEvent1<MGUI_LookFeel *> eventEditLookFeelChanged;
		tEvent1<Texture *> eventEditSkinChanged;
		tEvent1<MGUI_Rect> eventEditRectChanged;

	public:
		LookFeelEditor();
		~LookFeelEditor();

		void Layout();

		void SetEditLookFeel(MGUI_LookFeel * _lookfeel);
		MGUI_LookFeel * GetEditLookFeel();

		void SetEditSkin(Texture * _skin);
		Texture * GetEditSkin();

		void SetEditRect(const MGUI_Rect & _rect);
		const MGUI_Rect & GetEditRect();

	protected:
		LookFeelPanel * mLookFeelPanel;
		LookFeelProperty * mLookFeelProperty;
		LookFeelSkinEd * mLookFeelSkinEd;

		MGUI_LookFeel * mEditLookFeel;
		Texture * mEditSkin;
		MGUI_Rect mEditRect;
	};

}