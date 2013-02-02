/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "HashTable.h"


// default constructor
template <class Key, class Value>
HashTable<Key, Value>::HashTable()
{
	mTotalNumEntries = 0;
	mHashFunction	 = NULL;
}


// extended constructor
template <class Key, class Value>
HashTable<Key, Value>::HashTable(const int maxElements, HashFunction<Key>* hashFunction)
{
	mHashFunction	 = NULL;
	mTotalNumEntries = 0;
	Init(maxElements, hashFunction);
}


// copy constructor
template <class Key, class Value>
HashTable<Key, Value>::HashTable(const HashTable<Key, Value>& other) : mHashFunction(NULL), mTotalNumEntries(0)
{
	*this = other;
}



// destructor
template <class Key, class Value>
HashTable<Key, Value>::~HashTable()
{
	Clear();
}


// clear the table
template <class Key, class Value>
void HashTable<Key, Value>::Clear()
{
	// get rid of existing elements
	const int numElems = mElements.GetLength();
	for (int i=0; i<numElems; ++i)
	{
		if (mElements[i] != NULL)
			delete mElements[i];
	}

	// clear the array
	mElements.Clear();

	// get rid of the hash function object
	if (mHashFunction)
	{
		delete mHashFunction;
		mHashFunction = NULL;
	}

	mTotalNumEntries = 0;
}


// find the entry with a given key
template <class Key, class Value>
bool HashTable<Key, Value>::FindEntry(const Key& key, int* outElementNr, int* outEntryNr) const
{
	// calculate the hash value
	int hashResult = mHashFunction->Hash(key) % mElements.GetLength();

	// check if the we have an entry at this hash position
	if (mElements[hashResult] == NULL)
		return false;

	// search inside the array of entries
	const int numElements = mElements[hashResult]->GetLength();
	for (int i=0; i<numElements; ++i)
	{
		// if we found the one we are searching for
		if (mElements[hashResult]->GetItem(i).mKey == key)
		{
			*outElementNr = hashResult;
			*outEntryNr   = i;
			return true;
		}
	}

	return false;
}


// initialize the table at a given maximum amount of elements
template <class Key, class Value>
void HashTable<Key, Value>::Init(const int maxElements, HashFunction<Key>* hashFunction)
{
	// get rid of existing elements
	Clear();

	// resize the array
	mElements.Resize( maxElements );
	mElements.Shrink();

	// reset all the elements
	for (int i=0; i<maxElements; ++i)
		mElements[i] = NULL;

	// get rid of any existing hash function
	if (mHashFunction)
		delete mHashFunction;

	// set the hash function
	mHashFunction = hashFunction;
}


// add an entry to the table
template <class Key, class Value>
void HashTable<Key, Value>::Add(const Key& key, const Value& value)
{
	// calculate the hash value
	int hashResult = mHashFunction->Hash(key) % mElements.GetLength();

	// make sure there isn't already an element with this key
	assert( Contains(key) == false );

	// make sure the hash value is positive
	assert(hashResult >= 0);

	// if the array isn't allocated yet, do so
	if (mElements[hashResult] == NULL)
		mElements[hashResult] = new MCore::Array< Entry >();

	// add the entry to the array
	mElements[hashResult]->Add( Entry(key, value) );

	// increase the total number of entries
	mTotalNumEntries++;
}


// get a value
template <class Key, class Value>
bool HashTable<Key, Value>::GetValue(const Key& inKey, Value* outValue) const
{
	// try to find the element
	int elementNr, entryNr;
	if (FindEntry(inKey, &elementNr, &entryNr))
	{
		*outValue = mElements[elementNr]->GetItem(entryNr).mValue;
		return true;
	}

	// nothing found
	return false;
}


// check if there is an entry using the specified key
template <class Key, class Value>
bool HashTable<Key, Value>::Contains(const Key& key) const
{
	int elementNr, entryNr;
	return FindEntry(key, &elementNr, &entryNr);
}


