#include "MOpcodePlugin.h"
#include "MOpcodeScene.h"

namespace Myway {

	MOpcodeScene * gOpcodeScene = NULL;

	MOpcodePlugin::MOpcodePlugin()
		: Plugin("MOpcode")
	{
	}

	MOpcodePlugin::~MOpcodePlugin()
	{
	}

	void MOpcodePlugin::Install()
	{
		d_assert (gOpcodeScene == NULL);

		gOpcodeScene = new MOpcodeScene();

		Engine::Instance()->SetPhyWorld(gOpcodeScene);
	}

	void MOpcodePlugin::Uninstall()
	{
		safe_delete (gOpcodeScene);
	}

}