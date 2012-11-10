#pragma once

#include "MWAllocator.h"

namespace Myway
{

template <class T>
struct __hash_t
{
    int operator ()(const T & k) const
    {
        assert (0);
        return 0;
    }
};

template <class T>
struct __equal_t
{
    bool operator ()(const T & left, const T & right) const
    {
        return left == right;
    }
};

template <>
struct __hash_t<int>
{ 
    int operator()(int x) const
    { 
        return x;
    }
};

template <class Key, class Type, class HashFunc = __hash_t<Key>, class Equal = __equal_t<Key>>
class HashMap : public AllocObj
{
    friend class Iterator;

protected:
    struct _pair
    {
        Key         first;
        Type        second;
    };

    struct _node
    {
        _node *     prev;
        _node *     next;
        _pair       val;
    };

    struct _list
    {
        _node *     head;
        _node *     tail;
    };

    
    //const iterator

public:
    class ConstIterator : public AllocObj
    {
        friend class HashMap;

    public:
        ConstIterator() : creator(NULL), hash(0), node(NULL) {}
        ~ConstIterator() {}

        ConstIterator(const ConstIterator & r)
        {
            creator = r.creator;
            hash = r.hash;
            node = r.node;
        }

        ConstIterator & operator =(const ConstIterator & r)
        {
            creator = r.creator;
            hash = r.hash;
            node = r.node;

            return *this;
        }

        const _pair * operator ->() const
        {
            return &node->val;
        }

        ConstIterator & operator ++()
        {
            int size = creator->HashSize();

            assert (hash < size);

            if (node)
            {
                node = node->next;
            }

            while (!node && ++hash < size)
            {
                node = creator->_MyHead(hash);
            }

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
            int size = creator->HashSize();

            if (node)
            {
                node = node->prev;
            }

            while (!node && --hash < size)
            {
                node = creator->_MyTail(hash);
            }

            return *this;
        }

        ConstIterator operator --(int)
        {
            ConstIterator iter = *this;

            --(*this);

            return iter;
        }

        bool operator ==(const ConstIterator & r) const
        {
#ifdef _DEBUG

            return creator == r.creator &&
                node == r.node;

#else

            return node == r.node;

#endif
        }

        bool operator !=(const ConstIterator & r) const
        {
#ifdef _DEBUG

            return creator == r.creator &&
                node != r.node;

#else

            return node != r.node;

#endif
        }

    protected:
        int              hash;
        const _node *       node;
        const HashMap *     creator;
    };


public:
    class Iterator : public AllocObj
    {
        friend class HashMap;

    public:
        Iterator() : creator(NULL), hash(-1), node(NULL) {}
        ~Iterator() {}

        Iterator(const Iterator & r)
        {
            creator = r.creator;
            hash = r.hash;
            node = r.node;
        }

        Iterator & operator =(const Iterator & r)
        {
            creator = r.creator;
            hash = r.hash;
            node = r.node;

            return *this;
        }

        _pair * operator ->() const
        {
            return &node->val;
        }

        Iterator & operator ++()
        {
            int size = creator->HashSize();

            assert (hash < size);

            if (node)
            {
                node = node->next;
            }

            while (!node && ++hash < size)
            {
                node = creator->_MyHead(hash);
            }

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
            int size = creator->HashSize();

            if (node)
            {
                node = node->prev;
            }

            while (!node && --hash < size)
            {
                node = creator->_MyTail(hash);
            }

            return *this;
        }

        Iterator operator --(int)
        {
            Iterator iter = *this;

            --(*this);

            return iter;
        }

        bool operator ==(const Iterator & r) const
        {
#ifdef _DEBUG

             return creator == r.creator &&
                    node == r.node;

#else

            return node == r.node;

#endif
        }

        bool operator !=(const Iterator & r) const
        {
#ifdef _DEBUG

            return creator == r.creator &&
                   node != r.node;

#else

            return node != r.node;

#endif
        }

    protected:
        int              hash;
        _node *             node;
        const HashMap *     creator;
    };

public:
    HashMap(int size);
    HashMap();
    ~HashMap();

    void Clear();
    int Size() const;
    int HashSize() const;
    bool Empty() const;

    void Resize(int size);

    ConstIterator Begin() const;
    ConstIterator End() const;
    ConstIterator Find(const Key & k) const;

    Iterator Begin();
    Iterator End();
    Iterator Find(const Key & k);

    Iterator Insert(const Key & k, const Type & v);
    Iterator Erase(Iterator whr);

    Type & operator [](const Key & k);
    const Type & operator [](const Key & k) const;

protected:
    _node *     _MyHead(int hash) const  { return mList[hash].head; }
    _node *     _MyTail(int hash) const  { return mList[hash].tail; }

protected:
    HashFunc                    mFunc;
    Equal                       mEqual;

    int                         mSize;
    _list *                     mList;

    Allocator<_node, 128>       mAlloc;
};

#include "MWHashMap.inl"

}