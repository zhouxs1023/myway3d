#include "MGUI_VScrollBar.h"
#include "MGUI_Engine.h"

namespace Myway {

	ImplementRTTI(MGUI_VScrollBar, MGUI_Widget);

	MGUI_VScrollBar::MGUI_VScrollBar(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent)
		: MGUI_Widget(_lookfeel, _parent)
		, mMouseOldY(0)
		, mDP(-1)
		, mDY(0)
	{
		d_assert (_lookfeel != NULL);

		mBnScrollUp = new MGUI_Button(_lookfeel->GetChild("ScrollUp"), this);
		mBnScrollDown = new MGUI_Button(_lookfeel->GetChild("ScrollDown"), this);
		mBnScroll = new MGUI_Button(_lookfeel->GetChild("ScrollTrack"), this);

		mBnScrollUp->eventMouseClick += OnScrollUpClick(this, &MGUI_VScrollBar::OnScrollUpClick_);
		mBnScrollDown->eventMouseClick += OnScrollDownClick(this, &MGUI_VScrollBar::OnScrollDownClick_);
		mBnScroll->eventMouseDrag += OnScrollDrag(this, &MGUI_VScrollBar::OnScrollDrag_);
		mBnScroll->eventMousePressed += OnScrollPressed(this, &MGUI_VScrollBar::OnScrollPressed_);
		mBnScroll->eventMouseReleased += OnScrollReleased(this, &MGUI_VScrollBar::OnScrollReleased_);
	}

	MGUI_VScrollBar::~MGUI_VScrollBar()
	{
	}

	void MGUI_VScrollBar::_AddRenderItem(MGUI_Layout * _layout)
	{
		if (!mVisible)
			return ;

		const MGUI_LookFeel * _lookfeel = mLookFeel;

		MGUI_RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), _lookfeel->GetShader(), _lookfeel->GetSkin());

		const MGUI_Rect & myRect = this->GetAbsRect();
		const MGUI_Rect & clRect = this->GetClientRect();
		const MGUI_RectF & uvRect = MGUI_Helper::Instance()->MapUVRect(_lookfeel->GetUVRect(mState), _lookfeel->GetSkin());
		const MGUI_RectF & uvClientRect = MGUI_Helper::Instance()->MapUVRect(_lookfeel->GetUVClientRect(mState), _lookfeel->GetSkin());
		Color4 color = mColor * _lookfeel->GetColor(mState);
		const MGUI_Rect & clipRect = MGUI_Helper::Instance()->GetClipRect(mParent);

		MGUI_Helper::Instance()->AddRenderItem(ri, myRect, clRect, uvRect, uvClientRect, color, clipRect);

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			mChildren[i]->_AddRenderItem(_layout);
		}
	}

	bool MGUI_VScrollBar::_IsScrollHiden()
	{
		const MGUI_Rect & clRect = GetAbsClientRect();

		return clRect.Width() * 2 >= clRect.Height();
	}

	void MGUI_VScrollBar::OnUpdate()
	{
		const MGUI_Rect & clRect = GetClientRect();

		int cw = clRect.Width(), ch = clRect.Height();

		MGUI_Rect myRectUp, myRectDown, myRectScroll;

		myRectUp.x0 = 0;
		myRectUp.x1 = cw;

		myRectDown.x0 = 0;
		myRectDown.x1 = cw;

		myRectScroll.x0 = 0;
		myRectScroll.x1 = cw;

		if (_IsScrollHiden())
		{
			mBnScroll->SetVisible(false);

			myRectUp.y0 = 0;
			myRectUp.y1 = ch / 2;

			myRectDown.y0 = ch / 2;
			myRectDown.y1 = ch;

			myRectScroll.y0 = myRectScroll.y1 = ch / 2;
		}
		else
		{
			mBnScroll->SetVisible(true);

			const MGUI_LookFeel * scrollLookFeel = (const MGUI_LookFeel *)mBnScroll->GetLookFeel();

			int minHeight = scrollLookFeel->GetUVRect(MGUI_WidgetState::Normal).Height();

			myRectUp.y0 = 0;
			myRectUp.y1 = cw;

			myRectDown.y0 = ch - cw;
			myRectDown.y1 = ch;

			int length = myRectDown.y0 - myRectUp.y1;

			int count = mMaxValue - mMinValue + 1;

			int dStep = length / count;

			d_assert (count >= 1);

			int pos = (mDP == -1) ? mPosition : mDP;

			float ky0 = float(pos) / (mMaxValue - mMinValue + 1);
			float ky1 = float(pos + 1) / (mMaxValue - mMinValue + 1);

			myRectScroll.y0 = myRectUp.y1 + int(ky0 * length);
			myRectScroll.y1 = myRectUp.y1 + int(ky1 * length);

			if (mDP != -1)
			{
				myRectScroll.y0 += mDY;
				myRectScroll.y1 += mDY;

				SetPosition(mDP + mDY / dStep);
			}

			if (myRectScroll.Height() < minHeight)
				myRectScroll.y1 = myRectScroll.y0 + minHeight - 1;

			int _h = myRectScroll.Height();

			if (myRectScroll.y0 < myRectUp.y1)
			{
				myRectScroll.y0 = myRectUp.y1;
				myRectScroll.y1 = myRectUp.y1 + _h - 1;
			}
			else if (myRectScroll.y1 > myRectDown.y0)
			{
				myRectScroll.y0 = myRectDown.y0 - _h + 1;
				myRectScroll.y1 = myRectDown.y0;
			}
		}

		mBnScroll->SetRect(myRectScroll);
		mBnScrollUp->SetRect(myRectUp);
		mBnScrollDown->SetRect(myRectDown);
	}

	void MGUI_VScrollBar::OnScrollUpClick_()
	{
		SetPosition(mPosition - 1);
	}

	void MGUI_VScrollBar::OnScrollDownClick_()
	{
		SetPosition(mPosition + 1);
	}

	void MGUI_VScrollBar::OnScrollDrag_(int _x, int _y)
	{
		mDY = _y - mMouseOldY;
	}

	void MGUI_VScrollBar::OnScrollPressed_(int _x, int _y, MGUI_MouseButton _id)
	{
		if (_id == MGUI_MouseButton::Left)
		{
			mMouseOldY = _y;
			mDY = 0;
			mDP = mPosition;
		}
	}

	void MGUI_VScrollBar::OnScrollReleased_(int _x, int _y, MGUI_MouseButton _id)
	{
		if (_id == MGUI_MouseButton::Left)
		{
			mMouseOldY = _y;
			mDY = 0;
			mDP = -1;
		}
	}

}