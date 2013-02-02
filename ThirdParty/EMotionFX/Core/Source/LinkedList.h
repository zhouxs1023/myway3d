/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H

#include "Exception.h"
#include <limits>
//#include "Iterator.h"
#include "StandardHeaders.h"
#include "SourceDeclaration.h"


namespace MCore
{

/// Incomplete class declarations
template <class T> class ListNode;
template <class T> class LinkedList;
template <class T> class ConstListIterator;
template <class T> class ListIterator;

/// List size type.
#define LISTSIZETYPE int

/**
 * The basic doubly linked LinkedList node.
 * Everything is private, and is accessible
 * only by the iterators and LinkedList classes.
 */
template <class T>
class ListNode
{
	DECLARE_CLASS(ListNode<>)

	public:
		/**
		 * Default constructor.
		 * @param _node The object.
		 * @param _prev The pointer to the previous object.
		 * @param _next The pointer to the next object.
		 */
		ListNode(const T& _object=T(), ListNode<T>* _prev=0, ListNode<T>* _next=0);

		/**
		 * Destructor.
		 */
		~ListNode();


	private:
		/// The object data.
		T data;

		/// The pointer to the previous object.
		ListNode<T>* prev;

		/// The pointer to the next object.
		ListNode<T>* next;

		friend class ConstListIterator<T>;
		friend class ListIterator<T>;
		friend class LinkedList<T>;
};



/**
 * Linked lists allow general access, arbitray insertion, deletion,
 * and find operations. The basic linked list consists of a collection
 * of connected, dynamically allocated nodes. In this implementation of a
 * doubly linked list each node consists of the data element and a pointer
 * to the next and to the previous element in the list. The first and the last
 * node in the list has a 0 next / 0 prev pointer. Linked list operations
 * use only a constant number of data movements.
 * Linked lists have the important property that insertion and splicing do not
 * invalidate iterators to list elements, and that even removal invalidates only
 * the iterators that point to the elements that are removed.
 * If you want to delete an object out of the middle of a list, but iterate further
 * through it, you have to assign the iterator to the one returned by the Erase()
 * function.
 * <code>
 *	for( LinkedList<MyClass*>::Iterator iterator = myClassList.Begin();
 *	iterator != myClassList.End();
 *	iterator++ )
 *	{
 *		if (!iterator.IsValid() || !iterator.IsInitialized())
 *			break;
 *
 *		MyClass* object = *iterator;
 *
 *		if( object )
 *		{
 *			delete object;
 *			iterator = mModificators.Erase(iterator);
 *
 *			if( iterator == mModificators.End() )
 *				break;
 *		}
 *	}
 * </code>
 */
template <class T>
class LinkedList
{
	DECLARE_CLASS(LinkedList<>)

	public:
		/// List specific iterator.
		typedef ListIterator<T> Iterator;

		/// List specific iterator.
		typedef ConstListIterator<T> ConstIterator;

		/**
		 * Default constructor.
		 */
		LinkedList();

		/**
		 * Destructor.
		 */
		virtual ~LinkedList();

		/**
		 * Copy constructor.
		 * @see operator=
		 * @param list The list to copy.
		 */
		LinkedList(const LinkedList& list);

		/**
		 * Assignment operator.
		 * @see LinkedList(const LinkedList& list)
 		 * @param list The list to copy.
		 * @return The list to assign.
		 */
		const LinkedList& operator=(const LinkedList& list);

		/**
		 * Accessor returns the size of the list meaning the amount
		 * of currently active objects.
		 * @result The size.
		 */
		inline LISTSIZETYPE Size() const;

		/**
		 * Accessor returns the size of the list meaning the amount
		 * of currently active objects.
		 * @result The size.
		 */
		inline LISTSIZETYPE GetSize() const;

		/**
		 * Accessor returns the maximum size of the list.
		 * @result The maximum size.
		 */
		inline LISTSIZETYPE MaxSize() const;

		/**
		 * Checks if the list is empty.
		 * @result True if the is empty, false if not.
		 */
		inline bool IsEmpty() const;

		/**
		 * Returns an iterator to the first valid object.
		 * @return The iterator that points to the first object.
		 */
		inline Iterator Begin();

