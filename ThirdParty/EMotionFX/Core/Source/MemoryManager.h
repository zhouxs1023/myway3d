/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __MEMORYMANAGER_H
#define __MEMORYMANAGER_H

// include core related headers
#include "SourceDeclaration.h"
#include "Singleton.h"

namespace MCore
{

/**
 * Enable or disable the use of the memory manager.
 * When this define is not made it means that the memory manager will simply use malloc/realloc and free.
 */
//#define USE_MEMORY_MANAGER

/**
 * A shortcut to the memory manager singleton.
 */
#define MEMMGR MCore::MemoryManager::GetInstance()

/**
 * The memory manager.
 * This memory manager can be used to take over the management of allocated and released memory.
 * It eliminates small allocations by allocating relatively big blocks of memory and returning
 * pointers to free areas inside these blocks instead of doing many real allocations.
 * The manager also tries to prevent and minimize memory fragmentation using some special techniques.
 * Next to the anti-fragmentation schemes the manager also allows you to specify the alignment of an allocation.
 * This will ensure you that the start memory address of your allocation will be aligned as you want.
 * Detailed memory statistics can be requested at any time. Also it is possible to split up your allocations in different
 * categories. Per category statistics can be calculated and returned, which is useful to see how much memory is spend
 * by what type of classes. Finally the memory manager will keep track of memory leaks and report them to you when
 * you close your application. When memory leaks are detected, these leaks will be released automatically.
 */
class MemoryManager
{
	DECLARE_CLASS( MemoryManager );
	SINGLETON( MemoryManager );

	public:
		// forward declaration
		class Allocation;

		/**
	 	 * The block type ID that specifies that every allocation is allowed in this block.
		 */
		enum { BLOCKTYPE_ANY = 0 };

		/**
		 * The global memory usage statistics, which includes all memory categories.
		 * NOTE: All sizes are in bytes, unless specified differently.
		 */
		struct Statistics
		{
			unsigned int mNumAllocations;			/**< The current number of allocations. */
			unsigned int mNumBlocks;				/**< The current number of blocks. */
			unsigned int mTotalBlockMem;			/**< The current total block memory. So this is the total allocated memory. However you should look at mAllocatedBytes if you like to measure memory usage of your code. */
			unsigned int mAllocatedBytes;			/**< The current allocated bytes (alignment overhead excluded). */
			unsigned int mBlockOverhead;			/**< The current block overhead, which is the amount of unused block memory (so what is allocated but unused). */
			unsigned int mAlignmentOverhead;		/**< The total overhead due to alignment. */
			unsigned int mManagementOverhead;		/**< The total allocation overhead. */
			unsigned int mAvgAllocSize;				/**< The average size of a single allocation. */
			unsigned int mAvgBlockSize;				/**< The average block size. */
			unsigned int mAvgBlockOverheadPerAlloc;	/**< The average block overhead per allocation. */
			unsigned int mAvgAlignmentOverhead;		/**< Average overhead due to alignment, per allocation. */
			unsigned int mMaxBlockMem;				/**< Maximum allocated block memory. */
			unsigned int mMaxAllocatedBytes;		/**< Maximum allocated bytes. */
			unsigned int mMaxNumAllocations;		/**< The maximum number of allocations that existed. */
			unsigned int mMaxNumBlocks;				/**< The maximum number of memory blocks that existed. */
		};


		/**
		 * Per memory category statistics.
		 * These statistics can be generated with the GenerateCategoryStatistics method.
		 * NOTE: All measured sizes are in bytes, unless specified differently.
		 */
		struct CategoryStatistics
		{
			unsigned int mCategoryID;		/**< The category ID for which these are the statistics. */
			unsigned int mNumAllocations;	/**< The current number of allocations for this category. */
			unsigned int mAllocatedBytes;	/**< The current amount of memory that has been allocated by this category. */
			unsigned int mAvgAllocSize;		/**< The current average allocation size. */
		};

		//-----------------------------------------------------------------

