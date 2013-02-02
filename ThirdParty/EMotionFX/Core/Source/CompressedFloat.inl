/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "CompressedFloat.h"


// constructor
template <class FloatType, class StorageType>
inline TCompressedFloat<FloatType, StorageType>::TCompressedFloat()
{ 
}


// constructor
template <class FloatType, class StorageType>
inline TCompressedFloat<FloatType, StorageType>::TCompressedFloat(const FloatType value, const FloatType minValue, const FloatType maxValue)
{
	// TODO: make sure due to rounding/floating point errors the result is not negative?
	const StorageType f = ((FloatType)1 / (maxValue - minValue)) * CONVERT_VALUE;
	mValue = (value - minValue) * f;
}


// create from a floating point value
template <class FloatType, class StorageType>
inline void TCompressedFloat<FloatType, StorageType>::FromFloat(const FloatType value, const FloatType minValue, const FloatType maxValue)
{
	// TODO: make sure due to rounding/floating point errors the result is not negative?
	const StorageType f = ((FloatType)1 / (maxValue - minValue)) * CONVERT_VALUE;
	mValue = (value - minValue) * f;
}


// uncompress into a floating point value
template <class FloatType, class StorageType>
inline void TCompressedFloat<FloatType, StorageType>::UnCompress(FloatType* output, const FloatType minValue, const FloatType maxValue) const
{
	// unpack and normalize
	const FloatType f = ((FloatType)1 / (FloatType)CONVERT_VALUE) * (maxValue - minValue);
	output->mValue = ((FloatType)mValue * f) + minValue;
}


// convert to a floating point value
template <class FloatType, class StorageType>
inline FloatType TCompressedFloat<FloatType, StorageType>::ToFloat(const FloatType minValue, const FloatType maxValue) const
{
	const FloatType f = ((FloatType)1 / (FloatType)CONVERT_VALUE) * (maxValue - minValue);
	return ((mValue*f) + minValue);
}