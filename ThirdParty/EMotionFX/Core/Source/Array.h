/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __ARRAY_H
#define __ARRAY_H

#include "Macros.h"
#include "Config.h"
#include "SourceDeclaration.h"
#include "MemoryManager.h"
#include <assert.h>
#include <new.h>


namespace MCore
{

/**
 * Dynamic array template.
 */
template <class T>
class Array
{
	public:
		/**
		 * The memory block ID, used inside the memory manager.
		 * This will make all arrays remain in the same memory blocks, which is more efficient in a lot of cases.
		 * However, array data can still remain in other blocks.
		 */
		enum { MEMORYBLOCK_ID = 2 };

		/**
		 * The memory category of the array template data.
		 */
		enum { MEMORYCATEGORY = 2 };

		/**
		 * Default constructor.
		 * Initializes the array so it's empty and has no memory allocated.
		 */
		inline Array()														: mLength(0), mMaxLength(0), mData(NULL)	{}

		/**
		 * Constructor which creates a given number of elements.
		 * @param e The element data.
		 * @num The number of elements in 'e'.
		 */
		inline explicit Array(T* e, int num)								: mLength(num), mMaxLength(AllocSize(num))	{ mData = (T*)MEMMGR.Allocate(mMaxLength * sizeof(T), MEMORYCATEGORY, 16, MEMORYBLOCK_ID); for (int i=0; i<mLength; i++) Construct(i, e[i]); }

		/**
		 * Constructor which initializes the length of the array on a given number.
		 * @param initSize The number of ellements to allocate space for.
		 */
		inline explicit Array(int initSize)									: mLength(initSize), mMaxLength(initSize)	{ mData = (T*)MEMMGR.Allocate(mMaxLength * sizeof(T), MEMORYCATEGORY, 16, MEMORYBLOCK_ID); for (int i=0; i<mLength; i++) Construct(i); }

		/**
		 * Copy constructor.
		 * @param other The other array to copy the data from.
		 */
		Array(const Array& other)											: mLength(0), mMaxLength(0), mData(NULL)	{ *this = other; }

		/**
		 * Destructor. Deletes all entry data.
		 * However, if you store pointers to objects, these objects won't be deleted.<br>
		 * Example:<br>
		 * <pre>
		 * Array<int*> data;
		 * for (int i=0; i<10; i++)
		 *    data.Add( new int );
		 * </pre>
		 * Now when the array 'data' will be destructed, it will NOT free up the memory of the integers which you allocated by hand, using new.
		 * In order to free up this memory, you can do this:
		 * <pre>
		 * while (data.GetLength())
		 * {
		 *    delete data.GetLast();
		 *    data.RemoveLast();
		 * }
		 * </pre>
		 */
		~Array()															{ for (int i=0; i<mLength; i++) Destruct(i); MEMMGR.Delete(mData); }

		/**
		 * Get a pointer to the first element.
		 * @result A pointer to the first element.
		 */
		inline T* GetPtr()													{ return mData; }

		/**
		 * Get a given item/element.
		 * @param pos The item/element number.
		 * @result A reference to the element.
		 */
		inline T& GetItem(int pos)											{ return mData[pos]; }

		/**
		 * Get the first element.
		 * @result A reference to the first element.
		 */
		inline T& GetFirst()												{ return mData[0]; }

		/**
		 * Get the last element.
		 * @result A reference to the last element.
		 */
		inline T& GetLast()													{ return mData[mLength-1]; }

		/**
		 * Get a read-only pointer to the first element.
		 * @result A read-only pointer to the first element.
		 */
		inline const T*	GetReadPtr() const									{ return mData; }

		/**
		 * Get a read-only reference to a given element number.
		 * @param pos The element number.
		 * @result A read-only reference to the given element.
		 */
		inline const T&	GetItem(int pos) const								{ return mData[pos]; }

		/**
		 * Get a read-only reference to the first element.
		 * @result A read-only reference to the first element.
		 */
		inline const T&	GetFirst() const									{ return mData[0]; }

		/**
		 * Get a read-only reference to the last element.
		 * @result A read-only reference to the last element.
		 */
		inline const T&	GetLast() const										{ return mData[mLength-1]; }

