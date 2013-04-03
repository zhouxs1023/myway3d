#include "MWWaterManager.h"
#include "MWEnvironment.h"
#include "MWUnderWater.h"
#include "Engine.h"

namespace Myway {

	IMP_SLN(WaterManager);

	WaterManager::WaterManager()
	{
		INIT_SLN;

		mOcean = NULL;

		mFog = new UnderWaterFog();
		mNoise = new UnderWaterNoise();
		mGodRay = new UnderWaterGodRay();
		mCaustics = new UnderWaterCaustics();
		mBubble = new UnderWaterBubble();

		mWater = new Water();
	}

	WaterManager::~WaterManager()
	{
		SHUT_SLN;

		safe_delete(mOcean);

		delete mFog;
		delete mNoise;
		delete mGodRay;
		delete mCaustics;
		delete mBubble;
		delete mWater;
	}

	bool WaterManager::IsUnderWater()
	{
		Camera * cam = World::Instance()->MainCamera();

		if (mOcean)
		{
			Vec3 pos = mOcean->GetPosition();

			if (pos.y > cam->GetPosition().y)
				return true;
		}

		if (mWater)
		{
			const Vec3 & position = World::Instance()->MainCamera()->GetPosition();

			Terrain * terrain = Environment::Instance()->GetTerrain();

			if (!terrain)
				return false;

			float gridSize = mWater->GetGridSize();

			int x = int((position.x / gridSize));
			int z = int((position.z / gridSize));

			if (x >= 0 && x < mWater->GetSizeX() &&
				z >= 0 && z < mWater->GetSizeZ() &&
				mWater->GetData(x, z) == 1)
			{
				if (position.y < mWater->GetHeight())
					return true;
			}
		}

		return false;
	}

	void WaterManager::Render(Texture * depthTex, Texture * colorTex)
	{
		if (mWater)
			mWater->Render(depthTex, colorTex);

		if (mOcean)
			mOcean->Render(depthTex, colorTex);
	}

	void WaterManager::RenderUnderFog(Texture * depthTex, Texture * colorTex)
	{
		if (IsUnderWater())
			mFog->Render(depthTex, colorTex);
	}

	void WaterManager::RenderUnderNoise(Texture * colorTex)
	{
		if (IsUnderWater())
			mNoise->Render(colorTex);
	}

	void WaterManager::RenderUnderGodRay()
	{
		if (IsUnderWater())
			mGodRay->Render(mOcean->GetPosition());
	}

	void WaterManager::RenderUnderCaustics(Texture * depthTex, Texture * colorTex)
	{
		if (IsUnderWater())
			mCaustics->Render(colorTex, depthTex);
	}

	void WaterManager::RenderUnderBubble()
	{
		if (IsUnderWater())
			mBubble->Render();
	}

	Ocean * WaterManager::CreateOcean()
	{
		d_assert (mOcean == NULL);

		mOcean = new Ocean();

		return mOcean;
	}

	void WaterManager::Resize(int w, int h)
	{
		mWater->Resize(w, h);

		if (mOcean)
			mOcean->Resize(w, h);
	}

}