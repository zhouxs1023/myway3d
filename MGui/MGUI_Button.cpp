#include "MGUI_Button.h"
#include "MGUI_Engine.h"

namespace Myway {

	ImplementRTTI(MGUI_Button, MGUI_Widget);

	MGUI_Button::MGUI_Button(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent)
		: MGUI_Widget(_lookfeel, _parent)
	{
		SetCaption(L"");
		mCharHeight = 20;
	}

	MGUI_Button::~MGUI_Button()
	{
	}

	void MGUI_Button::_AddRenderItem(MGUI_Layout * _layout)
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

		if (mCaption.Length() > 0)
		{
			const wchar_t * wstr = mCaption.c_str();
			int length = wcslen(wstr);

			const MGUI_Rect & clRect = this->GetAbsClientRect();
			Color4 color = mColor * _lookfeel->GetTextColor(mState);

			MGUI_RectF myRect;
			myRect.x0 = (float)clRect.x0;
			myRect.y0 = clRect.CenterY() - mCharHeight / 2.0f;
			myRect.x1 = (float)clRect.x0;
			myRect.y1 = clRect.CenterY() + mCharHeight / 2.0f;

			MGUI_RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), 
				MGUI_Engine::Instance()->GetDefaultShader(), MGUI_Font::Instance()->GetTexture().c_ptr());

			MGUI_RectF _rect, _uv;

			for (int i = 0; i < length; ++i)
			{
				const MGUI_Glyph * glyph = MGUI_Font::Instance()->GetGlyph(wstr[i]);

				if (glyph == NULL)
					glyph = MGUI_Font::Instance()->GetGlyph(MGUI_Helper::Instance()->GetUnknownChar());

				if (glyph == NULL)
					continue;

				myRect.x1 = myRect.x0 + glyph->aspect * mCharHeight;

				if (MGUI_Helper::Instance()->Clip(_rect, _uv, myRect, glyph->uv, clipRect))
				{
					ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv, color));
				}

				myRect.x0 = myRect.x1;
			}
		}

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			mChildren[i]->_AddRenderItem(_layout);
		}
	}

	void MGUI_Button::OnMouseLostFocus(MGUI_Widget* _new)
	{
		mState = MGUI_WidgetState::Normal;
	}

	void MGUI_Button::OnMouseSetFocus(MGUI_Widget* _old)
	{
		mState = MGUI_WidgetState::Focused;
	}

	void MGUI_Button::OnMousePressed(int _x, int _y, MGUI_MouseButton _id)
	{
		if (_id == MGUI_MouseButton::Left)
			mState = MGUI_WidgetState::Pressed;
	}

	void MGUI_Button::OnMouseReleased(int _x, int _y, MGUI_MouseButton _id)
	{
		if (_id == MGUI_MouseButton::Left)
			mState = MGUI_WidgetState::Focused;
	}

}