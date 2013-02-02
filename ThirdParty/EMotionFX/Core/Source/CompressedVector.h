/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __COMPRESSEDVECTORS_H
#define __COMPRESSEDVECTORS_H

// include the required headers
#include "Macros.h"
#include "SourceDeclaration.h"
#include "Vector.h"
#include "Algorithms.h"


namespace MCore
{

/**
 * The compressed / packed vector class.
 * The class can automatically compress a vector which has components of type 'VectorType' into a vector that
 * represents these component values inside 'StorageType' component types. An example would be a Vector3 object
 * of floats (VectorType=float) compressed into a CompressedVector3 that stores the xyz components as 16-byte unsigned shorts.
 * This would reduce the Vector3 size from 12 bytes into 6 bytes, at the trade of a bit loss in precision and CPU overhead for
 * uncompression into a Vector3 of floats again. When used with care, the CPU overhead and precision loss are negligible.
 * During both compression and uncompression you also have to specify the range (minimum and maximum possible values) of the
 * components of the vector you want to compress/decompress. If you are dealing with normalized normals, the minimum value
 * would be -1, and the maximum value would be +1. When uncompressing (converting back to a Vector3 of floats) you
 * have to be sure you use the same minimum and maximum values as when you used to compress it! This is very important!
 * The bigger the range, the more precision loss. There is however no performance impact linked to the range.
 */
template <class VectorType, class StorageType>
class TCompressedVector3
{
	public:
		/**
		 * Default constructor.
		 * This leaves the members uninitialized, so if you get the uncompressed version, the result is unknown.
		 */
		inline TCompressedVector3();

		/**
		 * The constructor that sets the compressed values directly.
		 * @param x The compressed x component.
		 * @param y The compressed y component.
		 * @param z The compressed z component.
		 */
		inline TCompressedVector3(StorageType x, StorageType y, StorageType z) : mX(x), mY(y), mZ(z) {}

		/**
		 * Create a compressed vector from an uncompressed one.
		 * @param vec The vector you want to compress.
		 * @param minValue The minimum possible value of the xyz components of the uncompressed vector. So in case of a normalized normal, this would be -1.
		 * @param maxValue The maximum possible value of the xyz components of the uncompressed vector. So in case of a normalized normal, this would be +1.
		 */
		inline TCompressedVector3(const TVector3<VectorType>& vec, const VectorType minValue, const VectorType maxValue);

		/**
		 * Create a compressed vector from an uncompressed one.
		 * @param vec The vector you want to compress.
		 * @param minValue The minimum possible value of the xyz components of the uncompressed vector. So in case of a normalized normal, this would be -1.
		 * @param maxValue The maximum possible value of the xyz components of the uncompressed vector. So in case of a normalized normal, this would be +1.
		 */
		inline void FromVector3(const TVector3<VectorType>& vec, const VectorType minValue, const VectorType maxValue);

		/**
		 * Uncompress this compressed vector into an uncompressed Vector3 of floats.
		 * Please note that the minimum and maximum values you specify are the same as when you created this compressed vector3 or when you for the last
		 * time called the FromVector3 method.
		 * @param output The uncompressed vector to store the result in.
		 * @param minValue The minimum possible value of the xyz components of the uncompressed vector. So in case of a normalized normal, this would be -1.
		 * @param maxValue The maximum possible value of the xyz components of the uncompressed vector. So in case of a normalized normal, this would be +1.
		 */
		inline void UnCompress(TVector3<VectorType>* output, const VectorType minValue, const VectorType maxValue) const;

		/**
		 * Uncompress this compressed vector into an uncompressed Vector3 of floats.
		 * Please note that the minimum and maximum values you specify are the same as when you created this compressed vector3 or when you for the last
		 * time called the FromVector3 method.
		 * @param minValue The minimum possible value of the xyz components of the uncompressed vector. So in case of a normalized normal, this would be -1.
		 * @param maxValue The maximum possible value of the xyz components of the uncompressed vector. So in case of a normalized normal, this would be +1.
		 * @result The uncompressed version of this vector.
		 */
		inline TVector3<VectorType> ToVector3(const VectorType minValue, const VectorType maxValue) const;


	public:
		StorageType mX, mY, mZ;	/**< The compressed/packed vector components. */

		// the number of steps within the specified range
		enum { CONVERT_VALUE = (1 << (sizeof(StorageType) << 3)) - 1 };
};

// include the inline code
#include "CompressedVector.inl"

// declare standard types
typedef TCompressedVector3<MReal, unsigned short>	Compressed16BitVector3;
typedef TCompressedVector3<MReal, unsigned char>	Compressed8BitVector3;

} // namespace MCore


#endif