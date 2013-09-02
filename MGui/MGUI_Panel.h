#pragma once

#include "MGUI_HScrollBar.h"
#include "MGUI_VScrollBar.h"

namespace Myway {

	class MGUI_ENTRY MGUI_Panel : public MGUI_Widget
	{
		DeclareRTTI();

	public:
		MGUI_Panel(MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent);
		virtual ~MGUI_Panel();

		virtual void OnUpdate();

	protected:
		MGUI_VScrollBar * mVScrollBar;
		MGUI_HScrollBar * mHScrollBar;

		int mLogicX;
		int mLogicY;
	};
}