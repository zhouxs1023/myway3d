#include "MWRenderScheme.h"

namespace Myway
{

Event RenderScheme::OnBeginRender;

Event RenderScheme::OnAfterRenderSolid;

Event RenderScheme::OnAfterDeffererShading;

Event RenderScheme::OnAfterRender;


IMP_SLN(RenderScheme);
RenderScheme::RenderScheme()
{
    INIT_SLN;
}

RenderScheme::~RenderScheme()
{
    SHUT_SLN;
}


}