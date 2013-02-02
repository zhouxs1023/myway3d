/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __HASHFUNCTIONS_H
#define __HASHFUNCTIONS_H

// include required headers
#include "Vector.h"
#include "String.h"


namespace MCore
{

/**
 * The hash function base class/template.
 * The has function must return an non-negative (so positive) integer, based on a key value.
 */
template <class Key>
class HashFunction
{
	public:
		/**
		 * The main method, which returns the hash value for a given key.
		 * Please note that the returned integer must be positive, and is NOT allowed to be negative.
		 * @param key The key to calculate the hash value for.
		 * @result The hash value for the given key, which is a positive integer.
		 */
		virtual int Hash(const Key& key) const = 0;

		/**
		 * Clone the hash function.
		 * @result A cloned version of the hash function object.
		 */
		virtual HashFunction<Key>* Clone() = 0;
};


/**
 * The string hash function.
 */
class StringHashFunction : public HashFunction<String>
{
	public:
		/**
		 * The main method, which returns the hash value for a given key.
		 * Please note that the returned integer must be positive, and is NOT allowed to be negative.
		 * @param key The key to calculate the hash value for.
		 * @result The hash value for the given key, which is a positive integer.
		 */
		int Hash(const String& key) const
		{
			int len = key.GetLength();
			if (len == 0)
				return 0;

			return len + key[0] + key[len-1] + key[len>>1];
		}

		/**
		 * Clone the hash function.
		 * @result A cloned version of the hash function object.
		 */
		HashFunction<String>* Clone()	{ return new StringHashFunction(); }
};


/**
 * The integer hash function.
 */
class IntHashFunction : public HashFunction<int>
{
	public:
		/**
		 * The main method, which returns the hash value for a given key.
		 * Please note that the returned integer must be positive, and is NOT allowed to be negative.
		 * @param key The key to calculate the hash value for.
		 * @result The hash value for the given key, which is a positive integer.
		 */
		int Hash(const int& key) const
		{
			return Math::Abs(key);
		}

		/**
		 * Clone the hash function.
		 * @result A cloned version of the hash function object.
		 */
		HashFunction<int>* Clone()	{ return new IntHashFunction(); }
};


/**
 * The float hash function.
 */
class FloatHashFunction : public HashFunction<float>
{
	public:
		/**
		 * The main method, which returns the hash value for a given key.
		 * Please note that the returned integer must be positive, and is NOT allowed to be negative.
		 * @param key The key to calculate the hash value for.
		 * @result The hash value for the given key, which is a positive integer.
		 */
		int Hash(const float& key) const
		{
			return Math::Abs(key * 12345.0f);
		}

		/**
		 * Clone the hash function.
		 * @result A cloned version of the hash function object.
		 */
		HashFunction<float>* Clone()	{ return new FloatHashFunction(); }
};


/**
 * The double has function.
 */
class DoubleHashFunction : public HashFunction<double>
{
	public:
		/**
		 * The main method, which returns the hash value for a given key.
		 * Please note that the returned integer must be positive, and is NOT allowed to be negative.
		 * @param key The key to calculate the hash value for.
		 * @result The hash value for the given key, which is a positive integer.
		 */
		int Hash(const double& key) const
		{
			return Math::Abs(key * 12345.0);
		}

		/**
		 * Clone the hash function.
		 * @result A cloned version of the hash function object.
		 */
		HashFunction<double>* Clone()	{ return new DoubleHashFunction(); }
};


/**
 * The vector3 hash function.
 */
class Vector3HashFunction : public HashFunction<Vector3>
{
	public:
		/**
		 * The main method, which returns the hash value for a given key.
		 * Please note that the returned integer must be positive, and is NOT allowed to be negative.
		 * @param key The key to calculate the hash value for.
		 * @result The hash value for the given key, which is a positive integer.
		 */
		int Hash(const Vector3& key) const
		{
			return Math::Abs(key.x*101.0f + key.y*1002.0f + key.z * 10003.0f);
		}

		/**
		 * Clone the hash function.
		 * @result A cloned version of the hash function object.
		 */
		HashFunction<Vector3>* Clone()	{ return new Vector3HashFunction(); }
};


}	// namespace MCore

#endif