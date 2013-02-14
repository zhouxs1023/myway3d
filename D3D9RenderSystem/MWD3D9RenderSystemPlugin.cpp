#include "MWD3D9RenderSystemPlugin.h"
#include "MWD3D9RenderSystem.h"

namespace Myway
{
	D3D9RenderSystem * gD3D9RenderSystem = NULL;

    D3D9RenderSystemPlugin::D3D9RenderSystemPlugin()
        : Plugin("D3D9RenderSystem")
    {
    }

    D3D9RenderSystemPlugin::~D3D9RenderSystemPlugin()
    {
    }

    void D3D9RenderSystemPlugin::Install()
    {
		d_assert (gD3D9RenderSystem == NULL);

		gD3D9RenderSystem = new D3D9RenderSystem();

        gD3D9RenderSystem->Init();

        Engine::Instance()->SetRenderSystem(gD3D9RenderSystem);
    }

    void D3D9RenderSystemPlugin::Uninstall()
    {
		safe_delete (gD3D9RenderSystem);
    }
}
