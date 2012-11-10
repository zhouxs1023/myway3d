#pragma once

#include "MWString.h"
#include "MWTString.h"
#include "MWRefObj.h"
#include "MWSmartPtr.h"
#include "MWFile.h"

namespace Myway
{

class MW_ENTRY DataStream : public RefObj
{
    DeclareRefObj();

public:
    DataStream() {}
    virtual ~DataStream() {}

    virtual const void * GetData() const = 0;
    virtual void * GetData() = 0;

    virtual void Close() = 0;

    virtual int Size() const = 0;

    virtual bool Eof() const = 0;

    virtual int Read(void * data, int size) = 0;
    virtual int ReadLine(void * data, int maxsize) = 0;
    virtual int ReadString(String & data) = 0;

    virtual void Skip(int sk) = 0;
    virtual int SkipLine() = 0;

    virtual void Seek(int pos) = 0;

    virtual int Tell() = 0;
};

DeclareSmartPtr(DataStream);


class MemoryStream : public DataStream
{
public:
    MemoryStream(int size);
    virtual ~MemoryStream();

    virtual const void * GetData() const;
    virtual void * GetData();

    virtual void Close();

    virtual int Size() const;

    virtual bool Eof() const;

    virtual int Read(void * data, int size);
    virtual int ReadLine(void * data, int maxsize);
    virtual int ReadString(String & data);

    virtual void Skip(int sk);
    virtual int SkipLine();

    virtual void Seek(int pos);

    virtual int Tell();

protected:
    int      mSize;
    char *   mData;
    int      mCursor;
    bool     mEndof;
};


class MW_ENTRY FileStream : public DataStream
{
public:
    FileStream(const TString128 & file);
    virtual ~FileStream();

    virtual const void * GetData() const;
    virtual void * GetData();

    virtual void Close();

    virtual int Size() const;

    virtual bool Eof() const;

    virtual int Read(void * data, int size);
    virtual int ReadLine(void * data, int maxsize);
    virtual int ReadString(String & data);

    virtual void Skip(int sk);
    virtual int SkipLine();

    virtual void Seek(int pos);

    virtual int Tell();

protected:
    mutable File        mFile;
    mutable char *      mData;
};


int MW_ENTRY ReadBuffer(void * data, int size, const void * buff, int buffSize, int offset);
int MW_ENTRY WriteBuffer(void * buff, int buffSize, int offset, const void * data, int size);

}