		/**
		 * A memory block, which is a big continuous chunk of memory that can contain a list of allocations.
		 * Memory blocks are often also called Heaps.
		 */
		class MemoryBlock
		{
			friend class MemoryManager;

			public:
				/**
				 * The constructor.
				 * @param numBytes The size of this block, in bytes.
				 * @param isPrivate Is this a private memory block? If so, it will not accept any allocations, except for the one where
				 *                  this block was created for.
				 * @param blockTypeID The block type ID of this memory block. This can be used to group specific classes into the same block.
				 *                    On default the BLOCKTYPE_ANY value is set, which means it accepts objects of all classes.
				 */
				MemoryBlock(const unsigned int numBytes, const bool isPrivate=false, const unsigned int blockTypeID=MCore::MemoryManager::BLOCKTYPE_ANY);

				/**
				 * The destructor.
				 * Automatically deletes all allocations inside this block. Also releases the allocated memory by this block.
				 */
				~MemoryBlock();

				/**
				 * Get a pointer to the first allocation inside this memory block.
				 * This can be NULL in case the block has no allocations inside it.
				 * @result A pointer to the first allocation, or NULL when the block has no allocations inside it yet.
				 */
				inline Allocation* GetFirstAllocation() const		{ return mFirstAllocation; }

				/**
				 * Get the number of bytes of this memory block.
				 * So in other words, this is the memory size of the memory block.
				 * @result The memory size of this memory block, in bytes.
				 */
				inline unsigned int GetNumBytes() const				{ return mTotalBytes; }

				/**
				 * Get the block type ID.
				 * This type ID number is used to group specific allocations together.
				 * It is possible that the user specifies a memory block ID where he wants to force the allocation to be
				 * made in. This is used to group things such as MCore::String or MCore::Array objects together since this is more efficient for 
				 * these type of objects. The default block type ID equals MCore::MemoryManager::BLOCKTYPE_ANY, which means that this memory block 
				 * allows any type of allocation to be made in it, unless the user specifies differently when he makes the allocation.
				 * @result The memory block type ID.
				 */
				inline unsigned int GetBlockType() const			{ return mBlockTypeID; }

				/**
				 * Check if the memory block is private or not.
				 * Private memory blocks contain only one single allocation that was bigger than the regular block size that
				 * you can get with MemoryManager::GetBlockSize(). Private memory blocks do not accept any other additinal allocations
				 * to be inserted inside this memory block.
				 * @result Returns true if the memory block is private, otherwise false is returned.
				 */
				inline unsigned int IsPrivate() const				{ return mIsPrivate; }

				/**
				 * Get the start memory address of the memory block.
				 * The number of bytes to follow after this memory address is returned by the GetNumBytes() method.
				 * @result A pointer to the start memory address of this memory block.
				 */
				inline void* GetMemoryAddress() const				{ return mMemory; }


			private:
				void*			mMemory;			/**< The memory start address of this block. */
				Allocation*		mFirstAllocation;	/**< The first allocation (linked list), or NULL when there are no allocations. */
				unsigned int	mTotalBytes;		/**< The size in bytes of this block. */
				unsigned int	mBlockTypeID;		/**< The block ID. Only allocations with the same ID are allowed inside this block. When the ID is 0, it allows all types. */
				bool			mIsPrivate;			/**< Is this a private block? (accepts no other allocations). */

				/**
				 * Set the size of this memory block.
				 * This should not be changed if there are any allocations inside this block already.
				 * @param numBytes The new size of the block, in bytes.
				 */
				void SetBlockSize(const unsigned int numBytes);

				/**
				 * Remove a given allocation that has its address at a specified memory address.
				 * This is the returned address to the user, so NOT the mRealAddress member, but the mAddress member.
				 * @param memAddress The memory address of the allocation to remove.
				 * @param outBlockIsEmpty The method will write true to this boolean when the block is empty after removing the specified allocation. It is set to false otherwise.
				 * @result Returns true when the allocation has been removed or false when no allocation with the given memory address was found inside this memory block.
				 */
				bool RemoveAllocation(void* memAddress, bool* outBlockIsEmpty);

