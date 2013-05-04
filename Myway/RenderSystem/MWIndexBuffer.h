#pragma once

#include "MWRenderDefine.h"

namespace Myway
{

class MW_ENTRY IndexBuffer : public RefObj
{
public:
    IndexBuffer() {}
    virtual ~IndexBuffer() {}

    virtual void * Lock(int iOffsetBytes, int iLockSize, int LockFlags) = 0;
    virtual void Unlock() = 0;

    int GetSize() const { return mSize; }
    USAGE GetUsage() const { return mUsage; }
    bool GetIndex16() const { return mIndex16; }

protected:
    int mSize;
    USAGE mUsage;
	bool mIndex16;
};

DeclareSmartPtr(IndexBuffer);

}