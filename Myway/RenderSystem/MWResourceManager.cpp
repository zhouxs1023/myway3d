#include "MWResourceManager.h"

using namespace Myway;


ResourceGroup::ResourceGroup(const TString128 & group)
: mGroup(group)
{
}

ResourceGroup::~ResourceGroup()
{
    Shudown();
}

void ResourceGroup::Init()
{
    ArchiveList::Iterator whr = mArchives.Begin();
    ArchiveList::Iterator end = mArchives.End();

    while (whr != end)
    {
        (*whr)->Load();

        ++whr;
    }
}

void ResourceGroup::Shudown()
{
    ArchiveList::Iterator whr = mArchives.Begin();
    ArchiveList::Iterator end = mArchives.End();

    while (whr != end)
    {
        (*whr)->Unload();
        delete *whr;

        ++whr;
    }

    mArchives.Clear();
}

const TString128 & ResourceGroup::GetName() const
{
    return mGroup;
}

bool ResourceGroup::Exist(const TString128 & name) const
{
    ArchiveList::ConstIterator whr = mArchives.Begin();
    ArchiveList::ConstIterator end = mArchives.End();

    while (whr != end && !(*whr)->Exist(name))
    {
        ++whr;
    }

    return whr != end ? TRUE : FALSE;
}

void ResourceGroup::AddArchive(Archive * archive)
{
    d_assert (GetArchive(archive->GetName()) == NULL);

    mArchives.PushBack(archive);
}

Archive * ResourceGroup::GetArchive(const TString128 & name)
{
    ArchiveList::Iterator whr = mArchives.Begin();
    ArchiveList::Iterator end = mArchives.End();

    while (whr != end && (*whr)->GetName() != name)
    {
        ++whr;
    }

    return whr != end ? *whr : NULL;
}

ResourceGroup::ArchiveVisitor ResourceGroup::GetArchives() const
{
    return ArchiveVisitor(mArchives.Begin(), mArchives.End());
}

int ResourceGroup::GetArchiveSize() const
{
    return mArchives.Size();
}

const Archive::FileInfo * ResourceGroup::GetFileInfo(const TString128 & name) const
{
    ArchiveList::ConstIterator whr = mArchives.Begin();
    ArchiveList::ConstIterator end = mArchives.End();

    const Archive::FileInfo * info = NULL;

    while (whr != end && !info)
    {
        info = (*whr)->GetFileInfo(name);
        ++whr;
    }

    return info;
}

void ResourceGroup::GetFileInfosByKey(Archive::FileInfoList & list, const TString128 & key) const
{
    ArchiveList::ConstIterator whr = mArchives.Begin();
    ArchiveList::ConstIterator end = mArchives.End();

    while (whr != end)
    {
        (*whr)->GetFileInfosByKey(list, key);
        ++whr;
    }
}


void ResourceGroup::RemoveArchive(const TString128 & name)
{
    ArchiveList::Iterator whr = mArchives.Begin();
    ArchiveList::Iterator end = mArchives.End();

    while (whr != end && (*whr)->GetName() != name)
    {
        ++whr;
    }

    if (whr != end)
    {
        mArchives.Erase(whr);
    }
}

void ResourceGroup::RemoveArchive(Archive * archive)
{
    RemoveArchive(archive->GetName());
}

DataStreamPtr ResourceGroup::Open(const TString128 & name)
{
    ArchiveList::Iterator whr = mArchives.Begin();
    ArchiveList::Iterator end = mArchives.End();

    while (whr != end && !(*whr)->Exist(name))
    {
        ++whr;
    }

    assert (whr != end);
    
    return (*whr)->Open(name);
}






IMP_SLN(ResourceManager);
ResourceManager::ResourceManager()
{
    INIT_SLN;
    mResourceLoader = new ResourceLoader();
}

ResourceManager::~ResourceManager()
{
    SHUT_SLN;
    Shutdown();
    
    safe_delete(mResourceLoader);
}

void ResourceManager::Init()
{
    ResourceGroupMap::Iterator whr = mGroups.Begin();
    ResourceGroupMap::Iterator end = mGroups.End();

    while (whr != end)
    {
        whr->second->Init();
        ++whr;
    }

}

void ResourceManager::Shutdown()
{
    FactoryMap::Iterator fwhr = mFactorys.Begin();
    FactoryMap::Iterator fend = mFactorys.End();

    while (fwhr != fend)
    {
        delete fwhr->second;
        ++fwhr;
    }

    mFactorys.Clear();

    ResourceGroupMap::Iterator rwhr = mGroups.Begin();
    ResourceGroupMap::Iterator rend = mGroups.End();

    while (rwhr != rend)
    {
        rwhr->second->Shudown();
        delete rwhr->second;
        ++rwhr;
    }

    mGroups.Clear();
}

