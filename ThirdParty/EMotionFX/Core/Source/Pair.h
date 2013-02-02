/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __PAIR_H
#define __PAIR_H

#include "StandardHeaders.h"
#include "SourceDeclaration.h"


namespace MCore
{

/**
 * The Composite(Pair) contains two data members combining them into a
 * single object. The most common situation in which multipile objects
 * need to be returned is the case of two objects.
 * For instance pairs are useful for implementing maps and
 * dictionaries. In both these abstractions, we maintain key-value pairs:
 * Pairs are added to the map or dictionary; we then search for a key,
 * returning its value.
 */
template <class T, class U>
class Pair
{
	public:
		/**
		 * Default constructor.
		 */
		Pair();

		/**
		 * Constructor that creates a pair with the passed values.
		 * @param first The initial value for the first member variable.
		 * @param second The initial value for the second member variable.
		 */
		Pair(const T& first, const U& second);

		/**
		 * Copy constructor.
		 * @see operator=
		 * @param pair The pair to copy.
		 */
		Pair(const Pair& pair);

		/**
		 * Destructor.
		 */
		virtual ~Pair();

		/**
		 * Assignment operator.
		 * @see Pair(const Pair& pair)
 		 * @param pair The pair to copy.
		 * @return The pair to assign.
		 */
		Pair<T, U>& operator=(const Pair& pair);

		/**
		 * Comparison operator.
		 * Compares this pair and a passed pair for equality.
		 * @param pair The pair to compare with.
		 * @result True if the two pairs are equal, false if not.
		 */
		bool operator==(const Pair& pair) const;

		/**
		 * Comparison operator.
		 * Pairs are inequal if either their first or their second member variables
		 * are inequal.
		 * @param pair The pair to compare with.
		 * @result True if the two pairs are inequal, false if not.
		 */
		bool operator!=(const Pair& pair) const;

		/**
		 * Comparison operator.
		 * Compares this pair and a passed pair for lessness. One pair is less than another,
		 * if its first member is less than the other's, or - if they're equal - if its
		 * second member is less than the other's.
		 * @param pair The pair to compare with.
		 * @result True if the pair is less than the passed pair, false if not.
		 */
		bool operator<(const Pair& pair) const;

		/**
		 * Mutator sets the first element of the pair.
		 * @param first The value to which the first element of the pair should be set.
		 */
		void SetFirst(const T& first);

		/**
		 * Mutator sets the second element of the pair.
		 * @param second The value to which the second element of the pair should be set.
		 */
		void SetSecond(const U& second);

		/**
		 * Accessor returns a constant reference to the first element of the pair.
		 * @result A constant reference to the first element of the pair.
		 */
		const T& GetFirst() const;

		/**
		 * Accessor returns a reference to the first element of the pair.
		 * @result A reference to the first element of the pair.
		 */
		T& GetFirst();

		/**
		 * Accessor returns a constant reference to the second element of the pair.
		 * @result A constant reference to the second element of the pair.
		 */
		const U& GetSecond() const;

		/**
		 * Accessor returns a reference to the second element of the pair.
		 * @result A reference to the second element of the pair.
		 */
		U& GetSecond();


	private:
		/**
		 * The first element of the pair.
		 */
		T mFirst;

		/**
		 * The second element of the pair.
		 */
		U mSecond;
};

// include inline code
#include "Pair.inl"

}	// namespace MCore


#endif