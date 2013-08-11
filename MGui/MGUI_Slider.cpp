#include "MGUI_Slider.h"

namespace Myway {

	MGUI_Slider::MGUI_Slider()
		: mMinValue(0)
		, mMaxValue(0)
		, mPosition(0)
	{
	}

	MGUI_Slider::~MGUI_Slider()
	{
	}

	void MGUI_Slider::SetRange(int _min, int _max)
	{
		d_assert (_min <= _max);
		mMinValue = _min;
		mMaxValue = _max;
		mPosition = Math::Clamp(mPosition, mMinValue, mMaxValue);
	}

	void MGUI_Slider::SetPosition(int position)
	{
		position = Math::Clamp(position, mMinValue, mMaxValue);

		if (mPosition != position)
		{
			mPosition = position;
			eventPositionChanged(mPosition);
		}
	}

}