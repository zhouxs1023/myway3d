/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __IMPORTER_H
#define __IMPORTER_H

// include the Core system
#include "../../Core/Source/MCore.h"

// include the chunk processors
#include "ChunkProcessors.h"
#include "../MemoryCategories.h"


namespace EMotionFX
{

// forward declarations
class Actor;
class Node;
class Motion;
class FacialMotion;
class MotionPart;

// a macro for easy access
#define IMPORTER Importer::GetInstance()


/**
 * The chunk iterator will be used to process a specified type of chunks from a file.
 * For example you could use the iterator to get all nodes from the file after each other, even if there aren't
 * after each other.
 *
 * <pre>
 * MCore::DiskFile* file = new DiskFile;
 * file->Open("Electranna.lma", MCore::DiskFile::EMode::READ, true);
 * ChunkIterator iter(LMA_CHUNK_NODE, file);
 * for (file=iter.Next(); !iter.IsDone(); file=iter.Next())
 * {
 *    LOG("ITERATOR: Node found. Skipping.");
 *    if (file)
 *	     file->Forward(sizeof(LMA_Node));
 * }
 * file->Close();
 * </pre>
 */
class ChunkIterator
{
	DECLARE_CLASS(ChunkIterator);
	MEMORYOBJECTCATEGORY(ChunkIterator, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_IMPORTER);

	public:
		/**
		* Constructor.
		* The iterator won't open the file, so do that before.
		* @param chunkFilterType The iterator will only stop iteration at chunks which have the specified chunk id.
		*						  All chunks with that id will be filtered out and get returned by the Next() function.
		* @param file The file to iterate.
		*/
		ChunkIterator(const int chunkFilterType, MCore::File* file);

		/**
		* Destructor.
		*/
		virtual ~ChunkIterator();

		/**
		* Step to next chunk of the iterator's type.
		* Return the file at current position so that chunk processors are able to process the chunk. If you don't use
		* chunk processors skip the file yourself via file->Forward(), else it will end up in an unlimited loop.
		* @param chunkSize The size of the following chunk in bytes.
		* @return A pointer to the file which includes the current(seeked by the iterator) position.
		*/
		MCore::File* Next(int& chunkSize);

		/**
		* Equal to the file's end of file function. Return true if the end of the file has been reached, false
		* if not.
		* @return True if the end of the file has been reached, false if not.
		*/
		bool IsDone();

		/**
		* Get the iteration chunk filter/id.
		* @return The id at which the iteration will be stopped so that you can read in that chunks.
		*/
		int GetChunkFilterType() const;

	private:
		MCore::File*			mFile;				/**< The opened file. */
		int						mChunkFilterType;	/**< Iterator's chunk type. Only this type of chunks will be iterated. */
		LMA_Header				mLMAHeader;			/**< The LMA header. */
};



/**
 * EMotion FX Actor & Animation importer.
 * This class can be used to load actors and motions from any kind of resource (from a MCore::File).
 * Also it provides easy functions to load from disk and from memory.
 * The class is a Singleton, which means that it is some global manager. Here is an example on 
 * how to use the class:
 * 
 * Actor *actor = Importer::Instance().LoadActor("TestActor.lma", "Some Test Actor");
 *
 * You can also use the IMPORTER macro. The same can then be done on this way:
 *
 * Actor *actor = IMPORTER.LoadActor("TestActor.lma", "Some Test Actor");
 */
class Importer
{
	DECLARE_CLASS(Importer);
	MEMORYOBJECTCATEGORY(Importer, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_IMPORTER);
	SINGLETON(Importer);

	public:
		/**
		 * Shared importer data class.
		 * Chunks can load data, which might be shared between other chunks during import.
		 * All types of shared data are inherited from this class, and can be added to the importer
		 * during import time.
		 */
		class SharedData
		{
			MEMORYOBJECTCATEGORY(SharedData, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_IMPORTER);

			public:
				/**
				 * The constructor.
				 */
				SharedData() {}

				/**
				 * The destructor.
				 */
				virtual ~SharedData() { Reset(); }

				/**
				 * Get the unique attribute type.
				 * @result The unique attribute ID.
				 */
				virtual int	GetType() const = 0;

