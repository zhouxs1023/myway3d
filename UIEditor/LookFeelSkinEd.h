#pragma once

#include "RectEd.h"

namespace UIEditor {

	class LookFeelSkinEd
	{
	public:
		LookFeelSkinEd();
		~LookFeelSkinEd();

		void Layout();

	protected:
		void OnEditSkinChanged_(Texture * _skin);
		void OnEditRectChanged_(MGUI_Rect _rect);


	protected:
		MGUI_Layout * mLayout;
		MGUI_ImageBox * mImageBox_Back;
		MGUI_ImageBox * mImageBox_Skin;

		RectEd * mRectEd;

		tListener1<LookFeelSkinEd, Texture *> OnEditSkinChanged;
		tListener1<LookFeelSkinEd, MGUI_Rect> OnEditRectChanged;
	};

}