		/**
		 * Check if the array is empty or not.
		 * @result Returns true when there are no elements in the array, otherwise false is returned.
		 */
		inline bool	IsEmpty() const											{ return mLength==0; }

		/**
		 * Get the maximum index in the array. This is equal to GetLength()-1.
		 * @result The maximum index.
		 */
		inline int GetMaxIndex() const										{ return mLength-1; }

		/**
		 * Checks if the passed index is in the array's range.
		 * @param index The index to check.
		 * @return True if the passed index is valid, false if not.
		 */
		inline bool IsValidIndex(const int index) const						{ return ( (index >= 0) && (index < mLength) ); }

		/**
		 * Get the number of elements in the array.
		 * @result The number of elements in the array.
		 */
		inline int GetLength() const										{ return mLength; }

		/**
		 * Get the number of elements in the array.
		 * @result The number of elements in the array.
		 */
		inline int Size() const												{ return mLength; }

		/**
		 * Get the maximum number of elements. This is the number of elements there currently is space for to store.
		 * However, never use this to make for-loops to iterate through all elements. Use GetLength() instead for that.
		 * This purely has to do with pre-allocating, to reduce the number of reallocs.
		 * @result The maximum array length.
		 */
		inline int GetMaxLength() const										{ return mMaxLength; }

		/**
		 * Calculates the memory usage, currently in use by the data of the array.
		 * Keep in mind that if you store pointers to objects, only the size of the pointers is counted (like 4 bytes per element)
		 * and not the actual size of the object you allocated. So it's purely the size of the array itself.
		 * @result The number of bytes allocated by this array.
		 */
		inline int CalcMemoryUsage() const									{ return mMaxLength*sizeof(T); }

		/**
		 * Set a given element to a given value.
		 * @param pos The element number.
		 * @param value The value to store at that element number.
		 */
		inline void	SetElem(int pos, const T& value)						{ mData[pos] = value; }

		/**
		 * Add a given element to the back of the array.
		 * @param x The element to add.
		 */
		inline void	Add(const T& x)											{ Grow(++mLength); Construct(mLength-1, x); }

		/**
		 * Add a given array to the back of this array.
		 * @param a The array to add.
		 */
		inline void	Add(const Array<T>& a)									{ int l=mLength; Grow(mLength+a.mLength); for (int i=0; i<a.GetLength(); i++) Construct(l+i, a[i]); }

		/**
		 * Add an empty (default constructed) element to the back of the array.
		 */
		inline void	AddIsEmpty()											{ Grow(++mLength); Construct(mLength-1); }

		/**
		 * Remove the first array element.
		 */
		inline void	RemoveFirst()											{ assert(mLength>0);Remove((int)0); }

		/**
		 * Remove the last array element.
		 */
		inline void	RemoveLast()											{ assert(mLength>0);Destruct(--mLength); }

		/**
		 * Insert an empty element (default constructed) at a given position in the array.
		 * @param pos The position to create the empty element.
		 */
		inline void	Insert(int pos)											{ Grow(mLength+1); MoveEls(pos+1, pos, mLength-pos-1); Construct(pos); }

		/**
		 * Insert a given element at a given position in the array.
		 * @param pos The position to insert the empty element.
		 * @param x The element to store at this position.
		 */
		inline void	Insert(int pos, const T &x)								{ Grow(mLength+1); MoveEls(pos+1, pos, mLength-pos-1); Construct(pos, x); }

		/**
		 * Remove an element at a given position.
		 * @param pos The element number to remove.
		 */
		inline void	Remove(int pos)											{ Destruct(pos); MoveEls(pos, pos+1, mLength-pos-1); mLength--; }

		/**
		 * Remove a given number of elements starting at a given position in the array.
		 * @param pos The start element, so to start removing from.
		 * @param num The number of elements to remove from this position.
		 */
		inline void	Remove(int pos, int num)								{ assert(num>=0); for (int i=pos; i<pos+num; i++) Destruct(i); MoveEls(pos, pos+num, mLength-pos-num); mLength-=num; }

		/**
		 * Remove a given element.
		 * @param item The item/element to remove.
		 */
		inline bool	Remove(const T& item)									{ int index=Find(item); if (index==-1) return false; Remove(index); return true; }

