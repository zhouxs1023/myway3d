/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */


// constructor
template <class QuatType, class StorageType>
inline TCompressedQuaternion<QuatType, StorageType>::TCompressedQuaternion() : mX(0), mY(0), mZ(0), mW(CONVERT_VALUE)	
{ 
}


// constructor
template <class QuatType, class StorageType>
inline TCompressedQuaternion<QuatType, StorageType>::TCompressedQuaternion(const TQuaternion<QuatType>& quat) : mX(quat.x*CONVERT_VALUE), mY(quat.y*CONVERT_VALUE), mZ(quat.z*CONVERT_VALUE), mW(quat.w*CONVERT_VALUE)
{
}


// create from a quaternion
template <class QuatType, class StorageType>
inline void TCompressedQuaternion<QuatType, StorageType>::FromQuaternion(const TQuaternion<QuatType>& quat)
{
	// pack it
	mX = quat.x * CONVERT_VALUE;
	mY = quat.y * CONVERT_VALUE;
	mZ = quat.z * CONVERT_VALUE;
	mW = quat.w * CONVERT_VALUE;
}


// uncompress into a quaternion
template <class QuatType, class StorageType>
inline void TCompressedQuaternion<QuatType, StorageType>::UnCompress(TQuaternion<QuatType>* output) const
{
	// unpack and normalize
	const QuatType f = (QuatType)1 / (QuatType)CONVERT_VALUE;
	output->mX = mX * f;
	output->mY = mY * f;
	output->mZ = mZ * f;
	output->mW = mW * f;
	//output->Normalize();
}


// convert to a quaternion
template <class QuatType, class StorageType>
inline TQuaternion<QuatType> TCompressedQuaternion<QuatType, StorageType>::ToQuaternion() const
{
	const QuatType f = (QuatType)1 / (QuatType)CONVERT_VALUE;
	return TQuaternion<QuatType>(mX*f, mY*f, mZ*f, mW*f);//.Normalize();
}
