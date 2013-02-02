/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __COMPRESSEDFLOAT_H
#define __COMPRESSEDFLOAT_H

#include "Macros.h"
#include "SourceDeclaration.h"
#include "Algorithms.h"


namespace MCore
{

/**
 * The compressed / packed float class.
 * The class can automatically compresses a float of type 'FloatType' into a float that
 * represents this value inside the 'StorageType' type. An example would be a floating point value
 * (FloatType=float) compressed into a CompressedFloat that stores the value as 8-bit unsigned char.
 * This would reduce the float size from 4 bytes into 1 byte, at the trade of a bit loss in precision and CPU overhead for
 * uncompression into a float again. When used with care, the CPU overhead and precision loss are negligible.
 * During both compression and uncompression you also have to specify the range (minimum and maximum possible values) of the
 * value you want to compress/decompress. If you are dealing with normalized values, the minimum value
 * would be 0, and the maximum value would be +1. When uncompressing (converting back to a float) you
 * have to be sure you use the same minimum and maximum values as when you used to compress it! This is very important!
 * The bigger the range, the more precision loss. There is however no performance impact linked to the range.
 */
template <class FloatType, class StorageType>
class TCompressedFloat
{
	public:
		/**
		 * Default constructor.
		 * This leaves the member uninitialized, so if you get the uncompressed version, the result is unknown.
		 */
		inline TCompressedFloat();

		/**
		 * Create a compressed float from an uncompressed one.
		 * @param value The floating point value you want to compress.
		 * @param minValue The minimum possible value of the uncompressed floating point value.
		 * @param maxValue The maximum possible value of the uncompressed floating point value.
		 */
		inline TCompressedFloat(const FloatType value, const FloatType minValue, const FloatType maxValue);

		/**
		 * Create a compressed float from an uncompressed one.
		 * @param value The floating point value you want to compress.
		 * @param minValue The minimum possible value of the uncompressed floating point value.
		 * @param maxValue The maximum possible value of the uncompressed floating point value.
		 */
		inline void FromFloat(const FloatType value, const FloatType minValue, const FloatType maxValue);

		/**
		 * Uncompress this compressed floating point value into an uncompressed float.
		 * Please note that the minimum and maximum values you specify are the same as when you created
		 * this compressed floating point value or when you for the last time called the FromFloat method.
		 * @param output The uncompressed floating point value to store the result in.
		 * @param minValue The minimum possible value of the uncompressed floating point value.
		 * @param maxValue The maximum possible value of the uncompressed floating point value.
		 */
		inline void UnCompress(FloatType* output, const FloatType minValue, const FloatType maxValue) const;

		/**
		 * Uncompress this compressed floating point value into an uncompressed float.
		 * Please note that the minimum and maximum values you specify are the same as when you created
		 * this compressed floating point value or when you for the last time called the FromFloat method.
		 * @param minValue The minimum possible value of the uncompressed floating point value.
		 * @param maxValue The maximum possible value of the uncompressed floating point value.
		 * @result The uncompressed version of this floating point value.
		 */
		inline FloatType ToFloat(const FloatType minValue, const FloatType maxValue) const;


	public:
		StorageType mValue;	/**< The compressed/packed value. */

		// the number of steps within the specified range
		enum { CONVERT_VALUE = (1 << (sizeof(StorageType) << 3)) - 1 };
};

// include the inline code
#include "CompressedFloat.inl"

// declare standard types
typedef TCompressedFloat<MReal, unsigned short>	Compressed16BitFloat;
typedef TCompressedFloat<MReal, unsigned char>	Compressed8BitFloat;

} // namespace MCore


#endif