#include "MWMemoryPool.h"

using namespace Myway;

MemoryPool::MemoryPool()
{
}

MemoryPool::~MemoryPool()
{
    Shutdown();
}


void MemoryPool::Initialize(int _16ByteCount,
                            int _32ByteCount,
                            int _64ByteCount,
                            int _128ByteCount,
                            int _256ByteCount)
{
    mAlloc16.Initialize(_16ByteCount);
    mAlloc32.Initialize(_32ByteCount);
    mAlloc64.Initialize(_64ByteCount);
    mAlloc128.Initialize(_128ByteCount);
    mAlloc256.Initialize(_256ByteCount);
}


void MemoryPool::Shutdown()
{
    mAlloc16.Shutdown();
    mAlloc32.Shutdown();
    mAlloc64.Shutdown();
    mAlloc128.Shutdown();
    mAlloc256.Shutdown();
}


void * MemoryPool::Alloc(int iSize, short & flag)
{
    void * mem = NULL;

    if (iSize <= 16 && !mem)
    {   
        mem = mAlloc16.Alloc();
        flag = MEMORY_IN_POOL16;
    }

    if (iSize <= 32 && !mem)
    {
        mem = mAlloc32.Alloc();
        flag = MEMORY_IN_POOL32;
    }

    if (iSize <= 64 && !mem)
    {
        mem = mAlloc64.Alloc();
        flag = MEMORY_IN_POOL64;
    }

    if (iSize <= 128 && !mem)
    {
        mem = mAlloc128.Alloc();
        flag = MEMORY_IN_POOL128;
    }

    if (iSize <= 256 && !mem)
    {
        mem = mAlloc256.Alloc();
        flag = MEMORY_IN_POOL256;
    }

    return mem;
}

void MemoryPool::Free(void * mem, short flag)
{
    switch (flag)
    {
    case MEMORY_IN_POOL16:
        mAlloc16.Free(mem);
        break;

    case MEMORY_IN_POOL32:
        mAlloc32.Free(mem);
        break;

    case MEMORY_IN_POOL64:
        mAlloc64.Free(mem);
        break;

    case MEMORY_IN_POOL128:
        mAlloc128.Free(mem);
        break;

    case MEMORY_IN_POOL256:
        mAlloc256.Free(mem);
        break;

    default:
        assert(0 && "Bad Memory.");
        break;
    }
}