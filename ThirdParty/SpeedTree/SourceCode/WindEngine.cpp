///////////////////////////////////////////////////////////////////////  
//         Name: WindEngine.cpp
//
//  *** INTERACTIVE DATA VISUALIZATION (IDV) PROPRIETARY INFORMATION ***
//
//      Copyright (c) 2001-2004 IDV, Inc.
//      All Rights Reserved.
//
//      IDV, Inc.
//      1233 Washington St. Suite 610
//      Columbia, SC 29201
//      Voice: (803) 799-1699
//      Fax:   (803) 931-0320
//      Web:   http://www.idvinc.com
//
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Interactive Data Visualization and may not 
//  be copied or disclosed except in accordance with the terms of that 
//  agreement.

#include "Debug.h"
#include "SpeedTreeRT.h"
#include "WindEngine.h"


// static member variables
float CWindEngine::m_fTime = 0.0f;
CWindMatrices CWindEngine::m_cWindMatrices;


///////////////////////////////////////////////////////////////////////  
//  CWindEngine::CWindEngine definition

CWindEngine::CWindEngine( ) :
    m_bRockingLeaves(true),
    m_eBranchMethod(CSpeedTreeRT::WIND_NONE),
    m_eFrondMethod(CSpeedTreeRT::WIND_NONE),
    m_eLeafMethod(CSpeedTreeRT::WIND_NONE),
    m_fLeafFrequency(0.0f),
    m_fTimeFrequencyShift(0.0f),
    m_fLeafThrow(0.0f),
    m_fWindStrength(0.25f),
    m_nStartingMatrix(0),
    m_nMatrixSpan(4)
{
    m_fLeafFactors[0] = m_fLeafFactors[1] = 1.0f;
}


///////////////////////////////////////////////////////////////////////  
//  CWindEngine::SetWindStrength definition
//
//  Returns the newly computed frequency time shift

float CWindEngine::SetWindStrength(float fNewStrength, float fOldStrength, float fOldTimeShift)
{
    float fPreviousLeafFrequency = c_fStrengthToFrequencyRatio * fOldStrength * m_fLeafFactors[1];

    m_fWindStrength = fNewStrength;
    m_fLeafFrequency = c_fStrengthToFrequencyRatio * m_fWindStrength * m_fLeafFactors[1];
    m_fLeafThrow = c_fStrengthToThrowRatio * m_fWindStrength * m_fLeafFactors[0];

    m_fTimeFrequencyShift = m_fTime * fPreviousLeafFrequency + fOldTimeShift - m_fTime * m_fLeafFrequency;

    return m_fTimeFrequencyShift;
}


///////////////////////////////////////////////////////////////////////  
//  CWindEngine::Init definition

void CWindEngine::Init(const SIdvWindInfo& cWindInfo)
{
    m_fLeafFactors[0] = cWindInfo.m_cLeafFactors[0];
    m_fLeafFactors[1] = cWindInfo.m_cLeafFactors[1];
    m_fLeafFrequency = c_fStrengthToFrequencyRatio * cWindInfo.m_fStrength * m_fLeafFactors[1];
    m_fLeafThrow = c_fStrengthToThrowRatio * cWindInfo.m_fStrength * m_fLeafFactors[0];
    m_fWindStrength = cWindInfo.m_fStrength;
}


///////////////////////////////////////////////////////////////////////  
//  CWindEngine::RockLeaf definition

float CWindEngine::RockLeaf(float fTimeOffset)
{
    float fWindAngle = 0.0f;

    if (m_bRockingLeaves)
    {
        float fPercent = 0.5f * (sinf(fTimeOffset + (m_fTime * m_fLeafFrequency) + m_fTimeFrequencyShift) + 1.0f);
        fWindAngle = VecInterpolate(-m_fLeafThrow, m_fLeafThrow, fPercent);
    }
    else
    {
        float fPercent = (sinf(fTimeOffset) + 1.0f) * 0.5f; 
        fWindAngle = VecInterpolate(m_fLeafFactors[0] * -0.15f, m_fLeafFactors[0] * 0.15f, fPercent);
    }

    return fWindAngle;
}


///////////////////////////////////////////////////////////////////////  
//  CWindEngine::SetLocalMatrices definition

void CWindEngine::SetLocalMatrices(unsigned int nStartingMatrix, unsigned int nMatrixSpan)
{
    m_nStartingMatrix = nStartingMatrix;
    m_nMatrixSpan = nMatrixSpan;
}


///////////////////////////////////////////////////////////////////////  
//  CWindEngine::ResetLeafWindState definition

void CWindEngine::ResetLeafWindState( )
{
    m_fLeafFrequency = 0.0f;
    m_fTimeFrequencyShift = 0.0f;
    m_fLeafThrow = 0.0f;
    m_fWindStrength = 0.25f;
}


