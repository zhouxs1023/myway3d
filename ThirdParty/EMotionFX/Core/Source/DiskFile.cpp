/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#include <stdio.h>
#include "DiskFile.h"
#include "String.h"


namespace MCore
{

// try to open the file
bool DiskFile::Open(const char *filename, EMode mode, bool binary)
{
	// if the file already is open, close it first
	if (mFile)
		Close();

	String fileMode;

	if (mode == READ)				fileMode = "r";		// open for reading, file must exist
	if (mode == WRITE)				fileMode = "w";		// open for writing, file will be overwritten if it already exists
	if (mode == READWRITE)			fileMode = "r+";	// open for reading and writing, file must exist
	if (mode == READWRITECREATE)	fileMode = "w+";	// open for reading and writing, file will be overwritten when already exists, or created when it doesn't
	if (mode == APPEND)				fileMode = "a";		// open for writing at the end of the file, file will be created when it doesn't exist
	if (mode == READWRITEAPPEND)	fileMode = "a+";	// open for reading and appending (writing), file will be created if it doesn't exist

	// construct the filemode string
	fileMode += binary ? "b" : "t";	// open in binary or textmode

	// try to open the file
	mFile = fopen(filename, fileMode.GetReadPtr());

	// set the file mode we used
	mFileMode = mode;

	// set the filename
	mFilename = filename;

	// check on success
	return (mFile != NULL);
}


// close the file
void DiskFile::Close()
{
	if (mFile)
	{
		fclose(mFile);
		mFile = NULL;
	}
}


// flush the file
void DiskFile::Flush()
{
	assert(mFile);
	fflush(mFile);
}


bool DiskFile::IsOpen() const
{
	return (mFile!=NULL);
}



// return true when we have reached the end of the file
bool DiskFile::IsEOF() const
{
	assert(mFile);
	return (feof(mFile) != 0);
}


// returns the next byte in the file
char DiskFile::GetNextByte()
{
	assert(mFile);
	assert((mFileMode == READ) || (mFileMode == READWRITE) || (mFileMode == READWRITEAPPEND) || (mFileMode == APPEND) || (mFileMode == READWRITECREATE));	// make sure we opened the file in read mode

	return fgetc(mFile);
}


// returns the position (offset) in the file in bytes
int DiskFile::GetPos() const
{
	assert(mFile);
	return ftell(mFile);
}


// write a given byte to the file
bool DiskFile::WriteByte(char value)
{
	assert(mFile);
	assert((mFileMode == WRITE) || (mFileMode == READWRITE) || (mFileMode == READWRITEAPPEND) || (mFileMode == READWRITECREATE));	// make sure we opened the file in write mode

	if (fputc(value, mFile) == EOF)
		return false;

	return true;
}


// seek a given number of bytes ahead from it's current position
bool DiskFile::Forward(int numBytes)
{
	assert(mFile);

	if (fseek(mFile, numBytes, SEEK_CUR) != 0)
		return false;

	return true;
}


// seek to an absolute position in the file (offset in bytes)
bool DiskFile::Seek(int offset)
{
	assert(mFile);

	if (fseek(mFile, offset, SEEK_SET) != 0)
		return false;

	return true;
}


// write data to the file
int DiskFile::Write(void *data, int length)
{
	assert(mFile);
	assert((mFileMode == WRITE) || (mFileMode == READWRITE) || (mFileMode == READWRITEAPPEND) || (mFileMode == READWRITECREATE));	// make sure we opened the file in write mode

	if (fwrite(data, length, 1, mFile) == 0)
		return 0;

	return length;
}


// read data from the file
int DiskFile::Read(void *data, int length)
{
	assert(mFile);
	assert((mFileMode == READ) || (mFileMode == READWRITE) || (mFileMode == READWRITEAPPEND) || (mFileMode == APPEND) || (mFileMode == READWRITECREATE));	// make sure we opened the file in read mode

	if (fread(data, length, 1, mFile) == 0)
		return 0;

	return length;
}


// returns the filesize in bytes
int	DiskFile::GetFileSize() const
{
	assert(mFile);
	if (mFile == NULL)
		return -1;

	// get the current file position
	int curPos = GetPos();

	// seek to the end of the file
	fseek(mFile, 0, SEEK_END);

	// get the position, whis is the size of the file
	int fileSize = GetPos();

	// seek back to the original position
	fseek(mFile, curPos, SEEK_SET);

	// return the size of the file
	return fileSize;
}

} // namespace MCore