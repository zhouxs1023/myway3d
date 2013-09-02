#pragma once

#include "MGUI_Widget.h"
#include "MGUI_Button.h"

namespace Myway {

	class MGUI_ENTRY MGUI_Scroll
	{
	public:
		tEvent1<int> eventPositionChanged;

	public:
		MGUI_Scroll();
		~MGUI_Scroll();

		void SetRange(int _min, int _max);
		void SetPosition(int position);

		int GetMinValue() { return mMinValue; }
		int GetMaxValue() { return mMaxValue; }
		int GetPosition() { return mPosition; }

	protected:
		int mMinValue;
		int mMaxValue;
		int mPosition;
	};

	
	
}