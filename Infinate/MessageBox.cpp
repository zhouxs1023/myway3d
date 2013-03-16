#include "stdafx.h"

#include "MessageBox.h"

namespace Infinite {

	IMP_SLN(MMessageBox);

	MMessageBox::MMessageBox()
		: BaseLayout("MMessageBox.layout")
	{
		INIT_SLN;

		mWindow = mMainWidget->castType<MyGUI::Window>();

		assignWidget(mText, "Text");
		assignWidget(mOK, "Ok");
		assignWidget(mCancel, "Cancel");

		mOK->eventMouseButtonClick += MyGUI::newDelegate(this, &MMessageBox::OnOK);
		mCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &MMessageBox::OnCancel);
	}

	MMessageBox::~MMessageBox()
	{
		SHUT_SLN;
	}

	void MMessageBox::DoModal(const char * message, const char * caption)
	{
		mText->setCaption(message);
		mWindow->setCaption(caption);

		MyGUI::InputManager::getInstance().addWidgetModal(mMainWidget);
		MyGUI::LayerManager::getInstance().upLayerItem(mMainWidget);

		mMainWidget->setVisible(true);
	}

	void MMessageBox::EndDialog()
	{
		mMainWidget->setVisible(false);

		MyGUI::InputManager::getInstance().removeWidgetModal(mMainWidget);
	}

	void MMessageBox::OnOK(MyGUI::Widget *)
	{
		Event_OnOK();
		EndDialog();
	}

	void MMessageBox::OnCancel(MyGUI::Widget *)
	{
		Event_OnCancel();
		EndDialog();
	}
}