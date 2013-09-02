#include "MGUI_Button.h"
#include "MGUI_Engine.h"

namespace Myway {

	ImplementRTTI(MGUI_Button, MGUI_Widget);

	MGUI_Button::MGUI_Button(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent)
		: MGUI_Widget(_lookfeel, _parent)
		, mCaptionChanged(true)
		, mCharHeight(20)
		, mMousePressed(false)
	{
		SetCaption(L"");
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

		if (mCaptionChanged)
		{
			mCaptionGlyphs.Clear();
			mCaptionWidth = 0;

			const wchar_t * wstr = mCaption.c_str();
			int length = wcslen(wstr);

			for (int i = 0; i < length; ++i)
			{
				const MGUI_Glyph * glyph = MGUI_Font::Instance()->GetGlyph(wstr[i]);

				if (glyph == NULL)
					glyph = MGUI_Font::Instance()->GetGlyph(MGUI_Helper::Instance()->GetUnknownChar());

				if (glyph == NULL)
					continue;

				mCaptionGlyphs.PushBack(glyph);

				mCaptionWidth += glyph->aspect * mCharHeight;
			}

			mCaptionChanged = false;
		}

		if (mCaptionGlyphs.Size() > 0)
		{
			const MGUI_Rect & clRect = this->GetAbsClientRect();
			Color4 color = mColor * _lookfeel->GetTextColor(state);

			MGUI_RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(),
				MGUI_Engine::Instance()->GetDefaultShader(), MGUI_Font::Instance()->GetTexture().c_ptr());

			MGUI_RectF myRect;
			myRect.x0 = (float)clRect.x0 + 0.5f * (clRect.DX() - mCaptionWidth);
			myRect.x1 = myRect.x0;

			myRect.y0 = clRect.CenterY() - mCharHeight / 2.0f;
			myRect.y1 = clRect.CenterY() + mCharHeight / 2.0f;

			MGUI_RectF _rect, _uv;

			for (int i = 0; i < mCaptionGlyphs.Size(); ++i)
			{
				const MGUI_Glyph * glyph = mCaptionGlyphs[i];

				myRect.x1 = myRect.x0 + glyph->aspect * mCharHeight;

				if (MGUI_Helper::Instance()->Clip(_rect, _uv, myRect, glyph->uv, clipRect))
				{
					ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv, color));
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

	void MGUI_Button::OnMousePressed(int _x, int _y, MGUI_MouseButton _id)
	{
		if (_id == MGUI_MouseButton::Left)
		{
			mMousePressed = true;
		}
	}

	void MGUI_Button::OnMouseReleased(int _x, int _y, MGUI_MouseButton _id)
	{
		if (_id == MGUI_MouseButton::Left)
		{
			mMousePressed = false;
		}
	}
}