#include "stdafx.h"

#include "RectEd.h"
#include "AllLookFeel.h"

namespace UIEditor {

	RectEd::RectEd(MGUI_Widget * _parent)
		: MGUI_Widget(NULL, _parent)
	{
		mLeft = new MGUI_Widget(AllLookFeel::Instance()->GetWhite(), this);
		mTop = new MGUI_Widget(AllLookFeel::Instance()->GetWhite(), this);
		mRight = new MGUI_Widget(AllLookFeel::Instance()->GetWhite(), this);
		mBottom = new MGUI_Widget(AllLookFeel::Instance()->GetWhite(), this);

		mLeft->SetColor(MGUI_Color(1, 1, 0, 0.8f));
		mTop->SetColor(MGUI_Color(1, 1, 0, 0.8f));
		mRight->SetColor(MGUI_Color(1, 1, 0, 0.8f));
		mBottom->SetColor(MGUI_Color(1, 1, 0, 0.8f));
	}

	RectEd::~RectEd()
	{
	}

	void RectEd::SetEditRect(const MGUI_Rect & _rect)
	{
		MGUI_Rect rcLeft, rcTop, rcRight, rcBottom;

		int dx = _rect.DX(), dy = _rect.DY();

		rcLeft.x0 = 0;
		rcLeft.x1 = 1;
		rcLeft.y0 = 0;
		rcLeft.y1 = dy;

		rcTop.x0 = 0;
		rcTop.x1 = dx;
		rcTop.y0 = 0;
		rcTop.y1 = 1;

		rcRight.x0 = dx - 1;
		rcRight.x1 = dx;
		rcRight.y0 = 0;
		rcRight.y1 = dy;

		rcBottom.x0 = 0;
		rcBottom.x1 = dx;
		rcBottom.y0 = dy - 1;
		rcBottom.y1 = dy;

		mLeft->SetRect(rcLeft);
		mTop->SetRect(rcTop);
		mRight->SetRect(rcRight);
		mBottom->SetRect(rcBottom);
		SetRect(_rect);
	}

	const MGUI_Rect & RectEd::GetEditRect()
	{
		return GetRect();
	}

	void RectEd::OnMouseLostFocus(MGUI_Widget* _new)
	{
		Cursor::Instance()->Unbind();
	}

	void RectEd::OnMouseSetFocus(MGUI_Widget* _old)
	{
		Cursor::Instance()->Bind(Cursor::kSize);
	}

	void RectEd::OnMouseDrag(int _x, int _y)
	{
		if (MGUI_InputManager::Instance()->IsMouseCaptured())
		{
			int dx = _x - mX;
			int dy = _y - mY;

			MGUI_Rect rect = GetEditRect();

			rect.x0 += dx;
			rect.x1 += dx;
			rect.y0 += dy;
			rect.y1 += dy;

			mX = _x; mY = _y;

			SetEditRect(rect);
		}
	}

	void RectEd::OnMousePressed(int _x, int _y, MGUI_MouseButton _id)
	{
		if (_id == MGUI_MouseButton::Left)
		{
			mX = _x;
			mY = _y;
		}
	}

	void RectEd::OnMouseReleased(int _x, int _y, MGUI_MouseButton _id)
	{
		if (_id == MGUI_MouseButton::Left)
		{
			mX = _x;
			mY = _y;
		}
	}


}