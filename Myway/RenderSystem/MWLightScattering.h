#pragma once

#include "MWMath.h"

namespace Myway
{

    /*
        @Remark:
            Formula:          
                L = Lc * Fex(s) + Lin(s, @)

            Extinction:
                          -(r + m) * s
                Fex(s) = e  

                s  -- distance
                r  -- Rayleigh scattering coefficient
                m  -- Mie scattering coefficient

            In-Scattering:  
                                                   Br(@) + Bm(@) 
                Lin(s, @) = Esun * (1 - Fex(s)) * ---------------
                                                       r + m

                          3                         
                Br(@) =  ---- * PI * r * (1 + cos(@) * cos(@))
                          16
                                                       
                          1                       1 - g * g
                Bm(@) =  --- * PI * m * -------------------------------
                          4                                        3 / 2
                                         (1 + g * g  - 2g * cos(@))
    */

struct LightScatterParam
{
    Vec4  Beta1;            //Rayeleigh Coefficient
    Vec4  Beta2;            //Mie Coefficient
    Vec4  BetaD1;           //3.0f / 16.0f * PI * Beta1
    Vec4  BetaD2;           //1.0f / 4.0f * PI * Beta2
    Vec4  SumBeta1Beta2;    //Sum of Beta1, Beta2
    Vec4  InvSumBeta1Beta2; //Inverse Sum of Beta1, Beta2
    Vec4  HG;               //Vec4(HG * HG, 1 - HG, 2 * HG, 1.0f)
    Vec4  Reflective;       //Reflective
    Vec4  Constants;
};

class MW_ENTRY LightScattering : public AllocObj
{
public:
    LightScattering();
    ~LightScattering();

    void SetHenyeyG(float HenyeyG);
    void SetRayleighScale(float RayleighScale);
    void SetMieScale(float MieScale);
    void SetInScatteringScale(float InScatteringScale);
    void SetExtinctionScale(float ExtinctionScale);
    void SetReflectivePower(float ReflectivePower);

    float GetHenyeyG();
    float GetRayleighScale();
    float GetMieScale();
    float GetInScatteringScale();
    float GetExtinctionScale();
    float GetReflectivePower();
    const LightScatterParam & GetParam();

protected:
    LightScatterParam       m_Param;
    float                   m_HenyeyG;
    float                   m_RayleighScale;
    float                   m_MieScale;
    float                   m_InScatteringScale;
    float                   m_ExtinctionScale;
    float                   m_ReflectivePower;
    bool                    m_bNeedUpdate;
};

}