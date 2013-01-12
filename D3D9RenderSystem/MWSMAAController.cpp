#include "MWSMAAController.h"
#include "MWD3D9RenderSystem.h"
#include "Engine.h"

namespace Myway {

	SMAAController::SMAAController()
	{
		mSMAA = NULL;
		mType = eSmaaType::SMAA_NONE;
	}

	SMAAController::~SMAAController()
	{
		Shutdown();
	}

	void SMAAController::Init()
	{
		Shutdown();

		if (mType == eSmaaType::SMAA_NONE)
			return ;

		SMAA::Preset preset = SMAA::PRESET_HIGH;

		switch (mType)
		{
		case eSmaaType::SMAA_LOW:
			preset = SMAA::PRESET_LOW;
			break;

		case eSmaaType::SMAA_MEDIUM:
			preset = SMAA::PRESET_MEDIUM;
			break;

		case eSmaaType::SMAA_HIGH:
			preset = SMAA::PRESET_HIGH;
			break;

		case eSmaaType::SMAA_ULTRA:
			preset = SMAA::PRESET_ULTRA;
			break;
		}

		const DeviceProperty * dp = Engine::Instance()->GetDeviceProperty();
		IDirect3DDevice9 * device = ((D3D9RenderSystem *)RenderSystem::Instance())->GetD3DDevice();

		mSMAA = new SMAA(device, dp->Width, dp->Height, preset);
	}

	void SMAAController::Shutdown()
	{
		safe_delete(mSMAA);
	}

	void SMAAController::SetSMAAType(eSmaaType::enum_t type)
	{
		if (mType != type)
		{
			mType = type;
			Init();
		}
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

