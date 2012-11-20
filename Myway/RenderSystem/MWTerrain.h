#pragma once

#include "MWRenderSystem.h"
#include "MWTerrainDef.h"

namespace Myway {

class TerrainLod;

class MW_ENTRY Terrain
{
    DECLARE_ALLOC();
	DECLARE_SINGLETON(Terrain);

    friend class TerrainSection;

public:
	enum Consts
	{
		kMaxLayers = 4,
		kSectionVertexSize = 65,
		kSectionVertexSize_2 =  kSectionVertexSize * kSectionVertexSize,
		kMaxDetailLevel = 4,
		kWeightMapSize = 256,
		kMaxPixelError = 4,
		kMaxResolution = 768,
	};

	struct Layer
	{
		TString128 detail;
		TString128 normal;
		TString128 specular;
		float scale;

		Layer() : scale(0) {}
	};

	struct Config {
		float xSize, zSize;
		int xVertexCount, zVertexCount;
		Layer layers[kMaxLayers];
		bool morphEnable;
		float morphStart;

		float xSectionSize, zSectionSize;
		int xSectionCount, zSectionCount;

		float fSectionWidth;
		float fSectionHeight;

		Config()
		{
			xSize = zSize = 0;
			xVertexCount = zVertexCount = 0;
			morphEnable = false;
			morphStart = 0;

			xSectionCount = zSectionCount = 0;
			xSectionSize = zSectionSize = 0;
		}
	};

public:
    Terrain();
    virtual ~Terrain();

	void				Create(const Config & config);

    const Config &		GetConfig() const { return mConfig; }
    TerrainLod *        GetTerrainLod() { return mLod; }

	TerrainSection *	GetSection(int x, int z);
	float				GetHeight(int x, int z);
	Vec3				GetNormal(int x, int z);

	const float *		GetHeights() const { return mHeights; }
	const Vec3 *		GetNormals() const { return mNormals; }

protected:
    void                OnPreVisibleCull(void * data);
	VertexBufferPtr		GetXYVertexBuffer() { return mXYStream; }

protected:
    Config mConfig;

    Array<SceneNode*> mSceneNodes;
    Array<TerrainSection*> mSections;
	Array<TerrainSection*> mVisibleSections;

    TerrainLod * mLod;
    Technique * mTech;
	VertexBufferPtr mXYStream;

	float *	mHeights;
	Vec3 *	mNormals;

	Array<TexturePtr> mDetailMaps;
	Array<TexturePtr> mNormalMaps;
	Array<TexturePtr> mSpecularMaps;

	tEventListener<Terrain> tOnPreVisibleCull;
};

}