		/**
		 * Returns an const iterator to the first valid object.
		 * @return The const iterator that points to the first object.
		 */
		inline ConstIterator Begin() const;

		/**
		 * Returns an iterator to the last valid object.
		 * @return The iterator that points to the last object.
		 */
		inline Iterator End();

		/**
		 * Returns an const iterator to the last valid object.
		 * @return The const iterator that points to the last object.
		 */
		inline ConstIterator End() const;

		/**
		 * Returns the first object of the list.
		 * @result The first object.
		 */
		inline T& Front();

		/**
		 * Returns the first object of the list.
		 * @result The first object.
		 */
		inline const T& Front() const;

		/**
		 * Returns the last object of the list.
		 * @result The last object.
		 */
		inline T& Back();

		/**
		 * Returns the last object of the list.
		 * @result The last object.
		 */
		inline const T& Back() const;

		/**
		 * Inserts an object into the list creating a new
		 * node, copy in the data, set next and prev pointer.
		 * This function will insert the object at front position.
		 * @param object The object to insert.
		 */
		inline void PushFront(const T& object);

		/**
		 * Inserts an object into the list creating a new
		 * node, copy in the data, set next and prev pointer.
		 * This function will insert the object at back position.
		 * @param object The object to insert.
		 */
		inline void PushBack(const T& object);

		/**
		 * Deletes the front object of the list.
		 */
		inline void PopFront();

		/**
		 * Deletes the front object of the list.
		 */
		inline void PopBack();

		/**
		 * Deletes the front object of the list.
		 */
		inline void RemoveFirst();

		/**
		 * Deletes the front object of the list.
		 */
		inline void RemoveLast();

		/**
		 * Inserts an object into the list creating a new
		 * node, copy in the data, set next and prev pointer.
		 * This function will insert the object before the iterator.
		 * @param object The object to insert.
		 * @param iterator The insertion position provided by an iterator.
		 */
		Iterator Insert(Iterator iterator, const T& object);

		/**
		 * Erases an object out of the list.
		 * This function will delete the object at iterator position.
		 * @param iterator The erase position provided by an iterator.
		 * @return A valid iterator that points to the object next to iterator.
		 */
		Iterator Erase(Iterator iterator);

		/**
		 * Erases objects out of the list.
		 * This function will delete objects ranging from
		 * start to end position.
		 * @param object The object to insert.
		 * @param iterator The insertion position provided by an iterator.
		 * @return A valid iterator that points to the object next to iterator.
		 */
		inline Iterator Erase(const Iterator& start, Iterator end);

		/**
		 * Removes all elements that compare equal to object. The relative order
		 * of elements that are not removed is unchanged, and iterators to elements
		 * that are not removed remain valid. This function is linear time: it performs
		 * exactly size() comparisons for equality.
		 * @param object The object to search for.
		 */
		inline void Remove(const T& object);

		/**
		 * Erases every object out of the list. After calling this funtion
		 * the linked list should be empty.
		 */
		inline void MakeEmpty();

		/**
		 * Erases every object out of the list. After calling this funtion
		 * the linked list should be empty.
		 */
		inline void Clear();

		/**
		 * Find object.
		 * @param object The object to search for.
		 * @return Returns an iterator pointing to the object if it is existing,
		           if not the function will throw an exception returning a non
				   initialized iterator.
		 */
		Iterator Find(const T& object);

		/**
		 * Find object.
		 * @param object The object to search for.
		 * @return Returns an iterator pointing to the object if it is existing,
		           if not the function will throw an exception returning a non
				   initialized iterator.
		 */
		ConstIterator Find(const T& object) const;

		friend class ConstListIterator<T>;
		friend class ListIterator<T>;


	private:
		/// Private list node declaration.
		typedef ListNode<T> node;

		/// The list size.
		LISTSIZETYPE mSize;

