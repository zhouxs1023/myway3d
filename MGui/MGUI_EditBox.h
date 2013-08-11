#pragma once

#include "MGUI_Widget.h"
#include "MGUI_Font.h"
#include "MGUI_String.h"

namespace Myway {

	class MGUI_ENTRY MGUI_EditBox : public MGUI_Widget
	{
		DeclareRTTI();

	public:
		tEvent1<const MGUI_String &> eventTextChanged;
		tEvent0 eventTextReturn;

	public:
		MGUI_EditBox(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent);
		virtual ~MGUI_EditBox();

		void SetCaption(const MGUI_String & caption) { mCaption = caption; }
		const MGUI_String & GetCaption() const { return mCaption; }

		void SetCharHeight(int _height) { mCharHeight = _height; }
		int GetCharHeight() const { return mCharHeight; }

		virtual void _AddRenderItem(MGUI_Layout * _layout);

		void SetSelectBackColor(const Color4 & _color) { mSelectBackColor = _color; }
		const Color4 & GetSelectBackColor() { return mSelectBackColor; }

		void SetStatic(bool _static);
		bool GetStatic() { return mStatic; }

	protected:
		virtual void OnMouseLostFocus(MGUI_Widget* _new);
		virtual void OnMouseSetFocus(MGUI_Widget* _old);
		virtual void OnMousePressed(int _x, int _y, MGUI_MouseButton _id);
		virtual void OnMouseDrag(int _x, int _y);

		virtual void OnKeyLostFocus(MGUI_Widget * _new);
		virtual void OnKeySetFocus(MGUI_Widget* _old);
		virtual void OnKeyPressed(MGUI_KeyCode _key, MGUI_Char _char);

	protected:
		bool mKeyFocused;

		MGUI_String mCaption;
		int mCharHeight;

		int mSelectIndex;
		int mSelectStartIndex;
		int mSelectEndIndex;

		bool mStatic;

		Color4 mSelectBackColor;
	};


}