				/**
				 * Remove a given allocation that has its address at a specified memory address.
				 * @param allocation The allocation to remove. This assumes the allocation is part of this block and does exist.
				 * @param outBlockIsEmpty The method will write true to this boolean when the block is empty after removing the specified allocation. It is set to false otherwise.
				 */
				void RemoveAllocation(Allocation* allocation, bool* outBlockIsEmpty);

				/**
				 * Allocate a given number of bytes.
				 * @param numBytes The number of bytes to alocate.
				 * @param categoryID The category ID of the allocation.
				 * @param alignment The number of bytes to align the memory address on.
				 * @param blockType The type memory block to add this allocation to. This can be used to for example make sure that all
				 *                  allocations made by a String class will end up in the same type of block. On default this value is set to
				 *                  BLOCKTYPE_ANY, which means that the allocation can end up in any block, no matter what its type is.
				 *                  When you create custom types, make sure these type IDs are not equal to zero, since this is the value for BLOCKTYPE_ANY.
				 * @result A pointer to the allocation, or NULL when the allocation couldn't be made inside this memory block.
				 */
				Allocation* Allocate(const unsigned int numBytes, const unsigned int categoryID, const unsigned int alignment=16, const unsigned int blockType=MCore::MemoryManager::BLOCKTYPE_ANY);

				/**
				 * Try to find an allocation with a given memory start address, inside this block.
				 * @param memoryAddress The memory address once returned by Allocate or ReAllocate.
				 * @result A pointer to the allocation that has its start address at the given memory address. A value of NULL is returned when the allocation cannot be found.
				 */
				Allocation* FindAllocation(void* memoryAddress);
		};

		//-----------------------------------------------------------------

		/**
		 * An allocation that has been made by the user. They are stored inside a linked list.
		 * The allocation is 32 bytes to general 16-byte alignment more efficient.
		 */
		class Allocation
		{
			friend class MemoryManager;
			friend class MemoryBlock;

			public:
				// get info about this allocation
				inline Allocation*	GetPrevious() const			{ return mPrev; }		// NULL when no previous.
				inline Allocation*	GetNext() const				{ return mNext; }		// NULL when no next.
				inline void*		GetMemoryAddress() const	{ return mAddress; }	// The memory address where the data starts.
				inline unsigned int	GetNumBytes() const			{ return mNumBytes; }	// Number of bytes to come after GetMemoryAddress().
				inline unsigned int	GetAlignment() const		{ return mAlignment; }	// The alignment, in bytes.
				inline unsigned int	GetBlockType() const		{ return mBlockType; }	// The block type ID where this allocation should be placed in.
 
			private:
				Allocation*		mPrev;			/**< A pointer to the previous allocation, or NULL when there is no allocation in front of it. */
				Allocation*		mNext;			/**< A pointer to the next allocation, or NULL when there is no allocation after this one. */
				void*			mAddress;		/**< The returned address, which is always aligned according to the alignment specifications made. */
				unsigned int	mNumBytes;		/**< The number of bytes that has been allocated. This is the number of bytes that comes after the mAddress attribute. */
				unsigned int	mCategory;		/**< The memory category of this allocation. */
				unsigned int	mAlignment;		/**< The alignment of the allocation's memory address, in bytes. */
				unsigned int	mBlockType;		/**< The block type for this allocation. */
				unsigned int	mReserved;		/**< Used to align this struct nicely, so currently only used for padding. */
		};

		//-----------------------------------------------------------------

		/**
		 * Allocate memory, like malloc of the C/C++ language.
		 * @param numBytes The number of bytes you'd like to allocate.
		 * @param categoryID The category ID of the allocation. This is used purely for statistics, so that you can see
		 *                   per category statistics.
		 * @param alignment The aligment, in bytes, of the memory start address of the allocation.
		 * @param blockTypeID The type memory block to add this allocation to. This can be used to for example make sure that all
		 *                    allocations made by a String class will end up in the same type of block. On default this value is set to
		 *                    BLOCKTYPE_ANY, which means that the allocation can end up in any block, no matter what its type is.
		 *                    When you create custom types, make sure these type IDs are not equal to zero, since this is the value for BLOCKTYPE_ANY.
		 * @result A pointer to the start memory address of the allocated piece of memory.
		 */
		void* Allocate(unsigned int numBytes, const unsigned int categoryID, const unsigned int alignment=16, unsigned int blockTypeID=BLOCKTYPE_ANY);

