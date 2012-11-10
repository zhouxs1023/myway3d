#pragma once

#include "MWRenderSystem.h"

namespace Myway {


    struct TnConst {
        enum 
        {
            kMaxLayers = 8,
            kSectionVertexSize = 65,
            KSectionVertexSize_2 =  kSectionVertexSize * kSectionVertexSize,
            kMaxDetailLevel = 4,

            kMaxPixelError = 4,
            kMaxResolution = 768,
        };

    };

    struct TnLayer
    {
        TString128 texture;
        float scale;

        TnLayer()
        {
            scale = 0;
        }
    };

    struct TnConfig {
        TString128 mResourceGroup;
        float width, height, hight;
        TString128 hightmap;
        TString128 weightmap0;
        TString128 weightmap1;
        TnLayer layers[TnConst::kMaxLayers];
        bool morphEnable;
        float morphStart;

        TnConfig()
        {
            width = height = hight = 0;
            morphEnable = false;
            morphStart = 0;
        }
    };

}