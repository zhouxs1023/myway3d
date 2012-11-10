

template <class T>
inline DQueue<T>::DQueue()
: mSize(0),
  mIncreaseSize(DEFAULT_INCREASE_SIZE),
  mMaxSize(0),
  mMember(0)
{
}

template <class T>
inline DQueue<T>::DQueue(int iAllcateSize)
{
    mSize = 0;
    mMember = new T[iAllcateSize];
    mMaxSize = iAllcateSize;
    mIncreaseSize = (mMaxSize < DEFAULT_INCREASE_SIZE) ? DEFAULT_INCREASE_SIZE : mMaxSize;
}

template <class T>
inline DQueue<T>::~DQueue()
{
    Clear();
    if (mMember)
        delete[] mMember;
}

template <class T>
inline T & DQueue<T>::Front()
{
    debug_assert(mSize != 0, "DQueue is empty.");
    return mMember[0];
}

template <class T>
inline const T & DQueue<T>::Front() const
{
    debug_assert(mSize != 0, "DQueue is empty.");
    return mMember[0];
}

template <class T>
inline T & DQueue<T>::Back()
{
    debug_assert(mSize != 0, "DQueue is empty.");
    return mMember[mSize - 1];
}

template <class T>
inline const T & DQueue<T>::Back() const
{
    debug_assert(mSize != 0, "DQueue is empty.");
    return mMember[mSize - 1];
}

template <class T>
void DQueue<T>::PushFront(const T & Val)
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
                pTmp[i + 1] = mMember[i];
                i++;
            }

            delete[] mMember;
        }

        mMember = pTmp;
        mIncreaseSize = mMaxSize;
    }
    else
    {
        int i = mSize;
        while (i > 0)
        {
            mMember[i] = mMember[i - 1];
            i--;
        }
    }

    mMember[0] = Val;
    mSize++;
}

template <class T>
void DQueue<T>::PushBack(const T & Val)
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
                i++;
            }

            delete[] mMember;
        }
        
        mMember = pTmp;
        mIncreaseSize = mMaxSize;
    }

    mMember[mSize] = Val;
    mSize++;
}

template <class T>
inline inline void DQueue<T>::PopFront()
{
    debug_assert(mSize != 0, "DQueue is empty.");
    mSize--;

    int i = 0;
    while (i < mSize)
    {
        mMember[i] = mMember[i + 1];
        i++;
    }
}

template <class T>
inline void DQueue<T>::PopBack()
{
    debug_assert(mSize != 0, "DQueue is empty.");
    mSize--;
}


template <class T>
inline int DQueue<T>::Size() const
{
    return mSize;
}

template <class T>
inline bool DQueue<T>::Empty() const
{
    return mSize == 0;
}

template <class T>
inline void DQueue<T>::Clear()
{
    mSize = 0;
}