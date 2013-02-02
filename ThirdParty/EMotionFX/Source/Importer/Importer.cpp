/*
 * EMotion FX 2 - Character Animation System
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "Importer.h"

#include "../Node.h"
#include "../Motion.h"
#include "../SkeletalMotion.h"
#include "../FacialMotion.h"
#include "../MotionPart.h"
#include "../Actor.h"
#include "../Mesh.h"
#include "../MeshDeformerStack.h"
#include "../SoftSkinDeformer.h"
#include "../SimpleMesh.h"
#include "../SubMesh.h"
#include "../Material.h"
#include "../SkinningInfoVertexAttributeLayer.h"
#include "../SoftSkinManager.h"
#include "../LinearInterpolators.h"

// use the Core namespace
using namespace MCore;


// 
//#define EMOTIONFXEVALUATION
#undef EMOTIONFXEVALUATION

#ifdef EMOTIONFXEVALUATION
	// include evaluation cheking
	#include "../../Evaluation/EvalLicenseeInfo.h"
#endif


namespace EMotionFX
{

// constructor
Importer::Importer()
{
	// register all standard chunks
	RegisterStandardChunks();

	// init some default values
	mLoggingActive	= true;
	mLogDetails		= false;
}


// destructor
Importer::~Importer()
{
	// remove of all chunk processors
	while (mChunkProcessors.GetLength())
	{
		delete mChunkProcessors.GetLast();
		mChunkProcessors.RemoveLast();
	}

	// remove all shared data
	while (mSharedData.GetLength())
	{
		delete mSharedData.GetLast();
		mSharedData.RemoveLast();
	}

	// we're  not initialized anymore
	mStandardProcessorsInitialized = false;
}

//-------------------------------------------------------------------------------------------------

// try to load an actor from disk
Actor* Importer::LoadActor(const String& filename, const String& actorName, bool usePerPixelLighting)
{
	DECLARE_FUNCTION(LoadActor)

	// register all standard chunks if this has not ye been done
	if (!mStandardProcessorsInitialized)
		RegisterStandardChunks();

	// release the shared data
	ResetSharedData();

	if (GetLogging())
		LOG("Trying to load actor '%s' from file '%s'...", actorName.GetReadPtr(), filename.GetReadPtr());

	// try to open the file from disk
	DiskFile f;
	if (!f.Open(filename.GetReadPtr(), DiskFile::READ))
	{
		if (GetLogging())
			LOG("Failed open the file '%s' for actor '%s', actor not loaded!", filename.GetReadPtr(), actorName.GetReadPtr());
		return NULL;
	}

	// old way: load actor step by step from disk
	//Actor *result = LoadActor(&f, actorName, usePerPixelLighting);

	// retrieve the filesize
	const int fileSize = f.GetFileSize();

	// create a temporary buffer for the file
	unsigned char* fileBuffer = (unsigned char*)MEMMGR.Allocate(fileSize, MEMCATEGORY_IMPORTER, 16);

	// read in the complete file
	f.Read(fileBuffer, fileSize);

	// close the file again
	f.Close();

	// create the actor reading from memory
	Actor *result = LoadActor( fileBuffer, fileSize, actorName, usePerPixelLighting);

	// delete the filebuffer again
	MEMMGR.Delete(fileBuffer);

	// check if it worked :)
	if (result == NULL)
	{
		if (GetLogging())
			LOG("Failed to load actor '%s' from file '%s'", actorName.GetReadPtr(), filename.GetReadPtr());
	}

	// return the result
	return result;
}


// load an actor from memory
Actor* Importer::LoadActor(unsigned char* memoryStart, int lengthInBytes, const String& actorName, bool usePerPixelLighting)
{
	DECLARE_FUNCTION(LoadActor)

	// register all standard chunks if this has not ye been done
	if (!mStandardProcessorsInitialized)
		RegisterStandardChunks();

	// release the shared data
	ResetSharedData();

	if (GetLogging())
		LOG("Trying to load actor '%s' from memory location 0x%x...", actorName.GetReadPtr(), memoryStart);

	// create the memory file
	MemoryFile memFile;
	memFile.Open(memoryStart, lengthInBytes);

	// try to load the file
	Actor* result = LoadActor(&memFile, actorName, usePerPixelLighting);
	if (result == NULL)
	{
		if (GetLogging())
			LOG("Failed to load actor '%s' from memory location 0x%x", actorName.GetReadPtr(), memoryStart);
	}

	// close the memory file again
	memFile.Close();

	// return the result
	return result;
}


// try to load a LMA file
Actor* Importer::LoadActor(File* f, const String& actorName, bool usePerPixelLighting)
{
	DECLARE_FUNCTION(LoadActor)

	#ifdef EMOTIONFXEVALUATION
		LicenseValidator gLicenseChecker;

		// check if the license is valid and update it
		if (!gLicenseChecker.HasValidLicense())
			return NULL;
	#endif

	// register all standard chunks if this has not ye been done
	if (!mStandardProcessorsInitialized)
		RegisterStandardChunks();

	assert(f);
	assert(f->IsOpen());

	if (GetLogging())
		LOG("- Trying to load actor '%s'...", actorName.GetReadPtr());

	// release some buffers
	ResetSharedData();

	// read the header
	LMA_Header header;
	f->Read(&header, sizeof(LMA_Header));

	mLMAHeader = header;

	// check FOURCC
	if (header.mFourcc[0]!='L' || header.mFourcc[1]!='M' || header.mFourcc[2]!='A' || header.mFourcc[3]!=' ')
	{
		if (GetLogging())
			LOG("Importer::LoadActor() - Not a valid LMA file!");
		f->Close();
		return NULL;
	}

	if (header.mHiVersion != LMA_HIGHVERSION)
	{
		if (GetLogging())
			LOG("Importer::LoadActor() - Cannot read this version! Can only read LMA files with high version %d.x, and this is version %d.%d!", LMA_HIGHVERSION, header.mHiVersion, header.mLoVersion);
		f->Close();
		return NULL;
	}

	// no actor in this file
	if (!header.mActor)
	{
		if (GetLogging())
			LOG("Importer::LoadActor() - This file does NOT contain an actor, but a motion!");
		f->Close();
		return NULL;
	}

	// display some header info
	if (GetLogging() && GetLogDetails())
		LOG("  + Version = %d.%d", header.mHiVersion, header.mLoVersion);

	// create our actor
	Actor* actor = new Actor(actorName);

	// read the chunks
	while (ProcessChunk(f, actor, NULL, usePerPixelLighting))
	{
	}

	// update the hierarchy pointers
	UpdateHierarchy(actor);

	// update the character so that everything is up-to-date
	actor->Update(0.0, true);
	actor->UpdateBounds(0, Actor::BOUNDS_MESH_BASED, true);

	// reset all shared data again
	ResetSharedData();

	// close the file and return a pointer to the actor we loaded
	f->Close();

	// return the created actor
	return actor;
}

//-------------------------------------------------------------------------------------------------

// try to load a motion from disk
Motion* Importer::LoadMotion(const String& filename, const String& motionName)
{
	DECLARE_FUNCTION(LoadMotion)

	// register all standard chunks if this has not ye been done
	if (!mStandardProcessorsInitialized)
		RegisterStandardChunks();

	// release the shared data
	ResetSharedData();

	if (GetLogging())
		LOG("Trying to load motion '%s' from file '%s'...", motionName.GetReadPtr(), filename.GetReadPtr());

	// try to open the file from disk
	DiskFile f;
	if (!f.Open(filename.GetReadPtr(), DiskFile::READ))
	{
		if (GetLogging())
			LOG("Failed open the file '%s' for motion '%s', motion not loaded!", filename.GetReadPtr(), motionName.GetReadPtr());
		return NULL;
	}

	// try to load the motion
	//Motion *result = LoadMotion(&f, motionName);

	// retrieve the filesize
	const int fileSize = f.GetFileSize();

	// create a temporary buffer for the file
	unsigned char* fileBuffer = (unsigned char*)MEMMGR.Allocate(fileSize, MEMCATEGORY_IMPORTER, 16);

	// read in the complete file
	f.Read(fileBuffer, fileSize);

	// close the file again
	f.Close();

	// create the motion reading from memory
	Motion* result = LoadMotion(fileBuffer, fileSize, motionName);

	// delete the filebuffer again
	MEMMGR.Delete(fileBuffer);

	// check if it worked :)
	if (result == NULL)
	{
		if (GetLogging())
			LOG("Failed to load motion '%s' from file '%s'", motionName.GetReadPtr(), filename.GetReadPtr());
	}

	// return the result
	return result;
}


Motion* Importer::LoadMotion(unsigned char* memoryStart, int lengthInBytes, const String& motionName)
{
	DECLARE_FUNCTION(LoadMotion)

	// register all standard chunks if this has not ye been done
	if (!mStandardProcessorsInitialized)
		RegisterStandardChunks();

	// release some buffers
	ResetSharedData();

	if (GetLogging()) 
		LOG("Trying to load motion '%s' from memory location 0x%x...", motionName.GetReadPtr(), memoryStart);

	// create the memory file
	MemoryFile memFile;
	memFile.Open(memoryStart, lengthInBytes);

	// try to load it
	Motion* result = LoadMotion(&memFile, motionName);

	// check if it worked
	if (result == NULL)
	{
		if (GetLogging())
			LOG("Failed to load motion '%s' from memory location 0x%x", motionName.GetReadPtr(), memoryStart);
	}

	// return the result
	return result;
}


// try to load a motion from a LMA file
Motion* Importer::LoadMotion(File *f, const String& motionName)
{
	DECLARE_FUNCTION(LoadMotion)

#ifdef EMOTIONFXEVALUATION
	LicenseValidator gLicenseChecker;

	// check if the license is valid and update it
	if (!gLicenseChecker.HasValidLicense())
		return NULL;
#endif

	// register standard chunk processors if it hasn't been done yet
	if (!mStandardProcessorsInitialized)
		RegisterStandardChunks();

	assert(f);
	assert(f->IsOpen());

	if (GetLogging())
		LOG("- Trying to load LMA motion file '%s'...", motionName.GetReadPtr());

	// release some buffers
	ResetSharedData();

	// read the header
	LMA_Header header;
	f->Read(&header, sizeof(LMA_Header));

	mLMAHeader = header;

	// check FOURCC
	if (header.mFourcc[0]!='L' || header.mFourcc[1]!='M' || header.mFourcc[2]!='A' || header.mFourcc[3]!=' ')
	{
		if (GetLogging())
			LOG("Importer::LoadMotion() - Not a valid LMA file!");
		f->Close();
		return NULL;
	}

	// no actor in this file
	if (header.mActor)
	{
		if (GetLogging())
			LOG("Importer::Load() - This file does NOT contain a motion, but only an actor!");
		f->Close();
		return NULL;
	}

	// version check
	if (header.mHiVersion != LMA_HIGHVERSION)
	{
		if (GetLogging())
			LOG("Importer::LoadMotion() - Cannot read this version! Can only read LMA files of version %d.x, and this is version %d.%d!", LMA_HIGHVERSION, header.mHiVersion, header.mLoVersion);
		f->Close();
		return NULL;
	}

	// display some header info
	if (GetLogging() && GetLogDetails())
		LOG("  + Version         = %d.%d", header.mHiVersion, header.mLoVersion);

	// create our motion
	SkeletalMotion* motion = new SkeletalMotion(motionName);

	// read the chunks
	while (ProcessChunk(f, NULL, motion, false))
	{
	}

	// update the max time of the motion
	motion->UpdateMaxTime();

	// get rid of all shared data
	ResetSharedData();

	// close the file and return a pointer to the actor we loaded
	f->Close();
	return motion;
}

//-------------------------------------------------------------------------------------------------

// try to load a facial motion from disk
FacialMotion* Importer::LoadFacialMotion(const String& filename, const String& motionName)
{
	DECLARE_FUNCTION(LoadFacialMotion)

	// register all standard chunks if this has not ye been done
	if (!mStandardProcessorsInitialized)
		RegisterStandardChunks();

	// release the shared data
	ResetSharedData();

	if (GetLogging())
		LOG("Trying to load facial motion '%s' from file '%s'...", motionName.GetReadPtr(), filename.GetReadPtr());

	// try to open the file from disk
	DiskFile f;
	if (!f.Open(filename.GetReadPtr(), DiskFile::READ))
	{
		if (GetLogging())
			LOG("Failed open the file '%s' for facial motion '%s', facial motion not loaded!", filename.GetReadPtr(), motionName.GetReadPtr());
		return NULL;
	}

	// try to load the facial motion
	//FacialMotion* result = Importer::Instance().LoadFacialMotion(&f, motionName);


	// retrieve the filesize
	const int fileSize = f.GetFileSize();

	// create a temporary buffer for the file
	unsigned char* fileBuffer = (unsigned char*)MEMMGR.Allocate(fileSize, MEMCATEGORY_IMPORTER, 16);

	// read in the complete file
	f.Read(fileBuffer, fileSize);

	// create the facial motion reading from memory
	FacialMotion* result = Importer::GetInstance().LoadFacialMotion(fileBuffer, fileSize, motionName);

	// delete the filebuffer again
	MEMMGR.Delete(fileBuffer);


	// check if it worked :)
	if (result == NULL)
	{
		if (GetLogging())
			LOG("Failed to load facial motion '%s' from file '%s'", motionName.GetReadPtr(), filename.GetReadPtr());
	}

	// return the result
	return result;
}


FacialMotion* Importer::LoadFacialMotion(unsigned char *memoryStart, int lengthInBytes, const String& motionName)
{
	DECLARE_FUNCTION(LoadFacialMotion)

	// register all standard chunks if this has not ye been done
	if (!mStandardProcessorsInitialized)
		RegisterStandardChunks();

	// release some buffers
	ResetSharedData();

	if (GetLogging()) 
		LOG("Trying to load facial motion '%s' from memory location 0x%x...", motionName.GetReadPtr(), memoryStart);

	// create the memory file
	MemoryFile memFile;
	memFile.Open(memoryStart, lengthInBytes);

	// try to load it
	FacialMotion* result = Importer::GetInstance().LoadFacialMotion(&memFile, motionName);

	// check if it worked
	if (result == NULL)
	{
		if (GetLogging())
			LOG("Failed to load facial motion '%s' from memory location 0x%x", motionName.GetReadPtr(), memoryStart);
	}

	// return the result
	return result;
}


// try to load a facial motion from a LMF file
FacialMotion* Importer::LoadFacialMotion(File *f, const String& motionName)
{
	DECLARE_FUNCTION(LoadFacialMotion)

#ifdef EMOTIONFXEVALUATION
	LicenseValidator gLicenseChecker;

	// check if the license is valid and update it
	if (!gLicenseChecker.HasValidLicense())
		return NULL;
#endif

	// register standard chunk processors if it hasn't been done yet
	if (!mStandardProcessorsInitialized)
		RegisterStandardChunks();

	assert(f);
	assert(f->IsOpen());

	if (GetLogging())
		LOG("- Trying to load LMF facial motion file '%s'...", motionName.GetReadPtr());

	// release some buffers
	ResetSharedData();

	// read the header
	LMF_Header header;
	f->Read(&header, sizeof(LMF_Header));

	// fill in lma header which will be used by the ProcessChunk routine to detect the correct processors
	mLMAHeader.mHiVersion = header.mHiVersion;
	mLMAHeader.mLoVersion = header.mLoVersion;

	// check FOURCC
	if (header.mFourcc[0]!='L' || header.mFourcc[1]!='M' || header.mFourcc[2]!='F' || header.mFourcc[3]!=' ')
	{
		if (GetLogging())
			LOG("Importer::LoadFacialMotion() - Not a valid LMF file!");
		f->Close();
		return NULL;
	}

	// version check
	if (header.mHiVersion != LMA_HIGHVERSION)
	{
		if (GetLogging())
			LOG("Importer::LoadFacialMotion() - Cannot read this version! Can only read LMF files of version %d.x, and this is version %d.%d!", LMA_HIGHVERSION, header.mHiVersion, header.mLoVersion);
		f->Close();
		return NULL;
	}

	// display some header info
	if (GetLogging() && GetLogDetails())
		LOG("  + Version         = %d.%d", header.mHiVersion, header.mLoVersion);

	// create our motion
	FacialMotion* facialMotion = new FacialMotion(motionName);

	// read the chunks
	while (ProcessChunk(f, NULL, facialMotion, false))
	{
	}

	// update the max time of the motion
	facialMotion->UpdateMaxTime();

	// get rid of all shared data
	ResetSharedData();

	// close the file and return a pointer to the actor we loaded
	f->Close();
	return facialMotion;
}

//-------------------------------------------------------------------------------------------------

// retrieve the low and high version of an lma file
void Importer::GetLMAFileVersion(int& lowVersion, int& highVersion, const MCore::String& fileName)
{
	// reset the version number
	lowVersion = highVersion = -1;

	// check for a valid name
	if (fileName.IsEmpty())
		return;

	// get the chunk header
	MCore::DiskFile file;
	file.Open(fileName.AsChar(), MCore::DiskFile::READ, true);
	LMA_Header header;
	file.Read(&header, sizeof(LMA_Header));
	file.Close();

	highVersion = header.mHiVersion;
	lowVersion	= header.mLoVersion;
}


// return number of the given chunks
int Importer::GetNumChunks(const int chunkType, File* file) const
{
	int chunkCounter = 0;

	// check if the file is valid, else return error
	if (!file)
		return -1;

	assert(file->IsOpen());

	// get the offset position so that we can reset the file afterwards
	int offsetPos = file->GetPos();

	// construct the chunk iterator
	MCore::File* iteratorFile = file;
	ChunkIterator iter(chunkType, iteratorFile);

	// since chunks can have a dynamic size, try to get it
	int chunkSize = 0;

	iteratorFile=iter.Next(chunkSize);

	while(iteratorFile)
	{
		// skip the chunk
		iteratorFile->Forward(chunkSize);

		chunkCounter++;
		//LOG("Increasing chunk counter.");

		iteratorFile=iter.Next(chunkSize);
	}

	// reset the offset position, nothing has happened ;)
	file->Seek(offsetPos);

	return chunkCounter;
}


// return the size of the given chunk
int Importer::GetChunkSize(const int chunkType, const int index, MCore::File* file) const
{
	// check if the file is valid, else return error
	if (!file)
		return -1;

	assert(file->IsOpen());

	// since chunks can have a dynamic size, try to get it
	int chunkSize = -1;

	// check if the index is in range
	if (index >= 0 && index < GetNumChunks(chunkType, file))
	{
		// get the offset position so that we can reset the file afterwards
		int offsetPos = file->GetPos();

		// construct the chunk iterator
		MCore::File* iteratorFile = file;
		ChunkIterator iter(chunkType, iteratorFile);

		int chunkCounter = 0;
		iteratorFile=iter.Next(chunkSize);

		while(iteratorFile)
		{
			// that's our chunk
			if (index == chunkCounter)
			{
				// nothing more to do, leave loop
				break;
			}

			// skip the chunk
			iteratorFile->Forward(chunkSize);

			// new chunk of the given type found, increase counter
			chunkCounter++;

			iteratorFile=iter.Next(chunkSize);
		}

		// reset the offset position, nothing has happened ;)
		file->Seek(offsetPos);
	}

	return chunkSize;
}


// return the the seek position of the given chunk in bytes
int Importer::GetSeekPositionByChunk(const int chunkType, const int index, File* file) const
{
	int chunkSeekPosition = -1;

	// check if the file is valid, else return error
	if (!file)
		return -1;

	assert(file->IsOpen());

	// check if the index is in range
	if (index >= 0 && index < GetNumChunks(chunkType, file))
	{
		// get the offset position so that we can reset the file afterwards
		int offsetPos = file->GetPos();

		// construct the chunk iterator
		MCore::File* iteratorFile = file;
		ChunkIterator iter(chunkType, iteratorFile);

		// since chunks can have a dynamic size, try to get it
		int chunkSize = -1;

		int chunkCounter = 0;
		for (iteratorFile=iter.Next(chunkSize); !iter.IsDone() && iteratorFile!=NULL; iteratorFile=iter.Next(chunkSize))
		{
			// check if the file is still valid
			if (iteratorFile)
			{
				// that's our chunk
				if (index == chunkCounter)
				{
					// get the seek position of the given chunk
					chunkSeekPosition = iteratorFile->GetPos();

					// nothing more to do, leave loop
					break;
				}

				// skip the chunk
				iteratorFile->Forward(chunkSize);

				// new chunk of the given type found, increase counter
				chunkCounter++;
			}
		}

		// reset the offset position, nothing has happened ;)
		file->Seek(offsetPos);
	}

	return chunkSeekPosition;
}


// remove the given chunk from the file
bool Importer::RemoveChunk(const int chunkType, const int index, const String& fileName) const
{
	// check for a valid name
	if (fileName.IsEmpty())
		return false;

	// get the chunk header size (could be the new or the old version)
	int chunkHeaderSize = 0;
	MCore::DiskFile file;
	file.Open(fileName.AsChar(), MCore::DiskFile::READ, true);
	LMA_Header header;
	file.Read(&header, sizeof(LMA_Header));
	file.Close();

	if (header.mHiVersion == 1 && header.mLoVersion == 0)
		chunkHeaderSize = sizeof(LMA_ChunkOld);
	else
		chunkHeaderSize = sizeof(LMA_Chunk);

	// try to open the file from disk
	DiskFile inputFile;
	if (!inputFile.Open(fileName.GetReadPtr(), DiskFile::READ, true))
	{
		if (GetLogging())
			LOG("Importer::RemoveChunk: Failed open the file '%s'", fileName.GetReadPtr());
		return false;
	}

	// get the seek position of the given chunk
	int chunkSeekPosition	= GetSeekPositionByChunk(chunkType, index, &inputFile);
	int chunkSize			= GetChunkSize(chunkType, index, &inputFile);
	//LOG("Chunk Seek Position: %i", chunkSeekPosition);
	//LOG("Chunk Size: %i", chunkSize);

	// return if doesn't exist
	if (chunkSeekPosition == -1)
		return false;

	// get the file size
	int inputFileSize = inputFile.GetFileSize();
	if (inputFileSize <= 0)
		return false;
	//LOG("Input File Size: %i", inputFileSize);

	// create file buffer
	char* inputData = (char*)MEMMGR.Allocate(inputFileSize, MEMCATEGORY_IMPORTER, 16);

	// read in data
	inputFile.Read(inputData, inputFileSize);

	// close the input file
	inputFile.Close();


	// output buffer
	int outputFileSize = inputFileSize - chunkHeaderSize - chunkSize;
	//LOG("Header Size: %i", sizeof(LMA_Chunk));
	//LOG("Chunk Size: %i", chunkSize);
	char* outputData = (char*)MEMMGR.Allocate(outputFileSize, MEMCATEGORY_IMPORTER, 16);
	//LOG("Output File Size: %i", outputFileSize);

	int i;
	int outputIndex=0;
	// copy data before the chunk(before chunk header, since it isn't needed anymore too
	int beforeSize = chunkSeekPosition - chunkHeaderSize;
	//LOG("Before Size: %i", beforeSize);
	for (i=0; i<beforeSize; i++)
	{
		outputData[outputIndex] = inputData[i];
		outputIndex++;
	}
	// copy data after the chunk
	//LOG("Copy data after the chunk: i=%i to %i", chunkSeekPosition+chunkSize, inputFileSize);
	for (i=chunkSeekPosition+chunkSize; i<inputFileSize; i++)
	{
		outputData[outputIndex] = inputData[i];
		outputIndex++;
	}

	// try to open the file from disk
	DiskFile outputFile;
	if (!outputFile.Open(fileName.GetReadPtr(), DiskFile::WRITE, true))
	{
		if (GetLogging())
			LOG("Importer::RemoveChunk: Failed open the file '%s'. Check if file is write protected.", fileName.GetReadPtr());

		MEMMGR.Delete(inputData);
		MEMMGR.Delete(outputData);

		return false;
	}

	// write output data
	outputFile.Write(outputData, outputFileSize);

	// close the output file
	outputFile.Close();

	// get rid of our file buffers
	MEMMGR.Delete(inputData);
	MEMMGR.Delete(outputData);

	// everything went right
	return true;
}


// remove all chunks of the given type from file
bool Importer::RemoveAllChunks(const int chunkType, const String& fileName) const
{
	bool result = false;

	// try to open the file from disk
	DiskFile file;
	if (!file.Open(fileName.GetReadPtr(), DiskFile::READ, true))
	{
		if (GetLogging())
			LOG("Importer::RemoveAllChunks: Failed open the file '%s'", fileName.GetReadPtr());
		return result;
	}

	// get number of given chunks
	int numSpecificChunks = GetNumChunks(chunkType, &file);
	//LOG("Number Of Chunks To Delete: %i", numSpecificChunks);

	// close the file
	file.Close();

	// remove all chunks
	for (int i=0; i<numSpecificChunks; i++)
	{
		bool chunkResult= RemoveChunk(chunkType, 0, fileName);
		//LOG("Removing Chunk: %i", i);

		if (!chunkResult)
		{
			result = false;
			LOG("Error removing Chunk: %i", i);
		}
	}

	return result;
}


// replace the given chunk by another one
bool Importer::ReplaceChunk(const int chunkType, const int index, char* chunkData, const MCore::String& fileName) const
{
	// check for a valid name
	if (fileName.IsEmpty())
		return false;

	// try to open the file from disk
	DiskFile inputFile;
	if (!inputFile.Open(fileName.GetReadPtr(), DiskFile::READ, true))
	{
		if (GetLogging())
			LOG("Importer::RemoveChunk: Failed open the file '%s'", fileName.GetReadPtr());
		return false;
	}

	// get the seek position of the given chunk
	int chunkSeekPosition	= GetSeekPositionByChunk(chunkType, index, &inputFile);
	//LOG("Chunk Seek Position: %i", chunkSeekPosition);
	int chunkSize			= GetChunkSize(chunkType, index, &inputFile);
	//LOG("Chunk Size: %i", chunkSize);

	// return if it doesn't exist
	if (chunkSeekPosition == -1)
		return false;

	// get the file size
	int fileSize = inputFile.GetFileSize();
	if (fileSize <= 0)
		return false;
	//LOG("Input File Size: %i", inputFileSize);

	// create file buffer
	char* inputData = (char*)MEMMGR.Allocate(fileSize, MEMCATEGORY_IMPORTER, 16);

	// read in data
	inputFile.Read(inputData, fileSize);

	// close the input file
	inputFile.Close();

	// replace chunk data
	for (int i=0; i<chunkSize; i++)
		inputData[ chunkSeekPosition + i ] = chunkData[i];
	//LOG("Overwriting bytes from %i to %i", chunkSeekPosition, chunkSeekPosition + chunkSize);

	// try to open the file from disk
	DiskFile outputFile;
	if (!outputFile.Open(fileName.GetReadPtr(), DiskFile::WRITE, true))
	{
		if (GetLogging())
			LOG("Importer::RemoveChunk: Failed open the file '%s'", fileName.GetReadPtr());

		MEMMGR.Delete(inputData);
		return false;
	}

	// write output data
	outputFile.Write(inputData, fileSize);

	// close the output file
	outputFile.Close();

	// get rid of our file buffer
	MEMMGR.Delete(inputData);

	// everything went right
	return true;
}


// insert chunk to the given file at a given position
bool Importer::InsertChunk(const int filePosition, const LMA_Chunk& chunkHeader, const int chunkSize, char* chunkData, const MCore::String& fileName) const
{
	// check for a valid name
	if (fileName.IsEmpty() || chunkData == NULL)
		return false;

	// try to open the file from disk
	DiskFile inputFile;
	if (!inputFile.Open(fileName.GetReadPtr(), DiskFile::READ, true))
	{
		if (GetLogging())
			LOG("Importer::InsertChunk: Failed open the file '%s'", fileName.GetReadPtr());
		return false;
	}

	// get the file size
	int fileSize = inputFile.GetFileSize();
	if (fileSize <= 0)
		return false;
	//LOG("Input File Size: %i", inputFileSize);

	// create file buffer
	char* inputData = (char*)MEMMGR.Allocate(fileSize, MEMCATEGORY_IMPORTER, 16);

	// read in data
	inputFile.Read(inputData, fileSize);

	// close the input file
	inputFile.Close();

	// try to open the file from disk
	DiskFile outputFile;
	if (!outputFile.Open(fileName.GetReadPtr(), DiskFile::WRITE, true) || filePosition >= fileSize)
	{
		if (GetLogging())
			LOG("Importer::RemoveChunk: Failed open the file '%s'", fileName.GetReadPtr());

		MEMMGR.Delete(inputData);
		return false;
	}

	// write output data
	outputFile.Write(inputData, filePosition);
	outputFile.Write((void*)&chunkHeader, sizeof(LMA_Chunk));
	outputFile.Write(chunkData, chunkSize);
	outputFile.Write(inputData + filePosition, fileSize - filePosition);

	// close the output file
	outputFile.Close();

	// get rid of our file buffer
	MEMMGR.Delete(inputData);

	// everything went right
	return true;
}

//-------------------------------------------------------------------------------------------------

// register a new chunk processor
void Importer::RegisterChunkProcessor(ChunkProcessor* processorToRegister)
{
	assert( processorToRegister );
	mChunkProcessors.Add(processorToRegister);
}


// add shared data object to the importer
void Importer::AddSharedData(SharedData* sharedData)
{
	assert( sharedData );
	mSharedData.Add(sharedData);
}


// search for shared data
Importer::SharedData* Importer::FindSharedData(const int type)
{
	// for all shared data
	const int numSharedData = mSharedData.GetLength();
	for (int i=0; i<numSharedData; i++)
	{
		SharedData* sharedData = mSharedData[i];

		// check if it's the type we are searching for
 		if (sharedData->GetType() == type)
			return sharedData;
	}
 
	// nothing found
	return NULL;
}


void Importer::SetLogging(const bool enabled)
{
	mLoggingActive = enabled;
}


bool Importer::GetLogging() const
{
	return mLoggingActive;
}


void Importer::SetLogDetails(const bool detailLoggingActive)
{
	mLogDetails = detailLoggingActive;

	// set the processors logging flag
	const int numProcessors = mChunkProcessors.GetLength();
	for(int i=0; i<numProcessors; i++)
	{
		ChunkProcessor* processor = mChunkProcessors[i];
		processor->SetLogging( (mLoggingActive && detailLoggingActive) );	// only enable if logging is also enabled
	}
}


bool Importer::GetLogDetails() const
{
	return mLogDetails;
}


// update parent and childs
void Importer::UpdateHierarchy(Actor *actor)
{
	// find the shared data
	SharedHierarchyInfo* sharedHierarchyInfo = (SharedHierarchyInfo*)FindSharedData( SharedHierarchyInfo::TYPE_ID );

	// if no shared hierarchy info is found
	if (!sharedHierarchyInfo)
		return;

	// get the data
	MCore::Array<SharedHierarchyInfo::Link>&	links = sharedHierarchyInfo->mLinks;
	MCore::Array<Node*>& nodes = sharedHierarchyInfo->mNodes;

	// set parents
	int a, i;
	const int numLinks = links.GetLength();
	const int numNodes = nodes.GetLength();
	for (a=0; a<numLinks; a++)
	{
		bool found = false;
		for (i=0; (i<numNodes) && (!found); i++)
		{
			if (strcmp(nodes[i]->GetName(), links[a].mParent)==0)
			{
				links[a].mNode->SetParent( nodes[i] );
				found = true;
			}
		}

		if (!found)
			if (GetLogging())
				LOG("Failed to resolve parent node '%s'", links[a].mParent.GetReadPtr());
	}


	// set childs
	for (i=0; i<numNodes; i++)
	{
		if (nodes[i]->IsRootNode()) continue;

		for (a=0; a<numNodes; a++)
		{
			// if node i's parent is node a, add node i to the child list of node a
			if (nodes[i]->GetParent() == nodes[a])
				nodes[a]->AddChild(nodes[i]);
		}
	}


	// link parents to the actor
	for (i=0; i<numNodes; i++)
	{
		if (nodes[i]->IsRootNode())
		{
			nodes[i]->SetParent(actor);
			actor->AddChild(nodes[i]);
		}
	}
}


// reset shared objects so that the importer is ready for use again
void Importer::ResetSharedData()
{
	const int numSharedData = mSharedData.GetLength();

	for (int i=0; i<numSharedData; i++)
	{
		SharedData* data = mSharedData[i];

		if (!data)
			continue;

		data->Reset();
	}
}


// find chunk, return NULL if it hasn't been found
ChunkProcessor* Importer::FindChunk(const int chunkID, const int version)
{
	// for all chunk processors
	const int numProcessors = mChunkProcessors.GetLength();
	for (int i=0; i<numProcessors; i++)
	{
		ChunkProcessor* processor = mChunkProcessors[i];

		// if this chunk is the type we are searching for AND it can process our chunk version, return it
 		if (processor->GetChunkID() == chunkID && processor->GetVersion() == version)
			return processor;
	}
 
	// nothing found
	return NULL;
}


// register basic chunk processors
void Importer::RegisterStandardChunks() 
{
	// version 1
	RegisterChunkProcessor( new NodeChunkProcessor1(this)			);
	RegisterChunkProcessor( new MeshChunkProcessor1(this)			);
	RegisterChunkProcessor( new SkinningInfoChunkProcessor1(this)	);
	//RegisterChunkProcessor( new CollisionMeshChunkProcessor1(this));
	RegisterChunkProcessor( new MaterialChunkProcessor1(this)		);
	RegisterChunkProcessor( new MaterialLayerChunkProcessor1(this)	);
	RegisterChunkProcessor( new MotionPartChunkProcessor1(this)		);
	RegisterChunkProcessor( new AnimationChunkProcessor1(this)		);
	RegisterChunkProcessor( new LimitChunkProcessor1(this)			);
	RegisterChunkProcessor( new PhysicsInfoChunkProcessor1(this)	);
	RegisterChunkProcessor( new MeshExpressionPartChunkProcessor1(this)		);
	RegisterChunkProcessor( new ExpressionMotionPartChunkProcessor1(this)	);
	RegisterChunkProcessor( new PhonemeMotionDataChunkProcessor1(this)		);
	RegisterChunkProcessor( new FXMaterialChunkProcessor1(this)		);

	// version 2
	RegisterChunkProcessor( new MaterialChunkProcessor2(this)		);
	RegisterChunkProcessor( new MaterialLayerChunkProcessor2(this)	);
	RegisterChunkProcessor( new MeshChunkProcessor2(this)			);
	RegisterChunkProcessor( new MeshExpressionPartChunkProcessor2(this)		);

	// version 3
	RegisterChunkProcessor( new MaterialChunkProcessor3(this)		);
	RegisterChunkProcessor( new MeshChunkProcessor3(this)			);

	// version 10000
	RegisterChunkProcessor( new MeshChunkProcessor10000(this) );
	RegisterChunkProcessor( new MaterialChunkProcessor10000(this) );
	
	// create standard shared objects
	AddSharedData( new SharedHierarchyInfo()	);
	AddSharedData( new SharedMotionInfo()		);

	mStandardProcessorsInitialized	= true;
}


// find the chunk processor and read in the chunk
bool Importer::ProcessChunk(MCore::File* file, Actor* actor, Motion* motion, bool usePerPixelLighting)
{
	DECLARE_FUNCTION(ProcessChunk)

	// if we have reached the end of the file, exit
	if (file->IsEOF())
		return false;

	LMA_ChunkOld	chunkOld = { 0, 0 };		// initialization to prevent a warning
	LMA_Chunk		chunk    = { 0, 0, 0 };		// initialization to prevent a warning

	// try to read the chunk header
	if (mLMAHeader.mHiVersion == 1 && mLMAHeader.mLoVersion == 0)
	{
		if (!file->Read(&chunkOld, sizeof(LMA_ChunkOld)))
			return false; // failed reading chunk
	}
	else
	{
		if (!file->Read(&chunk, sizeof(LMA_Chunk)))
			return false; // failed reading chunk
	}

	// try to find the chunk processor which can process this chunk
	ChunkProcessor* processor = NULL;

	if (mLMAHeader.mHiVersion == 1 && mLMAHeader.mLoVersion == 0)
		processor = FindChunk(chunkOld.mChunkID, 1); // using version 1
	else
		processor = FindChunk(chunk.mChunkID, chunk.mVersion);

	// if we cannot find the chunk, skip the chunk
	if (processor == NULL)
	{
		if (mLMAHeader.mHiVersion == 1 && mLMAHeader.mLoVersion == 0)
		{		
			if (GetLogging())
				LOG("Importer::ProcessChunk() - Unknown chunk (ID=%d  Size=%d bytes), skipping...", chunkOld.mChunkID, chunkOld.mSizeInBytes);
			file->Forward(chunkOld.mSizeInBytes);
		}
		else
		{
			if (GetLogging())
				LOG("Importer::ProcessChunk() - Unknown chunk (ID=%d  Size=%d bytes), skipping...", chunk.mChunkID, chunk.mSizeInBytes);
			file->Forward(chunk.mSizeInBytes);
		}

		return true;
	}

	// try to process the chunk
	return processor->Process(file, actor, motion, usePerPixelLighting);
}


// check if the given file contains an actor
bool Importer::IsActor(const MCore::String& fileName)
{
	// check for a valid name
	if (fileName.IsEmpty())
		return false;

	// get the chunk header
	MCore::DiskFile file;
	if (file.Open(fileName.AsChar(), MCore::DiskFile::READ, true) == false)
		return false;

	// try to read the header
	LMA_Header header;
	if (file.Read(&header, sizeof(LMA_Header)) == 0)
	{
		file.Close();
		return false;
	}

	// close the file again
	file.Close();

	// check if it is really an LMA file
	if (header.mFourcc[0]!='L' || header.mFourcc[1]!='M' || header.mFourcc[2]!='A' || header.mFourcc[3]!=' ')
		return false;

	// check if we are dealing with an actor
	return header.mActor;
}


// check if the given file contains a motion
bool Importer::IsMotion(const MCore::String& fileName)
{
	// check for a valid name
	if (fileName.IsEmpty())
		return false;

	// get the chunk header
	MCore::DiskFile file;
	if (file.Open(fileName.AsChar(), MCore::DiskFile::READ, true) == false)
		return false;

	// try to read the header
	LMA_Header header;
	if (file.Read(&header, sizeof(LMA_Header)) == 0)
	{
		file.Close();
		return false;
	}

	// close the file again
	file.Close();

	// check if it is really an LMA file
	if (header.mFourcc[0]!='L' || header.mFourcc[1]!='M' || header.mFourcc[2]!='A' || header.mFourcc[3]!=' ')
		return false;

	// check if we are dealing with a motion
	return !header.mActor;
}


// constructor
ChunkIterator::ChunkIterator(const int chunkFilterType, MCore::File* file)
{
	// set the file to iterate through
	mFile = file;

	// set chunk type
	mChunkFilterType = chunkFilterType;

	// read the header
	mFile->Read(&mLMAHeader, sizeof(LMA_Header));
}


// destructor
ChunkIterator::~ChunkIterator()
{
}


// step forward
MCore::File* ChunkIterator::Next(int& chunkSize)
{
	// if not check if there still is a chunk of the iterator's type
	LMA_ChunkOld	chunkOld = { 0, 0 };		// initialization to prevent a warning
	LMA_Chunk		chunk    = { 0, 0, 0 };		// initialization to prevent a warning

	// we are done...
	if (IsDone())
		return NULL;

	// try to read the chunk header
	if (mLMAHeader.mHiVersion == 1 && mLMAHeader.mLoVersion == 0)
	{
		int result = mFile->Read(&chunkOld, sizeof(LMA_ChunkOld));

		if (result != sizeof(LMA_ChunkOld))
		{
			//LOG("Read failed. Return!");
			return NULL;
		}

		if (chunkOld.mChunkID == mChunkFilterType)
		{
			chunkSize = chunkOld.mSizeInBytes;
			//LOG("Chunk Found.");
			return mFile;
		}
		else
		{
			//LOG("Skipping: Size: %i, SeekPos: %i", chunkOld.mSizeInBytes, mFile->GetPos());
			mFile->Forward(chunkOld.mSizeInBytes);
		}
	}
	else
	{
		int result = mFile->Read(&chunk, sizeof(LMA_Chunk));

		if (result != sizeof(LMA_Chunk))
		{
			//LOG("Read failed. Return!");
			return NULL;
		}

		if (chunk.mChunkID == mChunkFilterType)
		{
			chunkSize = chunk.mSizeInBytes;
			//LOG("Chunk Found.");
			return mFile;
		}
		else
		{
			//LOG("Skipping: Size: %i, SeekPos: %i", chunk.mSizeInBytes, mFile->GetPos());
			mFile->Forward(chunk.mSizeInBytes);
		}
	}

	// go again
	if (!IsDone())
		return Next(chunkSize);

	// if all hasn't helped return NULL
	return NULL;
}


// end of iteration ?
bool ChunkIterator::IsDone()
{
	// if we have reached the end of the file, return false
	if (mFile->IsEOF())
		return true;

	// still chunks to process
	return false;
}


// iteration chunk type
int ChunkIterator::GetChunkFilterType() const
{
	return mChunkFilterType;
}


} // namespace EMotionFX