		/// The head node.
		node* mHead;
		/// The tail node.
		node* mTail;
};

/**
 * Iterators are a generalization of pointers. They are objects that
 * point to other objects. Iterators allow programs to step through the
 * elements of a container sequentially without exposing the underlying
 * representation.
 * Iterators can be advanced from one element to the next
 * by incrementing them. Some iterators can also be decremented or allow
 * arbitrary jumps from one element to another. When they are
 * dereferenced, iterators yield a reference to a container element. In
 * addition, they can be compared to each other for equality or inequality.
 * Iterators are central to generic programming because they are an interface
 * between containers and algorithms. algorithms typically take iterators as
 * arguments, so a container need only provide a way to access its elements
 * using iterators. This makes it possible to write a generic algorithm that
 * operates on many different kinds of containers, even containers as different
 * as a vector and a doubly linked list.
 */
template <class T>
class ConstListIterator// : public Iterator<T>
{
	DECLARE_CLASS(ConstListIterator<>)

	public:
		/**
		 * Default constructor.
		 */
		ConstListIterator();

		/**
		 * Copy constructor.
		 * @ see operator=
		 * @param iterator The iterator.
		 */
		ConstListIterator(const ConstListIterator<T>& iterator);

		/**
		 * Destructor.
		 */
		virtual ~ConstListIterator();

		/**
		 * Copy constructor.
		 * @ see ConstListIterator(const ConstListIterator<T>& iterator)
		 * @param iterator The iterator.
		 */
		inline ConstListIterator<T>& operator=(const ConstListIterator<T>& iterator);

		/**
		 * Comparison operator.
		 * Compares this iterator and a passed iterator for equality.
		 * @param iterator The iterator to compare with.
		 * @result true if the two iterators are equal, false if not.
		 */
		inline bool operator==(const ConstListIterator<T>& iterator) const;

		/**
		 * Comparison operator.
		 * Compares this iterator and a passed iterator for inequality.
		 * @param iterator The iterator to compare with.
		 * @result true if the two iterators aren't equal, false if they are.
		 */
		inline bool operator!=(const ConstListIterator<T>& iterator) const;

		/**
		 * Returns the object stored at the current position.
		 * For iterator, there is an accessor with a
		 * const reference return type and a mutator with
		 * a reference return type. For const iterator, there
		 * is only an accessor with a const reference return type.
		 * @return The reference/const reference of the object.
		 */
		inline virtual const T& operator*() const;

		/**
		 * Step to the next object in the collection.
		 * @result The iterator pointing to the next object.
		 */
		inline ConstListIterator<T>& Advance();

		/**
		 * Step to the next object in the collection.
		 * @result The iterator pointing to the next object.
		 */
		inline ConstListIterator<T>& operator++();

		/**
		 * Step to the next object in the collection.
		 * @result The iterator pointing to the next object.
		 */
		inline ConstListIterator<T> operator++(int);

		/**
		 * Steps forward x times and returns the iterator after
		 * reaching the position.
		 * @result The iterator.
		 */
		ConstListIterator<T>& operator+=(int steps);

		/**
		 * Step to the previous object in the collection.
		 * @result The iterator pointing to the previous object.
		 */
		inline ConstListIterator<T>& operator--();

		/**
		 * Step to the previous object in the collection.
		 * @result The iterator pointing to the previous object.
		 */
		inline ConstListIterator<T> operator--(int);

		/**
		 * Steps backward x times and returns the iterator after
		 * reaching the position.
		 * @result The iterator.
		 */
		ConstListIterator<T>& operator-=(int steps);

		/**
		 * Throws an exception if this iterator is obviously
		 * uninitialized. Otherwise, returns safely.
		 * @return True if it is initialized, false if not.
		 */
		inline bool IsInitialized() const;

		/**
		 * Throws an exception if the mCurrent position is
		 * not somewhere in the range from begin to end, inclusive.
		 * Otherwise, returns safely.
		 * @return True if it is valid, false if not.
		 */
		inline bool IsValid() const;

		/**
		 * Throws an exception if operator++ cannot be safely applied
		 * to the mCurrent position. Otherwise, returns safely.
		 * Checks if there is next object in the collection.
 		 * @return True if it can advance, false if not.
		 */
		inline bool CanAdvance() const;

		/**
		 * Throws an exception if operator-- cannot be safely applied
		 * to the mCurrent position. Otherwise, returns safely.
 		 * @return True if it can retrat, false if not.
		 */
		inline bool CanRetreat() const;


