#include "MWTerrainLod.h"

#include "MWVideoBufferManager.h"

#define __North         (1 << 0)
#define __South         (1 << 1)
#define __East          (1 << 2)
#define __West          (1 << 3)

#define __NorthIndex    0
#define __SouthIndex    1
#define __EastIndex     2
#define __WestIndex     3


namespace Myway {

bool TerrainLod::InvalidKey(_Key key)
{
	return key.level >= Terrain::kMaxDetailLevel ||
		   key.east >= Terrain::kMaxDetailLevel ||
		   key.south >= Terrain::kMaxDetailLevel ||
		   key.west >= Terrain::kMaxDetailLevel ||
		   key.north >= Terrain::kMaxDetailLevel;
}


TerrainLod::TerrainLod(int lodLevel)
{
    mLevelDetails = lodLevel;

    memset(mBodyIndex, 0, Terrain::kMaxDetailLevel * sizeof(_IndexPool));
    memset(mConecterIndex, 0, Terrain::kMaxDetailLevel * Terrain::kMaxDetailLevel * 4 * sizeof(_IndexPool));

    GenerateBodyIndex();
    GenerateConecterIndex();
}

TerrainLod::~TerrainLod()
{
    for (int i = 0; i < Terrain::kMaxDetailLevel; ++i)
    {
        safe_free(mBodyIndex[i].indeces);

        for (int j = 0; j < Terrain::kMaxDetailLevel; ++j)
        {
            safe_free(mConecterIndex[i][j][__NorthIndex].indeces);
            safe_free(mConecterIndex[i][j][__SouthIndex].indeces);
            safe_free(mConecterIndex[i][j][__WestIndex].indeces);
            safe_free(mConecterIndex[i][j][__EastIndex].indeces);
        }
    }
}

const TerrainLod::_IndexData &  TerrainLod::GetIndexData(const _Key & k)
{
	d_assert(!InvalidKey(k));

    Map<_Key, _IndexData>::Iterator iter = mMIndexBuffer.Find(k);

    if (iter == mMIndexBuffer.End())
    {
        _IndexData data;
        data.start = 0;
        data.index_count = 0;
        data.prim_count = 0;
        data.prim_type = PRIM_TRIANGLESTRIP;

        const _IndexPool & body = mBodyIndex[k.level];
        const _IndexPool & north = mConecterIndex[k.level][k.north][__NorthIndex];
        const _IndexPool & south = mConecterIndex[k.level][k.south][__SouthIndex];
        const _IndexPool & west = mConecterIndex[k.level][k.west][__WestIndex];
        const _IndexPool & east = mConecterIndex[k.level][k.east][__EastIndex];

        if (body.indeces)
            data.index_count += body.size + 2;

        if (north.indeces)
            data.index_count += north.size + 2;

        if (south.indeces)
            data.index_count += south.size + 2;

        if (west.indeces)
            data.index_count += west.size + 2;

        if (east.indeces)
            data.index_count += east.size;

        d_assert (data.index_count > 0 && data.index_count < 65536);

        data.index_buffer = VideoBufferManager::Instance()->CreateIndexBuffer(data.index_count * sizeof(short), FMT_INDEX16);
        int index = 0;
        bool flag = false;

        short * indeces = (short *)data.index_buffer->Lock(0, 0, LOCK_NORMAL);

        if (body.indeces)
        {
            Memcpy(indeces + index, body.indeces, body.size * sizeof(short));
            index += body.size;
            flag = true;
        }

        if (north.indeces)
        {
            if (flag)
            {
                indeces[index++] = indeces[index - 1];
                indeces[index++] = north.indeces[0];
            }

            Memcpy(indeces + index, north.indeces, north.size * sizeof(short));
            index += north.size;
            flag = true;
        }

        if (south.indeces)
        {
            if (flag)
            {
                indeces[index++] = indeces[index - 1];
                indeces[index++] = south.indeces[0];
            }

            Memcpy(indeces + index, south.indeces, south.size * sizeof(short));
            index += south.size;
            flag = true;
        }

        if (west.indeces)
        {
            if (flag)
            {
                indeces[index++] = indeces[index - 1];
                indeces[index++] = west.indeces[0];
            }

            Memcpy(indeces + index, west.indeces, west.size * sizeof(short));
            index += west.size;
            flag = true;
        }

        if (east.indeces)
        {
            if (flag)
            {
                indeces[index++] = indeces[index - 1];
                indeces[index++] = east.indeces[0];
            }

            Memcpy(indeces + index, east.indeces, east.size * sizeof(short));
            index += east.size;
            flag = true;
        }

        data.index_buffer->Unlock();

        data.prim_count = index - 2;

        iter = mMIndexBuffer.Insert(k, data);
    }

    return iter->second;
}

void TerrainLod::GenerateBodyIndex()
{
    for (int i = 0; i < mLevelDetails; ++i)
    {
        _GenerateBodyIndex(i);
    }
}

void TerrainLod::_GenerateBodyIndex(int level)
{
    d_assert (mBodyIndex[level].size == 0 &&
              mBodyIndex[level].indeces == NULL);

    int xtile = (Terrain::kSectionVertexSize - 1) >> level;
    int ytile = (Terrain::kSectionVertexSize - 1) >> level;
    int step = 1 << level;

    int start = 0;

    if (level < mLevelDetails - 1)
    {
        xtile -= 2;
        ytile -= 2;
        start = step * Terrain::kSectionVertexSize + step;
    }

    if (xtile == 0 || ytile == 0)
        return;

    int count = (xtile * 2 + 2 + 2) * ytile;
    mBodyIndex[level].indeces = (short*)alloc_mem(sizeof(short) * count);

    int & index = mBodyIndex[level].size;
    short * indeces = mBodyIndex[level].indeces;

    // generate triangle strip cw
    //
    int x = 0, y = 0;
    short row_c = start;
    short row_n = row_c + Terrain::kSectionVertexSize * step;
    for (y = 0; y < ytile; ++y)
    {
        for (x = 0; x < xtile + 1; ++x)
        {
            indeces[index++] = row_n + x * step;
            indeces[index++] = row_c + x * step;  
        }

        row_c += Terrain::kSectionVertexSize * step;
        row_n += Terrain::kSectionVertexSize * step;

        // degenerate triangle
        indeces[index++] = indeces[index - 1];
        indeces[index++] = row_n;
    }

    assert (index == count);
}

void TerrainLod::GenerateConecterIndex()
{
    for (int i = 0; i < mLevelDetails; ++i)
    {
        for (int j = 0; j < mLevelDetails; ++j)
        {
            _GenerateConecterIndexNorth(i, j);
            _GenerateConecterIndexSouth(i, j);
            _GenerateConecterIndexWest(i, j);
            _GenerateConecterIndexEast(i, j);
        }
    }
}

void TerrainLod::_GenerateConecterIndexNorth(int level, int conecter)
{
    assert (mConecterIndex[level][conecter][__NorthIndex].size == 0 &&
            mConecterIndex[level][conecter][__NorthIndex].indeces == NULL);

    if (conecter < level || level == mLevelDetails - 1)
    {
        mConecterIndex[level][conecter][__NorthIndex].size = 0;
        mConecterIndex[level][conecter][__NorthIndex].indeces = NULL;
        return;
    }

    int self_step = 1 << level;
    int neighbor_step = 1 << conecter;
    int self_tile = (Terrain::kSectionVertexSize - 1) >> level;
    int neighbor_tile = (Terrain::kSectionVertexSize - 1) >> conecter;

    assert (self_tile >= neighbor_tile);

    int count = self_tile * 2 + 2;

    mConecterIndex[level][conecter][__NorthIndex].indeces = (short*)alloc_mem(sizeof(short) * count);

    int & index = mConecterIndex[level][conecter][__NorthIndex].size;
    short * indeces = mConecterIndex[level][conecter][__NorthIndex].indeces;

    // starter
    indeces[index++] = 0;
    indeces[index++] = 0;

    // middler
    for (int i = 1; i < self_tile; ++i)
    {
        int x1 = i * self_step;
        int y1 = self_step;
        int x0 = x1 / neighbor_step * neighbor_step;
        int y0 = y1 - self_step;

        int index0 = y1 * Terrain::kSectionVertexSize + x1;
        int index1 = y0 * Terrain::kSectionVertexSize + x0;

        indeces[index++] = index0;
        indeces[index++] = index1;
    }

    //ender
    indeces[index++] = Terrain::kSectionVertexSize - 1;
    indeces[index++] = Terrain::kSectionVertexSize - 1;

    assert (index == count);
}

void TerrainLod::_GenerateConecterIndexSouth(int level, int conecter)
{
    assert (mConecterIndex[level][conecter][__SouthIndex].size == 0 &&
            mConecterIndex[level][conecter][__SouthIndex].indeces == NULL);

    if (conecter < level || level == mLevelDetails - 1)
    {
        mConecterIndex[level][conecter][__SouthIndex].size = 0;
        mConecterIndex[level][conecter][__SouthIndex].indeces = NULL;
        return;
    }

    int self_step = 1 << level;
    int neighbor_step = 1 << conecter;
    int self_tile = (Terrain::kSectionVertexSize - 1) >> level;
    int neighbor_tile = (Terrain::kSectionVertexSize - 1) >> conecter;

    assert (self_tile >= neighbor_tile);

    int count = self_tile * 2 + 2;

    mConecterIndex[level][conecter][__SouthIndex].indeces = (short*)alloc_mem(sizeof(short) * count);

    int & index = mConecterIndex[level][conecter][__SouthIndex].size;
    short * indeces = mConecterIndex[level][conecter][__SouthIndex].indeces;

    //starter
    indeces[index++] = (Terrain::kSectionVertexSize - 1) * Terrain::kSectionVertexSize;
    indeces[index++] = (Terrain::kSectionVertexSize - 1) * Terrain::kSectionVertexSize;

    // middler
    for (int i = 1; i < self_tile; ++i)
    {
        int x0 = i * self_step;
        int y0 = Terrain::kSectionVertexSize - 1 - self_step;
        int x1 = x0 / neighbor_step * neighbor_step;
        int y1 = y0 + self_step;

        int index0 = y1 * Terrain::kSectionVertexSize + x1;
        int index1 = y0 * Terrain::kSectionVertexSize + x0;

        indeces[index++] = index0;
        indeces[index++] = index1;
    }

    //ender
    indeces[index++] = Terrain::kSectionVertexSize * Terrain::kSectionVertexSize - 1;
    indeces[index++] = Terrain::kSectionVertexSize * Terrain::kSectionVertexSize - 1;

    assert (index == count);
}

void TerrainLod::_GenerateConecterIndexWest(int level, int conecter)
{
    assert (mConecterIndex[level][conecter][__WestIndex].size == 0 &&
            mConecterIndex[level][conecter][__WestIndex].indeces == NULL);

    if (conecter < level || level == mLevelDetails - 1)
    {
        mConecterIndex[level][conecter][__WestIndex].size = 0;
        mConecterIndex[level][conecter][__WestIndex].indeces = NULL;
        return;
    }

    int self_step = 1 << level;
    int neighbor_step = 1 << conecter;
    int self_tile = (Terrain::kSectionVertexSize - 1) >> level;
    int neighbor_tile = (Terrain::kSectionVertexSize - 1) >> conecter;

    assert (self_tile >= neighbor_tile);

    int count = self_tile * 2 + 2;

    mConecterIndex[level][conecter][__WestIndex].indeces = (short*)alloc_mem(sizeof(short) * count);

    int & index = mConecterIndex[level][conecter][__WestIndex].size;
    short * indeces = mConecterIndex[level][conecter][__WestIndex].indeces;

    //starter
    indeces[index++] = 0;
    indeces[index++] = 0;

    // middler
    for (int i = 1; i < self_tile; ++i)
    {
        int x0 = 0;
        int y0 = i * self_step / neighbor_step * neighbor_step;
        int x1 = self_step;
        int y1 = i * self_step;

        int index0 = y0 * Terrain::kSectionVertexSize + x0;
        int index1 = y1 * Terrain::kSectionVertexSize + x1;

        indeces[index++] = index0;
        indeces[index++] = index1;
    }

    //ender
    indeces[index++] = (Terrain::kSectionVertexSize - 1) * Terrain::kSectionVertexSize;
    indeces[index++] = (Terrain::kSectionVertexSize - 1) * Terrain::kSectionVertexSize;

    assert (index == count);
}

void TerrainLod::_GenerateConecterIndexEast(int level, int conecter)
{
    assert (mConecterIndex[level][conecter][__EastIndex].size == 0 &&
            mConecterIndex[level][conecter][__EastIndex].indeces == NULL);

    if (conecter < level || level == mLevelDetails - 1)
    {
        mConecterIndex[level][conecter][__EastIndex].size = 0;
        mConecterIndex[level][conecter][__EastIndex].indeces = NULL;
        return;
    }

    int self_step = 1 << level;
    int neighbor_step = 1 << conecter;
    int self_tile = (Terrain::kSectionVertexSize - 1) >> level;
    int neighbor_tile = (Terrain::kSectionVertexSize - 1) >> conecter;

    assert (self_tile >= neighbor_tile);

    int count = self_tile * 2 + 2;

    mConecterIndex[level][conecter][__EastIndex].indeces = (short*)alloc_mem(sizeof(short) * count);

    int & index = mConecterIndex[level][conecter][__EastIndex].size;
    short * indeces = mConecterIndex[level][conecter][__EastIndex].indeces;

    //starter
    indeces[index++] = Terrain::kSectionVertexSize - 1;
    indeces[index++] = Terrain::kSectionVertexSize - 1;

    // middler
    for (int i = 1; i < self_tile; ++i)
    {
        int x0 = Terrain::kSectionVertexSize - 1 - self_step;
        int y0 = i * self_step;
        int x1 = Terrain::kSectionVertexSize - 1;
        int y1 = i * self_step / neighbor_step * neighbor_step;

        int index0 = y0 * Terrain::kSectionVertexSize + x0;
        int index1 = y1 * Terrain::kSectionVertexSize + x1;

        indeces[index++] = index0;
        indeces[index++] = index1;
    }

    //ender
    indeces[index++] = Terrain::kSectionVertexSize * Terrain::kSectionVertexSize - 1;
    indeces[index++] = Terrain::kSectionVertexSize * Terrain::kSectionVertexSize - 1;

    assert (index == count);
}

}
