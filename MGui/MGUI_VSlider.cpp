#include "MGUI_VSlider.h"
#include "MGUI_Engine.h"

namespace Myway {

	ImplementRTTI(MGUI_VSlider, MGUI_Widget);

	MGUI_VSlider::MGUI_VSlider(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent)
		: MGUI_Widget(_lookfeel, _parent)
		, mMouseOldY(0)
		, mDP(-1)
		, mDY(0)
	{
		mBnSlider = new MGUI_Button(_lookfeel->GetChild("SliderTrack"), this);

		mBnSlider->eventMousePressed += OnSliderPressed(this, &MGUI_VSlider::OnSliderPressed_);
		mBnSlider->eventMouseReleased += OnSliderReleased(this, &MGUI_VSlider::OnSliderReleased_);
		mBnSlider->eventMouseDrag += OnSliderDrag(this, &MGUI_VSlider::OnSliderDrag_);
	}

	MGUI_VSlider::~MGUI_VSlider()
	{
	}

	void MGUI_VSlider::_AddRenderItem(MGUI_Layout * _layout)
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

	void MGUI_VSlider::UpdateChildren()
	{
		float acespt = (float)(mBnSlider->GetLookFeel()->GetUVRect(MGUI_WidgetState::Normal).DY()) /
			(float)(mBnSlider->GetLookFeel()->GetUVRect(MGUI_WidgetState::Normal).DX());

		MGUI_Rect myRectSlider;

		int w = mClientRect.DX();
		int h = int(acespt * w);
		float hh = h * 0.5f;

		myRectSlider.x0 = 0;
		myRectSlider.x1 = mClientRect.DX();

		float length = mClientRect.DY() - hh;

		if (length < 0)
			length  = 0;

		int count = mMaxValue - mMinValue + 1;

		float dStep = length / (float)count;

		d_assert (count >= 1);

		int pos = (mDP == -1) ? mPosition : mDP;

		float ky = float(pos) / (mMaxValue - mMinValue + 1);
		float y = ky * length +  hh;

		myRectSlider.y0 = int(y - hh);
		myRectSlider.y1 = int(y + hh);

		if (mDP != -1)
		{
			myRectSlider.y0 += mDY;
			myRectSlider.y1 += mDY;

			SetPosition(mDP + int(mDY / dStep));
		}

		if (myRectSlider.y0 < 0)
		{
			myRectSlider.y0 = 0;
			myRectSlider.y1 = myRectSlider.y0 + h;
		}
		else if (myRectSlider.y1 > mClientRect.DY())
		{
			myRectSlider.y0 = mClientRect.DY() - h;
			myRectSlider.y1 = mClientRect.DY();
		}

		mBnSlider->SetRect(myRectSlider);

		for (int i = 0; i < mChildren.Size(); ++i)
			mChildren[i]->Update();
	}

	void MGUI_VSlider::OnMousePressed(int _x, int _y, MGUI_MouseButton _id)
	{
		MGUI_Rect myRect = mBnSlider->GetAbsRect();

		int dy = _y - myRect.CenterY();

		if (dy > 0)
			SetPosition(mPosition + 1);
		else if (dy < 0)
			SetPosition(mPosition - 1);
	}

	void MGUI_VSlider::OnSliderDrag_(int _x, int _y)
	{
		mDY = _y - mMouseOldY;
	}

	void MGUI_VSlider::OnSliderPressed_(int _x, int _y, MGUI_MouseButton _id)
	{
		if (_id == MGUI_MouseButton::Left)
		{
			mDP = mPosition;
			mMouseOldY = _y;
			mDY = 0;
		}
	}

	void MGUI_VSlider::OnSliderReleased_(int _x, int _y, MGUI_MouseButton _id)
	{
		if (_id == MGUI_MouseButton::Left)
		{
			mDP = -1;
			mMouseOldY = 0;
			mDY = 0;
		}
	}

}