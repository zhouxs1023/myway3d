#pragma once

#include "MGUI_Widget.h"

namespace Myway {

	class MGUI_ENTRY MGUI_ProgressBar : public MGUI_Widget
	{
		DeclareRTTI();

	public:
		MGUI_ProgressBar(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent);
		virtual ~MGUI_ProgressBar();

		void SetPercentage(int _percentage);

		virtual void OnUpdate();

	protected:
		int mPercentage;
		MGUI_Widget * mPercentageWidget;
	};

}