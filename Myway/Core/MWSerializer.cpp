#include "MWSerializer.h"

namespace Myway {

	FileSerializer::FileSerializer(const char * filename, bool saving)
		: mIsSave(saving)
	{
		if (saving)
			mFile.Open(filename, OM_WRITE_BINARY);
		else
			mFile.Open(filename, OM_READ_BINARY);
	}

	FileSerializer::~FileSerializer()
	{
	}

	bool FileSerializer::IsOpen()
	{
		return mFile.IsOpen();
	}

	bool FileSerializer::eof()
	{
		return mFile.eof();
	}

	void FileSerializer::Read(void * data, int size)
	{
		d_assert(!IsSave());

		mFile.Read(data, size);
	}

	void FileSerializer::Write(const void * data, int size)
	{
		d_assert (IsSave());

		mFile.Write(data, size);
	}

}