#include "MWDIInputSystemPlugin.h"
#include "MWDIInputSystem.h"
#include "Engine.h"

namespace Myway
{

DIInputSystemPlugin::DIInputSystemPlugin()
: Plugin("DI8InputSystem")
{
}

DIInputSystemPlugin::~DIInputSystemPlugin()
{
}

void DIInputSystemPlugin::Install()
{
    const DeviceProperty * device = Engine::Instance()->GetDeviceProperty();
    DIInputSystem * pInputSystem = new DIInputSystem(device->hInst, device->hWnd);
    Engine::Instance()->SetInputSystem(pInputSystem);
}

void DIInputSystemPlugin::Uninstall()
{
}

}
