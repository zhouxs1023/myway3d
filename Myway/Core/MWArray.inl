

#pragma warning(push)
#pragma warning(disable : 4244)

template <class T>
Array<T>::Array()
: mMaxSize(0), 
  mIncreaseSize(DEFAULT_INCREASE_SIZE), mSize(0), mMember(0)
{
}

template <class T>
Array<T>::Array(int iSize)
: mMaxSize(iSize), mSize(iSize)
{
    mIncreaseSize = (mMaxSize > DEFAULT_INCREASE_SIZE) ? mMaxSize : DEFAULT_INCREASE_SIZE;
    mMember = new T[iSize];
}

template <class T>
Array<T>::~Array()
{
    safe_delete_array(mMember);
}

template <class T>
void Array<T>::Resize(int iSize)
{
    if (mMaxSize < iSize)
    {
        mMaxSize = iSize;

        T * pTmp = new T[mMaxSize];
        if (mSize > 0)
        {
            int i = 0;
            while (i < mSize)
            {
                pTmp[i] = mMember[i];
                i++;
            }

            safe_delete_array(mMember);
        }

        mMember = pTmp;
        mIncreaseSize = (mMaxSize > DEFAULT_INCREASE_SIZE) ? mMaxSize : DEFAULT_INCREASE_SIZE;
    }

    mSize = iSize;
}

template <class T>
int Array<T>::Size() const
{
    return mSize;
}

template <class T>
int Array<T>::MaxSize() const
{
    return mMaxSize;
}

template <class T>
bool Array<T>::Empty() const
{
    return mSize == 0;
}

template <class T>
void Array<T>::Clear()
{
    mSize = 0;
}

template <class T>
void Array<T>::Destroy()
{
    safe_delete_array(mMember);
    mMaxSize = 0; 
    mIncreaseSize = DEFAULT_INCREASE_SIZE;
    mSize = 0;
}

template <class T>
typename Array<T>::Iterator Array<T>:: Begin()
{
    return Iterator(mMember);
}

template <class T>
typename Array<T>::Iterator Array<T>::End()
{
    return Iterator(mMember + mSize);
}

template <class T>
typename Array<T>::ConstIterator Array<T>::Begin() const
{
    return ConstIterator(mMember);
}

template <class T>
typename Array<T>::ConstIterator Array<T>::End() const
{
    return ConstIterator(mMember + mSize);
}

template <class T>
T * Array<T>::Increase()
{
    if (mSize == mMaxSize)
    {
        mMaxSize += mIncreaseSize;
        T * pTmp = new T[mMaxSize];

        if (mSize > 0)
        {
            int i = 0;
            while (i < mSize)
            {
                pTmp[i] = mMember[i];
                ++i;
            }

            safe_delete_array(mMember);
        }

        mMember = pTmp;
        mIncreaseSize = mMaxSize;
    }

    return &mMember[mSize++];
}

template <class T>
typename Array<T>::Iterator Array<T>::Insert(typename Array<T>::Iterator iter, const T & Mem)
{
    int index = iter.mem - mMember;
    Insert(index, Mem);
    iter.mem = mMember + index;
    return iter;
}

template <class T>
typename Array<T>::Iterator Array<T>::Erase(typename Array<T>::Iterator iter)
{
    int index = iter.mem - mMember;
    Erase(index);
    iter.mem = mMember + index;
    return iter;
}

template <class T>
void Array<T>::Insert(int index, const T & Val)
{
    debug_assert(index >= 0 && index <= mSize, "Invalid index!");

    if (mSize == mMaxSize)
    {
        mMaxSize += mIncreaseSize;
        T * pTmp = new T[mMaxSize];

        if (mSize > 0)
        {
            int i = 0;
            while (i < index)
            {
                pTmp[i] = mMember[i];
                ++i;
            }

            while (i < mSize)
            {
                pTmp[i + 1] = mMember[i];
                ++i;
            }

            safe_delete_array(mMember);
        }

        mMember = pTmp;
        mIncreaseSize = mMaxSize;
    }
    else
    {
        int i = mSize;
        while (i > index)
        {
            mMember[i] = mMember[i - 1];
            --i;
        }
    }

    mMember[index] = Val;
    ++mSize;
}

template <class T>
void Array<T>::Erase(int index)
{
    debug_assert(index >= 0 && index < mSize, "Invalid index!");

    int num_copy = mSize - index - 1;
    int i = index;
    int n = 0;
    while (n < num_copy)
    {
        mMember[i] = mMember[i + 1];
        ++i;
        ++n;
    }

    --mSize;
}

template <class T>
void Array<T>::PushFront(const T & Val)
{
    Insert(0, Val);
}

template <class T>
void Array<T>::PopFront()
{
    Erase(0);
}

template <class T>
void Array<T>::PushBack(const T & Val)
{
    if (mSize == mMaxSize)
    {
        mMaxSize += mIncreaseSize;
        T * pTmp = new T[mMaxSize];

        if (mSize > 0)
        {
            int i = 0;
            while (i < mSize)
            {
                pTmp[i] = mMember[i];
                ++i;
            }

            safe_delete_array(mMember);
        }

        mMember = pTmp;
        mIncreaseSize = mMaxSize;
    }

    mMember[mSize] = Val;
    ++mSize;
}

template <class T>
void Array<T>::PopBack()
{
    assert(mSize);
    --mSize;
}

template <class T>
T & Array<T>::operator [](int index)
{
    debug_assert(index >= 0 && index < mSize, "Invalid index!");
    return mMember[index];
}

template <class T>
const T & Array<T>::operator [](int index) const
{
    debug_assert(index >= 0 && index < mSize, "Invalid index!");
    return mMember[index];
}

template <class T>
Array<T>::Array(const Array<T> & vec)
{
    mSize = vec.mSize;
    mMaxSize = vec.mMaxSize;
    mIncreaseSize = vec.mIncreaseSize;
    mMember = new T[mMaxSize];
    for (int i = 0; i < mSize; i++)
    {
        mMember[i] = vec.mMember[i];
    }
}

template <class T>
Array<T>& Array<T>::operator =(const Array<T> & vec)
{
    if (mMaxSize > vec.mSize)
    {
        for (int i = 0; i < vec.mSize; i++)
        {
            mMember[i] = vec.mMember[i];
        }

        mSize = vec.mSize;
    }
    else if (vec.mSize == 0)
    {
        mSize = 0;
    }
    else 
    {
        mSize = vec.mSize;
        mMaxSize = vec.mMaxSize;
        mIncreaseSize = vec.mIncreaseSize;
        safe_delete_array(mMember);
        mMember = new T[mMaxSize];
        for (int i = 0; i < mSize; i++)
        {
            mMember[i] = vec.mMember[i];
        }
    }

    return *this;
}

template <class T>
void Array<T>::Swap(Array<T> & vec)
{
    int iSize = vec.mSize;
    int iMaxSize = vec.mMaxSize;
    int iIncreaseSize = vec.mIncreaseSize;
    T * pMember = vec.mMember;

    mSize = vec.mSize;
    mMaxSize = vec.mMaxSize;
    mIncreaseSize = vec.mIncreaseSize;
    mMember = vec.mMember;

    vec.mSize = iSize;
    vec.mMaxSize = iMaxSize;
    vec.mIncreaseSize = iIncreaseSize;
    vec.mMember = pMember;
}

#pragma warning(pop)