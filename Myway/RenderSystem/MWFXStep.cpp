#include "MWFXStep.h"
#include "MWMaterialManager.h"

using namespace Myway;


FXStep::FXStep()
: m_RenderMode(FXRM_DEFAULT),
  m_RenderMark(0xFFFFFFFF),
  m_RenderFrame(1),
  m_bEnable(FALSE),
  m_CurFrame(0)
{
    for (int i = 0; i < MAX_RENDER_TARGET; ++i)
        m_RenderTarget[i] = NULL;
}

FXStep::~FXStep()
{
}

void FXStep::UpdateFrame()
{
    if (++m_CurFrame == m_RenderFrame)
    {
        m_CurFrame = 0;
        m_bEnable = TRUE;
    }
    else 
        m_bEnable = FALSE;
}

bool FXStep::GetEnable()
{
    return m_bEnable;
}

void FXStep::SetRenderMode(FX_RENDER_MODE mode)
{
    m_RenderMode = mode;
}

void FXStep::SetRenderMark(int mark)
{
    m_RenderMark = mark;
}

void FXStep::SetRenderFrame(int frame)
{
    m_RenderFrame = frame;
}

void FXStep::SetRenderTarget(int index, RenderTarget * target)
{
    m_RenderTarget[index] = target;
}

FX_RENDER_MODE FXStep::GetRenderMode()
{
    return m_RenderMode;
}

int FXStep::GetRenderMark()
{
    return m_RenderMark;
}

int FXStep::GetRenderFrame()
{
    return m_RenderFrame;
}

RenderTarget * FXStep::GetRenderTarget(int index)
{
    return m_RenderTarget[index];
}

MaterialPtr FXStep::GetMaterial()
{
    return m_Material;
}

void FXStep::SetMaterial(MaterialPtr mtl)
{
    m_Material = mtl;
}

void FXStep::SetMaterial(const String & sName)
{
    MaterialPtr mtl = MaterialManager::Instance().Find(sName);
    if (mtl.IsNull())
    {
        mtl = MaterialManager::Instance().Load(sName, sName);
    }

    m_Material = mtl;
}

void FXStep::Initialize()
{
}

void FXStep::NotifyActive()
{
    m_bEnable = FALSE;
    m_CurFrame = 0;
}