#pragma once

namespace UIEditor {

	class MainStatusBar
	{
	public:
		MainStatusBar();
		~MainStatusBar();

		void Layout();

	protected:
		MGUI_Layout * mLayout;
	};
}