		/**
		 * Re-allocate a previously allocated piece of memory, like realloc of the C/C++ language.
		 * The categoryID, alignment and blockType parameters will only have influence when the memory parameter equals NULL.
		 * In all other cases the categoryID, alignment and blockType values of the original allocation are used.
		 * NOTE: it is possible, but not recommended, to reallocate a NULL address. In this case it will behave like a call to Allocate.
		 * @param memory The memory address to reallocate. When NULL is passed, this function behaves like Allocate. Using Allocate however can be much faster!
		 * @param numBytes The new size of the allocation.
		 * @param categoryID The category ID of the allocation. This is used purely for statistics, so that you can see
		 *                   per category statistics.
		 * @param alignment The aligment, in bytes, of the memory start address of the allocation.
		 * @param blockTypeID The type memory block to add this allocation to. This can be used to for example make sure that all
		 *                    allocations made by a String class will end up in the same type of block. On default this value is set to
		 *                    BLOCKTYPE_ANY, which means that the allocation can end up in any block, no matter what its type is.
		 *                    When you create custom types, make sure these type IDs are not equal to zero, since this is the value for BLOCKTYPE_ANY.
		 * @result A pointer to the new memory address of the allocated piece of memory.
		 */
		void* ReAlloc(void* memory, const unsigned int numBytes, const unsigned int categoryID, const unsigned int alignment=16, unsigned int blockTypeID=BLOCKTYPE_ANY);

		/**
		 * Delete/Release/Free an allocated piece of memory.
		 * Of course the passed memory pointer must have been a returned value by either Allocate or ReAllocate.
		 * @param memory The memory address of the allocation you'd like to release.
		 */
		void Delete(void* memory);

		/**
		 * Report the current global memory usage statistics.
		 * This of course only includes memory allocations made using the memory manager and not using global new/delete or malloc/realloc/free.
		 * @param reportLeaks When set to true, the method will also report all current allocations as memory leaks.
		 *                    When set to false, the allocations are ignored and only the statistics are reported.
		 */
		void Report(bool reportLeaks=false);

		/**
		 * Try to find an allocation with a given memory start address.
		 * @param memoryAddress The memory address once returned by Allocate or ReAllocate.
		 * @param outBlock A pointer to a variable that is a pointer to a memory block. This memory block will contain the memory block where
		 *                 this allocation remains inside when the allocation can be found.
		 * @result A pointer to the allocation that has its start address at the given memory address. A value of NULL is returned when the allocation cannot be found.
		 */
		Allocation* FindAllocation(void* memoryAddress, MemoryBlock** outBlock=NULL);

		/**
		 * Get the global memory usage statistics.
		 * @param A reference to the struct that contains all the statistics.
		 */
		const Statistics& GetStatistics() const;

		/**
		 * Generate the statistics for a given memory category.
		 * This means that it will count the memory usage of all allocations made with the given category ID.
		 * @param categoryID The category ID to generate the statistics for.
		 * @result A struct with the statistics for the specified category.
		 */
		CategoryStatistics GenerateCategoryStatistics(const unsigned int categoryID);

		/**
		 * Align a given memory address. This function is static so can also be used for other things.
		 * @param memAddress The address to align.
		 * @param alignment The alignment, in bytes.
		 * @result A pointer to the aligned version of the input memory address. The memory address will be aligned on 'alignment' bytes.
		 */
		static void* AlignMemAddress(void* memAddress, const unsigned int alignment);

		/**
		 * Set the size of the memory blocks in bytes.
		 * The default is set to 64kb.
		 * @param numBytes The number of bytes of memory blocks that will be created from now on.
		 */
		void SetBlockSize(const unsigned int numBytes);

