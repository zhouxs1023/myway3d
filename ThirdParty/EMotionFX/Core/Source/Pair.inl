/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Pair.h"


template<class T, class U>
Pair<T, U>::Pair()
{
}


template<class T, class U>
Pair<T, U>::Pair(const T& first, const U& second)
: mFirst(first), mSecond(second)
{
}


template<class T, class U>
Pair<T, U>::Pair(const Pair<T, U>& pair)
: mFirst(pair.mFirst), mSecond(pair.mSecond)
{
}


template<class T, class U>
Pair<T, U>::~Pair()
{
}


template<class T, class U>
Pair<T, U>& Pair<T, U>::operator=(const Pair<T, U>& pair)
{
	if(this == &pair)
		return *this;

	mFirst = pair.mFirst;
	mSecond = pair.mSecond;

	return *this;
}


template<class T, class U>
bool Pair<T, U>::operator==(const Pair& pair) const
{
	return (mFirst == pair.mFirst) && (mSecond == pair.mSecond);
}

template<class T, class U>
bool Pair<T, U>::operator!=(const Pair& pair) const
{
	return (mFirst != pair.mFirst) || (mSecond != pair.mSecond);
}

template<class T, class U>
bool Pair<T, U>::operator<(const Pair& pair) const
{
	return (mFirst < pair.mFirst) || (mFirst == pair.mFirst && mSecond < pair.mSecond);
}


template<class T, class U>
void Pair<T, U>::SetFirst(const T& first)
{
	mFirst = first;
}

template<class T, class U>
void Pair<T, U>::SetSecond(const U& second)
{
	mSecond = second;
}


template<class T, class U>
const T& Pair<T, U>::GetFirst() const
{
	return mFirst;
}

template<class T, class U>
T& Pair<T, U>::GetFirst()
{
	return mFirst;
}

template<class T, class U>
const U& Pair<T, U>::GetSecond() const
{
	return mSecond;
}

template<class T, class U>
U& Pair<T, U>::GetSecond()
{
	return mSecond;
}