				/**
				 * Get rid of the information provided by the shared data.
				 */
				virtual void Reset() {}
		};

		//-----------------------------------------------------------------------------------------

		/**
		 * Shared hierarchy information.
		 */
		class SharedHierarchyInfo : public SharedData
		{
			MEMORYOBJECTCATEGORY(SharedHierarchyInfo, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_IMPORTER);

			public:
				// the type returned by GetType()
				enum { TYPE_ID = 0x00000001 };

				/**
				 * The constructor.
				 */
				SharedHierarchyInfo() : mLastNode(NULL) {}

				/**
				 * The destructor.
				 */
				virtual ~SharedHierarchyInfo() {}

				/**
				 * Get the unique attribute type.
				 * @result The unique attribute ID.
				 */
				int	GetType() const		{ return TYPE_ID; }

				/**
				 * Get rid of the information provided by the
				 * shared data. Reset it.
				 */
				void Reset()
				{
					mLastNode = NULL;
					mNodes.Clear();
					mLinks.Clear();
				}

				/**
				 * The struct holding the linking information.
				 * This is used for linking the childs to their parents, etc.
				 */
				struct Link 
				{ 
					Node*			mNode;		/**< The child node. */
					MCore::String	mParent;	/**< The name of its parent. */
				};

			public:
				MCore::Array<Node*>	mNodes;			/**< All loaded nodes until now. */
				MCore::Array<Link>	mLinks;			/**< The hierarchy information. */
				Node*				mLastNode;		/**< The last loaded node. */
		};

		//-----------------------------------------------------------------------------------------

		/**
		 * Shared motion information.
		 */
		class SharedMotionInfo : public SharedData
		{
			MEMORYOBJECTCATEGORY(SharedMotionInfo, EMFX_DEFAULT_ALIGNMENT, MEMCATEGORY_IMPORTER);

			public:
				// the type returned by GetType()
				enum { TYPE_ID = 0x00000002 };

				/**
				 * The constructor.
				 */
				SharedMotionInfo() : mLastPart(NULL) {}

				/**
				 * The destructor.
				 */
				virtual ~SharedMotionInfo() {}

				/**
				 * Get the unique attribute type.
				 * @result The attribute ID.
				 */
				int	GetType() const		{ return TYPE_ID; }

				/**
				 * Get rid of the information provided by the shared data.
				 */
				void Reset()			{ mLastPart = NULL; }

			public:
				MotionPart*	mLastPart;		/**< The last loaded motion part. */
		};

		//-----------------------------------------------------------------------------------------

		/**
		 * The constructor.
		 */
		Importer();

		/**
		 * The destructor.
		 */
		~Importer();

		//-------------------------------------------------------------------------------------------------

		/**
		 * Load an actor from a given file.
		 * A file does not have to be stored on disk, but can also be in memory or in an archive or 
		 * on some network stream. Anything is possible.
		 * @param f The file to load the actor from (after load, the file will be closed).
		 * @param actorName The name/description of the actor (The name of the loaded actor will be set to this name).
		 * @param userPerPixelLighting If set to true, the tangent and binormals inside the meshes will be allocated and calculated.
         *                             However, if you do not use per pixel lighting, set this to false. This will save memory increase performance.
		 * @result Returns a pointer to the loaded actor, or NULL when something went wrong and the actor could not be loaded.
		 */
		Actor* LoadActor(MCore::File* f, const MCore::String& actorName, bool usePerPixelLighting=false);

		/**
		 * Loads an actor from a file on disk.
		 * @param filename The name of the file on disk.
		 * @param actorName The name/description of the actor (The name of the loaded actor will be set to this name).
		 * @param userPerPixelLighting If set to true, the tangent and binormals inside the meshes will be allocated and calculated.
		 *                             However, if you do not use per pixel lighting, set this to false. This will save memory increase performance.
		 * @result Returns a pointer to the loaded actor, or NULL when something went wrong and the actor could not be loaded.
		 */
		Actor* LoadActor(const MCore::String& filename, const MCore::String& actorName, bool usePerPixelLighting=false);

