#include "stdafx.h"
#include "Game.h"

GmRoot * GmRoot::msInstance = NULL;

GmRoot::GmRoot()
	: mCurrentMode(NULL)
	, mShaderLib(NULL)
{
	msInstance = this;
}

GmRoot::~GmRoot()
{
	msInstance = NULL;
}

void GmRoot::Init()
{
	mShaderLib = ShaderLibManager::Instance()->LoadShaderLib("ShaderLib", "Game.ShaderLib");
	d_assert (mShaderLib);

	mUIUtil.Init();

	mDataManager.Init();

	mCurrentMode = NULL;
}

void GmRoot::Shutdown()
{
	mDataManager.Shutdown();

	mUIUtil.Shutdown();

	if (mCurrentMode)
	{
		mCurrentMode->Shutdown();
		delete mCurrentMode;
	}
}

void GmRoot::Update()
{
	if (mCurrentMode)
		mCurrentMode->Update(Engine::Instance()->GetFrameTime());
}

void GmRoot::SetMode(GmMode * mode)
{
	d_assert (mode != NULL);

	if (mCurrentMode != NULL)
		mCurrentMode->Shutdown();

	safe_delete (mCurrentMode);

	mCurrentMode = mode;

	mCurrentMode->Init();
}

GmMode * GmRoot::GetMode()
{
	return mCurrentMode;
}

