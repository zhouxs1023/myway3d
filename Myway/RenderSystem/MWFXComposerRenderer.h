#pragma once

#include "MWRenderer.h"

namespace Myway
{

class MW_ENTRY FXComposerRectObj : public Renderer
{
public:
    FXComposerRectObj();
    ~FXComposerRectObj();

protected:
    virtual void                    Initialize();
};

}