#include "MGUI_DataManager.h"
#include "MyGUI_UString.h"

namespace Myway {

	class MGUI_DataStream : public MyGUI::IDataStream
	{
	public:
		MGUI_DataStream(DataStreamPtr stream) { mStream = stream; }
		~MGUI_DataStream() {}

		virtual bool eof()
		{
			return mStream->Eof();
		}

		virtual size_t size()
		{
			return mStream->Size();
		}

		virtual void readline(std::string& _source, MyGUI::Char _delim)
		{
			char buffer[2048] = { 0 };

			mStream->ReadLine(buffer, 2048);

			_source = buffer;
		}

		virtual size_t read(void* _buf, size_t _count)
		{
			return mStream->Read(_buf, _count);
		}

	protected:
		DataStreamPtr mStream;
	};



	MGUI_DataManager::MGUI_DataManager() : mIsInitialise(false)
	{
	}

	void MGUI_DataManager::initialise()
	{
		d_assert(!mIsInitialise);

		mIsInitialise = true;
	}

	void MGUI_DataManager::shutdown()
	{
		d_assert(mIsInitialise);

		mIsInitialise = false;
	}

	MyGUI::IDataStream* MGUI_DataManager::getData(const std::string& _name)
	{
		std::string filepath = getDataPath(_name);
		if (filepath.empty())
			return nullptr;

		DataStreamPtr dstream = ResourceManager::Instance()->OpenResource(_name.c_str());

		d_assert (dstream != NULL);

		return new MGUI_DataStream(dstream);
	}

	bool MGUI_DataManager::isDataExist(const std::string& _name)
	{
		return ResourceManager::Instance()->Exist(_name.c_str());
	}

	const MyGUI::VectorString& MGUI_DataManager::getDataListNames(const std::string& _pattern)
	{
		static MyGUI::VectorString result;
		result.clear();

		Archive::FileInfoList list;
		ResourceManager::Instance()->GetFileInfosByKey(list, _pattern.c_str());

		Archive::FileInfoList::Iterator whr = list.Begin();
		Archive::FileInfoList::Iterator end = list.End();

		while (whr != end)
		{
			result.push_back(MyGUI::UString((*whr).name.c_str()).asUTF8());

			++whr;
		}

		return result;
	}

	const std::string& MGUI_DataManager::getDataPath(const std::string& _name)
	{
		static std::string result;

		const Archive::FileInfo * file = ResourceManager::Instance()->GetFileInfo(_name.c_str());

		result = (file != NULL) ? file->archive->GetName().c_str() : "";

		return result;
	}

}