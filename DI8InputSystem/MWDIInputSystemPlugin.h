#pragma once

#include "MWDIInputSystemEntry.h"
#include "MWPlugin.h"

namespace Myway
{
    class DI_ENTRY DIInputSystemPlugin : public Plugin
    {
    public:
        DIInputSystemPlugin();
        virtual ~DIInputSystemPlugin();

        virtual void Install();
        virtual void Uninstall();
    };

}