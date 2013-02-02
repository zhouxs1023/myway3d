/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "LinkedList.h"


template <class T>
ListNode<T>::ListNode(const T& _object, ListNode<T>* _prev, ListNode<T>* _next) : data(_object), prev(_prev), next(_next)
{
}

template <class T>
ListNode<T>::~ListNode()
{

}


template <class T>
LinkedList<T>::LinkedList() : mSize(0)
{
	mHead = new ListNode<T>();
	mTail = new ListNode<T>();
	mHead->next = mTail;
	mTail->prev = mHead;
}

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& list) : mSize(0)
{
	mHead = new ListNode<T>();
	mTail = new ListNode<T>();
	mHead->next = mTail;
	mTail->prev = mHead;

    *this = list;
}

template <class T>
LinkedList<T>::~LinkedList()
{
	MakeEmpty();

	delete mHead;
	mHead = 0;

	delete mTail;
	mTail = 0;
}

template <class T>
const LinkedList<T>& LinkedList<T>::operator=(const LinkedList& list)
{
	if(this == &list)
		return *this;

	MakeEmpty();

	for(ConstIterator iterator = list.Begin(); iterator != list.End(); ++iterator)
		PushBack(*iterator);

	return *this;
}

template <class T>
inline LISTSIZETYPE LinkedList<T>::Size() const
{
	return mSize;
}

template <class T>
inline LISTSIZETYPE LinkedList<T>::GetSize() const
{
	return mSize;
}

template <class T>
inline LISTSIZETYPE LinkedList<T>::MaxSize() const
{
	return (LISTSIZETYPE)std::numeric_limits< LISTSIZETYPE >::infinity();
}

template <class T>
inline bool LinkedList<T>::IsEmpty() const
{
	return Size() == 0;
}

template <class T>
inline void LinkedList<T>::MakeEmpty()
{
	while(!IsEmpty())
		PopFront();
}

template <class T>
inline void LinkedList<T>::Clear()
{
	while(!IsEmpty())
		PopFront();
}

template <class T>
inline ListIterator<T> LinkedList<T>::Begin()
{
	Iterator iterator(*this, mHead);

	return ++iterator;
}

template <class T>
inline ConstListIterator<T> LinkedList<T>::Begin() const
{
	ConstIterator iterator(*this, mHead);

	return ++iterator;
}

template <class T>
inline ListIterator<T> LinkedList<T>::End()
{
	return Iterator(*this, mTail);
}


template <class T>
inline ConstListIterator<T> LinkedList<T>::End() const
{
	return ConstIterator(*this, mTail);
}

template <class T>
inline T& LinkedList<T>::Front()
{
	return *Begin();
}

template <class T>
inline const T& LinkedList<T>::Front() const
{
	return *Begin();
}

template <class T>
inline T& LinkedList<T>::Back()
{
	return *--End();
}

template <class T>
inline const T& LinkedList<T>::Back() const
{
    return *--End();
}

template <class T>
inline void LinkedList<T>::PushFront(const T& x)
{
	Insert(Begin(), x);
}

template <class T>
inline void LinkedList<T>::PushBack(const T& x)
{
	Insert(End(), x);
}

template <class T>
inline void LinkedList<T>::PopFront()
{
	Erase(Begin());
}


template <class T>
inline void LinkedList<T>::PopBack()
{
	Erase(--End());
}


template <class T>
inline void LinkedList<T>::RemoveFirst()
{
	Erase(Begin());
}


template <class T>
inline void LinkedList<T>::RemoveLast()
{
	Erase(--End());
}


template <class T>
ListIterator<T> LinkedList<T>::Insert(Iterator iterator, const T& x)
{
	DECLARE_FUNCTION(Insert)

	iterator.IsValid();

	if( iterator.mHead != mHead )
		throw Exception("Insert iterator not in this linked list", HERE);

	ListNode<T>* pNode = iterator.mCurrent;
	pNode->prev->next = new ListNode<T>(x, pNode->prev, pNode);
	pNode->prev = pNode->prev->next;

	mSize++;

	return Iterator(*this, pNode->prev);
}

template <class T>
ListIterator<T> LinkedList<T>::Erase(Iterator iterator)
{
	DECLARE_FUNCTION(Erase)

	iterator.IsValid();
	if(iterator == End())
		throw Exception("Cannot erase object at End()", HERE);
	if(iterator.mHead != mHead)
		throw Exception("Erase iterator not in this linked list", HERE);

	ListNode<T> *pNode = iterator.mCurrent;
	Iterator retVal(*this, pNode->next);
	pNode->prev->next = pNode->next;
	pNode->next->prev = pNode->prev;
	delete pNode;

	mSize--;

	return retVal;
}

template <class T>
inline ListIterator<T> LinkedList<T>::Erase(const Iterator& from, Iterator to)
{
	for(Iterator iterator = from; iterator != to; )
		iterator = Erase(iterator);

	return to;
}

template <class T>
inline void LinkedList<T>::Remove(const T& object)
{
	for(ListIterator<T> iterator = Begin(); iterator != End(); iterator++)
	{
		if(*iterator == object)
		{
			iterator = Erase(iterator);
		}
	}
}

template <class T>
ListIterator<T> LinkedList<T>::Find(const T& object)
{
	DECLARE_FUNCTION(Find)

	for(ListIterator<T> iterator = Begin(); iterator != End(); iterator++)
	{
		if(*iterator == object)
			return iterator;
	}

	return ListIterator<T>();
}

template <class T>
ConstListIterator<T> LinkedList<T>::Find(const T& object) const
{
	DECLARE_FUNCTION(Find)

	for(ConstListIterator<T> iterator = Begin(); iterator != End(); iterator++)
	{
		if(*iterator == object)
			return iterator;
	}

	return ConstListIterator<T>();
}


