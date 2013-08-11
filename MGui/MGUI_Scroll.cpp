#include "MGUI_Scroll.h"

namespace Myway {

	MGUI_Scroll::MGUI_Scroll()
		: mMinValue(0)
		, mMaxValue(0)
		, mPosition(0)
	{
	}

	MGUI_Scroll::~MGUI_Scroll()
	{
	}

	void MGUI_Scroll::SetRange(int _min, int _max)
	{
		d_assert (_min <= _max);
		mMinValue = _min;
		mMaxValue = _max;
		mPosition = Math::Clamp(mPosition, mMinValue, mMaxValue);
	}

	void MGUI_Scroll::SetPosition(int position)
	{
		position = Math::Clamp(position, mMinValue, mMaxValue);

		if (mPosition != position)
		{
			mPosition = position;
			eventPositionChanged(mPosition);
		}
	}

}