#include "stdafx.h"
#include "GameSerializer.h"

namespace game {

	GSerializer::GSerializer(const char * filename, bool saving)
		: mIsSave(saving)
	{
		if (saving)
			mFile.Open(filename, OM_WRITE_BINARY);
		else
			mFile.Open(filename, OM_READ_BINARY);
	}

	GSerializer::~GSerializer()
	{
	}

	bool GSerializer::IsOpen()
	{
		return mFile.IsOpen();
	}

	bool GSerializer::eof()
	{
		return mFile.eof();
	}

	void GSerializer::Read(void * data, int size)
	{
		d_assert(!IsSave());

		mFile.Read(data, size);
	}

	void GSerializer::Write(const void * data, int size)
	{
		d_assert (IsSave());

		mFile.Write(data, size);
	}

}