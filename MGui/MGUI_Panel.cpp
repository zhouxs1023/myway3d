#include "MGUI_Panel.h"

namespace Myway {

	ImplementRTTI(MGUI_Panel, MGUI_Widget);

	MGUI_Panel::MGUI_Panel(MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent)
		: MGUI_Widget(_lookfeel, _parent)
	{
		mVScrollBar = new MGUI_VScrollBar(_lookfeel->GetChild("VScrollBar"), this);
		mHScrollBar = new MGUI_HScrollBar(_lookfeel->GetChild("HScrollBar"), this);
	}

	MGUI_Panel::~MGUI_Panel()
	{
	}

	void MGUI_Panel::OnUpdate()
	{
		MGUI_Rect clRect = GetClientRect();
		MGUI_Rect myRect = clRect;

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			const MGUI_Rect & rcRect = mChildren[i]->GetRect();

			if (myRect.x0 > rcRect.x0)
				myRect.x0 = rcRect.x0;

			if (myRect.y0 < rcRect.y0)
				myRect.y0 = rcRect.y0;

			if (myRect.x1 < rcRect.x1)
				myRect.x1 = rcRect.x1;

			if (myRect.y1 < rcRect.y1)
				myRect.y1 = rcRect.y1;
		}

		mLogicX = Math::Clamp(mLogicX, myRect.x0, myRect.x1);
		mLogicY = Math::Clamp(mLogicY, myRect.y0, myRect.y1);

		bool needVScroll = myRect.DY() > clRect.DY();
		bool needHScroll = myRect.DX() > clRect.DX();

		if (!needHScroll && !needVScroll)
		{
			mLogicX = 0;
			mLogicY = 0;
			mHScrollBar->SetVisible(false);
			mVScrollBar->SetVisible(false);
		}
		else if (needVScroll && !needHScroll)
		{
			mHScrollBar->SetVisible(false);
			mVScrollBar->SetAlign(MGUI_Align::Right | MGUI_Align::VStretch);
		}
		else if (needHScroll && !needVScroll)
		{
			mVScrollBar->SetVisible(false);
			mHScrollBar->SetAlign(MGUI_Align::Bottom | MGUI_Align::HStretch);
		}
		else
		{
			mVScrollBar->SetAlign(MGUI_Align::Right | MGUI_Align::VStretch);
			mHScrollBar->SetAlign(MGUI_Align::Bottom);

			mHScrollBar->SetRect(MGUI_Rect(0, 0, 
				mClientRect.DX() - mVScrollBar->GetRect().DX(),
				mHScrollBar->GetRect().DY()));
		}
	}

}