		/**
		 * Loads an actor from memory.
		 * @param memoryStart The start address of the file in memory.
		 * @param lengthInBytes The length of the file, in bytes.
		 * @param actorName The name/description of the actor (The name of the loaded actor will be set to this name).
		 * @param userPerPixelLighting If set to true, the tangent and binormals inside the meshes will be allocated and calculated.
		 *                             However, if you do not use per pixel lighting, set this to false. This will save memory increase performance.
		 * @result Returns a pointer to the loaded actor, or NULL when something went wrong and the actor could not be loaded.
		 */
		Actor* LoadActor(unsigned char *memoryStart, int lengthInBytes, const MCore::String& actorName, bool usePerPixelLighting=false);

		//-------------------------------------------------------------------------------------------------

		/**
		 * Load a motion from a given file.
		 * A file does not have to be stored on disk, but can also be in memory or in an archive or 
		 * on some network stream. Anything is possible.
		 * @param f The file to load the motion from (after load, the file will be closed).
		 * @param motionName The name/description of the motion (The name of the loaded motion will be set to this name).
		 * @result Returns a pointer to the loaded motion, or NULL when something went wrong and the motion could not be loaded.
		 */
		Motion* LoadMotion(MCore::File* f, const MCore::String& motionName);

		/**
		 * Loads a motion from a file on disk.
		 * @param filename The name of the file on disk.
		 * @param motionName The name/description of the motion (The name of the loaded motion will be set to this name).
		 * @result Returns a pointer to the loaded motion, or NULL when something went wrong and the motion could not be loaded.
		 */
		Motion* LoadMotion(const MCore::String& filename, const MCore::String& motionName);
		
		/**
		 * Loads a motion from memory.
		 * @param memoryStart The start address of the file in memory.
		 * @param lengthInBytes The length of the file, in bytes.
		 * @param motionName The name/description of the motion (The name of the loaded motion will be set to this name).
		 * @result Returns a pointer to the loaded motion, or NULL when something went wrong and the motion could not be loaded.
		 */
		Motion* LoadMotion(unsigned char *memoryStart, int lengthInBytes, const MCore::String& motionName);

		//-------------------------------------------------------------------------------------------------

		/**
		 * Load a facial motion from a given file.
		 * A file does not have to be stored on disk, but can also be in memory or in an archive or 
		 * on some network stream. Anything is possible.
		 * @param f The file to load the facial motion from (after load, the file will be closed).
		 * @param motionName The name/description of the facial motion (The name of the loaded facial motion will be set to this name).
		 * @result Returns a pointer to the loaded facial motion, or NULL when something went wrong and the facial motion could not be loaded.
		 */
		FacialMotion* LoadFacialMotion(MCore::File* f, const MCore::String& motionName);

		/**
		 * Loads a facial motion from a file on disk.
		 * @param filename The name of the file on disk.
		 * @param motionName The name/description of the facial motion (The name of the loaded facial motion will be set to this name).
		 * @result Returns a pointer to the loaded facial motion, or NULL when something went wrong and the facial motion could not be loaded.
		 */
		FacialMotion* LoadFacialMotion(const MCore::String& filename, const MCore::String& motionName);

		/**
		 * Loads a facial motion from memory.
		 * @param memoryStart The start address of the file in memory.
		 * @param lengthInBytes The length of the file, in bytes.
		 * @param motionName The name/description of the facial motion (The name of the loaded facial motion will be set to this name).
		 * @result Returns a pointer to the loaded facial motion, or NULL when something went wrong and the facial motion could not be loaded.
		 */
		FacialMotion* LoadFacialMotion(unsigned char *memoryStart, int lengthInBytes, const MCore::String& motionName);

		//-------------------------------------------------------------------------------------------------

		/**
		 * Retrieve the actor file's low and high version from an lma file.
		 * The low and high version will be -1 if there has been an error.
		 * @param lowVersion Reference to write the low version in.
		 * @param highVersion Reference to write the high version in.
		 * @param fileName The name of the file on disk.
		 */
		void GetLMAFileVersion(int& lowVersion, int& highVersion, const MCore::String& fileName);

		/**
		 * Check if the given file contains an actor.
		 * @param fileName The filename of the file to check.
		 * @return True if the file contains an actor, false if it contains a motion or if it is an invalid file.
		 */
		bool IsActor(const MCore::String& fileName);

