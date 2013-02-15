#pragma once

#include "MWFMAudioSystemEntry.h"
#include "MWPlugin.h"


namespace Myway
{

class FM_ENTRY FMAudioSystemPlugin : public Plugin
{
public:
    FMAudioSystemPlugin();
    virtual ~FMAudioSystemPlugin();

    virtual void Install();
    virtual void Uninstall();
};

}