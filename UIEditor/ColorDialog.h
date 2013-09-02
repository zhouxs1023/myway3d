#pragma once

namespace Myway {

	class ColorDialog
	{
	public:
		ColorDialog();
		~ColorDialog();

	protected:
		MGUI_Layout * mLayout;

		MGUI_Label * mLabel_Caption;

		MGUI_ImageBox * mImageBox_Color;
		MGUI_ImageBox * mImageBox_ColorPicker;
		MGUI_ImageBox * mImageBox_ColorSlider;

		MGUI_Label * mLabel_R;
		MGUI_Label * mLabel_G;
		MGUI_Label * mLabel_B;

		MGUI_EditBox * mEditBox_R;
		MGUI_EditBox * mEditBox_G;
		MGUI_EditBox * mEditBox_B;
	};

}