#include "MWFMAudioSystemPlugin.h"
#include "MWFMAudioSystem.h"
#include "Engine.h"

namespace Myway
{

FMAudioSystemPlugin::FMAudioSystemPlugin()
: Plugin("FMAudioSystem")
{
}

FMAudioSystemPlugin::~FMAudioSystemPlugin()
{
}

void FMAudioSystemPlugin::Install()
{
    const DeviceProperty * device = Engine::Instance()->GetDeviceProperty();

    FMAudioSystem * pAudioSystem = new FMAudioSystem(device->hWnd);
    Engine::Instance()->SetAudioSystem(pAudioSystem);
}

void FMAudioSystemPlugin::Uninstall()
{
}

}