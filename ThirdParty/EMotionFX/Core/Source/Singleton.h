/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __SINGLETON_H
#define __SINGLETON_H

#include "StandardHeaders.h"


namespace MCore
{

/**
 * Use this macro to make a class to a singleton.
 * It creates a specific GetInstance() function to provide singleton behaviors.
 * Make sure that you have private constructors and destructors. This is essential to make sure
 * that there will be no other instance of the object, because noone is then able to create a new
 * instance from it. Both the copy constructor and assignment operator will be declared but have not defined in
 * private scope, too.
 */
#define SINGLETON(ClassName) \
	public: \
		static ClassName& GetInstance() \
		{ \
			return MCore::Singleton< ClassName >::GetInstance(); \
		} \
		friend class MCore::InstanceManager< ClassName >; \
	private: \
		ClassName(const ClassName& object); \
		ClassName& operator=(const ClassName& object);


/**
 * The instance manager base class is an adapter class which is needed to handle singletons
 * from different types in a manager. This class also stores destruction phase information.
 * The bigger the destruction phase the later the singleton will be destructed.
 * The default destruction phase value is set to zero.
 */
class InstanceManagerBase
{
	public:
		/**
		 * Default constructor.
		 * The default destruction phase value is zero.
		 */
		InstanceManagerBase();

		/**
		 * Destructor.
		 */
		virtual ~InstanceManagerBase();

		/**
		 * Destroy the singleton object. Each singleton has its own implementation of
		 * this function to delete its object from memory.
		 */
		virtual void Destroy() = 0;

		/**
		 * Comparison operator.
		 * @param instanceBase The instance manager base to compare with.
		 * @result True if the destruction phase is less than the passed destruction phase, false if not.
		 */
		bool operator<(const InstanceManagerBase& instanceBase);

		/**
		 * Comparison operator.
		 * @param instanceBase The instance manager base to compare with.
		 * @result True if the destruction phase is greater than the passed destruction phase, false if not.
		 */
		bool operator>(const InstanceManagerBase& instanceBase);

		/**
		 * Comparison operator.
		 * Compares this instance manager base and a passed instance manager base for equality.
		 * @param instanceBase The instance manager base to compare with.
		 * @result true if the two destruction phases are equal, false if not.
		 */
		bool operator==(const InstanceManagerBase& instanceBase);

		/**
		 * Set the destruction phase for this singleton. The bigger the destruction phase the
		 * later the singleton gets destructed.
		 * @param phase The destruction phase for the singleton.
		 */
		void SetDestructionPhase(const int phase);

		/**
		 * Get the destruction phase of the singleton. The bigger the destruction phase the
		 * later the singleton gets destructed.
		 * @return The destruction phase of the singleton.
		 */
		int GetDestructionPhase() const;

	protected:
		int	mPhase; /**< The destruction phase of the singleton. */
};


/**
 * The instance manager template class is the concrete implementation of the instance manager of the singleton and
 * is inherited from the instance manager base to get the destruction phase funcionality.
 * One instance of this class is stored in the singleton manager for each singleton. 
 * This class stores the singleton object and is responsible for its creation and destruction.
 */
template <class T>
class InstanceManager : public InstanceManagerBase
{
	public:
		/**
		 * Default constructor.
		 */
		InstanceManager();

		/**
		 * Destructor.
		 */
		~InstanceManager();

		/**
		 * Destroy the singleton object. Each singleton has its own implementation of
		 * this function to delete its object from memory.
		 */
		void Destroy();

		/**
		 * Return a pointer to the singleton instance.
		 * @return A pointer to the singleton object.
		 */
		inline T* GetObject();

	protected:
		T* mObject; /** Pointer to the singleton object. */
};


/**
 * The singleton manager is responsible for the destruction of the singleton objects. Each time a singleton is created, by
 * calling the Singleton::GetInstance() function the first time, the singleton registers its concrete instance manager implementation.
 * The singleton manager is the only real singleton. All the other singletons act like singletons but the destruction is managed by
 * this class. The singleton objects get destructed automatically with the singleton manager, so there is no need to destruct them by hand.
 */
class SingletonManager
{
	public:
		/**
		 * Default constructor.
		 */
		SingletonManager();

		/**
		 * Destructor.
		 */
		~SingletonManager();

		/// Not implemented.
		SingletonManager(const SingletonManager& singletonManager);

		/// Not implemented.
		SingletonManager& operator=(const SingletonManager& singletonManager);

		/**
		 * Register concrete instance manager implementation of a singleton.
		 * @param instanceManager Pointer to the instance manager implementation.
		 */
		void RegisterSingleton(InstanceManagerBase* instanceManager);

		/**
		 * Return the number of singeltons that have already been constructed.
		 * @return The number of singeltons managed by this class.
		 */
		int GetNumSingletons();

		/**
		 * Singleton destruction.
		 * This function sorts the singletons by their destruction phase and destruct them in 
		 * the right order. The bigger the destruction phase the later the singletons get destructed.
		 */
		void DestroySingletons();

		/**
		 * Return the singleton instance.
		 * @return The reference to the unique singleton manager.
		 */
		static SingletonManager& GetInstance();

	private:
		InstanceManagerBase**	mSingletons;	/**< The singleton objects. */
		int						mNumSingletons;	/**< The number of singletons managed by this class. */
};


/**
 * The singleton pattern ensures only a single instance of a
 * class exist throughout the duration of the application. It also
 * provides a global point of access to the object. The creation of
 * the Singleton is deferred until the first time it is accessed.
 * The singleton object is initialized the first time GetInstance() is
 * called and deallocated when main exits.
 *
 * <pre>
 * These are the recommended steps to make a class into a singleton:
 * - Make constructors and destructors private.
 * - Declare but do not define both the copy ctor and assignment operator.
 * - Define a typedef of the singleton holder for accessing the class. Clients
 *   access the class using this typedef. Example: #define LOG_MANAGER LogManager::GetInstance()
 * </pre>
 *
 * To set a singleton's destruction phase call this from any place:<br>
 * MCore::Singleton< ClassName >::SetDestructionPhase( DestructionPhase );
 *
 * @Note The nonstatic members control a single instance of class held by singleton holder.
 */
template <class T>
class Singleton
{
	private:
		/**
		 * Default constructor.
		 */
		Singleton();

		/**
		 * Destructor.
		 */
		~Singleton();

		/// Not implemented.
		Singleton(const Singleton<T>& singleton);

		/// Not implemented.
		Singleton& operator=(const Singleton<T>& singleton);

	public:
		/**
		 * Returns the singleton instance.
		 * @return The reference to the unique object.
		 */
		static inline T& GetInstance();

		/**
		 * Set the destruction phase for this singleton. The bigger the destruction phase the
		 * later the singleton gets destructed. So singletons in phase 0 get destructed firstly,
		 * which is also the default phase.
		 * @param phase The destruction phase for the singleton.
		 */
		static void SetDestructionPhase(const int phase);

		/**
		 * Get the destruction phase of the singleton. The bigger the destruction phase the
		 * later the singleton gets destructed.
		 * @return The destruction phase of the singleton.
		 */
		static int GetDestructionPhase();

	private:
		static InstanceManager<T>* mInstanceManager; /**< The concrete implementation of the instance manager template for the singleton object. */
};

// include inline code
#include "Singleton.inl"

} // namespace MCore


#endif