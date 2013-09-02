#include "MGUI_ProgressBar.h"

namespace Myway {

	ImplementRTTI(MGUI_ProgressBar, MGUI_Widget);

	MGUI_ProgressBar::MGUI_ProgressBar(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent)
		: MGUI_Widget(_lookfeel, _parent)
		, mPercentage(0)
	{
		mPercentageWidget = new MGUI_Widget(_lookfeel->GetChild("Track"), this);
	}

	MGUI_ProgressBar::~MGUI_ProgressBar()
	{
	}

	void MGUI_ProgressBar::SetPercentage(int _percentage)
	{
		mPercentage = _percentage;
		mPercentage = Math::Clamp(mPercentage, 0, 100);
	}

	void MGUI_ProgressBar::OnUpdate()
	{
		MGUI_Rect myRect;

		myRect.x0 = 0;
		myRect.y0 = 0;
		myRect.x1 = mPercentage * mClientRect.DX() / 100;
		myRect.y1 = mClientRect.DY();

		mPercentageWidget->SetRect(myRect);
	}
}