#pragma once

#include "MWRenderer.h"

namespace Myway
{

class MW_ENTRY SkyBox : public Renderer
{
public:
    SkyBox(const String & sName, const String & sMaterialName, float width, float height, float depth);
    virtual ~SkyBox();

    const String &          GetName() const;
    float                   GetWidth() const;
    float                   GetHeight() const;
    float                   GetDepth() const;

    void                    SetEnable(bool bEnable);
    bool                    GetEnable() const;

protected:
    void                    _Initialize();

protected:
    String                  m_sName;
    float                   mWidth;
    float                   mHeight;
    float                   mDepth;
    bool                    m_bEnable;
};

}