#pragma once

#include "MWMath.h"
#include "MWCamera.h"
#include "MWSceneNode.h"

namespace Myway
{
    struct OctBBox
    {
        int x0, y0, z0;
        int x1, y1, z1;
    };

    struct OctDirt
    {
        int zoneId;
        int nodeId;
    };

    class OctZone
    {
        DECLARE_ALLOC();

    public:
        OctZone();
        ~OctZone();

        void Init(const OctBBox & bound);
        void ImpVisibility(Camera * cam);
        bool UpdateSceneNode(SceneNode * node);


    protected:
        void _cull(int number, int parent, int depth, const OctBBox & bbox, Camera * cam);
        void _fill(int number, int parent, int depth);
        void _split();

        void _as_flag(int index, unsigned char flag);

    protected:
        OctBBox mBBox;
        Aabb mBound;

        int mDepth;

        unsigned char * mBits;
        int mSize;
    };


}