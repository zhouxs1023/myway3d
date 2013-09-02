#pragma once

#include "MGUI_Types.h"

namespace Myway
{

	class MGUI_ENTRY MGUI_Timer
	{
	public:
		MGUI_Timer();

		void Reset();
		unsigned long GetMilliseconds();
		unsigned long GetCurrentMilliseconds();

	private:
		unsigned long mTimeStart;
	};

	struct MGUI_ENTRY MGUI_Counter
	{
		int Id;
		unsigned long IntervalTime;
		unsigned long LastTime;
	};

}

