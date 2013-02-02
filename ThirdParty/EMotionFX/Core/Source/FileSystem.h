/*
 * MysticGD Core Library
 * Copyright (c) 2001-2004 Mystic Game Development - http://www.mysticgd.com
 * All Rights Reserved.
 */

#ifndef __FILESYSTEM_H
#define __FILESYSTEM_H

#include "SourceDeclaration.h"


namespace MCore
{

/**
 * The file system class.
 * This currently contains some utility classes to deal with files on disk.
 */
class FileSystem
{
	public:
		/**
		 * Check if a specified file exists and can be opened.
		 * @param filename The filename of the file we'd like to check.
		 * @result Returns true when the file exists and can be opened, otherwise false is returned.
		 */
		static bool FileExists(const MCore::String& filename);
};


} // namespace MCore

#endif	