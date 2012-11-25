#pragma once

#include "MWArchive.h"

namespace Myway
{
// 1. 加入文件筛选器
// 2. 导入文件
// 3. 根据需要取文件列表

class MW_ENTRY FileSystem : public Archive
{
    /*
public:
    enum FileType
    {
        FILE_DIRECTORY          = 0,
        FILE_ARCHIVE,

        MW_ALIGN_ENUM(FileType)
    };

    struct FileTime
    {
        int low_date_time;
        int high_date_time;
    };

    struct FileInfo								
    {
        TString128      name;
        FileType    type;
        FileTime    ftCreationTime;
        FileTime    ftLastAccessTime;
        FileTime    ftLastWriteTime;
        int      nFileSizeHigh;
        int      nFileSizeLow;
    };

    typedef HashMap<TString128, FileInfo, TString128::hash_t> FileInfoMap;
    typedef Visitor<FileInfoMap::ConstIterator> FileVisitor;
    */

public:
    FileSystem(const TString128 & name);
    virtual~FileSystem();

    virtual void Load();
    virtual void Unload();

    virtual DataStreamPtr Open(const TString128 & filename);

protected:
	void _load(const TString128 & prex, const TString128 & dir);
};

class MW_ENTRY FileSystemFactory : public ArchiveFactory
{
public:
    FileSystemFactory();
    virtual ~FileSystemFactory();

    virtual const TString128 & GetType() const;

    virtual Archive * CreateInstance(const TString128 & name);
    virtual void DestroyInstance(Archive * myclass);

protected:
    TString128  mType;
};

}
