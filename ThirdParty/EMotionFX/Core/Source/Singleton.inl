/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Singleton.h"


// default constructor
template<class T>
InstanceManager<T>::InstanceManager() : InstanceManagerBase()
{
	mObject = NULL;
}


// destructor
template<class T>
InstanceManager<T>::~InstanceManager()
{
}


// destruction of the singleton object
template<class T>
void InstanceManager<T>::Destroy()
{
	if (mObject)
		delete mObject;
}


// return singleton object
template<class T>
inline T* InstanceManager<T>::GetObject()
{
	if (!mObject)
		mObject = new T();

	return mObject;
}




// initialize the object
template<class T> InstanceManager<T>* Singleton<T>::mInstanceManager = NULL;


// default constructor
template<class T>
Singleton<T>::Singleton()
{
}


// destructor
template<class T>
Singleton<T>::~Singleton()
{
	mInstanceManager = NULL;
}


// return the singleton instance
template<class T>
inline T& Singleton<T>::GetInstance()
{
	if (!mInstanceManager)
	{
		mInstanceManager = new InstanceManager<T>();
		SingletonManager::GetInstance().RegisterSingleton(mInstanceManager);
	}

	return *(mInstanceManager->GetObject());
}


// set the destruction phase of the singleton
template<class T>
void Singleton<T>::SetDestructionPhase(const int phase)
{
	// if the instance manager isn't valid this means the singleton hasn't been
	// constructed yet
	if (!mInstanceManager)
		GetInstance();

	// set the destruction phase
	mInstanceManager->SetDestructionPhase(phase);
}


// get the destruction phase of the singleton
template<class T>
int Singleton<T>::GetDestructionPhase()
{
	// if the instance manager isn't valid this means the singleton hasn't been
	// constructed yet
	if (!mInstanceManager)
		GetInstance();

	// return the destruction phase
	return mInstanceManager->GetDestructionPhase();
}