#include "MGUI_CheckBox.h"
#include "MGUI_Engine.h"

namespace Myway {

	ImplementRTTI(MGUI_CheckBox, MGUI_Widget);

	MGUI_CheckBox::MGUI_CheckBox(MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent)
		: MGUI_Widget(_lookfeel, _parent)
	{
		mChecked = false;

		mCharHeight = 20;
		SetCaption("CheckBox");
	}

	MGUI_CheckBox::~MGUI_CheckBox()
	{
	}
	
	void MGUI_CheckBox::_AddRenderItem(MGUI_Layout * _layout)
	{
		if (!mVisible)
			return ;

		MGUI_LookFeel * _lookfeel = (MGUI_LookFeel *)mLookFeel;
		MGUI_Rect clipRect = MGUI_Helper::Instance()->GetClipRect(mParent);
		int state = MGUI_Helper::Instance()->GetWidgetState(this);

		float textOffX = 0;

		if (_lookfeel)
		{
			MGUI_RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), _lookfeel->GetShader(), _lookfeel->GetSkin());

				  MGUI_Rect   myRect = this->GetAbsRect();
			const MGUI_Rect & clRect = this->GetClientRect();
			const MGUI_RectF & uvRect = MGUI_Helper::Instance()->MapUVRect(_lookfeel->GetUVRect(state), _lookfeel->GetSkin());
			const MGUI_RectF & uvClientRect = MGUI_Helper::Instance()->MapUVRect(_lookfeel->GetUVClientRect(state), _lookfeel->GetSkin());

			float acspect = uvRect.DX() / uvRect.DY();

			myRect.x1 = myRect.x0 + int(acspect * myRect.DY());

			textOffX = myRect.x1 + 4.0f;

			MGUI_Helper::Instance()->AddRenderItem(ri, myRect, clRect, uvRect, uvClientRect, mColor, clipRect);

			/*if (mChecked)
			{
				const MGUI_RectF & uvCheckedRect = MGUI_Helper::Instance()->MapUVRect(_lookfeel->GetCheckedUVRect(), _lookfeel->GetSkin());

				const MGUI_Vertex * quad = MGUI_Helper::Instance()->GetQuad(myRect, uvCheckedRect, color);

				ri->AddQuad(quad);
			}*/
		}

		// render text
		if (mCaption.Length() > 0)
		{
			const wchar_t * wstr = mCaption.c_wstr();
			int length = wcslen(wstr);

			const MGUI_Rect & clRect = this->GetAbsClientRect();
			Color4 color = mColor * _lookfeel->GetTextColor(state);

			MGUI_RectF myRect;
			myRect.x0 = textOffX;
			myRect.y0 = clRect.CenterY() - mCharHeight / 2.0f;
			myRect.x1 = myRect.x0;
			myRect.y1 = clRect.CenterY() + mCharHeight / 2.0f;

			MGUI_RectF _rect, _uv;

			MGUI_RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(),
				MGUI_Engine::Instance()->GetDefaultShader(), MGUI_Font::Instance()->GetTexture().c_ptr());

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

	void MGUI_CheckBox::OnMousePressed(int _x, int _y, MGUI_MouseButton _id)
	{
	}

	void MGUI_CheckBox::OnMouseReleased(int _x, int _y, MGUI_MouseButton _id)
	{
		if (_id == MGUI_MouseButton::Left)
		{
			mChecked = !mChecked;

			eventChecked(mChecked);
		}
	}



}