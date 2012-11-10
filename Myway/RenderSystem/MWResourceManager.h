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
    ResourceGroup(const TString128 & group);
    ~ResourceGroup();

    void Init();
    void Shudown();

    const TString128 & GetName() const;

    bool Exist(const TString128 & name) const;

    void AddArchive(Archive * archive);
    Archive * GetArchive(const TString128 & name);
    ArchiveVisitor GetArchives() const;
    int GetArchiveSize() const;

    const Archive::FileInfo * GetFileInfo(const TString128 & name) const;
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
    typedef Map<TString128, ResourceGroup*>                 ResourceGroupMap;
    typedef Visitor<ResourceGroupMap::ConstIterator>        ResourceGroupVisitor;

public:
    ResourceManager();
    ~ResourceManager();

    void Init();
    void Shutdown();

    void AddArchiveFactory(ArchiveFactory * factory);
    void RemoveArchiveFactory(const TString128 & type);
    ArchiveFactory * GetArchiveFactory(const TString128 & type);

    void AddArchive(const TString128 & name, const TString128 & type, const TString128 & group);
    bool Exist(const TString128 & name, const TString128 & group) const;
    Archive * GetArchive(const TString128 & name, const TString128 & group);

    const Archive::FileInfo * GetFileInfo(const TString128 & name, const TString128 & group) const;
    void GetFileInfosByKey(Archive::FileInfoList & list, const TString128 & key) const;

    void AddGroup(const TString128 & group);
    ResourceGroup * GetGroup(const TString128 & group);
    ResourceGroupVisitor GetGroups() const;
    void RemoveGroup(const TString128 & group);
    void RemoveGroup(ResourceGroup * group);

    DataStreamPtr OpenResource(const char * source);
    DataStreamPtr OpenResource(const char * source, const char * group);

    void SetResourceLoader(ResourceLoader * loader);
    ResourceLoader * GetResourceLoader();

protected:
    ResourceGroupMap                mGroups;
    FactoryMap                      mFactorys;
    ResourceLoader *                mResourceLoader;
};

}