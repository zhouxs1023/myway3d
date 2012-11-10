#pragma once

#include "MWArchive.h"

struct zzip_dir;

typedef zzip_dir ZZIP_DIR;

namespace Myway
{

//class MW_ENTRY ZipStream : public DataStream
//{
//public:
//    ZipStream();
//    virtual ~ZipStream();
//
//    virtual void Close();
//
//    virtual int Size();
//
//    virtual bool Eof() const;
//
//    virtual int Read(void * data, int size);
//    virtual int ReadLine(void * data, int maxsize);
//    virtual int ReadTString128(TString128 & data);
//
//    virtual void Skip(int sk);
//    virtual int SkipLine();
//
//    virtual void Seek(int pos);
//
//    virtual int Tell();
//
//protected:
//};

class MW_ENTRY ArchiveZip : public Archive
{
public:
    ArchiveZip(const TString128 & name, const TString128 & type);
    virtual ~ArchiveZip();

    virtual void Load();
    virtual void Unload();

    virtual DataStreamPtr Open(const TString128 & filename);

protected:
    void CheckError(int err);
    TString128 GetErrorDesc(int err);

protected:
    Archive::FileInfoMap       mItems;
    ZZIP_DIR *                 mDir;
};




class MW_ENTRY ArchiveZipFactory : public ArchiveFactory
{
public:
    ArchiveZipFactory();
    virtual ~ArchiveZipFactory();

    virtual const TString128 & GetType() const;

    virtual Archive * CreateInstance(const TString128 & name);
    virtual void DestroyInstance(Archive * myclass);

protected:
    TString128  mType;
};

}