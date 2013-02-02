/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __DIRECTORY_H
#define __DIRECTORY_H


#if PLATFORM == PLATFORM_WINDOWS	// only use this when compiling for Windows

#include "String.h"
#include "Array.h"


namespace MCore
{

/**
 * The directory class, which is a util class to easily get a list of files inside a given directory.
 */
class Directory
{
	public:
		/**
		 * Default constructor.
		 */
		Directory();

		/**
		 * Extended constructor, which initializes the directory object for a given path.
		 * This means it automatically collects all information for the given path, such as the filenames and subdirectory names.
		 * So there is no need to call ParseDirectory() anymore, because this is already done by this constructor.
		 * @param path The path to initialize this directory object for.
		 */
		Directory(const MCore::String& path);

		/**
		 * Constructor.
		 * @param directoryName The name of the directory without the path.
		 * @param parent A pointer to the parent directory.
		 * @param subDirectories Array out of pointers to all sub directories.
		 * @param files String array which contains all file names including extension.
		 */
		Directory(const MCore::String& directoryName, Directory* parent, MCore::Array<Directory*> subDirectories, MCore::Array<MCore::String> files);

		/**
		 * Destructor.
		 */
		~Directory();

		/**
		 * Parse a directory in the file system.
		 * @param directoryFullPath Full path of the directory to parse.
		 */
		void ParseDirectory(const MCore::String& directoryFullPath);

		/**
		 * Set name of the directory.
		 * @param name The name without the path.
		 */
		void SetName(const MCore::String& name);

		/**
		 * Set full path of the directory.
		 * @param The full path of the directory.
		 */
		void SetPath(const MCore::String& fullPath);

		/**
		 * Set parent directory.
		 * @param parent A pointer to the parent directory.
		 */
		void SetParent(Directory* parent);

		/**
		 * Return the name of the directory.
		 * @return The name of the directory without the path.
		 */
		const MCore::String& GetName() const;

		/**
		 * Return the full path of the directory.
		 * @return The full path of the directory.
		 */
		const MCore::String& GetPath() const;

		/**
		 * Return a pointer to the parent directory.
		 * @return A pointer to the parent directory.
		 */
		Directory* GetParent() const;

		/**
		 * Add a sub directory.
		 * Child directories will be automatically destroyed with this object.
		 * @param directory A pointer to the child directory to add.
		 */
		void AddSubDirectory(Directory* directory);

		/**
		 * Return the given directory.
		 * NULL will be returned if directory cannot be found.
		 * @param directoryName The name of the directory to search for.
		 * @result A pointer to the directory object, or NULL when it has not been found.
		 */
		Directory* GetSubDirectoryByName(const MCore::String& directoryName);

		/**
		 * Return the given directory.
		 * NULL will be returned if the index is out-of-range or the found directory is not valid.
		 * @param index The index of the directory to return.
		 * @return A pointer to the given directory.
		 */
		Directory* GetSubDirectory(const int index);

		/**
		 * Return number of sub directories.
		 * @return Number of sub directories.
		 */
		int GetNumSubDirectories() const;

		/**
		 * Get access to sub directories.
		 * @param fileNames Array will be filled with the file names. Remark: Array will be cleared before the operation.
		 * @param recursiveSearch If true sub directories will get parsed too, if false they won't get parsed.
		 */
		void GetSubDirectories(MCore::Array<Directory*>* directories);

		/**
		 * Add a file to this directories file list.
		 * @param fileName The name of the file to add.
		 */
		void AddFile(const MCore::String& fileName);

		/**
		 * Get the list of files inside this directory.
		 * @param fileNames Array will be filled with the file names. Remark: Array will be cleared before the operation.
		 * @param recursiveSearch When set to true, sub directories will be parsed as well.
		 */
		void GetFiles(MCore::Array<MCore::String>* fileNames, const bool recursiveSearch=false);

		/**
		 * Get the number of files inside this directory.
		 * @result The number of files.
		 */
		int GetNumFiles() const;

		/**
		 * Get the filename of a given file number.
		 * @param nr The file number, which must be in range of [0 .. GetNumFiles()-1]
		 * @result The string containing the name of the requested file.
		 */
		const MCore::String& GetFile(const int nr) const;


	private:
		MCore::Array<MCore::String>		mFiles;				/**< The array of files contained inside this directory. */
		MCore::Array<Directory*>		mSubDirectories;	/**< The array of subdirectories. */
		Directory*						mParent;			/**< A pointer to the parent directory. */
		MCore::String					mName;				/**< The name of this directory. */
		MCore::String					mPath;				/**< The path of the directory. */
};

}

#endif	// #if PLATFORM == PLATFORM_WINDOWS


#endif