		/**
		 * Get the current memory block sizes in bytes.
		 * This returns the size of the memory blocks that will be created from now on.
		 * On default this value is set to 64kb, but you can adjust this with the SetBlockSize method.
		 * @result The size, in bytes, of memory blocks that will be created when needed.
		 */
		unsigned int GetBlockSize() const;

		/**
		 * Get the number of currently allocated memory blocks.
		 * @result The number of allocated memory blocks.
		 */
		unsigned int GetNumMemoryBlocks() const;

		/**
		 * Get access to a given memory block.
		 * @param nr The memory block number to get.
		 * @result A pointer to the memory block.
		 */
		MemoryBlock* GetMemoryBlock(const int nr) const;


	private:
		Statistics		mStats;				/**< The current global memory usage statistics. */
		MemoryBlock**	mMemoryBlocks;		/**< The array with memory blocks. */
		MemoryBlock*	mCachedBlock;		/**< The last block where an allocation succeeded. */
		MemoryBlock*	mCachedDeleteBlock;	/**< The last block where we deleted from. */
		int				mNumMemoryBlocks;	/**< The number of memory blocks. */
		int				mBlockSize;			/**< The size of memory blocks, in bytes (default is 64kb). */

		/**
		 * The constructor.
		 * It is private because this class is a singleton.
		 */
		MemoryManager();

		/**
		 * The destructor.
		 * It is private because this class is a singleton.
		 */
		~MemoryManager();

		/**
		 * Delete all memory blocks, which means also all allocations will be deleted.
		 */
		void DeleteAllBlocks();

		/**
		 * Add a memory block to the array of memory blocks.
		 * @param block The memory block to add.
		 */
		void AddMemoryBlock(MemoryBlock* block);

		/**
		 * Remove a memory block from the array of memory blocks.
		 * This deletes also the block that is passed as parameter.
		 * @param block The block to remove from the array and to delete from memory.
		 */
		void RemoveMemoryBlock(MemoryBlock* block);

		/**
		 * Add an allocation to the statistics.
		 * @param alloc The allocation to add to the statistics.
		 */
		void AddAllocationToStats(Allocation* alloc);

		/**
		 * Remove an allocation from the statistics.
		 * @param alloc The allocation to remove from the statistics.
		 */
		void RemoveAllocationFromStats(Allocation* alloc);

		/**
		 * Add extra bytes to the statistics (can also be negative in order to remove).
		 * @param extraBytes The amount of bytes to add or remove.
		 */
		void AddExtraAllocBytesToStats(int extraBytes);

		/**
		 * Output some message to the debug window of Visual Studio, the same way printf works.
		 * This is used because the we cannot use the log manager, because that is already destructed before the memory
		 * manager, and the memory manager should always be deleted as last.
		 * @param what The text to display in the output window.
		 */
		void Log(const char* what, ...);

