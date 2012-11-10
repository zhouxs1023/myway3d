#include "zzip/zzip.h"
#include "MWArchiveZip.h"
#include "MWException.h"

using namespace Myway;

void ArchiveZip::CheckError(int err)
{
    if (err != ZZIP_NO_ERROR)
    {
        TString128 msg = GetErrorDesc(static_cast<zzip_error_t>(err));
        EXCEPTION("Zip Archive '" + mName + "' error: " + err);
    }
}

TString128 ArchiveZip::GetErrorDesc(int err)
{
    TString128 msg;
    switch (err)
    {
    case ZZIP_NO_ERROR:
        break;

    case ZZIP_OUTOFMEM:
        msg = "Out of memory.";
        break;

    case ZZIP_DIR_OPEN:
    case ZZIP_DIR_STAT: 
    case ZZIP_DIR_SEEK:
    case ZZIP_DIR_READ:
        msg = "Unable to read zip file.";
        break;          

    case ZZIP_UNSUPP_COMPR:
        msg = "Unsupported compression format.";
        break; 

    case ZZIP_CORRUPTED:
        msg = "Corrupted archive.";
        break;  

    default:
        msg = "Unknown error.";
        break;            
    };

    return msg;
}


ArchiveZip::ArchiveZip(const TString128 & name, const TString128 & type)
: Archive(name, type),
  mDir(NULL)
{
}

ArchiveZip::~ArchiveZip()
{
}

void ArchiveZip::Load()
{
    if (!mDir)
    {
        zzip_error_t error;
        mDir = zzip_dir_open(mName.c_str(), &error);
        CheckError(error);

        ZZIP_DIRENT dirent;
        while (zzip_dir_read(mDir, &dirent))
        {
            Archive::FileInfo info;
            info.archive = this;
            info.name = dirent.d_name;
            info.name.SplitFileNameR(info.base, info.path);
            info.type = Archive::FILE_ARCHIVE;

            if (info.base.Length() == 0)
            {
                info.name = info.name.SubStr(0, info.name.Length() - 1);
                info.name.SplitFileNameR(info.base, info.path);
                info.type = Archive::FILE_DIRECTORY;
            }

            mFiles.Insert(info.base, info);
        }
    }
}

void ArchiveZip::Unload()
{
    if (mDir)
    {
        zzip_dir_close(mDir);
        mDir = NULL;
        mFiles.Clear();
    }
}


DataStreamPtr ArchiveZip::Open(const TString128 & filename)
{
    // open file
    ZZIP_FILE * file = zzip_file_open(mDir, filename.c_str(), ZZIP_ONLYZIP | ZZIP_CASELESS);
    
    // open failed.
    if (!file)
    {
        int err = zzip_error(mDir);
        CheckError(err);
    }

    // get uncompressed size
    ZZIP_STAT zstat;
    zzip_dir_stat(mDir, filename.c_str(), &zstat, ZZIP_CASEINSENSITIVE);

    MemoryStream * stream = new MemoryStream(zstat.st_size);
    void * data = stream->GetData();
    
    zzip_file_read(file, data, zstat.st_size);
        
    return DataStreamPtr();
}










ArchiveZipFactory::ArchiveZipFactory()
: mType("Zip")
{
}

ArchiveZipFactory::~ArchiveZipFactory()
{
}

const TString128 & ArchiveZipFactory::GetType() const
{
    return mType;
}

Archive * ArchiveZipFactory::CreateInstance(const TString128 & name)
{
    return new ArchiveZip(name, mType);
}

void ArchiveZipFactory::DestroyInstance(Archive * myclass)
{
    delete myclass;
}