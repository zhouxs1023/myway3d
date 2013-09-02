#pragma once

#include "MGUI_Widget.h"
#include "MGUI_Font.h"
#include "MGUI_String.h"

namespace Myway {

	class MGUI_ENTRY MGUI_Button : public MGUI_Widget
	{
		DeclareRTTI();

	public:
		MGUI_Button(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent);
		virtual ~MGUI_Button();

		void SetCaption(const MGUI_String & caption) { mCaption = caption; mCaptionChanged = true; }
		const MGUI_String & GetCaption() const { return mCaption; }

		void SetCharHeight(int _height) { mCharHeight = _height; }
		int GetCharDY() const { return mCharHeight; }

		virtual void _AddRenderItem(MGUI_Layout * _layout);

	protected:
		virtual void OnMousePressed(int _x, int _y, MGUI_MouseButton _id);
		virtual void OnMouseReleased(int _x, int _y, MGUI_MouseButton _id);

	protected:
		MGUI_String mCaption;
		int mCharHeight;

		bool mCaptionChanged;
		Array<const MGUI_Glyph *> mCaptionGlyphs;
		float mCaptionWidth;

		bool mMousePressed;
	};

}