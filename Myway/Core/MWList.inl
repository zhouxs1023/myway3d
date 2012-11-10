

template <class T>
List<T>::List()
{
    mEnd.next = mEnd.prev = NULL;
    mFront = mBack = &mEnd;
}

template <class T>
List<T>::~List()
{
}

template <class T>
typename List<T>::Iterator List<T>::Begin()
{
    return Iterator(this, mFront);
}

template <class T>
typename List<T>::Iterator List<T>::End()
{
    return Iterator(this, mBack);
}

template <class T>
typename List<T>::ConstIterator List<T>::Begin() const
{
    return ConstIterator(this, mFront);
}

template <class T>
typename List<T>::ConstIterator List<T>::End() const
{
    return ConstIterator(this, mBack);
}

template <class T>
int List<T>::Size() const
{
    return mAlloc.GetActiveCount();
}

template <class T>
void List<T>::Clear()
{
    _node * node = mFront;

    while (node != mBack)
    {
        _node * next = node->next;
        mAlloc.Free(node);
        node = next;
    }

    assert (Size() == 0);

    mEnd.next = mEnd.prev = NULL;
    mFront = mBack = &mEnd;
}

template <class T>
bool List<T>::Empty() const
{
    return Size() == 0;
}

template <class T>
typename List<T>::Iterator  List<T>::Insert(const typename List<T>::Iterator & whr, const T & val)
{
    _node * newnode = mAlloc.Alloc();
    newnode->val = val;

    newnode->prev = whr.node->prev;
    newnode->next = whr.node;

    if (whr.node->prev)
    {
        whr.node->prev->next = newnode;
    }
    else 
    {
        mFront = newnode;
    }

    whr.node->prev = newnode;
    
    return Iterator(this, newnode);
}

template <class T>
T * List<T>::Increase()
{
    Iterator whr = Insert(End(), T());
    return &whr.node->val;
}

template <class T>
typename List<T>::Iterator List<T>::Erase(const typename List<T>::Iterator & whr)
{
    assert(whr != End());

    _node * node = whr.node;

    _node * next = whr.node->next;
    _node * prev = whr.node->prev;

    next->prev = prev;

    if (prev)
    {
        prev->next = next;
    }
    else
    {
        mFront = next;
    }

    mAlloc.Free(node);

    return Iterator(this, next);
}


template <class T>
void List<T>::PushFront(const T & val)
{
    Insert(Begin(), val);
}

template <class T>
void List<T>::PushBack(const T & val)
{
    Insert(End(), val);
}

template <class T>
void List<T>::PopFront()
{
    assert(Size());
    Erase(Begin());
}

template <class T>
void List<T>::PopBack()
{
    assert(Size());
    Erase(--End());
}

template <class T>
List<T>::List(const List<T> & r)
{
    mEnd.next = mEnd.prev = NULL;
    mFront = mBack = &mEnd;

    *this = r;
}

template <class T>
List<T> & List<T>::operator =(const List<T> & r)
{
    assert (this != &r);

    Clear();

    _node * node = r.mFront;

    while (node != r.mBack)
    {
        PushBack(node->val);
        node = node->next;
    }

    return *this;
}