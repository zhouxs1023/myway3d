#pragma once

#include "BaseLayout.h"

namespace Infinite {

	class MMessageBox : public wraps::BaseLayout
	{
		DECLARE_SINGLETON(MMessageBox);

	public:
		Event Event_OnOK;
		Event Event_OnCancel;

	public:
		MMessageBox();
		virtual ~MMessageBox();

		void DoModal(const char * message, const char * caption = "MessageBox");

	protected:
		void EndDialog();

		void OnOK(MyGUI::Widget *);
		void OnCancel(MyGUI::Widget *);


	protected:
		MyGUI::Window * mWindow;
		MyGUI::TextBox * mText;
		MyGUI::Button * mOK;
		MyGUI::Button * mCancel;
	};

}