	protected:
		/**
		 * Protected constructor.
		 * Expects the LinkedList that owns the iterator and a
		 * pointer that represents the mCurrent position.
		 * @param source The linked list.
		 * @param node The node to point at.
		 */
		ConstListIterator(const LinkedList<T>& source, ListNode<T>* node);

		/**
		 * Protected helper in ConstListIterator that returns the object
		 * stored at the mCurrent position. Can be called by all
		 * three versions of operator* without any type conversions.
		 * @return The object stored at the mCurrent position.
		 */
		T& Retrieve() const;


		/// Pointer to the list's head node.
		ListNode<T>* mHead;
		/// Pointer to the current node.
		ListNode<T>* mCurrent;

		friend class LinkedList<T>;
};

/**
 * Iterators are a generalization of pointers. They are objects that
 * point to other objects. Iterators allow programs to step through the
 * elements of a container sequentially without exposing the underlying
 * representation.
 * Iterators can be advanced from one element to the next
 * by incrementing them. Some iterators can also be decremented or allow
 * arbitrary jumps from one element to another. When they are
 * dereferenced, iterators yield a reference to a container element. In
 * addition, they can be compared to each other for equality or inequality.
 * Iterators are central to generic programming because they are an interface
 * between containers and algorithms. algorithms typically take iterators as
 * arguments, so a container need only provide a way to access its elements
 * using iterators. This makes it possible to write a generic algorithm that
 * operates on many different kinds of containers, even containers as different
 * as a vector and a doubly linked list.
 */
template <class T>
class ListIterator : public ConstListIterator<T>
{
	DECLARE_CLASS(ListIterator<>)

	public:
		/**
		 * Default constructor.
		 * Calls the base-class constructor.
		 * Must be provided because the private constructor
		 * is written; otherwise zero-parameter constructor
		 * would be disabled.
		 */
		ListIterator();

		/**
		 * Destructor.
		 */
		~ListIterator();

		/**
		 * Returns the object stored at the current position.
		 * For iterator, there is an accessor with a
		 * const reference return type and a mutator with
		 * a reference return type. For const iterator, there
		 * is only an accessor with a const reference return type.
		 * @return The reference/const reference of the object.
		 */
		inline T& operator*();

		/**
		 * Returns the object stored at the current position.
		 * For iterator, there is an accessor with a
		 * const reference return type and a mutator with
		 * a reference return type. For const iterator, there
		 * is only an accessor with a const reference return type.
		 * @return The reference/const reference of the object.
		 */
		inline const T& operator*() const;

		/**
		 * Step to the next object in the collection.
		 * @result The iterator pointing to the next object.
		 */
		inline ListIterator<T>& Advance();

		/**
		 * Step to the next object in the collection.
		 * @result The iterator pointing to the next object.
		 */
		inline ListIterator<T>& operator++();

		/**
		 * Step to the next object in the collection.
		 * @result The iterator pointing to the next object.
		 */
		inline ListIterator<T> operator++(int);

		/**
		 * Steps forward x times and returns the iterator after
		 * reaching the position.
		 * @result The iterator.
		 */
		ListIterator<T>& operator+=(int steps);

		/**
		 * Step to the previous object in the collection.
		 * @result The iterator pointing to the previous object.
		 */
		inline ListIterator<T>& operator--();

		/**
		 * Step to the previous object in the collection.
		 * @result The iterator pointing to the previous object.
		 */
		inline ListIterator<T> operator--(int);

		/**
		 * Steps backward x times and returns the iterator after
		 * reaching the position.
		 * @result The iterator.
		 */
		ListIterator<T>& operator-=(int steps);


	protected:
		friend class LinkedList<T>;

		/**
		 * Protected constructor.
		 * Expects the LinkedList that owns the iterator and a
		 * pointer that represents the mCurrent position.
		 * @param source The linked list.
		 * @param node The node to point at.
		 */
		ListIterator(const LinkedList<T>& source, ListNode<T>* node);
};

// include inline code
#include "LinkedList.inl"

}	// namespace MCore


#endif