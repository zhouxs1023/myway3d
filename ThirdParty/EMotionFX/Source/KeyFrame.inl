/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "KeyFrame.h"


// default constructor
template <class ReturnType, class StorageType>
KeyFrame<ReturnType, StorageType>::KeyFrame() : mTime(0)
{
}


// extended constructor
template <class ReturnType, class StorageType>
KeyFrame<ReturnType, StorageType>::KeyFrame(const float time, const ReturnType& value)	: mTime(time)
{
	SetValue(value);
}


// destructor
template <class ReturnType, class StorageType>
KeyFrame<ReturnType, StorageType>::~KeyFrame()
{
}


template <class ReturnType, class StorageType>
inline float KeyFrame<ReturnType, StorageType>::GetTime() const
{
	return mTime;
}


template <class ReturnType, class StorageType>
inline ReturnType KeyFrame<ReturnType, StorageType>::GetValue() const
{
	return mValue;
}


template <class ReturnType, class StorageType>
inline void KeyFrame<ReturnType, StorageType>::SetTime(const float time)
{
	mTime = time;
}


template <class ReturnType, class StorageType>
inline void KeyFrame<ReturnType, StorageType>::SetValue(const ReturnType& value)
{
	mValue= value;
}
