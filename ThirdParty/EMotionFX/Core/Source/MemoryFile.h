/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __MEMORYFILE_H
#define __MEMORYFILE_H

#include "File.h"

namespace MCore
{

/**
 * A file stored in memory, located at a given position with a known length in bytes.
 */
class MemoryFile : public File
{
	DECLARE_CLASS(MemoryFile)

	public:
		/**
		 * Constructor.
		 */
		MemoryFile() : File(), mMemoryStart(NULL), mCurrentPos(NULL), mLength(0) {}

		/**
		 * Destructor. Automatically closes the file.
		 */
		~MemoryFile() { Close(); }

		/**
		 * Open the file from a given memory location, with a given length in bytes.
		 * @param memoryStart The memory position of the first byte in the file.
		 * @param length The length in bytes of the file. So the size in bytes of the memory block.
		 * @result Returns true when the file could be opened, otherwise false.
		 */
		bool Open(unsigned char* memoryStart, int length);

		/**
		 * Close the file.
		 */
		void Close();

		/**
		 * Flush the file. All cached (not yet written) data will be forced to written when calling this method.
		 */
		void Flush();

		/**
		 * Returns true if we reached the end of this file, otherwise false is returned.
		 */
		bool IsEOF() const;

		/**
		 * Reads and returns the next byte/character in the file. So this will increase the position in the file with one.
		 * @result The character or byte read.
		 */
		char GetNextByte();

		/**
		 * Returns the position in the file.
		 * @result The offset in the file in bytes.
		 */
		int GetPos() const;

		/**
		 * Write a character or byte in the file.
		 * @param value The character or byte to write.
		 * @result Returns true when successfully written, otherwise false is returned.
		 */
		bool WriteByte(char value);

		/**
		 * Seek ahead a given number of bytes. This can be used to skip a given number of upcoming bytes in the file.
		 * @param numBytes The number of bytes to seek ahead.
		 * @result Returns true when succeeded or false when an error occured (for example when we where trying to read past the end of the file).
		 */
		bool Forward(int numBytes);

		/**
		 * Seek to a given byte position in the file, where 0 would be the beginning of the file.
		 * So we're talking about the absolute position in the file. After successfully executing this method
		 * the method GetPos will return the given offset.
		 * @param offset The offset in bytes (the position) in the file to seek to.
		 * @result Returns true when successfully moved to the given position in the file, otherwise false.
		 */
		bool Seek(int offset);

		/**
		 * Writes a given amount of data to the file.
		 * @param data The pointer to the data to write.
		 * @param length The size in bytes of the data to write.
		 * @result Returns the number of written bytes.
		 */
		int Write(void *data, int length);

		/**
		 * Read a given amount of data from the file.
		 * @param data The pointer where to store the read data.
		 * @param length The size in bytes of the data to read.
		 * @result The number of bytes read.
		 */
		int Read(void *data, int length);

		/**
		 * Check if the file has been opened already.
		 * @result Returns true when the file has been opened, otherwise false.
		 */
		bool IsOpen() const;

		/**
		 * Returns the size of this file in bytes.
		 * @result The filesize in bytes.
		 */
		int	 GetFileSize() const;

	private:
		unsigned char*	mMemoryStart;	/**< the location of the file */
		unsigned char*	mCurrentPos;	/**< the current location */
		int				mLength;		/**< the total length of the file */
};

} // namespace MCore

#endif	