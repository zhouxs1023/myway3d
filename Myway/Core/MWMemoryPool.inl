

/* MemoryPool Alloc
------------------------------------------------------
------------------------------------------------------
*/
template <int size>
inline MemoryPoolAlloc<size>::MemoryPoolAlloc()
: mTotal(0),
  mActive(0),
  mFree(NULL),
  mElements(NULL),
  mMaxRequest(0)
{
}

template <int size>
inline MemoryPoolAlloc<size>::~MemoryPoolAlloc()
{
    Shutdown();
}

template <int size>
inline void * MemoryPoolAlloc<size>::Alloc()
{
    if (!mFree)
    {
        return NULL;
    }
    else
    {
        ++mActive;
        if (mActive > mMaxRequest)
            mMaxRequest = mActive;

#ifdef MW_DEBUG_MEMORY_POOL

        for (int i = 0; i < size; ++i)
        {
            assert(mFree->mem[i] == 0);
        }

#endif

        element * e = mFree;
        mFree = mFree->next;
        e->next = NULL;
        return &e->mem;
    }
}

template <int size>
inline void MemoryPoolAlloc<size>::Free(void * mem)
{
    element * e = (element*)((char*)mem -  sizeof(element*));

    assert(!e->next && e - mElements >= 0 && e - mElements < (int)mTotal);

#ifdef MW_DEBUG_MEMORY_POOL

    Memzero(e, sizeof(element));

#endif

    e->next = mFree;
    mFree = e;
    --mActive;
}

template <int size>
inline void MemoryPoolAlloc<size>::Initialize(int count)
{
    assert (!mElements);

    mElements = (element*)_alloc(sizeof(element) * count, MEMORY_ALIGN_NONE, "internal", 0);

#ifdef MW_DEBUG_MEMORY_POOL

    Memzero(mElements, sizeof(element) * count);

#endif

    for (int i = 0; i < count; ++i)
    {
        mElements[i].next = mFree;
        mFree = &mElements[i];
    }

    mTotal = count;
}

template <int size>
inline void MemoryPoolAlloc<size>::Shutdown()
{
    assert(mActive == 0);

#ifdef MW_DEBUG_MEMORY_POOL

    for (int i = 0; i < mTotal; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            assert(mElements[i].mem[j] == 0);
        }
    }

#endif

    if (mElements)
    {
        _free(mElements, "internal", 0);
        mElements = NULL;
    }

    mFree = NULL;
    mActive = 0;
    mTotal = 0;
}

template <int size>
inline int MemoryPoolAlloc<size>::GetTotalCount() const
{
    return mTotal;
}

template <int size>
inline int MemoryPoolAlloc<size>::GetActiveCount() const
{
    return mActive;
}

template <int size>
inline int MemoryPoolAlloc<size>::GetFreeCount() const
{
    return mTotal - mActive;
}

template <int size>
inline int MemoryPoolAlloc<size>::GetMaxRequest() const
{
    return mMaxRequest;
}