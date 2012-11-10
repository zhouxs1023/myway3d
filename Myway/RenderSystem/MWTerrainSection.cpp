#include "MWTerrainSection.h"
#include "MWTerrain.h"

namespace Myway {

#define _POSITION    0
#define _NORMAL      1
#define _TEXCOORD    2
#define _MORPH       3


TerrainSection::TerrainSection(Terrain * pTerrain, int x, int y)
: Mover("TerrainSection")
, mTerrain(pTerrain)
, mSectionX(x)
, mSectionY(y)
, mMorph(0)
{
    mxVertex = mSectionX * (TnConst::kSectionVertexSize - 1);
    myVertex = mSectionY * (TnConst::kSectionVertexSize - 1);

    CalcuMorphBuffer();
    CalcuErrorMetrics();
    CalcuLevelDistance();

    Init();
}

TerrainSection::~TerrainSection()
{
}

void TerrainSection::Init()
{
    int xSectionVertSize = TnConst::kSectionVertexSize;
    int ySectionVertSize = TnConst::kSectionVertexSize;
    short xVertSize = mTerrain->GetXVertexSize();
    short yVertSize = mTerrain->GetYVertexSize();
    int xtile = xSectionVertSize - 1;
    int ytile = ySectionVertSize - 1;

    const Vec3 * pPositions = mTerrain->GetPositions();
    const Vec3 * pNormals = mTerrain->GetNormals();
    const Vec2 * pTexcoords = mTerrain->GetTexcoords();

    const Vec3 * pOffset = pPositions + mSectionY * ytile * xVertSize + mSectionX * xtile;

   //create vertex declaration
    VertexDeclarationPtr pVertexDecl = VideoBufferManager::Instance()->CreateVertexDeclaration();
    pVertexDecl->AddElement(_POSITION, 0,  DT_FLOAT3, DU_POSITION, 0);
    pVertexDecl->AddElement(_NORMAL, 0,  DT_FLOAT3, DU_NORMAL, 0);
    pVertexDecl->AddElement(_TEXCOORD, 0, DT_FLOAT2, DU_TEXCOORD, 0);
    pVertexDecl->AddElement(_MORPH, 0, DT_FLOAT1, DU_BLENDWEIGHT, 0);
    pVertexDecl->Init();

    //create vertex buffer
    VideoBufferManager & mgr = *VideoBufferManager::Instance();

    int iVertexCount = xSectionVertSize * ySectionVertSize;

    int iStride0 = sizeof(Vec3);
    VertexBufferPtr pVertexBuffer0 = mgr.CreateVertexBuffer(iVertexCount * iStride0);

    int iStride1 = sizeof(Vec3);
    VertexBufferPtr pVertexBuffer1 = mgr.CreateVertexBuffer(iVertexCount * iStride1);

    int iStride2 = sizeof(Vec2);
    VertexBufferPtr pVertexBuffer2 = mgr.CreateVertexBuffer(iVertexCount * iStride2);

    int iSrcOffset = mSectionY * ytile * xVertSize + mSectionX * xtile;

    Vec3 * positions = (Vec3 *)pVertexBuffer0->Lock(0, 0, LOCK_DISCARD);
    Vec3 * normals = (Vec3 *)pVertexBuffer1->Lock(0, 0, LOCK_DISCARD);
    Vec2 * texcoords = (Vec2 *)pVertexBuffer2->Lock(0, 0, LOCK_DISCARD);

    {
        pPositions += iSrcOffset;
        pNormals += iSrcOffset;
        pTexcoords += iSrcOffset;
        for (int i = 0; i < ySectionVertSize; ++i)
        {
            Memcpy(positions, pPositions, xSectionVertSize * sizeof(Vec3));
            Memcpy(normals, pNormals, xSectionVertSize * sizeof(Vec3));
            Memcpy(texcoords, pTexcoords, xSectionVertSize * sizeof(Vec2));

            positions += xSectionVertSize;
            normals += xSectionVertSize;
            texcoords += xSectionVertSize;

            pPositions += xVertSize;
            pNormals += xVertSize;
            pTexcoords += xVertSize;
        }
    }

    pVertexBuffer0->Unlock();
    pVertexBuffer1->Unlock();
    pVertexBuffer2->Unlock();

	mLevel = 0;
    mkKey.level = 0;
    mkKey.north = 0;
    mkKey.south = 0;
    mkKey.west = 0;
    mkKey.east = 0;

    mRender.vxStream.SetDeclaration(pVertexDecl);
    mRender.vxStream.SetCount(iVertexCount);
    mRender.vxStream.Bind(0, pVertexBuffer0, iStride0);
    mRender.vxStream.Bind(1, pVertexBuffer1, iStride1);
    mRender.vxStream.Bind(2, pVertexBuffer2, iStride2);

    mRender.iPrimCount = 0;
    mRender.ePrimType = PRIM_TRIANGLESTRIP;

    //calculate bounds
    pPositions = mTerrain->GetPositions() + iSrcOffset;

    mAabbLocal = Aabb::Invalid;

    for (int i = 0; i < ySectionVertSize; ++i)
    {
        for (int j = 0; j < xSectionVertSize; ++j)
        {
            Math::VecMinimum(mAabbLocal.minimum, mAabbLocal.minimum, pPositions[j]);
            Math::VecMaximum(mAabbLocal.maximum, mAabbLocal.maximum, pPositions[j]);
        }

        pPositions += xVertSize;
    }

    mSphLocal.center = mAabbLocal.GetCenter();
    mSphLocal.radius = Math::VecDistance(mSphLocal.center, mAabbLocal.maximum);

    mAabbWorld = mAabbLocal;
    mSphWorld = mSphLocal;
}


void TerrainSection::UpdateLod()
{
    Camera * cam = World::Instance()->MainCamera();

    mLevel = 0;

    const Aabb & aabb = GetWorldAabb();
    float d = Math::VecDistanceSq(aabb.minimum, cam->GetPosition());
    d = Math::Minimum(d, Math::VecDistanceSq(aabb.minimum, cam->GetPosition()));

    bool finished = false;
    int max_level = TnConst::kMaxDetailLevel - 1;

    while (!finished && mLevel < max_level)
    {
        float d0 = mLevelDistSq[mLevel];
        float d1 = mLevelDistSq[mLevel + 1];

        if (d > d1)
        {
            ++mLevel;
        }
        else
        {
            finished = true;
        }
    }

    if (mTerrain->IsMorphEnable())
    {
        float start = mTerrain->GetMorphStart();

        if (mLevel == max_level || d < start)
        {
            mMorph = 0.0f;
        }
        else
        {
            float d0 = mLevelDistSq[mLevel];
            float d1 = mLevelDistSq[mLevel + 1];
            mMorph = (d - d0) / (d1 - d0);
        }
    }
}

void TerrainSection::PreRender()
{
    TerrainLod::_Key key;
    key.level = mLevel;
    key.north = mLevel;
    key.south = mLevel;
    key.west = mLevel;
    key.east = mLevel;

    int x = mSectionX;
    int y = mSectionY;

    if (y > 0)
    {
        TerrainSection * section = mTerrain->GetSection(x, y - 1);
        key.north = section->GetLevel();

        if (key.north < mLevel)
            key.north = mLevel;
    }

    if (y < mTerrain->GetYSectionSize() - 1)
    {
        TerrainSection * section = mTerrain->GetSection(x, y + 1);
        key.south = section->GetLevel();

        if (key.south < mLevel)
            key.south = mLevel;
    }

    if (x > 0)
    {
        TerrainSection * section = (TerrainSection*)mTerrain->GetSection(x - 1, y);
        key.west = section->GetLevel();

        if (key.west < mLevel)
            key.west = mLevel;
    }

    if (x < mTerrain->GetXSectionSize() - 1)
    {
        TerrainSection * section = (TerrainSection*)mTerrain->GetSection(x + 1, y);
        key.east = section->GetLevel();

        if (key.east < mLevel)
            key.east = mLevel;
    }

    if (key != mkKey || mRender.iPrimCount == 0)
    {
        const TerrainLod::_IndexData & data = mTerrain->GetTerrainLod()->GetIndexData(key);

        mRender.vxStream.Bind(_MORPH, mMorphBuffer[key.level], sizeof(float));

        mRender.ixStream.Bind(data.index_buffer, data.start);
        mRender.ixStream.SetCount(data.index_count);
        mRender.iPrimCount = data.prim_count;
        mRender.ePrimType = data.prim_type;

        mkKey = key;
    }
}

void TerrainSection::AddRenderQueue(RenderQueue * rq)
{
    mTerrain->mVisibleSections.PushBack(this);
}

void TerrainSection::CalcuMorphBuffer()
{
    if (mTerrain->IsMorphEnable())
    {
        int size = TnConst::kSectionVertexSize * TnConst::kSectionVertexSize * sizeof(float);
        for (int i = 0; i < TnConst::kMaxDetailLevel - 1; ++i)
        {
            mMorphBuffer[i] = VideoBufferManager::Instance()->CreateVertexBuffer(size);
            _CalcuMorphBuffer(i);
        }
    }
}

void TerrainSection::_CalcuMorphBuffer(int level)
{
    VertexBufferPtr buffer = mMorphBuffer[level++];

    int step = 1 << level;

    int xVertSize = mTerrain->GetXVertexSize();
    int yVertSize = mTerrain->GetYVertexSize();
    int xSectionVertSize = TnConst::kSectionVertexSize;
    int ySectionVertSize = TnConst::kSectionVertexSize;

    int xOffset = mSectionX * (xSectionVertSize - 1);
    int yOffset = mSectionY * (ySectionVertSize - 1);

    int xSides = (xSectionVertSize - 1) >> level;
    int ySides = (ySectionVertSize - 1) >> level;

    float err = 0.0f;

    float * data = (float *)buffer->Lock(0, 0, LOCK_NORMAL);

    Memzero(data, buffer->GetSize());

    // 求水平方向的误差值
    for (int y = step; y < ySectionVertSize - step; y += step)
    {
        for (int x = 0; x < xSides; ++x)
        {
            int x0 = x * step;
            int x1 = x0 + step;

            int xm = (x1 + x0) / 2;

            float h0 = mTerrain->GetHeight(x0 + xOffset, y + yOffset);
            float h1 = mTerrain->GetHeight(x1 + xOffset, y + yOffset);
            float hm = mTerrain->GetHeight(xm + xOffset, y + yOffset);
            float hmi = (h0 + h1) / 2;

            float delta = hmi - hm;

            data[y * xSectionVertSize + xm] = delta;
        }
    }

    //垂直方向
    for (int x = step; x < xSectionVertSize - step; x += step)
    {
        for (int y = 0; y < ySides; ++y)
        {
            int y0 = y * step;
            int y1 = y0 + step;

            int ym = (y0 + y1) / 2;

            float h0 = mTerrain->GetHeight(x + xOffset, y0 + yOffset);
            float h1 = mTerrain->GetHeight(x + xOffset, y1 + yOffset);
            float hm = mTerrain->GetHeight(x + xOffset, ym + yOffset);
            float hmi = (h0 + h1) / 2;

            float delta = hmi - hm;

            data[ym * xSectionVertSize + x] = delta;
        }
    }

    //中点
    for (int y = 0; y < ySides; ++y)
    {
        int y0 = y * step;
        int y1 = y0 + step;
        int ym = (y0 + y1) / 2;

        for (int x = 0; x < xSides; ++x)
        {
            int x0 = x * step;
            int x1 = x0 + step;
            int xm = (x0 + x1) / 2;

            float h0 = mTerrain->GetHeight(x0 + xOffset, y0 + yOffset);
            float h1 = mTerrain->GetHeight(x1 + xOffset, y1 + yOffset);
            float hm = mTerrain->GetHeight(xm + xOffset, ym + yOffset);
            float hmi = (h0 + h1) / 2;

            float delta = hmi - hm;

            data[ym * xSectionVertSize + xm] = delta;
        }
    }

    buffer->Unlock();
}


void TerrainSection::CalcuErrorMetrics()
{
    mErrorMetric[0] = 0;

    for (int i = 1; i < TnConst::kMaxDetailLevel; ++i)
    {
        mErrorMetric[i] = _CalcuErrorMetric(i);
    }

    //lod大的必须误差值必须大
    for (int i = 2; i < TnConst::kMaxDetailLevel; ++i)
    {
        mErrorMetric[i] = Math::Maximum(mErrorMetric[i], mErrorMetric[i - 1]);
    }
}

float TerrainSection::_CalcuErrorMetric(int level)
{
    ///////////////////////////////////////////////////////////////////////////
    // 求每个级别的最大误差值
    // 对于该栅格求误差值, 取最大值.
    // 
    //
    ///////////////////////////////////////////////////////////////////////////

    int step = 1 << level;

    int xVertSize = mTerrain->GetXVertexSize();
    int yVertSize = mTerrain->GetYVertexSize();
    int xSectionVertSize = TnConst::kSectionVertexSize;
    int ySectionVertSize = TnConst::kSectionVertexSize;

    int xOffset = mSectionX * (xSectionVertSize - 1);
    int yOffset = mSectionY * (ySectionVertSize - 1);

    int xSides = (xSectionVertSize - 1) >> level;
    int ySides = (ySectionVertSize - 1) >> level;

    float err = 0.0f;

    // 求水平方向的误差值
    for (int y = 0; y < ySectionVertSize; y += step)
    {
        for (int x = 0; x < xSides; ++x)
        {
            int x0 = x * step;
            int x1 = x0 + step;

            int xm = (x1 + x0) / 2;

            float h0 = mTerrain->GetHeight(x0 + xOffset, y + yOffset);
            float h1 = mTerrain->GetHeight(x1 + xOffset, y + yOffset);
            float hm = mTerrain->GetHeight(xm + xOffset, y + yOffset);
            float hmi = (h0 + h1) / 2;

            float delta = Math::Abs(hm - hmi);

            err = Math::Maximum(err, delta);
        }
    }

    //垂直方向
    for (int x = 0; x < xSectionVertSize; x += step)
    {
        for (int y = 0; y < ySides; ++y)
        {
            int y0 = y * step;
            int y1 = y0 + step;

            int ym = (y0 + y1) / 2;

            float h0 = mTerrain->GetHeight(x + xOffset, y0 + yOffset);
            float h1 = mTerrain->GetHeight(x + xOffset, y1 + yOffset);
            float hm = mTerrain->GetHeight(x + xOffset, ym + yOffset);
            float hmi = (h0 + h1) / 2;

            float delta = Math::Abs(hm - hmi);

            err = Math::Maximum(err, delta);
        }
    }

    //中点
    for (int y = 0; y < ySides; ++y)
    {
        int y0 = y * step;
        int y1 = y0 + step;
        int ym = (y0 + y1) / 2;

        for (int x = 0; x < xSides; ++x)
        {
            int x0 = x * step;
            int x1 = x0 + step;
            int xm = (x0 + x1) / 2;

            float h0 = mTerrain->GetHeight(x0 + xOffset, y0 + yOffset);
            float h1 = mTerrain->GetHeight(x1 + xOffset, y1 + yOffset);
            float hm = mTerrain->GetHeight(xm + xOffset, ym + yOffset);
            float hmi = (h0 + h1) / 2;

            float delta = Math::Abs(hm - hmi);

            err = Math::Maximum(err, delta);
        }
    }

    return err;
}

void TerrainSection::CalcuLevelDistance()
{
    float pixelerr = TnConst::kMaxPixelError;
    float resolution = TnConst::kMaxResolution;

    float c = 1.0f / (2 * pixelerr / resolution);

    for (int i = 0; i < TnConst::kMaxDetailLevel; ++i)
    {
        float e = mErrorMetric[i];
        float d = e * c;
        mLevelDistSq[i] = d * d;
    }

    if (mTerrain->IsMorphEnable())
    {
        for (int i = 1; i < TnConst::kMaxDetailLevel - 1; ++i)
        {
            float d0 = mLevelDistSq[i - 1];
            float d1 = mLevelDistSq[i];
            float d2 = mLevelDistSq[i + 1];

            if (d2 - d1 < Math::EPSILON_E4)
            {
                d1 =  d0 + 0.5f * (d2 - d0);
                mLevelDistSq[i] = d1;
            }
        }
    }
}



}