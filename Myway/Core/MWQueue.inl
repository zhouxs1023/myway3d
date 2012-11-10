

template <class T>
inline Queue<T>::Queue()
: mFront(NULL),
  mBack(NULL)
{
}

template <class T>
inline Queue<T>::~Queue()
{
    Clear();
}

template <class T>
inline T & Queue<T>::Front()
{
    debug_assert(Size(), "Queue is empty.");
    return mFront->mem;
}

template <class T>
inline const T & Queue<T>::Front() const
{
    debug_assert(Size(), "Queue is empty.");
    return mFront->mem;
}

template <class T>
inline T & Queue<T>::Back()
{
    debug_assert(Size(), "Queue is empty.");
    return mBack->mem;
}

template <class T>
inline const T & Queue<T>::Back() const
{
    debug_assert(Size(), "Queue is empty.");
    return mBack->mem;
}

template <class T>
inline void Queue<T>::Push(const T & Val)
{
   if (Size())
   {
       _member * new_mem = mAlloc.Alloc();
       new_mem->mem = Val;
       new_mem->next = NULL;
       mBack->next = new_mem;
   }
   else
   {
       mFront = mAlloc.Alloc();
       mFront->mem = Val;
       mFront->next = NULL;
       mBack = mFront;
   }
}

template <class T>
inline void Queue<T>::Pop()
{
    debug_assert(Size(), "Queue is empty.");

    _member * mem = mFront;

    mFront = mem->next;

   if (!mFront)
       mBack = NULL;

   mAlloc.Free(mem);
}

template <class T>
inline int Queue<T>::Size() const
{
    return mAlloc->GetActiveCount();
}

template <class T>
inline bool Queue<T>::Empty() const
{
    return Size() == 0;
}

template <class T>
inline void Queue<T>::Clear()
{   
    mAlloc.Shutdown();
    mFront = mBack = NULL;
}