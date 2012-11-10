
template <class Key, class Type, class HashFunc, class Equal>
HashMap<Key, Type, HashFunc, Equal>::HashMap(int size)
{
    assert (size > 0);

    // must be pow of 2.
    --size; 
    size |= size >> 1; 
    size |= size >> 2;
    size |= size >> 4;
    size |= size >> 8;
    size |= size >> 16;
    ++size;


    mList = (_list*)alloc_mem(size * sizeof(_list));
    Memzero(mList, sizeof(_list) * size);

    mSize = size;
}

template <class Key, class Type, class HashFunc, class Equal>
HashMap<Key, Type, HashFunc, Equal>::HashMap()
{
    mList = (_list*)alloc_mem(256 * sizeof(_list));
    Memzero(mList, sizeof(_list) * 256);

    mSize = 256;
}

template <class Key, class Type, class HashFunc, class Equal>
HashMap<Key, Type, HashFunc, Equal>::~HashMap()
{
    safe_free(mList);
}

template <class Key, class Type, class HashFunc, class Equal>
void HashMap<Key, Type, HashFunc, Equal>::Clear()
{
    for (int i = 0; i < mSize; ++i)
    {
        _node * node = mList[i].head;

        while (node)
        {
            mAlloc.Free(node);

            node = node->next;
        }
    }

    Memzero(mList, sizeof(_list) * mSize);
}

template <class Key, class Type, class HashFunc, class Equal>
int HashMap<Key, Type, HashFunc, Equal>::Size() const
{
    return mAlloc.GetActiveCount();
}

template <class Key, class Type, class HashFunc, class Equal>
bool HashMap<Key, Type, HashFunc, Equal>::Empty() const
{
    return Size() == 0;
}

template <class Key, class Type, class HashFunc, class Equal>
int HashMap<Key, Type, HashFunc, Equal>::HashSize() const
{
    return mSize;
}

template <class Key, class Type, class HashFunc, class Equal>
void HashMap<Key, Type, HashFunc, Equal>::Resize(int size)
{
    assert (size > 0);

    // must be pow of 2.
    --size; 
    size |= size >> 1; 
    size |= size >> 2;
    size |= size >> 4;
    size |= size >> 8;
    size |= size >> 16;
    ++size;

    _list * oldlist = (_list*)alloc_mem(size * sizeof(_list));
    int oldsize = mSize;

    _list tmp = mList;
    mList = oldlist;
    oldlist = tmp;
    mSize = size;

    for (int i = 0; i < oldsize; ++i)
    {
        _node * node = oldlist[i].head;

        while (node)
        {
            Insert(node->val.first, node->val.second);
            mAlloc.Free(node);

            node = node->next;
        }
    }

    safe_free(oldlist);
}

template <class Key, class Type, class HashFunc, class Equal>
typename HashMap<Key, Type, HashFunc, Equal>::ConstIterator HashMap<Key, Type, HashFunc, Equal>::Begin() const
{
    int hash = 0;
    _node * node = mList[hash].head;

    while (!node && ++hash < mSize)
    {
        node = mList[hash].head;
    }

    ConstIterator iter;

    iter.creator = this;
    iter.hash = hash;
    iter.node = node;

    return iter;
}

template <class Key, class Type, class HashFunc, class Equal>
typename HashMap<Key, Type, HashFunc, Equal>::ConstIterator HashMap<Key, Type, HashFunc, Equal>::End() const
{
    ConstIterator iter;

    iter.creator = this;
    iter.hash = mSize;
    iter.node = NULL;

    return iter;
}

template <class Key, class Type, class HashFunc, class Equal>
typename HashMap<Key, Type, HashFunc, Equal>::ConstIterator HashMap<Key, Type, HashFunc, Equal>::Find(const Key & k) const
{
    int hash = mFunc(k) & (mSize - 1);

    _node * node = mList[hash].head;

    while (node && !mEqual(node->val.first, k))
    {
        node = node->next;
    }

    if (!node)
        hash = mSize;

    ConstIterator iter;

    iter.creator = this;
    iter.hash = hash;
    iter.node = node;

    return iter;
}

template <class Key, class Type, class HashFunc, class Equal>
typename HashMap<Key, Type, HashFunc, Equal>::Iterator HashMap<Key, Type, HashFunc, Equal>::Begin()
{
    int hash = 0;
    _node * node = mList[hash].head;

    while (!node && ++hash < mSize)
    {
        node = mList[hash].head;
    }

    Iterator iter;

    iter.creator = this;
    iter.hash = hash;
    iter.node = node;

    return iter;
}

template <class Key, class Type, class HashFunc, class Equal>
typename HashMap<Key, Type, HashFunc, Equal>::Iterator HashMap<Key, Type, HashFunc, Equal>::End()
{
    Iterator iter;

    iter.creator = this;
    iter.hash = mSize;
    iter.node = NULL;

    return iter;
}

template <class Key, class Type, class HashFunc, class Equal>
typename HashMap<Key, Type, HashFunc, Equal>::Iterator HashMap<Key, Type, HashFunc, Equal>::Find(const Key & k)
{
    int hash = mFunc(k) & (mSize - 1);

    _node * node = mList[hash].head;

    while (node && !mEqual(node->val.first, k))
    {
        node = node->next;
    }

    if (!node)
        hash = mSize;

    Iterator iter;

    iter.creator = this;
    iter.hash = hash;
    iter.node = node;

    return iter;
}

template <class Key, class Type, class HashFunc, class Equal>
typename HashMap<Key, Type, HashFunc, Equal>::Iterator HashMap<Key, Type, HashFunc, Equal>::Insert(const Key & k, const Type & v)
{
    assert (Find(k) == End());

    int hash = mFunc(k) & (mSize - 1);

    _node * prev = mList[hash].tail;
    _node * node = mAlloc.Alloc();

    node->next = NULL;
    node->prev = prev;
    node->val.first = k;
    node->val.second = v;

    if (prev)
        prev->next = node;
    else
        mList[hash].head = node;

    mList[hash].tail = node;


    Iterator iter;

    iter.creator = this;
    iter.hash = hash;
    iter.node = node;

    return iter;
}

template <class Key, class Type, class HashFunc, class Equal>
typename HashMap<Key, Type, HashFunc, Equal>::Iterator HashMap<Key, Type, HashFunc, Equal>::Erase(typename HashMap<Key, Type, HashFunc, Equal>::Iterator iter)
{
    assert (iter.creator == this && iter != End());

    int hash = iter.hash;
    _node * node = iter.node;

    assert (node);

    ++iter; 
    
    if (node->prev)
    {
        node->prev->next = node->next;
    }
    else
    {
        mList[hash].head = node->next;
    }

    if (node->next)
    {
        node->next->prev = node->prev;
    }
    else
    {
        mList[hash].tail = node->prev;
    }

    mAlloc.Free(node);

    return iter;
}

template <class Key, class Type, class HashFunc, class Equal>
typename Type & HashMap<Key, Type, HashFunc, Equal>::operator [](const Key & k)
{
    Iterator iter = Find(k);

    if (iter == End())
    {
        iter = Insert(k, Type());
    }

    return iter->second;
}

template <class Key, class Type, class HashFunc, class Equal>
const typename Type & HashMap<Key, Type, HashFunc, Equal>::operator [](const Key & k) const
{
    Iterator iter = Find(k);

    assert (iter != End());

    return iter->second;
}