#pragma once
 
#include "MGUI_Widget.h"
#include "MGUI_Font.h"
#include "MGUI_String.h"

namespace Myway {

	class MGUI_ENTRY MGUI_ListBoxItem : public MGUI_Widget
	{
		DeclareRTTI();

		friend class MGUI_ListBox;

	public:
		MGUI_ListBoxItem(MGUI_ListBox * _listBox, const MGUI_LookFeel * _lookfeel,
			             const MGUI_String & _text = L"", void * _userData = NULL);
		virtual ~MGUI_ListBoxItem();

		void SetText(const MGUI_String & text) { mText = text; }
		const MGUI_String & GetText() const { return mText; }

		void SetUserData(void * _userData) { mUserData = _userData; }
		void * GetUserData() const { return mUserData; }

		virtual void _AddRenderItem(MGUI_Layout * _layout);

	protected:
		virtual void OnMouseLostFocus(MGUI_Widget* _new);
		virtual void OnMouseSetFocus(MGUI_Widget* _old);

	protected:
		MGUI_ListBox * mListBox;
		MGUI_String mText;
		void * mUserData;
	};
	
}