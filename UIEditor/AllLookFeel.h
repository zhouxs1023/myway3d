#pragma once

namespace UIEditor {

	class AllLookFeel
	{
		DECLARE_SINGLETON (AllLookFeel);

	public:
		AllLookFeel();
		~AllLookFeel();

		MGUI_LookFeel * GetMenu() { return mMenu; }
		MGUI_LookFeel * GetLookFeelFile() { return mLookFeelFile; }

		MGUI_LookFeel * GetPanel() { return mPanel; }

		MGUI_LookFeel * GetVScrollBar() { return mVScrollBar; }
		MGUI_LookFeel * GetHScrollBar() { return mHScrollBar; }

		MGUI_LookFeel * GetListBox() { return mListBox; }

		MGUI_LookFeel * GetVSlider() { return mVSlider; }
		MGUI_LookFeel * GetHSlider() { return mHSlider; }

		MGUI_LookFeel * GetProgressBar() { return mProgressBar; }

		MGUI_LookFeel * GetEditBox() { return mEditBox; }

		MGUI_LookFeel * GetComboBox() { return mComboBox; }

	protected:
		MGUI_LookFeel * mMenu;
		MGUI_LookFeel * mLookFeelFile;
		
		// panel
		MGUI_LookFeel * mPanel;

		// Scroll
		MGUI_LookFeel * mVScrollBar;
		MGUI_LookFeel * mHScrollBar;

		// ListBox
		MGUI_LookFeel * mListBox;

		// Slider
		MGUI_LookFeel * mVSlider;
		MGUI_LookFeel * mHSlider;

		// ProgressBar
		MGUI_LookFeel * mProgressBar;

		// EditBox
		MGUI_LookFeel * mEditBox;

		// ComboBox
		MGUI_LookFeel * mComboBox;
	};

}