#pragma once

#include "BaseLayout.h"
#include "PropertyGrid.h"

namespace Infinite {

	class EnvironmentPane : public wraps::BaseLayout
	{
	public:
		EnvironmentPane(MyGUI::Widget * _parent);
		~EnvironmentPane();

	protected:
		void OnTabChangeSelect(MyGUI::TabControl* _sender, size_t _index);

		void _OnEnvChanged(Event * _sender);

	protected:
		tEventListener<EnvironmentPane> OnEnvChanged;

		PropertyGrid * mNight;
		PropertyGrid * mMorning;
		PropertyGrid * mNoon;
		PropertyGrid * mEvening;
		PropertyGrid * mGlobal;

		MyGUI::TabControl * mTab;
	};

}