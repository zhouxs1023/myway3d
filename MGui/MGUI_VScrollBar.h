#pragma once

#include "MGUI_Scroll.h"

namespace Myway {

	// VScrollBar
	//
	class MGUI_ENTRY MGUI_VScrollBar : public MGUI_Widget, public MGUI_Scroll
	{
		DeclareRTTI();

	public:
		MGUI_VScrollBar(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent);
		virtual ~MGUI_VScrollBar();

		virtual void _AddRenderItem(MGUI_Layout * _layout);
		virtual void OnUpdate();

	protected:
		void OnScrollUpClick_();
		void OnScrollDownClick_();
		void OnScrollDrag_(int _x, int _y);
		void OnScrollPressed_(int _x, int _y, MGUI_MouseButton _id);
		void OnScrollReleased_(int _x, int _y, MGUI_MouseButton _id);

	protected:
		bool _IsScrollHiden();

	protected:
		MGUI_Button * mBnScrollUp;
		MGUI_Button * mBnScrollDown;
		MGUI_Button * mBnScroll;

		int mMouseOldY;

		int mDY;
		int mDP;

		tListener0<MGUI_VScrollBar> OnScrollUpClick;
		tListener0<MGUI_VScrollBar> OnScrollDownClick;
		tListener2<MGUI_VScrollBar, int, int> OnScrollDrag;
		tListener3<MGUI_VScrollBar, int, int, MGUI_MouseButton> OnScrollPressed;
		tListener3<MGUI_VScrollBar, int, int, MGUI_MouseButton> OnScrollReleased;
	};
}