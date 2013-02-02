/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __DISKFILE_H
#define __DISKFILE_H

#include "File.h"
#include "String.h"

namespace MCore
{

/**
 * A standard file as we normally think of. In other words, a file stored on the harddisk or a CD or any other comparable medium.
 * The implementation should be platform independent.
 */
class DiskFile : public File
{
	DECLARE_CLASS(DiskFile)

	public:
		/**
		 * File opening modes.
		 * Do not use a combination of these modes. Just pick one.
		 */
		enum EMode
		{
			READ			= 0x00000001,	/**< open for reading, if the file doesn't exist the Open method will fail. */
			WRITE			= 0x00000002,	/**< open for writing, if the file already exists it will be overwritten. */
			READWRITE		= 0x00000003,	/**< opens the file for both reading and writing, the file must already exist else the Open method will fail. */
			READWRITECREATE	= 0x00000004,	/**< opens the file for both reading and writing, if the file exists already it will be overwritten. */
			APPEND			= 0x00000005,	/**< opens for writing at the end of the file, will create a new file if it doesn't yet exist. */
			READWRITEAPPEND = 0x00000006 	/**< opens for reading and appending (writing), creates the file when it doesn't exist. */
		};

		/**
		 * The constructor.
		 */
		DiskFile() : File(), mFile(NULL) {}

		/**
		 * The destructor. Automatically closes the file.
		 */
		~DiskFile() { Close(); }

		/**
		 * Try to open the file, given a filename and open mode.
		 * @param filename The filename on disk.
		 * @param mode The file open mode. See enum EMode for more information. Do NOT use a combination of these modes.
		 * @param binary If true is specified, the file is being opened in binary mode, otherwise in textmode.
		 * @result Returns true when successfully opened the file, otherwise false is returned.
		 */
		bool Open(const char *filename, EMode mode, bool binary=true);

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
		 * @result Returns the number of bytes read.
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

		/**
		 * Returns the mode the file has been opened with.
		 * @result The mode the file has been opened with. See enum EMode.
		 */
		EMode GetFileMode() const			{ return mFileMode; }

		/**
		 * Returns the name of the file as it has been opened.
		 * @result The string containing the filename as it has been passed to the method Open.
		 */
		String GetFilename() const			{ return mFilename; }


	private:
		FILE*	mFile;		/**< The file handle. */
		EMode	mFileMode;	/**< The mode we opened the file with. */
		String	mFilename;	/**< The filename */
};

} // namespace MCore

#endif