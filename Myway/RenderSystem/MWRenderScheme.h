#pragma once


#include "MWRenderDefine.h"

namespace Myway
{

class MW_ENTRY RenderLoop
{
    DECLARE_ALLOC();

public:
    RenderLoop() {}
    virtual ~RenderLoop() {}

    virtual void DoRender() = 0;
};

class MW_ENTRY RenderScheme
{
    DECLARE_ALLOC();
    DECLARE_SINGLETON(RenderScheme);

public:
    RenderScheme();
    virtual ~RenderScheme();

    virtual void DoRender() = 0;
};



}