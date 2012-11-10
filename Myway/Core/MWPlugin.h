#pragma once

#include "MWDebug.h"
#include "MWSingleton.h"

namespace Myway
{

class MW_ENTRY Plugin : public AllocObj
{
public:
    Plugin(const char * name);
    virtual ~Plugin();

    const char * GetName();

    virtual void Install() = 0;
    virtual void Uninstall() = 0;

protected:
    char mName[32];
};

class MW_ENTRY PluginManager : public AllocObj
{
    DECLARE_SINGLETON(PluginManager);

    static const int MAX_PLUGINS = 2000;

public:
    PluginManager();
    ~PluginManager();

    void AddPlugin(Plugin * p);

    void InstallAll();
    void UninstallAll();

protected:
    Plugin *   mPlugins[MAX_PLUGINS];
};

}