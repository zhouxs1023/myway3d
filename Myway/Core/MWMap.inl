

template <class Key, class T, class Less>
Map<Key, T, Less>::Map()
: mRoot(NULL)
{
}


template <class Key, class T, class Less>
Map<Key, T, Less>::~Map()
{
}

template <class Key, class T, class Less>
void Map<Key, T, Less>::Clear()
{
    Iterator iter = Begin();
    Iterator end = End();

    while (iter != end)
    {
        Iterator erase = iter++;
        mAlloc.Free(erase.node);
    }

    mRoot = NULL;
}

template <class Key, class T, class Less>
int Map<Key, T, Less>::Size() const
{
    return mAlloc->GetActiveCount();
}

template <class Key, class T, class Less>
bool Map<Key, T, Less>::Empty() const
{
    return Size() == 0;
}

template <class Key, class T, class Less>
T & Map<Key, T, Less>::operator[](const Key & k)
{
    Iterator iter = Find(k);

    if (iter == End())
    {
        iter = Insert(k, T());
    }

    return iter->second;
}

template <class Key, class T, class Less>
const T & Map<Key, T, Less>::operator[](const Key & k) const
{
    Iterator iter = Find(k);

    if (iter == End())
    {
        iter = Insert(k, T());
    }

    return iter->second;
}

template <class Key, class T, class Less>
typename Map<Key, T, Less>::ConstIterator Map<Key, T, Less>::Begin() const
{
    ConstIterator beg(this, FALSE);
    beg._begin();
    return beg;
}

template <class Key, class T, class Less>
typename Map<Key, T, Less>::ConstIterator Map<Key, T, Less>::End() const
{
    ConstIterator end(this, TRUE);
    return end;
}

template <class Key, class T, class Less>
typename Map<Key, T, Less>::ConstIterator Map<Key, T, Less>::Find(const Key & k) const
{
    _node * node = mRoot;

    while (node)
    {
        if (mLess(k, node->GetKey()))
            node = node->GetLeftChild();
        else if (mLess(node->GetKey(), k))
            node = node->GetRightChild();
        else 
            break;
    }

    return ConstIterator(this, node, node == NULL);
}

template <class Key, class T, class Less>
typename Map<Key, T, Less>::Iterator Map<Key, T, Less>::Begin()
{
    Iterator beg(this, FALSE);
    beg._begin();
    return beg;
}

template <class Key, class T, class Less>
typename Map<Key, T, Less>::Iterator Map<Key, T, Less>::End()
{
    Iterator end(this, TRUE);
    return end;
}

template <class Key, class T, class Less>
typename Map<Key, T, Less>::Iterator Map<Key, T, Less>::Find(const Key & k)
{
    _node * node = mRoot;

    while (node)
    {
        if (mLess(k, node->GetKey()))
            node = node->GetLeftChild();
        else if (mLess(node->GetKey(), k))
            node = node->GetRightChild();
        else 
            break;
    }
    
    return Iterator(this, node, node == NULL);
}

template <class Key, class T, class Less>
typename Map<Key, T, Less>::Iterator Map<Key, T, Less>::Insert(const Key & k, const T & v)
{
    _node * node = mAlloc.Alloc();

	node->Init();

    node->SetPair(k, v);

    _Insert(node);

    Iterator iter;
    iter.node = node;

    while (!node->IsRoot() && (node->GetParent()->IsRed()))
    {
        if (node->GetParent()->IsLeftChild())
        {
            _node * uncle = node->GetParent()->GetParent()->GetRightChild();
            if (uncle && uncle->IsRed())
            {
                node->GetParent()->SetBlack();
                uncle->SetBlack();
                node->GetParent()->GetParent()->SetRed();
                node = node->GetParent()->GetParent();
            }
            else
            {
                if (node->IsRightChild())
                {
                    node = node->GetParent();
                    _RotateLeft(node);
                }

                node->GetParent()->SetBlack();
                node->GetParent()->GetParent()->SetRed();
                _RotateRight(node->GetParent()->GetParent());
            }
        }
        else
        {
            _node * uncle = node->GetParent()->GetParent()->GetLeftChild();
            if (uncle && uncle->IsRed())
            {
                node->GetParent()->SetBlack();
                uncle->SetBlack();
                node->GetParent()->GetParent()->SetRed();
                node = node->GetParent()->GetParent();
            }
            else
            {
                if (node->IsLeftChild())
                {
                    node = node->GetParent();
                    _RotateRight(node);
                }

                node->GetParent()->SetBlack();
                node->GetParent()->GetParent()->SetRed();
                _RotateLeft(node->GetParent()->GetParent());
            }
        }
    }

    mRoot->SetBlack();

    return iter;
}


template <class Key, class T, class Less>
void Map<Key, T, Less>::_Insert(typename Map<Key, T, Less>::_node * newnode)
{
    if (mRoot == NULL)
    {
        mRoot = newnode;
        newnode->SetBlack();
    }
    else
    {
        _node * node = mRoot;

        while (node)
        {
            if (mLess(newnode->GetKey(), node->GetKey()))
            {
                if (node->GetLeftChild())
                    node = node->GetLeftChild();
                else
                {
                    node->SetLeftChild(newnode);
                    node = NULL;
                }
            }
            else if (mLess(node->GetKey(), newnode->GetKey()))
            {
                if (node->GetRightChild())
                    node = node->GetRightChild();
                else
                {
                    node->SetRightChild(newnode);
                    node = NULL;
                }
            }
            else
            {
                assert(0);
                node = NULL;
            }
        }
    }

    mRoot->SetBlack();
}

template <class Key, class T, class Less>
typename Map<Key, T, Less>::Iterator Map<Key, T, Less>::Erase(typename Map<Key, T, Less>::Iterator whr)
{
    assert (whr.creator == this && whr.endof == FALSE && whr.node != NULL);

    _node * enode = whr.node;
    ++whr;

    while (enode->GetRightChild())
        _RotateLeft(enode);

    _node * left = enode->GetLeftChild();

    if (enode->IsLeftChild())
        enode->GetParent()->SetLeftChild(left);
    else if (enode->IsRightChild())
        enode->GetParent()->SetRightChild(left);
    else
        _SetRoot(left);
    
    mAlloc.Free(enode);

    return whr;
}

template <class Key, class T, class Less>
typename Map<Key, T, Less>::Iterator Map<Key, T, Less>::Erase(const Key & k)
{
    Erase(Find(k));
}

template <class Key, class T, class Less>
void Map<Key, T, Less>::_SetRoot(typename Map<Key, T, Less>::_node * node)
{
    mRoot = node;
    if (mRoot)
    {
        mRoot->SetParent(NULL);
        mRoot->SetBlack();
    }
}

template <class Key, class T, class Less>
void Map<Key, T, Less>::_RotateLeft(typename Map<Key, T, Less>::_node * node)
{
    _node * right = node->GetRightChild();

    node->SetRightChild(right->GetLeftChild());

    if (node->IsLeftChild())
        node->GetParent()->SetLeftChild(right);
    else if (node->IsRightChild())
        node->GetParent()->SetRightChild(right);
    else
        _SetRoot(right);

    right->SetLeftChild(node);
}

template <class Key, class T, class Less>
void Map<Key, T, Less>::_RotateRight(typename Map<Key, T, Less>::_node * node)
{
    _node * left = node->GetLeftChild();

    node->SetLeftChild(left->GetRightChild());

    if (node->IsLeftChild())
        node->GetParent()->SetLeftChild(left);
    else if (node->IsRightChild())
        node->GetParent()->SetRightChild(left);
    else
        _SetRoot(left);

    left->SetRightChild(node);
}
