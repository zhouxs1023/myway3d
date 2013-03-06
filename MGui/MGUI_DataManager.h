//////////////////////////////////////////////////////////////////////////
//
// MyGui Integration.
//   MyGui engine: http://mygui.info
//   
//	 author: Myway
//	 e-mail: Myway3D@Gmail.com
//
//   this soft is free, no license.
//
//
#pragma once

#include "MGUI_Entry.h"
#include "MyGUI_DataManager.h"

namespace Myway
{
	class MGUI_ENTRY MGUI_DataManager : public MyGUI::DataManager
	{
		DECLARE_ALLOC();

	public:
		MGUI_DataManager();

		void initialise();
		void shutdown();

		static MGUI_DataManager & getInstance()
		{
			return *getInstancePtr();
		}
		static MGUI_DataManager * getInstancePtr()
		{
			return static_cast<MGUI_DataManager *>(DataManager::getInstancePtr());
		}

		virtual MyGUI::IDataStream* getData(const std::string& _name);

		virtual bool isDataExist(const std::string& _name);

		virtual const MyGUI::VectorString& getDataListNames(const std::string& _pattern);

		virtual const std::string& getDataPath(const std::string& _name);

	private:
		bool mIsInitialise;
	};

}

