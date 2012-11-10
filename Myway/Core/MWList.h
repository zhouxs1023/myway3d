#pragma once

#include "MWAllocator.h"

namespace Myway
{

template <class T>
class List : public AllocObj
{
protected:
    struct _node
    {
        _node *    next;
        _node *    prev;
        T          val;
    };


public:
    class ConstIterator
    {
        friend class List<T>;

#if _DEBUG
        ConstIterator(const List * l, const _node * n) : creator(l), node(n) {}
#else
        ConstIterator(const List * l, const _node * n) : node(n) {}
#endif

    public:
        ConstIterator() {}
        ~ConstIterator() {}

#if _DEBUG
        ConstIterator(const ConstIterator & r) : creator(r.creator), node(r.node) {}
#else
        ConstIterator(const ConstIterator & r) : node(r.node) {}
#endif

        ConstIterator & operator ++()
        {
            assert (creator->_MyEnd() != node);

            node = node->next;
            return *this;
        }

        ConstIterator operator ++(int)
        {
            ConstIterator iter = *this;
            ++(*this);
            return iter;
        }

        ConstIterator & operator --()
        {
            assert(creator->_MyBeg() != node);

            node = node->prev;
            return *this;
        }

        ConstIterator operator --(int)
        {
            ConstIterator iter = *this;
            --(*this);
            return iter;
        }

        ConstIterator & operator =(const ConstIterator & r)
        {
#ifdef _DEBUG
            creator = r.creator;
#endif
            node = r.node;
            return *this;
        }

        bool operator ==(const ConstIterator & r) const
        {
#ifdef _DEBUG
            return creator == r.creator && node == r.node;
#else
            return node == r.node;
#endif
        }

        bool operator !=(const ConstIterator & r) const
        {
            return node != r.node;
        }

        const T & operator *()
        {
            return node->val;
        }

        const T * operator ->()
        {
            return &node->val;
        }

    protected:

#if _DEBUG
        const List *   creator;
#endif
        const _node *  node;
    };

public:
    class Iterator
    {
        friend class List<T>;

#if _DEBUG
        Iterator(const List * l, _node * n) : creator(l), node(n) {}
#else
        Iterator(const List * l, _node * n) : node(n) {}
#endif

    public:
        Iterator() {}
        ~Iterator() {}

#if _DEBUG
        Iterator(const Iterator & r) : creator(r.creator), node(r.node) {}
#else
        Iterator(const Iterator & r) : node(r.node) {}
#endif


        Iterator & operator ++()
        {
            assert (creator->_MyEnd() != node);

            node = node->next;
            return *this;
        }

        Iterator operator ++(int)
        {
            Iterator iter = *this;
            ++(*this);
            return iter;
        }

        Iterator & operator --()
        {
            assert(creator->_MyBeg() != node);

            node = node->prev;
            return *this;
        }

        Iterator operator --(int)
        {
            Iterator iter = *this;
            --(*this);
            return iter;
        }

        Iterator & operator =(const Iterator & r)
        {
#ifdef _DEBUG
            creator = r.creator;
#endif
            node = r.node;
            return *this;
        }

        bool operator ==(const Iterator & r) const
        {
#ifdef _DEBUG
            return creator == r.creator && node == r.node;
#else
            return node == r.node;
#endif
        }

        bool operator !=(const Iterator & r) const
        {
            return node != r.node;
        }

        T & operator *()
        {
            return node->val;
        }

        T * operator ->()
        {
            return &node->val;
        }

    protected:

#if _DEBUG
        const List *    creator;
#endif
        _node *         node;
    };

public:
    List();
    ~List();

    Iterator        Begin();
    Iterator        End();

    ConstIterator   Begin() const;
    ConstIterator   End() const;


    int             Size() const;
    void            Clear();
    bool            Empty() const;

    Iterator        Insert(const Iterator & whr, const T & val);
    Iterator        Erase(const Iterator & whr);

    T *             Increase();

    void            PushFront(const T & val);
    void            PushBack(const T & val);
    void            PopFront();
    void            PopBack();

                    List(const List & l);
    List&           operator =(const List & l);

protected:
    _node *         _MyBeg() const { return mFront; }
    _node *         _MyEnd() const { return mBack; }

protected:
    Allocator<_node, 128>  mAlloc;

    _node *                mFront;
    _node *                mBack;
    _node                  mEnd;

};

#include "MWList.inl"

}