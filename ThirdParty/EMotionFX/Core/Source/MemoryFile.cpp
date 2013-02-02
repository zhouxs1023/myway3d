/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include "MemoryFile.h"


namespace MCore
{


// try to open the memory location
bool MemoryFile::Open(unsigned char* memoryStart, int length)
{
	assert(memoryStart);
	assert(length > 0);

	if (memoryStart == NULL || length <= 0)
		return false;

	mMemoryStart= memoryStart;
	mCurrentPos	= memoryStart;
	mLength		= length;

	return true;
}


// close the file
void MemoryFile::Close()
{
	mMemoryStart= NULL;
	mCurrentPos = NULL;
	mLength		= 0;
}


// flush the file
void MemoryFile::Flush()
{
}


bool MemoryFile::IsOpen() const
{
	return (mMemoryStart != NULL);
}



// return true when we have reached the end of the file
bool MemoryFile::IsEOF() const
{

	return ((mCurrentPos-mMemoryStart) >= mLength);
}


// returns the next byte in the file
char MemoryFile::GetNextByte()
{
	char value = *mCurrentPos;
	mCurrentPos++;
	return value;
}


// returns the position (offset) in the file in bytes
int MemoryFile::GetPos() const
{
	return (int)(mCurrentPos - mMemoryStart);
}


// write a given byte to the file
bool MemoryFile::WriteByte(char value)
{
	*mCurrentPos = value;
	mCurrentPos++;

	return true;
}


// seek a given number of bytes ahead from it's current position
bool MemoryFile::Forward(int numBytes)
{
	unsigned char *newPos = mCurrentPos + numBytes;
	if (newPos > (mMemoryStart + mLength))
		return false;
	
	mCurrentPos = newPos;
	return true;
}


// seek to an absolute position in the file (offset in bytes)
bool MemoryFile::Seek(int offset)
{
	if (offset > mLength)
		return false;
	
	mCurrentPos = mMemoryStart + offset;
	return true;
}


// write data to the file
int MemoryFile::Write(void *data, int length)
{
	assert( (((unsigned char*)data + length) - mMemoryStart) < mLength );	// make sure we don't write past the end of the file
	memcpy(mCurrentPos, data, length);
	Forward(length);
	return length;
}


// read data from the file
int MemoryFile::Read(void *data, int length)
{
	memcpy(data, mCurrentPos, length);
	Forward(length);
	return length;
}


// returns the filesize in bytes
int	MemoryFile::GetFileSize() const
{
	return mLength;
}


} // namespace MCore