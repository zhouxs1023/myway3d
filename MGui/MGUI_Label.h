#pragma once

#include "MGUI_Widget.h"
#include "MGUI_Font.h"
#include "MGUI_String.h"

namespace Myway {

	class MGUI_ENTRY MGUI_Label : public MGUI_Widget
	{
		DeclareRTTI();

	public:
		MGUI_Label(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent);
		virtual ~MGUI_Label();

		void SetCaption(const MGUI_String & caption) { mCaption = caption; }
		const MGUI_String & GetCaption() const { return mCaption; }

		void SetCharHeight(int _height) { mCharHeight = _height; }
		int GetCharDY() const { return mCharHeight; }

		virtual void _AddRenderItem(MGUI_Layout * _layout);

	protected:
		MGUI_String mCaption;
		int mCharHeight;
	};
}