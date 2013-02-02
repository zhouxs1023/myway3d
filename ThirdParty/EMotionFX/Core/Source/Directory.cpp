/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#if PLATFORM == PLATFORM_WINDOWS	// only use this when compiling for Windows

#include "Directory.h"
#include <windows.h>


namespace MCore
{

// constructor
Directory::Directory()
{
	mParent = NULL;
}


Directory::Directory(const MCore::String& path)
{
	mParent = NULL;
	ParseDirectory( path );
}



// constructor
Directory::Directory(const MCore::String& directoryName, Directory* parent, MCore::Array<Directory*> subDirectories, MCore::Array<MCore::String> files)
{
	mName		= directoryName;
	mParent		= parent;
}


// destructor
Directory::~Directory()
{
	// remove all subdirectories
	while (mSubDirectories.GetLength())
	{
		delete mSubDirectories.GetLast();
		mSubDirectories.RemoveLast();
	}

	// remove all files
	mFiles.Clear();
}


// parse directory in the file system
void Directory::ParseDirectory(const MCore::String& directoryFullPath)
{
	// check if the string isn't empty
	if (directoryFullPath.Length() <= 0)
		return;

	// windows find data
	WIN32_FIND_DATA findData;

	// fill in string buffer
	char buffer[1024];
	sprintf(buffer, "%s/*", directoryFullPath.AsChar());

	// find the first file in the directory
	HANDLE findHandle = FindFirstFile(buffer, &findData);

	// read in files and sub directories
	if (findHandle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (findData.cFileName[0] != '.' )
			{
				// add a new child directory or file
				if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					mSubDirectories.Add(new Directory(findData.cFileName, this, MCore::Array<Directory*>(), MCore::Array<MCore::String>()));
				}
				else
				{
					mFiles.Add(findData.cFileName);
				}
			}

		} while (FindNextFile(findHandle, &findData));
	}
	else
	{
		// error:unable to parse directory
	}

	// stop directory parsing
	FindClose(findHandle);
}


// set the name of the directory
void Directory::SetName(const MCore::String& name)
{
	mName = name;
}


// set the full path of the directory
void Directory::SetPath(const MCore::String& fullPath)
{
	mPath = fullPath;
}


// set the parent of the directory
void Directory::SetParent(Directory* parent)
{
	mParent = parent;
}


// return the name of the directory
const MCore::String& Directory::GetName() const
{
	return mName;
}


// return the path of the directory
const MCore::String& Directory::GetPath() const
{
	return mPath;
}


// return the parent of the directory
Directory* Directory::GetParent() const
{
	return mParent;
}


// add a sub directory
void Directory::AddSubDirectory(Directory* directory)
{
	mSubDirectories.Add(directory);
}


// return the given sub directory
Directory* Directory::GetSubDirectoryByName(const MCore::String& directoryName)
{
	for (int i=0; i<mSubDirectories.GetLength(); i++)
	{
		Directory* directory = mSubDirectories[i];

		if (directory)
		{
			if (directory->GetName() == directoryName)
				return directory;
		}
	}

	return NULL;
}


// return the given sub directory
Directory* Directory::GetSubDirectory(const int index)
{
	if (index < mSubDirectories.GetLength() && index > -1)
		return mSubDirectories[index];

	return NULL;
}


// return number of sub directories
int Directory::GetNumSubDirectories() const
{
	return mSubDirectories.GetLength();
}


// get sub directories
void Directory::GetSubDirectories(MCore::Array<Directory*>* directories)
{
	for (int i=0; i<mSubDirectories.GetLength(); i++)
	{
		Directory* directory = mSubDirectories[i];

		if (directory)
			directories->Add(directory);
	}
}


// add file
void Directory::AddFile(const MCore::String& fileName)
{

}


// get files
void Directory::GetFiles(MCore::Array<MCore::String>* fileNames, const bool recursiveSearch)
{
	int i;
	for (i=0; i<mFiles.GetLength(); i++)
	{
		fileNames->Add(mFiles[i]);
	}

	if (recursiveSearch)
	{
		for (i=0; i<mSubDirectories.GetLength(); i++)
		{
			Directory* directory = mSubDirectories[i];

			if (directory)
				directory->GetFiles(fileNames, recursiveSearch);
		}
	}
}



// get the number of files in the directory
int Directory::GetNumFiles() const
{
	return mFiles.GetLength();
}


const MCore::String& Directory::GetFile(const int nr) const
{
	return mFiles[nr];
}



}	// namespace MCore

#endif	// #if PLATFORM == PLATFORM_WINDOWS