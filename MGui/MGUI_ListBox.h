#pragma once

#include "MGUI_Widget.h"
#include "MGUI_HScrollBar.h"
#include "MGUI_VScrollBar.h"
#include "MGUI_ListBoxItem.h"

namespace Myway {

	class MGUI_ENTRY MGUI_ListBox : public MGUI_Widget
	{
		DeclareRTTI();

	public:
		tEvent2<int, int> eventSelectChanged;

	public:
		MGUI_ListBox(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent);
		virtual ~MGUI_ListBox();

		void Append(const MGUI_String & _text, void * _userData = NULL);
		void Insert(int _index, const MGUI_String & _text, void * _userData = NULL);
		void Remove(int _index);
		void Clear();

		int GetCount();
		const MGUI_String & GetText(int _index);
		void * GetUserData(int _index);
		MGUI_ListBoxItem * GetItem(int _index);

		void SetItemHeight(int _height) { mItemHeight = _height; }
		int GetItemHeight() const { return mItemHeight; }

		void SetSelectIndex(int _index);
		int GetSelectIndex() const;
		MGUI_ListBoxItem * GetSelectItem();

		virtual void _AddRenderItem(MGUI_Layout * _layout);

		virtual void OnUpdate();

	protected:
		void _updateScroll();
		void _updateItems();

	protected:
		virtual void OnMouseLostFocus(MGUI_Widget* _new);
		virtual void OnMouseMove(int _x, int _y);
		virtual void OnMousePressed(int _x, int _y, MGUI_MouseButton _id);
		virtual void OnMouseReleased(int _x, int _y, MGUI_MouseButton _id);

		void OnVScroll_(int _pos);

	protected:
		int mItemHeight;

		const MGUI_LookFeel * mItemLookFeel;
		Array<MGUI_ListBoxItem *> mItems;
		int mSelectIndex;
		
		int mTopIndex;
		MGUI_VScrollBar * mVScrollBar;

		tListener1<MGUI_ListBox, int> OnVScroll;
	};


	
}