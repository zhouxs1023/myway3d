


template <class Key, class Type>
inline Tree<Key, Type>::Tree()
: m_Root(NULL)
{
}

template <class Key, class Type>
inline Tree<Key, Type>::~Tree()
{
    Clear();
}

template <class Key, class Type>
inline void Tree<Key, Type>::Insert(Key key, Type val)
{
    assert (Find(key) != End());

    if (m_Root)
    {
        _node * node = m_Root;

        while (1)
        {
            if (key < node->key)
            {
                if (node->left)
                    node = node->left;
                else
                {
                    node * newnode = AllocNode();
                    newnode->key = key;
                    newnode->val = val;
                    newnode->parent = node;
                    node->left = newnode;
                    break;
                }
            }
            else
            {
                if (node->right)
                    node = node->right;
                else
                {
                    node * newnode = AllocNode();
                    newnode->key = key;
                    newnode->val = val;
                    newnode->parent = node;
                    node->right = newnode;
                    break;
                }
            }
        }
    }
    else
    {
        m_Root = AllocNode();
        m_Root->key = key;
        m_Root->val = val;
    }
}

template <class Key, class Type>
inline typename Tree<Key, Type>::Iterator Tree<Key, Type>::Erase(typename Tree<Key, Type>::Iterator iter)
{
    assert(iter != End());

    Iterator ret = iter;
    ++ret;

    _node * parent = iter.node->parent;

    if (parent)
    {
        _node * node = iter.node->left;
        if (node)
        {
            while (node->right)
                node = node->right;

            node->right = iter->right;
            parent->left = node->left;
        }
        else
        {
            parent->left = node->right;
        }
    }
    else
    {
        _node * node = iter.node->left;
        if (node)
        {
            m_Root = node;

            while (node->right)
                node = node->right;

            node->right = iter->right;
        }
        else
        {
            m_Root = node->right;
        }

        m_Root->parent = NULL;
    }

    m_Alloc.Free(iter.node);

    return ret;
}

template <class Key, class Type>
inline typename Tree<Key, Type>::Iterator Tree<Key, Type>::Begin() const
{
    Iterator iter;

    iter.node = m_Root;
    __CLEARNIL(iter.node->flag);

    if (iter.node->left)
        __CLEARNIL(iter.node->left);

    if (iter.node->right)
        __CLEARNIL(iter.node->right);

    return iter;
}

template <class Key, class Type>
inline typename Tree<Key, Type>::Iterator Tree<Key, Type>::End() const
{
    Iterator iter;

    iter.node = NULL;

    return iter;
}

template <class Key, class Type>
inline typename  Tree<Key, Type>::Iterator Tree<Key, Type>::Find(Key key)
{
    _node * node = m_Root;

    while (node)
    {
        if (key < node->key)
            node = node->left;
        else if (key > node->key)
            node = node->left;
        else
            break;
    }

    Iterator iter;

    iter.node = node;

    if (node)
    {
        __CLEARNIL(node->flag);

        if (iter.node->left)
            __CLEARNIL(node->left);

        if (iter.node->right)
            __CLEARNIL(node->right);
    }

    return iter;
}

template <class Key, class Type>
inline int Tree<Key, Type>::Size() const
{
    return m_Alloc.GetActiveCount();
}

template <class Key, class Type>
inline bool Tree<Key, Type>::Empty() const
{
    return m_Alloc.GetActiveCount() == 0;
}


template <class Key, class Type>
void Tree<Key, Type>::Clear()
{
    m_Root = NULL;
    m_Alloc.Shutdown();
}

template <class Key, class Type>
inline typename Tree<Key, Type>::_node * Tree<Key, Type>::AllocNode()
{
    _node * node = m_Alloc.Alloc();
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->flag = 0;
}