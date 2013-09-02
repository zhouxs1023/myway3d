#include "stdafx.h"

#include "AllLookFeel.h"

namespace Myway {

	AllLookFeel * AllLookFeel::msInstance = NULL;

	AllLookFeel::AllLookFeel()
	{
		msInstance = this;

		// Menu
		mMenu = new MGUI_LookFeel("MainMenu");
		mMenu->SetSkin("EditorSkin.png");
		mMenu->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(202, 2, 202 + 27, 2 + 26));
		mMenu->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(202, 2, 202 + 27, 2 + 26));
		mMenu->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(202, 2, 202 + 27, 2 + 26));
		mMenu->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(202, 2, 202 + 27, 2 + 26));
		mMenu->SetClientRect(MGUI_Rect(6, 0, 21, 26));

		// LookFeelFile
		mLookFeelFile = new MGUI_LookFeel("ButtonLookFeelFile");
		mLookFeelFile->SetSkin("LookFeel.png");
		mLookFeelFile->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(0, 0, 127, 127));
		mLookFeelFile->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(0, 0, 127, 127));
		mLookFeelFile->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(0, 0, 127, 127));
		mLookFeelFile->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(0, 0, 127, 127));
		mLookFeelFile->SetClientRect(MGUI_Rect(0, 0, 127, 127));

		// Button
		mButton = new MGUI_LookFeel("Button");
		mButton->SetSkin("EditorSkin.png");
		mButton->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(113, 198, 113 + 18, 198 + 13));
		mButton->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(113, 183, 113 + 18, 183 + 13));
		mButton->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(113, 213, 113 + 18, 213 + 13));
		mButton->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(113, 228, 113 + 18, 228 + 13));
		mButton->SetTextColor(MGUI_WidgetState::Disabled, Color4(0.5f, 0.5f, 0.5f));
		mButton->SetTextColor(MGUI_WidgetState::Normal, Color4(1.0f, 1.0f, 1.0f));
		mButton->SetTextColor(MGUI_WidgetState::Focused, Color4(0.15f, 0.66f, 1));
		mButton->SetTextColor(MGUI_WidgetState::Pressed, Color4(0, 0, 0));
		mButton->SetClientRect(MGUI_Rect(4, 4, 8, 10));

		// Panel
		mPanel =  new MGUI_LookFeel("Panel");
		mPanel->SetSkin("EditorSkin.png");
		mPanel->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(341, 2, 341 + 23, 2 + 22));
		mPanel->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(341, 2, 341 + 23, 2 + 22));
		mPanel->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(341, 2, 341 + 23, 2 + 22));
		mPanel->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(341, 2, 341 + 23, 2 + 22));
		mPanel->SetClientRect(MGUI_Rect(4, 4, 4 + 13, 4 + 12));

		mWhite =  new MGUI_LookFeel("White");
		mWhite->SetSkin("EditorSkin.png");
		mWhite->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(true, 454, 6, 2, 2));
		mWhite->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(true, 454, 6, 2, 2));
		mWhite->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(true, 454, 6, 2, 2));
		mWhite->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(true, 454, 6, 2, 2));
		mWhite->SetClientRect(MGUI_Rect(true, 0, 0, 2, 2));

		// scroll v
		MGUI_LookFeel * mBnVScrollUp = new MGUI_LookFeel("ScrollUp");
		mBnVScrollUp->SetSkin("EditorSkin.png");
		mBnVScrollUp->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(95, 68, 95 + 14, 68 + 11));
		mBnVScrollUp->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(95, 54, 95 + 14, 54 + 11));
		mBnVScrollUp->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(95, 82, 95 + 14, 82 + 11));
		mBnVScrollUp->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(95, 96, 95 + 14, 96 + 11));
		mBnVScrollUp->SetClientRect(MGUI_Rect(0, 0, 13, 10));

		MGUI_LookFeel * mBnVScrollDown = new MGUI_LookFeel("ScrollDown");
		mBnVScrollDown->SetSkin("EditorSkin.png");
		mBnVScrollDown->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(95, 15, 95 + 14, 15 + 11));
		mBnVScrollDown->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(95, 2, 95 + 14, 2 + 11));
		mBnVScrollDown->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(95, 28, 95 + 14, 28 + 11));
		mBnVScrollDown->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(95, 41, 95 + 14, 41 + 11));
		mBnVScrollDown->SetClientRect(MGUI_Rect(0, 0, 13, 10));

		MGUI_LookFeel * mBnVScroll = new MGUI_LookFeel("ScrollTrack");
		mBnVScroll->SetSkin("EditorSkin.png");
		mBnVScroll->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(113, 57, 113 + 13, 57 + 18));
		mBnVScroll->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(113, 37, 113 + 13, 37 + 18));
		mBnVScroll->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(113, 77, 113 + 13, 77 + 18));
		mBnVScroll->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(113, 97, 113 + 13, 97 + 18));
		mBnVScroll->SetClientRect(MGUI_Rect(4, 4, 8, 13));

		mVScrollBar = new MGUI_LookFeel("VScrollBar");
		mVScrollBar->SetSkin("EditorSkin.png");
		mVScrollBar->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(341, 73, 341 + 15, 73 + 19));
		mVScrollBar->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(341, 73, 341 + 15, 73 + 19));
		mVScrollBar->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(341, 73, 341 + 15, 73 + 19));
		mVScrollBar->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(341, 73, 341 + 15, 73 + 19));
		mVScrollBar->SetClientRect(MGUI_Rect(2, 2, 13, 17));
		mVScrollBar->AddChild(mBnVScroll);
		mVScrollBar->AddChild(mBnVScrollUp);
		mVScrollBar->AddChild(mBnVScrollDown);

		// scroll h
		MGUI_LookFeel * mBnHScrollLeft = new MGUI_LookFeel("ScrollLeft");
		mBnHScrollLeft->SetSkin("EditorSkin.png");
		mBnHScrollLeft->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(95, 130, 95 + 11, 130 + 13));
		mBnHScrollLeft->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(95, 114, 95 + 11, 114 + 13));
		mBnHScrollLeft->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(95, 146, 95 + 11, 146 + 13));
		mBnHScrollLeft->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(95, 162, 95 + 11, 162 + 13));
		mBnHScrollLeft->SetClientRect(MGUI_Rect(0, 0, 11, 13));

		MGUI_LookFeel * mBnHScrollRight = new MGUI_LookFeel("ScrollRight");
		mBnHScrollRight->SetSkin("EditorSkin.png");
		mBnHScrollRight->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(95, 196, 95 + 11, 196 + 13));
		mBnHScrollRight->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(95, 180, 95 + 11, 180 + 13));
		mBnHScrollRight->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(95, 212, 95 + 11, 212 + 13));
		mBnHScrollRight->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(95, 228, 95 + 11, 228 + 13));
		mBnHScrollRight->SetClientRect(MGUI_Rect(0, 0, 11, 13));

		MGUI_LookFeel * mBnHScroll = new MGUI_LookFeel("ScrollTrack");
		mBnHScroll->SetSkin("EditorSkin.png");
		mBnHScroll->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(113, 198, 113 + 18, 198 + 13));
		mBnHScroll->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(113, 183, 113 + 18, 183 + 13));
		mBnHScroll->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(113, 213, 113 + 18, 213 + 13));
		mBnHScroll->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(113, 228, 113 + 18, 228 + 13));
		mBnHScroll->SetClientRect(MGUI_Rect(4, 4, 8, 13));

		mHScrollBar = new MGUI_LookFeel("HScrollBar");
		mHScrollBar->SetSkin("EditorSkin.png");
		mHScrollBar->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(341, 54, 341 + 18, 54 + 14));
		mHScrollBar->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(341, 54, 341 + 18, 54 + 14));
		mHScrollBar->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(341, 54, 341 + 18, 54 + 14));
		mHScrollBar->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(341, 54, 341 + 18, 54 + 14));
		mHScrollBar->SetClientRect(MGUI_Rect(2, 2, 16, 12));
		mVScrollBar->AddChild(mBnHScroll);
		mVScrollBar->AddChild(mBnHScrollLeft);
		mVScrollBar->AddChild(mBnHScrollRight);

		// ListBox
		MGUI_LookFeel * mListBoxItem = new MGUI_LookFeel("ListBoxItem");
		mListBoxItem->SetSkin("EditorSkin.png");
		mListBoxItem->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(204, 160, 204 + 42, 160 + 9));
		mListBoxItem->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(204, 160, 204 + 42, 160 + 9));
		mListBoxItem->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(204, 174, 204 + 42, 174 + 9));
		mListBoxItem->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(204, 188, 204 + 42, 188 + 9));
		mListBoxItem->SetUVRect(MGUI_WidgetState::Selected, MGUI_Rect(204, 188, 204 + 42, 188 + 9));
		mListBoxItem->SetTextColor(MGUI_WidgetState::Disabled, Color4(0.5f, 0.5f, 0.5f));
		mListBoxItem->SetTextColor(MGUI_WidgetState::Normal, Color4(1.0f, 1.0f, 1.0f));
		mListBoxItem->SetTextColor(MGUI_WidgetState::Focused, Color4(0.15f, 0.66f, 1));
		mListBoxItem->SetTextColor(MGUI_WidgetState::Pressed, Color4(0, 0, 0));
		mListBoxItem->SetTextColor(MGUI_WidgetState::Selected, Color4(0, 0, 0));
		mListBoxItem->SetClientRect(MGUI_Rect(2, 2, 41, 8));

		mListBox = new MGUI_LookFeel("ListBox");
		mListBox->SetSkin("EditorSkin.png");
		mListBox->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(341, 2, 341 + 22, 2 + 21));
		mListBox->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(341, 2, 341 + 22, 2 + 21));
		mListBox->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(341, 2, 341 + 22, 2 + 21));
		mListBox->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(341, 2, 341 + 22, 2 + 21));
		mListBox->SetUVRect(MGUI_WidgetState::Selected, MGUI_Rect(341, 2, 341 + 22, 2 + 21));
		mListBox->SetClientRect(MGUI_Rect(4, 4, 4 + 13, 4 + 12));
		mListBox->AddChild(mListBoxItem);
		mListBox->AddChild(mVScrollBar->Clone("VScrollBar"));

		// slider
		MGUI_LookFeel * mBnVSliderTrack = new MGUI_LookFeel("SliderTrack");
		mBnVSliderTrack->SetSkin("EditorSkin.png");
		mBnVSliderTrack->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(113, 10, 113 + 13, 10+ 6));
		mBnVSliderTrack->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(113, 2, 113 + 13, 2 + 6));
		mBnVSliderTrack->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(113, 18, 113 + 13, 18 + 6));
		mBnVSliderTrack->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(113, 26, 113 + 13, 26 + 6));
		mBnVSliderTrack->SetClientRect(MGUI_Rect(4, 0, 8, 6));

		mVSlider = new MGUI_LookFeel("VSlider");
		mVSlider->SetSkin("EditorSkin.png");
		mVSlider->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(341, 73, 341 + 15, 73 + 19));
		mVSlider->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(341, 73, 341 + 15, 73 + 19));
		mVSlider->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(341, 73, 341 + 15, 73 + 19));
		mVSlider->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(341, 73, 341 + 15, 73 + 19));
		mVSlider->SetClientRect(MGUI_Rect(2, 2, 13, 17));
		mVSlider->AddChild(mBnVSliderTrack);

		MGUI_LookFeel * mBnHSliderTrack = new MGUI_LookFeel("SliderTrack");
		mBnHSliderTrack->SetSkin("EditorSkin.png");
		mBnHSliderTrack->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(113, 135, 113 + 6, 135 + 13));
		mBnHSliderTrack->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(113, 120, 113 + 6, 120 + 13));
		mBnHSliderTrack->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(113, 150, 113 + 6, 150 + 13));
		mBnHSliderTrack->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(113, 165, 113 + 6, 160 + 13));
		mBnHSliderTrack->SetClientRect(MGUI_Rect(0, 4, 6, 8));

		mHSlider = new MGUI_LookFeel("VSlider");
		mHSlider->SetSkin("EditorSkin.png");
		mHSlider->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(341, 54, 341 + 18, 54 + 14));
		mHSlider->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(341, 54, 341 + 18, 54 + 14));
		mHSlider->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(341, 54, 341 + 18, 54 + 14));
		mHSlider->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(341, 54, 341 + 18, 54 + 14));
		mHSlider->SetClientRect(MGUI_Rect(2, 2, 16, 12));
		mHSlider->AddChild(mBnHSliderTrack);

		
		//ProgressBar
		MGUI_LookFeel * mProgressBarTrack = new MGUI_LookFeel("Track");
		mProgressBarTrack->SetSkin("EditorSkin.png");
		mProgressBarTrack->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(113, 135, 113 + 6, 135 + 13));
		mProgressBarTrack->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(113, 120, 113 + 6, 120 + 13));
		mProgressBarTrack->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(113, 135, 113 + 6, 135 + 13));
		mProgressBarTrack->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(113, 135, 113 + 6, 135 + 13));
		mProgressBarTrack->SetClientRect(MGUI_Rect(2, 2, 3, 10));

		mProgressBar = new MGUI_LookFeel("ProgressBar");
		mProgressBar->SetSkin("EditorSkin.png");
		mProgressBar->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(341, 54, 341 + 18, 54 + 14));
		mProgressBar->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(341, 54, 341 + 18, 54 + 14));
		mProgressBar->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(341, 54, 341 + 18, 54 + 14));
		mProgressBar->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(341, 54, 341 + 18, 54 + 14));
		mProgressBar->SetClientRect(MGUI_Rect(2, 2, 16, 12));
		mProgressBar->AddChild(mProgressBarTrack);
		
		// EditBox
		mEditBox = new MGUI_LookFeel("EditBox");
		mEditBox->SetSkin("EditorSkin.png");
		mEditBox->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(169, 29, 169 + 28, 29 + 25));
		mEditBox->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(169, 2, 169 + 28, 2 + 25));
		mEditBox->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(169, 56, 169 + 28, 56 + 25));
		mEditBox->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(169, 164, 169 + 28, 164 + 25));
		mEditBox->SetUVRect(MGUI_WidgetState::Selected, MGUI_Rect(169, 164, 169 + 28, 164 + 25));
		mEditBox->SetClientRect(MGUI_Rect(8, 3, 8 + 11, 10 + 13));


		// ComboBox
		mComboBox = new MGUI_LookFeel("ComboxBox");
		mComboBox->AddChild(mEditBox->Clone("EditBox"));
		mComboBox->AddChild(mListBox->Clone("ListBox"));
		mComboBox->AddChild(mBnVScrollDown->Clone("DropButton"));

		
		// Dialog
		mDialog = new MGUI_LookFeel("Dialog");
		mDialog->SetSkin("EditorSkin.png");
		mDialog->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(true, 296, 197, 21, 20));
		mDialog->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(true, 296, 197, 21, 20));
		mDialog->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(true, 296, 197, 21, 20));
		mDialog->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(true, 296, 197, 21, 20));
		mDialog->SetUVRect(MGUI_WidgetState::Selected, MGUI_Rect(true, 296, 197, 21, 20));
		mDialog->SetClientRect(MGUI_Rect(5, 2, 16, 13));

		mDialogCaption = new MGUI_LookFeel("Dialog");
		mDialogCaption->SetSkin("EditorSkin.png");
		mDialogCaption->SetUVRect(MGUI_WidgetState::Normal, MGUI_Rect(true, 268, 36, 68, 28));
		mDialogCaption->SetUVRect(MGUI_WidgetState::Disabled, MGUI_Rect(true, 268, 36, 68, 28));
		mDialogCaption->SetUVRect(MGUI_WidgetState::Focused, MGUI_Rect(true, 268, 36, 68, 28));
		mDialogCaption->SetUVRect(MGUI_WidgetState::Pressed, MGUI_Rect(true, 268, 36, 68, 28));
		mDialogCaption->SetUVRect(MGUI_WidgetState::Selected, MGUI_Rect(true, 268, 36, 68, 28));
		mDialogCaption->SetClientRect(MGUI_Rect(true, 16, 0, 34, 28));
	}

	AllLookFeel::~AllLookFeel()
	{
		delete mMenu;
		delete mLookFeelFile;

		delete mButton;

		delete mPanel;
		delete mWhite;

		delete mVScrollBar;
		delete mHScrollBar;
		
		delete mListBox;

		delete mVSlider;
		delete mHSlider;

		delete mProgressBar;

		delete mEditBox;

		delete mComboBox;

		delete mDialog;
		delete mDialogCaption;

		msInstance = NULL;
	}

}