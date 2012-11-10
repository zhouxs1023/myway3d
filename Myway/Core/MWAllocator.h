#pragma once

#include "MWAllocObj.h"

namespace Myway
{

/* Block Alloc
------------------------------------------------------
------------------------------------------------------
*/
template <class type, int size>
class Allocator : public AllocObj
{
public:
    Allocator();
    ~Allocator();

    void    Shutdown();

    void    Reset();

    type *  Alloc();
    void    Free(type * mem);

    int   GetTotalCount() const;
    int   GetActiveCount() const;
    int   GetFreeCount() const;

protected:
    struct element : public PoolAllocObj
    {
        element *   next;
        type        mem;
    };

    struct block : public PoolAllocObj
    {
        block *     next;
        element     elements[size];
    };

    int         mTotal;
    int         mActive;
    element *   mFree;
    block *     mBlocks;
};

   

/* Dynamic Alloc
------------------------------------------------------
------------------------------------------------------
*/
template <class type>
class DynamicAlloc : public AllocObj
{
public:
    DynamicAlloc();
    ~DynamicAlloc();

    void    Initialize(int allocsize);
    void    Shutdown();

    type *  Alloc();
    void    Free(type * mem);

    int   GetAllocSize() const;
    int   GetTotalCount() const;
    int   GetActiveCount() const;
    int   GetFreeCount() const;

protected:
    struct element : public PoolAllocObj
    {
        element *   next;
        type        mem;
    };

    struct block : public PoolAllocObj
    {
        block *     next;
        element *   elements;
    };

    int         mTotal;
    int         mAllocSize;
    int         mActive;
    element *   mFree;
    block *     mBlocks;
};

#include "MWAllocator.inl"

}