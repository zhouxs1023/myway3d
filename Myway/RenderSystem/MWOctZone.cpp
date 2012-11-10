#include "MWOctZone.h"

namespace Myway
{
    inline int _oct_index(int number, int parent, int depth)
    {
        const int START_ID[] = {
            0,
            0 + 8,
            8 + 8 * 8,
            8 + 8 * 8 + 8 * 8 * 8,
            8 + 8 * 8 + 8 * 8 * 8 + 8 * 8 * 8 * 8 };

        int index = START_ID[depth];

        return index + parent * 8 + number;
    }

    inline int _oct_row(int number, int parent)
    {
        return parent * 8 + number;
    }

    inline OctBBox _oct_bbox(int number, int depth, const OctBBox & bbox)
    {
        OctBBox rbox;

        struct _oct_b
        {
            int x, y, z;
        };

        const int d_size[] = {
            256,
            128,
            64,
            32,
            16
        };

        const int s = d_size[depth];

        const _oct_b d_v[] = {
            { 0, 0, 0 }, { s, 0, 0 },
            { 0, s, 0 }, { s, s, 0 },
            { 0, 0, s }, { s, 0, s },
            { 0, s, s }, { s, s, s },
        };

        rbox.x0 = bbox.x0 + d_v[number].x;
        rbox.y0 = bbox.y0 + d_v[number].y;
        rbox.z0 = bbox.z0 + d_v[number].z;

        rbox.x1 = rbox.x0 + s;
        rbox.y1 = rbox.y0 + s;
        rbox.z1 = rbox.z0 + s;

        return rbox;
    }

    inline Aabb _oct_aabb(const OctBBox & bbox)
    {
        Aabb abb;

        abb.mi.x = (float)bbox.x0;
        abb.mi.y = (float)bbox.y0;
        abb.mi.z = (float)bbox.z0;

        abb.ma.x = (float)bbox.x1;
        abb.ma.y = (float)bbox.y1;
        abb.ma.z = (float)bbox.z1;

        return abb;
    }


    OctZone::OctZone()
    {
    }

    OctZone::~OctZone()
    {
        free_m(mBits);
    }

    void OctZone::Init(const OctBBox & bound)
    {
        mBBox = bound;
        mBound = _oct_aabb(mBBox);

        mDepth = 5;

        _split();
    }

    void OctZone::ImpVisibility(Camera * cam)
    {
        Memset8(mBits, 0, mSize);

        const char fullV = Camera::VB_FULL;
        const char FULL_V = (fullV << 6) | (fullV << 4) | (fullV << 2) | fullV;

        Camera::Visibility vb = cam->GetVisibility(mBound);

        if (vb == Camera::VB_FULL)
            Memset8(mBits, FULL_V, mSize);
        else
        {
            for (int i = 0; i < 8; ++i)
            {
                _cull(i, 0, 0, mBBox, cam);
            }
        }
    }

    bool OctZone::UpdateSceneNode(SceneNode * node)
    {
        if (mBound.Intersection(node->GetWorldAabb()) == -1)
            return false;


        return true;
    }

    void OctZone::_cull(int number, int parent, int depth, const OctBBox & bbox, Camera * cam)
    {
        if (depth >= mDepth)
            return ;

        OctBBox cbox = _oct_bbox(number, depth, bbox);
        Aabb abb = _oct_aabb(cbox);

        Camera::Visibility vb = cam->GetVisibility(_oct_aabb(mBBox));

        switch (vb)
        {
        case Camera::VB_NONE:
            break;

        case Camera::VB_FULL:
            _fill(number, parent, depth);
            break;

        case Camera::VB_PARTIAL:
            {
                int index = _oct_index(number, parent, depth);
                int row = _oct_row(number, parent);

                _as_flag(index, Camera::VB_PARTIAL);

                for (int i = 0; i < 8; ++i)
                {
                    _cull(i, row, ++depth, cbox, cam);
                }
            }
            break;
        }
    }

    void OctZone::_fill(int number, int parent, int depth)
    {
        if (depth >= mDepth)
            return ;

        int index = _oct_index(number, parent, depth);
        int row = _oct_row(number, parent);

        _as_flag(index, Camera::VB_FULL);

        for (int i = 0; i < 8; ++i)
        {
            _fill(i, row, ++depth);
        }
    }

    void OctZone::_as_flag(int index, unsigned char flag)
    {
        int bytes = index / 4;
        int bit = index - bytes * 4;

        flag = flag << (bit * 2);
        mBits[bytes] |= flag;
    }
    
    void OctZone::_split()
    {
        int num = 1;

        const int _8_n[] = {
            8,
            8 * 8,
            8 * 8 * 8,
            8 * 8 * 8 * 8,
            8 * 8 * 8 * 8 * 8
        };

        for (int i = 0; i < mDepth; ++i)
        {
            num += _8_n[i];
        }

        int bits = num * 2;
        int bytes = bits / 8 + 1;

        mBits = (unsigned char *)alloc_m(bytes);
        Memset8(mBits, 0, bytes);
        mSize = bytes;
    }


}