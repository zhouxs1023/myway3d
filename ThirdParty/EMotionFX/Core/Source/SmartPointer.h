/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __SMARTPOINTER_H
#define __SMARTPOINTER_H

#include "StandardHeaders.h"
#include "SourceDeclaration.h"


namespace MCore
{

// forward declaration
template<class T> class Pointer;


/**
 * Interface for reference counting.
 * Classes can implement it themselves or use the standard reference counting
 * system ReferenceCounter.
 */
template<class T>
class IReferenceCounter
{
	public:
		/**
		 * Increase the reference counter.
		 */
		virtual void IncRefCount() = 0;

		/**
		 * Decrease the reference counter.
		 */
		virtual void DecRefCount() = 0;

		/**
		 * Return reference count.
		 * @result The reference count.
		 */
		virtual long GetRefCount() const = 0;

		/**
		 * Return pointer to the object.
		 * @result A pointer to the object.
		 */
		virtual T* GetPointer() = 0;
};



/**
 * A standard implementation of the reference counter interface IReferenceCounter
 * To use it just derive your class from it:
 * class CMyObject : public ReferenceCounter< MyObject > { ... };
 */
template<class T>
class ReferenceCounter : public IReferenceCounter<T>
{
	public:
		/**
		 * Default constructor.
		 */
		ReferenceCounter(const long startCount = 0);

		/**
		 * Destructor.
		 */
		virtual ~ReferenceCounter();

		/**
		 * Increase the reference counter.
		 */
		virtual void IncRefCount() ;

		/**
		 * Decrease the reference counter.
		 */
		virtual void DecRefCount();

		/**
		 * Return reference count.
		 * @result The reference count.
		 */
		long GetRefCount() const;

		/**
		 * Return pointer to the object.
		 * @result A pointer to the object.
		 */
		virtual T* GetPointer();


	protected:
		/**
		 * Destroy reference.
		 */
		virtual void DestroyRef() ;

	private:
		long mCount; /**< The reference count */
};



/**
 * An internal implementation of IReferenceCounter
 * for classes that don't implement it.
 * Pointer will automatically choose between its internal and 
 * class' implementation of IReferenceCounter.
 */
template<class T>
class PointerReferenceCounter : public ReferenceCounter<T>
{
	public:
		/**
		 * Constructor.
		 * @param pointer A pointer to the object.
		 */
		PointerReferenceCounter(T* pointer);

		/**
		 * Destructor.
		 */
		~PointerReferenceCounter();

		/**
		 * Returns a pointer to the object.
		 * @result A pointer to the object.
		 */
		inline T* GetPointer();


	private:
		/**
		 * Destroy reference.
		 */
		inline void DestroyRef();

		T* mPointer; /**< A pointer to the object. */
};



/**
 * The (Smart)Pointer class is a generic wrapper that encapsulates pointers. It keeps
 * track of the total number of reference counts and performs garbage collection
 * when the pointed object is no more referenced by any "pointer". Smart pointer
 * is implemented as a template class. Reference Counting Garbage Collection -
 * Pointer will free dynamically allocated memory automatically.
 * Usage:
 * <pre>
 * <code>
 * ======================
 * 1. In a program block
 * ======================
 * Pointer<MyClass> ptr1(new MyClass);  // creates object 1
 * Pointer<MyClass> ptr2(new MyClass);  // creates object 2
 * ptr1 = ptr2;							// destroys object 1
 * ptr2 = NULL;
 * ptr1 = new MyClass;					// creates object 3, destroys object 2
 * ptr1->methodcall(...);
 * MyClass o1;
 * MyClass* ptr1 = &o1;   // DON'T ! Only memory allocated by new operator should be used
 * MyClass *o2 = new MyClass;
 * ptr1 = o2;
 * //ptr2 = o2;   // DON'T ! unless MyClass implements IRefCount
 *                // try to use ptr1 = ptr2 instead, it's always safe;
 *
 * ======================
 * 2. in a function call
 * ======================
 * void func(MyClass *o) {...}
 * ...
 * Pointer<MyClass> ptr(new MyClass)
 * func(ptr);
 *
 * ======================
 * 3. As a return value
 * ======================
 * Pointer<MyClass> func()
 * {
 *      Pointer<MyClass> ptr(new MyClass);
 *      return ptr;
 * }
 * </code>
 * </pre>
 */
template<class T>
class Pointer
{
	public:
		/**
		 * Default constructor.
		 */
		Pointer();

		/**
		 * Constructor.
		 * @param pointer A pointer to the newly created object. See Usage.
		 */
		inline Pointer(T* pointer);

		/**
		 * Copy constructor.
		 * @param The pointer to copy.
		 */
		inline Pointer(const Pointer& pointer);

		/**
		 * Destructor.
		 */
		inline virtual ~Pointer();

		/**
		 * Return pointer to the object.
		 * @result A pointer to the object.
		 */
		inline T* GetPointer() const;

		/**
		 * Return reference count.
		 * @result The reference count.
		 */
		inline long GetRefCount() const;

		/**
		 * Assignment operator.
		 * @param pointer The smart pointer to assign.
		 * @result A reference to *this which allows concatenation.
		 */
		Pointer& operator=(const Pointer& pointer);

		/**
		 * Assignment operator.
		 * @param pointer The pointer or NULL to assign.
		 * @result A reference to *this which allows concatenation.
		 */
		Pointer& operator=(T* pointer);

		/**
		 * Provide access to members of T.
		 * @result A pointer to the object.
		 */
		inline T* operator->();

		/**
		 * Conversion operator.
		 * Conversion to T* (for function calls)
		 * @result Converted object pointer.
		 */
		inline operator T*() const;

		/**
		 * Return true if object is NULL, false if not.
		 * @result True if object is NULL, false if not.
		 */
		inline bool operator!();

		/**
		 * Comparison operator.
		 * Compares this smart pointer and a passed smart pointer for equality.
		 * @param pointer The smart pointer to compare with.
		 * @result true if the two smart pointers are equal, false if not.
		 */
		bool operator==(const Pointer& pointer);

		/**
		 * Comparison operator.
		 * Compares this smart pointer and a passed smart pointer for inequality.
		 * @param pointer The smart pointer to compare with.
		 * @result true if the two smart pointers aren't equal, false if they are.
		 */
		bool operator!=(const Pointer& pointer);


	private:
		IReferenceCounter<T>* mReferenceCounter; /**< The reference counting system. */

		/**
		 * This method is called if T does not implement IReferenceCounter.
		 * @param pointer The pointer to assign.
		 */
		inline void Assign(void* pointer);

		/**
		 * This method is picked over Assign(void *pointer)
		 * if T implements IReferenceCounter.
		 * This allows some memory usage optimization.
		 * @param refCounter The reference counter to assign.
		 */
		inline void Assign(IReferenceCounter<T>* refCounter);
};

// include inline code
#include "SmartPointer.inl"

}	// namespace MCore


#endif