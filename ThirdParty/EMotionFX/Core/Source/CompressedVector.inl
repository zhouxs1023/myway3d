/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */


// constructor
template <class VectorType, class StorageType>
inline TCompressedVector3<VectorType, StorageType>::TCompressedVector3()
{ 
}


// constructor
template <class VectorType, class StorageType>
inline TCompressedVector3<VectorType, StorageType>::TCompressedVector3(const TVector3<VectorType>& vec, const VectorType minValue, const VectorType maxValue)
{
	// TODO: make sure due to rounding/floating point errors the result is not negative?
	const StorageType f = ((VectorType)1 / (maxValue - minValue)) * CONVERT_VALUE;
	mX = (vec.x - minValue) * f;
	mY = (vec.y - minValue) * f; 
	mZ = (vec.z - minValue) * f;
}


// create from a Vector3
template <class VectorType, class StorageType>
inline void TCompressedVector3<VectorType, StorageType>::FromVector3(const TVector3<VectorType>& vec, const VectorType minValue, const VectorType maxValue)
{
	// TODO: make sure due to rounding/floating point errors the result is not negative?
	const StorageType f = ((VectorType)1 / (maxValue - minValue)) * CONVERT_VALUE;
	mX = (vec.x - minValue) * f;
	mY = (vec.y - minValue) * f;
	mZ = (vec.z - minValue) * f;
}


// uncompress into a Vector3
template <class VectorType, class StorageType>
inline void TCompressedVector3<VectorType, StorageType>::UnCompress(TVector3<VectorType>* output, const VectorType minValue, const VectorType maxValue) const
{
	// unpack and normalize
	const VectorType f = ((VectorType)1 / (VectorType)CONVERT_VALUE) * (maxValue - minValue);
	output->mX = ((VectorType)mX * f) + minValue;
	output->mY = ((VectorType)mY * f) + minValue;
	output->mZ = ((VectorType)mZ * f) + minValue;
}


// convert to a Vector3
template <class VectorType, class StorageType>
inline TVector3<VectorType> TCompressedVector3<VectorType, StorageType>::ToVector3(const VectorType minValue, const VectorType maxValue) const
{
	const VectorType f = ((VectorType)1 / (VectorType)CONVERT_VALUE) * (maxValue - minValue);
	return TVector3<VectorType>((mX*f)+minValue, (mY*f)+minValue, (mZ*f)+minValue);
}