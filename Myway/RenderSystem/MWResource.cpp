#include "MWResource.h"
#include "MWResourceManager.h"

namespace Myway
{
    Resource::Resource()
        : mLoadState(UNLOADED)
        , mSourceName("")
        , mGroupName("")
    {
    }

    Resource::~Resource()
    {
    }










    ResourceLoader::ResourceLoader()
    {
    }

    ResourceLoader::~ResourceLoader()
    {
    }

    void ResourceLoader::Load(Resource * res)
    {
        ForceLoad(res);
    }

    void ResourceLoader::ForceLoad(Resource * res)
    {
        if (res->GetLoadState() == Resource::LOADED)
            return ;

        const char * source = res->GetSourceName().c_str();

        DataStreamPtr stream = ResourceManager::Instance()->OpenResource(source);
        res->LoadImp(stream);
    }
}