		/**
		 * Remove a given element in the array and place the last element in the array at the created empty position.
		 * So if we have an array with the following characters : ABCDEFG<br>
		 * And we perform a SwapRemove(2), we will remove element C and place the last element (G) at the empty created position where C was located.
		 * So we will get this:<br>
		 * AB.DEFG [where . is empty, after we did the SwapRemove(2)]<br>
		 * ABGDEF [this is the result. G has been moved to the empty position].
		 */
		inline void	SwapRemove(int pos)										{ Destruct(pos); if (pos != mLength-1) { Construct(pos, mData[mLength-1]); Destruct(mLength-1); } mLength--; } // remove element at <pos> and place the last element of the array in that position

		/**
		 * Swap two elements.
		 * @param pos1 The first element number.
		 * @param pos2 The second element number.
		 */
		inline void	Swap(int pos1, int pos2)								{ if (pos1 != pos2) MCore::Swap(GetItem(pos1), GetItem(pos2)); }

		/**
		 * Clear the array contents. So GetLength() will return 0 after performing this method.
		 * @param clearMem If set to true (default) the allocated memory will also be released. If set to false, GetMaxLength() will still return the number of elements
		 * which the array contained before calling the Clear() method.
		 */
		inline void	Clear(bool clearMem=true)								{ for (int i=0; i<mLength; i++) Destruct(i); mLength=0;	if (clearMem) Free(); }

		/**
		 * Make sure the array has enough space to store a given number of elements.
		 * @param newLength The number of elements we want to make sure that will fit in the array.
		 */
		inline void	AssureSize(int newLength)								{ if (mLength >= newLength) return; int oldLen=mLength; Grow(newLength); for (int i=oldLen; i<newLength; i++) Construct(i); }

		/**
		 * Make sure this array has enough allocated storage to grow to a given number of elements elements without having to realloc.
		 * @param minLength The minimum length the array should have (actually the minimum maxLength, because this has no influence on what GetLength() will return).
		 */
		inline void	Reserve(int minLength)									{ if (mMaxLength < minLength) Realloc(minLength); }

		/**
		 * Make the array as small as possible. So remove all extra pre-allocated data, so that the array consumes the least possible amount of memory.
		 */
		inline void	Shrink()												{ if (mLength == mMaxLength) return; assert(mMaxLength > mLength); Realloc(mLength); }

		/**
		 * Check if the array contains a given element.
		 * @param x The element to check.
		 * @result Returns true when the array contains the element, otherwise false is returned.
		 */
		inline bool	Contains(const T& x) const								{ return (Find(x) != -1); }

		/**
		 * Find the position of a given element.
		 * @param x The element to find.
		 * @result Returns the index in the array, ranging from [0 to GetMaxIndex()] when found, otherwise -1 is returned.
		 */
		inline int	Find(const T& x) const									{ for (int i=0; i<mLength; i++) { if (mData[i] == x) return i; } return -1; }


		// sort function and standard sort function
		typedef int	(__cdecl *CmpFunc)(const T& itemA, const T& itemB);
		static	int	__cdecl StdCmp(const T& itemA, const T& itemB)			{ if (itemA < itemB) return -1; else if (itemA == itemB) return 0; else return 1; }
		static	int	__cdecl StdPtrObjCmp(const T& itemA, const T& itemB)	{ if (*itemA < *itemB) return -1; else if (*itemA == *itemB) return 0; else return 1; }

		/**
		 * Sort the complete array using a given sort function.
		 * @param cmp The sort function to use.
		 */
		inline void	Sort(CmpFunc cmp)										{ InnerSort(0, mLength-1, cmp); }

		/**
		 * Sort a given part of the array using a given sort function.
		 * The default parameters are set so that it will sort the compelete array with a default compare function (which uses the < and > operators).
		 * The method will sort all elements between the given 'first' and 'last' element (first and last are also included in the sort).
		 * @param first The first element to start sorting.
		 * @param last The last element to sort (when -1, GetMaxIndex() will be used).
		 * @param cmp The compare function.
		 */
		inline void	Sort(int first=0, int last=-1, CmpFunc cmp=StdCmp) 		{ if (last==-1) last=mLength-1; InnerSort(first, last, cmp); }

