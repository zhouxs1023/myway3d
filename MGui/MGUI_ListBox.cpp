#include "MGUI_ListBox.h"
#include "MGUI_Helper.h"
#include "MGUI_Layout.h"
#include "MGUI_Engine.h"

namespace Myway {

	


	ImplementRTTI(MGUI_ListBox, MGUI_Widget);

	MGUI_ListBox::MGUI_ListBox(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent)
		: MGUI_Widget(_lookfeel, _parent)
		, mSelectIndex(-1)
	{
		mItemHeight = 24;
		mItemLookFeel = _lookfeel->GetChild("ListBoxItem");

		mVScrollBar = new MGUI_VScrollBar(_lookfeel->GetChild("VScrollBar"), this);
		mVScrollBar->eventPositionChanged += OnVScroll(this, &MGUI_ListBox::OnVScroll_);

		mTopIndex = 0;
	}

	MGUI_ListBox::~MGUI_ListBox()
	{
		Clear();
	}

	void MGUI_ListBox::Append(const MGUI_String & _text, void * _userData)
	{
		mItems.PushBack(new MGUI_ListBoxItem(this, mItemLookFeel, _text, _userData));
	}

	void MGUI_ListBox::Insert(int _index, const MGUI_String & _text, void * _userData)
	{
		mItems.Insert(_index, new MGUI_ListBoxItem(this, mItemLookFeel, _text, _userData));
	}

	void MGUI_ListBox::Remove(int _index)
	{
		delete mItems[_index];
		mItems.Erase(_index);
	}

	void MGUI_ListBox::Clear()
	{
		for (int i = 0; i < mItems.Size(); ++i)
			delete mItems[i];
		
		mItems.Clear();
	}

	int MGUI_ListBox::GetCount()
	{
		return mItems.Size();
	}

	const MGUI_String & MGUI_ListBox::GetText(int _index)
	{
		return mItems[_index]->GetText();
	}

	void * MGUI_ListBox::GetUserData(int _index)
	{
		return mItems[_index]->GetUserData();
	}

	MGUI_ListBoxItem * MGUI_ListBox::GetItem(int _index)
	{
		return mItems[_index];
	}

	void MGUI_ListBox::SetSelectIndex(int _index)
	{
		d_assert (_index < GetCount());

		if (_index != mSelectIndex)
		{
			int tIndex = mSelectIndex;
			mSelectIndex = _index;

			eventSelectChanged(tIndex, _index);
		}
	}

	int MGUI_ListBox::GetSelectIndex() const
	{
		return mSelectIndex;
	}

	MGUI_ListBoxItem * MGUI_ListBox::GetSelectItem()
	{
		if (mSelectIndex != -1)
			return mItems[mSelectIndex];

		return NULL;
	}

	void MGUI_ListBox::_AddRenderItem(MGUI_Layout * _layout)
	{
		if (!mVisible)
			return ;

		const MGUI_LookFeel * _lookfeel = mLookFeel;
		const MGUI_Rect & clipRect = MGUI_Helper::Instance()->GetClipRect(mParent);

		if (_lookfeel)
		{
			MGUI_RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), _lookfeel->GetShader(), _lookfeel->GetSkin());

			const MGUI_Rect & myRect = this->GetAbsRect();
			const MGUI_Rect & clRect = this->GetClientRect();
			const MGUI_RectF & uvRect = MGUI_Helper::Instance()->MapUVRect(_lookfeel->GetUVRect(mState), _lookfeel->GetSkin());
			const MGUI_RectF & uvClientRect = MGUI_Helper::Instance()->MapUVRect(_lookfeel->GetUVClientRect(mState), _lookfeel->GetSkin());
			Color4 color = mColor * _lookfeel->GetColor(mState);

