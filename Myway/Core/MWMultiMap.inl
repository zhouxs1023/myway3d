

template <class Key, class T>
inline MultiMap<Key, T>::MultiMap()
: mHead(NULL)
{
}

template <class Key, class T>
inline MultiMap<Key, T>::~MultiMap()
{
    Clear();
}

template <class Key, class T>
inline void MultiMap<Key, T>::Clear()
{
    _node * node = mHead;

    while (node)
    {
        node->mem.second.Clear();
        node = node->next;
    }

    mHead = NULL;
}

template <class Key, class T>
inline int MultiMap<Key, T>::Size() const
{
    return mAlloc.GetActiveCount();
}

template <class Key, class T>
inline bool MultiMap<Key, T>::Empty() const
{
    return Size() == 0;
}

template <class Key, class T>
inline typename MultiMap<Key, T>::Iterator MultiMap<Key, T>::Insert(const Key & k, const T & v)
{
    _node * node = mHead;

    while (node)
    {
        if (node->mem.first == k)
        {
            break;
        }

        node = node->next;
    }

    if (node)
    {
        node->mem.second.PushBack(v);
    }
    else
    {
        node = mAlloc.Alloc();
        node->mem.first = k;
        node->mem.second.PushBack(v);
        node->prev = NULL;
        node->next = mHead;
        mHead = node;
    }

    Iterator iter;
    iter.node = node;

    return iter;
}

template <class Key, class T>
inline typename MultiMap<Key, T>::Iterator MultiMap<Key, T>::Erase(const Key & k)
{
    _node * node = mHead;

    while (node)
    {
        if (node->mem.first == k)
            break;

        node = node->next;
    }

    assert(node->mem.first == k);

    Iterator iter;
    iter.node = node->next;

    if (node->next)
    {
        node->next->prev = node->prev;
    }

    if (node->prev)
    {
        node->prev->next = node->next;
    }
    //head
    else
    {
        mHead = NULL;
    }

    node->mem.second.Clear();
    mAlloc.Free(node);

    return iter;
}

template <class Key, class T>
inline bool MultiMap<Key, T>::Exist(const Key & k) const
{
    _node * node = mHead;

    while (node)
    {
        if (node->mem.first == k)
            return true;

        node = node->next;
    }

    return false;
}

template <class Key, class T>
inline typename MultiMap<Key, T>::Iterator MultiMap<Key, T>::Begin() const
{
    Iterator Iter;
    Iter.node = mHead;
    return Iter;
}

template <class Key, class T>
inline typename MultiMap<Key, T>::Iterator MultiMap<Key, T>::End() const
{
    Iterator Iter;
    Iter.node = NULL;
    return Iter;
}

template <class Key, class T>
inline typename MultiMap<Key, T>::Iterator MultiMap<Key, T>::Find(const Key & k) const
{
    Iterator iter;
    _node * node = mHead;

    while (node)
    {
        if (node->mem.first == k)
            break;

        node = node->next;
    }

    iter.node = node;
    
    return iter;
}

template <class Key, class T>
inline int MultiMap<Key, T>::Count(const Key & k) const
{
    _node * node = mHead;

    while (node)
    {
        if (node->mem.first == k)
        {
            return node->mem.second.Size();
        }

        node = node->next;
    }

    return 0;
}

