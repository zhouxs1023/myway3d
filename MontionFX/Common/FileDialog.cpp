#include "stdafx.h"
#include "FileDialog.h"

#include "FileSystemInfo.h"

namespace common
{
	FileDialog::FileDialog()
		: BaseLayout("FileDialog.layout")
		, mWindow(nullptr)
		, mListFiles(nullptr)
		, mEditFileName(nullptr)
		, mButtonUp(nullptr)
		, mCurrentFolderField(nullptr)
		, mFolderMode(false)
		, mFileMask("*.*")
	{
		assignWidget(mListFiles, "ListFiles");
		assignWidget(mEditFileName, "EditFileName");
		assignWidget(mButtonUp, "ButtonUp");
		assignWidget(mCurrentFolderField, "CurrentFolder");
		assignWidget(mOK, "OK");
		assignWidget(mCancel, "Cancel");
		assignWidget(mListExts, "ExternNameList");

		mListFiles->eventListChangePosition += MyGUI::newDelegate(this, &FileDialog::notifyListChangePosition);
		mListFiles->eventListSelectAccept += MyGUI::newDelegate(this, &FileDialog::notifyListSelectAccept);
		mEditFileName->eventEditSelectAccept += MyGUI::newDelegate(this, &FileDialog::notifyEditSelectAccept);
		mButtonUp->eventMouseButtonClick += MyGUI::newDelegate(this, &FileDialog::notifyUpButtonClick);
		mCurrentFolderField->eventComboAccept += MyGUI::newDelegate(this, &FileDialog::notifyDirectoryComboAccept);
		mCurrentFolderField->eventComboChangePosition += MyGUI::newDelegate(this, &FileDialog::notifyDirectoryComboChangePosition);
		mOK->eventMouseButtonClick += MyGUI::newDelegate(this, &FileDialog::notifyOK_Click);
		mCancel->eventMouseButtonClick += MyGUI::newDelegate(this, &FileDialog::notifyCancel_Click);

		mListExts->eventComboChangePosition += MyGUI::newDelegate(this, &FileDialog::notifyListExt_SelChanged);

		mWindow = mMainWidget->castType<MyGUI::Window>();
		mWindow->eventWindowButtonPressed += MyGUI::newDelegate(this, &FileDialog::notifyWindowButtonPressed);

		mCurrentFolder = common::getSystemCurrentFolder();

		mMainWidget->setVisible(false);

		update();
	}

