#pragma once

#include "MWFactory.h"
#include "MWHashMap.h"
#include "MWList.h"
#include "MWVisitor.h"
#include "MWTString.h"
#include "MWDataStream.h"

namespace Myway
{

class MW_ENTRY Archive
{
public:
    enum FileType
    {
        FILE_DIRECTORY          = 0,
        FILE_ARCHIVE,

        MW_ALIGN_ENUM(FileType)
    };

    struct FileInfo
    {
        Archive *   archive;
        FileType    type;
        TString128  name;
    };

    typedef List<FileInfo>  FileInfoList;
    typedef HashMap<TString128, FileInfo, TString128::hash_t> FileInfoMap;
    typedef Visitor<FileInfoMap::ConstIterator> FileInfoVisitor;

public:
    Archive(const TString128 & name, const TString128 & type);
    virtual ~Archive();

    const TString128 & GetName() const;
    const TString128 & GetType() const;

	void GetFileInfosByFloder(Archive::FileInfoList & list, const TString128 & floder) const;
    void GetFileInfosByKey(FileInfoList & files, const TString128 & key) const;
    bool Exist(const TString128 & name) const;
    const FileInfo * GetFileInfo(const TString128 & name) const; 
    FileInfoVisitor GetFileInfos() const;
    int GetFileInfoSize() const;

    virtual void Load() = 0;
    virtual void Unload() = 0;
    virtual DataStreamPtr Open(const TString128 & filename) = 0;

protected:
    TString128 mName;
    TString128 mType;
    FileInfoMap mFiles;
};

class MW_ENTRY ArchiveFactory : public Factory<Archive>
{
public:
    ArchiveFactory() {}
    virtual ~ArchiveFactory() {}
};

}