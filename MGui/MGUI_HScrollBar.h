#pragma once

#include "MGUI_Scroll.h"

namespace Myway {

	class MGUI_ENTRY MGUI_HScrollBar : public MGUI_Widget, public MGUI_Scroll
	{
		DeclareRTTI();

	public:
		MGUI_HScrollBar(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent);
		virtual ~MGUI_HScrollBar();

		virtual void _AddRenderItem(MGUI_Layout * _layout);
		virtual void OnUpdate();

	protected:
		void OnScrollLeftClick_();
		void OnScrollRightClick_();
		void OnScrollDrag_(int _x, int _y);
		void OnScrollPressed_(int _x, int _y, MGUI_MouseButton _id);
		void OnScrollReleased_(int _x, int _y, MGUI_MouseButton _id);

	protected:
		bool _IsScrollHiden();

	protected:
		MGUI_Button * mBnScrollLeft;
		MGUI_Button * mBnScrollRight;
		MGUI_Button * mBnScroll;

		int mMouseOldX;
		int mDX;
		int mDP;

		tListener0<MGUI_HScrollBar> OnScrollLeftClick;
		tListener0<MGUI_HScrollBar> OnScrollRightClick;
		tListener2<MGUI_HScrollBar, int, int> OnScrollDrag;
		tListener3<MGUI_HScrollBar, int, int, MGUI_MouseButton> OnScrollPressed;
		tListener3<MGUI_HScrollBar, int, int, MGUI_MouseButton> OnScrollReleased;
	};


}