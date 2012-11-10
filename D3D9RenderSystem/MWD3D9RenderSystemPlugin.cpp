#include "MWD3D9RenderSystemPlugin.h"
#include "MWD3D9RenderSystem.h"

namespace Myway
{
    D3D9RenderSystemPlugin::D3D9RenderSystemPlugin()
        : Plugin("D3D9RenderSystem")
    {
    }

    D3D9RenderSystemPlugin::~D3D9RenderSystemPlugin()
    {
    }

    void D3D9RenderSystemPlugin::Install()
    {
        D3D9RenderSystem * render = new D3D9RenderSystem();

        render->Init();

        Engine::Instance()->SetRenderSystem(render);
    }

    void D3D9RenderSystemPlugin::Uninstall()
    {
    }

}
