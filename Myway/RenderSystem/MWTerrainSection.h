#pragma once

#include "MWTerrainLod.h"

namespace Myway {


class MW_ENTRY TerrainSection : public Mover
{
    DECLARE_ALLOC();

    friend class Terrain;

public:
    TerrainSection(Terrain * pTerrain, int x, int y);
    ~TerrainSection();

    virtual void AddRenderQueue(RenderQueue * rq);

    int GetLevel() const { return mLevel; }
    int GetSectionX() const { return mSectionX; }
    int GetSectionY() const { return mSectionY; }

    void UpdateLod();
    void PreRender();

protected:
    void                    Init();

    void                    CalcuMorphBuffer();
    void                    _CalcuMorphBuffer(int level);
    void                    CalcuErrorMetrics();
    float                   _CalcuErrorMetric(int level);

    void                    CalcuLevelDistance();

protected:
    Terrain *               mTerrain;
    int                     mSectionX;
    int                     mSectionY;
    int                     mxVertex;
    int                     myVertex;

    int                     mLevel;
    float                   mErrorMetric[TnConst::kMaxDetailLevel];
    float                   mLevelDistSq[TnConst::kMaxDetailLevel];
    VertexBufferPtr         mMorphBuffer[TnConst::kMaxDetailLevel];
    TerrainLod::_Key        mkKey;
    float                   mMorph;

    RenderDesc              mRender;
};

}