/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __STACK_H
#define __STACK_H

#include "Macros.h"
#include "Config.h"
#include "SourceDeclaration.h"
#include "LinkedList.h"
#include <assert.h>

// TODO: make a .inl file

namespace MCore
{

/**
 * The stack template.
 * You can push and pop things from the stack. Where pushing things on the stack is adding it on the top and where
 * popping from the stack is removing from the top again.
 */
template <class T>
class Stack : protected LinkedList<T>
{
	DECLARE_CLASS(Stack<>)

	public:
		/**
		 * Default constructor.
		 */
		Stack<T>()									: LinkedList<T>()	{}

		/**
		 * Destructor. Automatically clears the stack.
		 */
		virtual ~Stack<T>()							{ LinkedList<T>::Clear(); }

		/**
		 * Empty the stack. Remove all its items.
		 */
		inline void MakeEmpty()						{ LinkedList<T>::Clear(); }

		/**
		 * Push (add) an object on top of the stack.
		 * @param item The item to push on the stack.
		 */
		inline void Push(const T& item)				{ LinkedList<T>::PushBack(item); }

		/**
		 * Pop (get) the last added object from the stack (so also remove it). So the object which is on the top of the stack.
		 * @result The object on top of the stack, which has now been removed from the stack.
		 */
		inline T Pop()								{ T result = LinkedList<T>::Back(); LinkedList<T>::PopBack(); return result; }

		/**
		 * Get the last added object from the stack. So the object which is on the top of the stack.
		 * @result The object on top of the stack.
		 */
		inline T& Top()								{ return LinkedList<T>::Back();}

		/**
		 * Check if the stack is empty or not.
		 * @result Returns true when the stack is empty, so does not contains any items, otherwise it returns false.
		 */
		inline bool IsEmpty() const					{ return LinkedList<T>::IsEmpty(); }

		/**
		 * Get the number of elements on the stack.
		 * @result The number of items on the stack.
		 */
		inline int GetSize() const					{ return LinkedList<T>::GetSize(); }

		/**
		 * Get the number of elements on the stack.
		 * @result The number of items on the stack.
		 */
		inline int Size() const						{ return LinkedList<T>::GetSize(); }
};


} // namespace MCore


#endif