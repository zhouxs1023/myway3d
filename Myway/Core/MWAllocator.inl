
/* Block Alloc
------------------------------------------------------
------------------------------------------------------
*/
template <class type, int size>
inline Allocator<type, size>::Allocator()
: mTotal(0),
  mActive(0),
  mFree(NULL),
  mBlocks(NULL)
{
}

template <class type, int size>
inline Allocator<type, size>::~Allocator()
{
    Shutdown();
}

template <class type, int size>
inline type * Allocator<type, size>::Alloc()
{
    if (!mFree)
    {
        block * bl = new block();
        bl->next = mBlocks;
        mBlocks = bl;

        for (int i = 0; i < size; ++i)
        {
            bl->elements[i].next = mFree;
            mFree = &bl->elements[i];
        }

        mTotal += size;
    }

    ++mActive;
    element * e = mFree;
    mFree = mFree->next;
    e->next = NULL;
    return &e->mem;
}

template <class type, int size>
inline void Allocator<type, size>::Reset()
{
    block * bl = mBlocks;
    mFree = NULL;

    while (bl)
    {
        for (int i = 0; i < size; ++i)
        {
            bl->elements[i].next = mFree;
            mFree = &bl->elements[i];
        }

        bl = bl->next;
    }

    mActive = 0;
}

template <class type, int size>
inline void Allocator<type, size>::Free(type * mem)
{
    element * e = (element*)(((char*)mem) -  sizeof(element*));
    e->next = mFree;
    mFree = e;
    --mActive;
}

template <class type, int size>
inline void Allocator<type, size>::Shutdown()
{
    while (mBlocks)
    {
        block * bl = mBlocks;
        mBlocks = mBlocks->next;
        delete bl;
    }

    mBlocks = NULL;
    mFree = NULL;
    mActive = 0;
    mTotal = 0;
}

template <class type, int size>
inline int Allocator<type, size>::GetTotalCount() const
{
    return mTotal;
}

template <class type, int size>
inline int Allocator<type, size>::GetActiveCount() const
{
    return mActive;
}

template <class type, int size>
inline int Allocator<type, size>::GetFreeCount() const
{
    return mTotal - mActive;
}



/* Dynamic Alloc
------------------------------------------------------
------------------------------------------------------
*/
template <class type>
inline DynamicAlloc<type>::DynamicAlloc()
: mTotal(0),
  mAllocSize(0),
  mActive(0),
  mFree(0),
  mBlocks(0)
{
}

template <class type>
inline DynamicAlloc<type>::~DynamicAlloc()
{
    Shutdown();
}

template <class type>
inline void DynamicAlloc<type>::Initialize(int allocsize)
{
    mAllocSize = allocsize;
}

template <class type>
inline void DynamicAlloc<type>::Shutdown()
{
    while (mBlocks)
    {
        block * bl = mBlocks;
        delete[] bl->elements;
        mBlocks = mBlocks->next;
        delete bl;
    }

    mBlocks = NULL;
    mFree = NULL;
    mActive = 0;
    mTotal = 0;
}

template <class type>
inline type * DynamicAlloc<type>::Alloc()
{
    if (!mFree)
    {
        block * bl = new block();
        bl->elements = new element[mAllocSize];
        bl->next = mBlocks;
        mBlocks = bl;

        for (int i = 0; i < mAllocSize; ++i)
        {
            bl->elements[i].next = mFree;
            mFree = &bl->elements[i];
        }

        mTotal += size;
    }

    ++mActive;
    element * e = mFree;
    mFree = mFree->next;
    e->next = NULL;
    return &e->mem;
}

template <class type>
inline void DynamicAlloc<type>::Free(type * mem)
{
    element * e = (element*)(((char*)mem) -  sizeof(element*));
    e->next = mFree;
    mFree = e;
    --mActive;
}

template <class type>
inline int DynamicAlloc<type>::GetAllocSize() const
{
    return mAllocSize;
}

template <class type>
inline int DynamicAlloc<type>::GetTotalCount() const
{
    return mTotal;
}

template <class type>
inline int DynamicAlloc<type>::GetActiveCount() const
{
    return mActive;
}

template <class type>
inline int DynamicAlloc<type>::GetFreeCount() const
{
    return mTotal - mActive;
}