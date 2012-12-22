///////////////////////////////////////////////////////////////////////  
//  Overlay Shaders
//
//  *** INTERACTIVE DATA VISUALIZATION (IDV) CONFIDENTIAL AND PROPRIETARY INFORMATION ***
//
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Interactive Data Visualization, Inc. and
//  may not be copied, disclosed, or exploited except in accordance with 
//  the terms of that agreement.
//
//      Copyright (c) 2003-2008 IDV, Inc.
//      All rights reserved in all media.
//
//      IDV, Inc.
//      Web: http://www.idvinc.com


///////////////////////////////////////////////////////////////////////  
//  Preprocessor

#include "CompileCommands.fx"


///////////////////////////////////////////////////////////////////////  
//  Overlay-specific global variables

float4x4    g_mModelViewProj;   // composite modelview/projection matrix
float4      g_vOverlayColor;    // used as current color when rendering the map


///////////////////////////////////////////////////////////////////////  
//  Overlay VS Ouput, PS Input

struct SOverlayOutput
{
    float4 vPosition    : POSITION;
};


///////////////////////////////////////////////////////////////////////  
//  OverlayVS
//
//  Overlay vertex shader

SOverlayOutput OverlayVS(float4 vPosition : POSITION)
{
    // this will be fed to the overlay pixel shader
    SOverlayOutput sOutput;
    
    // project to screen
    sOutput.vPosition = mul(g_mModelViewProj, float4(vPosition.xyz, 1.0f));

    return sOutput;
}


///////////////////////////////////////////////////////////////////////  
//  OverlayPS

float4 OverlayPS(SOverlayOutput In) : COLOR
{
    return g_vOverlayColor;
}


///////////////////////////////////////////////////////////////////////  
//  Overlay Technique

technique Overlay
{
    pass P0
    {          
        VS_COMPILE_COMMAND OverlayVS( );
        PS_COMPILE_COMMAND OverlayPS( );
    }
}
