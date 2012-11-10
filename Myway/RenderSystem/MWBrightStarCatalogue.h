#pragma once

namespace Myway {

namespace Env {

    struct BrightStarCatalogueEntry 
    {
        signed char rasc_hour;
        signed char rasc_min;
        float rasc_sec;
        signed char decl_deg;
        signed char decl_min;
        float decl_sec;
        float magn;
    };
    
    #define BrightStarCatalogueSize 9110

    extern const BrightStarCatalogueEntry BrightStarCatalogue[BrightStarCatalogueSize];
}

}