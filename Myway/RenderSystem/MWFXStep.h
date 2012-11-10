#pragma once

#include "MWVideoBufferManager.h"
#include "MWMaterial.h"

namespace Myway
{

enum FX_RENDER_MODE
{
    FXRM_UNKNOWN,
    FXRM_DEFAULT,
    FXRM_RECT,

    MW_ALIGN_ENUM(FX_RENDER_MODE)
};

class MW_ENTRY FXStep : public AllocObj
{
public:
                    FXStep();
                    ~FXStep();

    void            UpdateFrame();

    void            SetRenderMode(FX_RENDER_MODE mode);
    void            SetRenderMark(int mark);
    void            SetRenderFrame(int frame);
    void            SetRenderTarget(int index, RenderTarget * target);

    FX_RENDER_MODE  GetRenderMode();
    int          GetRenderMark();
    int          GetRenderFrame();
    RenderTarget *  GetRenderTarget(int index);
    bool            GetEnable();

    MaterialPtr     GetMaterial();

    void            SetMaterial(MaterialPtr mtl);
    void            SetMaterial(const String & sName);

    friend class FXComposer;
    friend class FXComposerImport;
    friend class FXComposerExport;

protected:
    void            Initialize();
    void            NotifyActive();

protected:
    FX_RENDER_MODE  m_RenderMode;
    int          m_RenderMark;
    int          m_RenderFrame;

    bool            m_bEnable;
    int          m_CurFrame;

    RenderTarget *  m_RenderTarget[MAX_RENDER_TARGET];
    MaterialPtr     m_Material;
};

}