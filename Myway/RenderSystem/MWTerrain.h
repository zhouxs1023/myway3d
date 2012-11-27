#pragma once

#include "MWRenderSystem.h"

namespace Myway {

class TerrainLod;

class MW_ENTRY Terrain
{
    DECLARE_ALLOC();

    friend class TerrainSection;

public:
	enum Consts
	{
		kMaxLayers = 128,
		kMaxBlendLayers = 4,
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
		char material;

		Layer() : scale(100), material(-1) {}
	};

	struct Config {
		float xSize, zSize;

		int xVertexCount, zVertexCount;
		int iVertexCount;

		bool morphEnable;
		float morphStart;

		// auto calculate
		float xSectionSize, zSectionSize;
		int xSectionCount, zSectionCount;
		int iSectionCount;

		int xWeightMapSize;
		int zWeightMapSize;

		Config()
		{
			xSize = zSize = 1024;

			xVertexCount = zVertexCount = 1025;

			iVertexCount = 0;

			morphEnable = false;
			morphStart = 0;

			xSectionCount = zSectionCount = 0;
			xSectionSize = zSectionSize = 0;
			iSectionCount = 0;

			xWeightMapSize = 0;
			zWeightMapSize = 0;
		}
	};

public:
    Terrain(const Config & config);
	Terrain(const char * source);
    virtual ~Terrain();

	int					AddLayer(const Layer & layer);
	void				RemoveLayer(int layer);

    const Config &		GetConfig() const { return mConfig; }
    TerrainLod *        GetTerrainLod() { return mLod; }

	TerrainSection *	GetSection(int x, int z);
	Vec3				GetPosition(int x, int z);
	float				GetHeight(int x, int z);
	Vec3				GetNormal(int x, int z);
	Color				GetWeight(int x, int z);
	TexturePtr			GetWeightMap(int x, int z);

	const float *		GetHeights() const { return mHeights; }
	const Vec3 *		GetNormals() const { return mNormals; }

	void				Render();
	void				RenderInMirror();

	int					GetVisibleSectionCount() const { return mVisibleSections.Size(); }
	TerrainSection *	GetVisibleSection(int i) { return mVisibleSections[i]; }

	float				GetHeight(float x, float y);
	Vec3				GetPosition(const Ray & ray);
	// for editor
	float *				LockHeight(const Rect & rc);
	void				UnlockHeight();
	bool				IsLockedHeight() { return mLockedData != NULL; }
	const Rect &		GetLockedHeightRect() { return mLockedRect; }
	const float *		GetLockedHeightData() { return mLockedData; }

	Color *				LockWeightMap(const Rect & rc);
	void				UnlockWeightMap();
	bool				IsLockedWeightMap() { return mLockedWeightMapData != NULL; }
	const Rect &		GetLockedWeightMapRect() { return mLockedWeightMapRect; }
	const Color *		GetLockedWeightmapData() { return mLockedWeightMapData; }

protected:
    void                OnPreVisibleCull(void * data);
	VertexBufferPtr		GetXYVertexBuffer() { return mXYStream; }

	void				Create(const Config & config);
	void				Load(const char * source);

	TexturePtr			_getDetailMap(int layer);
	TexturePtr			_getNormalMap(int layer);
	TexturePtr			_getSpecularMap(int layer);
	Vec3				_getPosition(int x, int z);

protected:
    Config mConfig;

    Array<SceneNode*> mSceneNodes;
    Array<TerrainSection*> mSections;
	Array<TerrainSection*> mVisibleSections;
	Array<TexturePtr> mWeightMaps;

    TerrainLod * mLod;
    Technique * mTech;
	VertexBufferPtr mXYStream;

	float *	mHeights;
	Vec3 *	mNormals;
	Color * mWeights;

	Aabb mBound;

	TexturePtr mDefaultDetailMap;
	TexturePtr mDefaultNormalMap;
	TexturePtr mDefaultSpecularMap;

	Layer mLayer[kMaxLayers];
	TexturePtr mDetailMaps[kMaxLayers];
	TexturePtr mNormalMaps[kMaxLayers];
	TexturePtr mSpecularMaps[kMaxLayers];

	tEventListener<Terrain> tOnPreVisibleCull;

	// for editor
	Rect mLockedRect;
	float * mLockedData;

	Rect mLockedWeightMapRect;
	Color * mLockedWeightMapData;
};

}