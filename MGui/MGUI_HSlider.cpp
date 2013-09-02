#include "MGUI_HSlider.h"
#include "MGUI_Engine.h"

namespace Myway {

	ImplementRTTI(MGUI_HSlider, MGUI_Widget);

	MGUI_HSlider::MGUI_HSlider(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent)
		: MGUI_Widget(_lookfeel, _parent)
		, mMouseOldX(0)
		, mDP(-1)
		, mDX(0)
	{
		mBnSlider = new MGUI_Button(_lookfeel->GetChild("SliderTrack"), this);

		mBnSlider->eventMousePressed += OnSliderPressed(this, &MGUI_HSlider::OnSliderPressed_);
		mBnSlider->eventMouseReleased += OnSliderReleased(this, &MGUI_HSlider::OnSliderReleased_);
		mBnSlider->eventMouseDrag += OnSliderDrag(this, &MGUI_HSlider::OnSliderDrag_);
	}

	MGUI_HSlider::~MGUI_HSlider()
	{
	}

	void MGUI_HSlider::_AddRenderItem(MGUI_Layout * _layout)
	{
		if (!mVisible)
			return ;

		if (mRect.DX() == 0 || mRect.DY() == 0)
			return ;

		const MGUI_LookFeel * _lookfeel = mLookFeel;
		MGUI_RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), _lookfeel->GetShader(), _lookfeel->GetSkin());
		int state = MGUI_Helper::Instance()->GetWidgetState(this);

		const MGUI_Rect & myRect = this->GetAbsRect();
		const MGUI_Rect & clRect = this->GetClientRect();
		const MGUI_RectF & uvRect = MGUI_Helper::Instance()->MapUVRect(_lookfeel->GetUVRect(state), _lookfeel->GetSkin());
		const MGUI_RectF & uvClientRect = MGUI_Helper::Instance()->MapUVRect(_lookfeel->GetUVClientRect(state), _lookfeel->GetSkin());
		const MGUI_Rect & clipRect = MGUI_Helper::Instance()->GetClipRect(mParent);

		MGUI_Helper::Instance()->AddRenderItem(ri, myRect, clRect, uvRect, uvClientRect, mColor, clipRect);

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			mChildren[i]->_AddRenderItem(_layout);
		}
	}

	void MGUI_HSlider::UpdateChildren()
	{
		float acespt = (float)(mBnSlider->GetLookFeel()->GetUVRect(MGUI_WidgetState::Normal).DX()) /
			(float)(mBnSlider->GetLookFeel()->GetUVRect(MGUI_WidgetState::Normal).DY());

		MGUI_Rect myRectSlider;

		int h = mClientRect.DY();
		int w = int(acespt * h);
		float hw = w * 0.5f;

		myRectSlider.y0 = 0;
		myRectSlider.y1 = mClientRect.DY();

		float length = mClientRect.DX() - hw;

		if (length < 0)
			length  = 0;

		int count = mMaxValue - mMinValue + 1;

		float dStep = length / (float)count;

		d_assert (count >= 1);

		int pos = (mDP == -1) ? mPosition : mDP;

		float kx = float(pos) / (mMaxValue - mMinValue + 1);
		float x = int(kx * length) +  hw;

		myRectSlider.x0 = int(x - hw);
		myRectSlider.x1 = int(x + hw);

		if (mDP != -1)
		{
			myRectSlider.x0 += mDX;
			myRectSlider.x1 += mDX;

			SetPosition(mDP + int(mDX / dStep));
		}

		if (myRectSlider.x0 < 0)
		{
			myRectSlider.x0 = 0;
			myRectSlider.x1 = myRectSlider.x0 + w;
		}
		else if (myRectSlider.x1 > mClientRect.DX())
		{
			myRectSlider.x0 = mClientRect.DX() - w;
			myRectSlider.x1 = mClientRect.DX();
		}

		mBnSlider->SetRect(myRectSlider);

		for (int i = 0; i < mChildren.Size(); ++i)
			mChildren[i]->Update();
	}

	void MGUI_HSlider::OnMousePressed(int _x, int _y, MGUI_MouseButton _id)
	{
		MGUI_Rect myRect = mBnSlider->GetAbsRect();

		int dx = _x - myRect.CenterX();

		if (dx > 0)
			SetPosition(mPosition + 1);
		else if (dx < 0)
			SetPosition(mPosition - 1);
	}

	void MGUI_HSlider::OnSliderDrag_(int _x, int _y)
	{
		mDX = _x - mMouseOldX;
	}

	void MGUI_HSlider::OnSliderPressed_(int _x, int _y, MGUI_MouseButton _id)
	{
		if (_id == MGUI_MouseButton::Left)
		{
			mDP = mPosition;
			mMouseOldX = _x;
			mDX = 0;
		}
	}

	void MGUI_HSlider::OnSliderReleased_(int _x, int _y, MGUI_MouseButton _id)
	{
		if (_id == MGUI_MouseButton::Left)
		{
			mDP = -1;
			mMouseOldX = 0;
			mDX = 0;
		}
	}

}