		/**
		 * Check if the given file contains a motion.
		 * @param fileName The filename of the file to check.
		 * @return True if the file contains a motion, false if it contains an actor or if it is an invalid file.
		 */
		bool IsMotion(const MCore::String& fileName);

		/**
		 * Return the number of the given chunks in the file. (file need to be already open)
		 * @param chunkType The id/type of the chunk to check. (e.g. LMA_CHUNK_NODE)
		 * @param file A pointer to the file to check. The file must be already opened and won't be closed by the function!
		 * @return Number of given chunks in the file. -1 will be returned if an error occured.
		 */
		int GetNumChunks(const int chunkType, MCore::File* file) const;

		/**
		 * Return the size of the given chunk. (file need to be already open)
		 * @param chunkType The id/type of the chunk to check. (e.g. LMA_CHUNK_NODE)
		 * @param index The index of the chunk. The index ranges from 0 (first chunk) to GetNumChunks()-1 (last chunk).
		 * @param file A pointer to the file to check. The file must be already opened and won't be closed by the function!
		 * @return The size of the given chunk. -1 will be returned if an error occured.
		 */
		int GetChunkSize(const int chunkType, const int index, MCore::File* file) const;

		/**
		 * Return the the seek position of the given chunk in bytes. The returned position does not include the header,
		 * so the the position of the header = GetSeekPositionByChunk()-sizeof(LMA_Chunk). (file need to be already open)
		 * @param chunkType The id/type of the chunk to check. (e.g. LMA_CHUNK_NODE)
		 * @param index The index of the chunk. The index ranges from 0 (first chunk) to GetNumChunks()-1 (last chunk).
		 * @param file A pointer to the file to check. The file must be already opened and won't be closed by the function!
		 * @return The the seek position of the given chunk in bytes. -1 will be returned if an error occured.
		 */
		int GetSeekPositionByChunk(const int chunkType, const int index, MCore::File* file) const;

		/**
		 * Remove the given chunk from the file. (file should not be opened by anyone else)
		 * @param chunkType The id/type of the chunk to check. (e.g. LMA_CHUNK_NODE)
		 * @param index The index of the chunk. The index ranges from 0 (first chunk) to GetNumChunks()-1 (last chunk).
		 * @param fileName The name of the file on disk.
		 * @return True if everything went okay, false if not.
		 */
		bool RemoveChunk(const int chunkType, const int index, const MCore::String& fileName) const;

		/**
		 * Remove all chunks of the given type from file. (file should not be opened by anyone else)
		 * @param chunkType The id/type of the chunk to check. (e.g. LMA_CHUNK_NODE)
		 * @param fileName The name of the file on disk.
		 * @return True if everything went okay, false if not.
		 */
		bool RemoveAllChunks(const int chunkType, const MCore::String& fileName) const;

		/**
		 * Replace the given chunk by another one. (file should not be opened by anyone else)
		 * <code>
		 * LMA_Node node;
		 * node.mLocalPos.mX =  1.0;
		 * node.mLocalPos.mY = -1.0;
		 * node.mLocalPos.mZ =  5.0;
		 * IMPORTER.ReplaceChunk(LMA_CHUNK_NODE, 1, (char*)&node, "Skeleton.lma");
		 * </code>
		 * In this example we create a new node and fill it with data. The ReplaceChunk() function will overwrite the second
		 * node, since the index starts with 0. You have to type-cast your node address to a (char*).
		 * @param chunkType The id/type of the chunk to check. (e.g. LMA_CHUNK_NODE)
		 * @param index The index of the chunk. The index ranges from 0 (first chunk) to GetNumChunks()-1 (last chunk).
		 * @param chunkData A pointer to the chunk to replace. You have to type-cast the chunk pointer to a char pointer.
		 * @param fileName The name of the file on disk.
		 * @return True if everything went okay, false if not.
		 */
		bool ReplaceChunk(const int chunkType, const int index, char* chunkData, const MCore::String& fileName) const;

