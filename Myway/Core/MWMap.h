#pragma once

#include "MWAllocObj.h"

namespace Myway
{

#define __RBT_RED   1
#define __RBT_BLACK 0

template <class T>
struct __less_t
{
    bool operator ()(const T & left, const T & right) const
    {
        return left < right;
    }
};

template <class Key, class T, class Less = __less_t<Key>>
class Map : public AllocObj
{
    friend class Iterator;
    friend class ConstIterator;

    struct _pair
    {
        Key         first;
        T           second;
    };

    class _node : public AllocObj
    {
    public:
        _node() : parent(NULL), left(NULL), right(NULL), color(__RBT_RED) {}
        ~_node() {}

        void SetLeftChild(_node * node)
        {
            left = node;
            if (node)
                node->SetParent(this);
        }

        void SetRightChild(_node * node)
        {
            right = node;
            if (node)
                node->SetParent(this);
        }

        void SetParent(_node * node)
        {
            parent = node;
        }

        void SetPair(const Key & k, const T & v)
        {
            pair.first = k;
            pair.second = v;
        }

        void SetVaule(const T & v)
        {
            pair.second = v;
        }

        void SetRed()
        {
            color = __RBT_RED;
        }

        void SetBlack()
        {
            color = __RBT_BLACK;
        }

        void SetColor(int c)
        {
            color = c;
        }

        _node * GetLeftChild() const
        {
            return left;
        }

        _node * GetRightChild() const
        {
            return right;
        }

        _node * GetParent() const
        {
            return parent;
        }

        bool IsRoot() const
        {
            return parent == NULL;
        }

        bool IsLeaf() const
        {
            return left == NULL && right == NULL;
        }

        bool IsLeftChild() const
        {
            return parent && parent->GetLeftChild() == this;
        }

        bool IsRightChild() const
        {
            return parent && parent->GetRightChild() == this;
        }

        const Key & GetKey() const
        {
            return pair.first;
        }

        T & GetValue()
        {
            return pair.second;
        }

        const T & GetValue() const
        {
            return pair.second;
        }

        int GetLevel() const
        {
            if (IsRightChild())
                return 1;
            else
                return GetParent()->GetLevel() + 1;
        }

        bool IsRed() const
        {
            return color == __RBT_RED;
        }

        bool IsBlack() const
        {
            return color == __RBT_BLACK;
        }

        int GetColor() const
        {
            return color;
        }

        _pair * GetPair()
        {
            return &pair;
        }

        const _pair * GetPair() const
        {
            return &pair;
        }

    protected:
        _node *  parent;
        _node *  left;
        _node *  right;
        _pair    pair;
        int      color;
    };

public:
    class ConstIterator
    {
        friend class Map<Key, T, Less>;

        ConstIterator(const Map * m, const _node * node, bool e)
            : creator(m), node(node), endof(e)
        {
        }

        ConstIterator(const Map * m, const bool e)
            : creator(m), node(NULL), endof(e)
        {
        }

    public:

        ConstIterator() : creator(NULL), node(NULL), endof(FALSE) {}
        ~ConstIterator() {}

        const _pair * operator ->() const
        {
            return node->GetPair();
        }

        ConstIterator & operator ++()
        {
            assert (node);
            _inc();
            return *this;
        }

        ConstIterator operator ++(int)
        {
            ConstIterator iter = *this;
            _inc();
            return iter;
        }

        ConstIterator & operator --()
        {
            assert (node);
            _dec();
            return *this;
        }

        ConstIterator operator --(int)
        {
            ConstIterator iter = *this;
            dec();
            return iter;
        }

        ConstIterator & operator =(const ConstIterator & r)
        {
            creator = r.creator;
            node = r.node;
            endof = r.endof;
            return *this;
        }

        ConstIterator(const ConstIterator & r)
        {
            creator = r.creator;
            node = r.node;
            endof = r.endof;
        }

        bool operator ==(const ConstIterator & r)
        {
#ifdef _DEBUG
            return node == r.node && creator == r.creator;
#else
            return node == r.node;
#endif
        }

        bool operator !=(const ConstIterator & r)
        {
#ifdef _DEBUG
            return node != r.node && creator == r.creator;
#else
            return node != r.node;
#endif
        }

    protected:

        void _begin()
        {
            node = _minimum(creator->_Root());
            endof = node == NULL;
        }

        void _end()
        {
            node = _maximum(creator->_Root());
            endof = node == NULL;
        }

        static const _node * _minimum(const _node * n)
        {
            while (n->GetLeftChild())
                n = n->GetLeftChild();

            return n;
        }

        static const _node * _maximum(const _node * n)
        {
            while (n->GetRightChild())
                n = n->GetRightChild();

            return n;
        }

        static _node * _successor(const _node * n) 
        {
            assert (n);
            return _minimum(n);
        }

        static bool _is_left_child(const _node * n)
        {
            return n->GetParent() && n->GetParent()->GetLeftChild() == n;
        }

        static bool _is_right_child(const _node * n)
        {
            return n->GetParent() && n->GetParent()->GetRightChild() == n;
        }

