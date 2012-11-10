#pragma once

#include "MWRenderer.h"

namespace Myway
{

class MW_ENTRY SkyPlane : public Renderer
{
public:
    SkyPlane(const String & sName, const String & sMaterialName, float width, float height, float depth);
    virtual ~SkyPlane();

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