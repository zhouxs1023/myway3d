#pragma once

#include "MWDebug.h"
#include "MWMemory.h"

namespace Myway
{

template <int size>
class MemoryPoolAlloc
{
public:
    MemoryPoolAlloc();
    ~MemoryPoolAlloc();

    void    Initialize(int count);
    void    Shutdown();

    void *  Alloc();
    void    Free(void * mem);

    int  GetTotalCount() const;
    int  GetActiveCount() const;
    int  GetFreeCount() const;
    int  GetMaxRequest() const;

protected:
    struct element
    {
        element *   next;
        char        mem[size];
    };

    int mTotal;
    int mActive;
    int mMaxRequest;
    element * mFree;
    element * mElements;
};

class MemoryPool
{
public:
    MemoryPool();
    ~MemoryPool();

    void Initialize(int _16ByteCount,
                    int _32ByteCount,
                    int _64ByteCount,
                    int _128ByteCount,
                    int _256ByteCount);
    void Shutdown();

    void * Alloc(int iSize, short & flag);
    void Free(void * mem, short flag);

    friend class Memory;

protected:
    MemoryPoolAlloc<16>     mAlloc16;
    MemoryPoolAlloc<32>     mAlloc32;
    MemoryPoolAlloc<64>     mAlloc64;
    MemoryPoolAlloc<128>    mAlloc128;
    MemoryPoolAlloc<256>    mAlloc256;
};

#include "MWMemoryPool.inl"

}