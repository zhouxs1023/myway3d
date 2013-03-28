#include "stdafx.h"

#include "PluginDialog.h"

#include "Editor.h"

namespace Infinite {

	PluginDialog::PluginDialog()
		: BaseLayout("Plugin.layout")
	{
		assignWidget(mList, "List");
		assignWidget(mOK, "Ok");
		assignWidget(mCancel, "Cancel");

		mOK->eventMouseButtonClick += MyGUI::newDelegate(this, &PluginDialog::OnOK);
		mCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &PluginDialog::OnCancel);
	}

	PluginDialog::~PluginDialog()
	{
	}

	iPlugin * PluginDialog::getPlugin()
	{
		size_t sel = mList->getIndexSelected();

		if (sel == MyGUI::ITEM_NONE)
			return NULL;

		return *mList->getItemDataAt<iPlugin *>(sel);
	}


	void PluginDialog::DoModal(int usages)
	{
		_update(usages);

		MyGUI::InputManager::getInstance().addWidgetModal(mMainWidget);
		MyGUI::LayerManager::getInstance().upLayerItem(mMainWidget);

		mMainWidget->setVisible(true);
	}

	void PluginDialog::EndDialog()
	{
		mMainWidget->setVisible(false);

		MyGUI::InputManager::getInstance().removeWidgetModal(mMainWidget);
	}

	void PluginDialog::OnOK(MyGUI::Widget *)
	{
		eventEndDialog(this, true);
		EndDialog();
	}

	void PluginDialog::OnCancel(MyGUI::Widget *)
	{
		eventEndDialog(this, false);
		EndDialog();
	}

	void PluginDialog::_update(int usages)
	{
		mList->removeAllItems();

		int index = 0;

		for (int i = 0; i < Editor::Instance()->GetPluginCount(); ++i)
		{
			iPlugin * plugin = Editor::Instance()->GetPlugin(i);

			if (!(plugin->GetUsages() | usages))
				continue ;

			mList->insertItemAt(index++, plugin->getName(), plugin);
		}
	}

}