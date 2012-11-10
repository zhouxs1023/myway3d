#pragma once

#include "MWD3D9RenderSystemEntry.h"
#include "Myway.h"

namespace Myway
{

class RS_ENTRY D3D9RenderSystemPlugin : public Plugin
{
public:
    D3D9RenderSystemPlugin();
    virtual ~D3D9RenderSystemPlugin();

    virtual void Install();
    virtual void Uninstall();
};

}