#include "MWLightScattering.h"

using namespace Myway;

// Rayleigh scattering coefficient
static const Vec4 RAYLEIGH_BETA(0.000697153f,
                                0.00117891f,
                                0.00244460f,
                                1.0f); 

// Rayleigh Angular scattering 
/*
    3 / (16PI) * Rayleigh scattering coefficient
*/
static const Vec4 RAYLEIGH_ANGULAR_BETA(4.16082e-005f,
                                        7.03612e-005f,
                                        0.000145901f,
                                        1.0f);

//Mie scattering coefficient
static const Vec4 MIE_BETA(0.00574060f,
                           0.00739969f,
                           0.0105143f,
                           1.0f);

//Mie Angular scattering coefficient
/*
    3 / (16PI) * Mie scattering coefficient
*/
static const Vec4 MIE_ANGULAR_BETA(0.00133379f,
                                   0.00173466f,
                                   0.00249762f,
                                   1.0f);

// Reflective spectrum data for soil.
// As calculated by Hoffman, Mitchell 
// and Preetham
static const Vec4 SOIL_REFLECTIVITY(0.138f,
                                    0.113f,
                                    0.08f,
                                    1.0f);

LightScattering::LightScattering()
: m_HenyeyG(0.9866f),
  m_RayleighScale(0.06f),
  m_MieScale(0.001f),
  m_InScatteringScale(0.27f),
  m_ExtinctionScale(1.33f),
  m_ReflectivePower(0.1f),
  m_bNeedUpdate(TRUE)
{
}

LightScattering::~LightScattering()
{
}

void LightScattering::SetHenyeyG(float HenyeyG)
{
    m_HenyeyG = HenyeyG;
    m_bNeedUpdate = TRUE;
}

void LightScattering::SetRayleighScale(float RayleighScale)
{
    m_RayleighScale = RayleighScale;
    m_bNeedUpdate = TRUE;
}

void LightScattering::SetMieScale(float MieScale)
{
    m_MieScale = MieScale;
    m_bNeedUpdate = TRUE;
}

void LightScattering::SetInScatteringScale(float InScatteringScale)
{
    m_InScatteringScale = InScatteringScale;
    m_bNeedUpdate = TRUE;
}

void LightScattering::SetExtinctionScale(float ExtinctionScale)
{
    m_ExtinctionScale = ExtinctionScale;
    m_bNeedUpdate = TRUE;
}

void LightScattering::SetReflectivePower(float ReflectivePower)
{
    m_ReflectivePower = ReflectivePower;
    m_bNeedUpdate = TRUE;
}

float LightScattering::GetHenyeyG()
{
    return m_HenyeyG;
}

float LightScattering::GetRayleighScale()
{
    return m_RayleighScale;
}

float LightScattering::GetMieScale()
{
    return m_MieScale;
}

float LightScattering::GetInScatteringScale()
{
    return m_InScatteringScale;
}

float LightScattering::GetExtinctionScale()
{
    return m_ExtinctionScale;
}

float LightScattering::GetReflectivePower()
{
    return m_ReflectivePower;
}

const LightScatterParam & LightScattering::GetParam()
{
    if (m_bNeedUpdate)
    {
        m_Param.Beta1 = RAYLEIGH_BETA * m_RayleighScale;
        m_Param.Beta2 = MIE_BETA * m_MieScale;

        m_Param.BetaD1 = RAYLEIGH_ANGULAR_BETA * m_RayleighScale;
        m_Param.BetaD2 = MIE_ANGULAR_BETA * m_MieScale;

        m_Param.SumBeta1Beta2 = m_Param.Beta1 + m_Param.Beta2;

        m_Param.HG = Vec4(1.0f - m_HenyeyG * m_HenyeyG,
                          1.0f + m_HenyeyG,
                          2.0f * m_HenyeyG,
                          1.0f);
        m_Param.InvSumBeta1Beta2 = Vec4(1.0f / m_Param.SumBeta1Beta2.x,
                                        1.0f / m_Param.SumBeta1Beta2.y,
                                        1.0f / m_Param.SumBeta1Beta2.z,
                                        1.0f);

        m_Param.Reflective = SOIL_REFLECTIVITY * m_ReflectivePower;

        m_Param.Constants.x = m_InScatteringScale;
        m_Param.Constants.y = m_ExtinctionScale;
        m_Param.Constants.z = 1.0f / Math::Log(2.0f);
        m_Param.Constants.w = 1.0f;

        m_bNeedUpdate = FALSE;
    }

    return m_Param;
}