#include "MGUI_Label.h"
#include "MGUI_Engine.h"

namespace Myway {

	ImplementRTTI(MGUI_Label, MGUI_Widget);

	MGUI_Label::MGUI_Label(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent)
		: MGUI_Widget(_lookfeel, _parent)
	{
		SetCaption(L"Label");
		mCharHeight = 20;
	}

	MGUI_Label::~MGUI_Label()
	{
	}

	void MGUI_Label::_AddRenderItem(MGUI_Layout * _layout)
	{
		if (!mVisible)
			return ;

		const MGUI_LookFeel * _lookfeel = mLookFeel;

		MGUI_Rect clipRect = MGUI_Helper::Instance()->GetClipRect(mParent);

		int state = MGUI_WidgetState::Normal;

		if (!mEnable)
			state = MGUI_WidgetState::Disabled;

		if (_lookfeel)
		{
			MGUI_RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), _lookfeel->GetShader(), _lookfeel->GetSkin());

			const MGUI_Rect & myRect = this->GetAbsRect();
			const MGUI_Rect & clRect = this->GetClientRect();
			const MGUI_RectF & uvRect = MGUI_Helper::Instance()->MapUVRect(_lookfeel->GetUVRect(state), _lookfeel->GetSkin());
			const MGUI_RectF & uvClientRect = MGUI_Helper::Instance()->MapUVRect(_lookfeel->GetUVClientRect(state), _lookfeel->GetSkin());

			MGUI_Helper::Instance()->AddRenderItem(ri, myRect, clRect, uvRect, uvClientRect, mColor, clipRect);
		}

		const wchar_t * wstr = mCaption.c_str();
		int length = wcslen(wstr);

		const MGUI_Rect & clRect = this->GetAbsClientRect();

		MGUI_RectF myRect;
		myRect.x0 = (float)clRect.x0;
		myRect.y0 = clRect.CenterY() - mCharHeight / 2.0f;
		myRect.x1 = myRect.x0;
		myRect.y1 = clRect.CenterY() + mCharHeight / 2.0f;

		MGUI_RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(),
			MGUI_Engine::Instance()->GetDefaultShader(), MGUI_Font::Instance()->GetTexture().c_ptr());

		MGUI_RectF _rect, _uv;
		Color4 color = mColor;
		
		if (_lookfeel)
			color *= _lookfeel->GetTextColor(state);

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
				ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv, color));
			}

			myRect.x0 = myRect.x1;
		}
	}

}