		/**
		 * Insert chunk to the given file. (file should not be opened by anyone else)
		 * @param filePosition The seek position in the file in bytes. (dont't forget that this func also writes a chunk header, so filePosition must be after an chunk and not a chunk header.
		 * @param chunkHeader The header of the chunk. The old chunk header is not allowed!
		 * @param chunkSize The size of the chunk to insert in bytes.
		 * @param chunkData A pointer to the chunk+header to insert. You have to type-cast the chunk pointer to a char pointer.
		 * @param fileName The name of the file on disk.
		 * @return True if everything went okay, false if not.
		 */
		bool InsertChunk(const int filePosition, const LMA_Chunk& chunkHeader, const int chunkSize, char* chunkData, const MCore::String& fileName) const;

		//-------------------------------------------------------------------------------------------------

		/**
		 * Register a new chunk processor.
		 * It can either be a new version of a chunk processor to extend the current file format or
		 * a complete new chunk processor. It will be added to the processor database and will then be executable.
		 * @param processorToRegister The processor to register to the importer.
		 */
		void RegisterChunkProcessor(ChunkProcessor* processorToRegister);

		/**
		 * Add shared data.
		 * Shared data is stored in the importer and provide the chunk processors to use and modify it.
		 * Shared data objects need to be inherited from the Importer::SharedData base class and must get
		 * their unique ID. Use shared data if you have several chunk processors which need access to the same objects/data.
		 * This function will add a new shared data object to the importer.
		 * @param sharedData A pointer to your shared data object.
		 */
		void AddSharedData(SharedData* sharedData);

		/**
		 * Find shared data objects which have the same type as the ID passed as parameter.
		 * @param type The shared data ID to search for.
		 * @return A pointer to the shared data object, or NULL when no shared data of this type has been found.
		 */
		Importer::SharedData* FindSharedData(const int type);

		/**
		 * Enable or disable logging.
		 * @param Set to true if you want to enable logging, or false when you want to disable it.
		 */
		void SetLogging(const bool enabled);

		/**
		 * Check if logging is enabled or not.
		 * @return Returns true when the importer will perform logging, or false when it will be totally silent.
		 */
		bool GetLogging() const;

		/**
		 * Set if the importer should log the processor details or not.
		 * @param detailLoggingActive Set to true when you want to enable this feature, otherwise set it to false.
		 */
		void SetLogDetails(const bool detailLoggingActive);

		/**
		 * Check if detail-logging is enabled or not.
		 * @return Returns true when detail-logging is enabled, otherwise false is returned.
		 */
		bool GetLogDetails() const;


	private:
		MCore::Array<ChunkProcessor*>		mChunkProcessors;	/**< The array of registered chunk processors. */
		MCore::Array<SharedData*>			mSharedData;		/**< The shared data. */
		LMA_Header							mLMAHeader;			/**< The LMA header. */
		bool								mLoggingActive;		/**< Contains if the importer should perform logging or not or not. */
		bool								mLogDetails;		/**< Contains if the importer should perform detail-logging or not. */
		bool								mStandardProcessorsInitialized;	/**< Contains true if the standard processors have been registered already, otherwise it contains false. */

		/**
		 * Update parent and child pointers.
		 */
		void UpdateHierarchy(Actor *actor);

		/**
		 * Reset all shared data objects.
		 * Resetting these objects will clear/empty their internal data.
		 */
		void ResetSharedData();

		/**
		 * Find the chunk processor which has a given ID and version number.
		 * @param chunkID The ID of the chunk processor.
		 * @param version The version number of the chunk processor.
		 * @return A pointer to the chunk processor if it has been found, or NULL when no processor could be found
		 *		   which is able to process the given chunk or the given version of the chunk.
		 */
		ChunkProcessor* FindChunk(const int chunkID, const int version);

		/**
		 * Register the standard chunk processors to be able to import all standard LMA chunks.
		 * Shared data which is essential for the standard chunks will also be automatically created and added.
		 */
		void RegisterStandardChunks();

		/**
		 * Read and process the next chunk.
		 * @param file The file to read from.
		 * @param actor The actor to store the read data in (can be NULL when importing a motion).
		 * @param motion The motion to store the read data in (can be NULL when importing an actor).
		 * @param usePerPixelLighting Must be true when the tangent and binormal information must be allocated and generated.
		 * @result Returns false when the chunk could not be processed, due to an end of file or other file error.
		 */
		bool ProcessChunk(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting);
};

// include the LMA/LMF file format structs
#include "FileFormatStructs.inl"

} // namespace EMotionFX

#endif