#pragma once

#include "MWRenderSystem.h"
#include "MWTerrainDef.h"

namespace Myway {

class TerrainLod;

class MW_ENTRY Terrain : public EventListener
{
    DECLARE_ALLOC();

    friend class TerrainSection;

public:
    Terrain(const TString128 & source);
    virtual ~Terrain();

    const Vec3 &        GetPosition(int x, int y) const;
    const Vec3 &        GetNormal(int x, int y) const;
    const Vec2 &        GetTexcoord(int x, int y) const;

    const Vec3 *        GetPositions() const;
    const Vec3 *        GetNormals() const;
    const Vec2 *        GetTexcoords() const;

    TerrainSection *    GetSection(int x, int y);

    float               GetHeight(int x, int y);
    float               GetHeight(int index);
    float               GetHeight(float x, float z);

    Vec3                GetPosition(const Ray & ray);

    const Aabb &        GetBound() const;

    const TnConfig &    GetConfig() const { return mConfig; }

    int                 GetXVertexSize() const { return mxVertSize; }
    int                 GetYVertexSize() const { return myVertSize; }
    int                 GetVertexSize() const { return mVertSize; }
    int                 GetXSectionSize() const { return mxSectionSize; }
    int                 GetYSectionSize() const { return mySectionSize; }

    bool                IsMorphEnable() const { return mConfig.morphEnable; }
    float               GetMorphStart() const { return mConfig.morphStart; }

    TerrainLod *        GetTerrainLod() { return mLod; }

    void                Render();
    void                RenderInMirror();
    void                OnCall(Event * sender, void * data);

protected:
    void                Init();
    void                AllocSection();
    void                LoadConfig(const TString128 & source);
    float *             LoadHightmap();

    void                CalcuPositions(const float * pHeights, float xStart, float yStart, float xEnd, float yEnd);
    void                CalcuNormals();
    void                CalcuTexcoords(float startu, float startv, float endu, float endv);
    void                CalcuBounds();

protected:
    TnConfig mConfig;

    int mxVertSize;
    int myVertSize;
    int mVertSize;

    Vec3 * mPositions;
    Vec3 * mNormals;
    Vec2 * mTexcoords;

    int mxSectionSize;
    int mySectionSize;

    Aabb mBound;

    Array<SceneNode*> mSceneNodes;
    Array<TerrainSection*> mSections;
    Array<TerrainSection*> mVisibleSections;

    TerrainLod * mLod;

    Technique * mTech;
    TexturePtr mWeightmap[2];
    TexturePtr mLayer[TnConst::kMaxLayers];
    float mUVScale[TnConst::kMaxLayers];
};

}