template <class Key, class Value>
bool HashTable<Key, Value>::Remove(const Key& key)
{
	int elementNr, entryNr;
	if (FindEntry(key, &elementNr, &entryNr))
	{
		// remove the element
		mElements[elementNr]->Remove(entryNr);

		// remove the array if it is empty
		if (mElements[elementNr]->GetLength() == 0)
		{
			delete mElements[elementNr];
			mElements[elementNr] = NULL;
		}

		// decrease the total number of entries
		mTotalNumEntries--;

		// yeah, we successfully removed it
		return true;
	}

	// the element wasn't found, so cannot be removed
	return false;
}


// get the number of table elements
template <class Key, class Value>
int HashTable<Key, Value>::GetNumTableElements() const
{
	return mElements.GetLength();
}


// get the get the number of entries for a given table element
template <class Key, class Value>
int HashTable<Key, Value>::GetNumEntries(const int tableElementNr) const
{
	if (mElements[tableElementNr] == NULL)
		return 0;

	return mElements[tableElementNr]->GetLength();
}


// get the number of entries in the table
template <class Key, class Value>
int HashTable<Key, Value>::GetTotalNumEntries() const
{
	return mTotalNumEntries;
}


// calculate the load balance
template <class Key, class Value>
float HashTable<Key, Value>::CalcLoadBalance() const
{
	int numUsedElements = 0;

	// traverse all elements
	const int numElements = mElements.GetLength();
	for (int i=0; i<numElements; ++i)
	{
		if (mElements[i] != NULL)
			numUsedElements++;
	}

	if (numUsedElements == 0)
		return 0;

	return (numUsedElements / (float)numElements) * 100.0f;
}


template <class Key, class Value>
float HashTable<Key, Value>::CalcAverageNumEntries() const
{
	int numEntries = 0;
	int numUsedElements = 0;

	// traverse all elements
	const int numElements = mElements.GetLength();
	for (int i=0; i<numElements; ++i)
	{
		if (mElements[i] != NULL)
		{
			numUsedElements++;
			numEntries += mElements[i]->GetLength();
		}
	}

	if (numEntries == 0)
		return 0;

	return numEntries / (float)numUsedElements;
}


// update the value of the entry with a given key
template <class Key, class Value>
bool HashTable<Key, Value>::SetValue(const Key& key, const Value& value)
{
	// try to find the element
	int elementNr, entryNr;
	if (FindEntry(inKey, &elementNr, &entryNr))
	{
		mElements[elementNr]->GetItem(entryNr).mValue = value;
		return true;
	}

	// nothing found
	return false;
}


// get a given entry
template <class Key, class Value>
inline typename HashTable<Key, Value>::Entry& HashTable<Key, Value>::GetEntry(const int tableElementNr, const int entryNr)
{
	assert( tableElementNr >= 0 && tableElementNr < mElements.GetLength() );	// make sure the values are in range
	assert( mElements[tableElementNr] != NULL );								// this table element must have entries
	assert( entryNr >= 0 && entryNr < mElements[tableElementNr]->GetLength() );	// 

	return mElements[tableElementNr]->GetItem( entryNr );
}


// operator =
template <class Key, class Value>
HashTable<Key, Value>& HashTable<Key, Value>::operator = (const HashTable<Key, Value>& other)
{
	if (&other == this)
		return *this;

	// get rid of old data
	Clear();

	// copy the number of entries
	mTotalNumEntries = other.mTotalNumEntries;

	// resize the element array
	mElements.Resize( other.mElements.GetLength() );

	// copy the element entries
	const int numElements = mElements.GetLength();
	for (int i=0; i<numElements; ++i)
	{
		if (other.mElements[i] != NULL)
		{
			// create the array and copy the entries
			mElements[i]  = new MCore::Array< Entry >();
			*mElements[i] = *other.mElements[i];
		}
		else
			mElements[i] = NULL;
	}

	// copy the hash function
	if (other.mHashFunction)
		mHashFunction = other.mHashFunction->Clone();
	else
		mHashFunction = NULL;

	return *this;
}
