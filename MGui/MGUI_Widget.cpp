#include "MGUI_Widget.h"
#include "MGUI_Layout.h"
#include "MGUI_Helper.h"

namespace Myway {

	ImplementRootRTTI(MGUI_Widget);

	MGUI_Widget::MGUI_Widget(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent)
		: mParent(_parent)
		, mLookFeel(_lookfeel)
		, mColor(Color4::White)
		, mEnable(true)
		, mVisible(true)
		, mState(MGUI_WidgetState::Normal)
	{
		if (mParent)
			mParent->_notifyAttach(this);

		if (_lookfeel)
			_lookfeel->Init(this);
	}

	MGUI_Widget::~MGUI_Widget()
	{
		if (mParent)
			mParent->_notifyDetach(this);

		while (mChildren.Size() > 0)
			delete mChildren[0];
	}

	void MGUI_Widget::SetName(const TString128 & _name)
	{
		mName = _name;
	}

	const TString128 & MGUI_Widget::GetName() const
	{
		return mName;
	}

	MGUI_Widget * MGUI_Widget::GetChild(const TString128 & _name)
	{
		for (int i = 0; i < mChildren.Size(); ++i)
		{
			if (mChildren[i]->GetName() == _name)
				return mChildren[i];
		}

		return NULL;
	}

	void MGUI_Widget::_notifyAttach(MGUI_Widget * _child)
	{
		mChildren.PushBack(_child);
	}

	void MGUI_Widget::_notifyDetach(MGUI_Widget * _child)
	{
		for (int i = 0; i < mChildren.Size(); ++i)
		{
			if (mChildren[i] == _child)
			{
				mChildren.Erase(i);
				return ;
			}
		}

		d_assert(0);
	}

	MGUI_Widget * MGUI_Widget::GetParent()
	{
		return mParent;
	}

	const MGUI_LookFeel * MGUI_Widget::GetLookFeel()
	{
		return mLookFeel;
	}

	void MGUI_Widget::SetRect(const MGUI_Rect & _rect)
	{
		mRect = _rect;
	}

	void MGUI_Widget::Move(int x, int y)
	{
		int x1 = x + mRect.Width();
		int y1 = y + mRect.Height();

		SetRect(x, y, x1, y1);
	}

	const MGUI_Rect & MGUI_Widget::GetRect()
	{
		return mRect;
	}

	const MGUI_Rect & MGUI_Widget::GetAbsRect()
	{
		return mAbsRect;
	}

	void MGUI_Widget::SetClientRect(const MGUI_Rect & _rect)
	{
		mClientRect = _rect;
	}

	const MGUI_Rect & MGUI_Widget::GetClientRect()
	{
		return mClientRect;
	}

	const MGUI_Rect & MGUI_Widget::GetAbsClientRect()
	{
		return mAbsClientRect;
	}

	const MGUI_Rect & MGUI_Widget::GetClipRect()
	{
		return mClipRect;
	}

	void MGUI_Widget::SetAlign(MGUI_Align align)
	{
		if (mAlign != align)
		{
			mAlign = align;
		}
	}

	MGUI_Align MGUI_Widget::GetAlign()
	{
		return mAlign;
	}

	void MGUI_Widget::SetColor(const MGUI_Color & _color)
	{
		mColor = _color;
	}

	const MGUI_Color & MGUI_Widget::GetColor()
	{
		return mColor;
	}

	void MGUI_Widget::SetOrder(int _order)
	{
		mOrder = _order;
	}

	int MGUI_Widget::GetOrder()
	{
		return mOrder;
	}

	int MGUI_Widget::GetAbsOrder()
	{
		return mAbsOrder;
	}

