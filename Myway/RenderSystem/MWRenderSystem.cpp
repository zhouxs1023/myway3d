#include "MWRenderSystem.h"

namespace Myway
{
    IMP_SLN(RenderSystem);

    RenderSystem::RenderSystem()
    {
        INIT_SLN;
		
		mFlipCullMode = false;
    }

    RenderSystem::~RenderSystem()
    {
        SHUT_SLN;
    }
}