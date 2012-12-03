#include "MWTerrainSection.h"
#include "MWTerrain.h"

namespace Myway {

#define _POSITION    0
#define _HEIGHT		 1
#define _NORMAL      2
#define _MORPH       3


TerrainSection::TerrainSection(Terrain * pTerrain, int x, int z)
: Mover("TerrainSection")
, mTerrain(pTerrain)
, mSectionX(x)
, mSectionZ(z)
, mMorph(0)
{
	mOffX = mTerrain->GetConfig().xSectionSize * x;
	mOffZ = mTerrain->GetConfig().zSize - mTerrain->GetConfig().zSectionSize * (z + 1);

    Init();

	mLayer[0] = +0;
	mLayer[1] = -1;
	mLayer[2] = -1;
	mLayer[3] = -1;
}

TerrainSection::~TerrainSection()
{
}

void TerrainSection::Init()
{
	CalcuMorphBuffer();
	CalcuErrorMetrics();
	CalcuLevelDistance();

    int xSectionVertSize = Terrain::kSectionVertexSize;
    int zSectionVertSize = Terrain::kSectionVertexSize;
    int xVertSize = mTerrain->GetConfig().xVertexCount;
    int xtile = xSectionVertSize - 1;
    int ztile = zSectionVertSize - 1;

	const float * pHeights = mTerrain->GetHeights();
    const Color * pNormals = mTerrain->GetNormals();

   //create vertex declaration
    VertexDeclarationPtr pVertexDecl = VideoBufferManager::Instance()->CreateVertexDeclaration();
	pVertexDecl->AddElement(_POSITION, 0,  DT_FLOAT2, DU_POSITION, 0);
    pVertexDecl->AddElement(_HEIGHT, 0,  DT_FLOAT1, DU_TEXCOORD, 0);
    pVertexDecl->AddElement(_NORMAL, 0,  DT_COLOR, DU_NORMAL, 0);
    pVertexDecl->AddElement(_MORPH, 0, DT_FLOAT1, DU_BLENDWEIGHT, 0);
    pVertexDecl->Init();

    //create vertex buffer
    VideoBufferManager & mgr = *VideoBufferManager::Instance();

    int iVertexCount = xSectionVertSize * zSectionVertSize;

    int iStride0 = sizeof(Vec2);
    VertexBufferPtr pVertexBuffer0 = mTerrain->GetXYVertexBuffer();

    int iStride1 = sizeof(float);
    VertexBufferPtr pVertexBuffer1 = mgr.CreateVertexBuffer(iVertexCount * iStride1);

    int iStride2 = sizeof(Color);
    VertexBufferPtr pVertexBuffer2 = mgr.CreateVertexBuffer(iVertexCount * iStride2);

    int iSrcOffset = mSectionZ * ztile * xVertSize + mSectionX * xtile;

    float * heights = (float *)pVertexBuffer1->Lock(0, 0, LOCK_DISCARD);
    int * normals = (int *)pVertexBuffer2->Lock(0, 0, LOCK_DISCARD);
    {
        pHeights += iSrcOffset;
        pNormals += iSrcOffset;

        for (int i = 0; i < zSectionVertSize; ++i)
        {
			for (int j = 0; j < xSectionVertSize; ++j)
			{
				heights[j] = pHeights[j];
				normals[j] = RGBA(pNormals[j].r, pNormals[j].g, pNormals[j].b, 255);
			}

            heights += xSectionVertSize;
            normals += xSectionVertSize;

            pHeights += xVertSize;
            pNormals += xVertSize;
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
    pHeights = mTerrain->GetHeights() + iSrcOffset;

    mAabbLocal = Aabb::Invalid;

	mAabbLocal.minimum.x = 0 + mOffX;
	mAabbLocal.minimum.z = 0 + mOffZ;

	mAabbLocal.maximum.x = mTerrain->GetConfig().xSectionSize + mOffX;
	mAabbLocal.maximum.z = mTerrain->GetConfig().zSectionSize + mOffZ;

    for (int j = 0; j < zSectionVertSize; ++j)
    {
        for (int i = 0; i < xSectionVertSize; ++i)
        {
            mAabbLocal.minimum.y = Math::Minimum(mAabbLocal.minimum.y, pHeights[i]);
            mAabbLocal.maximum.y = Math::Maximum(mAabbLocal.maximum.y, pHeights[i]);
        }

        pHeights += xVertSize;
    }

    mSphLocal.center = mAabbLocal.GetCenter();
    mSphLocal.radius = Math::VecDistance(mSphLocal.center, mAabbLocal.maximum);

    mAabbWorld = mAabbLocal;
    mSphWorld = mSphLocal;

	if (mNode)
		mNode->_NotifyUpdate();
}

void TerrainSection::Shutdown()
{
	for (int i = 0; i < Terrain::kMaxDetailLevel; ++i)
	{
		mMorphBuffer[i] = NULL;
	}

	mRender.vxStream.Bind(_POSITION, 0, 0);
	mRender.vxStream.Bind(_HEIGHT, 0, 0);
	mRender.vxStream.Bind(_NORMAL, 0, 0);
	mRender.vxStream.Bind(_MORPH, 0, 0);
	mRender.vxStream.SetCount(0);

	mRender.ixStream.Bind(NULL, 0);
	mRender.ixStream.SetCount(0);
}

void TerrainSection::UpdateLod()
{
    Camera * cam = World::Instance()->MainCamera();

    mLevel = 0;

    const Aabb & aabb = GetWorldAabb();
    float d = Math::VecDistanceSq(aabb.minimum, cam->GetPosition());
    d = Math::Minimum(d, Math::VecDistanceSq(aabb.minimum, cam->GetPosition()));

    bool finished = false;
    int max_level = Terrain::kMaxDetailLevel - 1;

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

    if (mTerrain->GetConfig().morphEnable)
    {
        float start = mTerrain->GetConfig().morphStart;

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
    int z = mSectionZ;

	int xSectionCount = mTerrain->GetConfig().xSectionCount;
	int zSectionCount = mTerrain->GetConfig().zSectionCount;

    if (z > 0)
    {
        TerrainSection * section = mTerrain->GetSection(x, z - 1);
        key.north = section->GetLevel();

        if (key.north < mLevel)
            key.north = mLevel;
    }

    if (z < zSectionCount - 1)
    {
        TerrainSection * section = mTerrain->GetSection(x, z + 1);
        key.south = section->GetLevel();

        if (key.south < mLevel)
            key.south = mLevel;
    }

    if (x > 0)
    {
        TerrainSection * section = (TerrainSection*)mTerrain->GetSection(x - 1, z);
        key.west = section->GetLevel();

        if (key.west < mLevel)
            key.west = mLevel;
    }

    if (x < xSectionCount - 1)
    {
        TerrainSection * section = (TerrainSection*)mTerrain->GetSection(x + 1, z);
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
    if (mTerrain->GetConfig().morphEnable)
    {
        int size = Terrain::kSectionVertexSize * Terrain::kSectionVertexSize * sizeof(float);
        for (int i = 0; i < Terrain::kMaxDetailLevel - 1; ++i)
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

    int xSectionVertSize = Terrain::kSectionVertexSize;
    int ySectionVertSize = Terrain::kSectionVertexSize;

    int xOffset = mSectionX * (xSectionVertSize - 1);
    int yOffset = mSectionZ * (ySectionVertSize - 1);

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

    for (int i = 1; i < Terrain::kMaxDetailLevel; ++i)
    {
        mErrorMetric[i] = _CalcuErrorMetric(i);
    }

    //lod大的必须误差值必须大
    for (int i = 2; i < Terrain::kMaxDetailLevel; ++i)
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

    int step = 1 << level;

    int xSectionVertSize = Terrain::kSectionVertexSize;
    int ySectionVertSize = Terrain::kSectionVertexSize;

    int xOffset = mSectionX * (xSectionVertSize - 1);
    int yOffset = mSectionZ * (ySectionVertSize - 1);

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
    float pixelerr = Terrain::kMaxPixelError;
    float resolution = Terrain::kMaxResolution;

    float c = 1.0f / (2 * pixelerr / resolution);

    for (int i = 0; i < Terrain::kMaxDetailLevel; ++i)
    {
        float e = mErrorMetric[i];
        float d = e * c;
        mLevelDistSq[i] = d * d;
    }

    if (mTerrain->GetConfig().morphEnable)
    {
        for (int i = 1; i < Terrain::kMaxDetailLevel - 1; ++i)
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

void TerrainSection::NotifyUnlockHeight()
{
	const Rect & rc = mTerrain->GetLockedHeightRect();
	const Terrain::Config & config = mTerrain->GetConfig();
	int xtile = Terrain::kSectionVertexSize - 1;
	int ztile = Terrain::kSectionVertexSize - 1;

	Rect myRect;

	myRect.x1 = mSectionX * xtile;
	myRect.y1 = mSectionZ * ztile;
	myRect.x2 = mSectionX * xtile + xtile;
	myRect.y2 = mSectionZ * ztile + ztile;

	if (rc.x1 > myRect.x2 || rc.x2 < myRect.x1 ||
		rc.y1 > myRect.y2 || rc.y2 < myRect.y1)
		return ;

	Shutdown();
	Init();
}

}