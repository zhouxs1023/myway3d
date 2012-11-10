#pragma once

#include "MWAllocObj.h"

namespace Myway
{

template <class T>
class Array
{
    DECLARE_ALLOC();

public:
    class ConstIterator
    {
    public:
        ConstIterator() {}
        ~ConstIterator() {}

        ConstIterator(const T * m) : mem(m) {}

        const T & operator *() const
        {
            return *mem;
        }

        const T * operator ->() const
        {
            return mem;
        }

        ConstIterator & operator ++()
        {
            ++mem;
            return *this;
        }

        ConstIterator operator ++(int)
        {
            ConstIterator iter = *this;
            ++mem;
            return iter;
        }

        ConstIterator operator +(int i) const
        {
            ConstIterator iter = *this;
            iter.mem += i;
            return iter;
        }

        ConstIterator & operator --()
        {
            --mem;
            return *this;
        }

        ConstIterator operator --(int)
        {
            ConstIterator iter = *this;
            --mem;
            return iter;
        }

        ConstIterator operator -(int i) const
        {
            ConstIterator iter = *this;
            iter.mem -= i;
            return iter;
        }

        ConstIterator & operator +=(int i)
        {
            mem += i;
            return *this;
        }

        ConstIterator & operator -=(int i)
        {
            mem -= i;
            return *this;
        }

        ConstIterator & operator =(const ConstIterator & r)
        {
            mem = r.mem;
            return *this;
        }

        ConstIterator(const ConstIterator & r)
        {
            mem = r.mem;
        }

        bool operator ==(const ConstIterator & r) const
        {
            return mem == r.mem;
        }

        bool operator !=(const ConstIterator & r) const
        {
            return mem != r.mem;
        }

        friend class Array<T>;

    protected:
        const T *     mem;
    };

public:
    class Iterator
    {
    public:
        Iterator() {}
        ~Iterator() {}
        Iterator(T * m) : mem(m) {}

        T & operator *()
        {
            return *mem;
        }

        T * operator ->()
        {
            return mem;
        }

        Iterator & operator ++()
        {
            ++mem;
            return *this;
        }

        Iterator operator ++(int)
        {
            Iterator iter = *this;
            ++mem;
            return iter;
        }

        Iterator operator +(int i) const
        {
            Iterator iter = *this;
            iter.mem += i;
            return iter;
        }

        Iterator & operator --()
        {
            --mem;
            return *this;
        }

        Iterator operator --(int)
        {
            Iterator iter = *this;
            --mem;
            return iter;
        }

        Iterator operator -(int i) const
        {
            Iterator iter = *this;
            iter.mem -= i;
            return iter;
        }

        Iterator & operator +=(int i)
        {
            mem += i;
            return *this;
        }

        Iterator & operator -=(int i)
        {
            mem -= i;
            return *this;
        }

        Iterator & operator =(const Iterator & r)
        {
            mem = r.mem;
            return *this;
        }

        Iterator(const Iterator & r)
        {
            mem = r.mem;
        }

        bool operator ==(const Iterator & r) const
        {
            return mem == r.mem;
        }

        bool operator !=(const Iterator & r) const
        {
            return mem != r.mem;
        }

        friend class Array<T>;

    protected:
        T *     mem;
    };


public:
    Array(int size);
    Array();
    ~Array();

    void            Resize(int size);
    int             Size() const;
    int             MaxSize() const;
    bool            Empty() const;
    void            Clear();
    void            Destroy();

    Iterator        Begin();
    Iterator        End();

    ConstIterator   Begin() const;
    ConstIterator   End() const;

    T *             Increase();
    Iterator        Insert(Iterator whr, const T & v);
    Iterator        Erase(Iterator whr);

    void            Insert(int index, const T & v);
    void            Erase(int index);

    void            PushFront(const T & v);
    void            PopFront();
    void            PushBack(const T & v);
    void            PopBack();

    void            Swap(Array & vec);


    T &             operator [](int index);
    const T &       operator [](int index) const;

                    Array(const Array & vec);
    Array &        operator =(const Array & vec);


protected:
    int          mMaxSize;
    int          mIncreaseSize;
    int          mSize;
    T *          mMember;
};

#include "MWArray.inl"
}
