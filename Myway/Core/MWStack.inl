

template <class T>
inline Stack<T>::Stack()
: mTop(NULL)
{
}

template <class T>
inline Stack<T>::~Stack()
{
    Clear();
}

template <class T>
inline void Stack<T>::Clear()
{
    mAlloc.Shutdown();
    mTop = NULL;
}

template <class T>
inline bool Stack<T>::Empty() const
{
    return Size() == 0;
}

template <class T>
void Stack<T>::Push(const T & val)
{
    _member * mem = mAlloc.Alloc();

    mem->mem = val;
    mem->next = mTop;
    mTop = mem;
}

template <class T>
inline void Stack<T>::Pop()
{
    debug_assert(Size(), "Stack is empty.");

    _member * mem = mTop;
    mTop = mem->next;
    mAlloc.Free(mem);
}

template <class T>
inline int Stack<T>::Size() const
{
    return mAlloc.GetActiveCount();
}

template <class T>
inline T & Stack<T>::Top()
{
    debug_assert(Size(), "Stack is empty.");
    return mTop->mem;
}

template <class T>
inline const T & Stack<T>::Top() const
{
    debug_assert(Size(), "Stack is empty.");
    return mTop->mem;
}