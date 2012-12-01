#include "stdafx.h"
#include "xSerializer.h"

xSerializer::xSerializer(const char * filename, bool saving)
	: mIsSave(saving)
{
	if (saving)
		mFile.Open(filename, OM_WRITE_BINARY);
	else
		mFile.Open(filename, OM_READ_BINARY);
}

xSerializer::~xSerializer()
{
}

bool xSerializer::eof()
{
	return mFile.eof();
}

void xSerializer::Read(void * data, int size)
{
	d_assert(!IsSave());

	mFile.Read(data, size);
}

void xSerializer::Write(const void * data, int size)
{
	d_assert (IsSave());

	mFile.Write(data, size);
}
