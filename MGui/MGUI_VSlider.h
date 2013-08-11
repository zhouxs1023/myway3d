#pragma once

#include "MGUI_Slider.h"

namespace Myway {

	class MGUI_ENTRY MGUI_VSlider : public MGUI_Widget, public MGUI_Slider
	{
		DeclareRTTI();

	public:
		MGUI_VSlider(const MGUI_LookFeel * _lookfeel, MGUI_Widget * _parent);
		virtual ~MGUI_VSlider();

		virtual void _AddRenderItem(MGUI_Layout * _layout);
		virtual void UpdateChildren();

	protected:
		virtual void OnMousePressed(int _x, int _y, MGUI_MouseButton _id);

		void OnSliderDrag_(int _x, int _y);
		void OnSliderPressed_(int _x, int _y, MGUI_MouseButton _id);
		void OnSliderReleased_(int _x, int _y, MGUI_MouseButton _id);

	protected:
		MGUI_Button * mBnSlider;

		int mMouseOldY;
		int mDY;
		int mDP;

		tListener2<MGUI_VSlider, int, int> OnSliderDrag;
		tListener3<MGUI_VSlider, int, int, MGUI_MouseButton> OnSliderPressed;
		tListener3<MGUI_VSlider, int, int, MGUI_MouseButton> OnSliderReleased;
	};

}