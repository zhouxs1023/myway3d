#include "MWDataStream.h"
#include "MWFile.h"
#include "MWException.h"

namespace Myway
{


void DataStream::DeleteSelf()
{
    delete this;
}




MemoryStream::MemoryStream(int size)
: mSize(size),
  mCursor(0)
{
    mData = (char*)alloc_mem(size + 1);
    mEndof = size ? FALSE : TRUE;
    mData[mSize] = '\0';
}

MemoryStream::~MemoryStream()
{
    Close();
}

void MemoryStream::Close()
{
    safe_free(mData);

    mSize= 0;
    mCursor= 0;
    mEndof = TRUE;
}

const void * MemoryStream::GetData() const
{
    return mData;
}

void * MemoryStream::GetData()
{
    return mData;
}


bool MemoryStream::Eof() const
{
    return mEndof;
}

int MemoryStream::Read(void * data, int size)
{
    if (!mEndof)
    {
        int count = mSize - mCursor;

        if (size > count)
            size = count; 

        Memcpy(data, &mData[mCursor], size);

        mCursor += size;

        mEndof = (mCursor >= mSize);

        return size;
    }

    return 0;
}

int MemoryStream::ReadLine(void * data, int maxsize)
{
    char * buf = (char *)data;
    int size = 0;

    if (!mEndof)
    {

        while (size < maxsize && mData[mCursor] != '\n' && mCursor < mSize)
        {
                buf[size++] = mData[mCursor++];
        }

        mEndof = (mCursor >= mSize);

        if (mData[mCursor] == '\n')
            ++mCursor;
    }

    return 0;
}

int MemoryStream::ReadString(String & data)
{
    data.Clear();
    int count = 0;

    if (!mEndof)
    {
        char str[1024];
        int size;
        int pos;
        int sk;

        while (!mEndof)
        {
            size = Read(str, sizeof(char) * (1024 - 1));
            str[size] = '\0';

            pos = 0;
            while (pos < size && str[pos] != '\0')
            {
                ++pos;
            }

            data += str;

            if (pos < size)
            {
                count += pos + 1;
                sk = pos + 1 - size;
                Skip(sk);
                break;
            }
            else
            {
                count += pos;
            }
        }
    }

    return count;
}

void MemoryStream::Skip(int sk)
{
    if (sk < 0 && int(-sk) > mCursor)
    {
        mCursor = 0;
        mEndof = TRUE;
    }
    else
    {
        mCursor += sk;

        mEndof = (mCursor >= mSize);
    }
}

int MemoryStream::SkipLine()
{
    int cur = mCursor;

    while (mCursor < mSize)
    {
        if (mData[mCursor++] == '\n')
            break;
    }

    mEndof = (mCursor >= mSize);

    return mCursor - cur;
}

void MemoryStream::Seek(int pos)
{
    mCursor = pos;

    mEndof = (mCursor >= mSize);
}

int MemoryStream::Tell()
{
    return mCursor;
}

int MemoryStream::Size() const
{
    return mSize;
}




FileStream::FileStream(const TString128 & file)
{
    mData = NULL;
    mFile.Open(file.c_str(), OM_READ_BINARY);

    d_assert (mFile.IsOpen());
}

FileStream::~FileStream()
{
    Close();
}

void FileStream::Close()
{
    mFile.Close();
    safe_delete_array(mData);
}

int FileStream::Size() const
{
    return mFile.Size();
}

bool FileStream::Eof() const
{
    return mFile.eof();
}

int FileStream::Read(void * data, int size)
{
    return mFile.Read(data, 1, size);
}

int FileStream::ReadLine(void * data, int maxsize)
{
    char * buf = (char *)data;
    int size = 0;

    if (!Eof())
    {
        char c;

        mFile.Read(&c, sizeof(char), 1);

        while (size < maxsize && c != '\n' && !Eof())
        {
            buf[size++] = c;
        }
    }

    return size;
}

int FileStream::ReadString(String & data)
{
    return mFile.ReadString(data) ? data.Length() : 0;
}

void FileStream::Skip(int sk)
{
    mFile.Skip(sk);
}

int FileStream::SkipLine()
{
    int size = 0;

    if (!Eof())
    {
        char c;

        mFile.Read(&c, sizeof(char), 1);

        while (c != '\n' && !Eof())
        {
            ++size;
        }
    }

    return size;
}

void FileStream::Seek(int pos)
{
    mFile.Seek(pos);
}

int FileStream::Tell()
{
    return mFile.Position();
}

const void * FileStream::GetData() const
{
    if (!mData)
    {
        int size = Size();
        mData = new char[size + 1];
        mFile.Read(mData, size);
        mData[size] = 0;
    }

    return mData;
}

void * FileStream::GetData()
{
    if (!mData)
    {
        int size = Size();
        mData = new char[size + 1];
        mFile.Read(mData, size);
        mData[size] = 0;
    }

    return mData;
}



int Myway::ReadBuffer(void * data, int size, const void * buff, int buffSize, int offset)
{
    assert (offset + size <= buffSize);

    if (offset + size > buffSize)
        return 0;

    char * dest = (char *)data;
    const char * src = (const char *)buff + offset;

    Memcpy(dest, src, size);

    return size;
}

int Myway::WriteBuffer(void * buff, int buffSize, int offset, const void * data, int size)
{
    assert (offset + size <= buffSize);

    if (offset + size > buffSize)
        return 0;

    char * dest = (char *)buff + offset;
    const char * src = (const char *)data;

    Memcpy(dest, src, size);

    return size;
}

}
