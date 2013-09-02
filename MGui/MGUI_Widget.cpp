#include "MGUI_Widget.h"
#include "MGUI_Layout.h"
#include "MGUI_Helper.h"
#include "MGUI_Engine.h"

namespace Myway {

	ImplementRootRTTI(MGUI_Widget);

	MGUI_Widget::MGUI_Widget(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent)
		: mParent(_parent)
		, mLookFeel(_lookfeel)
		, mColor(Color4::White)
		, mEnable(true)
		, mVisible(true)
		, mOrder(0)
		, mAbsOrder(0)
	{
		if (mParent)
			mParent->_notifyAttach(this);

		if (_lookfeel)
			_lookfeel->Init(this);
	}

	MGUI_Widget::~MGUI_Widget()
	{
		if (MGUI_InputManager::Instance()->GetMouseFocusedWidget() == this)
			MGUI_InputManager::Instance()->SetMouseFocusedWidget(NULL);

		if (MGUI_InputManager::Instance()->GetKeyFocusedWidget() == this)
			MGUI_InputManager::Instance()->SetKeyFocusedWidget(NULL);

		while (mChildren.Size() > 0)
			delete mChildren[0];

		if (mParent)
			mParent->_notifyDetach(this);
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
		for (int i = 0; i < mChildren.Size(); ++i)
		{
			if (mChildren[i]->GetOrder() > _child->GetOrder())
			{
				mChildren.Insert(i, _child);
				return ;
			}
		}

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
		int x1 = x + mRect.DX();
		int y1 = y + mRect.DY();

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
		if (mOrder == _order)
			return ;

		mOrder = _order;

		if (mParent)
		{
			mParent->_notifyDetach(this);
			mParent->_notifyAttach(this);
		}
	}

	int MGUI_Widget::GetOrder()
	{
		return mOrder;
	}

	void MGUI_Widget::_AddRenderItem(MGUI_Layout * _layout)
	{
		if (!mVisible)
			return ;

		const MGUI_LookFeel * _lookfeel = mLookFeel;

		MGUI_Rect clipRect = MGUI_Helper::Instance()->GetClipRect(mParent);
		int state = MGUI_Helper::Instance()->GetWidgetState(this);

		if (_lookfeel)
		{
			MGUI_RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), _lookfeel->GetShader(), _lookfeel->GetSkin());

			const MGUI_Rect & myRect = this->GetAbsRect();
			const MGUI_Rect & clRect = this->GetClientRect();
			const MGUI_RectF & uvRect = MGUI_Helper::Instance()->MapUVRect(_lookfeel->GetUVRect(state), _lookfeel->GetSkin());
			const MGUI_RectF & uvClientRect = MGUI_Helper::Instance()->MapUVRect(_lookfeel->GetUVClientRect(state), _lookfeel->GetSkin());

			MGUI_Helper::Instance()->AddRenderItem(ri, myRect, clRect, uvRect, uvClientRect, mColor, clipRect);
		}

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			mChildren[i]->_AddRenderItem(_layout);
		}
	}

	void  MGUI_Widget::Update()
	{
		if (!mVisible)
			return ;

		eventUpdate();

		// update rect
		MGUI_Rect client = MGUI_Helper::Instance()->GetParentRect(mParent);

		if (mParent)
			client = mParent->GetClientRect();
		
		int halfSizeX = mRect.DX() / 2;
		int halfSizeY = mRect.DY() / 2;
		int centerX = client.DX() / 2;
		int centerY = client.DY() / 2;

		if (mAlign._value & MGUI_Align::Left)
		{
			Move(0, mRect.y0);
		}
		else if (mAlign._value & MGUI_Align::Right)
		{
			Move(client.DX() - mRect.DX(), mRect.y0);
		}

		if (mAlign._value & MGUI_Align::Top)
		{
			Move(mRect.x0, 0);
		}
		else if (mAlign._value & MGUI_Align::Bottom)
		{
			Move(mRect.x0, client.DY() - mRect.DY());
		}

		if (mAlign._value & MGUI_Align::HCenter)
		{
			Move(centerX - halfSizeX, mRect.y0);
		}

		if (mAlign._value & MGUI_Align::VCenter)
		{
			Move(mRect.x0, centerY - halfSizeY);
		}

		if (mAlign._value & MGUI_Align::HStretch)
		{
			Move(0, mRect.y0);
			mRect.x1 = client.DX();
		}

		if (mAlign._value & MGUI_Align::VStretch)
		{
			Move(mRect.x0, 0);
			mRect.y1 = client.DY();
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
		mClientRect = MGUI_Rect(0, 0, mRect.DX(), mRect.DY());

		if (mLookFeel)
			mLookFeel->Affect(this);

		mAbsClientRect.x0 = mClientRect.x0 + mAbsRect.x0;
		mAbsClientRect.y0 = mClientRect.y0 + mAbsRect.y0;
		mAbsClientRect.x1 = mClientRect.x1 + mAbsRect.x0;
		mAbsClientRect.y1 = mClientRect.y1 + mAbsRect.y0;

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

		// update order
		mAbsOrder = mOrder;
		if (mParent)
			mAbsOrder += mParent->GetAbsOrder();

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
			for (int i = mChildren.Size() - 1; i >= 0; --i)
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
