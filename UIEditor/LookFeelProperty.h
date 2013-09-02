#pragma once

namespace UIEditor {

	class LookFeelProperty
	{
	public:
		LookFeelProperty();
		~LookFeelProperty();

		void Layout();

	protected:
		void OnEditLookFeelChanged_(MGUI_LookFeel * _lookfeel);
		void OnEditRectChanged_(MGUI_Rect _rect);

		void OnSkinChanged_(int _index);
		void OnStateChanged_(int _index);

		void OnNameReturn_();
		void OnClientReturn_();
		void OnUVRectReturn_();

	protected:
		MGUI_Layout * mLayout;

		MGUI_EditBox * mEditBox_Name;
		MGUI_ComboBox * mComboBox_Skin;
		MGUI_EditBox * mEditBox_Client;
		MGUI_ComboBox * mComboBox_State;
		MGUI_EditBox * mEditBox_UVRect;
		MGUI_Widget * mWidget_TextColor;

		tListener1<LookFeelProperty, MGUI_LookFeel *> OnEditLookFeelChanged;
		tListener1<LookFeelProperty, MGUI_Rect> OnEditRectChanged;

		tListener1<LookFeelProperty, int> OnSkinChanged;
		tListener1<LookFeelProperty, int> OnStateChanged;

		tListener0<LookFeelProperty> OnNameReturn;
		tListener0<LookFeelProperty> OnClientReturn;
		tListener0<LookFeelProperty> OnUVRectReturn;
	};

}
