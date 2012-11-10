#pragma once

#include "MWRenderer.h"

namespace Myway
{

class MW_ENTRY SkyDome : public Renderer
{
public:
    SkyDome(const String & sName, const String & sMaterialName, int iRings, int iSegments, float fRadius);
    virtual ~SkyDome();

    const String &          GetName() const;
    int                  GetRings() const;
    int                  GetSegments() const;
    float                   GetRadius() const;

    void                    SetEnable(bool bEnable);
    bool                    GetEnable() const;

protected:
    void                    _Initialize();

protected:
    String                  m_sName;
    int                  mRings;
    int                  mSegments;
    float                   mRadius;
    bool                    m_bEnable;
};

}