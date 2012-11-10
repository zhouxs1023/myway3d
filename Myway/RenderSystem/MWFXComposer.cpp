#include "MWFXComposer.h"

using namespace Myway;

FXComposer::FXComposer(const String & sName)
: m_sName(sName)
{
}

FXComposer::~FXComposer()
{
}

void FXComposer::Initialize()
{
    for (int i = 0; i < m_Steps.Size(); ++i)
    {
        m_Steps[i].Initialize();
    }
}

void FXComposer::Active()
{
    for (int i = 0; i < m_Steps.Size(); ++i)
    {
        m_Steps[i].NotifyActive();
    }
}

void FXComposer::AddRenderTarget(RenderTargetPtr target)
{
    m_RenderTargets.PushBack(target);
}

FXStep * FXComposer::CreateStep()
{
    return m_Steps.Increase();
}

const String & FXComposer::GetName()
{
    return m_sName;
}

FXStep * FXComposer::GetStep(int index)
{
    return &m_Steps[index];
}

int FXComposer::GetStepCount()
{
    return m_Steps.Size();
}

RenderTarget * FXComposer::GetRenderTarget(const String & sName)
{
    for (int i = 0; i < m_RenderTargets.Size(); ++i)
    {
        if (m_RenderTargets[i]->GetName() == sName)
        {
            return m_RenderTargets[i].GetPointer();
        }
    }

    return NULL;
}

RenderTarget * FXComposer::GetRenderTarget(int index)
{
    return m_RenderTargets[index].GetPointer();
}

int FXComposer::GetRenderTargetCount()
{
    return m_RenderTargets.Size();
}