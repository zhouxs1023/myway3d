#include "MGUI_EditBox.h"
#include "MGUI_Engine.h"

namespace Myway {

	ImplementRTTI(MGUI_EditBox, MGUI_Widget);

	MGUI_EditBox::MGUI_EditBox(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent)
		: MGUI_Widget(_lookfeel, _parent)
	{
		SetCaption(L"");
		mCharHeight = 20;
		mSelectBackColor = Color4(0.5f, 0.5f, 0.8f);

		mSelectIndex = mSelectStartIndex = mSelectIndex = 0;

		mKeyFocused = false;
		mStatic = false;

		mTimerId = -1;
		mSelectVisible = false;
	}

	MGUI_EditBox::~MGUI_EditBox()
	{
	}

	void MGUI_EditBox::SetStatic(bool _static)
	{
		mStatic = _static;

		if (MGUI_InputManager::Instance()->GetKeyFocusedWidget() == this)
			MGUI_InputManager::Instance()->SetKeyFocusedWidget(NULL);
	}

	void MGUI_EditBox::_AddRenderItem(MGUI_Layout * _layout)
	{
		if (!mVisible)
			return ;

		MGUI_LookFeel * _lookfeel = (MGUI_LookFeel *)mLookFeel;
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

		const MGUI_Rect & clRect = this->GetAbsClientRect();
		const Color4 & colorSelector = _lookfeel->GetTextColor(MGUI_WidgetState::Selected);
		const Color4 & colorSelectorBack = mSelectBackColor;

		MGUI_RectF myRect;
		myRect.x0 = (float)clRect.x0;
		myRect.y0 = clRect.CenterY() - mCharHeight / 2.0f;
		myRect.x1 = (float)clRect.x0;
		myRect.y1 = clRect.CenterY() + mCharHeight / 2.0f;

		const wchar_t * wstr = mCaption.c_str();
		int length = wcslen(wstr);

		const MGUI_Glyph * glyphSelector = MGUI_Font::Instance()->GetGlyph(MGUI_Font::kSelector);
		const MGUI_Glyph * glyphSelectorBack = MGUI_Font::Instance()->GetGlyph(MGUI_Font::kSelectorBack);

		MGUI_RectF _rect, _uv;

		if (length > 0)
		{
			clipRect = GetAbsClientRect();

			MGUI_RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(),
				MGUI_Engine::Instance()->GetDefaultShader(), MGUI_Font::Instance()->GetTexture().c_ptr());

			for (int i = 0; i < length; ++i)
			{
				const MGUI_Glyph * glyph = MGUI_Font::Instance()->GetGlyph(wstr[i]);
				Color4 color = mColor * _lookfeel->GetTextColor(state);

				if (glyph == NULL)
					glyph = MGUI_Font::Instance()->GetGlyph(MGUI_Helper::Instance()->GetUnknownChar());

				if (glyph == NULL)
					continue;

				myRect.x1 = myRect.x0 + glyph->aspect * mCharHeight;

				// select back
				if (mKeyFocused)
				{
					if (mSelectStartIndex < mSelectEndIndex &&
						i >= mSelectStartIndex && i < mSelectEndIndex)
					{

						if (MGUI_Helper::Instance()->Clip(_rect, _uv, myRect, glyphSelectorBack->uv, clipRect))
							ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv, colorSelectorBack));

						color.r = 1 - color.r;
						color.g = 1 - color.g;
						color.b = 1 - color.b;
					}
				}

