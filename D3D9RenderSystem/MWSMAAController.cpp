#include "MWSMAAController.h"
#include "Engine.h"

namespace Myway {

	SMAAController::SMAAController()
	{
		mSMAA = NULL;
	}

	SMAAController::~SMAAController()
	{
		Shutdown();
	}

	void SMAAController::Init(IDirect3DDevice9 * device)
	{
		Shutdown();

		const DeviceProperty * dp = Engine::Instance()->GetDeviceProperty();

		if (dp->SmaaType == SMAA_NONE)
			return ;

		SMAA::Preset preset = SMAA::PRESET_HIGH;

		switch (dp->SmaaType)
		{
		case SMAA_LOW:
			preset = SMAA::PRESET_LOW;
			break;

		case SMAA_MEDIUM:
			preset = SMAA::PRESET_MEDIUM;
			break;

		case SMAA_HIGH:
			preset = SMAA::PRESET_HIGH;
			break;

		case SMAA_ULTRA:
			preset = SMAA::PRESET_ULTRA;
			break;
		}

		mSMAA = new SMAA(device, dp->Width, dp->Height, preset);
	}

	void SMAAController::Shutdown()
	{
		safe_delete(mSMAA);
	}

	void SMAAController::Do(RenderTarget * rt, Texture * colorTex)
	{
		if (!mSMAA)
			return ;

		D3D9Texture * d3dTex = (D3D9Texture *)colorTex;
		D3D9RenderTarget * d3dRt = (D3D9RenderTarget *)rt;

		mSMAA->go(d3dTex->GetD3DTexture(), d3dTex->GetD3DTexture(), d3dRt->GetD3DSurface(), SMAA::INPUT_COLOR);
	}
}