	void MGUI_Widget::_AddRenderItem(MGUI_Layout * _layout)
	{
		if (!mVisible)
			return ;

		const MGUI_LookFeel * _lookfeel = mLookFeel;

		MGUI_Rect clipRect = MGUI_Helper::Instance()->GetClipRect(mParent);

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

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			mChildren[i]->_AddRenderItem(_layout);
		}
	}

	void  MGUI_Widget::Update()
	{
		MGUI_Rect client = MGUI_Helper::Instance()->GetParentRect(mParent);

		if (mParent)
			client = mParent->GetClientRect();
		
		int halfSizeX = mRect.Width() / 2;
		int halfSizeY = mRect.Height() / 2;
		int centerX = client.Width() / 2;
		int centerY = client.Height() / 2;

		switch (mAlign._value)
		{
		case MGUI_Align::None:
			break;

		case MGUI_Align::HCenter:
			Move(centerX - halfSizeX, mRect.y0);
			break;

		case MGUI_Align::VCenter:
			Move(mRect.x0, centerY - halfSizeY);
			break;

		case MGUI_Align::Center:
			Move(centerX - halfSizeX, centerY - halfSizeY);
			break;

		case MGUI_Align::Left:
			Move(0, mRect.y0);
			break;

		case MGUI_Align::Right:
			Move(client.Width() - mRect.Width(), mRect.y0);
			break;

		case MGUI_Align::HStretch:
			Move(0, mRect.y0);
			mRect.x1 = client.Width();
			break;

		case MGUI_Align::Top:
			Move(mRect.x0, 0);
			break;

		case MGUI_Align::Bottom:
			Move(mRect.x0, client.Height() - mRect.Height());
			break;

		case MGUI_Align::VStretch:
			Move(mRect.x0, 0);
			mRect.y1 = client.Height();
			break;

		case MGUI_Align::Stretch:
			mRect.x0 = 0;
			mRect.y0 = 0;
			mRect.x1 = client.Width();
			mRect.y1 = client.Height();
			break;

		case MGUI_Align::LeftTop:
			Move(0, 0);
			break;

		case MGUI_Align::LeftBottom:
			Move(0, client.Height() - mRect.Height());
			break;

		case MGUI_Align::RightTop:
			Move(client.Width() - mRect.Width(), 0);
			break;

		case MGUI_Align::RightBottom:
			Move(client.Width() - mRect.Width(), client.Height() - mRect.Height());
			break;

		case MGUI_Align::LeftCenter:
			Move(0, centerY - halfSizeY);
			break;

		case MGUI_Align::RightCenter:
			Move(client.Width() - mRect.Width(), centerY - halfSizeY);
			break;

		case MGUI_Align::TopCenter:
			Move(centerX - halfSizeX, 0);
			break;

		case MGUI_Align::BottomCenter:
			Move(centerX - halfSizeX, client.Height() - mRect.Height());
			break;
		}

		// update abs rect
		if (mParent)
		{
			const MGUI_Rect & parentRect = mParent->GetAbsClientRect();
			mAbsRect.x0 = parentRect.x0 + mRect.x0;
			mAbsRect.y0 = parentRect.y0 + mRect.y0;
			mAbsRect.x1 = parentRect.x0 + mRect.x1;
			mAbsRect.y1 = parentRect.y0 + mRect.y1;
		}
		else
		{
			mAbsRect = mRect;
		}

		// update client rect
		mClientRect = mRect;

		if (mLookFeel)
			mLookFeel->Affect(this);

		mAbsClientRect.x0 = mClientRect.x0 + mAbsRect.x0;
		mAbsClientRect.y0 = mClientRect.y0 + mAbsRect.y0;
		mAbsClientRect.x1 = mClientRect.x1 + mAbsRect.x0;
		mAbsClientRect.y1 = mClientRect.y1 + mAbsRect.y0;

		if (!mEnable)
			mState = MGUI_WidgetState::Disabled;

		if (mParent)
		{
			const MGUI_Rect & clipRect = mParent->GetClipRect();

			mClipRect.x0 = Math::Maximum(mAbsClientRect.x0, clipRect.x0);
			mClipRect.y0 = Math::Maximum(mAbsClientRect.y0, clipRect.y0);
			mClipRect.x1 = Math::Minimum(mAbsClientRect.x1, clipRect.x1);
			mClipRect.y1 = Math::Minimum(mAbsClientRect.y1, clipRect.y1);
		}
		else
		{
			mClipRect = mAbsClientRect;
		}

		OnUpdate();

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			mChildren[i]->Update();
		}
	}

	MGUI_Widget * MGUI_Widget::Pick(int _x, int _y)
	{
		MGUI_Widget * widget = NULL;

		MGUI_Rect myRect = GetAbsRect();

		if (mVisible &&
			myRect.x0 < _x && _x < myRect.x1 &&
			myRect.y0 < _y && _y < myRect.y1)
		{
			for (int i = 0; i < mChildren.Size(); ++i)
			{
				widget = mChildren[i]->Pick(_x, _y);

				if (widget)
					return widget;
			}

			widget = this;
		}

		return widget;
	}
}
