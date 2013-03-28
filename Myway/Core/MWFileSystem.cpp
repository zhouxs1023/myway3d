#include "MWFileSystem.h"
#include "MWFile.h"
#include "MWException.h"

#ifdef MW_PLATFORM_WIN32
#include <windows.h>
#else
#error current not support.
#endif

using namespace Myway;

FileSystem::FileSystem(const TString128 & name)
: Archive(name, "FileSystem")
{
}

FileSystem::~FileSystem()
{
    Unload();
}

void FileSystem::Load()
{
    if (GetFileInfoSize())
        return ;

	_load("", mName);
}

void FileSystem::_load(const TString128 & prex, const TString128 & dir)
{
#ifdef MW_PLATFORM_WIN32

	TString128 file_path = dir + "\\*.*";
	WIN32_FIND_DATA file;
	HANDLE hFind = FindFirstFile(file_path.c_str(), &file);

	if (hFind == INVALID_HANDLE_VALUE) 
		return;

	Archive::FileInfo info;
	info.archive = this;

	do
	{
		if (file.cFileName[0] == '.')
			continue;

		DWORD nflag = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_TEMPORARY;

		if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			info.name = prex + file.cFileName;
			info.type = FILE_DIRECTORY;
			info.name.Lower();
			mFiles.Insert(info.name, info);

			TString128 prex = info.name + "\\";

			_load(prex, dir + "\\" + file.cFileName);
		}
		else if (!(file.dwFileAttributes & nflag))
		{
			if (!mUseBaseName)
				info.name = prex + file.cFileName;
			else
				info.name = file.cFileName;

			info.type = FILE_DIRECTORY;
			info.type = FILE_ARCHIVE;
			info.name.Lower();
			mFiles.Insert(info.name, info);
		}

	} while (FindNextFile(hFind, &file));

	FindClose(hFind);

#else

#error current not support.

#endif
}

void FileSystem::Unload()
{
    mFiles.Clear();
}

DataStreamPtr FileSystem::Open(const TString128 & filename)
{
    /*File file;

    if (!file.Open(mName + "/" + filename, OM_READ_BINARY))
        EXCEPTION("File Error: " + filename + " can't open.");

    int size = file.Size();

    MemoryStream * stream = new MemoryStream(size);

    void * data = stream->GetData();

    if (!file.Read(data, size))
        EXCEPTION("File Error: " + filename + " read failed");

    file.Close();*/

    FileStream * stream = new FileStream(mName + "/" + filename);

    return DataStreamPtr(stream);
}




FileSystemFactory::FileSystemFactory()
: mType("FileSystem")
{
}

FileSystemFactory::~FileSystemFactory()
{
}

const TString128 & FileSystemFactory::GetType() const
{
    return mType;
}

Archive * FileSystemFactory::CreateInstance(const TString128 & name)
{
    return new FileSystem(name);
}

void FileSystemFactory::DestroyInstance(Archive * myclass)
{
    delete myclass;
}