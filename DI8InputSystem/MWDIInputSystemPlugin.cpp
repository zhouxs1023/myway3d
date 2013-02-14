#include "MWDIInputSystemPlugin.h"
#include "MWDIInputSystem.h"
#include "Engine.h"

namespace Myway
{

	DIInputSystem * gInputSystem = NULL;

	DIInputSystemPlugin::DIInputSystemPlugin()
		: Plugin("DI8InputSystem")
	{
	}

	DIInputSystemPlugin::~DIInputSystemPlugin()
	{
	}

	void DIInputSystemPlugin::Install()
	{
		d_assert (gInputSystem == NULL);

		const DeviceProperty * device = Engine::Instance()->GetDeviceProperty();

		gInputSystem = new DIInputSystem(device->hInst, device->hWnd);

		Engine::Instance()->SetInputSystem(gInputSystem);
	}

	void DIInputSystemPlugin::Uninstall()
	{
		safe_delete (gInputSystem);
	}

}
