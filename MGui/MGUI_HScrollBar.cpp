#include "MGUI_HScrollBar.h"
#include "MGUI_Engine.h"

namespace Myway {


	ImplementRTTI(MGUI_HScrollBar, MGUI_Widget);

	MGUI_HScrollBar::MGUI_HScrollBar(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent)
		: MGUI_Widget(_lookfeel, _parent)
		, mMouseOldX(0)
		, mDP(-1)
		, mDX(0)
	{
		d_assert (_lookfeel != NULL);

		mBnScrollLeft = new MGUI_Button(_lookfeel->GetChild("ScrollLeft"), this);
		mBnScrollRight = new MGUI_Button(_lookfeel->GetChild("ScrollRight"), this);
		mBnScroll = new MGUI_Button(_lookfeel->GetChild("ScrollTrack"), this);

		mBnScrollLeft->eventMouseClick += OnScrollLeftClick(this, &MGUI_HScrollBar::OnScrollLeftClick_);
		mBnScrollRight->eventMouseClick += OnScrollRightClick(this, &MGUI_HScrollBar::OnScrollRightClick_);
		mBnScroll->eventMouseDrag += OnScrollDrag(this, &MGUI_HScrollBar::OnScrollDrag_);
		mBnScroll->eventMousePressed += OnScrollPressed(this, &MGUI_HScrollBar::OnScrollPressed_);
		mBnScroll->eventMouseReleased += OnScrollReleased(this, &MGUI_HScrollBar::OnScrollReleased_);
	}

	MGUI_HScrollBar::~MGUI_HScrollBar()
	{
	}

	void MGUI_HScrollBar::_AddRenderItem(MGUI_Layout * _layout)
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

	bool MGUI_HScrollBar::_IsScrollHiden()
	{
		const MGUI_Rect & clRect = GetAbsClientRect();

		return clRect.Height() * 2 >= clRect.Width();
	}

	void MGUI_HScrollBar::OnUpdate()
	{
		const MGUI_Rect & clRect = GetClientRect();

		int cw = clRect.Width(), ch = clRect.Height();

		MGUI_Rect myRectLeft, myRectRight, myRectScroll;

		myRectLeft.y0 = 0;
		myRectLeft.y1 = ch;

		myRectRight.y0 = 0;
		myRectRight.y1 = ch;

		myRectScroll.y0 = 0;
		myRectScroll.y1 = ch;

		if (_IsScrollHiden())
		{
			mBnScroll->SetVisible(false);

			myRectLeft.x0 = 0;
			myRectLeft.x1 = 0 + cw / 2;

			myRectRight.x0 = cw / 2;
			myRectRight.x1 = cw;

			myRectScroll.x0 = myRectScroll.x1 = cw / 2;
		}
		else
		{
			mBnScroll->SetVisible(true);

			const MGUI_LookFeel * scrollLookFeel = mBnScroll->GetLookFeel();

			int minWidth = scrollLookFeel->GetUVRect(MGUI_WidgetState::Normal).Width();

			myRectLeft.x0 = 0;
			myRectLeft.x1 = 0 + cw / 2;

			myRectRight.x0 = cw / 2;
			myRectRight.x1 = cw;

			int length = myRectRight.x0 - myRectLeft.x1;

			int count = mMaxValue - mMinValue + 1;

			int dStep = length / count;

			d_assert (count >= 1);

			int pos = (mDP == -1) ? mPosition : mDP;

			float ky0 = float(pos) / (mMaxValue - mMinValue + 1);
			float ky1 = float(pos + 1) / (mMaxValue - mMinValue + 1);

			myRectScroll.x0 = myRectLeft.x1 + int(ky0 * length);
			myRectScroll.x1 = myRectLeft.x1 + int(ky1 * length);

			if (mDP != -1)
			{
				myRectScroll.x0 += mDX;
				myRectScroll.x1 += mDX;

				SetPosition(mDP + mDX / dStep);
			}

			if (myRectScroll.Width() < minWidth)
				myRectScroll.x1 = myRectScroll.x0 + minWidth;

			int _w = myRectScroll.Width();

			if (myRectScroll.x0 < myRectLeft.x1)
			{
				myRectScroll.x0 = myRectLeft.x1;
				myRectScroll.x1 = myRectLeft.x1 + _w;
			}
			else if (myRectScroll.x1 > myRectRight.x0)
			{
				myRectScroll.x0 = myRectRight.x0 - _w;
				myRectScroll.x1 = myRectRight.x0;
			}
		}

		mBnScroll->SetRect(myRectScroll);
		mBnScrollLeft->SetRect(myRectLeft);
		mBnScrollRight->SetRect(myRectRight);
	}

	void MGUI_HScrollBar::OnScrollLeftClick_()
	{
		SetPosition(mPosition - 1);
	}

	void MGUI_HScrollBar::OnScrollRightClick_()
	{
		SetPosition(mPosition + 1);
	}

	void MGUI_HScrollBar::OnScrollDrag_(int _x, int _y)
	{
		mDX = _x - mMouseOldX;
	}

	void MGUI_HScrollBar::OnScrollPressed_(int _x, int _y, MGUI_MouseButton _id)
	{
		if (_id == MGUI_MouseButton::Left)
		{
			mMouseOldX = _x;
			mDX = 0;
			mDP = mPosition;
		}
	}

	void MGUI_HScrollBar::OnScrollReleased_(int _x, int _y, MGUI_MouseButton _id)
	{
		if (_id == MGUI_MouseButton::Left)
		{
			mMouseOldX = _x;
			mDX = 0;
			mDP = -1;
		}
	}

}