		/**
		 * Performs a sort on a given part of the array.
		 * @param first The first element to start the sorting at.
		 * @param last The last element to end the sorting.
		 * @param cmp The compare function.
		 */
		inline void	InnerSort(int first, int last, CmpFunc cmp)				{ if (first>=last) return; int split=Partition(first, last, cmp); InnerSort(first, split-1, cmp); InnerSort(split+1, last, cmp); }

		/**
		 * Resize the array to a given size.
		 * This does not mean an actual realloc will be made. This will only happen when the new length is bigger than the maxLength of the array.
		 * @param newLength The new length the array should be.
		 */
		void Resize(int newLength)
		{
			assert(newLength>=0);
		
			// check for growing or shrinking array
			if (newLength > mLength)
			{
				// growing array, construct empty elements at end of array
				int old_len = mLength;
				GrowExact(newLength);
				for (int i=old_len; i<newLength; i++)
					Construct(i);
			}
			else
			{ 
				// shrinking array, destruct elements at end of array
				for (int i=newLength; i<mLength; i++)
					Destruct(i);
				
				mLength = newLength;
			}
		}

		// operators
		bool				operator==(const Array &other) const 			{ if (mLength != other.mLength) return false; for (int i=0; i<mLength; i++) if (mData[i] != other.mData[i]) return false; return true; }
		Array&				operator= (const Array &other)					{ if (&other != this) { Clear(); Grow(other.mLength); for (int i=0; i<mLength; i++) Construct(i, other.mData[i]); } return *this; }
		Array&				operator+ (const Array &other) const			{ Array narr; narr.Grow(mLength+other.mLength); for (int i=0; i<Length(); i++) narr.Construct(i, mData[i]); for (int j=0; j<other.Length(); j++) narr.Construct(i++, other.mData[j]); return narr; }
		Array&				operator+=(const T& other)						{ Add(other); return *this; }
		Array&				operator+=(const Array<T>& other)				{ Add(other); return *this; }
		inline T&			operator[](int pos)								{ return mData[pos]; }
		inline const T&	operator[](int pos) const							{ return mData[pos]; }

	private:
		int		mLength, mMaxLength;	// the length and maximum length
		T*		mData;					// the data
		
		// private functions
		inline void	Grow(int newLength)										{ mLength = newLength; if (mMaxLength >= newLength) return;	Realloc(AllocSize(newLength)); }	
		inline void	GrowExact(int newLength)								{ mLength = newLength; if (mMaxLength < newLength) Realloc(newLength); }
		inline int	AllocSize(int num)										{ return 1+num/*+num/8*/; }						// return the amount of elements to allocate for <num> entries
		inline void	Alloc(int num)											{ mData = (T*)MEMMGR.Allocate(num * sizeof(T), MEMORYCATEGORY, 16, MEMORYBLOCK_ID); }
		inline void	Realloc(int newSize)									{ mData = (T*)MEMMGR.ReAlloc(mData, newSize * sizeof(T), MEMORYCATEGORY, 16, MEMORYBLOCK_ID); mMaxLength=newSize; }
		inline void	Free()													{ mMaxLength=0; MEMMGR.Delete(mData); mData=NULL; }
		inline void	Destruct(unsigned int index)							{ (mData+index)->~T(); }					// destruct an element at <index>
		inline void	Construct(int index, const T& original)					{ ::new(mData+index) T(original); }			// copy-construct an element at <index> which is a copy of <original>
		inline void	Construct(int index)									{ ::new(mData+index) T; }					// construct an element at place <index>
		inline void	MoveEls(int dest, int src, int num)						{ memmove(mData+dest, mData+src, num * sizeof(T)); }

		// partition part of array (for sorting)
		int	Partition(int left, int right, CmpFunc cmp)
		{
			MCore::Swap(mData[left], mData[ (left+right)>>1 ]);

			T& target = mData[right];
			int i    = left-1;
			int j    = right;
			while (1) 
			{
				while (i < j) { if (cmp(mData[++i], target) >= 0) break; }
				while (j > i) { if (cmp(mData[--j], target) <= 0) break; }
				if (i >= j) break;
				MCore::Swap(mData[i], mData[j]);
			}
			MCore::Swap(mData[i], mData[right]);
			return i;
		}
};

}	// namespace MCore

#endif