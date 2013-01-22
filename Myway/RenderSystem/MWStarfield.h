#pragma once

#include "MWRenderSystem.h"
#include "MWBrightStarCatalogue.h"

namespace Myway {

    class MW_ENTRY Starfield
    {
        DECLARE_ALLOC();

        struct Star {
            float RightAscension;
            float Declination;
            float Magnitude;
        };

    public:
        Starfield();
        ~Starfield();

        void Render();
        void Resize(int size);

    protected:
        void _random();
        void _geometry();
        void _add(const BrightStarCatalogueEntry & entry);

    protected:
        float mMinPixelSize, mMaxPixelSize, mMag0PixelSize;
        float mMagnitudeScale;
        float mObserverLatitude, mObserverLongitude;

        int mIterator;
        Star * mStars;
        int mSize;

        RenderOp mRender;
        Technique * mTech;
    };

}