				if (MGUI_Helper::Instance()->Clip(_rect, _uv, myRect, glyph->uv, clipRect))
				{
					ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv,  color));
					ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv,  color));
				}

				// select
				if (mKeyFocused)
				{
					if (mSelectVisible &&
						mSelectStartIndex == mSelectEndIndex &&
						mSelectStartIndex == i)
					{
						MGUI_RectF myRect_ = myRect;

						float half_w = glyphSelector->aspect * mCharHeight / 2;

						myRect_.x0 = myRect.x0 - half_w - .5f;
						myRect_.x1 = myRect.x0 + half_w - .5f;

						if (MGUI_Helper::Instance()->Clip(_rect, _uv, myRect_, glyphSelector->uv, clipRect))
							ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv, colorSelector));
					}
				}

				myRect.x0 = myRect.x1;
			}
		}

		if (mKeyFocused)
		{
			if (mSelectVisible &&
				mSelectStartIndex == mSelectEndIndex &&
				mSelectStartIndex == length)
			{
				MGUI_RectF myRect_ = myRect;

				float half_w = glyphSelector->aspect * mCharHeight / 2;

				myRect_.x0 = myRect.x0 - half_w - .5f;
				myRect_.x1 = myRect.x0 + half_w - .5f;

				MGUI_RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(),
					MGUI_Engine::Instance()->GetDefaultShader(), MGUI_Font::Instance()->GetTexture().c_ptr());

				if (MGUI_Helper::Instance()->Clip(_rect, _uv, myRect_, glyphSelector->uv, clipRect))
					ri->AddQuad(MGUI_Helper::Instance()->GetQuad(_rect, _uv, colorSelector));
			}
		}

		
		for (int i = 0; i < mChildren.Size(); ++i)
		{
			mChildren[i]->_AddRenderItem(_layout);
		}
	}

	void MGUI_EditBox::OnMousePressed(int _x, int _y, MGUI_MouseButton _id)
	{
		if (mStatic)
			return ;

		mSelectStartIndex = mSelectEndIndex = 0;

		if (mCaption.Length() == 0)
			return ;

		const wchar_t * wstr = mCaption.c_str();
		int length = wcslen(wstr);

		int selectIndex = length;

		const MGUI_Rect & clRect = this->GetAbsClientRect();

		MGUI_RectF myRect;
		myRect.x0 = (float)clRect.x0;
		myRect.y0 = clRect.CenterY() - mCharHeight / 2.0f;
		myRect.x1 = (float)clRect.x0;
		myRect.y1 = clRect.CenterY() + mCharHeight / 2.0f;

		for (int i = 0; i < length; ++i)
		{
			const MGUI_Glyph * glyph = MGUI_Font::Instance()->GetGlyph(wstr[i]);

			if (glyph == NULL)
				glyph = MGUI_Font::Instance()->GetGlyph(MGUI_Helper::Instance()->GetUnknownChar());

			if (glyph == NULL)
				continue;

			myRect.x1 = myRect.x0 + glyph->aspect * mCharHeight;

			if (_x < myRect.x1 - 2)
			{
				selectIndex = i;
				break;
			}

			myRect.x0 = myRect.x1;
		}

		mSelectIndex = mSelectStartIndex = mSelectEndIndex = Math::Maximum(selectIndex, 0);
	}

	void MGUI_EditBox::OnMouseDrag(int _x, int _y)
	{
		if (mStatic)
			return ;

		if (mCaption.Length() == 0)
			return ;

		const wchar_t * wstr = mCaption.c_str();
		int length = wcslen(wstr);

		int selectIndex = length;

		const MGUI_Rect & clRect = this->GetAbsClientRect();

		MGUI_RectF myRect;
		myRect.x0 = (float)clRect.x0;
		myRect.y0 = clRect.CenterY() - mCharHeight / 2.0f;
		myRect.x1 = (float)clRect.x0;
		myRect.y1 = clRect.CenterY() + mCharHeight / 2.0f;

		for (int i = 0; i < length; ++i)
		{
			const MGUI_Glyph * glyph = MGUI_Font::Instance()->GetGlyph(wstr[i]);

			if (glyph == NULL)
				glyph = MGUI_Font::Instance()->GetGlyph(MGUI_Helper::Instance()->GetUnknownChar());

			if (glyph == NULL)
				continue;

			myRect.x1 = myRect.x0 + glyph->aspect * mCharHeight;

			if (_x < myRect.x1 - 2)
			{
				selectIndex = i;
				break;
			}

			myRect.x0 = myRect.x1;
		}

		mSelectStartIndex = Math::Minimum(mSelectIndex, selectIndex);
		mSelectEndIndex = Math::Maximum(mSelectIndex, selectIndex);
	}

	void MGUI_EditBox::OnKeyLostFocus(MGUI_Widget * _new)
	{
		mKeyFocused = false;

		if (mTimerId != -1)
		{
			MGUI_InputManager::Instance()->EndTimer(mTimerId);
			mTimerId = -1;
			mSelectVisible = false;

			MGUI_InputManager::Instance()->eventTimer -= OnTimer(this, &MGUI_EditBox::OnTimer_);
		}
	}

	void MGUI_EditBox::OnKeySetFocus(MGUI_Widget* _old)
	{
		mKeyFocused = true;

		if (!mEnable || mStatic)
			mKeyFocused = false;

		if (mKeyFocused)
		{
			mTimerId = MGUI_InputManager::Instance()->StartTimer(500);
			mSelectVisible = true;

			MGUI_InputManager::Instance()->eventTimer += OnTimer(this, &MGUI_EditBox::OnTimer_);
		}
	}

	void MGUI_EditBox::OnKeyPressed(MGUI_KeyCode _key, MGUI_Char _char)
	{
		if (!mEnable || mStatic)
			return ;

		if (_key == MGUI_KeyCode::Escape)
		{
			MGUI_InputManager::Instance()->SetKeyFocusedWidget(NULL);
		}
		
		else if (_key == MGUI_KeyCode::Backspace)
		{
			if (mSelectEndIndex == 0)
				return ;

			if (mSelectEndIndex > mSelectStartIndex)
			{
				mCaption.Remove(mSelectStartIndex, mSelectEndIndex - mSelectStartIndex);

				mSelectIndex = mSelectStartIndex;
				mSelectStartIndex = mSelectEndIndex = mSelectIndex;

				eventTextChanged(mCaption);
			}
			else
			{
				mCaption.Remove(mSelectIndex - 1, 1);
				mSelectIndex = mSelectIndex - 1;
				mSelectStartIndex = mSelectEndIndex = mSelectIndex;

				eventTextChanged(mCaption);
			}
		}

		else if (_key == MGUI_KeyCode::ArrowLeft)
		{
			if (mSelectEndIndex == 0)
				return ;

			if (mSelectEndIndex > mSelectStartIndex)
			{
				mSelectIndex = mSelectStartIndex;
				mSelectStartIndex = mSelectEndIndex = mSelectIndex;
			}
			else
			{
				mSelectIndex = mSelectIndex - 1;
				mSelectStartIndex = mSelectEndIndex = mSelectIndex;
			}
		}

		else if (_key == MGUI_KeyCode::ArrowRight)
		{
			if (mSelectEndIndex == 0)
				return ;

			if (mSelectEndIndex > mSelectStartIndex)
			{
				mSelectIndex = mSelectEndIndex;
				mSelectStartIndex = mSelectEndIndex = mSelectIndex;
			}
			else
			{
				mSelectIndex = Math::Minimum(mSelectIndex + 1, mCaption.Length());
				mSelectStartIndex = mSelectEndIndex = mSelectIndex;
			}
		}

		else if (_key == MGUI_KeyCode::NumpadEnter ||
			     _key == MGUI_KeyCode::Return)
		{
			eventTextReturn();
			MGUI_InputManager::Instance()->SetKeyFocusedWidget(NULL);
		}

		else if (_char != 0)
		{
			if (IKeyboard::Instance()->KeyPressed(KC_LCONTROL) || IKeyboard::Instance()->KeyPressed(KC_RCONTROL))
			{
				if (_key == MGUI_KeyCode::C)
				{
					MGUI_String text;

					for (int i = mSelectStartIndex; i < mSelectEndIndex; ++i)
					{
						text += mCaption[i];
					}

					if (text.Length() > 0)
						MGUI_Clipboard::Instance()->SetText(text);
				}
				else if (_key == MGUI_KeyCode::V)
				{
					MGUI_String text = MGUI_Clipboard::Instance()->GetText();

					if (text.Length() > 0)
					{
						if (mSelectEndIndex > mSelectStartIndex)
						{
							mCaption.Remove(mSelectStartIndex, mSelectEndIndex - mSelectStartIndex);

							mSelectIndex = mSelectStartIndex;
							mSelectStartIndex = mSelectEndIndex = mSelectIndex;
						}

						mCaption.Insert(mSelectIndex, text.c_str());

						mSelectIndex += text.Length();
						mSelectStartIndex = mSelectEndIndex = mSelectIndex;

						eventTextChanged(mCaption);
					}
				}
				else if (_key == MGUI_KeyCode::X)
				{
					MGUI_String text;

					for (int i = mSelectStartIndex; i < mSelectEndIndex; ++i)
					{
						text += mCaption[i];
					}

					if (mSelectEndIndex > mSelectStartIndex)
					{
						mCaption.Remove(mSelectStartIndex, mSelectEndIndex - mSelectStartIndex);

						mSelectIndex = mSelectStartIndex;
						mSelectStartIndex = mSelectEndIndex = mSelectIndex;

						eventTextChanged(mCaption);
					}

					if (text.Length() > 0)
						MGUI_Clipboard::Instance()->SetText(text);
				}
			}
			else
			{
				if (mSelectEndIndex > mSelectStartIndex)
				{
					mCaption.Remove(mSelectStartIndex, mSelectEndIndex - mSelectStartIndex);

					mSelectIndex = mSelectStartIndex;
					mSelectStartIndex = mSelectEndIndex = mSelectIndex;
				}

				mCaption.Insert(mSelectIndex, _char);
				
				mSelectIndex += 1;
				mSelectStartIndex = mSelectEndIndex = mSelectIndex;

				eventTextChanged(mCaption);
			}
		}
	}

	void MGUI_EditBox::OnTimer_(int _id)
	{
		if (_id == mTimerId)
		{
			mSelectVisible = !mSelectVisible;
		}
	}
}