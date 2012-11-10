#pragma once

#include "MWResourceManager.h"

namespace Myway
{
    class ShaderLib;

    class ShaderLibLoader
    {
    public:
        static void Load(ShaderLib * lib, DataStreamPtr stream);
    };
}