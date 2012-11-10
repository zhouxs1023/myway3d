#include "MWFileSystem.h"
#include "MWFile.h"
#include "MWException.h"

#ifdef MW_PLATFORM_WIN32
#include <windows.h>
#else
#error current not support.
#endif

using namespace Myway;

FileSystem::FileSystem(const TString128 & name, const TString128 & type)
: Archive(name, type)
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

#ifdef MW_PLATFORM_WIN32

    TString128 file_path = mName + "\\*.*";
    WIN32_FIND_DATA file;
    HANDLE hFind = FindFirstFile(file_path.c_str(), &file);

    if (hFind == INVALID_HANDLE_VALUE) 
        return;

    Archive::FileInfo info;
    info.archive = this;
    info.path = mName;

    do
    {
        if (file.cFileName[0] == '.')
            continue;

        if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            info.base = file.cFileName;
            info.name = mName + '/' + info.base;
            info.type = FILE_DIRECTORY;
            info.base.Lower();
            info.name.Lower();
            mFiles.Insert(info.base, info);
        }
        else if (file.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
        {
            info.base = file.cFileName;
            info.name = mName + '/' + info.base;
            info.type = FILE_DIRECTORY;
            info.type = FILE_ARCHIVE;
            info.base.Lower();
            info.name.Lower();
            mFiles.Insert(info.base, info);
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
    return new FileSystem(name, mType);
}

void FileSystemFactory::DestroyInstance(Archive * myclass)
{
    delete myclass;
}