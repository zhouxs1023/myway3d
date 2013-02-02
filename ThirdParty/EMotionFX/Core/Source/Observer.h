/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __OBSERVER_H
#define __OBSERVER_H

#include "LinkedList.h"


namespace MCore
{

/**
 * Observer
 */
template <class T>
class Observer
{
	public:
		/**
		 * Default constructor.
		 */
		Observer();

		/**
		 * Destructor.
		 */
		virtual ~Observer();

		/**
		* Update.
		* Observer updates the attributes of the subject.
		* @param observee The constant reference subject.
		*/
		virtual void Update(T& observee, const float& timeDelta=0) = 0;
};



/**
 * Observer subject
 */
template <class T>
class Subject
{
	public:
		/**
		 * Default constructor.
		 */
		Subject();
	
		/**
		 * Destructor.
		 */
		virtual ~Subject();

		/**
		 * Inserts an observer into the collection.
		 * @param observer The observer to insert.
		 */
		virtual void AddObserver(const Observer<T>& observer);

		/**
		 * Removes all observer that compare equal to the one.
		 * @param observer The observer type to to delete.
		 */
		virtual void RemoveObserver(const Observer<T>* observer);

		/**
		 * Walk through the observers and update each.
		 * @param observee The object to observe.
		 * @param timeDelta The time passed since the last call.
		 */
		virtual void NotifyAll(const T& observee, float timeDelta = 0);


	private:
		/// Observer collection/storage.
		LinkedList<Observer<T>*> mObservers;
};

// include inline code
#include "Observer.inl"

}	// namespace MCore


#endif