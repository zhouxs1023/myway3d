#pragma once

#include "MWRenderer.h"
#include "MWMover.h"

namespace Myway
{

class MW_ENTRY ViewAlignedQuad : public Mover, public Renderer
{
public:
    ViewAlignedQuad(const String & sName, const String & sMaterialName);
    virtual ~ViewAlignedQuad();

    const String &                  GetName();
    virtual void                    AddRenderQueue(RenderQueue * rq);

protected:
    String                          m_sName;
};

}