#pragma once

namespace UIEditor {

	class LookFeelPanel
	{
	public:
		LookFeelPanel();
		~LookFeelPanel();

		void Layout();

	protected:
		void OnWidgetTypeChanged_(int _index);
		void OnLookFeelChanged_(int _index);

		void OnCreate_();
		void OnDelete_();

	protected:
		MGUI_Layout * mLayout;
		MGUI_ComboBox * mComboBox_WidgetType;
		MGUI_ListBox * mListBox_LookFeel;

		tListener1<LookFeelPanel, int> OnWidgetTypeChanged;
		tListener1<LookFeelPanel, int> OnLookFeelChanged;

		tListener0<LookFeelPanel> OnCreate;
		tListener0<LookFeelPanel> OnDelete;
	};

}