void ResourceManager::AddArchiveFactory(ArchiveFactory * factory)
{
    mFactorys.Insert(factory->GetType(), factory);
}

void ResourceManager::RemoveArchiveFactory(const TString128 & type)
{
    FactoryMap::Iterator whr = mFactorys.Find(type);
    FactoryMap::Iterator end = mFactorys.End();

    if (whr != end)
    {
        delete whr->second;
        mFactorys.Erase(whr);
    }
}

void ResourceManager::AddArchive(const TString128 & name, const TString128 & type, const TString128 & group)
{
    ResourceGroupMap::Iterator rwhr = mGroups.Find(group);
    ResourceGroupMap::Iterator rend = mGroups.End();

    if (rwhr == rend)
    {
        AddGroup(group);

        rwhr = mGroups.Find(group);
        rend = mGroups.End();

        d_assert (rwhr != rend);
    }

    FactoryMap::Iterator fwhr = mFactorys.Find(type);
    FactoryMap::Iterator fend = mFactorys.End();

    if (fwhr == fend)
    {
        EXCEPTION("Source: " + name + " unknown type '" + type + "'.");
    }

    Archive * archive = fwhr->second->CreateInstance(name);

    rwhr->second->AddArchive(archive);
}

bool ResourceManager::Exist(const TString128 & name, const TString128 & group) const
{
    ResourceGroupMap::ConstIterator whr = mGroups.Find(group);
    ResourceGroupMap::ConstIterator end = mGroups.End();

    if (whr != end)
    {
        return whr->second->Exist(name);
    }

    return FALSE;
}

Archive * ResourceManager::GetArchive(const TString128 & name, const TString128 & group)
{
    ResourceGroupMap::Iterator whr = mGroups.Find(group);
    ResourceGroupMap::Iterator end = mGroups.End();

    if (whr != end)
    {
        return whr->second->GetArchive(name);
    }

    return FALSE;
}

const Archive::FileInfo * ResourceManager::GetFileInfo(const TString128 & name, const TString128 & group) const
{
    ResourceGroupMap::ConstIterator whr = mGroups.Find(group);
    ResourceGroupMap::ConstIterator end = mGroups.End();

    if (whr == end)
    {
        EXCEPTION("Source: " + name + " not exit in resource group '" + group + "'.");
    }

    return whr->second->GetFileInfo(name);
}

void ResourceManager::GetFileInfosByKey(Archive::FileInfoList & list, const TString128 & key) const
{
    ResourceGroupMap::ConstIterator whr = mGroups.Begin();
    ResourceGroupMap::ConstIterator end = mGroups.End();

    while (whr != end)
    {
        whr->second->GetFileInfosByKey(list, key);
        ++whr;
    }
}

void ResourceManager::AddGroup(const TString128 & group)
{
    mGroups.Insert(group, new ResourceGroup(group));
}

ResourceGroup * ResourceManager::GetGroup(const TString128 & group)
{
    ResourceGroupMap::Iterator whr = mGroups.Find(group);
    ResourceGroupMap::Iterator end = mGroups.End();

    return whr != end ? whr->second : NULL;
}

ResourceManager::ResourceGroupVisitor ResourceManager::GetGroups() const
{
    return ResourceGroupVisitor(mGroups.Begin(), mGroups.End());
}

void ResourceManager::RemoveGroup(const TString128 & group)
{
    ResourceGroupMap::Iterator whr = mGroups.Find(group);
    ResourceGroupMap::Iterator end = mGroups.End();

    if (whr != end)
    {
        delete whr->second;
        mGroups.Erase(whr);
    }
}

void ResourceManager::RemoveGroup(ResourceGroup * group)
{
    RemoveGroup(group->GetName());
}

DataStreamPtr ResourceManager::OpenResource(const char * source, const char * group)
{
    if (strcmp(group, "") == 0)
        return OpenResource(source);

    ResourceGroupMap::Iterator whr = mGroups.Find(group);
    ResourceGroupMap::Iterator end = mGroups.End();

    if (whr == end || !whr->second->Exist(source))
    {
        LOG_PRINT_FORMAT("Source: %s not exit in resource group %s", source, group);
        return NULL;
    }

    return whr->second->Open(source);
}

DataStreamPtr ResourceManager::OpenResource(const char * source)
{
    ResourceGroupMap::Iterator whr = mGroups.Begin();
    ResourceGroupMap::Iterator end = mGroups.End();

    while (whr != end && whr->second->Exist(source))
    {
        return whr->second->Open(source);
    }

    LOG_PRINT_FORMAT("Source: %s not exit", source);
    return NULL;
}

void ResourceManager::SetResourceLoader(ResourceLoader * loader)
{
    d_assert (loader);

    if (mResourceLoader != loader)
    {
        delete mResourceLoader;
        mResourceLoader = loader;
    }
}

ResourceLoader * ResourceManager::GetResourceLoader()
{
    return mResourceLoader;
}

