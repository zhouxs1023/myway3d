#pragma once

#include "MWCore.h"

namespace Myway
{

struct MW_ENTRY SkinData
{
    DECLARE_ALLOC();

    unsigned char   indices[4];
    float           weights[4];
};

}