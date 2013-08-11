#pragma once

#include "MGUI_Types.h"
#include "MGUI_String.h"

namespace Myway {

	class MGUI_Clipboard
	{
		DECLARE_SINGLETON(MGUI_Clipboard);

	public:
		MGUI_Clipboard();
		~MGUI_Clipboard();

		void SetText(const MGUI_String & _str);
		MGUI_String GetText();
	};

}