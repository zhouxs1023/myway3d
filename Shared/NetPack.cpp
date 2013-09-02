#include "StdAfx.h"
#include "NetPack.h"

compile_assert(sizeof(NetPacketHead) == 8);

PkOStream::PkOStream()
{
	mSize = 0;
}

PkOStream::~PkOStream()
{
}

void PkOStream::Write(const void * buffer, int size)
{
	d_assert (size + mSize <= PK_SIZE);

	memcpy(mBuffer + mSize, buffer, size);
	mSize += size;
}


PkIStream::PkIStream(const char * buffer, int size)
{
	d_assert (size <= PK_SIZE);

	memcpy(mBuffer, buffer, size);
	mSize = size;
	mPointer = 0;
}

PkIStream::~PkIStream()
{
}

void PkIStream::Read(void * buffer, int size)
{
	d_assert (mPointer + size <= mSize);

	memcpy(buffer, mBuffer + mPointer, size);

	mPointer += size;
}









NetPacketManager * NetPacketManager::msInstance = NULL;

NetPacketManager::NetPacketManager()
{
    msInstance = this;
}

NetPacketManager::~NetPacketManager()
{
    NetPacketMap::Iterator whr = mFactorys.Begin();
    NetPacketMap::Iterator end = mFactorys.End();

    while (whr != end)
    {
        delete whr->second;

        ++whr;
    }

	msInstance = NULL;
}

void NetPacketManager::AddPacket(NetPacket * factory)
{
    d_assert (GetPacket(factory->GetPacketId()) == NULL);

    mFactorys.Insert(factory->GetPacketId(), factory);
}

NetPacket * NetPacketManager::GetPacket(int id)
{
    NetPacketMap::Iterator whr = mFactorys.Find(id);
    NetPacketMap::Iterator end = mFactorys.End();

    return whr != end ? whr->second : NULL;
}