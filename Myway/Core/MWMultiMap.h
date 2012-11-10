#pragma once

#include "MWAllocObj.h"
#include "MWAllocator.h"

namespace Myway
{

template <class Key, class T>
class MultiMap
{
protected:
    struct _member
    {
        Key             first;
        List<T>         second;
    };

    struct _node
    {
        _node *         prev;
        _node *         next;
        _member         mem;
    };

public:
    class Iterator
    {
    public:
        inline Iterator() {}
        inline ~Iterator() {}

        inline _member * operator ->() const
        {
            return &node->mem;
        }

        inline Iterator & operator ++()
        {
            node = node->next;
            return *this;
        }

        inline Iterator operator ++(int)
        {
            Iterator Iter = *this;
            node = node->next;
            return Iter;
        }

        inline Iterator & operator --()
        {
            node = node->prev;
            return *this;
        }

        inline Iterator operator --(int)
        {
            Iterator Iter = *this;
            node = node->prev;
            return Iter;
        }

        inline Iterator & operator =(const Iterator & Iter)
        {
            node = Iter.node;
            return *this;
        }

        inline Iterator(const Iterator & Iter)
        {
            node = Iter.node;
        }

        inline bool operator ==(const Iterator & Right)
        {
            return node == Right.node;
        }

        inline bool operator !=(const Iterator & Right)
        {
            return node != Right.node;
        }

        friend class MultiMap<Key, T>;

    protected:
        _node * node;
    };

public:
                MultiMap();
                ~MultiMap();

    void        Clear();
    int         Size() const;
    bool        Empty() const;

    Iterator    Begin() const;
    Iterator    End() const;
    Iterator    Find(const Key & k) const;

    Iterator    Insert(const Key & k, const T & v);
    Iterator    Erase(const Key & k);
    bool        Exist(const Key & k) const;
    int         Count(const Key & k) const;

private:
    explicit    MultiMap(const MultiMap & m);
    MultiMap &  operator = (const MultiMap & m);

protected:
    _node *                     mHead;
    Allocator<_node, 128>       mAlloc;
};

#include "MWMultiMap.inl"

}