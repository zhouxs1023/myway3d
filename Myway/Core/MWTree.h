#pragma once

#include "MWAllocator.h"

namespace Myway
{

#define __NIL           0x80000000
#define __ISNIL(x)      (x->flag & 0x80000000)
#define __SETNIL(x)     (x->flag | 0x80000000)
#define __CLEARNIL(x)   (x->flag & 0x7FFFFFFF)

template <class Key, class Type>
class Tree : public AllocObj
{
    struct _node
    {
        Key         key;
        Type        val;
        _node *     parent;
        _node *     left;
        _node *     right;
        int       flag;
    };

public:
    class Iterator
    {
    public:
        inline Iterator() {}
        inline ~Iterator() {}
        inline Iterator(const Iterator & r) : node(r.node) {}

    public:
        inline Iterator & operator ++()
        {
            assert(node && !__ISNIL(node));

            if (node->left && !__ISNIL(node->left))
            {
                __CLEARNIL(node);
                node = node->left;

                if (node->left)
                    __CLEARNIL(node->left);
                
                if (node->right)
                    __CLEARNIL(node->right);
            }
            else if (node->right && !__ISNIL(node->right))
            {
                __SETNIL(node);
                node = node->right;

                if (node->left)
                    __CLEARNIL(node->left);

                if (node->right)
                    __CLEARNIL(node->right);
            }
            else
            {
                __SETNIL(node);
                node = node->parent;
                while (node && !__ISNIL(node))
                {
                    node = node->parent;
                }
            }

            return *this;
        }

        inline Iterator operator ++(int)
        {
            Iterator iter = *this;
            ++(*this);
            return iter;
        }

        /*
        inline Iterator & operator --()
        {
            return *this;
        }

        inline Iterator operator --(int)
        {
            Iterator iter = *this;
            --(*this);
            return iter;
        }
        */

        inline Iterator & operator =(const Iterator & r)
        {
            node = r.node;
            return *this;
        }

        inline bool operator ==(const Iterator & r) const
        {
            return node == r.node;
        }

        inline bool operator !=(const Iterator & r) const
        {
            return node != r.node;
        }

        inline Type & operator *() const
        {
            return node->val;
        }

        inline Type * operator ->() const
        {
            return &node->val;
        }

        friend class Tree<Type, Key>;

    protected:
        _node * node;
    };

public:
                                Tree();
                                ~Tree();

    void                        Insert(Key key, Type val);
    Iterator                    Erase(Iterator iter);

    Iterator                    Begin() const;
    Iterator                    End() const;

    Iterator                    Find(Key key);

    int                         Size() const;
    bool                        Empty() const;
    void                        Clear();

protected:
    _node *                     AllocNode();

    _node *                     m_Root;
    Allocator<_node, 128>       m_Alloc;
};

#include "MWTree.inl"

}