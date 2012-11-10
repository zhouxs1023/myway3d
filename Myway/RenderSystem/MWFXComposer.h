#pragma once

#include "MWFXStep.h"
#include "MWRenderTarget.h"

namespace Myway
{

class MW_ENTRY FXComposer : public AllocObj
{
public:
    FXComposer(const String & sName);
    ~FXComposer();

    void        Initialize();

    void        Active();
    void        AddRenderTarget(RenderTargetPtr target);
    FXStep *    CreateStep();

    const String & GetName();
    FXStep *    GetStep(int index);
    int       GetStepCount();
    RenderTarget *  GetRenderTarget(const String & sName);
    RenderTarget*   GetRenderTarget(int index);
    int       GetRenderTargetCount();

protected:
    String                      m_sName;
    Vector<FXStep>              m_Steps;
    Vector<RenderTargetPtr>     m_RenderTargets;
};

}