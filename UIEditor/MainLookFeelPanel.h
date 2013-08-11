#pragma once

namespace UIEditor {

	class MainLookFeelPanel
	{
		static const int K_LayoutId = 'MLFP';

	public:
		MainLookFeelPanel();
		~MainLookFeelPanel();

		void Layout();

	protected:
		MGUI_Layout * mLayout;

	};

}