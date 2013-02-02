/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

// include the required headers
#include "FileSystem.h"
#include "DiskFile.h"


namespace MCore
{


// check if a given file exists
bool FileSystem::FileExists(const MCore::String& filename)
{
	// try to open the disk file
	DiskFile file;
	if (file.Open(filename, DiskFile::READ))
	{
		file.Close();
		return true;
	}

	// we couldn't open the disk file
	return false;
}


}	// namespace MCore