		/**
		 * Delete an allocation inside a given block.
		 * @param allocation The allocation to remove.
		 * @param block The block to remove it from.
		 */
		void Delete(Allocation* allocation, MemoryBlock* block);
};



// if we want to use the memory manager
#ifdef USE_MEMORY_MANAGER
	/**
	 * Specify that a given class has to be managed by the memory manager.
	 * NOTE: You have to make a call to MEMORYCATEGORY(...) too, this can be done outside your class definition too.
	 * Another thing you have to remember is that when you want to use this macro on a template, you have to use the
	 * macro MEMORYOBJECTCATEGORY instead. That same macro can also be used on classes, but requires you to specify the
	 * memory category immediately, which is handled by that macro as well (but not by this MEMORYOBJECT macro!).
	 * Another option is to call MEMORYOBJECTCATEGORY(...) in case you want to immediately specify the memory category as well.
	 * @param CLASSNAME The class name, which would be "FooBar" in case you declare your class as "class FooBar { .... };".
	 * @param ALIGNMENT The alignment, in bytes, on which the memory address should be aligned when you 'new' a class of this type.
	 */
	#define MEMORYOBJECT(CLASSNAME, ALIGNMENT)                     \
		public:                                                    \
		static int CLASSNAME::GetMemoryCategory();                 \
		static int CLASSNAME::GetMemoryAlignment()		           \
		{												           \
			return ALIGNMENT;							           \
		}												           \
                                                                   \
		void* operator new(unsigned int numBytes)                  \
		{                                                          \
			int category  = GetMemoryCategory();                   \
			int alignment = GetMemoryAlignment();                  \
			return MEMMGR.Allocate(numBytes, category, alignment); \
		}                                                          \
                                                                   \
		void* operator new(unsigned int numBytes, void* location)  \
		{                                                          \
			return location;                                       \
		}                                                          \
		                                                           \
		void operator delete(void* memLocation)                    \
		{                                                          \
			MEMMGR.Delete( memLocation );                          \
		}                                                          \
                                                                   \
		void* operator new[](unsigned int numBytes)                \
		{                                                          \
			int category  = GetMemoryCategory();                   \
			int alignment = GetMemoryAlignment();                  \
			return MEMMGR.Allocate(numBytes, category, alignment); \
		}                                                          \
                                                                   \
		void* operator new[](unsigned int numBytes, void* place)   \
		{                                                          \
			return place;                                          \
		}                                                          \
                                                                   \
		void operator delete[](void* memLocation)                  \
		{                                                          \
			MEMMGR.Delete( memLocation );                          \
		}


	/**
	 * Specify that a given class has to be managed by the memory manager.
	 * @param CLASSNAME The class name, which would be "FooBar" in case you declare your class as "class FooBar { .... };".
	 * @param ALIGNMENT The alignment, in bytes, on which the memory address should be aligned when you 'new' a class of this type.
	 * @param CATEGORY An integer containing the memory category ID where this class belongs to.
	 */
	#define MEMORYOBJECTCATEGORY(CLASSNAME, ALIGNMENT, CATEGORY)   \
		public:                                                    \
	    static int GetMemoryCategory() { return CATEGORY; }        \
		static int GetMemoryAlignment()		                       \
		{												           \
			return ALIGNMENT;							           \
		}												           \
                                                                   \
		void* operator new(unsigned int numBytes)                  \
		{                                                          \
			int category  = GetMemoryCategory();                   \
			int alignment = GetMemoryAlignment();                  \
			return MEMMGR.Allocate(numBytes, category, alignment); \
		}                                                          \
                                                                   \
		void* operator new(unsigned int numBytes, void* location)  \
		{                                                          \
			return location;                                       \
		}                                                          \
		                                                           \
		void operator delete(void* memLocation)                    \
		{                                                          \
			MEMMGR.Delete( memLocation );                          \
		}                                                          \
                                                                   \
		void* operator new[](unsigned int numBytes)                \
		{                                                          \
			int category  = GetMemoryCategory();                   \
			int alignment = GetMemoryAlignment();                  \
			return MEMMGR.Allocate(numBytes, category, alignment); \
		}                                                          \
                                                                   \
		void* operator new[](unsigned int numBytes, void* place)   \
		{                                                          \
			return place;                                          \
		}                                                          \
                                                                   \
		void operator delete[](void* memLocation)                  \
		{                                                          \
			MEMMGR.Delete( memLocation );                          \
		}


	/**
	 * Specify the memory category for a given class.
	 * @param CLASSNAME The class name, which would be "FooBar" in case you declare your class as "class FooBar { .... };".
	 * @param CATEGORY An integer containing the memory category ID where this class belongs to.
	 */
	#define MEMORYCATEGORY(CLASSNAME, CATEGORY)   \
		int CLASSNAME::GetMemoryCategory() { return CATEGORY; }

#else	// when we want no memory manager

	#define MEMORYOBJECT(CLASSNAME, ALIGNMENT)
	#define MEMORYOBJECTCATEGORY(CLASSNAME, ALIGNMENT, CATEGORY)
	#define MEMORYCATEGORY(CLASSNAME, CATEGORY)

#endif


}	// namespace MCore

#endif