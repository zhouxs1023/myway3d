#include "MGUI_ListBoxItem.h"
#include "MGUI_Engine.h"
#include "MGUI_ListBox.h"

namespace Myway {

	ImplementRTTI(MGUI_ListBoxItem, MGUI_Widget);

	MGUI_ListBoxItem::MGUI_ListBoxItem(MGUI_ListBox * _listBox, const MGUI_LookFeel * _lookfeel,
									   const MGUI_String & _text, void * _userData)
		: MGUI_Widget(_lookfeel, _listBox->GetItemWidget())
	{
		mListBox = _listBox;
		mText = _text;
		mUserData = _userData;
	}

	MGUI_ListBoxItem::~MGUI_ListBoxItem()
	{
	}

	void MGUI_ListBoxItem::_AddRenderItem(MGUI_Layout * _layout)
	{
		const MGUI_LookFeel * _lookfeel = mLookFeel;

		int state = MGUI_Helper::Instance()->GetWidgetState(this);
		
		if (state != MGUI_WidgetState::Disabled && state != MGUI_WidgetState::Focused && mListBox->GetSelectItem() == this)
			state = MGUI_WidgetState::Selected;

		const MGUI_Rect & clipRect = mListBox->GetClipRect();

		if (_lookfeel)
		{
			MGUI_RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), _lookfeel->GetShader(), _lookfeel->GetSkin());

			const MGUI_Rect & myRect = this->GetAbsRect();
			const MGUI_Rect & clRect = this->GetClientRect();
			const MGUI_RectF & uvRect = MGUI_Helper::Instance()->MapUVRect(_lookfeel->GetUVRect(state), _lookfeel->GetSkin());
			const MGUI_RectF & uvClientRect = MGUI_Helper::Instance()->MapUVRect(_lookfeel->GetUVClientRect(state), _lookfeel->GetSkin());

			MGUI_Helper::Instance()->AddRenderItem(ri, myRect, clRect, uvRect, uvClientRect, mColor, clipRect);
		}

		if (mText.Length() > 0)
		{
			const wchar_t * wstr = mText.c_str();
			int length = wcslen(wstr);

			const MGUI_Rect & clRect = this->GetAbsClientRect();
			Color4 color = mColor * _lookfeel->GetTextColor(state);
			float charHeight = (float)clRect.DY();
			float centerY = (clRect.y0 + clRect.y1) * 0.5f;

			MGUI_RectF myRect;
			myRect.x0 = (float)clRect.x0;
			myRect.y0 = centerY - charHeight / 2.0f;
			myRect.x1 = (float)clRect.x0;
			myRect.y1 = centerY + charHeight / 2.0f;

			MGUI_RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(),
				MGUI_Engine::Instance()->GetDefaultShader(), MGUI_Font::Instance()->GetTexture().c_ptr());


			for (int i = 0; i < length; ++i)
			{
				const MGUI_Glyph * glyph = MGUI_Font::Instance()->GetGlyph(wstr[i]);

				if (glyph == NULL)
					glyph = MGUI_Font::Instance()->GetGlyph(MGUI_Helper::Instance()->GetUnknownChar());

				if (glyph == NULL)
					continue;

				myRect.x1 = myRect.x0 + glyph->aspect * charHeight;

				MGUI_RectF _rect, _uv;

				if (MGUI_Helper::Instance()->Clip(_rect, _uv, myRect, glyph->uv, clipRect))
				{
					ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv, color));
					ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv, color));
				}

				myRect.x0 = myRect.x1;
			}
		}
	}


	void MGUI_ListBoxItem::OnMousePressed(int _x, int _y, MGUI_MouseButton _id)
	{
		mListBox->OnSelect_(this);
	}

}