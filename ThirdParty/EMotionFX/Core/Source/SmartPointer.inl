/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "SmartPointer.h"


template<class T>
ReferenceCounter<T>::ReferenceCounter(const long startCount) : mCount(startCount)
{
}


template<class T>
ReferenceCounter<T>::~ReferenceCounter()
{
}


template<class T>
void ReferenceCounter<T>::IncRefCount() 
{
	mCount++; 
}


template<class T>
void ReferenceCounter<T>::DecRefCount()
{
	mCount--;

	if( mCount <= 0 )
	{
		DestroyRef();
	}
}


template<class T>
long ReferenceCounter<T>::GetRefCount() const
{
	return mCount;
}


template<class T>
T* ReferenceCounter<T>::GetPointer()
{
	return (T*)this;
}


template<class T>
void ReferenceCounter<T>::DestroyRef() 
{ 
	if( GetPointer() != NULL )
		delete GetPointer();
}



template<class T>
PointerReferenceCounter<T>::PointerReferenceCounter(T* pointer) : mPointer(pointer)
{

}


template<class T>
PointerReferenceCounter<T>::~PointerReferenceCounter()
{
	ReferenceCounter<T>::DestroyRef();
}


template<class T>
inline T* PointerReferenceCounter<T>::GetPointer()
{
	return mPointer;
}


template<class T>
inline void PointerReferenceCounter<T>::DestroyRef()
{
	delete this;
}



template<class T>
inline Pointer<T>::Pointer() : mReferenceCounter(NULL)
{

}


template<class T>
inline Pointer<T>::Pointer(T* pointer) : mReferenceCounter(NULL)
{
	Assign(pointer);
}


template<class T>
inline Pointer<T>::Pointer(const Pointer& pointer) : mReferenceCounter(NULL)
{
	Assign(pointer.mReferenceCounter);
}


template<class T>
inline Pointer<T>::~Pointer()
{
	Assign((IReferenceCounter<T>*)NULL);
}


template<class T>
inline T* Pointer<T>::GetPointer() const
{
	if(mReferenceCounter == NULL )
		return NULL;
   
	return mReferenceCounter->GetPointer();
}


template<class T>
inline long Pointer<T>::GetRefCount() const
{
	return mReferenceCounter->GetRefCount();
}


template<class T>
Pointer<T>& Pointer<T>::operator=(const Pointer& pointer)
{
	Assign(pointer.mReferenceCounter);
	
	return *this;
}


template<class T>
Pointer<T>& Pointer<T>::operator=(T* pointer)
{
	Assign(pointer);
	
	return *this;
}


template<class T>
inline T* Pointer<T>::operator->()
{
	assert(GetPointer() != NULL);

	return GetPointer();
}


template<class T>
inline Pointer<T>::operator T*() const
{
	return GetPointer();
}


template<class T>
inline bool Pointer<T>::operator!()
{
	return GetPointer() == NULL;
}


template<class T>
bool Pointer<T>::operator==(const Pointer& pointer)
{
	return GetPointer() == pointer.GetPointer();
}


template<class T>
bool Pointer<T>::operator!=(const Pointer& pointer)
{
	return GetPointer() != pointer.GetPointer();
}


template<class T>
inline void Pointer<T>::Assign(void* pointer)
{
	if( pointer == NULL )
	{
		Assign((IReferenceCounter<T>*)NULL);
	}
	else
	{
		Assign(new PointerReferenceCounter<T>(static_cast<T*>(pointer)));
	}
}


template<class T>
inline void Pointer<T>::Assign(IReferenceCounter<T>* refCounter)
{
	if( refCounter != NULL )
		refCounter->IncRefCount();

	IReferenceCounter<T>* oldRef = mReferenceCounter;
	mReferenceCounter = refCounter;

	if( oldRef != NULL )
		oldRef->DecRefCount();
}