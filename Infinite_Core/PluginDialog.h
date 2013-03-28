#pragma once

#include "Common\\BaseLayout.h"

namespace Infinite {

	class iPlugin;

	class PluginDialog : public wraps::BaseLayout
	{
	public:
		typedef MyGUI::delegates::CDelegate2<PluginDialog *, bool> EventHandle_Result;

		EventHandle_Result eventEndDialog;

	public:
		PluginDialog();
		virtual ~PluginDialog();

		iPlugin * getPlugin();

		void DoModal(int usages);

	protected:
		void EndDialog();

		void OnOK(MyGUI::Widget *);
		void OnCancel(MyGUI::Widget *);

		void _update(int usages);

	protected:
		MyGUI::ListBox * mList;
		MyGUI::Button * mOK;
		MyGUI::Button * mCancel;
	};
}