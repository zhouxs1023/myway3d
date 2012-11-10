#include "MWPlugin.h"

namespace Myway
{

    Plugin::Plugin(const char * name)
    {
        Strcpy(mName, 32, name);
    }

    Plugin::~Plugin()
    {
    }

    const char * Plugin::GetName()
    {
        return mName;
    }



    PluginManager _mgr;

    IMP_SLN(PluginManager);
    PluginManager::PluginManager()
    {
        INIT_SLN;
        Memzero(mPlugins, sizeof(Plugin*) * MAX_PLUGINS);
    }

    PluginManager::~PluginManager()
    {
        SHUT_SLN;
    }

    void PluginManager::AddPlugin(Plugin * p)
    {
        int i = 0;
        while (i < MAX_PLUGINS && mPlugins[i] != NULL)
        {
            ++i;
        }

        assert (i < MAX_PLUGINS);

        mPlugins[i] = p;
    }

    void PluginManager::InstallAll()
    {
        for (int i = 0; i < MAX_PLUGINS && mPlugins[i] != NULL; ++i)
        {
            mPlugins[i]->Install();
        }
    }

    void PluginManager::UninstallAll()
    {
        for (int i = 0; i < MAX_PLUGINS && mPlugins[i] != NULL; ++i)
        {
            mPlugins[i]->Uninstall();
        }

        Memzero(mPlugins, sizeof(Plugin*) * MAX_PLUGINS);
    }

}