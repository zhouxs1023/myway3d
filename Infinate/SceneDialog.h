#pragma once

#include "Common\\BaseLayout.h"

namespace Infinite {

	class SceneDialog : public wraps::BaseLayout
	{
	public:
		Event Event_OnOK;

	public:
		SceneDialog();
		~SceneDialog();

		void DoModel();

		const char * GetSceneFloder();
		const char * GetSceneName();

	protected:
		void OnInit();

		void OnFloderListSelChanged(MyGUI::ListBox* _sender, size_t _index);
		void OnSceneListSelChanged(MyGUI::ListBox* _sender, size_t _index);

		void EndDialog();

		void OnOK(MyGUI::Widget *);
		void OnCancel(MyGUI::Widget *);

	protected:
		MyGUI::ListBox * mFloderList;
		MyGUI::ListBox * mSceneList;
		MyGUI::EditBox * mSceneName;
		MyGUI::Button * mOK;
		MyGUI::Button * mCancel;
	};

} 