#include "MGUI_Types.h"

namespace Myway { 

	const TString128 MGUI_WidgetState::StateString[] = {
		"Normal",
		"Disabled",
		"Focused",
		"Pressed",
		"Selected"
	};

	int MGUI_WidgetState::GetState(const TString128 & _state)
	{
		for (int i = 0; i < Max; ++i)
		{
			if (_state == StateString[i])
				return i;
		}

		d_assert (0);

		return 0;
	}

	const TString128 & MGUI_WidgetState::GetStateString(int _state)
	{
		d_assert (_state < Max);

		return StateString[_state];
	}





	const TString128 MGUI_WidgetType::TypeString[MGUI_WidgetType::Max] = {

		"Button",
		"CheckBox",
		"ComboBox",
		"EditBox",
		"ImageBox",
		"Label",
		"ListBox",
		"Panel",
		"ProgressBar",
		"ScrollBar",
		"Slider",
		"Reserved0",
		"Reserved1",
		"Reserved2",
		"Reserved3",
		"Reserved4",
		"Reserved5",
		"User"
	};

	int MGUI_WidgetType::GetType(const TString128 & _type)
	{
		for (int i = 0; i < Max; ++i)
		{
			if (_type == TypeString[i])
				return i;
		}

		d_assert (0);

		return 0;
	}

	const TString128 & MGUI_WidgetType::GetTypeString(int _type)
	{
		d_assert (_type < Max);

		return TypeString[_type];
	}

}