			MGUI_Helper::Instance()->AddRenderItem(ri, myRect, clRect, uvRect, uvClientRect, color, clipRect);
		}

		Color4 color = mColor * _lookfeel->GetTextColor(mState);

		for (int i = 0; i < mItems.Size(); ++i)
		{
			mItems[i]->_AddRenderItem(_layout);
		}

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			mChildren[i]->_AddRenderItem(_layout);
		}
	}

	void MGUI_ListBox::OnUpdate()
	{
		if (!mVisible)
			return ;

		const MGUI_Rect myRect = mVScrollBar->GetRect();

		MGUI_Rect rect;
		rect.x0 = mClientRect.Width() - mVScrollBar->GetRect().Width();
		rect.x1 = mClientRect.Width();
		rect.y0 = 0;
		rect.y1 = mClientRect.Height();

		mVScrollBar->SetRect(rect);

		_updateItems();
		_updateScroll();
	}

	void MGUI_ListBox::_updateScroll()
	{
		if (GetCount() == 0)
		{
			mVScrollBar->SetVisible(false);
			return ;
		}

		int clHeight = GetClientRect().Height();
		int maxHeight = GetCount() * mItemHeight;

		if (maxHeight > clHeight)
			mVScrollBar->SetVisible(true);
		else
			mVScrollBar->SetVisible(false);

		mVScrollBar->SetRange(0, GetCount() - 1);
		mVScrollBar->SetPosition(mTopIndex);
	}

	void MGUI_ListBox::_updateItems()
	{
		MGUI_Rect myRect = GetAbsClientRect();

		for (int i = 0; i < GetCount(); ++i)
		{
			MGUI_ListBoxItem * item = mItems[i];

			MGUI_Rect _rect = myRect;

			_rect.y0 += (i - mTopIndex) * mItemHeight;
			_rect.y1 = _rect.y0 + mItemHeight - 1;

			item->SetRect(_rect);
			item->Update();
		}
	}

	void MGUI_ListBox::OnMouseLostFocus(MGUI_Widget* _new)
	{
		for (int i = 0; i < GetCount(); ++i)
		{
			mItems[i]->_notifyMouseLostFocus(NULL);
		}

		mVScrollBar->_notifyMouseLostFocus(NULL);
	}

	void MGUI_ListBox::OnMouseMove(int _x, int _y)
	{
		if (mVScrollBar->GetVisible() && mVScrollBar->Pick(_x, _y) == mVScrollBar)
		{
			mVScrollBar->_notifyMouseMove(_x, _y);
			return ;
		}

		const MGUI_Rect myRect = GetAbsClientRect();

		if (myRect.x0 < _x && _x < myRect.x1 &&
			myRect.y0 < _y && _y < myRect.y1)
		{
			for (int i = 0; i < GetCount(); ++i)
			{
				MGUI_ListBoxItem * item = mItems[i];

				if (item->Pick(_x, _y) == item)
					item->_notifyMouseSetFocus(NULL);
				else
					item->_notifyMouseLostFocus(NULL);
			}
		}
	}

	void MGUI_ListBox::OnMousePressed(int _x, int _y, MGUI_MouseButton _id)
	{
		if (mVScrollBar->GetVisible() && mVScrollBar->Pick(_x, _y) == mVScrollBar)
		{
			mVScrollBar->_notifyMousePressed(_x, _y, _id);
			return ;
		}

		if (_id != MGUI_MouseButton::Left)
			return ;

		const MGUI_Rect myRect = GetAbsClientRect();

		if (myRect.x0 < _x && _x < myRect.x1 &&
			myRect.y0 < _y && _y < myRect.y1)
		{
			for (int i = 0; i < GetCount(); ++i)
			{
				MGUI_ListBoxItem * item = mItems[i];

				if (item->Pick(_x, _y) == item)
				{
					SetSelectIndex(i);
					break;
				}
			}
		}
	}

	void MGUI_ListBox::OnMouseReleased(int _x, int _y, MGUI_MouseButton _id)
	{
		if (mVScrollBar->GetVisible() && mVScrollBar->Pick(_x, _y) == mVScrollBar)
		{
			mVScrollBar->_notifyMouseReleased(_x, _y, _id);
			return ;
		}
	}

	void MGUI_ListBox::OnVScroll_(int _pos)
	{
		mTopIndex = _pos;
	}
}