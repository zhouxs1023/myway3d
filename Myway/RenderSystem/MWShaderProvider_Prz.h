#pragma once

#include "MWShaderFX.h"

namespace Myway
{

	class MW_ENTRY ShaderProvider
	{
		DECLARE_ALLOC();

    public:        

    public:
        ShaderProvider();
        ~ShaderProvider();

        Technique * GetTechnique(eRenderTechType::enum_t type, bool skined);

        Technique * GetFrushTech() { return mFrushTech; }
        Technique * GetClearTech() { return mClearTech; }
		Technique * GetTech_PointLight() { return mTech_PointLight; }

    protected:
        ShaderLib * mShaderLib;
		Technique * mTechs[eRenderTechType::RTT_Max];
        Technique * mTechs_Skined[eRenderTechType::RTT_Max];

        Technique * mFrushTech;
        Technique * mClearTech;
		Technique * mTech_PointLight;
    };

}