        void _inc()
        {
            if (node->GetRightChild())
            {
                node = _minimum(node->GetRightChild());
            }
            else if (_is_left_child(node))
            {
                node = node->GetParent();
            }
            else
            {
                while (_is_right_child(node))
                    node = node->GetParent();

                node = node->GetParent();
            }

            if (!node)
                endof = TRUE;
        }

        void _dec()
        {
            if (endof)
            {
                assert (!node && creator);
                node = creator->mRoot;
                _end();
            }

            else if (node->GetLeftChild())
            {
                node = _maximum(node->GetLeftChild());
            }
            else if (_is_right_child(node))
            {
                node = node->GetParent();
            }
            else
            {
                while (_is_left_child(node))
                    node = node->GetParent();

                node = node->GetParent();
            }
        }

    protected:
        const _node * node;
        const Map *   creator;
        bool          endof;
    };

public:
    class Iterator
    {
        friend class Map<Key, T, Less>;

        Iterator(Map * m, _node * node, bool e)
            : creator(m), node(node), endof(e)
        {
        }

        Iterator(Map * m, bool e)
            : creator(m), node(NULL), endof(e)
        {
        }

    public:

        Iterator() : creator(NULL), node(NULL), endof(FALSE) {}
        ~Iterator() {}

        _pair * operator ->()
        {
            return node->GetPair();
        }

        Iterator & operator ++()
        {
            assert (node);
            _inc();
            return *this;
        }

        Iterator operator ++(int)
        {
            Iterator iter = *this;
            _inc();
            return iter;
        }

        Iterator & operator --()
        {
            assert (node);
            _dec();
            return *this;
        }

        Iterator operator --(int)
        {
            Iterator iter = *this;
            dec();
            return iter;
        }

        Iterator & operator =(const Iterator & r)
        {
            creator = r.creator;
            node = r.node;
            endof = r.endof;
            return *this;
        }

        Iterator(const Iterator & r)
        {
            creator = r.creator;
            node = r.node;
            endof = r.endof;
        }

        bool operator ==(const Iterator & r) const
        {
#ifdef _DEBUG
            return node == r.node && creator == r.creator;
#else
            return node == r.node;
#endif
        }

        bool operator !=(const Iterator & r) const
        {
#ifdef _DEBUG
            return node != r.node && creator == r.creator;
#else
            return node != r.node;
#endif
        }

    protected:

        void _begin()
        {
            node = _minimum(creator->_Root());
            endof = node == NULL;
        }

        void _end()
        {
            node = _maximum(creator->_Root());
            endof = node == NULL;
        }

        static _node * _minimum(_node * n)
        {
            while (n && n->GetLeftChild())
                n = n->GetLeftChild();

            return n;
        }

        static _node * _maximum(_node * n)
        {
            while (n && n->GetRightChild())
                n = n->GetRightChild();

            return n;
        }

        static _node * _successor(_node * n)
        {
            assert (n);
            return _minimum(n);
        }

        static bool _is_left_child(_node * n)
        {
            return n->GetParent() && n->GetParent()->GetLeftChild() == n;
        }

        static bool _is_right_child(_node * n)
        {
            return n->GetParent() && n->GetParent()->GetRightChild() == n;
        }

        void _inc()
        {
            if (node->GetRightChild())
            {
                node = _minimum(node->GetRightChild());
            }
            else if (_is_left_child(node))
            {
                node = node->GetParent();
            }
            else
            {
                while (_is_right_child(node))
                    node = node->GetParent();

                node = node->GetParent();
            }

            if (!node)
                endof = TRUE;
        }

        void _dec()
        {
            if (endof)
            {
                assert (!node && creator);
                node = creator->mRoot;
                _end();
            }

            else if (node->GetLeftChild())
            {
                node = _maximum(node->GetLeftChild());
            }
            else if (_is_right_child(node))
            {
                node = node->GetParent();
            }
            else
            {
                while (_is_left_child(node))
                    node = node->GetParent();

                node = node->GetParent();
            }
        }

    protected:
        _node *     node;
        Map *       creator;
        bool        endof;
    };

public:
                Map();
                ~Map();

    void Clear();
    int Size() const;
    bool Empty() const;

    ConstIterator    Begin() const;
    ConstIterator    End() const;
    ConstIterator    Find(const Key & k) const;

    Iterator    Begin();
    Iterator    End();
    Iterator    Find(const Key & k);

    Iterator    Insert(const Key & k, const T & v);
    Iterator    Erase(Iterator iter);
    Iterator    Erase(const Key & k);

    T &         operator[](const Key & k);
    const T &   operator[](const Key & k) const;

protected:
    void        _Insert(_node * node);
    void        _SetRoot(_node * node);
    void        _RotateLeft(_node * node);
    void        _RotateRight(_node * node);

    _node *     _Root() const { return mRoot; }

private:
    explicit    Map(const Map & m);
    Map &       operator=(const Map & m);

protected:
    _node *                     mRoot;
    
    Less                        mLess;

    Allocator<_node, 128>       mAlloc;
};

#include "MWMap.inl"

}