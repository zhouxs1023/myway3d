#include "MWFile.h"
#include "MWDebug.h"

#ifdef MW_PLATFORM_WIN32
#include "Windows.h"
#endif

#pragma warning(push)
#pragma warning(disable : 4999)
#pragma warning(disable : 4996)
#pragma warning(disable : 4267)

using namespace Myway;

const char File::ENDL('\n');


File::File() : mHandle(NULL), mFile(""), mEof(true)
{
}

File::~File()
{
    Close();
}

bool File::Open(const char * file, int mode)
{
	Close();
	switch (mode)
	{
	case OM_READ:
		mHandle = fopen(file,"r");
		break;

	case OM_WRITE:
		mHandle = fopen(file, "w");
		break;

	case OM_APPEND:
		mHandle = fopen(file, "a");
		break;

	case OM_READ_WRITE:
		mHandle = fopen(file, "w+");
		break;

	case OM_READ_WRITE_APPEND:
		mHandle = fopen(file, "a+");
		break;

    case OM_READ_BINARY:
        mHandle = fopen(file, "rb");
        break;

    case OM_WRITE_BINARY:
        mHandle = fopen(file, "wb");
        break;

    case OM_APPEND_BINARY:
        mHandle = fopen(file, "ab");
        break;

    case OM_READ_WRITE_BINARY:
        mHandle = fopen(file, "r+b");
        break;

    case OM_READ_WRITE_APPEND_BINARY:
        mHandle = fopen(file, "a+b");
        break;
	}

	mFile = file;
	mMode = mode;
    mEof = false;
	return mHandle != NULL;
}

bool File::IsOpen()
{
    return mHandle != NULL;
}

void File::Close()
{
	if (mHandle)
	{
		fflush(mHandle);
		fclose(mHandle);
		mHandle = 0;
	}
}

bool File::Skip(int offset)
{
	debug_assert(mHandle, "File is not openning");

	return !fseek(mHandle, offset, SEEK_CUR);
}

bool File::Seek(int pos)
{
	debug_assert(mHandle, "File is not openning");
	return !fseek(mHandle, pos, SEEK_SET);
}

int File::Position()
{
	debug_assert(mHandle, "File is not openning");

	return ftell(mHandle);
}

int File::Size()
{
	debug_assert(mHandle, "File is not openning");

	int cur = ftell(mHandle);
	fseek(mHandle, 0, SEEK_END);
	int size = ftell(mHandle);
	fseek(mHandle, cur, SEEK_SET);
	return size;
}

/*
int File::Length()
{
    debug_assert(mHandle, "File is not openning");

    return (int)_filelength((int)(FILE*)mHandle);
}
*/

void File::Flush()
{
    debug_assert(mHandle, "File is not openning");

    fflush(mHandle);
}

int File::Write(const void * data, int esize, int count)
{
    debug_assert(mHandle, "File is not openning");

    return fwrite(data, esize, count, mHandle);
}

bool File::ReadLine(char * data)
{
    debug_assert(mHandle, "File is not openning");

    mEof = (fgets(data, MAX_LINE_SIZE, mHandle) == NULL);
    return !mEof;
}

bool File::ReadLine(String & data)
{
    debug_assert(mHandle, "File is not openning");

    char c[MAX_LINE_SIZE] = {0};
    
    mEof = (fgets(c, MAX_LINE_SIZE, mHandle) == NULL);
    data = c;

    return !mEof;
}

bool File::ReadString(String & data)
{
    debug_assert(mHandle, "File is not openning");

    data.Clear();

    char str[1024];
    int pos;
    int count = 0;
    bool flag = false;

    while (!flag && !mEof)
    {
        count = fread(str, sizeof(char), 1024 - 1, mHandle);
        str[count] = '\0';

        for (pos = 0; pos < count; ++pos)
        {
            if (str[pos] == '\0')
            {
                flag = true;
                break;
            }
        }

        int sk = pos + 1 - count;

        Skip(sk);

        mEof = (count < 1024 - 1 && sk == 0);
       
        data += str;
    }

    return !mEof;
}

int File::Read(void * data, int esize, int count)
{
    debug_assert(mHandle, "File is not openning");

    int read_count = fread(data, esize, count, mHandle);
    mEof = (read_count != count);
    return read_count;
}

bool File::WriteString(const String & data)
{
    debug_assert(mHandle, "File is not openning");

    return Write(data.c_str(), data.Length() + 1, 1) == 1;
}

TString128 File::GetExternName(const TString128 & file)
{
    int len = file.Length();
    const char * str = file.c_str();

    while (len > 0 && str[len - 1] != '.')
        len -= 1;

    if (len < 1 || str[len - 1] != '.')
        return "";

    return &str[len];
}

TString128 File::GetFileDir(const TString128 & file)
{
	const char * str = file.c_str();
	int len = file.Length();

	while (len > 0 && (str[len - 1] != '\\' && str[len - 1] != '/'))
		--len;

	TString128 dir = file;

	dir[len - 1] = 0;

	return dir;
}

TString128 File::GetBaseName(const TString128 & file)
{
	const char * str = file.c_str();
	int len = file.Length();
	int kLen = len;

	while (len > 0 && (str[len - 1] != '\\' && str[len - 1] != '/'))
		--len;

	char baseName[128];

	int i = 0;
	while (len < kLen)
	{
		baseName[i++] = str[len++];
	}
	
	baseName[i] = 0;

	return baseName;
}

TString128 File::RemoveExternName(const TString128 & file)
{
    int len = file.Length();
    const char * str = file.c_str();
    int p = len;

    while (p > 0 && str[p - 1] != '.')
        p -= 1;

    if (p < 1 || str[p - 1] != '.')
        return file;

    char txt[128];

    Strcpy(txt, 128, file.c_str(), 128);

    txt[p - 1] = 0;

    return txt;
}

TString128 File::GetAbsoluteFileName(const TString128 & file)
{
	char moduleFileName[128] = { 0 };

#ifdef MW_PLATFORM_WIN32
	GetModuleFileName(NULL, moduleFileName, 128);
#endif

	int len = strlen(moduleFileName);
	while (len > 0 && moduleFileName[len] != '\\' && moduleFileName[len] != '/')
	{
		moduleFileName[len--] = 0;
	}

	moduleFileName[len] = 0;

	TString128 absFile = file;

	absFile.Replace('/', '\\');

	if (strstr(absFile.c_str(), "..\\") != NULL)
	{
		char * pfile = moduleFileName;
		const char * cfile = absFile.c_str();

		do 
		{
			len = strlen(pfile);

			while (len > 0 && pfile[len] != '\\' && pfile[len] != '/')
				pfile[len--] = 0;

			pfile[len] = 0;
			cfile += 2;

		} while (strstr(cfile, "..\\") != NULL);

		Strcat(pfile, 128, cfile);

		return pfile;
	}
	else
	{
		return absFile;
	}
}

#pragma warning(pop)
