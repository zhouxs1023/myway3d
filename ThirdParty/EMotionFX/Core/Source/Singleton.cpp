/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Singleton.h"
#include "Algorithms.h"


namespace MCore
{

// default constructor
InstanceManagerBase::InstanceManagerBase()
{
	mPhase = 0;
}


// destructor
InstanceManagerBase::~InstanceManagerBase()
{
}


// comparison operator
bool InstanceManagerBase::operator<(const InstanceManagerBase& instanceBase)
{
	return (mPhase < instanceBase.mPhase);
}


// comparison operator
bool InstanceManagerBase::operator>(const InstanceManagerBase& instanceBase)
{
	return (mPhase > instanceBase.mPhase);
}


// comparison operator
bool InstanceManagerBase::operator==(const InstanceManagerBase& instanceBase)
{
	return (mPhase == instanceBase.mPhase);
}


// set the destruction phase of the singleton
void InstanceManagerBase::SetDestructionPhase(const int phase)
{
	mPhase = phase;
}


// get the destruction phase of the singleton
int InstanceManagerBase::GetDestructionPhase() const
{
	return mPhase;
}




// default constructor
SingletonManager::SingletonManager()
{
	mNumSingletons = 0;
}


// destructor
SingletonManager::~SingletonManager()
{
	DestroySingletons();
}


// register concrete instance manager implementation
void SingletonManager::RegisterSingleton(InstanceManagerBase* instanceManager)
{
	// increase the size of the singleton pointer array
	mSingletons = (InstanceManagerBase**)realloc(mSingletons, (mNumSingletons+1) * sizeof(InstanceManagerBase*));
	
	// register the new instance manager
	mSingletons[mNumSingletons] = instanceManager;

	// increase the numer of singletons
	mNumSingletons++;
}


// return the number of singletons in the manager
int SingletonManager::GetNumSingletons()
{
	return mNumSingletons;
}


// singleton destruction, main function
void SingletonManager::DestroySingletons()
{
	// sort the singleton objects based on their destruction phase(using selection sort)
	int i;
	for (i = 0; i < (mNumSingletons - 1); i++)
	{
		int minIndex = i;

		// find the index of the minimum element
		for (int j = i + 1; j < mNumSingletons; j++)
		{
			// compare the destrucion phases
			if (*mSingletons[j] < *mSingletons[minIndex])
				minIndex = j;
		}

		// swap if the element isn't already smallest
		if (minIndex > i) 
			Swap<InstanceManagerBase*>(mSingletons[i], mSingletons[minIndex]);
	}

	// iterate through the sorted singleton objects and destruct them in the right order
	for (i=0; i<mNumSingletons; i++)
	{
		// retrieve the singleton instance manager implementation
		InstanceManagerBase* instanceManager = mSingletons[i];

		// if it is not valid continue
		if (!instanceManager)
			continue;

		// destruct the singleton object
		instanceManager->Destroy();

		// and get rid of the instance manager instance as well
		delete instanceManager;
	}

	// clear the singleton storage
	free(mSingletons);
}


// return the instance of the singleton manager
SingletonManager& SingletonManager::GetInstance()
{
	static SingletonManager manager;

	return manager;
}

}