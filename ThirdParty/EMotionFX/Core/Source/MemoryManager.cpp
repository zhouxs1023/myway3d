// include the memory manager
#include "MemoryManager.h"

// include Core related stuff
#include "Exception.h"
#include "Algorithms.h"
#include <assert.h>

//#include <stddef.h>	// according MSDN for uintPointer, but VC6 doesn't have it in any header


namespace MCore
{

//--------------------------------------------------------------------------------
// class MemoryBlock
//--------------------------------------------------------------------------------

MemoryManager::MemoryBlock::MemoryBlock(const unsigned int numBytes, const bool isPrivate, const unsigned int blockTypeID) 
{
	mMemory			 = NULL;
	mFirstAllocation = NULL;
	mIsPrivate		 = isPrivate;
	mBlockTypeID	 = blockTypeID;

	// perform the allocation
    SetBlockSize( numBytes );
}


MemoryManager::MemoryBlock::~MemoryBlock()
{
	free( mMemory );
}


void MemoryManager::MemoryBlock::SetBlockSize(const unsigned int numBytes)
{
	// update the new block size
	//int oldBytes = mTotalBytes;
	mTotalBytes  = numBytes;

	// resize the memory
	void* newMemory = realloc( mMemory, numBytes );
/*
	// copy over old data when we resize
	if (mMemory != NULL && newMemory != mMemory)
	{
		int bytesToMove = MCore::Min<int>(oldBytes, numBytes);
		memmove(newMemory, mMemory, bytesToMove);
	}
*/
	// update the memory pointer
	mMemory = newMemory;
}


// find an allocation inside a memory block
MemoryManager::Allocation* MemoryManager::MemoryBlock::FindAllocation(void* memoryAddress)
{
	// check if this block can have this memory piece inside it, if not, skip to the next block
	if ((memoryAddress < mMemory) || memoryAddress > ((char*)mMemory + mTotalBytes))
		return NULL;

	// start at the first allocation
	Allocation* curAllocation = mFirstAllocation;

	while (curAllocation)
	{
		// if we found the allocation
		if (curAllocation->mAddress == memoryAddress)
			return curAllocation;

		// next allocation
		curAllocation = curAllocation->mNext;
	}

	// nothing found
	return NULL;
}



// remove the allocation from the block, this assumes the allocation is located inside this block
void MemoryManager::MemoryBlock::RemoveAllocation(MemoryManager::Allocation* allocation, bool* outBlockIsEmpty)
{
	if (allocation->mPrev)
		allocation->mPrev->mNext = allocation->mNext;
	else
		mFirstAllocation = allocation->mNext;

	if (allocation->mNext)
		allocation->mNext->mPrev = allocation->mPrev;

	if (mFirstAllocation == NULL)
		*outBlockIsEmpty = true;
	else
		*outBlockIsEmpty = false;

	//ValidateAllocations();
}


bool MemoryManager::MemoryBlock::RemoveAllocation(void* memAddress, bool* outBlockIsEmpty)
{
	// start at the first allocation
	Allocation* curAllocation = mFirstAllocation;

	while (curAllocation)
	{
		// if we found the allocation
		if (curAllocation->mAddress == memAddress)
		{
			//LOG("Deleting %d byte allocation at address 0x%x",  curAllocation->mNumBytes, curAllocation->mRealAddress);

			// remove the allocation from the linked list
			if (curAllocation->mPrev)
				curAllocation->mPrev->mNext = curAllocation->mNext;
			else
				mFirstAllocation = curAllocation->mNext;

			if (curAllocation->mNext)
				curAllocation->mNext->mPrev = curAllocation->mPrev;

			if (mFirstAllocation == NULL)
				*outBlockIsEmpty = true;
			else
				*outBlockIsEmpty = false;

			//ValidateAllocations();

			// yeah, we removed successfully
			return true;
		}

		// go to the next allocation
		curAllocation = curAllocation->mNext;
	}

	*outBlockIsEmpty = false;

	//ValidateAllocations();

	// we didn't find the allocation inside this block
	return false;
}

/*
void MemoryManager::MemoryBlock::ValidateAllocations()
{
#ifdef USE_MEMORY_MANAGER
	char* memStart = (char*)mMemory;
	char* memEnd   = (char*)mMemory + mTotalBytes;

	Allocation* curAlloc = mFirstAllocation;
	if (curAlloc == NULL)
		return;

	assert((char*)curAlloc >= memStart && (char*)curAlloc < memEnd);

	while (curAlloc)
	{
		assert((char*)curAlloc->mAddress >= memStart && (char*)curAlloc->mAddress < memEnd);

		if (curAlloc->mNext)
		{
			assert(curAlloc->mNext->mPrev == curAlloc);
			assert(((char*)curAlloc->mAddress + curAlloc->mNumBytes) <= (char*)curAlloc->mNext);
		}
		
		if (curAlloc->mPrev)
		{
			assert(curAlloc->mPrev->mNext == curAlloc);
		}

		curAlloc = curAlloc->mNext;
	}
#endif
}
*/



// allocate function
MemoryManager::Allocation* MemoryManager::MemoryBlock::Allocate(const unsigned int numBytes, const unsigned int categoryID, const unsigned int alignment, const unsigned int blockType)
{
	// find the smallest place where we can insert this allocation
	Allocation* curAllocation = mFirstAllocation;

	// the smallest memory hole values we found
	unsigned int	minHoleSize = mTotalBytes;
	void*			minMemStart = mMemory;
	void*			minRealMemStart = mMemory;
	Allocation*		minAlloc	= NULL;
	bool			found		= false;

	// if it is the first allocation
	if (curAllocation == NULL)
	{
		// calculate the overhead due to alignment and how much memory we need to allocate if we want to align
		void* startAddress = (char*)mMemory + sizeof(Allocation);
		void* newAddress = MCore::MemoryManager::AlignMemAddress(startAddress, alignment);
		unsigned int extraBytesRequired = (char*)newAddress - (char*)mMemory;
		unsigned int totalBytesRequired = numBytes + extraBytesRequired;

		// check if this would fit
		if (mTotalBytes >= totalBytesRequired)
		{
			minHoleSize = mTotalBytes;
			minRealMemStart = mMemory;
			minMemStart = newAddress;
			minAlloc    = NULL;
			found		= true;
		}
	}

	// iterate through all allocations within this memory block
	while (curAllocation)
	{
		// calculate the size between the current and previous allocation
		unsigned int holeSize;

		// if this is the first allocation, check the available space in front of this allocation
		if (curAllocation->mPrev == NULL)
		{
			holeSize = (char*)curAllocation - (char*)mMemory;

			// if our allocation fits in this hole and if it is smaller than the smallest one we already found
			if ((numBytes <= holeSize) && (holeSize < minHoleSize) && (holeSize > 0))
			{
				// calculate the overhead due to alignment and how much memory we need to allocate if we want to align
				void* startAddress = (char*)mMemory + sizeof(Allocation);
				void* newAddress = MCore::MemoryManager::AlignMemAddress(startAddress, alignment);
				unsigned int extraBytesRequired = (char*)newAddress - (char*)mMemory;
				unsigned int totalBytesRequired = numBytes + extraBytesRequired;

				// check if this would fit
				if (holeSize >= totalBytesRequired)
				{
					minHoleSize = holeSize;
					minRealMemStart = mMemory;
					minMemStart = newAddress;
					minAlloc    = NULL;
					found		= true;

					// if we found a perfect fit
					if (holeSize == totalBytesRequired)
						break;
				}
			}
		}

		// calculate the amount of available bytes between the two allocations
		if (curAllocation->mNext)
			holeSize = (char*)curAllocation->mNext - ((char*)curAllocation->mAddress + curAllocation->mNumBytes);
		else
			holeSize = ((char*)mMemory + mTotalBytes) - ((char*)curAllocation->mAddress + curAllocation->mNumBytes);

		// if our allocation fits in this hole and if it is smaller than the smallest one we already found
		if ((numBytes <= holeSize) && (holeSize < minHoleSize) && (holeSize > 0))
		{
			// the unaligned memory address
			void* allocationAddress = (char*)curAllocation->mAddress + curAllocation->mNumBytes;
			void* memAddress = (char*)allocationAddress + sizeof(Allocation);
			void* newAddress = MCore::MemoryManager::AlignMemAddress(memAddress, alignment);
			unsigned int extraBytesRequired = (char*)newAddress - (char*)allocationAddress;
			unsigned int totalBytesRequired = numBytes + extraBytesRequired;

			if (holeSize >= totalBytesRequired)
			{
				minHoleSize = holeSize;
				minRealMemStart = allocationAddress;
				minMemStart = newAddress;
				minAlloc    = curAllocation;
				found		= true;

				// if we found a perfect fit
				if (holeSize == totalBytesRequired)
					break;
			}
		}

		// go to the next allocation
		curAllocation = curAllocation->mNext;
	}

	// when we can't fit the allocation inside this block, return NULL
	if (found == false && mFirstAllocation != NULL)
		return NULL;

	// create the allocation
	Allocation* newAlloc= (Allocation*)minRealMemStart;
	newAlloc->mNext		= NULL;
	newAlloc->mPrev		= NULL;
	newAlloc->mAddress	= minMemStart;
	newAlloc->mCategory = categoryID;
	newAlloc->mNumBytes = numBytes;
	newAlloc->mAlignment= alignment;
	newAlloc->mBlockType= blockType;

	// update the linked list
	if (minAlloc != NULL)
	{
		newAlloc->mPrev = minAlloc;
		newAlloc->mNext	= minAlloc->mNext;
	}
	else
	{
		newAlloc->mPrev = NULL;
		newAlloc->mNext	= mFirstAllocation;

		if (newAlloc->mNext)
			newAlloc->mNext->mPrev = newAlloc;
	}

	if (newAlloc->mNext == mFirstAllocation)
		mFirstAllocation = newAlloc;

	if (mFirstAllocation == NULL)
		mFirstAllocation = newAlloc;

	if (minAlloc)
	{
		if (minAlloc->mNext)
			minAlloc->mNext->mPrev = newAlloc;

		minAlloc->mNext	= newAlloc;
	}

	//ValidateAllocations();

//	LOG("Allocated %d bytes at address 0x%x in block 0x%x", numBytes, newAlloc->mAddress, mMemory);

	// return the allocation
	return newAlloc;
}



//--------------------------------------------------------------------------------
// class MemoryManager
//--------------------------------------------------------------------------------

// constructor
MemoryManager::MemoryManager()
{
	// init memory blocks
	mNumMemoryBlocks	= 0;
	mMemoryBlocks		= NULL;
	mCachedBlock		= NULL;
	mCachedDeleteBlock	= NULL;
	mBlockSize			= 64 * 1024;	// 64 kb

	// initialize the statistics
	mStats.mNumAllocations			= 0;
	mStats.mNumBlocks				= 0;
	mStats.mTotalBlockMem			= 0;
	mStats.mAllocatedBytes			= 0;
	mStats.mBlockOverhead			= 0;
	mStats.mAlignmentOverhead		= 0;
	mStats.mManagementOverhead		= 0;
	mStats.mAvgAllocSize			= 0;
	mStats.mAvgBlockSize			= 0;
	mStats.mAvgBlockOverheadPerAlloc= 0;
	mStats.mAvgAlignmentOverhead	= 0;
	mStats.mMaxBlockMem				= 0;
	mStats.mMaxAllocatedBytes		= 0;
	mStats.mMaxNumAllocations		= 0;
	mStats.mMaxNumBlocks			= 0;

	// we want to destruct the memory manager as last, so set the destruction phase to a high value
	MCore::Singleton< MCore::MemoryManager >::SetDestructionPhase( 10000 );
}


// destructor
MemoryManager::~MemoryManager()
{
	// if there are still blocks, it means there are memory leaks, so report them!
	if (mNumMemoryBlocks > 0)
	{
		Report(true);
		Log("MemoryManager: Releasing unreleased memory from detected memory leaks...\n");
	}

	// delete all memory blocks
	DeleteAllBlocks();
}


// delete all memory blocks
void MemoryManager::DeleteAllBlocks()
{
	// delete all the blocks
	for (int i=0; i<mNumMemoryBlocks; i++)
	{
		delete mMemoryBlocks[i];
		mMemoryBlocks[i] = NULL;
	}

	// delete the array itself
	free(mMemoryBlocks);
	mMemoryBlocks = NULL;

	// update the statistics
	mStats.mNumBlocks		= 0;
	mStats.mTotalBlockMem	= 0;
	AddExtraAllocBytesToStats( 0 );	// update the averages
}


// find the allocation for a given memory address
MemoryManager::Allocation* MemoryManager::FindAllocation(void* memoryAddress, MemoryManager::MemoryBlock** outBlock)
{
	// if we try to delete a NULL pointer, simply return
	if (memoryAddress == NULL)
		return NULL;

	// try to remove the allocation
	const int numBlocks = mNumMemoryBlocks;
	for (int i=0; i<numBlocks; i++)
	{
		MemoryBlock* block = mMemoryBlocks[i];

		// check if this block can have this memory piece inside it, if not, skip to the next block
		if ((memoryAddress < block->mMemory) || memoryAddress > ((char*)block->mMemory + block->mTotalBytes))
			continue;
		
		// search through all allocations inside this block
		Allocation* curAlloc = block->mFirstAllocation;
		while (curAlloc)
		{
			// if we found the allocation, return it
			if (curAlloc->mAddress == memoryAddress)
			{
				if (outBlock)
					*outBlock = block;

				return curAlloc;
			}

			// go to the next allocation
			curAlloc = curAlloc->mNext;
		}
	}

	// not found
	if (outBlock)
		*outBlock = NULL;

	return NULL;
}


// allocate memory
void* MemoryManager::Allocate(unsigned int numBytes, const unsigned int categoryID, const unsigned int alignment, unsigned int blockTypeID)
{
#ifdef USE_MEMORY_MANAGER
	//if (mStats.mAllocatedBytes > 7000 * 1024)
		//Log("Allocation of %d bytes in category %d", numBytes, categoryID);

	// check if we exceed the max block size
	// if so, create a new block with just this memory in it
	if (numBytes > (unsigned int)mBlockSize)
	{
		AddMemoryBlock( new MemoryBlock(numBytes + sizeof(Allocation) + alignment, true, blockTypeID) );	// + 256 bytes for some overhead for allocation structs
		MemoryBlock* block = mMemoryBlocks[ mNumMemoryBlocks-1 ];
		Allocation*  alloc = block->Allocate( numBytes, categoryID, alignment );

		// update the statistics
		AddAllocationToStats( alloc );

		return alloc->mAddress;
	}


	// try the cached memory block first
	if (mCachedBlock && (mCachedBlock->mIsPrivate==false) && (mCachedBlock->mBlockTypeID == blockTypeID))
	{
		Allocation* allocation = mCachedBlock->Allocate( numBytes, categoryID, alignment );
		if (allocation)
		{
			// update the statistics
			AddAllocationToStats( allocation );
			return allocation->mAddress;
		}
	}


	// try to find a block where it fits
	const int numBlocks = mNumMemoryBlocks;
	for (int i=0; i<numBlocks; i++)
	{
		MemoryBlock* block = mMemoryBlocks[i];
		if (block == mCachedBlock) continue;	// skip the already searched cached block
		if (block->mBlockTypeID != blockTypeID) continue;	// skip blocks that don't allow this allocation in it
		if (block->mIsPrivate) continue;		// skip private blocks

		// try to allocate inside the current block
		Allocation* allocation = block->Allocate( numBytes, categoryID, alignment );
		if (allocation)
		{
			mCachedBlock = block;

			// update the statistics
			AddAllocationToStats( allocation );
			return allocation->mAddress;
		}
	}

	// create a new block, because the rest seems full
	AddMemoryBlock( new MemoryBlock(mBlockSize + sizeof(Allocation)+alignment, false, blockTypeID) );
	MemoryBlock* block = mMemoryBlocks[ mNumMemoryBlocks-1 ];
	Allocation*  alloc = block->Allocate( numBytes, categoryID, alignment );
	mCachedBlock = block;
	assert(alloc != NULL);

	// update the statistics
	AddAllocationToStats( alloc );

	return alloc->mAddress;
#else
	return malloc(numBytes);
#endif
}


// realloc (resize) a given allocation
void* MemoryManager::ReAlloc(void* memory, const unsigned int numBytes, const unsigned int categoryID, const unsigned int alignment, const unsigned int blockTypeID)
{
#ifdef USE_MEMORY_MANAGER
	DECLARE_FUNCTION(ReAlloc)

	// TODO: what about if it is a huge alloc with its own block
	// we should then just realloc the block memory to enlarge it

	// make it behave like Allocate (or malloc) when the memory address is NULL
	if (memory == NULL)
		return Allocate(numBytes, categoryID, alignment, blockTypeID);

	// check if we want to delete the allocation
	if (numBytes == 0)
	{
		// look if we have an allocation with this address
		MemoryBlock* block;
		Allocation* allocation = FindAllocation(memory, &block);

		// if so, remove it, since we want to resize it to a size of 0
		if (allocation)
		{
			// remove the allocation from the stats
			RemoveAllocationFromStats( allocation );
			
			// delete the allocation itself
			bool isEmpty = false;
			block->RemoveAllocation(memory, &isEmpty);

			// if we can remove the block, because it is empty
			if (isEmpty)
			{
				RemoveMemoryBlock( block );
				AddExtraAllocBytesToStats( 0 );	// just to update the averages, because the block memory changed
			}
		}

		return NULL;
	}

	// try to find the allocation
	MemoryBlock* block = NULL;
	Allocation* allocation = FindAllocation(memory, &block);

	// oops, we cannot find the allocation, so the user is trying to realloc a piece of memory that hasn't even been allocated yet
	if (allocation == NULL)
	{
		Log("MemoryManager: Trying to realloc a memory address (0x%x) that isn't allocated or has already been deleted!", memory);
		throw Exception("MemoryManager: Trying to realloc memory that hasn't been allocated yet and that is not NULL!");
		//return NULL;
	}

	// now figure out if we can enlarge this allocation, check if there is space after it
	int spaceAfter;
	if (allocation->mNext)
		spaceAfter = (char*)allocation->mNext - ((char*)allocation->mAddress + allocation->mNumBytes);
	else
		spaceAfter = ((char*)block->mMemory + block->mTotalBytes) - ((char*)allocation->mAddress + allocation->mNumBytes);


	// calculate the total space that is free before and after our allocation
	int extraSpaceNeeded = numBytes - allocation->mNumBytes;

	// if we cannot resize the allocation, because it wouldn't fit
	if (spaceAfter < extraSpaceNeeded)
	{
		if ((int)numBytes > mBlockSize)
		{
			void* oldAddress     = allocation->mAddress;
			unsigned int oldSize = allocation->mNumBytes;

			// return the result of a new allocation
			const int cachedBufferSize = 1024 * 10;
			void* result = Allocate(numBytes + cachedBufferSize, allocation->mCategory, allocation->mAlignment, allocation->mBlockType);

			// copy the old contents
			unsigned int numBytesToMove = MCore::Min<unsigned int>(numBytes, oldSize);
			if (result != oldAddress)
				memmove(result, oldAddress, numBytesToMove);	

			// remove the allocation
			Delete(allocation, block);

			return result;
		}
		else
		{
			void* oldAddress     = allocation->mAddress;
			unsigned int oldSize = allocation->mNumBytes;

			// return the result of a new allocation
			void* result = Allocate(numBytes, allocation->mCategory, allocation->mAlignment, allocation->mBlockType);

			// copy the old contents
			unsigned int numBytesToMove = MCore::Min<unsigned int>(numBytes, oldSize);
			if (result != oldAddress)
				memmove(result, oldAddress, numBytesToMove);	

			// remove the allocation
			Delete(allocation, block);

			return result;
		}
	}
	else	// it fits, so just resize the allocation
	{	
		AddExtraAllocBytesToStats( extraSpaceNeeded );
		allocation->mNumBytes += extraSpaceNeeded;
		return allocation->mAddress;
	}
#else
	return realloc(memory, numBytes);
#endif
}



// delete/free/release allocated memory
void MemoryManager::Delete(void* memory)
{
#ifdef USE_MEMORY_MANAGER
	DECLARE_FUNCTION(Delete);

	// if we try to delete a NULL pointer, simply return
	if (memory == NULL)
		return;

	// try to find it in the cached delete block
	MemoryBlock* block = NULL;
	Allocation* result = NULL;

	// try the cached memory block first
	if (mCachedDeleteBlock)
	{
		result = mCachedDeleteBlock->FindAllocation(memory);
		if (result) 
			block = mCachedDeleteBlock;
	}

	// if it isn't in the cached block
	if (result == NULL)
	{
		bool found = false;
		const int numBlocks = mNumMemoryBlocks;
		for (int i=0; i<numBlocks && !found; i++)
		{
			// skip the cached block, since we already processed it
			if (mMemoryBlocks[i] == mCachedDeleteBlock) continue;

			// try to locate the allocation inside the current block
			result = mMemoryBlocks[i]->FindAllocation(memory);
			if (result)
			{
				block = mMemoryBlocks[i];
				mCachedDeleteBlock = block;
				found = true;
			}
		}
	}

	// if we haven't found the allocation, then it means we are trying to delete something that doesn't exist inside the memory manager
	if (result == NULL)
	{
		Log("MemoryManager - Trying to delete memory that does not exist or that has already been deleted!");
		throw Exception("MemoryManager - Trying to delete memory that does not exist or that has already been deleted!");
	}

	// remove the allocation from the stats
	RemoveAllocationFromStats( result );
	
	// delete the allocation itself
	bool isEmpty = false;
	block->RemoveAllocation(result, &isEmpty);

	// if we can remove the block, because it is empty
	if (isEmpty)
	{
		RemoveMemoryBlock( block );
		AddExtraAllocBytesToStats( 0 );	// just to update the averages, because the block memory changed
	}
#else
	free(memory);
#endif
}



// report memory statistics
void MemoryManager::Report(bool reportLeaks)
{
	Log("\n");
	Log("------------------------------------------------------------------");
	Log("MEMORY MANAGER REPORT");
	Log("------------------------------------------------------------------");
	Log("Num blocks............................ = %d", mStats.mNumBlocks);
	Log("Num allocations....................... = %d", mStats.mNumAllocations);
	Log("Allocated bytes....................... = %d bytes (%d kb / %.3f mb)", mStats.mAllocatedBytes, mStats.mAllocatedBytes / 1024, mStats.mAllocatedBytes / (float)(1024.0f*1024.0f));
	Log("Allocated block mem................... = %d bytes (%d kb / %.3f mb)", mStats.mTotalBlockMem, mStats.mTotalBlockMem / 1024, mStats.mTotalBlockMem / (float)(1024.0f*1024.0f));

	if (mStats.mNumAllocations > 0)
	Log("Block overhead........................ = %d bytes (%d kb / %.3f mb / %.2f%%)", mStats.mBlockOverhead, mStats.mBlockOverhead / 1024, mStats.mBlockOverhead / (float)(1024.0f*1024.0f), (mStats.mBlockOverhead / (float)mStats.mTotalBlockMem) * 100);
	else
	Log("Block overhead........................ = %d bytes (%d kb / %.3f mb / %.2f%%)", mStats.mBlockOverhead, mStats.mBlockOverhead / 1024, mStats.mBlockOverhead / (float)(1024.0f*1024.0f), 0.0f);

	Log("Alignment overhead.................... = %d bytes (%d kb)", mStats.mAlignmentOverhead, mStats.mAlignmentOverhead / 1024);
	Log("Allocation management overhead........ = %d bytes (%d kb)", mStats.mManagementOverhead, mStats.mManagementOverhead / 1024);
	Log("Average alloc size.................... = %d bytes (%d kb / %.3f mb)", mStats.mAvgAllocSize, mStats.mAvgAllocSize / 1024, mStats.mAvgAllocSize / (float)(1024.0f*1024.0f));
	Log("Average block size.................... = %d bytes (%d kb / %.3f mb)", mStats.mAvgBlockSize, mStats.mAvgBlockSize / 1024, mStats.mAvgBlockSize / (float)(1024.0f*1024.0f));

	if (mStats.mNumAllocations > 0)
	Log("Average block overhead per alloc...... = %d bytes (%d kb / %.3f mb / %.2f%%)", mStats.mAvgBlockOverheadPerAlloc, mStats.mAvgBlockOverheadPerAlloc / 1024, mStats.mAvgBlockOverheadPerAlloc / (float)(1024.0f*1024.0f), (mStats.mAvgBlockOverheadPerAlloc / (float)mStats.mAvgAllocSize) * 100);
	else
	Log("Average block overhead per alloc...... = %d bytes (%d kb / %.3f mb / %.2f%%)", mStats.mAvgBlockOverheadPerAlloc, mStats.mAvgBlockOverheadPerAlloc / 1024, mStats.mAvgBlockOverheadPerAlloc / (float)(1024.0f*1024.0f), 0.0f);

	Log("Average alignment overhead per alloc.. = %d bytes", mStats.mAvgAlignmentOverhead);
	Log("Maximum allocated memory.............. = %d bytes (%d kb / %.3f mb)", mStats.mMaxAllocatedBytes, mStats.mMaxAllocatedBytes / 1024, mStats.mMaxAllocatedBytes / (float)(1024*1024));
	Log("Maximum allocated block memory........ = %d bytes (%d kb / %.3f mb)", mStats.mMaxBlockMem, mStats.mMaxBlockMem / 1024, mStats.mMaxBlockMem / (float)(1024*1024));
	Log("Maximum number of allocations......... = %d", mStats.mMaxNumAllocations);
	Log("Maximum number of memory blocks....... = %d", mStats.mMaxNumBlocks);

/*
	// display block details (used to check the memory managers behavior)
	const int numBlocks = mNumMemoryBlocks;
	for (int b=0; b<numBlocks; b++)
	{
		MemoryBlock* block = mMemoryBlocks[b];
		unsigned int numBytes = 0;
		unsigned int numAllocs = 0;

		Allocation* firstAlloc = block->mFirstAllocation;
		while (firstAlloc)
		{
			numBytes += firstAlloc->mNumBytes;
			firstAlloc = firstAlloc->mNext;
			numAllocs++;
		}

		Log("%d kb with type %d (private=%d) numBytes=%d (%d kb) (numAllocs=%d)", block->mTotalBytes/1024, block->mBlockTypeID, block->mIsPrivate, numBytes, numBytes/1024, numAllocs);
	}
*/

	// display the memory leaks
	if (mStats.mNumAllocations > 0 && reportLeaks)
	{
		char buffer[128];
		Log("\n");
		Log("*** THE FOLLOWING %d MEMORY LEAK(S) HAVE BEEN DETECTED (totally %d bytes / %d kb): ***", mStats.mNumAllocations, mStats.mAllocatedBytes, mStats.mAllocatedBytes / 1024);
		int leakNr = 0;
		const int numBlocks = mNumMemoryBlocks;
		for (int i=0; i<numBlocks; i++)
		{
			MemoryBlock* block = mMemoryBlocks[i];
			Allocation*  curAlloc = block->mFirstAllocation;

			// loop through all allocations
			while (curAlloc)
			{
				leakNr++;

				// build a buffer that represents the data near the start of the memory leak
				memset(buffer, 0, 128);
				int bufferBytes = min(curAlloc->mNumBytes, 127);
				for (int b=0; b<bufferBytes; b++)
				{
					char c = *((char*)curAlloc->mAddress + b);
					if (c < 16)
						c = '.';

					buffer[b] = c;
				}
				
				// output the memory leak information
				Log("[MEMLEAK #%d] %d bytes at memory location 0x%x with category %d [data='%s']", leakNr, curAlloc->mNumBytes, curAlloc->mAddress, curAlloc->mCategory, buffer);

				// go to the next allocation
				curAlloc = curAlloc->mNext;
			}
		}
		Log("\n");
	}
	else
	{
		if (reportLeaks)
			Log("No memory leaks have been detected...");
		else
			Log("\n");
	}
}


// get the statistics
const MemoryManager::Statistics& MemoryManager::GetStatistics() const
{
	return mStats;
}


// add a memory block
void MemoryManager::AddMemoryBlock(MemoryBlock* block)
{
	mMemoryBlocks = (MemoryBlock**)realloc(mMemoryBlocks, (mNumMemoryBlocks+1) * sizeof(MemoryBlock*));
	mMemoryBlocks[mNumMemoryBlocks] = block;
	mNumMemoryBlocks++;

	// update the stats
	mStats.mNumBlocks++;
	mStats.mTotalBlockMem += block->mTotalBytes;

	if (mStats.mTotalBlockMem > mStats.mMaxBlockMem)
		mStats.mMaxBlockMem = mStats.mTotalBlockMem;
}


// remove a memory block
void MemoryManager::RemoveMemoryBlock(MemoryBlock* block)
{
	// find the index
	int index = -1;
	for (int i=0; i<mNumMemoryBlocks && (index==-1); i++)
		if (block == mMemoryBlocks[i])
			index = i;

	// make sure we don't try to delete something we don't have
	assert(index != -1);

	mStats.mNumBlocks--;
	mStats.mTotalBlockMem -= block->mTotalBytes;

	if (block == mCachedBlock)
		mCachedBlock = NULL;

	if (block == mCachedDeleteBlock)
		mCachedDeleteBlock = NULL;

	// delete the block from memory
	delete block;
	memmove(mMemoryBlocks+index, mMemoryBlocks+index+1, (mNumMemoryBlocks-index-1) * sizeof(MemoryBlock*));

	// shrink the array by one item
	mNumMemoryBlocks--;
	mMemoryBlocks = (MemoryBlock**)realloc(mMemoryBlocks, mNumMemoryBlocks * sizeof(MemoryBlock*));
}


MemoryManager::CategoryStatistics MemoryManager::GenerateCategoryStatistics(unsigned int categoryID)
{
	// reset the stats
	MemoryManager::CategoryStatistics stats;
	stats.mCategoryID	  = categoryID;
	stats.mNumAllocations = 0;
	stats.mAllocatedBytes = 0;
	stats.mAvgAllocSize   = 0;

	// process all blocks
	const int numBlocks = mNumMemoryBlocks;
	for (int i=0; i<numBlocks; i++)
	{
		MemoryBlock* block = mMemoryBlocks[i];

		// process all the allocations
		Allocation* curAlloc = block->mFirstAllocation;
		while (curAlloc)
		{
			// if this allocation is of the same category we are interested in
			if (curAlloc->mCategory == categoryID)
			{
				stats.mNumAllocations++;
				stats.mAllocatedBytes += curAlloc->mNumBytes;
			}

			// next allocation
			curAlloc = curAlloc->mNext;
		}
	}

	// calculate the average allocation size
	if (stats.mNumAllocations > 0)
		stats.mAvgAllocSize = stats.mAllocatedBytes / stats.mNumAllocations;
	else
		stats.mAvgAllocSize = 0;

	return stats;
}


// align a memory address and return the aligned version
void* MemoryManager::AlignMemAddress(void* memAddress, const unsigned int alignment)
{
	unsigned int remainder = (uintPointer)memAddress % alignment;
	if (remainder == 0)	remainder = alignment;
	void* result = (unsigned char*)memAddress + (alignment - remainder);
	return result;
}


// add an allocation to the statistics
void MemoryManager::AddAllocationToStats(Allocation* alloc)
{
	mStats.mNumAllocations++;
	mStats.mAllocatedBytes			+= alloc->mNumBytes;
	mStats.mAlignmentOverhead		+= (char*)alloc->mAddress - ((char*)alloc + sizeof(Allocation));
	mStats.mBlockOverhead			= mStats.mTotalBlockMem - (mStats.mAllocatedBytes + mStats.mAlignmentOverhead);
	mStats.mAvgBlockSize			= mStats.mNumBlocks ? (mStats.mTotalBlockMem / mStats.mNumBlocks) : 0;
	mStats.mAvgAllocSize			= mStats.mNumAllocations ? (mStats.mAllocatedBytes / mStats.mNumAllocations) : 0;
	mStats.mAvgAlignmentOverhead	= mStats.mNumAllocations ? (mStats.mAlignmentOverhead / mStats.mNumAllocations) : 0;
	mStats.mAvgBlockOverheadPerAlloc= mStats.mNumAllocations ? (mStats.mBlockOverhead / mStats.mNumAllocations) : 0;
	mStats.mManagementOverhead		= mStats.mNumAllocations * sizeof(Allocation);

	// update the maximum values
	mStats.mMaxAllocatedBytes = MCore::Max<unsigned int>(mStats.mAllocatedBytes, mStats.mMaxAllocatedBytes);
	mStats.mMaxBlockMem       = MCore::Max<unsigned int>(mStats.mTotalBlockMem, mStats.mMaxBlockMem);
	mStats.mMaxNumAllocations = MCore::Max<unsigned int>(mStats.mNumAllocations, mStats.mMaxNumAllocations);
	mStats.mMaxNumBlocks      = MCore::Max<unsigned int>(mStats.mNumBlocks, mStats.mMaxNumBlocks);
}


// remove an allocation from the stats
void MemoryManager::RemoveAllocationFromStats(Allocation* alloc)
{
	mStats.mNumAllocations--;
	mStats.mAllocatedBytes			-= alloc->mNumBytes;
	mStats.mAlignmentOverhead		-= (char*)alloc->mAddress - ((char*)alloc + sizeof(Allocation));
	mStats.mBlockOverhead			= mStats.mTotalBlockMem - (mStats.mAllocatedBytes + mStats.mAlignmentOverhead);
	mStats.mAvgBlockSize			= mStats.mNumBlocks ? (mStats.mTotalBlockMem / mStats.mNumBlocks) : 0;
	mStats.mAvgAllocSize			= mStats.mNumAllocations ? (mStats.mAllocatedBytes / mStats.mNumAllocations) : 0;
	mStats.mAvgAlignmentOverhead	= mStats.mNumAllocations ? (mStats.mAlignmentOverhead / mStats.mNumAllocations) : 0;
	mStats.mAvgBlockOverheadPerAlloc= mStats.mNumAllocations ? (mStats.mBlockOverhead / mStats.mNumAllocations) : 0;
	mStats.mManagementOverhead		= mStats.mNumAllocations * sizeof(Allocation);

	// update the maximum values
	mStats.mMaxAllocatedBytes = MCore::Max<unsigned int>(mStats.mAllocatedBytes, mStats.mMaxAllocatedBytes);
	mStats.mMaxBlockMem       = MCore::Max<unsigned int>(mStats.mTotalBlockMem, mStats.mMaxBlockMem);
	mStats.mMaxNumAllocations = MCore::Max<unsigned int>(mStats.mNumAllocations, mStats.mMaxNumAllocations);
	mStats.mMaxNumBlocks      = MCore::Max<unsigned int>(mStats.mNumBlocks, mStats.mMaxNumBlocks);
}


// add extra allocation space (used on reallocs)
void MemoryManager::AddExtraAllocBytesToStats(int extraBytes)
{
	mStats.mAllocatedBytes			+= extraBytes;
	mStats.mBlockOverhead			= mStats.mTotalBlockMem - (mStats.mAllocatedBytes + mStats.mAlignmentOverhead);
	mStats.mAvgBlockSize			= mStats.mNumBlocks ? (mStats.mTotalBlockMem / mStats.mNumBlocks) : 0;
	mStats.mAvgAllocSize			= mStats.mNumAllocations ? (mStats.mAllocatedBytes / mStats.mNumAllocations) : 0;
	mStats.mAvgAlignmentOverhead	= mStats.mNumAllocations ? (mStats.mAlignmentOverhead / mStats.mNumAllocations) : 0;
	mStats.mAvgBlockOverheadPerAlloc= mStats.mNumAllocations ? (mStats.mBlockOverhead / mStats.mNumAllocations) : 0;

	// update the maximum values
	mStats.mMaxAllocatedBytes = MCore::Max<unsigned int>(mStats.mAllocatedBytes, mStats.mMaxAllocatedBytes);
	mStats.mMaxBlockMem       = MCore::Max<unsigned int>(mStats.mTotalBlockMem, mStats.mMaxBlockMem);
	mStats.mMaxNumAllocations = MCore::Max<unsigned int>(mStats.mNumAllocations, mStats.mMaxNumAllocations);
	mStats.mMaxNumBlocks      = MCore::Max<unsigned int>(mStats.mNumBlocks, mStats.mMaxNumBlocks);
}


// log a message to the debug output
void MemoryManager::Log(const char* what, ...)
{
#if PLATFORM == PLATFORM_WINDOWS
	DECLARE_FUNCTION(Log)

	char textBuf[4096];
	va_list args;
	va_start(args, what);

	vsprintf(textBuf, what, args);
	va_end(args);

	int len = (int)strlen(textBuf);
	textBuf[len++] = '\n';
	textBuf[len] = '\0';

	OutputDebugString(textBuf);
#endif
}


void MemoryManager::SetBlockSize(const unsigned int numBytes)
{
	assert(numBytes >= 0);
	mBlockSize = numBytes;
}


unsigned int MemoryManager::GetBlockSize() const
{
	return mBlockSize;
}


unsigned int MemoryManager::GetNumMemoryBlocks() const				
{ 
	return mNumMemoryBlocks; 
}


MemoryManager::MemoryBlock* MemoryManager::GetMemoryBlock(const int nr) const		
{ 
	return mMemoryBlocks[nr]; 
}


void MemoryManager::Delete(MemoryManager::Allocation* allocation, MemoryManager::MemoryBlock* block)
{
	// remove the allocation from the stats
	RemoveAllocationFromStats( allocation );
	
	// delete the allocation itself
	bool isEmpty = false;
	block->RemoveAllocation(allocation, &isEmpty);

	mCachedDeleteBlock = block;

	// if we can remove the block, because it is empty
	if (isEmpty)
	{
		//Log("Removing memory block in MemoryManager::Delete");
		RemoveMemoryBlock( block );
		AddExtraAllocBytesToStats( 0 );	// just to update the averages, because the block memory changed
	}
}





}	// namespace MCore