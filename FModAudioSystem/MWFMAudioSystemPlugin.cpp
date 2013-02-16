#include "MWFMAudioSystemPlugin.h"
#include "MWFMAudioSystem.h"
#include "Engine.h"

namespace Myway
{

	FMAudioSystem * gAudioSystem = NULL;

	FMAudioSystemPlugin::FMAudioSystemPlugin()
	: Plugin("FMAudioSystem")
	{
	}

	FMAudioSystemPlugin::~FMAudioSystemPlugin()
	{
	}

	void FMAudioSystemPlugin::Install()
	{
		d_assert (gAudioSystem == NULL);

		const DeviceProperty * device = Engine::Instance()->GetDeviceProperty();

		gAudioSystem = new FMAudioSystem(device->hWnd);
		Engine::Instance()->SetAudioSystem(gAudioSystem);
	}

	void FMAudioSystemPlugin::Uninstall()
	{
		safe_delete (gAudioSystem);
	}

}