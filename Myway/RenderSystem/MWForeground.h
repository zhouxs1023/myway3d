#pragma once

#include "MWRenderer.h"

namespace Myway
{

class MW_ENTRY Foreground : public Renderer
{
public:
    Foreground(const String & sName, const String & sMaterialName);
    ~Foreground();

    void                            SetVisible(bool visible);
    bool                            GetVisible();
    const String &                  GetName();

    void                            SetZOrder(int iZOrder);
    int                          GetZOrder() const;

protected:
    String                          m_sName;
    bool                            m_bVisible;
    int                          mZOrder;
};

}