#include "MWTerrain.h"
#include "MWTerrainLod.h"
#include "MWTerrainSection.h"
#include "MWWorld.h"
#include "MWImage.h"
#include "MWResourceManager.h"
#include "MWEnvironment.h"
#include "MWRenderEvent.h"

namespace Myway {

IMP_SLN(Terrain);

Terrain::Terrain()
: tOnPreVisibleCull(RenderEvent::OnPreVisibleCull, this, &Terrain::OnPreVisibleCull)
{
	INIT_SLN;

    mLod = new TerrainLod(kMaxDetailLevel);
    mTech = Environment::Instance()->GetShaderLib()->GetTechnique("Terrain");
}

Terrain::~Terrain()
{
	SHUT_SLN;

    safe_delete (mLod);

    for (int i = 0; i < mSections.Size(); ++i)
    {
        delete mSections[i];
        World::Instance()->DestroySceneNode(mSceneNodes[i]);
    }

    mSceneNodes.Clear();
    mSections.Clear();
}

void Terrain::Create(const Config & config)
{
	mConfig = config;

	int x = config.xVertexCount - 1;
	int z = config.zVertexCount - 1;
	int k = kSectionVertexSize - 1;

	if ((x % k) != 0)
		mConfig.xVertexCount = (x / k + 1) * k + 1;

	if ((z % k) != 0)
		mConfig.zVertexCount = (z / k + 1) * k + 1;

	mConfig.zVertexCount = Math::FloorPowerOfTwo(config.zVertexCount) + 1;

	mConfig.xSectionCount = mConfig.xVertexCount / (kSectionVertexSize - 1);
	mConfig.zSectionCount = mConfig.zVertexCount / (kSectionVertexSize - 1);

	mConfig.xSectionSize = mConfig.xSize / mConfig.xSectionCount;
	mConfig.zSectionSize = mConfig.zSize / mConfig.zSectionCount;

	mSections.Resize(mConfig.xSectionCount * mConfig.zSectionCount);

	for (int i = 0; i < mConfig.xSectionCount * mConfig.zSectionCount; ++i)
		mSections[i] = NULL;

	mXYStream = VideoBufferManager::Instance()->CreateVertexBuffer(8 * kSectionVertexSize * kSectionVertexSize);
	
	float * vert = (float *)mXYStream->Lock(0, 0, LOCK_NORMAL);
	{
		float w = mConfig.xSize / mConfig.xSectionCount;
		float h = mConfig.zSize / mConfig.zSectionCount;

		for (int i = 0; i < kSectionVertexSize; ++i)
		{
			for (int j = 0; j < kSectionVertexSize; ++j)
			{
				*vert++ = i / (float)(kSectionVertexSize - 1) * w;
				*vert++ = (1 - j / (float)(kSectionVertexSize - 1)) * h;
			}
		}
	}
	mXYStream->Unlock();

	int index = 0;
	for (int i = 0; i < mConfig.xSectionCount; ++i)
	{
		for (int j = 0; j < mConfig.zSectionCount; ++j)
		{
			mSections[index] = new TerrainSection(this, i, j);
		}
	}
}

TerrainSection * Terrain::GetSection(int x, int z)
{
	d_assert (x < mConfig.xSectionCount && z < mConfig.zSectionCount);

	return mSections[z * mConfig.zSectionCount + x];
}

float Terrain::GetHeight(int x, int z)
{
	d_assert (x < mConfig.xVertexCount && z < mConfig.zVertexCount);

	return mHeights[z * mConfig.zVertexCount + x];
}

Vec3 Terrain::GetNormal(int x, int z)
{
	d_assert (x < mConfig.xVertexCount && z < mConfig.zVertexCount);

	return mNormals[z * mConfig.zVertexCount + x];
}

void Terrain::OnPreVisibleCull(void * data)
{
	mVisibleSections.Clear();
}


}