	void FileDialog::notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name)
	{
		if (_name == "close")
		{
			eventEndDialog(this, false);
			onEndModal();
		}
	}

	void FileDialog::notifyEditSelectAccept(MyGUI::EditBox* _sender)
	{
		accept();
	}

	void FileDialog::notifyOK_Click(MyGUI::Widget* _sender)
	{
		accept();
	}

	void FileDialog::notifyCancel_Click(MyGUI::Widget* _sender)
	{
		eventEndDialog(this, false);
		onEndModal();
	}

	void FileDialog::notifyListExt_SelChanged(MyGUI::ComboBox * _sender, size_t _index)
	{
		 mFileMask = "*" + _sender->getItemNameAt(_index);
		 update();
	}


	void FileDialog::notifyUpButtonClick(MyGUI::Widget* _sender)
	{
		upFolder();
	}

	void FileDialog::setDialogInfo(const MyGUI::UString& _caption, bool _folderMode)
	{
		mFolderMode = _folderMode;
		mWindow->setCaption(_caption);
		mEditFileName->setVisible(!_folderMode);
	}

	void FileDialog::notifyListChangePosition(MyGUI::ListBox* _sender, size_t _index)
	{
		if (_index == MyGUI::ITEM_NONE)
		{
			mEditFileName->setCaption("");
		}
		else
		{
			common::FileInfo info = *_sender->getItemDataAt<common::FileInfo>(_index);
			if (!info.folder)
				mEditFileName->setCaption(info.name);
		}
	}

	void FileDialog::notifyListSelectAccept(MyGUI::ListBox* _sender, size_t _index)
	{
		if (_index == MyGUI::ITEM_NONE) return;

		common::FileInfo info = *_sender->getItemDataAt<common::FileInfo>(_index);
		if (info.folder)
		{
			if (info.name == L"..")
			{
				upFolder();
			}
			else
			{
				mCurrentFolder = common::concatenatePath (mCurrentFolder.asWStr(), info.name);
				update();
			}
		}
		else
		{
			accept();
		}
	}

	void FileDialog::accept()
	{
		if (!mFolderMode)
		{
			mFileName = mEditFileName->getOnlyText();
			if (!mFileName.empty())
				eventEndDialog(this, true);
		}
		else
		{
			if (mListFiles->getIndexSelected() != MyGUI::ITEM_NONE)
			{
				common::FileInfo info = *mListFiles->getItemDataAt<common::FileInfo>(mListFiles->getIndexSelected());
				if (!common::isParentDir(info.name.c_str()))
					mCurrentFolder = common::concatenatePath (mCurrentFolder.asWStr(), info.name);
			}
			eventEndDialog(this, true);
		}

		onEndModal();
	}

	void FileDialog::upFolder()
	{
		size_t index = mCurrentFolder.find_last_of(L"\\/");
		if (index != std::string::npos)
		{
			mCurrentFolder = mCurrentFolder.substr(0, index);
		}
		update();
	}

	void FileDialog::setCurrentFolder(const MyGUI::UString& _folder)
	{
		mCurrentFolder = _folder.empty() ? MyGUI::UString(common::getSystemCurrentFolder()) : _folder;

		update();
	}

	void FileDialog::update()
	{
		if (mCurrentFolder.empty())
			mCurrentFolder = "/";
		mCurrentFolderField->setCaption(mCurrentFolder);

		mListFiles->removeAllItems();

		// add all folders first
		common::VectorFileInfo infos;
		getSystemFileList(infos, mCurrentFolder, L"*.*");

		for (common::VectorFileInfo::iterator item = infos.begin(); item != infos.end(); ++item)
		{
			if ((*item).folder)
				mListFiles->addItem(L"[" + (*item).name + L"]", *item);
		}

		if (!mFolderMode && mFileMask != "")
		{
			// add files by given mask
			infos.clear();
			getSystemFileList(infos, mCurrentFolder, mFileMask);

			for (common::VectorFileInfo::iterator item = infos.begin(); item != infos.end(); ++item)
			{
				if (!(*item).folder)
					mListFiles->addItem((*item).name, *item);
			}
		}
	}

	void FileDialog::setFileName(const MyGUI::UString& _value)
	{
		mFileName = _value;
		mEditFileName->setCaption(_value);
	}

	void FileDialog::addFileExt(const MyGUI::UString & _ext)
	{
		mFileExt.PushBack(_ext);
	}

	void FileDialog::clearFileExt()
	{
		mFileExt.Clear();
	}

	int FileDialog::getFileExtCount() const
	{
		return mFileExt.Size();
	}

	const MyGUI::UString& FileDialog::getFileExt(int index) const
	{
		return mFileExt[index];
	}

	const MyGUI::UString& FileDialog::getFileMask() const
	{
		return mFileMask;
	}

	void FileDialog::DoModal()
	{
		MyGUI::InputManager::getInstance().addWidgetModal(mMainWidget);
		MyGUI::LayerManager::getInstance().upLayerItem(mMainWidget);

		onDoModal();

		mMainWidget->setVisible(true);
	}

	void FileDialog::onDoModal()
	{
		mListExts->removeAllItems();

		for (int i = 0; i < mFileExt.Size(); ++i)
		{
			mListExts->insertItemAt(i, mFileExt[i]);
		}

		if (mFileExt.Size() > 0)
		{
			mListExts->setIndexSelected(0);
			notifyListExt_SelChanged(mListExts, 0);
		}
		else
		{
			mFileMask = "";
		}

		update();

		MyGUI::IntSize windowSize = mMainWidget->getSize();
		MyGUI::IntSize parentSize = mMainWidget->getParentSize();

		mMainWidget->setPosition((parentSize.width - windowSize.width) / 2, (parentSize.height - windowSize.height) / 2);
	}

	void FileDialog::onEndModal()
	{
		mMainWidget->setVisible(false);

		MyGUI::InputManager::getInstance().removeWidgetModal(mMainWidget);
	}

	void FileDialog::notifyDirectoryComboAccept(MyGUI::ComboBox* _sender, size_t _index)
	{
		setCurrentFolder(_sender->getOnlyText());
	}

	const MyGUI::UString& FileDialog::getCurrentFolder() const
	{
		return mCurrentFolder;
	}

	const MyGUI::UString& FileDialog::getFileName() const
	{
		return mFileName;
	}

	const MyGUI::UString& FileDialog::getMode() const
	{
		return mMode;
	}

	void FileDialog::setMode(const MyGUI::UString& _value)
	{
		mMode = _value;
	}

	void FileDialog::setRecentFolders(const VectorUString& _listFolders)
	{
		mCurrentFolderField->removeAllItems();

		for (VectorUString::const_iterator item = _listFolders.begin(); item != _listFolders.end(); ++ item)
			mCurrentFolderField->addItem((*item));
	}

	void FileDialog::notifyDirectoryComboChangePosition(MyGUI::ComboBox* _sender, size_t _index)
	{
		if (_index != MyGUI::ITEM_NONE)
			setCurrentFolder(_sender->getItemNameAt(_index));
	}

} // namespace tools