template <class T>
ConstListIterator<T>::ConstListIterator() : mHead(0), mCurrent(0)
{

}

template <class T>
ConstListIterator<T>::ConstListIterator(const ConstListIterator<T>& iterator)
: mHead(iterator.mHead), mCurrent(iterator.mCurrent)
{

}

template <class T>
ConstListIterator<T>::~ConstListIterator()
{

}


template <class T>
inline ConstListIterator<T>& ConstListIterator<T>::operator=(const ConstListIterator<T>& iterator)
{
	mHead = iterator.mHead;
	mCurrent = iterator.mCurrent;

	return *this;
}


template <class T>
inline bool ConstListIterator<T>::operator==(const ConstListIterator& iterator) const
{
	return mCurrent == iterator.mCurrent;
}

template <class T>
inline bool ConstListIterator<T>::operator!=(const ConstListIterator& iterator) const
{
	return !(*this == iterator);
}


template <class T>
inline const T& ConstListIterator<T>::operator*() const
{
    return Retrieve();
}

template <class T>
T& ConstListIterator<T>::Retrieve() const
{
	DECLARE_FUNCTION(Retrieve)

	IsValid();
	if(mCurrent->next == 0)
		throw Exception("Cannot perform *end() in linked list", HERE);

	return mCurrent->data;
}


template <class T>
inline ConstListIterator<T>& ConstListIterator<T>::Advance()
{
	CanAdvance();
	mCurrent = mCurrent->next;

	return *this;
}

template <class T>
inline ConstListIterator<T>& ConstListIterator<T>::operator++()
{
	CanAdvance();
	mCurrent = mCurrent->next;

	return *this;
}

template <class T>
inline ConstListIterator<T> ConstListIterator<T>::operator++(int)
{
	ConstListIterator<T> old = *this;
	++(*this);

	return old;
}

template <class T>
ConstListIterator<T>& ConstListIterator<T>::operator+=(int steps)
{
	for(int i=0; i<steps; i++)
	{
		this->operator++();
	}

	return *this;
}

template <class T>
inline ConstListIterator<T>& ConstListIterator<T>::operator--()
{
	CanRetreat();
	mCurrent = mCurrent->prev;

	return *this;
}

template <class T>
inline ConstListIterator<T> ConstListIterator<T>::operator--(int)
{
	ConstListIterator<T> old = *this;
	--(*this);

	return old;
}

template <class T>
ConstListIterator<T>& ConstListIterator<T>::operator-=(int steps)
{
	for(int i=0; i<steps; i++)
	{
		this->operator--();
	}

	return *this;
}


template <class T>
inline bool ConstListIterator<T>::IsInitialized() const
{
	DECLARE_FUNCTION(IsInitialized)

	if(mHead == 0 || mCurrent == 0)
		return false;
	//	throw Exception("Linked list iterator not initialized", HERE);

	return true;
}

template <class T>
inline bool ConstListIterator<T>::IsValid() const
{
	DECLARE_FUNCTION(IsValid)

    IsInitialized();
	if(mCurrent == mHead)
		return false;
	//	throw Exception("At position prior to begin() in linked list, iterator is not valid", HERE);

	return true;
}

template <class T>
inline bool ConstListIterator<T>::CanAdvance() const
{
	DECLARE_FUNCTION(CanAdvance)

    IsInitialized();
	if(mCurrent->next == 0)
		return false;
	//	throw Exception("Cannot perform ++end() in linked list", HERE);

	return true;
}

template <class T>
inline bool ConstListIterator<T>::CanRetreat() const
{
	DECLARE_FUNCTION(CanRetreat)

    IsValid();
	if(mCurrent->prev == mHead)
		return false;
	//	throw Exception("Cannot perform --begin() in linked list", HERE);

	return true;
}


template <class T>
ConstListIterator<T>::ConstListIterator(const LinkedList<T>& source, ListNode<T>* node)
: mHead(source.mHead), mCurrent(node)
{

}


template <class T>
ListIterator<T>::ListIterator()
{

}

template <class T>
ListIterator<T>::~ListIterator()
{

}


template <class T>
inline const T& ListIterator<T>::operator*() const
{
	return ConstListIterator<T>::operator*();
}

template <class T>
inline T& ListIterator<T>::operator*()
{
	return Retrieve();
}


template <class T>
inline ListIterator<T>& ListIterator<T>::Advance()
{
	CanAdvance();
	mCurrent = mCurrent->next;

	return *this;
}

template <class T>
inline ListIterator<T>& ListIterator<T>::operator++()
{
	CanAdvance();
	mCurrent = mCurrent->next;

	return *this;
}

template <class T>
inline ListIterator<T> ListIterator<T>::operator++(int)
{
	ListIterator<T> old = *this;
	++(*this);

	return old;
}

template <class T>
ListIterator<T>& ListIterator<T>::operator+=(int steps)
{
	for(int i=0; i<steps; i++)
	{
		this->operator++();
	}

	return *this;
}

template <class T>
inline ListIterator<T>& ListIterator<T>::operator--()
{
	CanRetreat();
	mCurrent = mCurrent->prev;

	return *this;
}


template <class T>
inline ListIterator<T> ListIterator<T>::operator--(int)
{
	ListIterator<T> old = *this;
	--(*this);

	return old;
}

template <class T>
ListIterator<T>& ListIterator<T>::operator-=(int steps)
{
	for(int i=0; i<steps; i++)
	{
		this->operator--();
	}

	return *this;
}

template <class T>
ListIterator<T>::ListIterator(const LinkedList<T>& source, ListNode<T>* node) : ConstListIterator<T>(source, node)
{

}