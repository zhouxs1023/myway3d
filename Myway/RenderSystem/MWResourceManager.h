#pragma once

#include "MWCore.h"
#include "MWResource.h"

namespace Myway
{

class MW_ENTRY ResourceGroup : public AllocObj
{
    friend class ResourceManager;

public:
    typedef List<Archive *>                                 ArchiveList;
    typedef Visitor<ArchiveList::ConstIterator>             ArchiveVisitor;

public:
    ResourceGroup();
    ~ResourceGroup();

    void Init();
    void Shudown();

    bool Exist(const TString128 & name) const;

    void AddArchive(Archive * archive);
    Archive * GetArchive(const TString128 & name);
    ArchiveVisitor GetArchives() const;
    int GetArchiveSize() const;

    const Archive::FileInfo * GetFileInfo(const TString128 & name) const;
	void GetFileInfosByFloder(Archive::FileInfoList & list, const TString128 & floder) const;
    void GetFileInfosByKey(Archive::FileInfoList & list, const TString128 & key) const;

    void RemoveArchive(const TString128 & name);
    void RemoveArchive(Archive * archive);

    DataStreamPtr Open(const TString128 & name);

protected:
    TString128      mGroup;
    ArchiveList     mArchives;
};



class MW_ENTRY ResourceManager
{
    DECLARE_SINGLETON (ResourceManager);
public:
    typedef Map<TString128, ArchiveFactory*>                FactoryMap;

public:
    ResourceManager();
    ~ResourceManager();

    void Init();
    void Shutdown();

    void AddArchiveFactory(ArchiveFactory * factory);
    void RemoveArchiveFactory(const TString128 & type);
    ArchiveFactory * GetArchiveFactory(const TString128 & type);

    Archive * AddArchive(const TString128 & name, const TString128 & type);
    bool Exist(const TString128 & name) const;
    Archive * GetArchive(const TString128 & name);

	const Archive::FileInfo * GetFileInfo(const TString128 & name) const;

	void GetFileInfosByFloder(Archive::FileInfoList & list, const TString128 & floder) const;
    void GetFileInfosByKey(Archive::FileInfoList & list, const TString128 & key) const;

    DataStreamPtr OpenResource(const char * source, bool _notInFileSystem = false);

    void SetResourceLoader(ResourceLoader * loader);
    ResourceLoader * GetResourceLoader();

	ResourceGroup * GetResourceGroup() { return &mResourceGroup; }

protected:
    FactoryMap                      mFactorys;
    ResourceLoader *                mResourceLoader;
	ResourceGroup					mResourceGroup;
};

}