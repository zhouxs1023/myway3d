#ifndef __OPEN_SAVE_FILE_DIALOG_H__
#define __OPEN_SAVE_FILE_DIALOG_H__

#include "BaseLayout.h"

namespace Infinite
{
	class FileDialog :
		public wraps::BaseLayout
	{
	public:
		typedef MyGUI::delegates::CDelegate2<FileDialog *, bool> EventHandle_Result;

		EventHandle_Result eventEndDialog;

	public:
		FileDialog();

		void setDialogInfo(const MyGUI::UString& _caption, bool _folderMode = false);

		void setCurrentFolder(const MyGUI::UString& _value);
		const MyGUI::UString& getCurrentFolder() const;

		void setFileName(const MyGUI::UString& _value);
		const MyGUI::UString& getFileName() const;

		const MyGUI::UString& getMode() const;
		void setMode(const MyGUI::UString& _value);

		typedef std::vector<MyGUI::UString> VectorUString;
		void setRecentFolders(const VectorUString& _listFolders);

		void addFileExt(const MyGUI::UString & _ext);
		void clearFileExt();
		int getFileExtCount() const;
		const MyGUI::UString& getFileExt(int index) const;
		const MyGUI::UString& getFileMask() const;

		void DoModal();

	protected:
		virtual void onDoModal();
		virtual void onEndModal();

	private:
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name);
		void notifyDirectoryComboAccept(MyGUI::ComboBox* _sender, size_t _index);
		void notifyDirectoryComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);
		void notifyListChangePosition(MyGUI::ListBox* _sender, size_t _index);
		void notifyListSelectAccept(MyGUI::ListBox* _sender, size_t _index);
		void notifyEditSelectAccept(MyGUI::EditBox* _sender);
		void notifyUpButtonClick(MyGUI::Widget* _sender);

		void notifyOK_Click(MyGUI::Widget* _sender);
		void notifyCancel_Click(MyGUI::Widget* _sender);
		void notifyListExt_SelChanged(MyGUI::ComboBox * _sender, size_t _index);

		void update();
		void accept();

		void upFolder();

	private:
		MyGUI::Window* mWindow;
		MyGUI::ListBox* mListFiles;
		MyGUI::EditBox* mEditFileName;
		MyGUI::Button* mButtonUp;
		MyGUI::ComboBox* mCurrentFolderField;
		MyGUI::Button* mOK;
		MyGUI::Button* mCancel;
		MyGUI::ComboBox* mListExts;


		MyGUI::UString mCurrentFolder;
		MyGUI::UString mFileName;

		MyGUI::UString mFileMask;

		Array<MyGUI::UString> mFileExt;

		MyGUI::UString mMode;
		bool mFolderMode;
	};

} // namespace tools

#endif // __OPEN_SAVE_FILE_DIALOG_H__
