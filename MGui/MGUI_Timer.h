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

	private:
		unsigned long GetCurrentMilliseconds();

	private:
		unsigned long mTimeStart;
	};

}

