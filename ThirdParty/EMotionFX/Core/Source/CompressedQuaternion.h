 /*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __COMPRESSEDQUATERNION_H
#define __COMPRESSEDQUATERNION_H

#include "Macros.h"
#include "SourceDeclaration.h"
#include "Quaternion.h"
#include "Algorithms.h"


namespace MCore
{

/**
 * The compressed / packed quaternion class.
 * This represents a unit (normalized) quaternion on a packed way, which is 8 bytes per quaternion instead of 16 bytes
 * when using floating point quaternions. Of course we loose a bit of precision, but it isn't too bad for most things.
 * The class provides methods to convert from and to uncompressed quaternions.
 */
template <class QuatType, class StorageType>
class TCompressedQuaternion
{
	public:
		/**
		 * Default constructor.
		 * This sets the quaternion to identity.
		 */
		inline TCompressedQuaternion();

		/**
		 * Create a compressed quaternion from an uncompressed one.
		 * Please note that the uncompressed quaternion has to be normalized or a unit quaternion!
		 * @param quat The normalized uncompressed quaternion.
		 */
		inline TCompressedQuaternion(const TQuaternion<QuatType>& quat);

		/**
		 * Update the compressed quaternion from an uncompressed one.
		 * Please note that the uncompressed quaternion has to be normalized or a unit quaternion!
		 * @param quat The normalized uncompressed quaternion.
		 */
		inline void FromQuaternion(const TQuaternion<QuatType>& quat);

		/**
		 * Uncompress the compressed quaternion into an uncompressed one.
		 * @param output The output uncompressed quaternion to write the result in.
		 */
		inline void UnCompress(TQuaternion<QuatType>* output) const;

		/**
		 * Convert the compressed quaternion into an uncompressed one.
		 * This method works the same as the UnCompress method, but it returns the result instead of specifying
		 * the output quaternion.
		 * @result The uncompressed version of this compressed quaternion.
		 */
		inline TQuaternion<QuatType> ToQuaternion() const;


	public:
		StorageType mX, mY, mZ, mW;	/**< The compressed/packed quaternion components values. */

		// the number of steps within the specified range
		enum { CONVERT_VALUE = ((1 << (sizeof(StorageType) << 3)) >> 1) - 1 };
};

// include the inline code
#include "CompressedQuaternion.inl"

// declare standard types
typedef TCompressedQuaternion<MReal, signed short>	Compressed16BitQuaternion;
typedef TCompressedQuaternion<MReal, signed char>	Compressed8BitQuaternion;

} // namespace MCore


#endif