#include "MGUI_ComboBox.h"
#include "MGUI_Engine.h"

namespace Myway {

	ImplementRTTI(MGUI_ComboBox, MGUI_Widget);

	MGUI_ComboBox::MGUI_ComboBox(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent)
		: MGUI_Widget(_lookfeel, _parent)
		, mItemHeight(24)
		, mDropHeight(128)
		, mPopuped(false)
	{
		mEditBox = new MGUI_EditBox(_lookfeel->GetChild("EditBox"), this);
		mBnDrop = new MGUI_Button(_lookfeel->GetChild("DropButton"), mEditBox);

		mEditBox->SetAlign(MGUI_Align::Stretch);
		mEditBox->SetStatic(true);
		mEditBox->SetCaption(L"");

		mBnDrop->SetAlign(MGUI_Align::RightCenter);

		mListBox = new MGUI_ListBox(_lookfeel->GetChild("ListBox"), _parent);
		mListBox->SetVisible(false);

		SetItemHeight(mItemHeight);

		mEditBox->eventMouseClick += OnEditBoxClick(this, &MGUI_ComboBox::OnDrop_);
		mBnDrop->eventMouseClick += OnDropButtonClick(this, &MGUI_ComboBox::OnDrop_);
		mListBox->eventSelectChanged += OnListBoxSelectChanged(this, &MGUI_ComboBox::OnSelectChanged_);
	}

	MGUI_ComboBox::~MGUI_ComboBox()
	{
	}

	void MGUI_ComboBox::Append(const MGUI_String & _text, void * _userData)
	{
		mListBox->Append(_text, _userData);
	}

	void MGUI_ComboBox::Insert(int _index, const MGUI_String & _text, void * _userData)
	{
		mListBox->Insert(_index, _text, _userData);
	}

	void MGUI_ComboBox::Remove(int _index)
	{
		mListBox->Remove(_index);
	}

	void MGUI_ComboBox::Clear()
	{
		mListBox->Clear();
	}

	void MGUI_ComboBox::SetSelectIndex(int _index)
	{
		mListBox->SetSelectIndex(_index);
	}

	int MGUI_ComboBox::GetSelectIndex() const
	{
		return mListBox->GetSelectIndex();
	}

	const MGUI_String & MGUI_ComboBox::GetText(int _index)
	{
		return mListBox->GetText(_index);
	}

	void * MGUI_ComboBox::GetUserData(int _index)
	{
		return mListBox->GetUserData(_index);
	}

	void MGUI_ComboBox::SetItemHeight(int _height)
	{
		mItemHeight = _height;
		mListBox->SetItemHeight(_height);
	}

	int MGUI_ComboBox::GetItemHeight() const
	{
		return mItemHeight;
	}

	void MGUI_ComboBox::OnUpdate()
	{
		if (mPopuped)
		{
			MGUI_Rect myRect;

			myRect.x0 = mRect.x0;
			myRect.x1 = mRect.x1;
			myRect.y0 = mRect.y1;
			myRect.y1 = mRect.y1 + mDropHeight;

			mListBox->SetRect(myRect);

			mListBox->SetVisible(true);
		}
		else
		{
			mListBox->SetVisible(false);
		}
	}

	void MGUI_ComboBox::_AddRenderItem(MGUI_Layout * _layout)
	{
		if (!mVisible)
			return ;

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			mChildren[i]->_AddRenderItem(_layout);
		}
	}
	
	void MGUI_ComboBox::OnDrop_()
	{
		mPopuped = !mPopuped;
	}

	void MGUI_ComboBox::OnSelectChanged_(int _old, int _new)
	{
		MGUI_String text;

		if (_new != -1)
			text = GetText(_new);

		mEditBox->SetCaption(text);

		eventSelectChanged(_old, _new);

		OnDrop_();
	}

}