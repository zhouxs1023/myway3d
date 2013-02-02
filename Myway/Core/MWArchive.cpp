#include "MWArchive.h"

using namespace Myway;

Archive::Archive(const TString128 & name, const TString128 & type)
: mName(name),
  mType(type)
{
}

Archive::~Archive()
{
}

const TString128 & Archive::GetName() const
{
    return mName;
}

const TString128 & Archive::GetType() const
{
    return mType;
}

void Archive::GetFileInfosByFloder(Archive::FileInfoList & files, const TString128 & floder) const
{
	FileInfoMap::ConstIterator iter;
	FileInfoMap::ConstIterator end;

	iter = mFiles.Begin();
	end = mFiles.End();

	while (iter != end)
	{
		if (iter->second.name.IsBegin(floder))
		{
			files.PushBack(iter->second);
		}

		++iter;
	}
}

void Archive::GetFileInfosByKey(FileInfoList & files, const TString128 & key) const
{
    FileInfoMap::ConstIterator iter;
    FileInfoMap::ConstIterator end;

    iter = mFiles.Begin();
    end = mFiles.End();

    while (iter != end)
    {
        if (iter->second.name.Find(key) != NPOS)
        {
            files.PushBack(iter->second);
        }

        ++iter;
    }
}

bool Archive::Exist(const TString128 & name) const
{
    FileInfoMap::ConstIterator iter = mFiles.Find(name.LowerR());
    FileInfoMap::ConstIterator end = mFiles.End();

    return iter != end;
}

const Archive::FileInfo * Archive::GetFileInfo(const TString128 & name) const
{
    FileInfoMap::ConstIterator iter = mFiles.Find(name.LowerR());
    FileInfoMap::ConstIterator end = mFiles.End();

    return iter != end ? &(iter->second) : NULL;
}

Archive::FileInfoVisitor Archive::GetFileInfos() const
{
    return FileInfoVisitor(mFiles.Begin(), mFiles.End());
}

int Archive::GetFileInfoSize() const
{
    return mFiles.Size();
}