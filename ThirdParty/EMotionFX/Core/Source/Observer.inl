/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Observer.h"


template< class T >
Observer< T >::Observer()
{

}

template< class T >
Observer< T >::~Observer()
{

}


template< class T >
Subject< T >::Subject()
{

}

template< class T >
Subject< T >::~Subject()
{
	mObservers.Clear();
}


template< class T >
void Subject< T >::AddObserver(const Observer< T >& observer)
{
	mObservers.PushFront(observer);
}

template< class T >
void Subject< T >::RemoveObserver(const Observer< T >* observer)
{
	mObservers.Remove(observer);
}


template< class T >
void Subject< T >::NotifyAll(const T& observee, float timeDelta)
{
	for(LinkedList< Observer<T>* >::Iterator iterator = Begin(); iterator != End(); iterator++)
	{
		(*iterator).Update(observee, timeDelta);
	}
}