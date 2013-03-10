#pragma once

#include "MRenderEntry.h"
#include "MWTerrain.h"

namespace Myway {

class MRENDER_ENTRY TerrainLod
{
public:
    struct _IndexPool
    {
        int              size;
        short *          indeces;

        _IndexPool() : size(0), indeces(0) {}
    };

    struct _IndexData
    {
        int              prim_count;
        PRIMITIVE_TYPE   prim_type;
        int              start;
        int              index_count;
        IndexBufferPtr   index_buffer;

        _IndexData() : prim_count(0), prim_type(PRIM_TRIANGLESTRIP),
            start(0), index_count(0) {}
    };

    struct _Key
    {
        union
        {
            struct {
                unsigned char       level;
                unsigned char       north;
                unsigned char       south;
                unsigned char       west;
                unsigned char       east;
                unsigned char       unused[3];
            };

            unsigned __int64 value;
        };

        _Key() : value(0) {}
        ~_Key() {}

        _Key(const _Key & k) : value(k.value) {}

        _Key & operator = (const _Key & r)
        {
            value = r.value;
            return *this;
        }

        bool operator < (const _Key & r) const
        {
            return value < r.value;
        }

        bool operator <= (const _Key & r) const
        {
            return value <= r.value;
        }

        bool operator > (const _Key & r) const
        {
            return value > r.value;
        }

        bool operator >= (const _Key & r) const
        {
            return value >= r.value;
        }

        bool operator == (const _Key & r) const
        {
            return value == r.value;
        }

        bool operator != (const _Key & r) const
        {
            return value != r.value;
        }
    };


public:
    TerrainLod(int lodLevel);
    ~TerrainLod();

    const _IndexData &  GetIndexData(const _Key & k);

protected:
    void                GenerateBodyIndex();
    void                _GenerateBodyIndex(int level);
    void                GenerateConecterIndex();
    void                _GenerateConecterIndexNorth(int level, int conecter);
    void                _GenerateConecterIndexSouth(int level, int conecter);
    void                _GenerateConecterIndexWest(int level, int conecter);
    void                _GenerateConecterIndexEast(int level, int conecter);

	bool				InvalidKey(_Key k);

protected:
    _IndexPool                  mBodyIndex[Terrain::kMaxDetailLevel];
    _IndexPool                  mConecterIndex[Terrain::kMaxDetailLevel][Terrain::kMaxDetailLevel][4];
    Map<_Key, _IndexData>       mMIndexBuffer;
    int                         mLevelDetails;
};

}